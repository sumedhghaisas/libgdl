/**
 * @file kif_flattener_main.cpp
 * @author Sumedh Ghaisas
 *
 * main file for KIF flattener.
 */
#include <iostream>
#include <fstream>

#include <libgdl/core.hpp>
#include <libgdl/gdlparser/kif.hpp>
#include <libgdl/gdlreasoner/kif_flattener.hpp>
#include <boost/program_options.hpp>

using namespace std;
using namespace libgdl;
using namespace boost::program_options;
using namespace libgdl::gdlparser;
using namespace libgdl::gdlreasoner;

/**
 * USAGE : flatten OUTPUT_FILENAME -c LIST_OF_SOURCE_FILES
 *
 * Sample usage
 * @command
 * flatten test.kif -c 3puzzle.kif arithmatic.kif ...(more files if required)
 * @endcommand
 *
 * Parameters :
 * -w off	      : will disable all the warnings (by default warnings are enabled)
 * -h[--help]   : Prints help.
 */
int main(int argc, char* argv[])
{
    // Declare the supported options.
    options_description desc("Allowed options");
    desc.add_options()
    ("help,h", "Description")
    ("enable-warnings,w", value<std::string>()->default_value("ON"), "enable/disable warnings")
    ("output-file", value<std::string>(), "Output filename")
    ("source-files,c", value<std::vector<std::string> >()->multitoken(), "source files");
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

    // to generate warnings
    bool warn = true;

    if (vm.count("help"))
    {
        std::string des = "USAGE : flatten OUTPUT_FILENAME -c LIST_OF_SOURCE_FILES";
        des = des +
                "\nSample usage" +
                "\nflatten test.kif -c 3puzzle.kif arithmatic.kif ...(more files if required)" +
                "\nOptional Parameters :" +
                "\n-w off       : will disable all the warnings (by default warnings are enabled" +
                "\n-h[--help]   : Prints help.";

        std::cerr << des << std::endl;
        return 0;
    }

    if(vm.count("output-file")) output_filename = vm["output-file"].as<std::string>();
    else
    {
        std::cerr << "ERROR: Output file has to be mentioned." << std::endl;
        std::cerr << "USAGE : flatten OUTPUT_FILE -c SOURCE_FILES" << std::endl;
        return 1;
    }

    if(vm.count("source-files"))
    {
        files = vm["source-files"].as<std::vector<std::string> >();
    }
    else
    {
        std::cerr << "ERROR: at least one source file has to be mentioned." << std::endl;
        std::cerr << "USAGE : flatten OUTPUT_FILE -c SOURCE_FILES" << std::endl;
        return 1;
    }

    if(vm.count("enable-warnings"))
    {
        std::string temp = vm["enable-warnings"].as<std::string>();
        if(temp == "OFF" || temp == "off" || temp == "Off") warn = false;
    }

    KIF kif(warn, 1, std::cout);
    for(size_t i = 0;i < files.size();i++)
      if(!kif.AddFile(files[i]))
      {
        cerr << "Unable to open file " << files[i] << endl;
      }
    if(!kif.Parse()) return 1;

    KIFFlattener kf;
    kf.Flatten(kif);
    kf.PrintToFile(output_filename);
}
