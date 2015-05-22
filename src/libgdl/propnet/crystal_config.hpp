#ifndef LIBGDL_PROPNET_CRYSTAL_INFO_HPP_INCLUDED
#define LIBGDL_PROPNET_CRYSTAL_INFO_HPP_INCLUDED

namespace libgdl
{
namespace propnet
{
namespace crystallization
{

struct CrystalConfig
{
  enum class Type { AND, OR, NOT, OR_UPDATE, AND_UPDATE};

  static signed short GetTypeInit(const Type& type)
  {
    if(type == Type::AND)
      return 0x8000;
    else if(type == Type::OR)
      return 0x7fff;
    else if(type == Type::NOT)
      return 0x0000;
    else if(type == Type::OR_UPDATE)
      return 0x7fff;

    return 0x0000;
  }

  static const signed short CrystalSignMask = 0x8000;

  static const signed short CrystalIncrementVal = 0x0001;

  static const signed short CrystalDecrementVal = 0xffff;
};

}
}
}

#endif // LIBGDL_PROPNET_CRYSTAL_INFO_HPP_INCLUDED
