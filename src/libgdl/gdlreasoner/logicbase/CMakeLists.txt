# Define the files we need to compile.
# Anything not in this list will not be compiled into libGDL.
set(SOURCES
  answer_types.hpp
  unify.cpp
  unify.hpp
  answer.hpp
  answer_decoder.hpp
  answer_decoder.cpp
  clause_answer.hpp
  clause_answer.cpp
  distinct_answer.hpp
  distinct_answer.cpp
  or_clause_answer.hpp
  or_clause_answer.cpp
  ground_question_answer.hpp
  ground_question_answer.cpp
  not_answer.hpp
  not_answer.cpp
)

# add directory name to sources
set(DIR_SRCS)
foreach(file ${SOURCES})
  set(DIR_SRCS ${DIR_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
# Append sources (with directory name) to list of all libgdl sources (used at
# the parent scope).
set(GDL_SRCS ${GDL_SRCS} ${DIR_SRCS} PARENT_SCOPE)
