#ifndef _LIBGDL_BENCHMARK_HANDLER_HPP_INCLUDED
#define _LIBGDL_BENCHMARK_HANDLER_HPP_INCLUDED

#include <list>
#include <string>
#include <sstream>
#include <map>

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
                   Log log = GLOBAL_LOG,
                   bool comapre = false,
                   const std::string& c_file = "",
                   bool saveToFile = false,
                   const std::string& filename = "");

 private:
  bool LoadCompareFile(const std::string& filename, Log log);

  bool RetrieveObject(const std::string& name, PMeasure& pm);

  std::list<BaseTest*> tests;
  std::map<std::string, PMeasure> compare_map;
};

}
}


#endif // _LIBGDL_BENCHMARK_HANDLER_HPP_INCLUDED
