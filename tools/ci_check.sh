#!/bin/sh

script_dir="$(dirname "$0")"
if [ -n "${SKIP_ANALISIS}" ] ; then
	echo "No se realizara el analisis del codigo fuente"
else
	set +e
	find . -name '*_xrced.cpp' | xargs -r rm -f

#	if [ -f "${script_dir}/etiquette_checker.sh" ] ; then
#		bash "${script_dir}/etiquette_checker.sh"
#	fi

	cppcheck -I $WORKSPACE/lib/suri/include --suppressions $HOME/cppcheck_supressions.lst --auto-dealloc $HOME/cppcheck_auto_dealloc.lst -q --enable=all --xml . 2>cppcheck.xml

#	find . -name *.cpp | grep -v CMakeFiles | grep -v _xrced.cpp | cccc --outdir=. -
	java -Xmx512m -cp :$HOME/opt/pmd/lib/asm-3.1.jar:$HOME/opt/pmd/lib/jaxen-1.1.1.jar:$HOME/opt/pmd/lib/junit-4.4.jar:$HOME/opt/pmd/lib/pmd-4.2.5.jar net.sourceforge.pmd.cpd.CPD --minimum-tokens 100 --files . --language cpp --format xml > cpd.xml
	mkdir -p docs/
#	cat Application.doxyfile ; echo "OUTPUT_DIRECTORY=$WORKSPACE/../../docs" | doxygen -
	doxygen Application.doxyfile
	set -e
fi

exit 0
