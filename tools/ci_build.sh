#!/bin/bash

script_dir="$(dirname "$0")"
# Si la variable OUT_OF_SOURCE_BUILD no es alguna variante de true, entonces la
# elimina. Esto funciona por si se asigna como false u otra cosa
if [ `echo "${OUT_OF_SOURCE_BUILD}" | tr '[:upper:]' '[:lower:]'` != "true" ] ; then
 unset OUT_OF_SOURCE_BUILD
fi

clean() {
	# si quiero que sea por fuera del arbol de fuentes
	if [ -z "${OUT_OF_SOURCE_BUILD}" ] ; then
		find . -type f -name "cmake_install.cmake" -exec rm -f {} \;
		find . -type f -name "Makefile" -exec rm -f {} \;
		find . -type f -name "CMakeCache.txt" -exec rm -f {} \;
		svn revert src/data/ayuda/ayuda.hhp
		svn revert src/data/ayuda/ayuda.hhc
		svn revert src/data/ayuda/ayuda.hhk
		svn revert src/Licencia.txt
		svn revert src/Leame.txt
		svn revert src/Cambios.txt
		svn revert LinuxShortcut.desktop
		svn revert Application.doxyfile
	fi
	# porque no maneja bien las dependencias dentro de los XRC
	find . -type f -name "*_xrced.cpp" -exec rm -f {} \;
}


if [ ! -f src/svn_version.h ] ; then 
   touch src/svn_version.h
fi

BUILD_ENVIRONMENT=${BUILD_ENVIRONMENT:-"19999"}

BUILD_TYPE=${BUILD_TYPE:-"Release"}

MAKE_PARAMS=${MAKE_PARAMS:-"-j 2"}

WORKSPACE=${WORKSPACE:-"`pwd`"}

COMPILER_PREFIX=${COMPILER_PREFIX:-"i686-w64-mingw32"}

DLLS_BASE_DIR=${DLLS_BASE_DIR:-"$HOME/opt/local-${COMPILER_PREFIX}"}

# ----------------------- WINDOWS -----------------------------
# si quiero que sea por fuera del arbol de fuentes
test -n "${OUT_OF_SOURCE_BUILD}" && { mkdir -p WinBuild ; cd WinBuild ; }

# Cargo variables del entorno
. "${DLLS_BASE_DIR}/share/sur/build_environment.sh"

# Para windows, no levanta bien los flags de release para el GCC, asi que los agrego
# en forma forzada.
case "${BUILD_TYPE}" in
  Release) COMPILER_FLAGS="${COMPILER_FLAGS} -O3 -DNDEBUG -static-libgcc -Wno-strict-aliasing";;
esac

CL_CMAKE_PARAMETERS="-DCMAKE_TOOLCHAIN_FILE=${WORKSPACE}/lib/suri/tools/Toolchain-mingw32.cmake \   
    -DMINGW_ROOT_PATH=/usr/${COMPILER_PREFIX} -DCOMPILER_PREFIX=${COMPILER_PREFIX} \                    
    -DSURILIB_INCLUDE_DIR=${WORKSPACE}/lib/suri/include -DUSR_LOCAL=${DLLS_BASE_DIR} ${CMAKE_PARAMETERS}" 

CL_COMPILER_FLAGS="${COMPILER_FLAGS} -O3 -DNDEBUG -static-libgcc -Wno-strict-aliasing -D__WINDOWS__"
CL_LINKER_FLAGS="-L${DLLS_BASE_DIR}/lib/"

OLDPATH=$PATH
export PATH=${HOME}/opt/local/bin/:${DLLS_BASE_DIR}/bin:$PATH
OLDLD_LIBRARY_PATH=$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${HOME}/opt/local/lib/

clean

cmake -Wno-dev -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DCMAKE_CXX_FLAGS="${CL_COMPILER_FLAGS}" \
      -DCMAKE_C_FLAGS="${CL_COMPILER_FLAGS}" -DSVNVERSION=svnversion \
      -DCMAKE_VERBOSE_MAKEFILE=FALSE -DCMAKE_EXE_LINKER_FLAGS=${CL_LINKER_FLAGS} ${CL_CMAKE_PARAMETERS} ${WORKSPACE}

make -k ${MAKE_PARAMS} || exit 1

iconv -f UTF-8 -t ISO-8859-15 src/data/ayuda/ayuda.hhc -o src/data/ayuda/ayuda_iso8859-15.hhc
mv src/data/ayuda/ayuda_iso8859-15.hhc src/data/ayuda/ayuda.hhc

