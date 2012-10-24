#ifndef METHOD_STATUS_QUERY_H_
#define METHOD_STATUS_QUERY_H_

#include "service_engine/rpc_stub.h"
#include "proto/config_server.pb.rpc.h"
#include "rpc/config_service_handler.h"


namespace config_server{

class MethodStatusQuery: public MethodConfigServerBase<StatusQuery>
{
public:
	MethodStatusQuery(boost::shared_ptr<service_engine::RpcHandler> handler, const base::packet::Header& header, base::packet::Packet& body):MethodConfigServerBase<StatusQuery>(handler,header,body){};;
	virtual ~MethodStatusQuery();

	virtual void call();

};


};
#endif
