#include "handler.hpp"

#include <string>
#include <sstream>

#define BASH_GREEN "\033[0;32m"
#define BASH_CLEAR "\033[0m"

using namespace std;
using namespace libgdl::benchmark;

void Handler::RunAllTests(bool verbose,
                          Log log,
                          bool saveAsXML,
                          const std::string& filename)

{
  for(list<BaseTest*>::iterator it = tests.begin();it != tests.end();it++)
  {
    log.Info << "Running bechmark " << (*it)->Name() << "... ";
    PMeasure pm = (*it)->Run();
    if(!verbose)
    {
      log.Info << BASH_GREEN << "Avg. -- " << pm.avg << " microsecond(s)" <<
      BASH_CLEAR << endl;
    }
    else
    {
      log.Info << endl;
      log.Info << "\tAvg.\tMin.\tMax" << endl;
      log.Info << "\t" << pm.avg << "\t" << pm.min << "\t" << pm.max << endl;
    }
  }
}
