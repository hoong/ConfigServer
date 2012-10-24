#ifndef CONFSVC_HANDLER_H_
#define CONFSVC_HANDLER_H_

#include "../service_status.h"
#include "rpc_handler.h"
namespace config_server
{
class ConfigServiceHandler:public service_engine::RpcHandler
{
public:
	ConfigServiceHandler(boost::shared_ptr<RpcStubsDispatcher> stub_dispatcher, int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl);
	virtual ~ConfigServiceHandler();

	//发送配置变更通知
	int Notify(std::string path,std::string sub_config);

	Status& status()
	{
		return status;
	};


private:
	Status status;
};

class SE_API ConfigServiceHandlerCreatorStrategy: public service_engine::RpcHandlerCreatorStrategy
{
public:
	ConfigServiceHandlerCreatorStrategy(RpcStubsManager* stubman);
	virtual ~ConfigServiceHandlerCreatorStrategy();

	virtual boost::shared_ptr<base::net::RefHandler> create(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl);

};


}

#endif
