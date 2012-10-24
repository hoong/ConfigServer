#ifndef METHOD_STATUS_REFRESH_H_
#define METHOD_STATUS_REFRESH_H_

#include "service_engine/rpc_stub.h"
#include "proto/config_server.pb.rpc.h"
#include "rpc/config_service_handler.h"


namespace config_server{

class MethodStatusRefresh: public MethodConfigServerBase<StatusRefresh>
{
public:
	MethodStatusRefresh(boost::shared_ptr<service_engine::RpcHandler> handler, const base::packet::Header& header, base::packet::Packet& body):MethodConfigServerBase<StatusRefresh>(handler,header,body){};;
	virtual ~MethodStatusRefresh();

	virtual void call();

};


};
#endif
