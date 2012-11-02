#include "config_service_handler.h"

#include <string.h>
#include <boost/shared_ptr.hpp>


namespace config_server{

ConfigServiceHandler::ConfigServiceHandler(boost::shared_ptr<RpcStubsDispatcher> stub_dispatcher,
	       	int fd,
	       	const base::net::SockAddr& addr,
	       	boost::shared_ptr<base::net::ReactorImpl> reactor_impl):
	RpcHandler(stub_dispatcher,fd,addr,reactor_impl)
{
}

int ConfigServiceHandler::notify(std::string path,std::string sub_config)
{

}

ConfigServiceHandlerCreatorStrategy::ConfigServiceHandlerCreatorStrategy(RpcStubsManager* stubman)
{
	if (stubman)
		stub_dispatcher_ = stubman->dispatcher();
}



boost::shared_ptr<base::net::RefHandler> ConfigServiceHandlerCreatorStrategy::create(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl)
{
	return boost::shared_ptr<base::net::RefHandler>(new ConfigServiceHandler(stub_dispatcher_, fd, addr, reactor_impl));
}

};
