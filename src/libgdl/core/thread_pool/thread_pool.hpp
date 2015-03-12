#ifndef _LIBGDL_CORE_THREAD_POOL_HPP_INCLUDED
#define _LIBGDL_CORE_THREAD_POOL_HPP_INCLUDED

#include <vector>
#include <future>
#include <thread>
#include <condition_variable>
#include <queue>
#include <functional>

namespace libgdl
{
namespace thread_pool
{


#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool
{
 public:
  ThreadPool(size_t);

  template<class F, class... Args>
  inline auto Enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>;

  ~ThreadPool();

  static ThreadPool& GetGlobalThreadPool()
  {
    static ThreadPool singleton(8);
    return singleton;
  }

  void Stop();

 private:
  std::vector<std::thread> workers;

  std::queue<std::function<void()>> tasks;

  std::mutex queue_mutex;
  std::condition_variable condition;
  bool stop;
};

}
}

#include "thread_pool_impl.hpp"

#endif // _LIBGDL_CORE_THREAD_POOL_HPP_INCLUDED
