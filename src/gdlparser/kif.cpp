/**
 * @file kif.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of KIF class.
 */
#include "kif.hpp"

// Color code escape sequences
#define BASH_RED "\033[0;31m"
#define BASH_YELLOW "\033[0;33m"
#define BASH_CLEAR "\033[0m"

using namespace gdlparser;

KIF::~KIF()
{
    for(std::map<std::string, DGraphNode*>::iterator it = dgraph.begin();it != dgraph.end();it++)
        delete it->second;
}

bool KIF::PrintDependencyGraph(const std::string& filename) const
{
    std::ofstream graph(filename.c_str());
    if(!graph.is_open())
    {
        *stream << BASH_RED "[ERROR] " BASH_CLEAR;
        std::cout << "Unable to open file " << filename << " for saving graph" << std::endl;
        return false;
    }

    graph << "digraph dependency_graph {" << std::endl;

    for(std::map<std::string, DGraphNode*>::const_iterator it = dgraph.begin(); it != dgraph.end(); it++)
    {
        graph << it->first << " [label = \"" << it->first << "\"];" << std::endl;
    }

    for(std::map<std::string, DGraphNode*>::const_iterator it = dgraph.begin(); it != dgraph.end(); it++)
    {
        const std::vector<DGraphNode*>& out = (it->second)->out;
        const std::vector<bool>& isNot = (it->second)->isNot;

        std::set<std::string> str_set;
        std::set<std::string>::iterator sit;
        for(size_t i = 0; i < out.size(); i++)
        {
            if((sit = str_set.find(out[i]->name)) == str_set.end())
            {
                if(isNot[i] == true) graph << it->first << " -> " << out[i]->name << " [color = red];" << std::endl;
                else graph << it->first << " -> " << out[i]->name << ";" << std::endl;
                str_set.insert(out[i]->name);
            }
        }
    }

    graph << "}";

    graph.close();

    return true;
}

bool KIF::PrintToFile(const std::string& filename) const
{
    std::ofstream out(filename.c_str());
    if(!out.is_open())
    {
        *stream << BASH_RED "[ERROR] " BASH_CLEAR;
        std::cout << "Unable to open file " << filename << std::endl;
        return false;
    }

    for(size_t i = 0; i < facts.size(); i++) out << facts[i] << std::endl;
    for(size_t i = 0; i < clauses.size(); i++) out << clauses[i] << std::endl;

    out.close();
    return true;
}
