#ifndef LIBGDL_PLAYER_DEFAULT_PLAYER_CONFIG_HPP_INCLUDED
#define LIBGDL_PLAYER_DEFAULT_PLAYER_CONFIG_HPP_INCLUDED

#include <libgdl/core.hpp>
#include <libgdl/core/util/preprocessor.hpp>
#include <libgdl/core/util/sfinae_utility.hpp>
#include <boost/utility/enable_if.hpp>

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

#endif // LIBGDL_PLAYER_DEFAULT_PLAYER_CONFIG_HPP_INCLUDED
