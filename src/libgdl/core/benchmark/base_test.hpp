#ifndef _LIBGDL_BENCHMARK_BASE_TEST_HPP_INCLUDED
#define _LIBGDL_BENCHMARK_BASE_TEST_HPP_INCLUDED

#include <string>

#include "pmeasure.hpp"

namespace libgdl
{
namespace benchmark
{

class BaseTest
{
 public:
  BaseTest(const std::string& mname,
           const std::string& name,
           const std::string& desc,
           const size_t iterations)
    : mname(mname),
    name(name),
    desc(desc),
    iterations(iterations)
  {}

  virtual PMeasure Run() = 0;

  const std::string& Name()
  {
    return name;
  }
  const std::string& Description()
  {
    return desc;
  }

 protected:
  std::string mname;
  std::string name;
  std::string desc;
  size_t iterations;
};

}
}


#endif // _LIBGDL_BENCHMARK_BASE_TEST_HPP_INCLUDED
