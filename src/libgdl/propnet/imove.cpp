#include "imove.hpp"

using namespace std;
using namespace libgdl::propnet;

void IMove::CodeGen(std::ostream& file)
{
  //! Add header guards
  file << "#ifndef LIBGDL_MOVE_HPP_INCLUDED" << endl;
  file << "#define LIBGDL_MOVE_HPP_INCLUDED" << endl << endl;

  //! Add required includes
  file << "#include <iostream>" << endl;
  file << "#include <atomic>" << endl;
  file << "#include <boost/intrusive_ptr.hpp>" << endl << endl;

  //! Put Move inside namespace libgdl
  file << "namespace libgdl" << endl;
  file << "{" << endl;

////////////////////////////////////////////////////////////////////////////////
/// Create RawMove
////////////////////////////////////////////////////////////////////////////////

  //! Create a raw object to be stored inside a main object, which will act as
  //! intrusive ptr
  file << "struct RawMove" << endl;
  file << "{" << endl;

  //! Create constructors
  file << "RawMove() : moves{} {}" << endl;

  file << "RawMove(const RawMove& rm)" << endl;
  file << "{" << endl;
  file << "for(size_t i = 0;i < " << map_list.size() << ";i++)" << endl;
  file << "{ moves[i] = rm.moves[i]; }" << endl;
  file << "}" << endl;

  file << "template<typename... Args>" << endl;
  file << "RawMove(Args... args)" << endl;
  file << " : moves{args...} {}" << endl << endl;

  //! Create get-set methods
  {
    size_t index = 0;
    for(auto it : map_list)
    {
      for(auto it2 : it)
      {
        file << "bool Get_" << index << "_" << it2.second << "(bool& out) const" << endl;
        file << "{" << endl;
        file << "if(moves[" << index << "] == " << it2.second << ") out = true;" << endl;
        file << "else out = false;" << endl;
        file << "return true;" << endl;
        file << "}" << endl;
      }
      index++;
    }
  }
  file << "size_t GetMove(size_t r_id) const" << endl;
  file << "{" << endl;
  file << "return moves[r_id];" << endl;
  file << "}" << endl << endl;

  file << "void SetMove(size_t r_id, size_t m_id)" << endl;
  file << "{" << endl;
  file << "moves[r_id] = m_id;" << endl;
  file << "}" << endl << endl;

  //! Create a local array which holds the move ids
  file << "size_t moves[" << map_list.size() << "];" << endl << endl;

  //! Create atomic Reference count for the raw object
  file << "size_t ref_count() { return count; }" << endl;
  file << "std::atomic_size_t count;" << endl << endl;

  //! end of RawMove
  file << "};" << endl << endl;

  //! Intrusive pointer increment decrement functions
  file << "inline void intrusive_ptr_release(RawMove* p) { " << endl;
  file << "if (--p->count == 0u) delete p;" << endl;
  file << "}" << endl << endl;

  file << "inline void intrusive_ptr_add_ref(RawMove* p) {" << endl;
  file << "++p->count;" << endl;
  file << "}" << endl << endl;

  //! Overload operator<< for RawMove
  file << "std::ostream& operator<<(std::ostream& stream, const RawMove& move)" << endl;
  file << "{" << endl;
  file << "stream << \"Move: \" << std::endl;" << endl;

  size_t index = 0;
  for(auto it : map_list)
  {
    Map::iterator it2 = it.begin();
    file << "if(move.moves[" << index << "] == " << it2->second << ") stream << \"\\t" << it2->first << "\" << std::endl;" << endl;
    for(it2 = it2;it2 != it.end();it2++)
    {
      file << "else if(move.moves[" << index << "] == " << it2->second << ") stream << \"\\t" << it2->first << "\" << std::endl;" << endl;
    }
    index++;
  }

  file << "return stream;" << endl;
  file << "}" << endl << endl;

////////////////////////////////////////////////////////////////////////////////
/// Create Move
////////////////////////////////////////////////////////////////////////////////

  //! Inherit boost::intrusive_ptr for basic functionality
  file << "struct Move : public boost::intrusive_ptr<RawMove>" << endl;
  file << "{" << endl;

  //! Default empty constructor
  file << "Move(RawMove* rm = NULL) : boost::intrusive_ptr<RawMove>(rm) {}" << endl << endl;

  //! Cloning function(Deep Copy)
  file << "Move Clone() const" << endl;
  file << "{" << endl;
  file << "return Move(new RawMove(*get()));" << endl;
  file << "}" << endl << endl;

  //! Redirection functions
  file << "size_t GetMove(size_t r_id) const" << endl;
  file << "{" << endl;
  file << "return get()->GetMove(r_id);" << endl;
  file << "}" << endl;

  file << "void SetMove(size_t r_id, size_t m_id)" << endl;
  file << "{" << endl;
  file << "get()->SetMove(r_id, m_id);" << endl;
  file << "}" << endl << endl;

  //! end of struct Move
  file << "};" << endl << endl;

  //! operator<< overload for Move
  file << "std::ostream& operator<<(std::ostream& stream, const Move& m)" << endl;
  file << "{" << endl;
  file << "stream << *m;" << endl;
  file << "return stream;" << endl;
  file << "}" << endl << endl;

  file << "} //namespace libgdl" << endl;

  file << "#endif //LIBGDL_MOVE_HPP_INCLUDED" << endl;
}
