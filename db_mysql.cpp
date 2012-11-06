/*
 * db_mysql.cpp
 *
 *  Created on: 2012-09-27
 *      Author: root
 */

#include "db_mysql.h"
#include "logger.h"

struct InfoParser
{
	InfoParser(const std::string& s):info(s),start(0),found(0){}
	~InfoParser(){}

	std::string pop()
	{
		if (found == string::npos)
			return "";
		if (found != 0)
			start = found;
		found = info.find(':',start);
		if (found == string::npos)
			return info.substr(start);
		else
			return info.substr(start,(found++)-start);
	}

	size_t start;
	size_t found;
	std::string info;
};

int DbInfo::parse(std::string& info)
{

	InfoParser infop(info);
	addr = infop.pop();
	port = infop.pop();
	db = infop.pop();
	user = infop.pop();
	passwd = infop.pop();
	return 0;
}

DBMysql::DBMysql()
{
}

DBMysql::~DBMysql()
{
}

bool DBMysql::connect()
{
	bool ret = true;
	try
	{
		LOG(trace)<<"db="<<dbinfo_.db
		<<",server="<<dbinfo_.addr
		<<",user="<<dbinfo_.user
		<<",passwd="<<dbinfo_.passwd
		<<",port="<<dbinfo_.port<<ENDL;

		ret = conn_.connect(dbinfo_.db.c_str(),
			dbinfo_.addr.c_str(),
			dbinfo_.user.c_str(),
			dbinfo_.passwd.c_str(),
			atoi(dbinfo_.port.c_str()));

	}
	catch(const mysqlpp::Exception &e)
	{
		LOG(error)<<"connect failed:"<<e.what()<<ENDL;
		ret = false;
	}
	catch(const std::exception& e)
	{
		LOG(error)<<"connect failed:"<<e.what()<<ENDL;
		ret = false;
	}

	return ret;
}


/*
//获取配置
int DBMysql::GetInstanceConfig(uint32_t inst_id,std::string& data)
{
	const std::string sql = "select cfg from service_instance where inst_id = %0";

	try
	{
		boost::mutex::scoped_lock lock(mutex_);
		mysqlpp::StoreQueryResult result ;

		mysqlpp::SQLQueryParms params;
		params << inst_id ;

		mysqlpp::Query query = conn_.query(sql.c_str());
		query.parse();
		result = query.store(params);

		if (result.empty())
		{
			LOG(trace)<<"No data found"<<ENDL;
			return 1;
		}
		mysqlpp::Row& row = result[0];
		data.assign(row["cfg"].data(),row["cfg"].size());


	}
	catch(const std::exception& e)
	{
		LOG(error)<<"GetInstanceConfig failed:"<<e.what()<<ENDL;
		return -1;
	}

	return 0;

}
*/

int DBMysql::getServiceConfig(const std::string& svc,std::string& data)
{
	const std::string sql = "select service_config from tb_service_config where service_type = %0 ";

	try
	{
		boost::mutex::scoped_lock lock(mutex_);
		mysqlpp::StoreQueryResult result ;

		mysqlpp::SQLQueryParms params;
		params << svc;

		mysqlpp::Query query = conn_.query(sql.c_str());
		query.parse();
		result = query.store(params);

		if (result.empty())
		{
			LOG(trace)<<"No data found"<<ENDL;
			return 1;
		}
		mysqlpp::Row& row = result[0];
		data.assign(row["service_config"].data(),row["service_config"].size());


	}
	catch(const std::exception& e)
	{
		LOG(error)<<"GetInstanceConfig failed:"<<e.what()<<ENDL;
		return -1;
	}

	return 0;

}


/*
//保存配置
int DBMysql::SetInstanceConfig(uint32_t inst_id,const std::string& cfg)
{
	const std::string sql = "update service_instance set cfg = %0 where inst_id = %1";

	try
	{
		boost::mutex::scoped_lock lock(mutex_);

		mysqlpp::SQLQueryParms params;
		params << inst_id ;

		mysqlpp::Query query = conn_.query(sql.c_str());
		query.parse();
		query.execute(params);
	}
	catch(std::exception& e)
	{
		LOG(error)<<"SetInstanceConfig failed:"<<e.what()<<ENDL;
		return -1;
	}

	return 0;

}
*/

