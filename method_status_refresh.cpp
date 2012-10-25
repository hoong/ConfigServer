#include "method_status_refresh.h"
#include "time.h"

namespace config_server
{

MethodStatusRefresh::~MethodStatusRefresh()
{

}

void MethodStatusRefresh::call()
{
	Status& s = handler()->status();
	if (req_.status().has_load())
		s.set_load(req_.status().load());
	s.set_last_activity ( (uint32_t)time(NULL));
	resp_.set_code(200);
	finish();

}
}

