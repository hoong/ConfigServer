#ifndef CONFIG_SERVER_SERVICE_CONFIG_H_
#define CONFIG_SERVER_SERVICE_CONFIG_H_
#include "atomic_operate.h"
#include "base/singleton.h"
#include "base/net/handler.h"
#include "rpc/config_server_handler.h"
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <string.h>

namespace config_server
{

#define DEADTIME 180


class ServiceConfigUpdate
{
public:
	ServiceConfigUpdate(const std::string& service_string,
			const std::string& path_str,
			const std::string& new_config_str,
			const std::string& old_config_str): 
		service_type(service_string), path(path_str), new_config(new_config_str),old_config(old_config_str) {};
	~ServiceConfigUpdate(){};

	int run();

private:
	std::string service_type;
	std::string path;
	std::string new_config;
	std::string old_config;
};

typedef base::Singleton<AtomicOperate<std::string,ServiceConfigUpdate> > ConfigUpdateOperate;


class ServiceInstances
{
public:
	typedef std::map<std::string,boost::shared_ptr<ConfigServiceHandler> > PoolType;
	typedef PoolType::iterator IteratorType;
	typedef std::pair<std::string,boost::shared_ptr<ConfigServiceHandler> > ValueType;
	typedef std::pair<ValueType,bool> ReturnType;

	int Insert(const std::string& addr,boost::shared_ptr<ConfigServiceHandler> ptr);
	int Remove(const std::string& addr);
	int GetService(std::string& addr);
	int GetAll(std::vector<Status*>& status_list);
	int Notify(const std::string& path,const std::string& cfg);
private:
	PoolType m_handler_pool;
	boost::mutex m_mutex;
};

struct IterateF
{
	virtual ~IterateF(){};
	virtual void doit(ServiceInstance&) =0;
};

class ServiceManager
{
public:
	ServiceInstances& Inst(const std::string& service);
	void Iterate(IterateF*);
private:
	std::map<std::string,ServiceInstances> data;
	boost::mutex m_mutex;
};

typedef base::Singleton<ServiceManager> SVCMGR; 

};

#endif
