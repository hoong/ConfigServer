#include "../json_man.h"
#include "commandline.h"
#include "logger.h"
#include <unistd.h>
#include "service_engine/service.h"
#include "service_engine/rpc_handler.h"
#include "service_engine/rpc_channel.h"
#include "service_engine/rpc_proxy.h"
#include <boost/lexical_cast.hpp>
#include <boost/interprocess/detail/atomic.hpp>

#include "../proto/config_server.pb.rpc.h"

using namespace std;
using namespace service_engine;
using namespace config_server;


const string the_json  = "\
{\
	\"int\":1,\
	\"array\":[{\"element\":0},{\"element\":1},{\"element\":2}],\
	\"string\":\"string value\" \
}\
";

std::string cfg_ = "";

template <typename INVOKE_COMMAND>
struct Invoker
{
	Invoker() { }
	virtual ~Invoker(){}
	virtual void setIn() = 0;
	virtual void getOut() = 0;
	void invoke(boost::shared_ptr<RpcHandler> handler)
	{
		setIn();
		service_engine::RpcProxy<INVOKE_COMMAND> prxy(handler);
		prxy.call(req_, res_, 10000);
		getOut();
	}

protected:
	typename INVOKE_COMMAND::RequestType req_;
	typename INVOKE_COMMAND::ResponseType res_;
};

struct InvokerConfigGet:public Invoker<ConfigGet>
{
	InvokerConfigGet() { }
	virtual ~InvokerConfigGet() { }
	virtual void setIn()
	{
		req_.set_service_type("IMS");
		req_.set_path(".");
	}
	virtual void getOut()
	{
		cfg_ = res_.config();
		LOG(trace)<<"Config = "<<cfg_<<ENDL;
	}

};

struct InvokerConfigUpdate:public Invoker<ConfigUpdate>
{
	InvokerConfigUpdate() {}
	virtual ~InvokerConfigUpdate() {}
	virtual void setIn()
	{
		req_.set_service_type("IMS");
		req_.set_path(".");
		req_.set_new_config(the_json);
		req_.set_old_config(cfg_);
	}
	virtual void getOut()
	{
	}

};

struct InvokerStatusRegister:public Invoker<StatusRegister>
{
	InvokerStatusRegister(){}
	virtual ~InvokerStatusRegister(){}
	virtual void setIn()
	{
		req_.mutable_status()->set_server_addr("127.0.0.1:7777");
		req_.mutable_status()->set_service_type("IMS");
		req_.mutable_status()->set_network_type(NET_CT);
		req_.mutable_status()->set_service_name("Local");
		req_.mutable_status()->set_load(60);
		req_.mutable_status()->set_status(0);
	}
	virtual void getOut()
	{
	}

};


struct InvokerStatusRefresh:public Invoker<StatusRefresh>
{
	InvokerStatusRefresh(){}
	virtual ~InvokerStatusRefresh(){}
	virtual void setIn()
	{
		req_.mutable_status()->set_server_addr("127.0.0.1:7777");
		req_.mutable_status()->set_service_type("IMS");
		req_.mutable_status()->set_network_type(NET_CT);
		req_.mutable_status()->set_service_name("Local");
		req_.mutable_status()->set_load(60);
		req_.mutable_status()->set_status(0);
	}
	virtual void getOut()
	{
	}

};

struct InvokerServerAcquire:public Invoker<ServerAcquire>
{
	InvokerServerAcquire(){}
	virtual ~InvokerServerAcquire(){}
	virtual void setIn()
	{
		req_.set_service_type("IMS");
	}
	virtual void getOut()
	{
		LOG(trace)<<"Server Addr Acquired is :"<<res_.server_addr()<<ENDL;
	}

};

int main(int argc,char** argv)
{

	base::Logger::instance()->setLevel(0);
	//base::Logger::instance()->setFile("./test", 20*1024*1024, 10);
	base::Logger::instance()->setAutoFlush(true);
	CommandLineParser cmdline;
	cmdline.addStringOption("ip", "ip");
	cmdline.addIntOption("port", "port");
	cmdline.parse(argc, argv);
	/*

	string path= cmdline.values().getStringOption("path");
	JsonMan jm;
	if (!jm.set(the_json))
	{
		LOG(error)<<"parse jason failed"<<ENDL;
		exit(-1);
	}
	string json_str;
	//jm >> json_str;

	if (0 != jm.getSub(path,json_str))
	{
		LOG(error)<<"unavailable path"<<ENDL;
		exit(-1);
	}
	LOG(info)<<json_str<<ENDL;
	sleep(1);
	*/


	try
	{

		service_engine::Communicator communicator;
		service_engine::RpcChannelFactory channelfactory(&communicator);

		boost::shared_ptr<RpcHandler> handler =	
			channelfactory.createHandler(
				cmdline.values().getStringOption("ip").c_str(),
				cmdline.values().getIntOption("port"));

		InvokerConfigGet i_get;
		InvokerConfigUpdate i_update;
		InvokerStatusRegister i_register;
		InvokerStatusRefresh i_refresh;
		InvokerServerAcquire i_acquire;

		i_get.invoke(handler);
		i_update.invoke(handler);
		i_register.invoke(handler);
		i_refresh.invoke(handler);
		i_acquire.invoke(handler);




	}
	catch (exception& e)
	{
		LOG(error) << e.what() << ENDL;
	}



	return 0;

};

