

#include "base/logger.h"
#include "util.h"
#include "service_engine/rpc_channel.h"
#include "service_engine/service.h"
#include "db_mysql.h"
#include "rpc/config_service_acceptor.h"

#include "method_config_get.h"
#include "method_config_update.h"
#include "method_server_acquire.h"
#include "method_status_query.h"
#include "method_status_refresh.h"
#include "method_status_register.h"

using namespace service_engine;
using namespace config_server;


int main(int argc,char**argv)
{

	base::util::daemonize();
    //base::log_init(0, NULL, 20*1024*1024, 10);
	base::Logger::instance()->setLevel(0);
	base::Logger::instance()->setFile("./log/config_server", 20*1024*1024, 10);
	base::Logger::instance()->setAutoFlush(true);

	try
	{

		CommandLineParser cmdline;
		cmdline.addIntOption("listen-port", "listen port");
		cmdline.addStringOption("db-info", "db information");
		//Service s(cmdline.parse(argc, argv));
		cmdline.parse(argc, argv);
		DbInfo db ; 
		std::string info = cmdline.values().getStringOption("db-info");
		db.parse(info);
		if (!MySqlMgr::instance().init(db))
		{
			LOG(error) << "\n\n====== connect to mysql failed ======\n\n";
			exit(-1);
		}
		LOG(info) << "\n\n====== server start ======\n\n";

		RpcStubsPool pool(
				boost::shared_ptr<RpcStubsPoolTraits>(new RpcStubsMultiThreadPoolTraits(4)));

		RpcStubsManager rpc_stub(&pool);
		rpc_stub.registerStub<MethodConfigGet>();
		rpc_stub.registerStub<MethodConfigUpdate>();
		rpc_stub.registerStub<MethodStatusRegister>();
		rpc_stub.registerStub<MethodStatusRefresh>();
		rpc_stub.registerStub<MethodStatusQuery>();
		rpc_stub.registerStub<MethodServerAcquire>();

		Communicator communicator;

		try
		{
			ConfigServiceAcceptor acceptor(
					cmdline.values().getIntOption("listen-port"),
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

