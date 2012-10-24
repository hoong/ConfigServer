#ifdef CONFIG_SERVER_METHOD_H_
#define CONFIG_SERVER_METHOD_H_

#include "service_engine/rpc_stub.h"
#include "rpc/config_service_handler.h"

namespace config_server
{

template<MESSAGE>
class MethodConfigServerBase::public service_engine::RpcMethod<MESSAGE>
{
public:
	MethodConfigServerBase(boost::shared_ptr<service_engine::RpcHandler> handler, const base::packet::Header& header, base::packet::Packet& body):
		service_engine::RpcMethod<MESSAGE>(handler,header,body){};
	virtual ~MethodConfigServerBase(){};
	boost::shared_ptr<ConfigServiceHandler> handler()
	{
		return boost::dynamic_pointer_cast<ConfigServiceHandler>(sphandler_);
	};

};



};

#endif
