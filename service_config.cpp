#include "service_config.h"
#include "db_mysql.h"
#include "json_man.h"

#define DBMGR MySqlMgr

namespace config_server {

int ServiceConfigUpdate::run()
{
	std::string cfg ; 
	DBMGR.GetServiceConfig(service_type,cfg);

	json_man jm;
	jm<<cfg;
	if (!jm.is_avail())
	{
		return 2;
	}

	int ret;
	if (0 !=(ret =jm.update(path,new_config,old_config)))
	{
		LOG(error)<<"update failed"<<ENDL;
		if (-2 == ret)
			return 1;
		else
			return 2;
	}

	jm >>cfg;
	DBMGR.SetServiceConfig(service_type,cfg);

	return 0;
};




ServiceInstances& ServiceManager::Inst(const std::string& service)
{
	std::map<std::string,ServiceInstances>::iterator it = data.find(service);
	if(it == data.end())
	{
		boost::scoped_lock lock(m_mutex);
		ServiceInstances si;
		std::pair<std::map<std::string,ServiceInstances>::iterator,bool> ret =
			data.insert(std::pair<std::string,ServiceInstances>(service,si));
		return ret.first->second;
	}
	return it->second;

};

void ServiceManager::Iterate(IterateF* iterate_function)
{
	std::map<std::string,ServiceInstances>::iterator it;
	for(it=data.first();it!=data.end();++it)
		iterate_function->doit(*it);
};

int ServiceInstances::Insert(const std::string& addr,boost::shared_ptr<base::net::handler> ptr)
{
	boost::scoped_lock lock(m_mutex);
	ReturnType ret = m_handler_pool.insert(VALUE_TYPE (addr,ptr));
	if (ret.second == false)
	{
		m_handler_pool.erase(ret.first);
	       	m_handler_pool.insert( VALUE_TYPE(addr,ptr));
	}

	return 0;
};

int ServiceInstances::Remove(const std::string& addr)
{
	boost::scoped_lock lock(m_mutex);
	m_handler_pool.erase(addr);

	return 0;
};

int ServiceInstances::GetAddr(std::string& addr)
{
	IT_TYPE it ;
	std::string server_addr="";
	uint32_t lowest =0xFFFFFFFFUL;

	uint32_t now = time(NULL);
	for(it = m_handler_pool.begin();it!=m_handler_pool.end();++it)
	{
		STATUS& status = it->second->status();

		if (status.last_activity - now >DEADTIME)
		{
			status.status = 1;
			continue;
		};

		if (status.load < lowest)
		{
			lowest = status.load;
			server_addr = it->first;
		}
	};

	if (lowest == 0xFFFFU)
		return -1;
	else
		return 0;

};

int ServiceInstances::GetAll(std::vector<std::string>& status_list)
{
	IT_TYPE it ;
	for(it = m_handler_pool.begin();it!=m_handler_pool.end();++it)
	{
		std::string info;
		STATUS& status = it->second->status();
		status>>info;
		status_list.push_back(info);
	};

	return 0;
};


int ServiceInstances::Notify(const std::string& path,const std::string& cfg)
{
	IT_TYPE it ;
	for(it = m_handler_pool.begin();it!=m_handler_pool.end();++it)
	{
		if (status.last_activity - now >DEADTIME)
		{
			it->second->Notify(path,cfg);
		};
	};
};

};

