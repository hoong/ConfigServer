#ifndef CONFIG_SERVER_SERVICE_CONFIG_H_
#define CONFIG_SERVER_SERVICE_CONFIG_H_
#include "atomic_operate.h"
#include "base/singleton.h"
#include "base/net/handler.h"
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <string.h>
namespace CONFIG_SERVER
{

#define DEADTIME 180

struct STATUS
{
	std::string addr;
	std::string name;
	std::string type;
	std::string network;
	uint32_t status;//0 服务正常 1 服务异常
	uint32_t	load;
	uint32_t	last_activity;
	void operator>>(std::string info);
};



class service_config_update
{
public:
	service_config_update(const std::string& svc_str,
			const std::string& path_str,
			const std::string& new_cfg_str,
			const std::string& old_cfg_str): 
		svc(svc_str), path(path_str), new_cfg(new_cfg_str),old_cfg(old_cfg_str) {};
	~service_config_update(){};

	int run();

private:
	std::string svc;
	std::string path;
	std::string new_cfg;
	std::string old_cfg;
};

typedef base::Singleton<ATOMIC_OPERATE<std::string,service_config_update> > CFG_UPDATE;


class service_instances
{
public:
	typedef std::map<std::string,boost::shared_ptr<base::net::handler> > POOL_TYPE;
	typedef POOL_TYPE::iterator IT_TYPE;
	typedef std::pair<std::string,boost::shared_ptr<base::net::handler> > VALUE_TYPE;
	typedef std::pair<VALUE_TYPE,bool> RET_TYPE;

	int insert(const std::string& addr,boost::shared_ptr<base::net::handler> ptr);
	int remove(const std::string& addr);
	int get_service(std::string& addr);
	int get_all(std::vector<std::string>& status_list);
	int notify(const std::string& path,const std::string& cfg);
private:
	POOL_TYPE m_handler_pool;
	boost::mutex m_mutex;
};

struct iterate_f
{
	virtual doit(service_instance&) =0;
};

typedef int (ITERATE_CB)(service_instance&,void*);
class service_mgr
{
public:
	service_instances& inst(const std::string& service);
	void iterate(iterate_f*);
private:
	std::map<std::string,service_instances> data;
	boost::mutex m_mutex;
};

typedef base::Singleton<service_mgr> SVCMGR; 

};

#endif
