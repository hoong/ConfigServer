#ifndef ATOMIC_OPERATE_H_
#define ATOMIC_OPERATE_H_

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <map>


template<typename KEY,typename TASK>
class AtomicOperate
{

	class Operate 
	{
	public:
	
		void Process(TASK& t)
		{
			boost::scoped_lock lock(mutex_);
			t->run();
		};
	private:
		boost::mutex mutex_;
	};

public:

int Operate(KEY k,TASK& t)
{
	boost::shared_ptr<Operate> op;

	std::map<KEY,boost::shared_ptr<Operate> >::iterator it;
	it = opmap.find(k);
	if (it == opmap::end())
	{
		boost::scoped_lock lock(map_mutex);
		op = boost::shared_ptr<Operate>(new Operate);
		opmap.insert(std::pair<KEY,boost::shared_ptr<Operate> >(k,op));
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
};



private:

	boost::mutex map_mutex;
	std::map<KEY,boost::shared_ptr<Operate> > opmap;
};



#endif
