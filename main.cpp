

#include "base/logger.h"
#include "service_engine/rpc_channel.h"
#include "service_engine/service.h"
#include "db_mysql.h"
#include "rpc/config_service_acceptor.h"

int main(int argc,char**argv)
{
	try
	{
		service_engine::CommandLineParser cmdline;
		cmdline.addIntOption("listen-port", "listen port");
		//service_engine::Service s(cmdline.parse(argc, argv));

		LOG(info) << "\n\n====== server start ======\n\n";

		service_engine::RpcStubsPool pool(
				boost::shared_ptr<service_engine::RpcStubsPoolTraits>(new service_engine::RpcStubsMultiThreadPoolTraits(4)));

		service_engine::RpcStubsManager rpc_stub(&pool);
		rpc_stub.registerStub<config::MethodConfigGet>();
		rpc_stub.registerStub<config::MethodConfigUpdate>();
		rpc_stub.registerStub<config::MethodStatusRegister>();
		rpc_stub.registerStub<config::MethodStatusRefresh>();
		rpc_stub.registerStub<config::MethodStatusQuery>();
		rpc_stub.registerStub<config::MethodServerAcquire>();

		service_engine::Communicator communicator;

		try
		{
			config_server::ConfigServiceAcceptor acceptor(
					cmdline.getIntOption("listen-port"),
					&communicator,
					&rpc_stub);
		}
		catch (exception& e)
		{
			LOG(error) << e.what() << ENDL;
			exit(-1);
		}


		communicator.wait();
	}
	catch (exception& e)
	{
		LOG(error) << e.what() << ENDL;
	}

	LOG(info) << "exit.";

	return 0;

};

