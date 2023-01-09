#!/bin/bash

if [ $# == 0 ] ; then
	echo "Forma de uso: script <check_suri o check_app>"
	echo " donde: "
	echo " - check_suri verifica la biblioteca"
	echo " - check_app verifica la aplicacion"
	exit 1
fi

if [ "$1" == "check_suri" ] ; then
	MESSAGES="lib/suri/include/suri/messages.h"
	INCLUDE_DIR="lib/suri"
	EXCLUDE_DIR="src"
else
	MESSAGES="src/messages.h"
	INCLUDE_DIR="src"
	EXCLUDE_DIR="lib/suri"
fi

echo "Buscando defines en ${MESSAGES}"
for define in $(cat ${MESSAGES} | grep -e '^#define' | sed -r 's/^#define\s+(\w*).*$/\1/g') ; do
	# busca si se usa en SRC
	used_in_src=$(grep -Rn -e "[^A-Z^a-z^_]${define}[^A-Z^a-z^_]" . --include=*.{cpp,h} | grep -v "${EXCLUDE_DIR}" | grep -v ${MESSAGES} | sed -r 's/\*/\\\*/g')
	# busca si se usa en SURI
	used_in_suri=$(grep -Rn -e "[^A-Z^a-z^_]${define}[^A-Z^a-z^_]" . --include=*.{cpp,h} | grep -v "${INCLUDE_DIR}"| grep -v ${MESSAGES} | sed -r 's/\*/\\\*/g')
	in_messages=$(cat ${MESSAGES} | grep $define | wc -l)

	if [[ (-n $used_in_src && $used_in_src != " ") || $in_messages -gt 1 ]] ; then
		echo -n ""
		#echo -n " [ SIRVE ] "
		#echo $define \($in_messages\)-\> $used_in_src
	else
		echo -n $define
		echo " [ ELIMINANDO ($in_messages) ($used_in_src)] "
		sed -r "s/(^.*)$define(.*$)//g" < ${MESSAGES} > /tmp/sedded && mv /tmp/sedded ${MESSAGES}
	fi
	used_in_src=""

	if [[ (-n $used_in_suri && $used_in_suri != " ") || $in_messages -gt 1 ]] ; then
		echo -n $define
		echo " [ ATENCION SE USA EN ${EXCLUDE_DIR} (${used_in_src})] "
	fi
	used_in_src=""
done
