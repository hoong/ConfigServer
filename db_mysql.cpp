/*
 * db_mysql.cpp
 *
 *  Created on: 2012-09-27
 *      Author: root
 */

#include "db_mysql.h"
#include "logger.h"



db_mysql::db_mysql() : m_bValid(false)
{
}

db_mysql::~db_mysql()
{
}

bool db_mysql::connect()
{
	bool ret = true;
	try
	{
		ret = m_conn.connect(m_dbinfo.db.c_str(),
			m_dbinfo.addr.c_str(),
			m_dbinfo.user.c_str(),
			m_dbinfo.passwd.c_str());

	}
	catch(const std::Exception& e)
	{
		LOG(error)<<"connect failed:"<<e.what()<<ENDL;
		ret = false;
	}

	return ret;
}


//获取配置
int db_mysql::get_instance_cfg(uint32_t inst_id,std::string& data)
{
	const std::string sql = "select cfg from service_instance where inst_id = %0";

	try
	{
		boost::mutex::scoped_lock lock(m_mutex);
		mysqlpp::StoreQueryResult result ;

		mysqlpp::SQLQueryParms params;
		params << inst_id ;

		mysqlpp::Query query = m_conn.query(sql.c_str());
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
	catch(const std::Exception& e)
	{
		LOG(error)<<"get_instance_cfg failed:"<<e.what()<<ENDL;
		return -1;
	}

	return 0;

}

//保存配置
int db_mysql::set_instance_cfg(uint32_t inst_id,const std::string& cfg)
{
	const std::string sql = "update service_instance set cfg = %0 where inst_id = %1";

	try
	{
		boost::mutex::scoped_lock lock(m_mutex);

		mysqlpp::SQLQueryParms params;
		params << inst_id ;

		mysqlpp::Query query = m_conn.query(sql.c_str());
		query.parse();
		query.execute(params);
	}
	catch(std::Exception& e)
	{
		LOG(error)<<"set_instance_cfg failed:"<<e.what()<<ENDL;
		return -1;
	}

	return 0;

}

//获取服务ID
int db_mysql::get_instance_service(uint32_t inst_id,uint32_t& service_id)
{
	const std::string sql = "select service_id from service_instance where inst_id = %0";

	try
	{
		boost::mutex::scoped_lock lock(m_mutex);
		mysqlpp::StoreQueryResult result ;

		mysqlpp::SQLQueryParms params;
		params << inst_id ;

		mysqlpp::Query query = m_conn.query(sql.c_str());
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
	catch(const std::Exception& e)
	{
		LOG(error)<<"get_instance_service failed:"<<e.what()<<ENDL;
		return -1;
	}

	return 0;


}

/*
int db_mysql::get_conf_data(uint32_t inst_id,const char* pre=NULL,std::vector<CFG_TYPE>& cfg_list)
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
		LOG(error)<<"db_mysql::get_conf_data Fail!n"<<ENDL;
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

int db_mysql::set_conf_data(uint32_t inst_id,const CFG_TYPE& cfg)
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
		LOG(error)<<"db_mysql::set_conf_data Fail!";
		return -1;
	}

	return 0;
}
*/
