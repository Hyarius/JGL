#pragma once

#include "jgl_includes.h"

namespace jgl
{
	template<typename T>
	class Locked_queue
	{
	protected:
		std::mutex muxQueue;
		std::deque<T> _content;
		std::condition_variable cvBlocking;
		std::mutex muxBlocking;

	public:
		/*
			Default constructor
		*/
		Locked_queue() = default;

		/*
			Copy constructor deleted to deny duplication of data
		*/
		Locked_queue(const Locked_queue<T>&) = delete;

		/*
			Destructor
		*/
		virtual ~Locked_queue() { clear(); }

		/*
			Return the first element of the queue
		*/
		const T& front()
		{
			std::scoped_lock lock(muxQueue);
			return _content.front();
		}

		/*
			Return the last element of the queue
		*/
		const T& back()
		{
			std::scoped_lock lock(muxQueue);
			return _content.back();
		}

		/*
			Take out the first element of the queue
		*/
		T pop_front()
		{
			std::scoped_lock lock(muxQueue);
			auto t = std::move(_content.front());
			_content.pop_front();
			return t;
		}

		/*
			Take out the last element of the queue
		*/
		T pop_back()
		{
			std::scoped_lock lock(muxQueue);
			auto t = std::move(_content.back());
			_content.pop_back();
			return t;
		}

		/*
			Add an element at the end of the queue
		*/
		void push_back(const T& item)
		{
			std::scoped_lock lock(muxQueue);
			_content.emplace_back(std::move(item));

			std::unique_lock<std::mutex> ul(muxBlocking);
			cvBlocking.notify_one();
		}

		/*
			Add an element at the start of the queue
		*/
		void push_front(const T& item)
		{
			std::scoped_lock lock(muxQueue);
			_content.emplace_front(std::move(item));

			std::unique_lock<std::mutex> ul(muxBlocking);
			cvBlocking.notify_one();
		}

		/*
			Check if the queue is empty
		*/
		bool empty()
		{
			std::scoped_lock lock(muxQueue);
			return _content.empty();
		}

		/*
			Returns the number of element in the queue
		*/
		uint32_t size()
		{
			std::scoped_lock lock(muxQueue);
			return _content.size();
		}

		/*
			Clear the queue content
		*/
		void clear()
		{
			std::scoped_lock lock(muxQueue);
			_content.clear();
		}

		/*
			Wait for an element to pop-in
		*/
		void wait()
		{
			while (empty())
			{
				std::unique_lock<std::mutex> ul(muxBlocking);
				cvBlocking.wait(ul);
			}
		}
	};
}