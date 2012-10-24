#ifndef METHOD_SERVER_ACQUIRE_H_
#define METHOD_SERVER_ACQUIRE_H_

#include "proto/config_server.pb.rpc.h"
#include "method.h"

namespace config_server{

class MethodServerAcquire: public MethodConfigServerBase<ServerAcquire>
{
public:
	MethodServerAcquire(boost::shared_ptr<service_engine::RpcHandler> handler, const base::packet::Header& header, base::packet::Packet& body):MethodConfigServerBase<ServerAcquire>(handler,header,body){};
	virtual ~MethodServerAcquire();

	virtual void call();

};


};
#endif
