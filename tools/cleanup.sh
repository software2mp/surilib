#!/bin/bash

DATAPATH="src/data"
HTMLPATH="${DATAPATH}/ayuda"
HTMLIMAGEPATH="${HTMLPATH}/Imagenes"

# busca las imagenes en los html y borra las que no se usan dentro de un tag
# img
for image in ${HTMLIMAGEPATH}/* ; do
	grep -Eq img.*\".*$(basename ${image})\" "${HTMLPATH}"/*.html
	if [ $? == 0 ] ; then
		echo found $image
		if [ ! -f ${image} ] ; then
			echo "ADVERTENCIA: Se encontro referencia a ${image}, pero no existe en el directorio"
		fi
	else
		rm "${image}"
	fi
done

# convierte la variable para que se pueda usar en SED
seddatapath="$(echo ${DATAPATH}/ | sed -e 's/\//\\\//g')"
# genera el manifiesto con los archivos de data/
find "${DATAPATH}" -type f | grep -v .svn | grep -v manifiesto | sort -n | sed -e "s/^${seddatapath}//g" > "${DATAPATH}"/manifiesto
