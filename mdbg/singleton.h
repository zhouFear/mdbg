#ifndef singleton_h__
#define singleton_h__
#include <mutex>

template <typename T>
class SingletonT {
private:
	SingletonT() {};
	~SingletonT() {};

public:
	static T* GetInstance();
private:
	static T* pInstance;
	static std::mutex m_mutex;
};

template <typename T>
T* SingletonT<T>::GetInstance()
{
	if (!pInstance)
	{
		std::unique_lock(m_mutex);
		if (!pInstance)
		{
			pInstance = new T();
		}
	}
	return T;
};

template <typename T>
T* SingletonT<T>::pInstance = nullptr;

template <typename T>
std::mutex SingletonT<T>::m_mutex;
#endif // singleton_h__
