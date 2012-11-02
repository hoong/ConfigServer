#ifndef CONFSVC_HANDLER_H_
#define CONFSVC_HANDLER_H_

#include "rpc_handler.h"
#include "proto/config_server.pb.h"

using namespace service_engine;

namespace config_server
{
class ConfigServiceHandler:public RpcHandler
{
public:
	ConfigServiceHandler(boost::shared_ptr<RpcStubsDispatcher> stub_dispatcher,
	int fd, 
	const base::net::SockAddr& addr, 
	boost::shared_ptr<base::net::ReactorImpl> reactor_impl);
	virtual ~ConfigServiceHandler(){};

	//发送配置变更通知
	int notify(std::string path,std::string sub_config);

	Status& status()
	{
		return status_;
	};


private:
	Status status_;
};

class SE_API ConfigServiceHandlerCreatorStrategy: public base::net::HandlerCreatorStrategyBase
{
public:
	ConfigServiceHandlerCreatorStrategy(RpcStubsManager* stubman);
	virtual ~ConfigServiceHandlerCreatorStrategy(){};

	virtual boost::shared_ptr<base::net::RefHandler> create(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl);

private:
	boost::shared_ptr<RpcStubsDispatcher> stub_dispatcher_;
};


};

#endif
