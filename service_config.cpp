#include "service_config.h"
#include "db_mysql.h"
#include "json_man.h"

#define DBMGR MYSQLMGR

namespace CONFIG_SERVER {

int service_config_update::run()
{
	std::string cfg ; 
	DBMGR.get_svc_cfg(svc,cfg);

	json_man jm;
	jm<<cfg;
	if (!jm.is_avail())
	{
		return 2;
	}

	int ret;
	if (0 !=(ret =jm.update(path,new_cfg,old_cfg)))
	{
		LOG(error)<<"update failed"<<ENDL;
		if (-2 == ret)
			return 1;
		else
			return 2;
	}

	jm >>cfg;
	DBMGR.set_svc_cfg(svc.cfg);

	return 0;
};




service_instances& service_mgr::inst(const std::string& service)
{
	std::map<std::string,service_instances>::iterator it = data.find(service);
	if(it == data.end())
	{
		boost::scoped_lock lock(m_mutex);
		service_instances si;
		std::pair<std::map<std::string,service_instances>::iterator,bool> ret =
			data.insert(std::pair<std::string,service_instances>(service,si));
		return ret.first->second;
	}
	return it->second;

};

void service_mgr::iterate(iterate_f*)
{
	std::map<std::string,service_instances>::iterator it;
	for(it=data.first();it!=data.end();++it)
		iterate_f->doit(*it);
};

int service_instances::insert(const std::string& addr,boost::shared_ptr<base::net::handler> ptr)
{
	boost::scoped_lock lock(m_mutex);
	RET_TYPE ret = m_handler_pool.insert(VALUE_TYPE (addr,ptr));
	if (ret.second == false)
	{
		m_handler_pool.erase(ret.first);
	       	m_handler_pool.insert( VALUE_TYPE(addr,ptr));
	}

	return 0;
};

int service_instances::remove(const std::string& addr)
{
	boost::scoped_lock lock(m_mutex);
	m_handler_pool.erase(addr);

	return 0;
};

int service_instances::get_addr(std::string& addr)
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

int service_instances::get_all(std::vector<std::string>& status_list)
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


int service_instances::notify(const std::string& path,const std::string& cfg)
{
	IT_TYPE it ;
	for(it = m_handler_pool.begin();it!=m_handler_pool.end();++it)
	{
		if (status.last_activity - now >DEADTIME)
		{
			it->second->notify(path,cfg);
		};
	};
};

};

