#!/bin/bash

set -e
set -u

black='\E[30;40m'
red='\E[31;40m'
green='\E[32;40m'
yellow='\E[33;40m'
blue='\E[34;40m'
magenta='\E[35;40m'
cyan='\E[36;40m'
white='\E[37;40m'
normal='\E[37;40m'

init_variable() {
	eval "default_value=\$$2"
	echo -n "$1 [${default_value}]: "
	read user_input
	if [ -z "$user_input" ] ; then
		eval "$2=\"$default_value\""
	else
		eval "$2=\"$user_input\""
	fi
}

path_to_windows() {
	return "$(echo \"\$$1\" | sed 's/\//\\/g')"
}

NO_NEWLINE="no-newline"
ADDSTAR=1
showmessage(){
	if [ $ADDSTAR == 1 ] ; then
		echo -ne " ${green}*${normal} "
	fi
	ADDSTAR=1
	if [ $# == 1 ] ; then
		echo -e "$1"
	else
		echo -ne "$1 "
		ADDSTAR=0
	fi
}

die() {
	showmessage "Terminando" ""
	if [ $# != 0 ] ; then
		showmessage "[ ${red}ERROR${normal}: $1 ]" ""
	fi
	showmessage ""
	exit -1
}

svn info 2>/dev/null >/dev/null && die 'El directorio . no debe ser una copia de trabajo'
test ! -d trunk || die 'No debe existir un directorio llamado trunk'

# nombre deseado
PROGRAMNAME="Software basado en SuriLib"
init_variable "Nombre completo de la aplicación" PROGRAMNAME
PROGRAMNAMEUS="SuriLib powered software"
init_variable "Nombre completo de la aplicación (en inglés)" PROGRAMNAMEUS
# nombre corto de la aplicacion
SHORTPROGRAMNAME="SuriApp"
init_variable "Nombre corto de la aplicación (para ejecutable)" SHORTPROGRAMNAME
# extension mime
MIMEEXTENSION="suri"
init_variable "Extensión de los archivos asociados al software" MIMEEXTENSION
# descripcion del soft
DESCRIPTION="Software de procesamiento de imágenes y SIG basado en SuriLib"
init_variable "Descripción del software" DESCRIPTION
DESCRIPTIONUS="Image processing and GIS Software powered by SuriLib"
init_variable "Descripción del software (en inglés)" DESCRIPTIONUS
# Nombre del archivo de proyecto asociado 
PROJECT="Project"
init_variable "Nombre formal para el tipo de archivo asociado a la aplicacion" PROJECT
# descripcion del mime
MIMEDESCRIPTION="Archivo de Proyecto"
init_variable "Descripción del archivo asociado al software" MIMEDESCRIPTION
MIMEDESCRIPTIONUS="Project File"
init_variable "Descripción del archivo asociado al software (en inglés)" MIMEDESCRIPTIONUS
# Version inicial del software
INITIALVERSION="0.0.0"
init_variable "Version inicial del software" INITIALVERSION
# Nombre de la clase principal de la aplicacion
MAINCLASSNAME="MainClass"
init_variable "Nombre de la clase principal" MAINCLASSNAME
# Destino en el SVN del nuevo proyecto
SVN_URL="https://svn.suremptec.com.ar/home/sur/svn"
init_variable "Url del servidor subversion" SVN_URL
# Destino en el SVN del nuevo proyecto
SVN_MODULE=""
init_variable "Directorio base del modulo en el svn" SVN_MODULE
if [ -z "${SVN_MODULE}" ] ; then
	SVN=1;
	showmessage "Modo local"
fi

# grupos donde lo instala en Linux
LINUXGROUPS="Math;Science;Graphics"

# Directorio de origen de datos
SRCDIR=src
# Directorio de los recursos
RESOURCEDIR="${SRCDIR}/resources"
# Directorio de los recursos de bitmaps
BITMAPDIR="${RESOURCEDIR}/bitmaps"
# Modulo base
SVN_BASE="Suri-2.0"
# Rama del modulo
SVN_BRANCH="trunk"
# Herramientas
SVN_TOOLS="${SVN_BASE}/${SVN_BRANCH}/tools"
# Modulo de bootstrap en el SVN
SVN_BOOT="${SVN_TOOLS}/bootstrap"

# Uppercase del nombre de la clase
MAINCLASSUPPER="$(echo ${MAINCLASSNAME} | tr "[:lower:]" "[:upper:]")"

# Directorios convertidos a win32
WIN_SRCDIR="$(echo $SRCDIR | sed 's/\//\\\\/g')"
WIN_RESOURCEDIR="$(echo RESOURCEDIR | sed 's/\//\\\\/g')"
WIN_BITMAPDIR="$(echo $BITMAPDIR | sed 's/\//\\\\/g')"

sed_file() {
	FILENAME="$1"
	shift
	EXPR="$1"
	shift
	VERSION="$(echo $1| sed 's/\//\\\//g')"
	shift
	if [ ! -f "${FILENAME}" ] ; then
		showmessage "${FILENAME} [ ${red}FAIL${normal} ]"
		exit -1
	fi
	showmessage "Actualizando en ${FILENAME} : ${EXPR} con ${VERSION}" no-newline
	tmp_file="/tmp/tmp-$$.expr"
	rm -f "$tmp_file"
	mv "${FILENAME}" "$tmp_file"
	sed -r "s/${EXPR}/\1${VERSION}\2/gi" "$tmp_file" > "${FILENAME}"
	if [ $? != 0 ] ; then
		showmessage "[ ${red}FAIL${normal} ]"
		cp "$tmp_file" "${FILENAME}"
		return 0
	fi
	! cmp -s "$tmp_file" "${FILENAME}" || return 1
	rm -f "$tmp_file"
	showmessage "[ ${green}OK${normal} ]"
}

replace_variables(){
	FILE_ID=1
	REPLACEMENTS=(
		"LinuxShortcut.desktop" "(^Exec=).*($)" "${LAUNCHSCRIPT} %f" 1													\
		"LinuxShortcut.desktop" "(^Icon=).*($)" ${ICON} 1																	\
		"LinuxShortcut.desktop" "(^MimeType=).*($)" "application.${MIMEEXTENSION}" 1								\
		"LinuxShortcut.desktop" "(^TryExec=).*($)" "${EXECUTABLENAME}" 1												\
		"LinuxShortcut.desktop" "(^Name=).*($)" "${PROGRAMNAME}" 1														\
		"LinuxShortcut.desktop" "(^Name\[en_US\]=).*($)" "${PROGRAMNAMEUS}" 1										\
		"LinuxShortcut.desktop" "(^Name\[es\]=).*($)" "${PROGRAMNAME}" 1												\
		"LinuxShortcut.desktop" "(^Categories=).*($)" "${LINUXGROUPS}" 1												\
		"LinuxShortcut.desktop" "(^GenericName=).*($)" "${DESCRIPTIONUS}" 1											\
		"LinuxShortcut.desktop" "(^GenericName\[en_US\]=).*($)" "${DESCRIPTIONUS}" 1								\
		"LinuxShortcut.desktop" "(^GenericName\[es\]=).*($)" "${DESCRIPTION}" 1										\
		"LinuxShortcut.desktop" "(^Comment=).*($)" "${DESCRIPTIONUS}" 1												\
		"LinuxShortcut.desktop" "(^Comment\[en_US\]=).*($)" "${DESCRIPTIONUS}" 1									\
		"LinuxShortcut.desktop" "(^Comment\[es\]=).*($)" "${DESCRIPTION}" 1											\
		"default.apspec" "(^RootName: \@suremptec\.com\/).*(:.*$)"	"${EXECUTABLENAMELOWERCASE}" 1			\
		"default.apspec" "(^DisplayName:).*($)" "\ ${PROGRAMNAME}" 1													\
		"default.apspec" "(^ShortName:).*($)"	"\ ${EXECUTABLENAMELOWERCASE}" 1										\
		"default.apspec" "(^installIcon).*($)" "\ hicolor" 1																\
		"default.apspec" '(^copyFiles).*($)' '\ data\ "$PREFIX"/share/'${EXECUTABLENAME} 1						\
		"default.apspec" '(^installExe).*($)' "\ ${EXECUTABLENAME} ${LAUNCHSCRIPT}" 1								\
		"default.apspec" '(^installMenuItem).*($)' "\ \"\"\ \"${DESKTOPNAME}\"" 1									\
		"default.apspec" '(^installMime).*($)' "\ \"application-${MIMEEXTENSION}.xml\"" 1						\
		"default.apspec" '(^PackageFileName:).*($)' "\ ${EXECUTABLENAME}-v\$SOFTWAREVERSION.package" 1		\
		"default.apspec" '(^Summary:).*($)' "\ ${DESCRIPTIONUS}" 1														\
		"default.apspec" '(^Summary\[es\]:).*($)' "\ ${DESCRIPTION}" 1													\
		"application.xml" '(\<apkg:icon>).*(<\/apkg:icon\>)' "${MIMEICON}.png" 1									\
		"application.xml" '(\<icon name=").*(")' "${MIMEICON}.png" 1													\
		"application.xml" '(\<glob pattern=").*(")' "\*\.${MIMEEXTENSION}" 1											\
		"application.xml" '(\<mime-type type="application\/).*(")' "${MIMEEXTENSION}" 1							\
		"application.xml" '(\<comment>).*(<\/comment\>)' "${MIMEDESCRIPTION}" 1										\
		"application.xml" '(\<comment xml:lang="en">).*(<\/comment\>)' "${MIMEDESCRIPTIONUS}" 1				\
		"application.xml" '(\<comment xml:lang="es">).*(<\/comment\>)' "${MIMEDESCRIPTION}" 1					\
		"CMakeLists.txt" '(^project.*\().*(\))' "${EXECUTABLENAME}" 1													\
		"CMakeLists.txt" '(^add_subdirectory\ \()(\)$)' "${SRCDIR}" 1													\
		"installer.nsi" '(^!define APP_SHORT_NAME).*($)' "\ \"${SHORTPROGRAMNAME}\"" 1							\
		"installer.nsi" '(^!define APP_NAME).*($)' "\ \"${PROGRAMNAME}\"" 1											\
		"installer.nsi" '(^!define EXEFILE).*($)' "\ \"${EXECUTABLENAME}.exe\"" 1									\
		"installer.nsi" '(^!define MIME_EXTENSION).*($)' "\ \"${MIMEEXTENSION}\"" 1								\
		"installer.nsi" '(^!define MIME_ICON).*($)' "\ \"${MIMEICON}.ico\"" 1										\
		"installer.nsi" '(^!define VENDOR).*($)' "\ \"${EXECUTABLENAME}\"" 1											\
		"installer.nsi" '(^!define INSTALLER_ICON).*($)' "\ \"${WIN_BITMAPDIR}icon-Installer.ico\"" 1		\
		"installer.nsi" '(^!define UNINSTALLER_ICON).*($)' "\ \"${WIN_BITMAPDIR}icon-UnInstaller.ico\"" 1	\
		"installer.nsi" '(^!define MIME_ICON).*($)' "\ \"${MIMEICON}.ico\"" 1										\
		"installer.nsi" '(^!define MIME_ICON_PATH).*($)' "\ \"${WIN_BITMAPDIR}\$\{MIME_ICON\}\"" 1			\
		"installer.nsi" '(^!define SRC_DIR).*($)' "\ \"\.\\\\${WIN_SRCDIR}\\\"" 1									\
		"installer.nsi" '(^!define USER_DATA_DIR).*($)' "\ \"\$\{USR_DIR\}\\${SHORTPROGRAMNAME}\"" 1			\
		"${SRCDIR}/Application.rc" "(^aICON ICON).*($)" "\ \"${WIN_BITMAPDIR}icon-Application.ico\""	1		\
		"${SRCDIR}/configuracion.xml" "(<app_name>).*(<\/app_name>)" "${PROGRAMNAME}"	1							\
		"${SRCDIR}/configuracion.xml" "(<app_short_name>).*(<\/app_short_name>)" "${SHORTPROGRAMNAME}"	1	\
		"${SRCDIR}/CMakeFiles.txt" "(^SET\(MAIN_SOURCE\s+).*(\)$)" "${MAINCLASSNAME}.cpp" 1						\
		"installer_data.sh" "(^SHORTPROGRAMNAME=).*($)" "\"${SHORTPROGRAMNAME}\""	1								\
		"installer_data.sh" "(^MIMEEXTENSION=).*($)" "${MIMEEXTENSION}"	1											\
		"installer_data.sh" "(^SRCDIR=).*($)" "${SRCDIR}"	1																\
		"installer_data.sh" "(^BITMAPDIR=).*($)" "${BITMAPDIR}"	1														\
		"installer_data.sh" "(^EXECUTABLENAME=).*($)" "${EXECUTABLENAME}"	1											\
		"installer_data.sh" "(^EXECUTABLENAMELOWERCASE=).*($)" "${EXECUTABLENAMELOWERCASE}"	1					\
		"installer_data.sh" "(^ICON=).*($)" "${ICON}"	1																	\
		"installer_data.sh" "(^MIMEICON=).*($)" "${MIMEICON}"	1															\
		"installer_data.sh" "(^DESKTOPNAME=).*($)" "${DESKTOPNAME}"	1													\
		"installer_data.sh" "(^LAUNCHSCRIPT=).*($)" "${LAUNCHSCRIPT}"	1												\
		"installer_data.sh" "(^PROJECT=).*($)" "${PROJECT}" 1																\
		"Application.doxyfile" "(^.*PROJECT_NAME.*=).*($)" "\ ${PROGRAMNAME}"	1									\
		"${SRCDIR}/${MAINCLASSNAME}.cpp" "()<::MainClass::>()" "${MAINCLASSNAME}"	1								\
		"${SRCDIR}/${MAINCLASSNAME}.h" "()<::MainClass::>()" "${MAINCLASSNAME}"	1									\
		"${SRCDIR}/${MAINCLASSNAME}.h" "()<::MAINCLASS_H_::>()" "${MAINCLASSUPPER}_H_"	1						\
	)
	for ((i=0;i<${#REPLACEMENTS[*]};i+=1)); do
		FILE=${REPLACEMENTS[${i}]}
		let "i = $i + 1"
		REGEXP=${REPLACEMENTS[${i}]}
		let "i = $i + 1"
		FILL=${REPLACEMENTS[${i}]}
		let "i = $i + 1"
		if [ ${REPLACEMENTS[${i}]} == ${FILE_ID} ] ; then
			sed_file "${FILE}" "${REGEXP}" "${FILL}" || showmessage "[ SIN MODIFICAR ]"
		fi
	done
}



if [ "${SRCDIR}/MainClass.cpp" != "${SRCDIR}/${MAINCLASSNAME}.cpp" ] ; then
	mv -u "${SRCDIR}/MainClass.cpp" "${SRCDIR}/${MAINCLASSNAME}.cpp"
fi
if [ "${SRCDIR}/MainClass.h" != "${SRCDIR}/${MAINCLASSNAME}.h" ] ; then
	mv -u "${SRCDIR}/MainClass.h" "${SRCDIR}/${MAINCLASSNAME}.h"
fi

if [ ! ${!SVN[@]} ] ; then
	showmessage "Creando estructura basica de SVN en ${SVN_URL}/${SVN_MODULE}"
	svn mkdir -q --parents -m "bootstrap: Estructura basica de SVN" ${SVN_URL}/${SVN_MODULE}/tags ${SVN_URL}/${SVN_MODULE}/branches ${SVN_URL}/${SVN_MODULE}/trunk
	showmessage "Obteniendo arbol SVN"
	svn co -q ${SVN_URL}/${SVN_MODULE}/trunk 
fi

# Obtengo la base desde el repositorio
showmessage "Trayendo archivos base"
svn export -q --force ${SVN_URL}/${SVN_BOOT} trunk
if [ ! ${!SVN[@]} ] ; then
	showmessage "Agregando archivos base"
	svn add -q --depth=infinity trunk/*

	showmessage "Generando propiedades svn:externals"
	#echo "src/CMakeLists.txt	${SVN_URL}/${SVN_BOOT}/src/CMakeLists.txt" >> externals.prop
	#echo "src/data/gdal	${SVN_URL}/${SVN_BASE}/${SVN_BRANCH}/data/gdal" >> externals.prop
	showmessage "Aplicando propiedades svn:externals"
	# si quiero mas de un external tengo que usar el archivo
	echo "Toolchain-mingw32.cmake	${SVN_URL}/${SVN_TOOLS}/Toolchain-mingw32.cmake" > externals.prop
	echo "installer.sh ${SVN_URL}/${SVN_TOOLS}/installer.sh" >> externals.prop
	svn propset -q svn:externals -F externals.prop trunk/
	svn propset -q svn:externals "CMakeLists.txt	${SVN_URL}/${SVN_TOOLS}/CMakeLists.txt" trunk/${SRCDIR}/
	svn propset -q svn:externals "gdal	${SVN_URL}/${SVN_BOOT}/${SRCDIR}/data/gdal" trunk/${SRCDIR}/data/
	#svn propset -q svn:externals  trunk/${SRCDIR}
	#svn propset -q svn:externals "resources	${SVN_URL}/${SVN_BASE}/${SVN_BRANCH}/lib/resources" trunk/lib
	svn propset -q svn:externals "suri	${SVN_URL}/${SVN_BASE}/${SVN_BRANCH}" trunk/lib/
	rm -f externals.prop
	
	#svn ci -q -m "bootstrap: svn:externals comunes a los proyectos" trunk
	#showmessage "Trayendo externals"
	#svn up -q trunk
	#showmessage "Haciendo commit de los svn:externals"
	#svn ci -m "bootstrap: commit de los svn:externals" trunk
	
	svn ci --depth=infinity -qm "bootstrap: Enviando propiedades" trunk/
	cd trunk/
	# Me traigo los externals
	showmessage "Trayendo archivos externos"
	svn up -q --depth=infinity
	
else
	showmessage "Obteniendo archivos comunes"
	svn export -q --force ${SVN_URL}/${SVN_TOOLS}/Toolchain-mingw32.cmake trunk/Toolchain-mingw32.cmake
	svn export -q --force ${SVN_URL}/${SVN_TOOLS}/installer.sh trunk/installer.sh
	svn export -q --force ${SVN_URL}/${SVN_TOOLS}/CMakeLists.txt trunk/${SRCDIR}/CMakeLists.txt
	svn export -q --force ${SVN_URL}/${SVN_BOOT}/${SRCDIR}/data/gdal trunk/${SRCDIR}/data/
	svn export -q --force ${SVN_URL}/${SVN_BASE}/${SVN_BRANCH} trunk/lib/suri/
	
	cd trunk/
fi



# Reemplazo los datos
showmessage "Aplicando datos a los archivos iniciales"
replace_variables

showmessage "Modificando archivos con la version inicial"
bash lib/suri/tools/setversion.sh ${INITIALVERSION}
showmessage "Limpiando estructuras y generando manifiesto inicial"
bash lib/suri/tools/cleanup.sh

PREVDIR=$(pwd)
showmessage "Generando archivos de traduccion iniciales"
cd ${SRCDIR} && bash ${PREVDIR}/lib/suri/tools/translate.sh ${PREVDIR}/${SRCDIR}/locale/es/Application.po
cd ${PREVDIR}

if [ ! ${!SVN[@]} ] ; then
	showmessage "Commiteando los archivos base"
	svn ci -q -m "bootstrap: Archivos base del proyecto" .
fi

#OLD_IFS=${IFS}
#IFS=$'\n'
#showmessage "Copiando estructura basica"
#for file in ./* ; do
#	if [ "${file}" != "./trunk" ] ; then
#		cp -Rn "${file}" trunk
#	fi
#done
#IFS="${OLD_IFS}"
#showmessage "Agregando los archivos"
#svn add -q --depth=infinity trunk/*

showmessage "Compilando por primera vez"
#cd trunk
touch ${SRCDIR}/svn_version.h

cmake-gui .

#cmake -DCMAKE_BUILD_TYPE=Debug													\
#		-DCMAKE_CXX_FLAGS="-Wall -Werror -Wno-format-security				\
#								-Wno-format-nonliteral -Wno-write-strings"	\
#		-DCMAKE_C_FLAGS="-Wall -Werror -Wno-format-security				\
#								-Wno-format-nonliteral -Wno-write-strings"	\
#		-DSVNVERSION=svnversion -DCMAKE_VERBOSE_MAKEFILE=FALSE

make
cd ..

echo FIN
