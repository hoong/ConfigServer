#ifndef METHOD_CONFIG_UPDATE_H_
#define METHOD_CONFIG_UPDATE_H_

#include "proto/config_server.pb.rpc.h"
#include "method_base.h"


namespace config_server{

class MethodConfigUpdate: public MethodConfigServerBase<ConfigUpdate>
{
public:
	MethodConfigUpdate(boost::shared_ptr<RpcHandler> handler, 
			const base::packet::Header& header, 
			base::packet::Packet& body):
		MethodConfigServerBase<ConfigUpdate>(handler,header,body){}
	virtual ~MethodConfigUpdate();

	virtual void onCall();

};


}
#endif
