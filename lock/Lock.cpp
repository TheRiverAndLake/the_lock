#include "stdafx.h"
#include "Lock.h"
#include <string>
#ifdef WIN32
#else
void InitializeCriticalSection(pthread_mutex_t* m)
{
	//*m = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_init(m, 0);
}
void DeleteCriticalSection(pthread_mutex_t* m)
{
	pthread_mutex_destroy(m);
	//*m = PTHREAD_MUTEX_INITIALIZER;
}
void EnterCriticalSection(pthread_mutex_t* m)
{
	pthread_mutex_lock(m);
}
void LeaveCriticalSection(pthread_mutex_t* m)
{
	pthread_mutex_unlock(m);
}
#endif
extern void userlog(const std::string& msg);
namespace base
{
	Lock::Lock(void)
	{
		::InitializeCriticalSection(&m_lock);
		m_lockcount = 0;
	}

	Lock::~Lock(void)
	{
		::DeleteCriticalSection(&m_lock);
	}
	
	void Lock::setname(const std::string& name)
	{
		m_name = name;
	}
	void Lock::lock()
	{
		EnterCriticalSection(&m_lock);
		if (m_name.length() > 0)
		{
			char buf[100]={0};
			sprintf(buf, "lock %p in ,tid %d,%s", this, (int)::GetCurrentThreadId(),m_name.c_str());
			userlog(buf);
		}
		++m_lockcount;
	}
	void Lock::unlock()
	{
		if (m_name.length() > 0)
		{
			char buf[100]={0};
			sprintf(buf, "lock %p out,tid %d,%s", this, (int)::GetCurrentThreadId(),m_name.c_str());
			userlog(buf);
		}
		
		int count = --m_lockcount;
		
		LeaveCriticalSection(&m_lock);
		//if (count != 0)
		//	throw std::string("somewhere not unlock");
	}
}