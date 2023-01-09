cmake_minimum_required(VERSION 2.6)

FIND_PACKAGE(SuriLib REQUIRED)
message("***************************************************SURILIB_LIBRARIES=${SURILIB_LIBRARIES}")

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
	COMMENT Para forzar la creacion/actualizacion de svn_version.h)
IF(SVNVERSION)
	IF(WIN32 AND NOT SVNVERSION STREQUAL "svnversion")
		ADD_CUSTOM_COMMAND(
			VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "#ifndef __DEBUG__" > "${PROJECT_SOURCE_DIR}/src/svn_version.template"
			VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "#	$WCMIXED?define SVN_ISMIXED \"El arbol de compilacion se encuentra en estado MEZCLADO, sincronizar con el repositorio. \":define SVN_ISMIXED \"\"$" >> "${PROJECT_SOURCE_DIR}/src/svn_version.template"
			VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "#	$WCMODS?define SVN_ISMODIFIED \"El arbol de compilacion se encuentra en estado MODIFICADO, sincronizar con el repositorio.\":define SVN_ISMODIFIED \"\"$" >> "${PROJECT_SOURCE_DIR}/src/svn_version.template"
			VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "#	define __APPLICATION_NOT_FIT_FOR_RELEASE__ \"Esta version de la aplicacion no puede ser utilizada como Release.\\n\" SVN_ISMIXED SVN_ISMODIFIED" >> "${PROJECT_SOURCE_DIR}/src/svn_version.template"
			VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "#endif // __DEBUG__" >> "${PROJECT_SOURCE_DIR}/src/svn_version.template"
			VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "#define SVN_REVISION \"$WCMIXED?$WCRANGE$:$WCREV$$$WCMODS?M:$\"" >> "${PROJECT_SOURCE_DIR}/src/svn_version.template"
			COMMAND ${SVNVERSION} ${PROJECT_SOURCE_DIR} "${PROJECT_SOURCE_DIR}/src/svn_version.template" "${PROJECT_SOURCE_DIR}/src/svn_version.h_" -e
			COMMAND ${CMAKE_COMMAND} -E remove "${PROJECT_SOURCE_DIR}/src/svn_version.template"
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/src/svn_version.h_" "${PROJECT_SOURCE_DIR}/src/svn_version.h"
			COMMAND ${CMAKE_COMMAND} -E remove "${PROJECT_SOURCE_DIR}/src/svn_version.h_"
			TARGET Version
		)
	ELSE()
		ADD_CUSTOM_COMMAND(
			VERBATIM COMMAND ${CMAKE_COMMAND} -E echo_append "#define SVN_REVISION \"" > "${PROJECT_SOURCE_DIR}/src/svn_version.h_"
			COMMAND ${SVNVERSION} -n "${PROJECT_SOURCE_DIR}" >> "${PROJECT_SOURCE_DIR}/src/svn_version.h_"
			VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "\"" >> "${PROJECT_SOURCE_DIR}/src/svn_version.h_"
			VERBATIM COMMAND ${CMAKE_COMMAND} -E echo_append "#if " >> "${PROJECT_SOURCE_DIR}/src/svn_version.h_"
			COMMAND ${SVNVERSION} -c "${PROJECT_SOURCE_DIR}" | sed /.*[A-Z,:].*/s/.*/1/g >> "${PROJECT_SOURCE_DIR}/src/svn_version.h_"
			VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "#	ifndef __DEBUG__" >> "${PROJECT_SOURCE_DIR}/src/svn_version.h_"
			COMMAND ${CMAKE_COMMAND} -E echo "#		define __APPLICATION_NOT_FIT_FOR_RELEASE__ \"Fuentes mezcladas o modificadas\"" >> "${PROJECT_SOURCE_DIR}/src/svn_version.h_"
			VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "#	endif // __DEBUG__" >> "${PROJECT_SOURCE_DIR}/src/svn_version.h_"
			COMMAND ${CMAKE_COMMAND} -E echo "#endif" >> "${PROJECT_SOURCE_DIR}/src/svn_version.h_"
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/src/svn_version.h_" "${PROJECT_SOURCE_DIR}/src/svn_version.h"
			COMMAND ${CMAKE_COMMAND} -E remove "${PROJECT_SOURCE_DIR}/src/svn_version.h_"
			TARGET Version
		)
	ENDIF(WIN32 AND NOT SVNVERSION STREQUAL "svnversion")
