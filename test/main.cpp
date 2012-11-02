#include "../json_man.h"
#include "commandline.h"
#include "logger.h"
#include <unistd.h>


using namespace std;
using namespace service_engine;


const string the_json  = "\
{\
	\"int\":1,\
	\"array\":[{\"element\":0},{\"element\":1},{\"element\":2}],\
	\"string\":\"string value\" \
}\
";

int main(int argc,char** argv)
{

	base::Logger::instance()->setLevel(0);
	//base::Logger::instance()->setFile("./test", 20*1024*1024, 10);
	base::Logger::instance()->setAutoFlush(true);
	CommandLineParser cmdline;
	cmdline.addStringOption("path", "Json Path");
	cmdline.parse(argc, argv);

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

	return 0;

};

