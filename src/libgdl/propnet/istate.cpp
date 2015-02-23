#include "istate.hpp"

#include <iostream>
#include <cmath>

using namespace std;
using namespace libgdl;
using namespace libgdl::propnet;

size_t IState::AddProp(const string& str)
{
  auto it = base_m.find(str);
  if(it == base_m.end())
  {
    base_m[str] = c_b_no++;
    l_base.push_back(str);
    return c_b_no - 1;
  }

  return it->second;
}

void IState::CodeGen(ostream& file)
{
  size_t size_char = l_base.size() / 8;
  if(l_base.size() % 8 != 0)
  {
    size_char++;
  }

  //! Add header guards
  file << "#ifndef LIBGDL_STATE_HPP_INCLUDED" << endl;
  file << "#define LIBGDL_STATE_HPP_INCLUDED" << endl << endl;

  //! Add required includes
  file << "#include <iostream>" << endl;
  file << "#include <atomic>" << endl;
  file << "#include <boost/intrusive_ptr.hpp>" << endl << endl;

  file << "#include <libgdl/core/util/template_utils.hpp>" << endl << endl;

  //! Protect with namespace
  file << "namespace libgdl {" << endl << endl;

////////////////////////////////////////////////////////////////////////////////
/// Generate RawState
////////////////////////////////////////////////////////////////////////////////

  //! Define RawState
  file << "struct RawState {" << endl;

  //! Empty constructor
  file << "RawState() : s{} {};" << endl;

  //! Copy constructor
  file << "RawState(const RawState& state) {" << endl;
  for(size_t i = 0;i < size_char;i++)
  {
    file << "s[" << i << "] = state.s[" << i << "];" << endl;
  }
  file << "}" << endl << endl;

  //! Define character array of state
  file << "char s[" << size_char << "];" << endl;

  //! Add Get, Set and Reset function
  for(size_t i = 0;i < l_base.size();i++)
  {
    size_t buff = i / 8;
    size_t p = pow(2, i % 8);
    size_t q = 255 & ~p;

    file << endl;

    //! Get function
    file << "bool Get_" << i << "(bool& out) const {" << endl;
    file << "char temp = s[" << buff << "] & " << p << ";" << endl;
    file << "if(temp != 0) out = true;" << endl;
    file << "else out = false;" << endl;
    file << "return true;" << endl;
    file << "}" << endl;

    file << "template<typename T>" << endl;
    file << "bool Get_" << i << "(T& t, size_t N) const" << endl;
    file << "{" << endl;
    file << "char temp = s[" << buff << "] & " << p << ";" << endl << endl;

    file << "T temp2 = 1;" << endl;
    file << "temp2 = temp2 << N;" << endl << endl;

    file << "if(temp != 0)" << endl;
    file << "{" << endl;
    file << "t = t | temp2;" << endl;
    file << "}" << endl;
    file << "else" << endl;
    file << "{" << endl;
    file << "t = t & ~temp2;" << endl;
    file << "}" << endl << endl;

    file << "return true;" << endl;
    file << "}" << endl;

    //! Set function
    file << "bool Set_" << i << "(bool val) {" << endl;
    file << "if(val) s[" << buff << "] = s[" << buff << "] | " << p << ";" << endl;
    file << "else s[" << buff << "] = s[" << buff << "] & " << q << ";" << endl;
    file << "return true;" << endl;
    file << "}" << endl;

    file << "template<typename T>" << endl;
    file << "bool Set_" << i << "(const T& t, size_t N)" << endl;
    file << "{" << endl;
    file << "T temp = 1;" << endl;
    file << "temp = temp << N;" << endl << endl;

    file << "temp = temp & t;" << endl;
    file << "if(temp != 0) s[" << buff << "] = s[" << buff << "] | " << p << ";" << endl;
    file << "else s[" << buff << "] = s[" << buff << "] & " << q << ";" << endl;
    file << "return true;" << endl;
    file << "}" << endl;

    file << endl;
  }

  file << endl;

  //! Atomic Reference count
  file << "size_t ref_count() { return count; }" << endl;
  file << "std::atomic_size_t count;" << endl << endl;

  //! End of RawState
  file << "};" << endl << endl;

  file << "inline void intrusive_ptr_release(RawState* p) { " << endl;
  file << "if (--p->count == 0u) delete p;" << endl;
  file << "}" << endl << endl;

  file << "inline void intrusive_ptr_add_ref(RawState* p) {" << endl;
  file << "++p->count;" << endl;
  file << "}" << endl << endl;

  //! overload operator<< for RawState
  file << "std::ostream& operator<<(std::ostream& stream, const RawState& state) {" << endl;
  file << "bool temp = false;" << endl;
  file << "stream << \"State: \" << std::endl;" << endl;
  for(size_t i = 0;i < l_base.size();i++)
  {
    file << "state.Get_" << i << "(temp);" << endl;
    file << "if(temp) stream << \"\\t\" << \"" << l_base[i] << "\" << std::endl;" << endl;
  }
  file << "return stream;" << endl;
  file << "}" << endl;

////////////////////////////////////////////////////////////////////////////////
/// Generate State
////////////////////////////////////////////////////////////////////////////////

  //! Declare State
  file << "struct State : public boost::intrusive_ptr<RawState> {" << endl;

  //! Empty constructor
  file << "State() : boost::intrusive_ptr<RawState>(NULL) {}" << endl;

  //! Default constructor
  file << "State(RawState* state) : boost::intrusive_ptr<RawState>(state) {}" << endl << endl;

  //! Close function
  file << "State Clone() const {" << endl;
  file << "return State(new RawState(*get()));" << endl;
  file << "}" << endl << endl;

  file << "template<size_t N>" << std::endl;
  file << "State Clone() const {" << std::endl;
  file << "return State(new RawState(*get()));" << std::endl;
  file << "}" << endl << endl;

  //! Forward functions
  for(size_t i = 0;i < l_base.size();i++)
  {
    file << endl;

    //! Get function
    file << "inline bool Get_" << i << "(bool& out) const {" << endl;
    file << "return (*this)->Get_" << i << "(out);" << endl;
    file << "}" << endl;

    file << "template<typename T>" << endl;
    file << "inline bool Get_" << i << "(T& t, size_t N) const" << endl;
    file << "{" << endl;
    file << "return (*this)->Get_" << i << "(t, N);" << endl;
    file << "}" << endl;

    //! Set function
    file << "inline bool Set_" << i << "(bool val) {" << endl;
    file << "return (*this)->Set_" << i << "(val);" << endl;
    file << "}" << endl;

    file << "template<typename T>" << endl;
    file << "inline bool Set_" << i << "(const T& t, size_t N)" << endl;
    file << "{" << endl;
    file << "return (*this)->Set_" << i << "(t, N);" << endl;
    file << "}" << endl;

    file << endl;
  }

  file << "};" << endl;
  //! End if struct State

  file << "std::ostream& operator<<(std::ostream& stream, const State& s) {" << std::endl;
  file << "stream << *s;" << endl;
  file << "return stream;" << endl;
  file << "}" << endl << endl;

  //! Generate StateGroup
  file << "template<size_t N>" << endl;
  file << "struct StateGroup" << endl;
  file << "{" << endl;
  file << "template<typename... Args>" << endl;
  file << "StateGroup(Args... args)" << endl;
  file << ": states(args...) {}" << endl << endl;

  file << "template<size_t... Args>" << endl;
  file << "StateGroup(State s, core::template_utils::SeriesHolder<Args...>)" << endl;
  file << ": StateGroup(s.Clone<Args>()...) {}" << endl << endl;

  file << "StateGroup(State s)" << endl;
  file << ": StateGroup(s, typename core::template_utils::SeriesGenerator<N - 1>::type()) {}" << endl << endl;

  file << "template<typename... Args>" << endl;
  file << "void temp2(Args... args) const {}" << endl;

  //! Add Get, Set and Reset function
  for(size_t i = 0;i < l_base.size();i++)
  {
    file << endl;

    //! Get function
    file << "template<typename T, size_t... Args>" << endl;
    file << "void Get_" << i << "_caller(T& t, core::template_utils::SeriesHolder<Args...>) const" << endl;
    file << "{" << endl;
    file << "temp2(std::get<Args>(states)->Get_" << i << "(t, Args)...);" << endl;
    file << "}" << endl;
    file << "template<typename T>" << endl;
    file << "void Get_" << i << "(T& t) const" << endl;
    file << "{" << endl;
    file << "Get_" << i << "_caller(t, typename core::template_utils::SeriesGenerator<N -1>::type());" << endl;
    file << "}" << endl;

    file << endl;
  }

  file << "typename core::template_utils::NTupleGenerator<N, State>::type states;" << endl;
  file << "};" << endl << endl;

  //! End of namespace
  file << "}; // namespace libgdl" << endl;

  //! End of state.hpp
  file << "#endif // LIBGDL_STATE_HPP_INCLUDED" << endl;
}
