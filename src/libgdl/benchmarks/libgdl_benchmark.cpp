/**
 * @file libgdl_benchmark.hpp
 *
 * Declaring the Benchmaking framework for libGDL.
 */
#include "libgdl_benchmark.hpp"

#include <string>

#include <boost/program_options.hpp>

using namespace std;
using namespace libgdl;
using namespace libgdl::benchmark;
using namespace boost::program_options;

/**
 * USAGE : libgdl_benchmark
 *
 * Optional Parameters :
 * -h             : To print help.
 * -c             : To provide saved benchmark for comparison.
 * -o             : To save generate benchmark to file.
 * -m             : To run a specific module.
 * --verbose      : To print verbose information.
 * --version      : For version information.
 */
int main(int argc, char* argv[])
{
  // Declare the supported options.
  options_description desc("Allowed options");
  desc.add_options()
  ("help,h", "Description")
  ("output,o", value<std::string>(), "Save benchmarks")
  ("module,m", value<std::string>(), "Run a specific module")
  ("compare,c", value<std::string>(), "Comapre to saved benchmark")
  ("verbose", "To print all the benchmarking information.")
  ("version", "Print version info.")
  ;
  
  //! To save to output file
  bool saveToFile = false;
  //! Output filename
  string o_filename = "";
  //! To compare with saved benchmark
  bool compare= false;
  //! Filename of the saved benchmark
  string c_filename = "";
  //! To run specific module
  bool modToRun = false;
  //! Name of the specific module to run
  string mname = "";
  //! To print verbose information
  bool verbose = false;
  
  variables_map vm;
  store(command_line_parser(argc, argv).
        options(desc).run(), vm);
  notify(vm);
  
  if(vm.count("version"))
  {
    cout << util::GetVersion() << endl;
    return 0;
  }
  
  if (vm.count("help"))
  {
    std::string des = "USAGE : libgdl_benchamrk";
    des = des + "\nOptional Parameters :" +
                "\n-h             : To print help." +
                "\n-c             : To provide saved benchmark for comparison." +
                "\n-o             : To save generate benchmark to file." +
                "\n-m             : To run a specific module." +
                "\n--verbose      : To print verbose information." +
                "\n--version      : For version information";

    std::cout << des << std::endl;
    return 0;
  }
  
  if(vm.count("output")) 
  {
    saveToFile = true;
    o_filename = vm["output"].as<std::string>();
  }
  
  if(vm.count("module")) 
  {
    modToRun = true;
    mname = vm["module"].as<std::string>();
  }
  
  if(vm.count("compare")) 
  {
    compare = true;
    c_filename = vm["compare"].as<std::string>();
  }
  
  if(vm.count("verbose")) 
  {
    verbose = true;
  }
  
  GLOBAL_INFO << "Running benchmarks..." << endl;
  
  if(modToRun)
  {
    Handler::Instance().ModToRun(mname);
  }
  
  Handler::Instance().RunAllTests(verbose, 
                                  GLOBAL_LOG,
                                  compare,
                                  c_filename,
                                  saveToFile,
                                  o_filename);
  return 0;
}
  
