

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
		cmdline.addIntOption("db-info", "db information");
		//service_engine::Service s(cmdline.parse(argc, argv));

    		base::log_init(0, NULL, 20*1024*1024, 10);

		LOG(info) << "\n\n====== server start ======\n\n";

		service_engine::RpcStubsPool pool(
				boost::shared_ptr<service_engine::RpcStubsPoolTraits>(new service_engine::RpcStubsMultiThreadPoolTraits(4)));

		service_engine::RpcStubsManager rpc_stub(&pool);
		rpc_stub.registerStub<config_server::MethodConfigGet>();
		rpc_stub.registerStub<config_server::MethodConfigUpdate>();
		rpc_stub.registerStub<config_server::MethodStatusRegister>();
		rpc_stub.registerStub<config_server::MethodStatusRefresh>();
		rpc_stub.registerStub<config_server::MethodStatusQuery>();
		rpc_stub.registerStub<config_server::MethodServerAcquire>();

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

