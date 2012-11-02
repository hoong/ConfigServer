#include "method_status_register.h"
#include "time.h"
#include "service_config.h"

namespace config_server
{

MethodStatusRegister::~MethodStatusRegister()
{

}

void MethodStatusRegister::onCall()
{
	Status& s = handler()->status();
	s = req_.status();

	s.set_status(0);
	s.set_last_activity((uint32_t)time(NULL));

	ServiceInstances& si = SVCMGR::instance().inst(s.service_type());
	si.insert(s.server_addr(),handler());
	resp_.set_code(200);
	finish();
}

}

