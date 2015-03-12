#include "trace_handler.hpp"

#include <fstream>

using namespace std;
using namespace libgdl::util;

bool TraceHandler::LoadTrace(const std::string& filename)
{
  clear();

  ifstream t_file(filename);
  if(!t_file.is_open())
  {
    log.Fatal << LOGID << "Could not find trace file " << filename << endl;
    return false;
  }

	string line;
  while(getline(t_file, line))
  {
    push_back(line);
  }
  t_file.close();

  isInitialized = true;
}
