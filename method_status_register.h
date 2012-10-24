#ifndef METHOD_STATUS_REGISTER_H_
#define METHOD_STATUS_REGISTER_H_

#include "service_engine/rpc_stub.h"
#include "proto/config_server.pb.rpc.h"
#include "rpc/config_service_handler.h"


namespace config_server{

class MethodStatusRegister: public MethodConfigServerBase<StatusRegister>
{
public:
	MethodStatusRegister(boost::shared_ptr<service_engine::RpcHandler> handler, const base::packet::Header& header, base::packet::Packet& body):MethodConfigServerBase<StatusRegister>(handler,header,body){};
	virtual ~MethodStatusRegister();

	virtual void call();

};


};
#endif
