#include "confsvc_handler.h"




ConfigServiceHandler::ConfigServiceHandler(boost::shared_ptr<RpcStubsDispatcher> stub_dispatcher,
	       	int fd,
	       	const base::net::SockAddr& addr,
	       	boost::shared_ptr<base::net::ReactorImpl> reactor_impl):
	service_engine::RpcHandler(stub_dispatcher,fd,addr,reactor_impl)
{
}

ConfigServiceHandlerCreatorStrategy::ConfigServiceHandlerCreatorStrategy(RpcStubsManager* stubman):
	RpcHandlerCreatorStrategy(stubman)
{
}



boost::shared_ptr<base::net::RefHandler> ConfigServiceHandlerCreatorStrategy::create(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl)
{
	return boost::shared_ptr<base::net::RefHandler>(new ConfigServiceHandler(stub_dispatcher_, fd, addr, reactor_impl));
}

