#include "service.h"
#include "config_service_acceptor.h"
#include "config_service_handler.h"
#include "rpc_exception.h"

ConfigServiceAcceptor::ConfigServiceAcceptor(int listen_port, Communicator* communicator, RpcStubsManager* rpc_stub_man)
: acceptor_(communicator->reactor())
{
	if (-1 == acceptor_.open(
			listen_port,
			-1,
			new ConfigServiceHandlerCreatorStrategy(rpc_stub_man)))
	{
		THROW_RPC_CALL_EXCEPTION(service::rpc::SC_RPC_TRANSPORT_ERROR, "listen failed!");
	}
}

