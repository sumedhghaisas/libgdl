#ifndef LIBGDL_PLAYER_DEFAULT_PLAYER_CONFIG_HPP_INCLUDED
#define LIBGDL_PLAYER_DEFAULT_PLAYER_CONFIG_HPP_INCLUDED

#include <libgdl/core.hpp>
#include <libgdl/core/util/preprocessor.hpp>
#include <libgdl/core/util/sfinae_utility.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/program_options.hpp>

namespace libgdl
{
namespace player
{

template<typename T>
struct ContainsExtraPrintFunction
{
  typedef char Yes;
  typedef Yes No[2];

  template <typename U, U> struct has;

  template <typename Fun>
  static Yes& fun_test(has<void (Fun::*)() const, &Fun::ExtraPrint>*);

  template<typename>
  static No& fun_test(...);

  static const bool value = (sizeof(fun_test<T>(0)) == sizeof(Yes));
};

#define LIBGDL_PLAYER_CONFIG_PARAMS LIBGDL_PP_REFLECTABLE

struct DefaultPlayerConfig
{
  LIBGDL_PLAYER_CONFIG_PARAMS
  (
    (size_t)(SelectMove_safety_time)(= 500000),
    (size_t)(MetaGame_safety_time)(= 500000)
  )

  void Configure(int argc, char* argv[])
  {
    boost::program_options::options_description desc("Allowed options");
    AddOptions(desc);
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).
        options(desc).run(), vm);
    boost::program_options::notify(vm);
  }

  virtual void AddOptions(boost::program_options::options_description& desc)
  {
    using namespace boost::program_options;

    desc.add_options()
    ("SelectMove-safety-time", value<size_t>(), "Set select move safety time.")
    ("MetaGame-safety-time", value<size_t>(), "Set meta game safety time.")
    ;
  }

  size_t SelectMoveSafetyTime()
  {
    return SelectMove_safety_time;
  }

  size_t MetaGameSafetyTime()
  {
    return MetaGame_safety_time;
  }

  virtual void PrintConfig() const
  {
    sfinae::reflection::Reflector::PrintFields(*this, *this);
  }

  template<typename T>
  void PrintField(const std::string& name, const T& t)
  {
    std::cout << name << " = " << t << std::endl;
  }

  static DefaultPlayerConfig& Global()
  {
    static DefaultPlayerConfig singleton;
    return singleton;
  }

  boost::program_options::variables_map vm;
};

}
}

#define LIBGDL_PLAYER_CONFIG(SUPERCLASS, ...)                                 \
struct PlayerConfig : public SUPERCLASS                                       \
{                                                                             \
  typedef SUPERCLASS SuperClass;                                              \
                                                                              \
  __VA_ARGS__                                                                 \
                                                                              \
  template<typename C>                                                        \
  void ExtraPrintCaller(const C* c, typename boost::enable_if_c<libgdl::player::ContainsExtraPrintFunction<C>::value>::type* = 0) const\
  {                                                                           \
    c->ExtraPrint();\
  }                                                                           \
  template<typename C>                                                        \
  void ExtraPrintCaller(const C* c, typename boost::disable_if<libgdl::player::ContainsExtraPrintFunction<C>>::type* = 0) const\
  {}                                                                          \
                                                                              \
  void PrintConfig() const                                                    \
  {                                                                           \
    ExtraPrintCaller(this);                                                   \
    sfinae::reflection::Reflector::PrintFields(*this, *this);                 \
    SuperClass::PrintConfig();                                                \
  }                                                                           \
                                                                              \
  static PlayerConfig& Global()                                               \
  {                                                                           \
    static PlayerConfig singleton;                                            \
    return singleton;                                                         \
  }                                                                           \
};

#define LIBGDL_PLAYER_CONFIG_CL_PARAMS(...)                                   \
virtual void AddOptions(boost::program_options::options_description& desc)    \
{                                                                             \
  SuperClass.AddOptions(desc);                                                \
  desc.add_options()                                                          \
  __VA_ARGS__                                                                 \
}

#endif // LIBGDL_PLAYER_DEFAULT_PLAYER_CONFIG_HPP_INCLUDED
