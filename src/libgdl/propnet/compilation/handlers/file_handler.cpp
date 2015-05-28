#include "file_handler.hpp"

#include <libgdl/core/thread_pool/thread_pool.hpp>

#include <libgdl/core/util/system_ni.hpp>

using namespace std;
using namespace libgdl::propnet;
using namespace libgdl::thread_pool;
using namespace libgdl::propnet::compilation::handlers;

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
    util::SystemNI::exec(stream.str());
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
  util::SystemNI::exec(stream.str());

  return true;
}
