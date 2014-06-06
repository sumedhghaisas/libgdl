================================================================================

           GDLParser: Game Description Language(GDL) parser

================================================================================

*** HOW TO INSTALL

1) create directory build 
2) cd into build and run command "cmake ../"
3) for building from source run command "make", this will create executable kif
   inside bin
4) to install run command "make install"

* Run command 'make test' for for testing

================================================================================

*** Dependencies

GDLParser has the following dependencies:

  Boost (program_options, math_c99, unit_test_framework, random)
  CMake         >= 2.8.5
  BISON		>= 3.0.0	(OPTIONAL)

================================================================================

*** Using command line parser

After installing GDLParser. Run command 

kif OUTPUT_FILENAME SOURCE_FILE_LIST

For example,
kif test.kif 3puzzle.kif arithmatic.kif

Parameters :
-g	: To save dependency graph generated in DOT representation

For example,
kif test.kif 3puzzle.kif arithmatic.kif -g test.dot

================================================================================
