/**
 * @file system_setup.hpp.hpp
 * @author Sumedh Ghaisas
 *
 * For cross platform dependencies.
 */

#ifndef LIBGDL_CORE_UTIL_SYSTEM_SETUP_HPP_INCLUDED
#define LIBGDL_CORE_UTIL_SYSTEM_SETUP_HPP_INCLUDED

//! Visual studio still does not support 'noexcept' qualifier
#ifdef _MSC_VER

#ifdef noexcept
#undef noexcept
#endif // noexcept
#define noexcept

#endif // _MSC_VER

//! 'register' storage class is deprecated from modern c++ compilers
#ifdef register
  #undef register
#endif // register

#define register

#endif // LIBGDL_CORE_UTIL_SYSTEM_SETUP_HPP_INCLUDED