iconv -f UTF-8 -t ISO-8859-15 src/data/ayuda/ayuda.hhk -o src/data/ayuda/ayuda_iso8859-15.hhk
mv src/data/ayuda/ayuda_iso8859-15.hhk src/data/ayuda/ayuda.hhk

iconv -f UTF-8 -t ISO-8859-15 src/data/ayuda/ayuda.hhp -o src/data/ayuda/ayuda_iso8859-15.hhp
mv src/data/ayuda/ayuda_iso8859-15.hhp src/data/ayuda/ayuda.hhp

test -f "src/Leame.txt" && todos src/Leame.txt
test -f "src/Cambios.txt" && todos src/Cambios.txt
test -f "src/Licencia.txt" && todos src/Licencia.txt

# convierto los archivos de UTF-8 a iso-88559-15
iconv -f UTF-8 -t ISO-8859-15 src/Licencia.txt -o src/Licencia-iso8859-15.txt
mv src/Licencia-iso8859-15.txt src/Licencia.txt

iconv -f UTF-8 -t ISO-8859-15 src/Cambios.txt -o src/Cambios-iso8859-15.txt
mv src/Cambios-iso8859-15.txt src/Cambios.txt

iconv -f UTF-8 -t ISO-8859-15 src/Leame.txt -o src/Leame-iso8859-15.txt
mv src/Leame-iso8859-15.txt src/Leame.txt

iconv -f UTF-8 -t ISO-8859-15 installer.nsi -o installer-iso8859-15.nsi || cp installer.nsi installer-iso8859-15.nsi

makensis -DDLLS_DIR=${DLLS_BASE_DIR}/bin/ -DBUILD_ENVIRONMENT=${BUILD_ENVIRONMENT} -DWX_DLLS_DIR=${DLLS_BASE_DIR}/lib/ installer-iso8859-15.nsi || exit 1

rm installer-iso8859-15.nsi

export PATH=$OLDPATH
export LD_LIBRARY_PATH=$OLDLD_LIBRARY_PATH

clean

# ------------------------ LINUX ------------------------------
# si quiero que sea por fuera del arbol de fuentes
test -n "${OUT_OF_SOURCE_BUILD}" && { cd .. ; mkdir -p LinuxBuild ; cd LinuxBuild/ ; }

# Cargo variables del entorno
. "${HOME}/opt/local/share/sur/build_environment.sh"

OLDPATH=$PATH
export PATH=$PATH:${HOME}/opt/local/bin:${HOME}/opt/local/lib/ 

OLDLD_LIBRARY_PATH=$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${HOME}/opt/local/lib/

CL_CMAKE_PARAMETERS="-DSURILIB_INCLUDE_DIR=${WORKSPACE}/lib/suri/include ${CMAKE_PARAMETERS}"
CL_COMPILER_FLAGS="${COMPILER_FLAGS} -D__LINUX__"
CL_LINKER_FLAGS="-L${HOME}/opt/local/lib/"

cmake -Wno-dev -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DCMAKE_CXX_FLAGS="${CL_COMPILER_FLAGS}" \
      -DCMAKE_C_FLAGS="${CL_COMPILER_FLAGS}" -DSVNVERSION=svnversion \
      -DCMAKE_VERBOSE_MAKEFILE=FALSE -DCMAKE_EXE_LINKER_FLAGS=${CL_LINKER_FLAGS} ${CL_CMAKE_PARAMETERS} ${WORKSPACE}

make -k ${MAKE_PARAMS} || exit 1
bash ./installer_deb.sh || exit 1

# Generacion del paquete .deb
cpack ${WORKSPACE}

deb_file=$(ls *.deb 2> /dev/null)
# fix de permisos sobre la generacion del .deb
mkdir fix_up_deb
dpkg-deb -x $deb_file fix_up_deb
dpkg-deb --control $deb_file fix_up_deb/DEBIAN
rm $deb_file
chmod 0644 fix_up_deb/DEBIAN/md5sums
find -type d -print0 |xargs -0 chmod 755
fakeroot dpkg -b fix_up_deb $deb_file
rm -rf fix_up_deb
# fin fix de permisos
rm -Rf package

export PATH=$OLDPATH
export LD_LIBRARY_PATH=$OLDLD_LIBRARY_PATH

clean

# si quiero que sea por fuera del arbol de fuentes
test -n "${OUT_OF_SOURCE_BUILD}" && cd ..

# Ejecuto los tests
test -f "${script_dir}/ci_check.sh" && bash ${script_dir}/ci_check.sh

exit 0
