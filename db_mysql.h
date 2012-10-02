/*
 * db_mysql.h
 *
 *  Created on: 2012-09-27
 *      Author: root
 */

#ifndef DB_MYSQL_H_
#define DB_MYSQL_H_

#include "mysqlppconnector/IMysqlConnMgr.h"
#include <map>
#include <stdint>
#include <string>
#include <vector>

typedef std::pair<std::string,std::string> CFG_TYPE;


class db_mysql
{

public:
	db_mysql();
	virtual ~db_mysql();

	bool Init();
	bool IsValid() { return m_bValid; }

public:

	int get_conf_data(uint32_t inst_id,const char* pre=NULL,std::vector<CFG_TYPE>& cfg_list);

	int set_conf_data(uint32_t inst_id,const CFG_TYPE& cfg);

private:
	boost::scoped_ptr<mysqlpp_connector::IMysqlConnMgr> m_spConnMgr;

	bool m_bValid;
};

#endif /* DB_MYSQL_H_ */

