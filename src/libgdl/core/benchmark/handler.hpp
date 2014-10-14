#ifndef _LIBGDL_BENCHMARK_HANDLER_HPP_INCLUDED
#define _LIBGDL_BENCHMARK_HANDLER_HPP_INCLUDED

#include <list>
#include <string>
#include <sstream>

#include <libgdl/core/util/log.hpp>

#include "base_test.hpp"
#include "pmeasure.hpp"

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

  void RunAllTests(bool verbose = false,
                   const Log& log = GLOBAL_LOG,
                   bool saveToFile = false,
                   const std::string& filename = "");

 private:
  std::list<BaseTest*> tests;
};

}
}


#endif // _LIBGDL_BENCHMARK_HANDLER_HPP_INCLUDED
