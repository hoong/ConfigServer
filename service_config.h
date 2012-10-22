#ifndef CONFIG_SERVER_SERVICE_H_
#define CONFIG_SERVER_SERVICE_H_
#include <string.h>
#include "atomic_operate.h"
#include "base/singleton.h"
#include "base/net/handler.h"

namespace CONFIG_SERVER
{

struct status
{
	std::string addr;
	std::string name;
	std::string type;
	std::string network;
	uint8_t status;//0 服务正常 1 服务异常
	uint16_t	load;
	uint32_t	last_activity;
	void* data; //指向对应handler的指针
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


struct service_instance
{


	boost::shared_ptr<base::net::handler> data;
};

typedef std::map<std::string, boost::shared_ptr<base::net::handler> > STATUS_SET;

};

#endif
