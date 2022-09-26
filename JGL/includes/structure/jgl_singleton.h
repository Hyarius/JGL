#pragma once

namespace jgl
{
	template <typename T>
	class Singleton
	{
		/*
		* Don't forget to add 'friend class Singleton<T>;' to T definition
		*/
	protected:
		static inline T* _instance = nullptr;

	public:
		static T* instanciate()
		{
			if (_instance == nullptr)
			{
				_instance = new T();
			}
			return (_instance);
		}
		static T* instance()
		{
			return (_instance);
		}
		static void release()
		{
			delete _instance;
		}
	};
}