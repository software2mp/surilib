

	FIND_PACKAGE(SuriGdal REQUIRED)
	FIND_PACKAGE(OpenGL REQUIRED)
	FIND_PACKAGE(Gettext REQUIRED)
	FIND_PACKAGE(MuParser REQUIRED)
	
	OPTION(SURILIB_MONOLITIC_BUILD "Compilar SuriLib como biblioteca unica." ON)
	OPTION(SURILIB_CHECK_ETIQUETTE "Verificar la etiqueta de programacion." OFF)

	SET(SURILIB_MONOLITIC_BUILD ON CACHE BOOL "Compilar SuriLib como biblioteca unica." FORCE)

	IF (CMAKE_BUILD_TYPE STREQUAL "Release")
		SET(wxWidgets_USE_DEBUG OFF)
	ENDIF(CMAKE_BUILD_TYPE STREQUAL "Release")

	IF (CMAKE_BUILD_TYPE STREQUAL "Debug" OR NOT CMAKE_BUILD_TYPE)
		ADD_DEFINITIONS(-D__DEBUG__)
	ENDIF (CMAKE_BUILD_TYPE STREQUAL "Debug" OR NOT CMAKE_BUILD_TYPE)

	FIND_PACKAGE(PythonInterp)
	
	SET(wxWidgets_USE_UNICODE OFF)
	SET(wxWidgets_USE_UNIVERSAL OFF)
	SET(wxWidgets_USE_GL ON)
	SET(MSYS TRUE)
	FIND_PACKAGE(wxWidgets REQUIRED xrc base core adv html net qa xml gl aui) # expat jpeg tiff media png regex zlib
			
	IF (wxWidgets_FOUND)
		INCLUDE(${wxWidgets_USE_FILE})
		INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
		
		ADD_DEFINITIONS(-D__WXWIDGETS__)
	
		MACRO(WXWIDGETS_ADD_RESOURCES INIT_NAMESPACE)
				STRING(TOUPPER ${INIT_NAMESPACE} INIT_NAMESPACE_UPPER)
				#MESSAGE(STATUS "directorio: " ${CMAKE_CURRENT_BINARY_DIR})
				SET(init_c_file ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/init_${INIT_NAMESPACE}.cpp)
				SET(init_forwards_h_file ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/init_${INIT_NAMESPACE}_forwards.h)
				SET(init_h_file ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/init_${INIT_NAMESPACE}.h)
				SET(init_global_resource_function "Init${INIT_NAMESPACE}XmlResource()")
				SET(init_global_resource_function_forward "void ${init_global_resource_function}")
				ADD_DEFINITIONS(-D${INIT_NAMESPACE_UPPER}_XRC_INITIALIZATION_FUNCTION_FORWARD=${init_global_resource_function_forward})
				ADD_DEFINITIONS(-D${INIT_NAMESPACE_UPPER}_XRC_INITIALIZATION_FUNCTION=${init_global_resource_function})
				ADD_CUSTOM_COMMAND(
					OUTPUT "${init_c_file}"
					COMMAND ${CMAKE_COMMAND} -E echo "#include" \"${init_h_file}\" > "${init_c_file}"
					COMMAND ${CMAKE_COMMAND} -E echo "#include" \"${init_forwards_h_file}\" >> "${init_c_file}"
					VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "${init_global_resource_function_forward}{" >> "${init_c_file}"
					MAIN_DEPENDENCY "${init_h_file}"
					DEPENDS "${init_forwards_h_file}"
				)
				ADD_CUSTOM_COMMAND(
					OUTPUT "${init_h_file}"
					VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "#ifndef ${INIT_NAMESPACE_UPPER}_XRC_INITIALIZATION_H_" > "${init_h_file}"
					VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "#define ${INIT_NAMESPACE_UPPER}_XRC_INITIALIZATION_H_" >> "${init_h_file}"
					VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "${init_global_resource_function_forward};" >> "${init_h_file}"
				)
				ADD_CUSTOM_COMMAND(
					OUTPUT "${init_forwards_h_file}"
					VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "#ifndef ${INIT_NAMESPACE_UPPER}_XRC_INITIALIZATION_FORWARDS_H_" > "${init_forwards_h_file}"
					VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "#define ${INIT_NAMESPACE_UPPER}_XRC_INITIALIZATION_FORWARDS_H_" >> "${init_forwards_h_file}"
				)
				FOREACH(xrc_file ${ARGN})
					GET_FILENAME_COMPONENT(name ${xrc_file} NAME_WE)
					GET_FILENAME_COMPONENT(dir ${xrc_file} PATH)
					SET(cpp_file ${name}_xrced.cpp)
					SET(init_function Init${name}XmlResource)
					ADD_CUSTOM_COMMAND(
						OUTPUT ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${cpp_file}
						COMMAND ${wxWidgets_wxrc_EXECUTABLE} --cpp-code --function=${init_function}
									--output=${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${cpp_file} ${CMAKE_CURRENT_SOURCE_DIR}/${xrc_file}
						MAIN_DEPENDENCY ${xrc_file}
					)
					ADD_CUSTOM_COMMAND(
						OUTPUT "${init_c_file}"
						VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "${init_function}();" >> "${init_c_file}"
						DEPENDS ${xrc_file} "${init_h_file}"
						APPEND
					)
					ADD_CUSTOM_COMMAND(
						OUTPUT "${init_forwards_h_file}"
						VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "void ${init_function}();" >> "${init_forwards_h_file}"
						DEPENDS ${xrc_file}
						APPEND
					)
				SET(RESOURCES_SOURCES ${RESOURCES_SOURCES} ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${cpp_file})
			ENDFOREACH(xrc_file)
			ADD_CUSTOM_COMMAND(
				OUTPUT "${init_c_file}"
				VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "}" >> "${init_c_file}"
				APPEND
			)
			ADD_CUSTOM_COMMAND(
				OUTPUT "${init_h_file}"
				VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "#endif" >> "${init_h_file}"
				APPEND
			)
			ADD_CUSTOM_COMMAND(
				OUTPUT "${init_forwards_h_file}"
				VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "#endif" >> "${init_forwards_h_file}"
				APPEND
			)
			SET(RESOURCES_SOURCES ${RESOURCES_SOURCES} "${init_c_file}")
		ENDMACRO(WXWIDGETS_ADD_RESOURCES)
	ENDIF(wxWidgets_FOUND)
	
	IF (NOT USR_LOCAL)
		SET(USR_LOCAL "/usr/local/")
	ENDIF(NOT USR_LOCAL)
	IF(WIN32)
		MACRO(ADD_RESOURCE)
			FOREACH(rc_file ${ARGV})
				IF(MINGW)
					IF(NOT CMAKE_RESOURCE_COMPILER)
						FIND_PROGRAM(CMAKE_RESOURCE_COMPILER windres)
						IF(NOT CMAKE_RESOURCE_COMPILER)
							MESSAGE(FATAL_ERROR "windres not found - aborting")
						ENDIF(NOT CMAKE_RESOURCE_COMPILER)
					ENDIF(NOT CMAKE_RESOURCE_COMPILER)
	
					FILE(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir/resources")
	
					# resource compilation for mingw
	
					GET_FILENAME_COMPONENT(name ${rc_file} NAME_WE)
					ADD_CUSTOM_COMMAND(
					OUTPUT "${PROJECT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir/resources/${name}.obj"
					COMMAND ${CMAKE_RESOURCE_COMPILER}
					--include-dir ${USR_LOCAL}/include/wx-2.8 --define __WIN32__ --define __WIN95__ --define __GNUWIN32__
					-o"${PROJECT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir/resources/${name}.obj"
					-i"${CMAKE_CURRENT_SOURCE_DIR}/${rc_file}"
					)
					SET(ADDITIONAL_SOURCES ${ADDITIONAL_SOURCES} "${PROJECT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir/resources/${name}.obj")
				ELSE(MINGW)
					SET(ADDITIONAL_SOURCES ${ADDITIONAL_SOURCES} "${rc_file}")
				ENDIF(MINGW)
			ENDFOREACH(rc_file)
		ENDMACRO(ADD_RESOURCE)
	ELSE()
		MACRO(ADD_RESOURCE)
		ENDMACRO(ADD_RESOURCE)
	ENDIF(WIN32)
	
	IF(GETTEXT_FOUND)
		MACRO(GENERATE_TRANSLATIONS OutputFile InputFile)
			GET_FILENAME_COMPONENT(outdir ${OutputFile} PATH)
			ADD_CUSTOM_COMMAND(
				OUTPUT ${OutputFile}
				COMMAND ${CMAKE_COMMAND} -E make_directory ${outdir}
				COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${OutputFile} ${InputFile}
				DEPENDS ${InputFile}
			)
			SET(ADDITIONAL_SOURCES ${ADDITIONAL_SOURCES} "${OutputFile}")
		ENDMACRO(GENERATE_TRANSLATIONS)
	ENDIF(GETTEXT_FOUND)

	INCLUDE_DIRECTORIES(${SURILIB_INCLUDE_DIR})
	
	IF (NOT SURILIB_LIBRARY)
		SET(SURILIB_LIBRARY suri)
	ENDIF(NOT SURILIB_LIBRARY)

IF(SURILIB_MONOLITIC_BUILD)
	SET(SURILIB_LIBRARIES ${SURILIB_LIBRARY})
ELSE(SURILIB_MONOLITIC_BUILD)
	SET(SURILIB_LIBRARIES ${SURILIB_LIBRARY}-core ${SURILIB_LIBRARY}-widgets)
ENDIF(SURILIB_MONOLITIC_BUILD)

SET(SURILIB_LIBRARIES ${SURILIB_LIBRARIES}
						${OPENGL_LIBRARIES}
						${wxWidgets_LIBRARIES}
						${GDAL_LIBRARIES}
						${MUPARSER_LIBRARIES}
						${CMAKE_DL_LIBS})

	MACRO(ADD_EXTRA_SOURCES SOURCES_LIST)
		ADD_DEFINITIONS(-D__${SOURCES_LIST}__)
		FOREACH(src_file ${ARGN})
			IF (NOT CMAKE_CURRENT_LIST_DIR)
				GET_FILENAME_COMPONENT(dir ${CMAKE_CURRENT_LIST_FILE} PATH)
			ELSE (NOT CMAKE_CURRENT_LIST_DIR)
				SET(dir ${CMAKE_CURRENT_LIST_DIR})
			ENDIF (NOT CMAKE_CURRENT_LIST_DIR)
			SET(${SOURCES_LIST}_SOURCES ${${SOURCES_LIST}_SOURCES} ${dir}/${src_file})
			INCLUDE_DIRECTORIES(${dir})
			IF (SURILIB_CHECK_ETIQUETTE)
				GET_FILENAME_COMPONENT(name ${src_file} NAME_WE)
				STRING(REPLACE ${CMAKE_CURRENT_SOURCE_DIR} "" relative_dir ${dir})
				IF (${dir} MATCHES ".*suri.*")
					SET(TARGET_DIR "suri")
					SET(ETIQUETTE_TARGET "suri")
				ELSE(${dir} MATCHES ".*suri.*")
					SET(TARGET_DIR ${PROJECT_NAME})
					SET(ETIQUETTE_TARGET "app")
				ENDIF(${dir} MATCHES ".*suri.*")
				SET(OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${TARGET_DIR}.dir${relative_dir}/")
				SET(ett_file ${OUTPUT_DIR}${name}.ett)
				SET(obj_file ${OUTPUT_DIR}${src_file}${CMAKE_CXX_OUTPUT_EXTENSION})
				GET_FILENAME_COMPONENT(basedir ${CMAKE_CURRENT_LIST_FILE} PATH)
# DEBUG				
#				MESSAGE(STATUS "------------------------------------------------")
#				MESSAGE(STATUS "dir          " ${dir})
#				MESSAGE(STATUS "ETIQUETTE_TARGET       " ${ETIQUETTE_TARGET})
#				MESSAGE(STATUS "ett_file     " ${ett_file})
#				MESSAGE(STATUS "obj_file     " ${obj_file})
#				MESSAGE(STATUS "relative_dir " ${relative_dir})
#				MESSAGE(STATUS "src_file     " ${src_file})
#				MESSAGE(STATUS "OUTPUT_DIR   " ${OUTPUT_DIR})
				ADD_CUSTOM_COMMAND(
					OUTPUT ${ett_file}
					COMMAND ${SURILIB_INCLUDE_DIR}/../tools/etiquette_checker.sh ${dir}/${src_file} "${ETIQUETTE_TARGET}"
					COMMAND ${CMAKE_COMMAND} -E touch ${ett_file}
					COMMENT "Verificando etiqueta de ${src_file}"
					DEPENDS ${obj_file}
				)
				IF (PYTHONINTERP_FOUND)
					ADD_CUSTOM_COMMAND(
						OUTPUT ${ett_file}
						COMMAND ${PYTHON_EXECUTABLE} ${SURILIB_INCLUDE_DIR}/../tools/cpplint.py ${dir}/${src_file}
						DEPENDS ${obj_file}
						APPEND
					)
				ENDIF (PYTHONINTERP_FOUND)
				SET(${SOURCES_LIST}_SOURCES ${${SOURCES_LIST}_SOURCES} ${ett_file})
			ENDIF(SURILIB_CHECK_ETIQUETTE)
		ENDFOREACH(src_file)
	ENDMACRO(ADD_EXTRA_SOURCES)

	MACRO(CHECK_ETIQUETTE ETIQEUTTE_LIST)
	ENDMACRO(CHECK_ETIQUETTE)

MACRO(GENERATE_SVN_VERSION SVN_VERSION_FILE)
	
   IF(NOT SVNVERSION)
      IF(WIN32)
         FIND_PROGRAM(SVNVERSION SubWCRev.exe 
            PATHS
            ENV PROGRAMFILES/TortoiseSVN/bin
            ENV PROGRAMFILES(X86)/TortoiseSVN/bin
            "C:/Program Files/TortoiseSVN/bin/"
            "C:/Archivos de Programa/TortoiseSVN/bin/")
      ELSE(WIN32)
         FIND_PROGRAM(SVNVERSION svnversion)
      ENDIF(WIN32)
   ENDIF(NOT SVNVERSION)
   
   ADD_CUSTOM_TARGET(Version ALL
      COMMENT Para forzar la creacion/actualizacion de ${SVN_VERSION_FILE})
      
   IF(SVNVERSION)
      IF(WIN32 AND NOT SVNVERSION STREQUAL "svnversion")
         ADD_CUSTOM_COMMAND(
            VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "#ifndef __DEBUG__" > "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}.template"
            VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "# $WCMIXED?define SVN_ISMIXED \"El arbol de compilacion se encuentra en estado MEZCLADO, sincronizar con el repositorio. \":define SVN_ISMIXED \"\"$" >> "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}.template"
            VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "# $WCMODS?define SVN_ISMODIFIED \"El arbol de compilacion se encuentra en estado MODIFICADO, sincronizar con el repositorio.\":define SVN_ISMODIFIED \"\"$" >> "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}.template"
            VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "# define __APPLICATION_NOT_FIT_FOR_RELEASE__ \"Esta version de la aplicacion no puede ser utilizada como Release.\\n\" SVN_ISMIXED SVN_ISMODIFIED" >> "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}.template"
            VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "#endif // __DEBUG__" >> "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}.template"
            VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "#define SVN_REVISION \"$WCMIXED?$WCRANGE$:$WCREV$$$WCMODS?M:$\"" >> "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}.template"
            COMMAND ${SVNVERSION} ${PROJECT_SOURCE_DIR} "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}.template" "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}_" -e
            COMMAND ${CMAKE_COMMAND} -E remove "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}.template"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}_" "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}"
            COMMAND ${CMAKE_COMMAND} -E remove "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}_"
            TARGET Version
         )
      ELSE()
         ADD_CUSTOM_COMMAND(
            VERBATIM COMMAND ${CMAKE_COMMAND} -E echo_append "#define SVN_REVISION \"" > "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}_"
            COMMAND ${SVNVERSION} -n "${PROJECT_SOURCE_DIR}" >> "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}_"
            VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "\"" >> "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}_"
            VERBATIM COMMAND ${CMAKE_COMMAND} -E echo_append "#if " >> "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}_"
            COMMAND ${SVNVERSION} -c "${PROJECT_SOURCE_DIR}" | sed "s/^[0-9]*:[0-9]*[^A-Z]$/0/g" | sed "s/^[0-9]*[^A-Z]$/0/g" | sed "/.*[A-Z].*/s/.*/1/g" >> "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}_"
            VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "# ifndef __DEBUG__" >> "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}_"
            COMMAND ${CMAKE_COMMAND} -E echo "#    define __APPLICATION_NOT_FIT_FOR_RELEASE__ \"Fuentes mezcladas o modificadas\"" >> "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}_"
            VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "# endif // __DEBUG__" >> "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}_"
            COMMAND ${CMAKE_COMMAND} -E echo "#endif" >> "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}_"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}_" "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}"
            COMMAND ${CMAKE_COMMAND} -E remove "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}_"
            TARGET Version
         )
      ENDIF(WIN32 AND NOT SVNVERSION STREQUAL "svnversion")
   ELSE()
      ADD_CUSTOM_COMMAND(
         VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "#define SVN_REVISION \"REVISION FALTANTE\"" > "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}_"
         COMMAND ${CMAKE_COMMAND} -E echo "#define __APPLICATION_NOT_FIT_FOR_RELEASE__ \"No se pudo definir la revision del software; falta el ejecutable ${SVNVERSION}\"" >> "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}_"
         COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}_" "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}"
         COMMAND ${CMAKE_COMMAND} -E remove "${PROJECT_SOURCE_DIR}/${SVN_VERSION_FILE}_"
         TARGET Version
      )
   ENDIF(SVNVERSION)	
	
ENDMACRO(GENERATE_SVN_VERSION)