# set(MU_PARSER_INCLUDE_DIR C:/msys/1.0/local/include/)
#
# Find the muParserDLL includes and library
#
# This module defines
# MUPARSER_INCLUDE_DIR, where to find muParserDLL.h, etc.
# MUPARSER_LIBRARIES, the libraries to link against to use muparser.dll.
# MUPARSER_FOUND, If false, do not try to use muparser.
# also defined, but not for general use are
# MUPARSER_LIBRARY, where to find the muparser library.
# MUPARSER_DEBUG_LIBRARY, where to find the muparser library in debug mode.
FIND_PATH(MUPARSER_INCLUDE_DIR muParserDLL.h
  /usr/local/include
  /usr/include
  C:/msys/1.0/local/include
  $HOME/opt/local/include
  ~/opt/local/include
)

FIND_LIBRARY(MUPARSER_LIBRARY muparser
  ${MUPARSER_INCLUDE_DIR}/../lib
  /usr/local/lib
  /usr/lib
  C:/msys/1.0/local/lib
  ~/opt/local/lib
  $HOME/opt/local/lib
)

IF(MUPARSER_INCLUDE_DIR)
  IF(MUPARSER_LIBRARY)
    MESSAGE(STATUS "MuParser Found: TRUE")
	INCLUDE_DIRECTORIES(${MUPARSER_INCLUDE_DIR})
	ADD_DEFINITIONS(-D__MU_PARSER__)
    SET(MUPARSER_FOUND "YES")
    SET(MUPARSER_LIBRARIES ${MUPARSER_LIBRARY} ${CMAKE_DL_LIBS})
  ENDIF(MUPARSER_LIBRARY)
ENDIF(MUPARSER_INCLUDE_DIR)