#include "service_config.h"
#include "db_mysql.h"
#include "JsonMan.h"

#define DBMGR MySqlMgr

namespace config_server {

int ServiceConfigUpdate::run()
{
	std::string cfg ; 
	DBMGR.GetServiceConfig(service_type,cfg);

	JsonMan jm;
	jm<<cfg;
	if (!jm.isAvail())
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
	DBMGR.setServiceConfig(service_type,cfg);

	return 0;
};




ServiceInstances& ServiceManager::inst(const std::string& service)
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

void ServiceManager::iterate(IterateF* iterate_function)
{
	std::map<std::string,ServiceInstances>::iterator it;
	for(it=data.first();it!=data.end();++it)
		iterate_function->doit(*it);
};

int ServiceInstances::insert(const std::string& addr,boost::shared_ptr<ConfigServiceHandler> ptr)
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

int ServiceInstances::remove(const std::string& addr)
{
	boost::scoped_lock lock(m_mutex);
	m_handler_pool.erase(addr);

	return 0;
};

int ServiceInstances::getAddr(std::string& addr)
{
	IT_TYPE it ;
	std::string server_addr="";
	uint32_t lowest =0xFFFFFFFFUL;

	uint32_t now = time(NULL);
	for(it = m_handler_pool.begin();it!=m_handler_pool.end();++it)
	{
		Status& status = it->second->status();

		if (status.last_activity() - now >DEADTIME)
		{
			status.status = 1;
			continue;
		};

		if (status.load() < lowest)
		{
			lowest = status.load();
			server_addr = it->first;
		}
	};

	if (lowest == 0xFFFFFFFFUL)
		return -1;
	else
		return 0;

};

int ServiceInstances::getAll(std::vector<Status*>& status_list)
{
	IT_TYPE it ;
	for(it = m_handler_pool.begin();it!=m_handler_pool.end();++it)
	{
		//std::string info;
		//Status& status = it->second->status();
		status_list.push_back(&(it->second->status()));
	};

	return 0;
};


int ServiceInstances::notify(const std::string& path,const std::string& cfg)
{
	IT_TYPE it ;
	for(it = m_handler_pool.begin();it!=m_handler_pool.end();++it)
	{
		if (status.last_activity() - now >DEADTIME)
		{
			it->second->notify(path,cfg);
		};
	};
};

};