ELSE()
	ADD_CUSTOM_COMMAND(
		VERBATIM COMMAND ${CMAKE_COMMAND} -E echo "#define SVN_REVISION \"REVISION FALTANTE\"" > "${PROJECT_SOURCE_DIR}/src/svn_version.h_"
		COMMAND ${CMAKE_COMMAND} -E echo "#define __APPLICATION_NOT_FIT_FOR_RELEASE__ \"No se pudo definir la revision del software; falta el ejecutable ${SVNVERSION}\"" >> "${PROJECT_SOURCE_DIR}/src/svn_version.h_"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/src/svn_version.h_" "${PROJECT_SOURCE_DIR}/src/svn_version.h"
		COMMAND ${CMAKE_COMMAND} -E remove "${PROJECT_SOURCE_DIR}/src/svn_version.h_"
		TARGET Version
	)
ENDIF(SVNVERSION)

SET(ADDITIONAL_SOURCES ${ADDITIONAL_SOURCES} svn_version.h)

ADD_RESOURCE(Application.rc)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

IF(GETTEXT_FOUND)
# es_ES
	GENERATE_TRANSLATIONS(${PROJECT_BINARY_DIR}/src/data/locale/es/LC_MESSAGES/Application.mo
									${PROJECT_SOURCE_DIR}/src/locale/es/Application.po)
	GENERATE_TRANSLATIONS(${PROJECT_BINARY_DIR}/src/data/locale/es/LC_MESSAGES/resources.mo
									${PROJECT_SOURCE_DIR}/src/locale/es/resources.po)
	GENERATE_TRANSLATIONS(${PROJECT_BINARY_DIR}/src/data/locale/es/LC_MESSAGES/suri.mo
									${PROJECT_SOURCE_DIR}/lib/suri/locale/es/suri.po)

# es_AR
    GENERATE_TRANSLATIONS(${PROJECT_BINARY_DIR}/src/data/locale/es_AR/LC_MESSAGES/Application.mo
                                    ${PROJECT_SOURCE_DIR}/src/locale/es_AR/Application.po)
    GENERATE_TRANSLATIONS(${PROJECT_BINARY_DIR}/src/data/locale/es_AR/LC_MESSAGES/resources.mo
                                    ${PROJECT_SOURCE_DIR}/src/locale/es_AR/resources.po)
    GENERATE_TRANSLATIONS(${PROJECT_BINARY_DIR}/src/data/locale/es_AR/LC_MESSAGES/suri.mo
                                    ${PROJECT_SOURCE_DIR}/lib/suri/locale/es_AR/suri.po)
ENDIF(GETTEXT_FOUND)

IF (NOT ${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
	MESSAGE(STATUS "Copiando datos de programa")
	FILE(COPY ${PROJECT_SOURCE_DIR}/src/data DESTINATION ${PROJECT_BINARY_DIR}/src/)
	FILE(COPY ${PROJECT_SOURCE_DIR}/lib/suri/tools/installer.sh DESTINATION ${PROJECT_BINARY_DIR}/)
	FILE(COPY ${PROJECT_SOURCE_DIR}/installer_data.sh DESTINATION ${PROJECT_BINARY_DIR}/)
	FILE(COPY ${PROJECT_SOURCE_DIR}/default.apspec DESTINATION ${PROJECT_BINARY_DIR}/)
	FILE(COPY ${PROJECT_SOURCE_DIR}/installer.nsi DESTINATION ${PROJECT_BINARY_DIR}/)
	FILE(COPY ${PROJECT_SOURCE_DIR}/LinuxShortcut.desktop DESTINATION ${PROJECT_BINARY_DIR}/)
	FILE(COPY ${PROJECT_SOURCE_DIR}/application.xml DESTINATION ${PROJECT_BINARY_DIR}/)
ENDIF (NOT ${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})

INCLUDE(CMakeFiles.txt)


ADD_EXECUTABLE(${PROJECT_NAME} ${MAIN_SOURCE} ${ADDITIONAL_SOURCES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SURILIB_LIBRARIES} resources)

ADD_DEPENDENCIES(${PROJECT_NAME} Version)
