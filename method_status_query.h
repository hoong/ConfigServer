#ifndef METHOD_STATUS_QUERY_H_
#define METHOD_STATUS_QUERY_H_

#include "proto/config_server.pb.rpc.h"
#include "method_base.h"
#include "service_config.h"


namespace config_server{


struct StatusQueryTask:public IterateF
{
	StatusQueryTask();
	virtual ~StatusQueryTask();
	virtual void doit(ServiceInstances& si);
	std::vector<Status*> status_list;
};

class MethodStatusQuery: public MethodConfigServerBase<StatusQuery>
{
public:
	MethodStatusQuery(boost::shared_ptr<RpcHandler> handler, 
			const base::packet::Header& header,
		       	base::packet::Packet& body):
		MethodConfigServerBase<StatusQuery>(handler,header,body){}
	virtual ~MethodStatusQuery();

	virtual void onCall();


};


}
#endif
