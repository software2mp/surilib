#!/bin/bash




max_revision=1
if [ -t 1 ] ; then
	black='\E[30;40m'
	red='\E[31;40m'
	green='\E[32;40m'
	yellow='\E[33;40m'
	blue='\E[34;40m'
	magenta='\E[35;40m'
	cyan='\E[36;40m'
	white='\E[37;40m'
	normal='\E[0;40m'
	bold='\E[1;40m'
else
	black=''
	red=''
	green=''
	yellow=''
	blue=''
	magenta=''
	cyan=''
	white=''
	normal=''
	bold=''
fi

level_to_string=(
	"#quiet"
	"#info"
	"#warning"
	""
	""
	"#error"
	"#verbose"
	"#debug"
	)
debug_level=7
verbose_level=6
error_level=5
warning_level=2
info_level=1
quiet_level=0

log(){
	message="$1"
	shift
	level="$1"
	shift
	file="$1"
	shift
	line="$1"
	shift
	if [ $level -le $loggin_level ] ; then
		echo "${file}:${linea}: ${level_to_string[${level}]}: ${message}." >&2
	fi
}

log_debug(){
	message="$1"
	shift
	log "${message}" ${debug_level} $*
}

log_error(){
	message="$1"
	shift
	log "${message}" ${error_level} $*
}

log_warning(){
	message="$1"
	shift
	log "${message}" ${warning_level} $*
}

log_info(){
	message="$1"
	shift
	log "${message}" ${info_level} $*
}

# Actual = WARNING
loggin_level=$warning_level
log_function=log_warning
etiquette_templates="$(dirname $0)/etiquette.templates"

get_template()
{
	local template_name="$1"
	shift
	local template_file="$(dirname $0)/${template_name}_template.file"
	eval $1=\"${template_file}\"
	shift

	set -f
	sed -n "/# begin ${template_name}/,/# end ${template_name}/p" "${etiquette_templates}"|grep -v "# begin"|grep -v "# end"|tr -d '\r' > "${template_file}"
	
	while [ $# -gt 0 ] ; do
		key=$(echo $1|cut -d = -f 1)
		value=$(echo $1|cut -d = -f 2-)
		shift
		cat "${template_file}" | sed "s/\${${key}}/${value}/g" > "${template_file}.tmp"
		rm -Rf "${template_file}"
		mv "${template_file}.tmp" "${template_file}"
	done
	set +f
	return 0
}

find_differences()
{
	local file="$1"
	shift
	local message="$1"
	shift
	local template_file="$1"
	shift
	local checked_file="$1"
	shift

	cmp -s "${template_file}" "${checked_file}"
	
	if [ $? ] ; then
		count=1
		set -f
		# El sed sirve para que los \ del template pasen a la variable obtenido
		cat "${checked_file}" | sed -e 's/\\/\\\\/g' | while read obtenido ; do
			linea=$(echo "${obtenido}"|cut -d ":" -f 1)
			obtenido=$(echo "${obtenido}"|cut -d ":" -f 2-)
			esperado="$(cat ${template_file} | sed -n ${count}p)"
			if [ "$(echo ${obtenido}|tr [A-Z] [a-z])" != "$(echo ${esperado}|tr [A-Z] [a-z])" ] ; then
				$log_function "${message}. Esperado: \"${esperado}\"." ${file} ${linea} 
				$log_function "${message}. Obtenido: \"${obtenido}\"." ${file} ${linea}
			fi
			count=`expr ${count} + 1`
		done
		set +f
	fi
	return 0
}

find_extras(){
	local file="$1"
	shift
	local message="$1"
	shift
	local template_file="$1"
	shift
	local checked_file="$1"
	shift
	if [ -s "${checked_file}" ] ; then
		count=1
		set -f
		# El sed sirve para que los \ del template pasen a la variable obtenido
		cat "${checked_file}" | sed -e 's/\\/\\\\/g' | while read obtenido ; do
			linea=$(echo "${obtenido}"|cut -d ":" -f 1)
			obtenido=$(echo "${obtenido}"|cut -d ":" -f 2-)
			esperado="$(cat ${template_file} | sed -n ${count}p)"
			if [ "$(echo ${obtenido}|tr [A-Z] [a-z])" != "$(echo ${esperado}|tr [A-Z] [a-z])" ] ; then
				$log_function "${message}. No debe usarse: \"${obtenido}\"." ${file} ${linea}
			fi
			count=`expr ${count} + 1`
		done
		set +f
	fi
	return 0
}

check_file_header()
{
	local checked_file="$1"
	local template_type="$2"

	local file_header="$(dirname $0)/file_header.file"
	
	# Verifica el header del archivo
	get_template "heading-${template_type}" template_header "filename=$(basename $1)" "year=$(date +%Y)"
	
	lines=$(cat ${template_header}|wc -l)
	lines=`expr ${lines}`
	cat "${checked_file}"|tr -d '\r'|head -n ${lines} | grep -nE ".*" > "${file_header}"

	find_differences "${checked_file}" "Header de archivo incorrecto" "${template_header}" "${file_header}"

	# Verifica los includes
	get_template "includes-${template_type}" template_includes
	regexp=$(cat ${template_includes} | sed -e 's/\/\/\s\s*/\/\/\\s*/g'|tr '\n' '|')
	regexp=${regexp%\|}
	cat "${checked_file}" | tr -d '\r'| grep -niE "(${regexp})" > "${file_header}"

	find_differences "${checked_file}" "Header de archivo incorrecto" "${template_includes}" "${file_header}"
	rm -Rf "${template_includes}"

	# Verifica que no sobren includes
	get_template "no-includes-${template_type}" template_no_includes
	regexp=$(cat ${template_no_includes} | sed -e 's/\/\/\s\s*/\/\/\\s*/g'|tr '\n' '|')
	regexp=${regexp%\|}
	cat "${checked_file}" | tr -d '\r'| grep -niE "(${regexp})" > "${file_header}"
	
	find_extras "${checked_file}" "Header de archivo incorrecto" "${template_no_includes}" "${file_header}"
	rm -Rf "${template_no_includes}"
	
	rm -Rf "${file_header}"
	
	return 0
}

file="$1"
mode="$2"
exit_value=0

check_files()
{
   included_dir="$1"
   shift
   dir_mode="$1"
   shift
   return_value=0
   for file in $(find . -iname "*.cpp" -or -iname "*.h" | grep "${included_dir}" | grep -v CMake | grep -v Test. | grep -v Tests.) ; do
      check_file_header "${file}" "${dir_mode}" || return_value=1
   done
   return ${return_value}
}

if [ $# == 0 ] ; then
   #echo "Comprobando etiqueta de los archivos de la biblioteca SURILIB desde revision $max_revision"
   check_files "suri/" "suri" || exit_value=1
   
   #echo "Comprobando etiqueta de los archivos de la APLICACION desde revision $max_revision"
   check_files "src/" "app" || exit_value=1
else
   check_file_header "$file" "$mode" || exit_value=1
fi

exit ${exit_value}




