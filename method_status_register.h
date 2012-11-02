#ifndef METHOD_STATUS_REGISTER_H_
#define METHOD_STATUS_REGISTER_H_

#include "proto/config_server.pb.rpc.h"
#include "method_base.h"


namespace config_server{

class MethodStatusRegister: public MethodConfigServerBase<StatusRegister>
{
public:
	MethodStatusRegister(boost::shared_ptr<RpcHandler> handler, 
			const base::packet::Header& header,
		       	base::packet::Packet& body):
		MethodConfigServerBase<StatusRegister>(handler,header,body){};
	virtual ~MethodStatusRegister();

	virtual void onCall();

};


};
#endif
