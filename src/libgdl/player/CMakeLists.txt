## Recurse parser
set(DIRS
  mcts
  mcts_qselect
  mcts_with_ols
)

foreach(dir ${DIRS})
    add_subdirectory(${dir})
endforeach()

# Define the files we need to compile.
# Anything not in this list will not be compiled into libGDL.
set(SOURCES
  default_player_config.hpp
  game_player.hpp
  random_player.cpp
  random_player.hpp
)

# add directory name to sources
set(DIR_SRCS)
foreach(file ${SOURCES})
  set(DIR_SRCS ${DIR_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
# Append sources (with directory name) to list of all libgdl sources (used at
# the parent scope).
set(GDL_SRCS ${GDL_SRCS} ${DIR_SRCS} PARENT_SCOPE)


#-------------------------------------------------------------------

macro( array2d_get_item out_value offset )
	math( EXPR _finalindex "${_array2d_index}+${offset}" )
	list( GET _array2d_array ${_finalindex} _item )
	set( ${out_value} "${_item}" )
endmacro()

#-------------------------------------------------------------------

macro( array2d_begin_loop out_advanced array width var_names )
	set( _array2d_out_advanced ${out_advanced} )
	set( _array2d_index 0 )
	set( _array2d_array ${array} )
	set( _array2d_width ${width} )
	set( _array2d_var_names ${var_names} )
	array2d_advance()
endmacro()

#-------------------------------------------------------------------

macro( array2d_advance )
	if( NOT _array2d_array )
		set( ${_array2d_out_advanced} false )
	else()	
		list( LENGTH _array2d_array _size )
		math( EXPR _remaining "${_size}-${_array2d_index}" )
		
		if( (_array2d_width LESS 1) OR (_size LESS _array2d_width) OR (_remaining LESS _array2d_width) )
			set( ${_array2d_out_advanced} false )
		else()
			math( EXPR _adjusted_width "${_array2d_width}-1" )
			foreach( offset RANGE ${_adjusted_width} )
				list( GET _array2d_var_names ${offset} _var_name )
				array2d_get_item( ${_var_name} ${offset} )
			endforeach()
			
			math( EXPR _index "${_array2d_index}+${_array2d_width}" )
			set( _array2d_index ${_index} )
			set( ${_array2d_out_advanced} true )
		endif()
	endif()
endmacro()

#-------------------------------------------------------------------

set( PLAYERS
MCTSPlayer <libgdl/player/mcts/player_include.hpp>
MCTSQSelectPlayer <libgdl/player/mcts_qselect/player_include.hpp>
)

array2d_begin_loop( advanced "${PLAYERS}" 2 "name;player_include" )
while( advanced )
 	add_executable(${name}
    player_runner_main.cpp
  )
  
  if(USE_STATIC_LIBGDL)
    target_link_libraries(${name}
      gdl_static
      boost_program_options
      pthread
      dl
    )
  else()
    target_link_libraries(${name}
      gdl
      boost_program_options
      pthread
      dl
    )
  endif()
  
  set(add_flag PLAYER_INCLUDE="${player_include}")

  set_target_properties(${name} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
  set_target_properties(${name} PROPERTIES COMPILE_DEFINITIONS PLAYER_INCLUDE=${player_include})

  install(TARGETS ${name} RUNTIME DESTINATION bin)
 	
 	array2d_advance()
 endwhile()
