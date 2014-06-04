/**
 * @file kif_main.cpp
 * @author Sumedh Ghaisas
 *
 * main file for kif.
 */
#include <iostream>
#include <fstream>

#include <gdlparser/kif.hpp>

using namespace std;
using namespace gdlparser;

/**
 * USAGE : kif OUTPUT_FILENAME LIST_OF_SOURCE_FILES
 *
 * Sample usage
 * @command
 * kif test.kif 3puzzle.kif arithmatic.kif ...(more files if required) -g test.dot
 * @endcommand
 *
 * Parameters :
 * -g   : To generate DOT representation of dependency graph
 *
 */
int main(int argc, char* argv[])
{
    if(argc < 3 || std::string(argv[1]) == "-g" || std::string(argv[2]) == "-g")
    {
        std::cerr << "USAGE : kif OUTPUT_FILENAME LIST_OF_SOURCE_FILES " << std::endl;
        return 1;
    }

    std::vector<std::string> files;
    std::string output_filename = "";
    std::string graph_filename = "";
    bool toGraph = false;

    output_filename = std::string(argv[1]);
    for(int i = 2;i < argc;i++)
    {
        std::string temp(argv[i]);
        if(temp != "-g") files.push_back(std::string(argv[i]));
        else
        {
            toGraph = true;
            if(i + 1 != argc) graph_filename = std::string(argv[i+1]);
            break;
        }
    }

    KIF kif(output_filename, toGraph, std::cerr);
    for(size_t i = 0;i < files.size();i++) kif.AddFile(files[i]);
    kif.Parse();
    return 0;
}

