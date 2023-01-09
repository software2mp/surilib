#!/bin/bash


set -e
set -u

SVN_URL="https://svn.suremptec.com.ar/svn/src"
SVN_TO_URL=${SVN_URL}
BRANCH_ROOT="root"

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
	showmessage "Salio con \$? = $?" ""
	if [ $# != 0 ] ; then
		showmessage "[ ERROR: $1 ]" ""
	fi
	showmessage ""
	exit -1
	return 1
}

get_directory(){
	local REMOTE_DIR="$1"
	shift
	local DESTINATION="$1"
	shift

	if [ ! -d "${DESTINATION}" ] ; then
		showmessage "Obteniendo raiz de ${SVN_TO_URL} -> ${DESTINATION}" $NO_NEWLINE
		svn checkout --depth=empty --ignore-externals ${SVN_TO_URL} "${DESTINATION}" > /tmp/svnout 2>&1 || (echo "" ; cat /tmp/svnout && die "obtener ${REMOTE_DIR}")
		showmessage "[ OK ]"
	fi

	oldIFS=${IFS}
	IFS='/'
	svn_path=()
	for dir in ${REMOTE_DIR} ; do
		IFS=${oldIFS}
		svn_path[${#svn_path[*]}]="${dir}"
	done
	accum_dir=""
	for ((i=0;i<${#svn_path[*]}-1;i+=1)); do
		dir="${svn_path[${i}]}"
		accum_dir=${accum_dir}/${dir}
		if svn_exists ${SVN_TO_URL}/${accum_dir} ; then
			showmessage "Obteniendo directorio ${DESTINATION}${accum_dir}" $NO_NEWLINE
			svn update --depth=empty --ignore-externals ${DESTINATION}${accum_dir} > /tmp/svnout 2>&1 ||
			(echo "" ; cat /tmp/svnout && die "al obtener ${accum_dir}")
		else
			showmessage "Creando directorio ${DESTINATION}${accum_dir}" $NO_NEWLINE
			mkdir ${DESTINATION}${accum_dir} > /tmp/svnout 2>&1 ||
				(echo "" ; cat /tmp/svnout && die "al crear ${accum_dir}")
			svn add ${DESTINATION}${accum_dir} > /tmp/svnout 2>&1 ||
				(echo "" ; cat /tmp/svnout && die "al agregar ${accum_dir}")
		fi
	showmessage "[ OK ]"
	done
}

svn_exists(){
	local MODULE="$1"
	shift

	set +e
	svn ls ${MODULE} > /tmp/svnout 2>&1
	retval=$?
	set -e
	return $retval
}

branch(){
	if [ $# -ne 2 ] ; then
		showmessage "Branch: Crea una rama de un modulo de SVN."
		showmessage ""
		showmessage "		FROM_MODULE: Módulo del SVN a branchear."
		showmessage "		TO_MODULE: Módulo destino."
		showmessage ""
		showmessage "		1. Si TO_MODULE existe en SVN_TO_URL realiza un update del mismo."
		showmessage "		2. Si TO_MODULE no existe en SVN_TO_URL :"
		showmessage "			a. Si SVN_TO_URL == SVN_URL realiza una copia (tag clasico)."
		showmessage "			b. Si SVN_TO_URL != SVN_URL realiza un import segudo de un add (tag de un repositorio a otro)."
		showmessage ""
		die "Parametros incorrectos."
	fi
	local FROM_MODULE="$1"
	shift
	local TO_MODULE="$1"
	shift

	get_directory ${TO_MODULE} ${BRANCH_ROOT}
	if ! svn_exists ${SVN_TO_URL}/${TO_MODULE} ; then
		if [ ${SVN_URL} == ${SVN_TO_URL} ] ; then
			showmessage "Copiando ${FROM_MODULE} -> ${BRANCH_ROOT}/${TO_MODULE}" $NO_NEWLINE
			svn copy --parents --ignore-externals ${SVN_URL}/${FROM_MODULE} ${BRANCH_ROOT}/${TO_MODULE} > /tmp/svnout 2>&1 || (echo "" ; cat /tmp/svnout && die "al copiar ${FROM_MODULE} -> ${BRANCH_ROOT}/${TO_MODULE}")
		else
			showmessage "Importando ${FROM_MODULE} -> ${BRANCH_ROOT}/${TO_MODULE}" $NO_NEWLINE
			svn export --ignore-externals ${SVN_URL}/${FROM_MODULE} ${BRANCH_ROOT}/${TO_MODULE} > /tmp/svnout 2>&1 || (echo "" ; cat /tmp/svnout && die "al importar ${FROM_MODULE} -> ${BRANCH_ROOT}/${TO_MODULE}")
			svn add ${BRANCH_ROOT}/${TO_MODULE} > /tmp/svnout 2>&1 || (echo "" ; cat /tmp/svnout && die "al agregar ${FROM_MODULE} -> ${BRANCH_ROOT}/${TO_MODULE}")
		fi
	else
		showmessage "Modulo existente, realizando checkout de ${TO_MODULE}" $NO_NEWLINE
		svn update --set-depth=infinity --ignore-externals ${BRANCH_ROOT}/${TO_MODULE} > /tmp/svnout 2>&1 || (echo "" ; cat /tmp/svnout && die "al actualizar ${BRANCH_ROOT}/${TO_MODULE}")
	fi
	showmessage "[ OK ]"
}

change_externals(){
	local MODULE="$1"
	shift
	local LIB_SVN_EXTERNAL="$1"
	shift

	# Modifico externals en ${MODULE} para que apunte al tag en Suri-2.0
	showmessage "Modificando svn:externals en ${MODULE}/lib/ -> ${LIB_SVN_EXTERNAL}" $NO_NEWLINE
	svn propset svn:externals "suri ${SVN_TO_URL}/${LIB_SVN_EXTERNAL}"			\
						${BRANCH_ROOT}/${MODULE}/lib/ >/tmp/svnout 2>&1 || (echo "" ; cat /tmp/svnout && die "al modificar svn:externals")
	showmessage "[ OK ]"
}


showhelp(){
		showmessage "TANCH: Tag y Branch para proyectos Suri svn."
		showmessage ""
		showmessage "Parametros (deben ser usados en este orden):"
		showmessage "	-l <version de biblioteca suri> (ej: 2.4.0)"
		showmessage "	-v <version de aplicacion suri> (ej: 1.90.0)"
		showmessage "	-a <modulo de la aplicacion> (default: SuriEdu)"
		showmessage "	-b [MANDATORIO, excluyente con -t] Indica que se trata de una rama. Los procesos svn generaran copias debajo del directorio branches/. (Los numeros de version seran truncados en su ultimo digito. ej: 2.4.0 -> 2.4)"
		showmessage "	-t [MANDATORIO, excluyente con -b] Indica que se trata de una rama. Los procesos svn generaran copias debajo del directorio tags/."
		showmessage "	-u URL del repositorio de origen de datos. (default: https://svn.suremptec.com.ar/svn/src)"
		showmessage "	-d URL del repositorio de destino de datos. (si no se define sera igual al de origen)"
		showmessage "	-f Origen de los datos (default: para -b trunk; para -t branches)"
		showmessage "	-m Origen de los datos (default: para -b trunk; para -t branches)"
		showmessage ""
		showmessage "$1"
}

check_variable() {
	if [ -z "$1" ] ; then
		showhelp "$2"
		exit 0
	fi
}


check_params() {
	set +u
	check_variable "${SURI_MODULE}" "No se definio el modulo suri."
	check_variable "${SURI_VERSION}" "No se definio la version de suri <-l #.#.#>"

	check_variable "${APPLICATION_MODULE}" "No se definio el modulo de la aplicacion."
	check_variable "${APP_VERSION}" "No se definio la version de la aplicacion <-v #.#.#>"

	check_variable "${FROM}" "No se definio el origen de la copia."
	check_variable "${TARGET}" "No se definio el tipo de operacion <-t: tag, -b: branch>"
	set -u
}



# Variables calculadas o hardcodeadas
APPLICATION_MODULE="SuriEdu"
SURI_MODULE="Suri-2.0"
VERSION_PREFIX="version_"

while test "$*" ; do
	case $1 in
		-l|--library_version) shift
			SURI_VERSION="$1"
			SURI_FULL_VERSION="$SURI_VERSION"
			shift
			;;
		-v|--version) shift
			APP_VERSION="$1"
			APP_FULLVERSION="$APP_VERSION"
			shift
			;;
		-a|--application) shift
			APPLICATION_MODULE="$1"
			shift
			;;
		-u|--source-url) shift
			SVN_URL="$1"
			set +u
			if [ -z "${SVN_TO_URL}" ] ; then
				SVN_TO_URL="${SVN_URL}"
			fi
			set -u
			shift
			;;
		-d|--target-url) shift
			SVN_TO_URL="$1"
			shift
			;;
		-f|--app-from) shift
			set +u
			if [ -n "$APP_FROM" ] ; then
				showmessage "ATENCION: -f debe ir antes de -b o -t"
				exit 1
			fi
			set -u
			APP_FROM="$1"
			shift
			;;
		-m|--lib-from) shift
			set +u
			if [ -n "$SURI_FROM" ] ; then
				showmessage "ATENCION: -m debe ir antes de -b o -t"
				exit 1
			fi
			set -u
			SURI_FROM="$1"
			shift
			;;
		-b|--branch)
			# Ejemplo branch
			#  Llevan version sin el minor number.
			#  El directorio de destino es branches/version_#.#
			SURI_VERSION=${SURI_VERSION%.*}
			APP_VERSION=${APP_VERSION%.*}
			TARGET="branches"
			set +u
			FROM="trunk"
			if [ -z "$SURI_FROM" ] ; then
				SURI_FROM=${FROM}
			fi
			if [ -z "${APP_FROM}" ] ; then
				APP_FROM=${FROM}
			fi
			set -u
			OPERATION="Branch"
			shift
			;;
		-t|--tag)
			# Ejemplo tag
			#  El directorio de destino es tags/version_#.#.#
			TARGET="tags"
			set +u
			FROM="branches"
			if [ -z "$SURI_FROM" ] ; then
				SURI_FROM="${FROM}/${VERSION_PREFIX}${SURI_VERSION%.*}"
			else
				SURI_FROM="${SURI_FROM}/${VERSION_PREFIX}${SURI_VERSION%.*}"
			fi
			if [ -z "${APP_FROM}" ] ; then
				APP_FROM="${FROM}/${VERSION_PREFIX}${APP_VERSION%.*}"
			else
				APP_FROM="APP_FROM*}"
			fi
			set -u
			OPERATION="Tag"
			shift
			;;
		*)
			showmessage "Parametro $1 ignorado."
			shift
			;;
	esac
