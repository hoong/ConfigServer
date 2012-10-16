/*
 * db_mysql.h
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
 
struct db_info
{
	std::string db;
	std::string addr;
	std::string user;
	std::string passwd;
};


class db_mysql
{

public:
	db_mysql();
	virtual ~db_mysql();

	bool Init(db_info& info)
	{
		m_dbinfo=info;
		return connect();
	};

	bool IsConnected() { return m_conn.connected();}
	
	//获取数据库信息
	void get_dbinfo(db_info& info)
	{
		info = m_dbinfo;
	};

public:

	//连接数据库
	bool connect();

	/*
	//获取服务默认配置
	int get_service_cfg(uint32_t service_id,std::string& data);
	*/

	//获取配置
	int get_instance_cfg(uint32_t inst_id,std::string& data);

	//保存配置
	int set_instance_cfg(uint32_t inst_id,const std::string& cfg);

	//获取服务ID
	int get_instance_service(uint32_t inst_id,uint32_t& service_id);

	/*
	//新建服务实例
	int create_instance(uint32_t service_id,const std::string& inst_name,uint32_t& inst_id);
	*/


private:
	mysqlpp::Connection m_conn;
	db_info m_dbinfo;
	boost::mutex m_mutex;
};

#endif /* DB_MYSQL_H_ */

