#!/bin/bash

set -e
set -u

# Cargo variables custom (puedo sobreescribir)
source installer_data.sh

# Directorio donde se hace el paquete de instalacion
PACKAGEDIR=package/
# Directorio (fuera del arbol de fuentes) donde guarda los archivos
# de autopackage
AUTOPACKAGELOCALDIR=$HOME/opt/
# nombre del ejecutable
EXECUTABLENAME=$(echo ${SHORTPROGRAMNAME} | sed 's/\s*//g' | sed 's/á/a/g' | sed 's/Á/A/g' | sed 's/é/e/g' | sed 's/É/E/g' | sed 's/í/i/g' | sed 's/Í/I/g' | sed 's/ó/o/g'  | sed 's/Ó/O/g' | sed 's/ú/u/g' | sed 's/Ú/U/g' | sed 's/ñ/n/g' | sed 's/Ñ/N/g' | sed 's/\///g')
# bibliotecas a incluir en el paquete
INCLUDEDLIBS="wx|gdal|hdf|mfhdf|proj|jasper|geos|geotiff|suri|curl|expat|kml|muparser|suri|minizip|uriparser|sqlite|openjp2|${AUTOPACKAGELOCALDIR}"

# nombre (deseado) del programa en minuscula
EXECUTABLENAMELOWERCASE=$(echo ${EXECUTABLENAME} | tr "[:upper:]" "[:lower:]")
# directorio dentro de la instalacion para los instaladores de autopackage
AUTOPACKAGES="${PACKAGEDIR}"autopackage/packages/
# nombre del icono
ICON="${EXECUTABLENAMELOWERCASE}.png"
MIMEICON="icon-$(echo ${PROJECT:0:1}|tr "[:lower:]" "[:upper:]")${PROJECT:1}File.png"
# nombre del script de lanzamiento
LAUNCHSCRIPT="${EXECUTABLENAME}.sh"
# desktop file
DESKTOPNAME="${EXECUTABLENAME}.desktop"

download_makepackage(){
	PACKAGENAME="$1"
	shift
	PACKAGESOURCE="$1"
	shift
	PACKAGEURL="$1"
	shift
	# si no existe el local, lo bajo
	if [ ! -f ${PACKAGESOURCE}/${PACKAGENAME} ] ; then
		mkdir -p ${PACKAGESOURCE}
		echo -n " * bajando - ${PACKAGENAME} -> ${PACKAGESOURCE}/${PACKAGENAME}"
		set +e
		set +u
		wget "${PACKAGEURL}" -O "${PACKAGESOURCE}/${PACKAGENAME}" -o wget-error.log
		if [ ! $? ] ; then
			echo " [ FALLA ]"
			cat wget-error.log
			rm "${PACKAGESOURCE}/${PACKAGENAME}"
		else
			echo " [ok]"
		fi
		set -e
		set -u
		rm -f wget-error.log
	fi
}

copy_files(){
	for ((i=0;i<$(eval echo \${#$1[*]});i+=1)); do
		SRC=$(eval echo \${$1[${i}]})
		let "i = $i + 1"
		DEST=$(eval echo \${$1[${i}]})
		echo -n " * Copiando ${SRC} -> ${DEST}"
		if [ -d ${SRC} ] ; then
			cp -R "${SRC}" "${DEST}"
		else
			DESTDIR=${DEST%/*}
			if [ ! -d ${DESTDIR} ] ; then
				mkdir -p "${DESTDIR}"
			fi
			if [ -f "${SRC}" ] ; then
				cp "${SRC}" "${DEST}"
			else
				echo " [ No se encontro ${SRC} ]"
				continue
			fi
		fi
		echo " [ OK ]"
	done
}

echo -n LIMPIANDO DIRECTORIO ${PACKAGEDIR}
rm -Rf "${PACKAGEDIR}" && echo " [ OK ]"
mkdir -p "${PACKAGEDIR}"/lib


echo COPIANDO ARCHIVOS
# copio los paquetes, si no existen, los baja
download_makepackage autopackage.tar.bz2 ${AUTOPACKAGELOCALDIR} http://autopackage.googlecode.com/files/autopackage-1.4.2-x86.tar.bz2
download_makepackage autopackage-gtk-1.4.2.package ${AUTOPACKAGELOCALDIR} http://autopackage.googlecode.com/files/autopackage-gtk-1.4.2.package

ARRAY=("${SRCDIR}/${EXECUTABLENAME}" "${PACKAGEDIR}"									\
		"default.apspec" "${PACKAGEDIR}"														\
		"${SRCDIR}/data" "${PACKAGEDIR}"														\
		"${BITMAPDIR}/icon-Application-48.png" "${PACKAGEDIR}hicolor/48x48/apps/${ICON}"			\
		"${BITMAPDIR}/icon-MimeFile-48.png" "${PACKAGEDIR}hicolor/48x48/mimetypes/${MIMEICON}"	\
		"LinuxShortcut.desktop" "${PACKAGEDIR}${DESKTOPNAME}"							\
		"${SRCDIR}/configuracion.xml" "${PACKAGEDIR}data/"								\
		${AUTOPACKAGELOCALDIR}autopackage.tar.bz2 "${AUTOPACKAGES}"					\
		${AUTOPACKAGELOCALDIR}autopackage-gtk-1.4.2.package "${AUTOPACKAGES}"	\
		application.xml "${PACKAGEDIR}application-${MIMEEXTENSION}.xml")
copy_files ARRAY

# genera el script bash de lanzamiento
echo -n " * Generando script de inicio en ${PACKAGEDIR}${EXECUTABLENAMELOWERCASE}"
echo -e \#\!/bin/bash \\n \\nCALLNAME=\$0 \\nCALLDIR=\$\{CALLNAME\%/*\} \\nexport LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\"\$CALLDIR/../lib\" \\n\"\$CALLDIR\"/"${EXECUTABLENAME}" '"$1"' > "${PACKAGEDIR}${LAUNCHSCRIPT}"
echo " [ OK ]"

# hago ejecutables al binario y al script
chmod a+x "${PACKAGEDIR}${EXECUTABLENAME}"
chmod a+x "${PACKAGEDIR}${LAUNCHSCRIPT}"

echo "ACHIVOS COPIADOS [ OK ]"

echo COPIANDO DEPENDENCIAS DINAMICAS
j=0
ARRAY=""
# armo el array con los archivos .so necesarios
for i in $(ldd "${PACKAGEDIR}${EXECUTABLENAME}" | grep -E "${INCLUDEDLIBS}" | cut -d ' ' -f 3); do
	ARRAY[${j}]="${i}"
	let "j = $j + 1"
	ARRAY[${j}]="${PACKAGEDIR}"lib
	let "j = $j + 1"
done
copy_files ARRAY

echo -n ELIMINANDO DIRECTORIOS SVN
for hidden_file in $(find "${PACKAGEDIR}" -name ".*") ; do
	rm -Rf ${hidden_file}
done
echo " [ OK ]"

if [ `which makepackage` ] ; then
	echo -n "GENERANDO INSTALADOR" ; set -x
	cd "${PACKAGEDIR}"
	makepackage default.apspec > package.log 2>&1
	if [ ! $? ] ; then
		echo " [ FALLA ]" ; cat package.log ; rm package.log ; exit 1
	fi
	echo " [ OK ]"
	cat package.log
	rm package.log
	cd ..
	cp "${PACKAGEDIR}"/*.package .
fi

echo FIN
exit 0