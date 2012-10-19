#include "json_man.h"



bool json_man::set(const std::string& data)
{
	Json::Reader reader;
	return (m_ready = reader.parse(data.c_str(),m_root));
}

/*
 * return:
 * 	0 success
 * 	-1 path not found
 * 	-2 check consistency failed
 * 	-3 unavailable new_data
 */
int json_man::update(const std::string& path,const std::string& new_data,const std::string& old_data)
{
	Json::Reader reader;
	Json::Value sub_value = get(path);
	if (sub_value.empty())
	{
		LOG(error)<<"cannot found path"<<ENDL;
		return -1;
	}

	//检查一致性
	Json::Value old_value ;
	reader.parse(old_data.c_str(),old_value);
	if(old_value != sub_value)
	{
		LOG(error)<<"check consistency failed"<<ENDL;
		return -2;
	}

	if (reader.parse(new_data.c_str(),sub_value))
	{
		LOG(error)<<"unavailable json string"<<ENDL;
		return -3;
	}
	sub_value = new_value;

	return 0;
}


Json::Value& json_man::get(const std::string& path)
{
	Json::Path json_path(path);
	return json_path.resolve(m_root);
}

void json_man::operator>>(std::string& jsonstr)
{
	Json::StyledWriter writer;
	jsonstr = writer(write(m_root));
}


void json_man::operator<<(const std::string& jsonstr)
{
	set(jsonstr);
}

