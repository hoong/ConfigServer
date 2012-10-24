#ifndef METHOD_CONFIG_GET_H_
#define METHOD_CONFIG_GET_H_

#include "service_engine/rpc_stub.h"
#include "proto/config_server.pb.rpc.h"
#include "rpc/config_service_handler.h"


namespace config_server{

class MethodConfigGet: public MethodConfigServerBase<ConfigGet>
{
public:
	MethodConfigGet(boost::shared_ptr<service_engine::RpcHandler> handler, const base::packet::Header& header, base::packet::Packet& body):MethodConfigServerBase<ConfigGet>(handler,header,body){};;
	virtual ~MethodConfigGet();

	virtual void call();

};


};
#endif
