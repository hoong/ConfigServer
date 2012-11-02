#include "method_config_get.h"
#include "db_mysql.h"
#include "json_man.h"

namespace config_server
{

MethodConfigGet::~MethodConfigGet()
{

}

void MethodConfigGet::onCall()
{
	std::string cfg,subcfg;
	if (MySqlMgr::instance().getServiceConfig(req_.service_type(),cfg) != 0)
	{
		finish(400,"unavailable service");
		return;
	}

	JsonMan jm;
	jm<<cfg;
	if (!jm.isAvail())
	{
		finish(500,"bad config");
		return;
	};

	if (jm.getSub(req_.path(),subcfg) != 0)
	{
		finish(401,"unavailable path");
		return;
	};

	resp_.set_config(subcfg);
	finish();

}

}

