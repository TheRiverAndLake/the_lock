#pragma once
#include <string>
#ifdef WIN32
#include <Windows.h>
#else
#include <pthread.h>
#define  CRITICAL_SECTION pthread_mutex_t
#endif

namespace base
{
	class Lock
	{
	public:
		Lock(void);
		~Lock(void);
		void lock();
		void unlock();
		void setname(const std::string& name);
	private:
		CRITICAL_SECTION m_lock;
		int m_lockcount;
		std::string m_name;
	};

	class AutoLock
	{
	public:
		AutoLock(Lock& lock)
		{
			m_pLock = &lock;
			m_pLock->lock();
		}
		void unlockManul(){
			m_pLock->unlock();
			m_pLock = 0;
		}
		~AutoLock()
		{
			if (m_pLock!=0){
				m_pLock->unlock();
				m_pLock = 0;
			}
		}
	private:
		Lock* m_pLock;
	};
}
