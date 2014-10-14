/**
 * @file base_test.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of BaseTest class.
 */
#ifndef _LIBGDL_BENCHMARK_BASE_TEST_HPP_INCLUDED
#define _LIBGDL_BENCHMARK_BASE_TEST_HPP_INCLUDED

#include <string>

#include "pmeasure.hpp"

namespace libgdl
{
namespace benchmark
{

/**
 * This class acts as the parent class for all the benchmarks. Each benchmark
 * is implemented as a new child class of this class. Every benchmark class
 * must implement run function which returns the associated measures.
 */
class BaseTest
{
 public:
  //! Constructs object with given module name, name, description, number of
  //! iterations and tolerance
  //!
  //! \param mname Module name of the benchmark
  //! \param name Name of the benchmark
  //! \param desc Description of the benchmark
  //! \param iterations Number of iterations to perform
  //! \param tolerance Tolerance while comparing the result with saved result
  //!
  //!
  BaseTest(const std::string& mname,
           const std::string& name,
           const std::string& desc,
           const size_t iterations,
           double tolerance)
    : mname(mname),
    name(name),
    desc(desc),
    iterations(iterations),
    tolerance(tolerance)
  {}

  //! Run this benchamrk
  //!
  //! \return PMeasure
  //!
  //!
  virtual PMeasure Run() = 0;

  //! Get name of this benchmark
  const std::string& Name() const
  {
    return name;
  }

  //! Get description of this benchmark
  const std::string& Description() const
  {
    return desc;
  }

  //! Get module name of this benchmark
  const std::string& ModuleName() const
  {
    return mname;
  }

  //! Get tolerance associated with this benchmark
  double Tolerance() const
  {
    return tolerance;
  }

 protected:
  //! Module name of the benchmark
  std::string mname;
  //! Name of the benchmark
  std::string name;
  //! Description of the benchmark
  std::string desc;
  //! Number of iterations to perform
  size_t iterations;
  //! Tolerance associated with this benchmark
  double tolerance;
}; // class BaseTest

}; // namespace benchmark
}; // namespace libgdl


#endif // _LIBGDL_BENCHMARK_BASE_TEST_HPP_INCLUDED
