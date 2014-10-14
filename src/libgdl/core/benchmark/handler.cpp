/**
 * @file handler.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of Handler class.
 */
#include "handler.hpp"

#include <string>
#include <sstream>
#include <fstream>

#define BASH_RED "\033[0;31m"
#define BASH_GREEN "\033[0;32m"
#define BASH_CLEAR "\033[0m"
#define BASH_BLUE "\033[0;34m"

using namespace std;
using namespace libgdl::benchmark;

void Handler::RunAllTests(bool verbose,
                          Log log,
                          bool compare,
                          const std::string& c_file,
                          bool saveToFile,
                          const std::string& filename)

{
  if(compare)
  {
    compare = LoadCompareFile(c_file, log);
  }

  std::ofstream* out = NULL;
  if(saveToFile)
  {
    out = new std::ofstream(filename);
  }

  for(list<BaseTest*>::iterator it = tests.begin();it != tests.end();it++)
  {
    if(runOnlyModule)
    {
      if((*it)->ModuleName() != mod_name)
        continue;
    }

    log.Info << "Running bechmark " << (*it)->Name() << "... ";
    PMeasure pm = (*it)->Run();

    if(!verbose)
    {
      if(compare)
      {
        PMeasure cpm;
        if(!RetrieveObject((*it)->Name(), cpm))
        {
          log.Info << BASH_GREEN << "Avg. -- " << pm.avg << " microsecond(s)" <<
          BASH_CLEAR << endl;
        }
        else
        {
          int c = PMeasure::Compare(pm, cpm, (*it)->Tolerance());
          if(c == -1)
          {
            log.Info  << "Avg. -- " << BASH_GREEN << pm.avg << BASH_CLEAR<<
            " microsecond(s)" << " [" << BASH_BLUE << cpm.avg << BASH_CLEAR <<
            "]" << endl;
          }
          else if(c == 1)
          {
            log.Info << "Avg. -- " << BASH_RED << pm.avg << BASH_CLEAR <<
            " microsecond(s)" << " [" << BASH_BLUE << cpm.avg << BASH_CLEAR <<
            "]" << endl;
          }
          else
          {
            log.Info << "Avg. -- " << BASH_BLUE << pm.avg << BASH_CLEAR <<
            " microsecond(s)" << " [" << BASH_BLUE << cpm.avg << BASH_CLEAR <<
            "]" << endl;
          }
        }
      }
      else
      {
        log.Info << "Avg. -- " << BASH_GREEN << pm.avg << BASH_CLEAR <<
        " microsecond(s)" << endl;
      }
    }
    else
    {
      if(compare)
      {
        PMeasure cpm;
        if(!RetrieveObject((*it)->Name(), cpm))
        {
          log.Info << (*it)->Description() << endl;
          log.Info << "\tAvg.\t\tMin.\t\tMax" << endl;
          log.Info << "\t" << pm.avg << "\t\t" << pm.min << "\t\t" <<
          pm.max << endl;
        }
        else
        {
          int c = PMeasure::Compare(pm, cpm, (*it)->Tolerance());
          log.Info << (*it)->Description() << endl;
          log.Info << "\tAvg.\t\tMin.\t\tMax." << endl;
          if(c == -1)
          {
            log.Info << BASH_GREEN;
          }
          else if(c == 1)
          {
            log.Info << BASH_RED;
          }
          else
            log.Info << BASH_BLUE;

          log.Info << "\t" << pm.avg << "\t\t" << pm.min << "\t\t" << pm.max <<
          BASH_CLEAR << endl;
          log.Info << BASH_BLUE;
          log.Info << "\t[" << cpm.avg << "]\t\t[" << cpm.min << "]\t\t[" <<
          cpm.max << "]" << BASH_CLEAR << endl;
        }
      }
      else
      {
        log.Info << (*it)->Description() << endl;
        log.Info << "\tAvg.\t\tMin.\t\tMax" << endl;
        log.Info << "\t" << pm.avg << "\t\t" << pm.min << "\t\t" << pm.max << endl;
      }
    }

    if(saveToFile)
    {
      *out << (*it)->Name() << endl;
      *out << pm.avg << endl;
      *out << pm.max << endl;
      *out << pm.min << endl;
    }
  }

  if(saveToFile)
    out->close();

  delete out;
}

bool Handler::LoadCompareFile(const std::string& filename,
                              Log log)
{
  std::ifstream c_file(filename);

  if(!c_file.is_open())
  {
    log.Fatal << "Could not open file '" << filename << "' for comparison." << endl;
    return false;
  }

  std::string name;
  std::string savg;
  std::string smin;
  std::string smax;

  while(getline(c_file, name))
  {
    if(!getline(c_file, savg))
    {
      log.Fatal << "Error while parsing '" << filename << "'." << endl;
      return false;
    }

    if(!getline(c_file, smax))
    {
      log.Fatal << "Error while parsing '" << filename << "'." << endl;
      return false;
    }

    getline(c_file, smin);

    PMeasure pm(stod(smin, NULL), stod(smax, NULL), stod(savg, NULL));
    compare_map[name] = pm;
  }
  return true;
}

bool Handler::RetrieveObject(const std::string& name,
                             PMeasure& pm)
{
  map<std::string, PMeasure>::iterator it = compare_map.find(name);
  if(it == compare_map.end())
    return false;

  pm = it->second;
  return true;
}
