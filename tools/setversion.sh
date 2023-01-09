#!/bin/bash

set -e
set -u


showhelp(){
	echo "setversion.sh : Cambia las referencias a la version del software, realiza el commit y un tag"
	echo ""
	echo "MODO DE USO: $0 $1"
	echo -e "Notas:\n$2\n"
	if [ $# \> 2 ] ; then
		echo "ERROR: $3"
	fi
	exit -1
}


change_version() {
	FILENAME="$1"
	shift
	EXPR="$1"
	shift
	VER="$1"
	shift

	if [ ! -f "${FILENAME}" ] ; then
		showmessage "No existe ${FILENAME}"
		exit -1
	fi
	showmessage "Actualizando ${FILENAME} a la version ${VER}" ""
	tmp_file="/tmp/tmp-$$.expr"
	rm -f "$tmp_file"
	mv "${FILENAME}" "$tmp_file"
	sed -r "s/${EXPR}/\1${VER}\2/gi" "$tmp_file" > "${FILENAME}"
	! cmp -s "$tmp_file" "${FILENAME}" || return 1
	rm -f "$tmp_file"
	showmessage "[ OK ]"
}

NO_NEWLINE="no-newline"
ADDSTAR=1
showmessage(){
	if [ $ADDSTAR == 1 ] ; then
		echo -n " * "
	fi
	ADDSTAR=1
	if [ $# == 1 ] ; then
		echo "$1"
	else
		echo -n "$1 "
		ADDSTAR=0
	fi
}

die() {
	showmessage "Terminando con error" ""
	if [ $# != 0 ] ; then
		showmessage "[ ERROR: $1 ]" ""
	fi
	showmessage ""
	exit -1
}

dosvn(){
	VER="$1"
	shift
	SVN_URL="$1"
	shift
	SVN_MODULE="$1"
	shift
	SVN_TAG="$1"
	shift
	DESCRIPCION="$1"
	shift
	LIB_SVN_TAG="$1"

	showmessage "Realizando tareas SVN"


	# Modifico externals en ${MODULE} para que apunte al tag en Suri-2.0
	showmessage "Modificando svn:externals en lib/" $NO_NEWLINE
	svn propset svn:externals "suri ${SVN_URL}/Suri-2.0/${LIB_SVN_TAG}"			\
						lib/ >/dev/null 2>&1 || die "al modificar svn:externals"
	showmessage "[ OK ]"
	
	# Copio la version local al repositorio en forma de un TAG
	showmessage "Aplicando TAG a la copia local" $NO_NEWLINE
	svn copy -m "Tag de ${MODULE} version #:${VER}\n${DESCRIPCION}" . 			\
								${SVN_URL}/${SVN_MODULE}/${SVN_TAG} >/dev/null 2>&1 ||\
								die "al commitear"
	showmessage "[ OK ]"
	
	#  Copio la version local de lib/suri al repositorio en forma de un TAG
	showmessage "Aplicando TAG en Suri-2.0" $NO_NEWLINE
	svn copy -m "Tag de Suri-2.0, para ${MODULE} version #:${VER}\n${DESCRIPCION}"	\
								lib/suri ${SVN_URL}/Suri-2.0/${LIB_SVN_TAG} >/dev/null 2>&1 ||	\
															die "al aplicar tag en Suri-2.0"
	showmessage "[ OK ]"
	
	showmessage "Revirtiendo svn:externals en lib/" $NO_NEWLINE
	svn revert lib/ >/dev/null 2>&1 || die "al revertir externals"
	showmessage "[ OK ]"
}

hudson(){
	HUDSON_URL="$1"
	shift
	HUDSON_JOB="$1"
	shift
	VER="$1"
	shift
	SVN_TAG="$1"
	shift
	HUDSON_USER="$1"
	shift
	HUDSON_PASSWORD="$1"
	shift
	showmessage "Disparando build automatico" ""
	wget --auth-no-challenge --http-user="${HUDSON_USER}" --http-password="${HUDSON_PASSWORD}" "${HUDSON_URL}/job/${HUDSON_JOB}/buildWithParameters?token=setversion.sh&cause=\"Lanzado desde script setversion.sh\"&delay=0sec&REPOSITORY=${SVN_TAG}&DIRECTORIO=version_${VER}" -O- >/dev/null|| die "al intentar disparar build"
	showmessage "[ OK ]"
}

upgrade(){
	VER="$1"
	shift
	FILE_ID=$1
	shift
	REPLACEMENTS=(
		"LinuxShortcut.desktop" "(^Version=).*($)"	1									\
		"default.apspec" "(^RootName: \@suremptec\.com\/.*:).*($)"	1	\
		"default.apspec" "(^SoftwareVersion: ).*($)"	1									\
		"installer.nsi" '(^.*VERSION .*").*(".*$)'	1									\
		"src/data/ayuda/about-Application.html" "(<b>Versi&oacute;n ).*(<\/b>)"	1\
		"src/messages.h" '(APPLICATION_BUILD *")[^"]*(.*$)'	1		\
		"src/Leame.txt" '(^ .*v).*\s*(.*$)'	1												\
		"Application.doxyfile" "(^.*PROJECT_NUMBER.*=\s*).*($)" 1					\
	)
	for ((i=0;i<${#REPLACEMENTS[*]};i+=1)); do
		FILE=${REPLACEMENTS[${i}]}
		let "i = $i + 1"
		REGEXP=${REPLACEMENTS[${i}]}
		let "i = $i + 1"
		if [ ${REPLACEMENTS[${i}]} == ${FILE_ID} ] ; then
			change_version "${FILE}" "${REGEXP}" "${VER}" || showmessage "[ SIN MODIFICAR ]"
			if [ ! -s "${FILE}" ] ; then
				showmessage "[ ERROR ]"
				return 1
			fi
		fi
	done
}

main(){
	VERSION=""
	REPOSITORY=""
	MODULE="Seymour"
	SVN_URL="https://svn.suremptec.com.ar/home/sur/svn"
	HUDSON_URL="http://hudson.suremptec.com.ar:8080"
	HUDSON_JOB="Seymour-Release"
	FILE_ID=1

	while test "$*" ; do
		case $1 in 
			-v|--version) shift
							VERSION="$1"
							shift
							;;
			-d|--description) shift
							DESCRIPCION="$1"
							shift
							;;
			-s|--source-dir) shift
							REPOSITORY="$1"
							if [ -z "${LIB_REPOSITORY}" ] ; then
								LIB_REPOSITORY="${REPOSITORY}"
							fi
							shift
							;;
			-u|--url) shift
							SVN_URL="$1"
							shift
							;;
			-h|--hudson) shift
							HUDSON_URL="$1"
							shift
							;;
			-j|--job) shift
							HUDSON_JOB="$1"
							shift
							;;
			-i|--file-id) shift
							FILE_ID=$1
							shift
							;;
			-m|--module) shift
							MODULE="$1"
							shift
							;;
			-l|--lib) shift
							LIB_REPOSITORY="$1"
							shift
							;;
			-r|--hudson_user) shift
							HUDSON_USR="$1"
							shift
							;;
			-p|--hudson_password) shift
							HUDSON_PWD="$1"
							shift
							;;
			*) VERSION="$1"
							shift
							;;
		esac
	done

	if [ -z "$VERSION" ] ; then
		HELP=""
		HELP="${HELP}\n <-d | --description> 		e.g. \"Version Release\""
		HELP="${HELP}\n <-s | --source-dir> 		Directorio de las fuentes dentro del modulo, relativo a url. Si no esta definido, no realiza operaciones de SVN. e.g. trunk"
		HELP="${HELP}\n <-u | --url> 			default = ${SVN_URL}"
		HELP="${HELP}\n <-h | --hudson> 		default = ${HUDSON_URL}"
		HELP="${HELP}\n <-j | --job> 			default = ${HUDSON_JOB}"
		HELP="${HELP}\n <-m | --module>		Modulo del SVN a generar. default = Seymour"
		HELP="${HELP}\n <-i | --file-id> 		Identificador de los archivos a modificar. default = ${FILE_ID}"
		HELP="${HELP}\n <-l | --lib>			Nombre del Tag para la biblioteca. default = version_<version>"
		HELP="${HELP}\n <-r | --hudson_user>		Usuario de Hudson. Usado para lanzar la compilacion"
		HELP="${HELP}\n <-p | --hudson_password>	Password de Hudson. Usado para lanzar la compilacion"
		HELP="${HELP}\n"
		HELP="${HELP}\n Ejemplo : setversion.sh <nro_de_version> -m <Seymour> -l <tags/version_nro_de_version_de_surilib> -s <tags/version_nro_de_version> -d <\"Version Release\">"
		showhelp "[version] Options" "${HELP}" "Cantidad de parametros incorrecta"
	fi

	upgrade ${VERSION} 1
	POINTVERSION=$(echo ${VERSION} | cut -d '.' -f 1,2)
	upgrade ${POINTVERSION} 0
	showmessage ""

	if [ ! -z $REPOSITORY ] ; then
		dosvn "${VERSION}" "${SVN_URL}" "${MODULE}" "${REPOSITORY}" "${DESCRIPCION}" "${LIB_REPOSITORY}"
		showmessage ""
		# Command Line Hudson
		hudson "${HUDSON_URL}" "${HUDSON_JOB}" "${VERSION}" "${REPOSITORY}" "${HUDSON_USR}" "${HUDSON_PWD}"
		showmessage ""
	fi
}

main "$@"
showmessage "EOP"
