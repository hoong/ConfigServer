#ifndef SERVICE_STATUS_H_
#define SERVICE_STATUS_H_

namespace config_server
{

struct Status
{
	std::string addr;
	std::string type;
	std::string network;
	std::string name;
	uint32_t status;//0 服务正常 1 服务异常
	uint32_t	load;
	uint32_t	last_activity;
	void operator>>(std::string info);
};


};
#endif
