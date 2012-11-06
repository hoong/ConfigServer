#ifndef CONFIG_SERVER_METHOD_BASE_H_
#define CONFIG_SERVER_METHOD_BASE_H_

#include "service_engine/rpc_stub.h"
#include "rpc/config_service_handler.h"

namespace config_server
{

template<class MESSAGE>
class MethodConfigServerBase:public RpcMethod<MESSAGE>
{
public:
	typedef RpcMethod<MESSAGE> super;

	MethodConfigServerBase(boost::shared_ptr<RpcHandler> handler, 
			const base::packet::Header& header, 
			base::packet::Packet& body): super(handler,header,body){}

	virtual ~MethodConfigServerBase(){}

	boost::shared_ptr<ConfigServiceHandler> handler()
	{
		return boost::dynamic_pointer_cast<ConfigServiceHandler>(super::sphandler_);
	}

};


}

#endif
