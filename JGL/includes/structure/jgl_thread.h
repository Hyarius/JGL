#pragma once

#include "jgl_includes.h"
#include "jgl_basic_types.h"
#include "jgl_string.h"
#include "jgl_stream.h"

namespace jgl
{
	extern thread_local Stream cout;

	class Thread
	{
	private:
		std::thread* _thread;
		static jgl::Size_t _nb_thread;

	public:
		static jgl::Size_t nb_thread() { return (_nb_thread); }

		template< class Function, class... Args>
		Thread(jgl::String p_name, Function&& p_funct, Args&&... p_argv)
		{
			_nb_thread++;
			_thread = new std::thread(
				[&](jgl::String p_name, Function&& p_funct, Args&&... p_argv)
				{
					cout = Stream(p_name);
					p_funct(p_argv...);
				},
				p_name, p_funct, std::forward<Args>(p_argv)...);
		}

		~Thread()
		{
			_nb_thread--;
		}

		Thread& operator=(Thread&& other);
		std::thread*& operator*();
		std::thread* const& operator*() const;
		std::thread::id get_id() const;
		void join();
	};
}