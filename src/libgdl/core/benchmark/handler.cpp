#include "handler.hpp"

#include <string>
#include <sstream>

using namespace std;
using namespace libgdl::benchmark;

void Handler::RunAllTests(bool verbose,
                          const Log& log,
                          bool saveToFile,
                          const std::string& filename)

{
  for(list<BaseTest*>::iterator it = tests.begin();it != tests.end();it++)
  {
    PMeasure pm = (*it)->Run();
  }
}
