#ifndef METHOD_CONFIG_GET_H_
#define METHOD_CONFIG_GET_H_

#include "proto/config_server.pb.rpc.h"
#include "method_base.h"


namespace config_server{


class MethodConfigGet: public MethodConfigServerBase<ConfigGet>
{
public:
	MethodConfigGet(boost::shared_ptr<RpcHandler> handler, 
			const base::packet::Header& header, 
			base::packet::Packet& body):
		MethodConfigServerBase<ConfigGet>(handler,header,body){}
	virtual ~MethodConfigGet();

	virtual void onCall();

};


}
#endif
