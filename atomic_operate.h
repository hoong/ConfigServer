#ifndef ATOMIC_OPERATE_H_
#define ATOMIC_OPERATE_H_

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <map>


template<typename KEY,typename TASK>
class ATOMIC_OPERATE
{

	class OPERATE 
	{
	public:
	
		void process(TASK& t)
		{
			boost::scoped_lock(mutex_);
			t->run();
		};
	private:
		boost::mutex mutex_;
	};

public:

int operate(KEY k,TASK& t)
{
	boost::shared_ptr<OPERATE> op;

	std::map<KEY,boost::shared_ptr<OPERATE> >::iterator it;
	it = opmap.find(k);
	if (it == opmap::end())
	{
		boost::scoped_lock(map_mutex);
		op = boost::shared_ptr<OPERATE>(new OPERATE);
		opmap.insert(std::pair<KEY,boost::shared_ptr<OPERATE> >(k,op));
	}
	else
	{
		op = it->second;
	};

	if (op.get())
	{
		return op->process(t);
	}
	else
	{
		return -1;
	};
};



private:

	boost::mutex map_mutex;
	std::map<KEY,boost::shared_ptr<OPERATE> > opmap;
};



#endif
