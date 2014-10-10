#libGDL: Game Description Language(GDL) library

![build status](https://travis-ci.org/sumedhghaisas/libgdl.svg?branch=master)

>_A human being(intelligent robot) should be able to change a diaper, plan an_
_invasion, butcher a hog, conn a ship, design a building, write a sonnet,_ 
_balance accounts, build a wall, set a bone, comfort the dying, take orders, give_
_orders, cooperate, act alone, solve equations, analyze a new problem,_ 
_pitch manure, program a computer, cook a tasty meal, fight efficiently,_ 
_die gallantly. Specialization is for insects._  
                                        **Robert Heinlein**

##Introduction

Game Description Language(GDL) is designed as a part of General Game Playing
Poject. GDL describes the state of a game as a series of facts, and the game
mechanics as logical rules.  
[GDL Language specifications](http://logic.stanford.edu/classes/cs227/2013/readings/gdl_spec.pdf)

libGDL accommodates GDL parser, reasoner and flattener. 

##How To Install

1. Create directory build.
2. cd into build and run command _**cmake ../**_
3. For building from source run command _**make**_, this will create executable kif
   and flatten inside bin.
4. To install run command _**sudo make install**_

##Running Tests

* Run command _**make test**_ for testing

##Dependencies

libGDL has the following dependencies:

* Boost (program_options, unit_test_framework)
* CMake   _>= 2.8.5_
* BISON		_>= 3.0.0	**(OPTIONAL)**_
* FLEX              _**(OPTIONAL)**_

##Command Line Usage

###Using command line PARSER

After installing libGDL. Run command  
_**kif OUTPUT_FILENAME -c SOURCE_FILE_LIST**_

For example,  
_**kif test.kif -c 3puzzle.kif arithmatic.kif**_

####Parameter List:
* **-g FILENAME**	To save dependency graph generated in DOT representation
* **-w off**  	  To disable warnings
* **-f[--flatten]** To save the flattened KIF
* **-h**          For help

To generate dependency graph in _DOT_ format  
_**kif test.kif -c 3puzzle.kif arithmatic.kif -g test.dot**_

**NOTE: GraphViz library can be used to visualized the DOT representation.**

###Using command line FLATTENER

After installing libGDL. Run command  
_**flatten OUTPUT_FILENAME -c SOURCE_FILE_LIST**_

For example,  
_**kif test.kif -c 3puzzle.kif arithmatic.kif**_

####Parameter List:
* **-w off**  To disable warnings
* **-h**      For help

##To Generate Documentation

To generate the documentation of libGDL run command **_make doc_** in folder build.
