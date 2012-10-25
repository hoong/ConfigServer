#include "method_config_get.h"
#include "db_mysql.h"
#include "json_man.h"

namespace config_server
{

MethodConfigGet::~MethodConfigGet()
{

}

void MethodConfigGet::call()
{
	std::string cfg,subcfg;
	if (MySqlMgr::instance().GetServiceConfig(req_.service_type(),cfg) != 0)
	{
		finish(400,"unavailable service");
		return;
	}

	JsonMan jm;
	jm<<cfg;
	if (!jm.IsAvail())
	{
		finish(500,"bad config");
		return;
	};

	if (jm.GetSub(req_.path(),subcfg) != 0)
	{
		finish(401,"unavailable path");
		return;
	};

	resp_.set_config(subcfg);
	finish();

}

}

