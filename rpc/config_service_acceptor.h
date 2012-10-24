
#include "base/net/acceptor.h"
#include "communicator.h"
#include "config_service_handler.h"

namespace config_server
{
class SE_API ConfigServiceAcceptor
{
public:
	ConfigServiceAcceptor(int listen_port, Communicator* communicator, RpcStubsManager* rpc_stub_man);
	virtual ~ConfigServiceAcceptor();

private:
	base::net::Acceptor acceptor_;
};
};

