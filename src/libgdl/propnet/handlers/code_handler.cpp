#include "code_handler.hpp"

using namespace std;
using namespace libgdl::propnet;

void CodeHandler::GenerateCode()
{
  std::ofstream file("state_machine/" + name + ".cpp");

  FileHandler::GetMasterFileHandler().AddFile("state_machine/" + name);

  file << init_ss.str() << endl << endl;

  file << "extern \"C\" " << r_type << " " << name << sig << endl;

  file << "{" << endl;

  file << fun_init_ss.str() << endl;

  for(auto it : s_entries)
    file << it << endl;

  file << fun_deinit_ss.str() << endl;

  file << "}" << endl;

  file.close();
}
