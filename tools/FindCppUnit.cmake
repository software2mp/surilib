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
)
# With Win32, important to have both
IF(WIN32)
  FIND_LIBRARY(MUPARSER_LIBRARY muparser
               ${MUPARSER_INCLUDE_DIR}/../lib
               /usr/local/lib
               /usr/lib)
  FIND_LIBRARY(MUPARSER_DEBUG_LIBRARY muparserd
               ${MUPARSER_INCLUDE_DIR}/../lib
               /usr/local/lib
               /usr/lib)
ELSE(WIN32)
  # On unix system, debug and release have the same name
  FIND_LIBRARY(MUPARSER_LIBRARY muparser
               ${MUPARSER_INCLUDE_DIR}/../lib
               /usr/local/lib
               /usr/lib)
  FIND_LIBRARY(MUPARSER_DEBUG_LIBRARY muparser
               ${MUPARSER_INCLUDE_DIR}/../lib
               /usr/local/lib
               /usr/lib)
ENDIF(WIN32)
IF(MUPARSER_INCLUDE_DIR)
  IF(MUPARSER_LIBRARY)
	INCLUDE_DIRECTORIES(${MUPARSER_INCLUDE_DIR})
    SET(MUPARSER_FOUND "YES")
    SET(MUPARSER_LIBRARIES ${MUPARSER_LIBRARY} ${CMAKE_DL_LIBS})
 IF(MUPARSER_DEBUG_LIBRARY) 
  SET(MUPARSER_DEBUG_LIBRARIES ${MUPARSER_DEBUG_LIBRARY} ${CMAKE_DL_LIBS})
 ENDIF(MUPARSER_DEBUG_LIBRARY) 
  ENDIF(MUPARSER_LIBRARY)
ENDIF(MUPARSER_INCLUDE_DIR)