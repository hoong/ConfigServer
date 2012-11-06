#include "service_config.h"
#include "db_mysql.h"
#include "json_man.h"

#define DBMGR MySqlMgr

namespace config_server {

int ServiceConfigUpdate::run()
{
	std::string cfg ; 
	DBMGR::instance().getServiceConfig(service_type_,cfg);

	JsonMan jm;
	jm<<cfg;
	if (!jm.isAvail())
	{
		return 2;
	}

	int ret;
	if (0 !=(ret =jm.update(path_,new_config_,old_config_)))
	{
		LOG(error)<<"update failed"<<ENDL;
		if (-2 == ret)
			return 1;
		else
			return 2;
	}

	jm >>cfg;
	DBMGR::instance().setServiceConfig(service_type_,cfg);

	return 0;
}




ServiceInstances& ServiceManager::inst(const std::string& service)
{
	std::map<std::string,boost::shared_ptr<ServiceInstances> >::iterator it = data.find(service);
	if (it == data.end())
	{
		boost::mutex::scoped_lock lock(mutex_);
		boost::shared_ptr<ServiceInstances> si(new ServiceInstances);
		std::pair<std::map<std::string,boost::shared_ptr<ServiceInstances> >::iterator,bool> ret =
			data.insert(std::pair<std::string,boost::shared_ptr<ServiceInstances> >(service,si));
		return *(ret.first->second);
	}
	return *(it->second);

}

void ServiceManager::iterate(IterateF* iterate_function)
{
	std::map<std::string,boost::shared_ptr<ServiceInstances> >::iterator it;
	for(it=data.begin();it!=data.end();++it)
		iterate_function->doit(*(it->second));
}

int ServiceInstances::insert(const std::string& addr,boost::shared_ptr<ConfigServiceHandler> ptr)
{
	boost::mutex::scoped_lock lock(mutex_);
	ReturnType ret = handler_pool_.insert(ValueType (addr,ptr));
	if (ret.second == false)
	{
		handler_pool_.erase(ret.first);
		handler_pool_.insert( ValueType(addr,ptr));
	}

	return 0;
}

int ServiceInstances::remove(const std::string& addr)
{
	boost::mutex::scoped_lock lock(mutex_);
	handler_pool_.erase(addr);

	return 0;
}

int ServiceInstances::getAddr(std::string& addr)
{
	IteratorType it ;
	std::string server_addr="";
	uint32_t lowest =0xFFFFFFFFUL;

	uint32_t now = time(NULL);
	for (it = handler_pool_.begin(); it!=handler_pool_.end(); ++it)
	{
		Status& status = it->second->status();

		if (status.last_activity() - now >DEADTIME)
		{
			status.set_status(1);
			continue;
		}

		if (status.load() < lowest)
		{
			lowest = status.load();
			server_addr = it->first;
		}
	}

	if (lowest == 0xFFFFFFFFUL)
		return -1;
	else
		return 0;

}

int ServiceInstances::getAll(std::vector<Status*>& status_list)
{
	IteratorType it ;
	for (it = handler_pool_.begin(); it!=handler_pool_.end(); ++it)
	{
		//std::string info;
		//Status& status = it->second->status();
		status_list.push_back(&(it->second->status()));
	}

	return 0;
}


int ServiceInstances::notify(const std::string& path,const std::string& cfg)
{
	IteratorType it ;
	time_t now = time(NULL);
	for (it = handler_pool_.begin(); it!=handler_pool_.end(); ++it)
	{
		if (it->second->status().last_activity() - now >DEADTIME)
		{
			it->second->notify(path,cfg);
		}
	}
}

}

