/**
 * @file handler.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of Handler class.
 */
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
namespace benchmark /** Benchmarking framework of libGDL. **/
{

/**
 * This class stores all the benchmarks and provides simple API to run them.
 */
class Handler
{
 public:
  //! To add benchmarks in multiple files there should be one global instance
  //! of Handler to register them.
  //! This functions returns such a global instance of Handler.
  //!
  //! \return Handler&
  //!
  //!
  static Handler& Instance()
  {
    static Handler singleton;
    return singleton;
  }

  //! Empty constructor
  //!
  //!
  //!
  Handler()
    : runOnlyModule(false) {}

  //! Register a new benchmarking task to run
  //! Always returns NULL
  //!
  //! \param test New benchmarking task
  //! \return size_t*
  //!
  //!
  size_t* RegisterTest(BaseTest* test)
  {
    tests.push_back(test);
    return NULL;
  }

  //! Run all the benchmarks registered by applying the specific module
  //! condition if added beforehand.
  //! To add such a condition use method 'ModToRun' by passing the name of the
  //! module to run
  //!
  //! \param verbose To print verbose messages
  //! \param log Logging stream
  //! \param compare To compare against saved benchmarking results
  //! \param c_file Saved benchmark file
  //! \param saveToFile To save the results to file
  //! \param filename Filename to save the results
  //! \return void
  //!
  //!
  void RunAllTests(bool verbose = false,
                   Log log = GLOBAL_LOG,
                   bool compare = false,
                   const std::string& c_file = "",
                   bool saveToFile = false,
                   const std::string& filename = "");

  //! This function can be used to run specific modules of benchmarking.
  //! This function adds filter so that RunAllTests will run benchmarks inside
  //! given modules
  //!
  //! \param mname Name of to module to run
  //! \return void
  //!
  //!
  void ModToRun(const std::string& mname)
  {
    runOnlyModule = true;
    mod_name = mname;
  }

 private:
  //! This unction loads the benchmarking results for comparison
  //!
  //! \param filename Filename of the benchmarking result file
  //! \param log Logging stream for error reporting
  //! \return bool
  //!
  //!
  bool LoadCompareFile(const std::string& filename, Log log);

  //! Get PMeasure object associated with given saved benchmark
  //!
  //! \param name const std::string&
  //! \param pm PMeasure&
  //! \return bool
  //!
  //!
  bool RetrieveObject(const std::string& name, PMeasure& pm);

  //! List of registered benchmarking tasks
  std::list<BaseTest*> tests;
  //! Mapping of test name to PMeasure object for comparison
  std::map<std::string, PMeasure> compare_map;

  //! To run specific module
  bool runOnlyModule;
  //! Name of specific module to run
  std::string mod_name;
}; // class Handler

}; // namespace benchmark
}; // namespace libgdl


#endif // _LIBGDL_BENCHMARK_HANDLER_HPP_INCLUDED
