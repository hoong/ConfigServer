/*
 * DBMysql.h
 *
 *  Created on: 2012-09-27
 *      Author: root
 */

#ifndef DB_MYSQL_H_
#define DB_MYSQL_H_

#include <mysql++.h>
#include <stdint>
#include <string>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "base/singleton.h"
 
struct DbInfo
{
	std::string db;
	std::string addr;
	std::string user;
	std::string passwd;
};


class DBMysql
{

public:
	DBMysql();
	virtual ~DBMysql();

	bool Init(& info)
	{
		m_dbinfo=info;
		return connect();
	};

	bool IsConnected() { return m_conn.connected();}
	
	//获取数据库信息
	void get_dbinfo(& info)
	{
		info = m_dbinfo;
	};

public:

	//连接数据库
	bool Connect();

	/*
	//获取服务默认配置
	int get_service_cfg(uint32_t service_id,std::string& data);
	*/

	//获取配置
	int GetInstanceConfig(uint32_t inst_id,std::string& data);
	int GetServiceConfig(const std::string& svc,std::string& data);

	//保存配置
	int SetInstanceConfig(uint32_t inst_id,const std::string& cfg);
	int SetServiceConfig(const std::string& svc,const std::string& cfg);

	/*
	//获取服务ID
	int GetInstanceService(uint32_t inst_id,uint32_t& service_id);
	*/

	/*
	//新建服务实例
	int create_instance(uint32_t service_id,const std::string& inst_name,uint32_t& inst_id);
	*/


private:
	mysqlpp::Connection m_conn;
	DbInfo m_dbinfo;
	boost::mutex m_mutex;
};

typedef Singleton<DBMysql> MySqlMgr;

#endif /* DB_MYSQL_H_ */

