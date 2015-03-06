#include "code_handler.hpp"

using namespace std;
using namespace libgdl::propnet;

void CodeHandler::GenerateCode()
{
  std::ofstream file("state_machine/" + name + ".cpp");

  FileHandler::GetMasterFileHandler().AddFile("state_machine/" + name);

  stringstream f_stream;

  stringstream h_stream;

  file << init_ss.str() << endl << endl;

  f_stream << "extern \"C\" " << r_type << " " << name << sig << endl;

  f_stream << "{" << endl;

  f_stream << fun_init_ss.str() << endl;

  auto it = s_entries.begin();
  size_t num_entries = 0;
  bool isHelp = false;
  for(it = it;it != s_entries.end();it++)
  {
    if(num_entries > limit)
    {
      isHelp = true;
      break;
    }
    f_stream << *it << endl;
    num_entries++;
  }

  if(isHelp)
  {
    helper = new CodeHandler("void", name + "_helper", helper_sig, helper_sig, helper_call);
    helper->init_ss << init_ss.str();

    f_stream << name << "_helper" << helper_call << ";" << endl;

    h_stream << "extern \"C\" void " << name << "_helper" << helper_sig << ";" << endl;

    for(it = it;it != s_entries.end();it++)
    {
      helper->AddEntry(*it);
    }

    helper->GenerateCode();
  }

  f_stream << fun_deinit_ss.str() << endl;

  f_stream << "}" << endl;

  file << h_stream.str() << endl;

  file << f_stream.str() << endl;

  file.close();
}
