#ifndef JSON_MAN_H_
#define JSON_MAN_H_

#include <string.h>
#include "json/json.h"


class json_man
{
public:
	json_man():m_ready(false){};
	~json_man(){};

	bool is_avail()
	{
		return m_ready;
	}

	bool set(const std::string& data);
	void operator>>(std::string& jsonstr);
	void operator<<(const std::string& jsonstr);

   /** \brief Experimental and untested: represents a "path" to access a node.
    *
    * Syntax:
    * - "." => root node
    * - ".[n]" => elements at index 'n' of root node (an array value)
    * - ".name" => member named 'name' of root node (an object value)
    * - ".name1.name2.name3"
    * - ".[0][1][2].name1[3]"
    * - ".%" => member name is provided as parameter
    * - ".[%]" => index is provied as parameter
    */

/*
 * return:
 * 	0 success
 * 	-1 path not found
 * 	-2 check consistency failed
 * 	-3 unavailable new_data
 */
	int update(const std::string& path,const std::string& new_data,const std::string& old_data);
private:
	Json::Value& get(const std::string& path);

private:
	//std::string m_data;
	Json::Value m_root;
	bool m_ready;
};



#endif
