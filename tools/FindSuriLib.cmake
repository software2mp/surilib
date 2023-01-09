cmake_minimum_required(VERSION 2.6)

IF(NOT SURILIB_FOUND)
	FIND_PATH(SURILIB_INCLUDE_DIR suri.h
		PATHS
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${PROJECT_SOURCE_DIR}/lib/suri/include
		ENV SURILIB_DIR
		/usr/local/include
		/usr/include
		~/opt/local/include
		C:/msys/1.0/local/include
	)
	# Si encuentra que la raiz del proyecto es igual al directorio actual
	# determina que se esta compilando solo la biblioteca surilib
	SET(SURILIB_STANDALONE_BUILD FALSE)
	IF(${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
		SET(SURILIB_STANDALONE_BUILD TRUE)
		SET(SURILIB_BUILT_FROM_WITHIN TRUE)
	ENDIF(${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	## MESSAGE(STATUS "Standalone: " ${SURILIB_STANDALONE_BUILD})
	
	IF(NOT SURILIB_STANDALONE_BUILD)
		# Si determino que el directorio de los include es igual al de fuentes
		# asumo que es surilib esta compilado desde el arbol de fuentes (versus 
		# estar instalado como biblioteca de sistema)
		## MESSAGE(STATUS "project source dir : " ${PROJECT_SOURCE_DIR}/lib/suri/include)
		## MESSAGE(STATUS "current source dir : " ${CMAKE_CURRENT_SOURCE_DIR}/include)
		## MESSAGE(STATUS "surilib include dir: " ${SURILIB_INCLUDE_DIR})
		SET(SURILIB_BUILT_FROM_WITHIN FALSE)
		IF(${SURILIB_INCLUDE_DIR} STREQUAL ${PROJECT_SOURCE_DIR}/lib/suri/include)
			SET(SURILIB_BUILT_FROM_WITHIN TRUE)
		ENDIF(${SURILIB_INCLUDE_DIR} STREQUAL ${PROJECT_SOURCE_DIR}/lib/suri/include)
	ENDIF(NOT SURILIB_STANDALONE_BUILD)
	
	SET(SURILIB_USE_FILE "")
	
	IF(SURILIB_INCLUDE_DIR)
		IF(SURILIB_BUILT_FROM_WITHIN)
			MESSAGE(STATUS "SuriLib source tree found.")
			MESSAGE(STATUS "SuriLib will be compiled from sources.")
			SET(SURILIB_FOUND TRUE)
			SET(SURILIB_LIBRARY suri)
		ELSE(SURILIB_BUILT_FROM_WITHIN)
			FIND_LIBRARY(SURILIB_LIBRARY suri
				PATHS
				${SURILIB_INCLUDE_DIR}/../lib
				${SURILIB_INCLUDE_DIR}/..
				/usr/local/lib
				/usr/lib
				~/opt/local/lib
				C:/msys/1.0/local/lib)
			IF(SURILIB_LIBRARY)
				MESSAGE(STATUS "SuriLib Found: TRUE")
				SET(SURILIB_FOUND TRUE)
			ELSE(SURILIB_LIBRARY)
				MESSAGE(SEND_ERROR "SuriLib Found: FALSE")
				SET(SURILIB_FOUND FALSE)
			ENDIF(SURILIB_LIBRARY)
		ENDIF(SURILIB_BUILT_FROM_WITHIN)
	ELSE(SURILIB_INCLUDE_DIR)
		MESSAGE(SEND_ERROR "SuriLib Found: FALSE")
	ENDIF(SURILIB_INCLUDE_DIR)
	## MESSAGE(STATUS "SuriLib Include: " ${SURILIB_INCLUDE_DIR})
	## MESSAGE(STATUS "SuriLib Library: " ${SURILIB_LIBRARY})
	IF (SURILIB_FOUND)
		GET_FILENAME_COMPONENT(USEFILE_PATH ${CMAKE_CURRENT_LIST_FILE} PATH)
		SET(SURILIB_USE_FILE ${USEFILE_PATH}/UseSuriLib.cmake)
	ENDIF(SURILIB_FOUND)
ENDIF(NOT SURILIB_FOUND)

## MESSAGE(STATUS "Use file: " ${SURILIB_USE_FILE})
IF (SURILIB_FOUND)
	INCLUDE(${SURILIB_USE_FILE})
ENDIF(SURILIB_FOUND)