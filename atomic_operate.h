#ifndef ATOMIC_OPERATE_H_
#define ATOMIC_OPERATE_H_

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <map>


struct OperateTask
{
	virtual ~OperateTask(){};
	virtual int run() = 0;
};

class AtomicOperate
{

	class Operate 
	{
	public:
	
		int Process(OperateTask* t)
		{
			boost::mutex::scoped_lock lock(mutex_);
			return t->run();
		};
	private:
		boost::mutex mutex_;
	};

public:

int operate(const std::string& k,OperateTask* t)
{
	boost::shared_ptr<Operate> op;

	std::map<std::string,boost::shared_ptr<Operate> >::iterator it;
	it = opmap_.find(k);
	if (it == opmap_.end())
	{
		boost::mutex::scoped_lock lock(mutex_);
		op = boost::shared_ptr<Operate>(new Operate);
		opmap_.insert(std::pair<std::string,boost::shared_ptr<Operate> >(k,op));
	}
	else
	{
		op = it->second;
	};

	if (op.get())
	{
		return op->Process(t);
	}
	else
	{
		return -1;
	};
}



private:
	boost::mutex mutex_;
	std::map<std::string,boost::shared_ptr<Operate> > opmap_;

};



#endif
