#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

namespace Aeth::Utils::StlExtensions
{
    template <typename T>
    class SharedQueue
    {
    public:
        template <class _Rep, class _Period>
        bool front(std::chrono::duration<_Rep, _Period> _timeout, T& _out)
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            if (queue_.empty())
                cond_.wait_for<_Rep, _Period>(mlock, _timeout);
            if (queue_.empty())
                return true;
            _out = queue_.front();
            return false;
        }
        template <class _Rep, class _Period>
        bool pop(std::chrono::duration<_Rep, _Period> _timeout)
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            if (queue_.empty())
                cond_.wait_for<_Rep, _Period>(mlock, _timeout);
            if (queue_.empty())
                return true;
            queue_.pop_front();
            return false;
        }

        void push(const T& item)
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            queue_.push_back(item);
            mlock.unlock();     // unlock before notificiation to minimize mutex con
            cond_.notify_one(); // notify one waiting thread
        }
        void push(T&& item)
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            queue_.push_back(std::move(item));
            mlock.unlock();     // unlock before notificiation to minimize mutex con
            cond_.notify_one(); // notify one waiting thread
        }

        size_t size()
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            size_t size = queue_.size();
            mlock.unlock();
            return size;
        }

        bool empty()
        {
            return size() == 0;
        }

        void clear()
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            queue_.clear();
            mlock.unlock();
        }

    private:
        std::deque<T> queue_;
        std::mutex mutex_;
        std::condition_variable cond_;
    };
}
