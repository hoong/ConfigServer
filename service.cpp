#include "service.h"
#include "db_mysql.h"
#include "json_man.h"

#define DBMGR MYSQLMGR

namespace CONFIG_SERVER {

int service_config_update::run()
{
	std::string cfg ; 
	DBMGR.get_svc_cfg(svc,cfg);

	json_man jm;
	jm<<cfg;
	if (!jm.is_avail())
	{
		return 2;
	}

	int ret;
	if (0 !=(ret =jm.update(path,new_cfg,old_cfg)))
	{
		LOG(error)<<"update failed"<<ENDL;
		if (-2 == ret)
			return 1;
		else
			return 2;
	}

	jm >>cfg;
	DBMGR.set_svc_cfg(svc.cfg);

	return 0;
};



};

