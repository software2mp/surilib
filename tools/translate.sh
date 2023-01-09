#!/bin/bash

if [ $# -lt 1 ] ; then
	echo "translate.sh [salida.po] <inputfile>"
	echo ""
	echo salida.po : mandatorio, nombre del archivo de salida
	echo inputfile : optativo, archivo sobre el que se realiza la busqueda, en caso de no definirse busca en todos los .h, .cpp y .xrc
	echo ""
	echo -n "Oneliner : cd src ; "
	echo -n "bash ../translate.sh ../src/locale/es/Application.po ; "
	echo -n "cd ../lib/resources/ ; "
	echo -n "bash ../../translate.sh ../../src/locale/es/resources.po ; "
	echo -n "cd ../suri/ ; "
	echo -n "bash ../../translate.sh locale/es/suri.po ; "
	echo "cd ../.."
	exit -1
fi

XGETTEXT=xgettext
XGETTEXTARGS="-a -C -s -j -o"
MSGMERGEARGS="-U -N"

NEWPO=temp.po
WANTEDPO="$1"

LOGFILE=error.log

rm -f "$NEWPO"
touch "$NEWPO"

if [ $# -gt 1 ] ; then
	echo "BUSCANDO STRINGS EN $2"
	xgettext $XGETTEXTARGS "$NEWPO" "$2" 2>$LOGFILE || exit -1
else
	echo "BUSCANDO STRINGS EN *.h"
	for i in $(find ./ -name "*.h" | grep -v test) ; do 
		echo PROCESANDO $i
		xgettext $XGETTEXTARGS "$NEWPO" "$i" 2>$LOGFILE || exit -1
	done

	echo "BUSCANDO STRINGS EN *.cpp"
	for i in $(find ./ -name "*.cpp" ! -name "*_xrced.cpp" | grep -v test) ; do
		echo PROCESANDO $i
 		xgettext $XGETTEXTARGS "$NEWPO" "$i" 2>$LOGFILE || (cat $LOGFILE && exit -1)
	done

	echo "BUSCANDO STRINGS EN *.xrc"
	for i in $(find ./ -name "*.xrc") ; do 
		echo PROCESANDO $i
		wxrc -g "$i" > "${i}.cpp"
		xgettext $XGETTEXTARGS "$NEWPO" "${i}.cpp" 2>$LOGFILE
		if [ $? == -1 ] ; then cat $LOGFILE && exit -1 ; fi
		rm "${i}.cpp"
	done
fi

if [ -f "$WANTEDPO" ] ; then
	echo "AGREGANDO AL CATALOGO EXISTENTE"
	msgmerge $MSGMERGEARGS "$WANTEDPO" "$NEWPO" 2>$LOGFILE || exit -1
	rm "$NEWPO"
else
	echo "NUEVO CATALOGO"
	mv $NEWPO $WANTEDPO
fi

rm -f $LOGFILE
