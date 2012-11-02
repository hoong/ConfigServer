#include "method_server_acquire.h"
#include "service_config.h"

namespace config_server
{

MethodServerAcquire::~MethodServerAcquire()
{

}

void MethodServerAcquire::onCall()
{
	ServiceInstances& si = SVCMGR::instance().inst(req_.service_type());
	std::string addr;
	if (si.getAddr(addr) == 0)
	{
		resp_.set_server_addr(addr);
		finish();
	}
	else
	{
		finish(400,"None Available Service Acquired");
	}

}
}