done

check_params

# Variables calculadas o hardcodeadas
SURI_DOT_VERSION=${SURI_VERSION%.*}
APP_DOT_VERSION=${APP_VERSION%.*}


main() {
	showmessage "Module: ${SURI_MODULE}"
	showmessage "SuriFrom: ${SURI_FROM}"
	showmessage "Target: ${TARGET}"
	showmessage "Prefix: ${VERSION_PREFIX}"
	showmessage "Suri Version: ${SURI_VERSION}"
	showmessage "App Module: ${APPLICATION_MODULE}"
	showmessage "AppFrom: ${APP_FROM}"
	showmessage "App Version: ${APP_VERSION}"
	showmessage "root: ${BRANCH_ROOT}"

	branch ${SURI_MODULE}/${SURI_FROM} ${SURI_MODULE}/${TARGET}/${VERSION_PREFIX}${SURI_VERSION}
	branch ${APPLICATION_MODULE}/${APP_FROM} ${APPLICATION_MODULE}/${TARGET}/${VERSION_PREFIX}${APP_VERSION}
	change_externals ${APPLICATION_MODULE}/${TARGET}/${VERSION_PREFIX}${APP_VERSION} ${SURI_MODULE}/${TARGET}/${VERSION_PREFIX}${SURI_VERSION}
	showmessage ""
	showmessage "Desea modificar la versión de ${OPERATION} corriendo setversion.sh ${APP_FULLVERSION} [Y/n]: " $NO_NEWLINE
	read ANSWER
	if [[ ${ANSWER} == "n" || ${ANSWER} == "N" ]] ; then
		echo -n ""
	else
		showmessage ""
		OLD_PWD=`pwd`
		cd ${BRANCH_ROOT}/${APPLICATION_MODULE}/${TARGET}/${VERSION_PREFIX}${APP_VERSION}
		bash ${OLD_PWD}/${BRANCH_ROOT}/${SURI_MODULE}/${TARGET}/${VERSION_PREFIX}${SURI_VERSION}/tools/setversion.sh "${APP_FULLVERSION}"
		cd "${OLD_PWD}"
	fi
	showmessage ""
	showmessage "Commando SVN a ejecutar: svn ci -m \"Sin TCK - ${OPERATION} de ${APPLICATION_MODULE} v${APP_VERSION} y ${SURI_MODULE} v${SURI_VERSION}\" ${BRANCH_ROOT}"
	showmessage ""
	showmessage "Se realizara commit de ${BRANCH_ROOT}. [ ENTER = Continuar, ^C = Abortar]: " $NO_NEWLINE
	read
	svn ci -m "Sin TCK - Tag de ${APPLICATION_MODULE} v${APP_VERSION} y ${SURI_MODULE} v${SURI_VERSION}" ${BRANCH_ROOT} >/tmp/svnout 2>&1 || (echo "" ; cat /tmp/svnout && die "al modificar svn:externals")
	showmessage " ----- Proceso realizado con exito. ----- "

}

main

