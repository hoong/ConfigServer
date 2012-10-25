#include "method_config_update.h"
#include "json_man.h"
#include "logger.h"
#include "service_config.h"
#include "atomic_operate.h"

namespace config_server
{

MethodConfigUpdate::~MethodConfigUpdate()
{

}

void MethodConfigUpdate::call()
{
	int ret = ConfigUpdateOperate::instance().Operate(req_.service_type(),
			ServiceConfigUpdate(req_.service_type(),
				req_.path(),
				req_.new_config(),
				req_.old_config())
			);

	if (ret != 0)
	{
		finish(400,"update failed");
		return;
	}
	
	//配置变更通知
	ServiceInstances si = SVCMGR::instance().inst(req_.service_type());
	si.Notify(req_.path(),req_.new_config());

	//返回
	resp_.set_code(200);
	finish();

}

}


