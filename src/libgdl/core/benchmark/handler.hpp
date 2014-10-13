#ifndef _LIBGDL_BENCHMARK_HANDLER_HPP_INCLUDED
#define _LIBGDL_BENCHMARK_HANDLER_HPP_INCLUDED

#include <list>

#include "base_test.hpp"

namespace libgdl
{
namespace benchmark
{

class Handler
{
 public:
  static Handler& Instance()
  {
    static Handler singleton;
    return singleton;
  }

  size_t* RegisterTest(BaseTest* test)
  {
    tests.push_back(test);
    return NULL;
  }

  void RunAllTests()
  {
    for(std::list<BaseTest*>::iterator it = tests.begin();it != tests.end();it++)
    {
      (*it)->Run();
    }
  }

 private:
  std::list<BaseTest*> tests;
};

}
}


#endif // _LIBGDL_BENCHMARK_HANDLER_HPP_INCLUDED
