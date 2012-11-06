/*
 * DBMysql.h
 *
 *  Created on: 2012-09-27
 *      Author: root
 */

#ifndef DB_MYSQL_H_
#define DB_MYSQL_H_

#include <mysql++/mysql++.h>
#include <stdint.h>
#include <string>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "base/singleton.h"
 
struct DbInfo
{

	int parse(std::string& info);

	std::string addr;
	std::string port;
	std::string db;
	std::string user;
	std::string passwd;
};


class DBMysql
{

public:
	DBMysql();
	virtual ~DBMysql();

	bool init(DbInfo& info)
	{
		dbinfo_=info;
		if (!connect())
		{
			return false;
		}
		return (0 == checkAndCreateTable());
	}

	bool isConnected() { return conn_.connected();}
	
	/*
	//获取数据库信息
	void get_dbinfo(DbInfo& info)
	{
		info = dbinfo_;
	}
	*/

public:

	//连接数据库
	bool connect();

	/*
	//获取服务默认配置
	int get_service_cfg(uint32_t service_id,std::string& data);
	*/

	//获取配置
	//int GetInstanceConfig(uint32_t inst_id,std::string& data);
	int getServiceConfig(const std::string& svc,std::string& data);

	//保存配置
	//int SetInstanceConfig(uint32_t inst_id,const std::string& cfg);
	int setServiceConfig(const std::string& svc,const std::string& cfg);

	/*
	//获取服务ID
	int GetInstanceService(uint32_t inst_id,uint32_t& service_id);
	*/

	/*
	//新建服务实例
	int create_instance(uint32_t service_id,const std::string& inst_name,uint32_t& inst_id);
	*/

private:
	int checkAndCreateTable();

private:
	mysqlpp::Connection conn_;
	DbInfo dbinfo_;
	boost::mutex mutex_;
};

typedef base::Singleton<DBMysql> MySqlMgr;

#endif /* DB_MYSQL_H_ */

