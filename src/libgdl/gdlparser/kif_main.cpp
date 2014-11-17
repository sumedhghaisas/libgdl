/**
 * @file kif_main.cpp
 * @author Sumedh Ghaisas
 *
 * main file for kif.
 */
#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>

#include <libgdl/core.hpp>
#include <libgdl/gdlparser/kif.hpp>
#include <libgdl/reasoners/gdlreasoner/kif_flattener.hpp>

using namespace std;
using namespace boost::program_options;

using namespace libgdl;
using namespace libgdl::gdlparser;
using namespace libgdl::gdlreasoner;

/**
 * USAGE : kif OUTPUT_FILENAME -c LIST_OF_SOURCE_FILES
 *
 * Sample usage
 * @command
 * kif test.kif -c 3puzzle.kif ...(more files if required) -g test.dot
 * @endcommand
 *
 * Parameters :
 * -g   	        : To generate DOT representation of dependency graph
 * -w off	        : will disable all the warnings (by default warnings are enabled)
 * -f[--flatten]  : Flattens the KIF before saving
 * -h             : Prints help.
 * --version      : For version
 */
int main(int argc, char* argv[])
{
  // Declare the supported options.
  options_description desc("Allowed options");
  desc.add_options()
  ("help,h", "Description")
  ("graph-filename,g", value<std::string>(), "graph output filename")
  ("enable-warnings,w", value<std::string>()->default_value("ON"), 
                        "enable/disable warnings")
  ("output-file,o", value<std::string>(), "Output filename")
  ("source-files,c", value<std::vector<std::string> >()->multitoken(), 
                     "source files")
  ("flatten,f", "Flatten the knowledge")
  ("version", "Print version info.")
  ("O0", "Optimization level 0")
  ("O1", "Optmization level 1")
  ;

  positional_options_description p;
  p.add("output-file", -1);

  variables_map vm;
  store(command_line_parser(argc, argv).
        options(desc).positional(p).run(), vm);
  notify(vm);

  // source files
  std::vector<std::string> files;
  // output filename
  std::string output_filename = "";
  // graph output filename
  std::string graph_filename = "";
  // to generate graph
  bool toGraph = false;
  // to generate warnings
  bool warn = true;
  //! to flatten
  bool isFlatten = false;
  //! opt level
  size_t o_level = 1;
  
  if(vm.count("version"))
  {
    cout << util::GetVersion() << endl;
    return 0;
  }

  if (vm.count("help"))
  {
    std::string des = "USAGE : kif OUTPUT_FILENAME -c LIST_OF_SOURCE_FILES";
    des = des + "\nSample usage" +
                "\nkif test.kif -c 3puzzle.kif arithmatic.kif ...(more files if required) -g test.dot" +
                "\nOptional Parameters :" +
                "\n-g             : To generate DOT representation of dependency graph" +
                "\n-w off         : Will disable all the warnings (by default warnings are enabled" +
                "\n-f[--flatten]  : To output flattened KIF" +
                "\n-h[--help]     : Prints help." +
                "\n--version      : For version information" +
                "\n--O1           : Enable premiss reordering(Enabled by default)." +
                "\n--O0           : Disable premiss reordering.";

    std::cout << des << std::endl;
    return 0;
  }

  if(vm.count("output-file")) output_filename = vm["output-file"].as<std::string>();
  else
  {
    std::cerr << "ERROR: Output file has to be mentioned." << std::endl;
    std::cerr << "USAGE : kif OUTPUT_FILE -c SOURCE_FILES" << std::endl;
    return 1;
  }

  if(vm.count("source-files"))
  {
    files = vm["source-files"].as<std::vector<std::string> >();
  }
  else
  {
    std::cerr << "ERROR: at least one source file has to be mentioned." << std::endl;
    std::cerr << "USAGE : kif OUTPUT_FILE -c SOURCE_FILES" << std::endl;
    return 1;
  }

  if (vm.count("graph-filename"))
  {
    toGraph = true;
    graph_filename = vm["graph-filename"].as<std::string>();
  }

  if(vm.count("enable-warnings"))
  {
    std::string temp = vm["enable-warnings"].as<std::string>();
    if(temp == "OFF" || temp == "off" || temp == "Off") warn = false;
  }
  
  if(vm.count("flatten"))
  {
    isFlatten = true;
  }
  
  if(vm.count("O0"))
  {
    o_level = 0;
  }

  KIF kif(warn, o_level, Log(std::cerr, false, false));
  for(size_t i = 0;i < files.size();i++)
    if(!kif.AddFile(files[i]))
    {
      cerr << "Unable to open file " << files[i] << endl;
    }
  
  if(!kif.Parse()) return 1;
  
  if(toGraph)
    kif.PrintDependencyGraph(graph_filename);
    
  if(isFlatten)
  {
    KIFFlattener kf;
    kf.Flatten(kif);
    kf.PrintToFile(output_filename);
  }
  else kif.PrintToFile(output_filename);
  
  return 0;
}


