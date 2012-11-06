#ifndef CONFIG_SERVER_SERVICE_CONFIG_H_
#define CONFIG_SERVER_SERVICE_CONFIG_H_
#include "atomic_operate.h"
#include "base/singleton.h"
#include "base/net/handler.h"
#include "rpc/config_service_handler.h"
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <string.h>

namespace config_server
{

#define DEADTIME 180


class ServiceConfigUpdate:public OperateTask
{
public:
	ServiceConfigUpdate(const std::string& service_type,
			const std::string& path,
			const std::string& new_config,
			const std::string& old_config): 
		service_type_(service_type), path_(path), new_config_(new_config),old_config_(old_config) {}
	virtual ~ServiceConfigUpdate(){}

	virtual int run() ;

private:
	std::string service_type_;
	std::string path_;
	std::string new_config_;
	std::string old_config_;
};

//typedef base::Singleton<AtomicOperate<std::string,ServiceConfigUpdate> > ConfigUpdateOperate;
typedef base::Singleton<AtomicOperate> ConfigUpdateOperate;


class ServiceInstances
{
public:
	typedef std::map<std::string,boost::shared_ptr<ConfigServiceHandler> > PoolType;
	typedef PoolType::iterator IteratorType;
	typedef std::pair<std::string,boost::shared_ptr<ConfigServiceHandler> > ValueType;
	typedef std::pair<IteratorType,bool> ReturnType;

	int insert(const std::string& addr,boost::shared_ptr<ConfigServiceHandler> ptr);
	int remove(const std::string& addr);
	int getAddr(std::string& addr);
	int getAll(std::vector<Status*>& status_list);
	int notify(const std::string& path,const std::string& cfg);
private:
	PoolType handler_pool_;
	boost::mutex mutex_;
};

struct IterateF
{
	virtual ~IterateF(){}
	virtual void doit(ServiceInstances&) =0;
};

class ServiceManager
{
public:
	ServiceInstances& inst(const std::string& service);
	void iterate(IterateF*);
private:
	std::map<std::string,boost::shared_ptr<ServiceInstances> >data;
	boost::mutex mutex_;
};

typedef base::Singleton<ServiceManager> SVCMGR; 

}

#endif
