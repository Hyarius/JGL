#include "jgl.h"

namespace jgl
{
    jgl::Size_t Thread::_nb_thread = 0;
    thread_local Stream cout("Main thread");

    Thread& Thread::operator=(Thread&& other) {
        _thread = std::move(other._thread);
        return *this;
    }

    std::thread*& Thread::operator*() {
        return _thread;
    }

    std::thread* const& Thread::operator*() const {
        return _thread;
    }

    std::thread::id Thread::get_id() const {
        return _thread->get_id();
    }

    void Thread::join() {
        if (_thread->joinable())
            _thread->join();
    }
}