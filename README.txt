================================================================================

           libGDL: Game Description Language(GDL) library

*** Introduction

Game Description Language(GDL) is designed as a part of General Game Playing
Poject. GDL describes the state of a game as a series of facts, and the game
mechanics as logical rules.
GDL Language specifications -
http://logic.stanford.edu/classes/cs227/2013/readings/gdl_spec.pdf

This library accommodate GDL parser and flattener.

================================================================================

*** HOW TO INSTALL

1) create directory build
2) cd into build and run command "cmake ../"
3) for building from source run command "make", this will create executable kif
   inside bin
4) to install run command "make install"

================================================================================

*** Running tests

* Run command 'make test' for testing

================================================================================

*** Dependencies

GDLParser has the following dependencies:

  Boost (program_options, unit_test_framework)
  CMake     >= 2.8.5
  BISON		>= 3.0.0	(OPTIONAL)

================================================================================

*** Using command line PARSER

After installing libGDL. Run command

kif OUTPUT_FILENAME -c SOURCE_FILE_LIST

For example,
kif test.kif -c 3puzzle.kif arithmatic.kif

Parameters :
-g FILENAME	: To save dependency graph generated in DOT representation
-w off  	: To disable warnings
-h          : For help

For example,
kif test.kif -c 3puzzle.kif arithmatic.kif -g test.dot

** NOTE: GraphViz library can be used to visualized the DOT representation.

================================================================================

*** Using command line FLATTENER

After installing libGDL. Run command

flatten OUTPUT_FILENAME -c SOURCE_FILE_LIST

For example,
kif test.kif -c 3puzzle.kif arithmatic.kif

Parameters :
-w off  	: To disable warnings
-h          : for help

================================================================================