int DBMysql::setServiceConfig(const std::string& svc,const std::string& cfg)
{
	const std::string sql = "update tb_service_config set service_config = %0 where service_type= %1";

	try
	{
		boost::mutex::scoped_lock lock(mutex_);

		mysqlpp::SQLQueryParms params;
		params << svc <<cfg;

		mysqlpp::Query query = conn_.query(sql.c_str());
		query.parse();
		query.execute(params);
	}
	catch(std::exception& e)
	{
		LOG(error)<<"SetInstanceConfig failed:"<<e.what()<<ENDL;
		return -1;
	}

	return 0;

}

/*
CREATE TABLE IF NOT EXISTS `tb_service_config` (
	`service_type` VARCHAR(50) NOT NULL DEFAULT '',
	`service_config` LONGTEXT NULL,
	PRIMARY KEY (`service_type`)
)
*/
int DBMysql::checkAndCreateTable()
{

	boost::mutex::scoped_lock lock(mutex_);

	try 
	{
		std::string sql = "select count(*) cnt from tb_service_config";
		mysqlpp::StoreQueryResult result ;


		mysqlpp::Query query = conn_.query(sql.c_str());
		query.parse();
		result = query.store();

		return 0;
	}
	catch(std::exception& e)
	{
		LOG(error)<<"table not exists ,create!::"<<e.what()<<ENDL;
	}

	try
	{
		std::string sql= "CREATE TABLE IF NOT EXISTS `tb_service_config` ( \
				`service_type` VARCHAR(50) NOT NULL DEFAULT '', \
				`service_config` LONGTEXT NULL, \
				PRIMARY KEY (`service_type`) ) ";


		mysqlpp::Query query = conn_.query(sql.c_str());
		query.parse();
		query.execute();
	}
	catch(std::exception& e)
	{
		LOG(error)<<""<<e.what()<<ENDL;
		return -1;
	}

	return 0;
}

/*
//获取服务ID
int DBMysql::GetInstanceService(uint32_t inst_id,uint32_t& service_id)
{
	const std::string sql = "select service_id from service_instance where inst_id = %0";

	try
	{
		boost::mutex::scoped_lock lock(mutex_);
		mysqlpp::StoreQueryResult result ;

		mysqlpp::SQLQueryParms params;
		params << inst_id ;

		mysqlpp::Query query = conn_.query(sql.c_str());
		query.parse();
		result = query.store(params);

		if (result.empty())
		{
			LOG(trace)<<"No data found"<<ENDL;
			return 1;
		}
		mysqlpp::Row& row = result[0];
		service_id = row["service_id"];

	}
	catch(const std::exception& e)
	{
		LOG(error)<<"GetInstanceService failed:"<<e.what()<<ENDL;
		return -1;
	}

	return 0;


}
*/

/*
int DBMysql::get_conf_data(uint32_t inst_id,const char* pre=NULL,std::vector<CFG_TYPE>& cfg_list)
{
	LOG(trace)<<"inst_id="<<inst_id<<",pre="<<pre<<ENDL;

	if	(!m_spConnMgr.get())
		return -1;

	mysqlpp::SQLQueryParms params;
	params << inst_id <<pre;

	const static std::string strQuery =
			"select cfg_key,cfg_value from tb_app_inst a where a.inst_id = %0 and cfg_key like '%1.%' ";

	mysqlpp::StoreQueryResult result ;
	if (m_spConnMgr->store(0, strQuery, params, result) < 0)
	{
		LOG(error)<<"DBMysql::get_conf_data Fail!n"<<ENDL;
		return -1;
	}


	if (result.num_rows() > 0)
	{
		for (size_t i=0; i<result.num_rows(); ++i)
		{
			if (!result[i]["cfg_key"].is_null() && !result[i]["cfg_value"].is_null())
			{
				CFG_TYPE elem (result[i]["cfg_key"],result[i]["cfg_value"]);
				cfg_list.push_back(elem);
			}
		}
	}

	return 0;
}

int DBMysql::set_conf_data(uint32_t inst_id,const CFG_TYPE& cfg)
{
	LOG(trace)<<"inst_id="<<inst_id<<",key="<<cfg.first<<",value="<<cfg.second<<ENDL;

	if (!m_spConnMgr.get())
		return -1;

	mysqlpp::SQLQueryParms params;
	params << inst_id <<cfg.first<< cfg.second;

	const static std::string strQuery =
			"insert into tb_app_inst(inst_id,cfg_key,cfg_value) values (%0,%1,%2) "
			"on duplicate key update cfg_value = %2 ";

	mysqlpp::SimpleResult result;
	if (m_spConnMgr->execute(0, strQuery, params, result) < 0)
	{
		LOG(error)<<"DBMysql::set_conf_data Fail!";
		return -1;
	}

	return 0;
}
*/
