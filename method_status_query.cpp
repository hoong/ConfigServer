#include "method_status_query.h"

namespace config_server
{

StatusQueryTask::StatusQueryTask()
{
	status_list.clear();
}

StatusQueryTask::~StatusQueryTask()
{
}

void StatusQueryTask::doit(ServiceInstances& si)
{
	si.getAll(status_list);
}

MethodStatusQuery::~MethodStatusQuery()
{

}

void MethodStatusQuery::onCall()
{
	StatusQueryTask sqt;
	SVCMGR::instance().iterate(&sqt);
	for (std::vector<Status*>::iterator it = sqt.status_list.begin();
			it != sqt.status_list.end(); ++it)
	{
		Status* s = resp_.add_status();
		(*s) = **it;
	}
	finish();
}

}

