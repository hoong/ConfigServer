#include "json_man.h"
#include "logger.h"



bool JsonMan::set(const std::string& data)
{
	Json::Reader reader;
	return (ready_ = reader.parse(data.c_str(),root_));
}

/*
 * return:
 * 	0 success
 * 	-1 path not found
 * 	-2 check consistency failed
 * 	-3 unavailable new_data
 */
int JsonMan::update(const std::string& path,const std::string& new_data,const std::string& old_data)
{
	Json::Reader reader;
	//Json::Value sub_value = get(path);
	Json::Value sub_value;   get(path,sub_value);
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

	Json::Value new_value ;
	if (reader.parse(new_data.c_str(),new_value))
	{
		LOG(error)<<"unavailable json string"<<ENDL;
		return -3;
	}
	sub_value = new_value;

	return 0;
}


/*
Json::Value& JsonMan::get(const std::string& path)
{
	Json::Path json_path(path);
	return json_path.resolve(root_);
}
*/

void JsonMan::get(const std::string& path,Json::Value& v)
{
	Json::Path json_path(path);
	v = json_path.resolve(root_,v);
}


void JsonMan::operator>>(std::string& jsonstr)
{
	Json::StyledWriter writer;
	jsonstr = writer.write(root_);
}


void JsonMan::operator<<(const std::string& jsonstr)
{
	set(jsonstr);
}

int JsonMan::getSub(const std::string& path,std::string& cfg)
{

	Json::Value v ; 
	get(path,v);
	if (v.isNull())
	{
		LOG(error)<<"cannot found path"<<ENDL;
		return -1;
	}
	Json::StyledWriter writer;
	cfg = writer.write(v);
	return 0;
}
