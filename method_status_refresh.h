#ifndef METHOD_STATUS_REFRESH_H_
#define METHOD_STATUS_REFRESH_H_

#include "proto/config_server.pb.rpc.h"
#include "method_base.h"


namespace config_server{

class MethodStatusRefresh: public MethodConfigServerBase<StatusRefresh>
{
public:
	MethodStatusRefresh(boost::shared_ptr<RpcHandler> handler, 
			const base::packet::Header& header, 
			base::packet::Packet& body):
		MethodConfigServerBase<StatusRefresh>(handler,header,body){}
	virtual ~MethodStatusRefresh();

	virtual void onCall();

};


}
#endif
