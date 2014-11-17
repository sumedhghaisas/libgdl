/**
 * @file libgdl_test.hpp
 * @author Sumedh Ghaisas
 *
 * Declaring the tests for libGDL.
 */
#ifndef _LIBGDL_TESTS_LIBGDL_TEST_HPP_INCLUDED
#define _LIBGDL_TESTS_LIBGDL_TEST_HPP_INCLUDED

#include <libgdl/core.hpp>
#include <fstream>

#define MARK_START std::cout << "Starting test " <<\
 boost::unit_test::framework::current_test_case().p_name << "... "

#define MARK_FAIL BOOST_FAIL("[Failed]")

#define MARK_END std::cout << "\033[0;32m" "[PASSED]" "\033[0m" << std::endl

#define OPEN_LOG                                                              \
std::ofstream out_file("test_log.log", std::fstream::app);                    \
libgdl::Log test_log(out_file, false, false); 
                 

#define TEST_LOG test_log

#endif // _GDLPARSER_TESTS_GDLPARSER_TEST_HPP_INCLUDED
