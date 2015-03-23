#include "file_handler.hpp"

#include <libgdl/core/thread_pool/thread_pool.hpp>

using namespace std;
using namespace libgdl::propnet;
using namespace libgdl::thread_pool;

std::string FileHandler::exec(const std::string& cmd)
{
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    string result = "";
    while(!feof(pipe))
    {
      if(fgets(buffer, 128, pipe) != NULL)
        result += buffer;
    }
    pclose(pipe);
    cout << "Done command " << cmd << endl;
    return result;
}

bool FileHandler::GenerateSharedObject(const std::string& shared_object)
{
  //std::list<std::future<string> > results;
  //ThreadPool& gtp = ThreadPool::GetGlobalThreadPool();

  string com_prefix = "g++ -O3 -std=c++11 -fPIC -Isrc";

  for(auto it : p_files)
  {
    stringstream stream;
    stream << com_prefix << " -c " << it << ".cpp -o " << it << ".o";
    //results.emplace_back(gtp.Enqueue(exec, stream.str()));
    exec(stream.str());
  }

  //for(auto&& it : results)
  //{
    //it.get();
  //}

  std::stringstream stream;
  stream << "g++ -O3 -shared -fPIC";

  for(auto it : p_files)
  {
    stream << " " << it << ".o";
  }
  stream << " -o " << shared_object;
  exec(stream.str());

  return true;
}
