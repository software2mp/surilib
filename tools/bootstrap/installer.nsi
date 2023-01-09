; includes
!include nsDialogs.nsh ; para crear controles.
!include LogicLib.nsh  ; permite hacer if...then...endif
!include FileFunc.nsh ; permite utilizar macro DirState

; Compresion
SetCompressor /SOLID lzma

; Remover accesos directos en la desinstalacion Windows Vista/7
; NOTE: You still need to check user rights with UserInfo!
RequestExecutionLevel admin 

; Macros
; See http://nsis.sourceforge.net/Check_if_a_file_exists_at_compile_time for documentation
!macro !defineifexist _VAR_NAME _FILE_NAME
	!tempfile _TEMPFILE
	!ifdef NSIS_WIN32_MAKENSIS
		; Windows - cmd.exe
		!system 'if exist "${_FILE_NAME}" echo !ifdef ${_VAR_NAME} > "${_TEMPFILE}"'
		!system 'if exist "${_FILE_NAME}" echo !undef ${_VAR_NAME} >> "${_TEMPFILE}"'
		!system 'if exist "${_FILE_NAME}" echo !endif >> "${_TEMPFILE}"'
		!system 'if exist "${_FILE_NAME}" echo !define ${_VAR_NAME} "${_FILE_NAME}" >> "${_TEMPFILE}"'
	!else
		; Posix - sh
		!system 'if [ -e "${_FILE_NAME}" ]; then echo "!ifdef ${_VAR_NAME}" > "${_TEMPFILE}"; fi'
		!system 'if [ -e "${_FILE_NAME}" ]; then echo "!undef ${_VAR_NAME}" >> "${_TEMPFILE}"; fi'
		!system 'if [ -e "${_FILE_NAME}" ]; then echo "!endif" >> "${_TEMPFILE}"; fi'
		!system 'if [ -e "${_FILE_NAME}" ]; then echo "!define ${_VAR_NAME}" "${_FILE_NAME}" >> "${_TEMPFILE}"; fi'
	!endif
	!include '${_TEMPFILE}'
	!delfile '${_TEMPFILE}'
	!undef _TEMPFILE
!macroend
!define !defineifexist "!insertmacro !defineifexist"

; defines
!ifndef DLLS_DIR
!define DLLS_DIR "C:\msys\1.0\local\bin\"
!endif
!ifndef WX_DLLS_DIR
!define WX_DLLS_DIR "C:\msys\1.0\local\lib\"
!endif
; Define las rutas de las bibliotecas (prefiere las del entorno 1.1.x)
${!defineifexist} GDAL_DLL_NAME ${DLLS_DIR}libgdal-1.dll
${!defineifexist} GDAL_DLL_NAME ${DLLS_DIR}libgdal.dll
${!defineifexist} GEOS_DLL_NAME ${DLLS_DIR}libgeos-3-0-3.dll
${!defineifexist} GEOS_DLL_NAME ${DLLS_DIR}libgeos-3-2-2.dll
${!defineifexist} SURI_DLL "lib\suri\libsuri.dll"

!define APP_SHORT_NAME
!define APP_NAME
; Nombre del ejecutable de la compilacion
!ifndef EXEFILE
!define EXEFILE
!endif

!define VENDOR
!define VERSION ""
!define INSTALLER_FILE "${VENDOR}-v${VERSION}.exe"
!define INSTALLER_ICON
!define UNINSTALLER_ICON
!define MIME_ICON
!define MIME_ICON_PATH
!define SRC_DIR
!define UNINSTALLER_NAME "Uninstall.exe"
!define USR_DIR "$APPDATA\${APP_SHORT_NAME}"
!define USER_DATA_DIR
!define SUR_REG_KEY_ROOT "Software\SUR Software"
!define REG_KEY "${SUR_REG_KEY_ROOT}\${APP_SHORT_NAME}"
!define MIME_EXTENSION

; Traducciones
;LoadLanguageFile "${NSISDIR}\Contrib\Language files\English.nlf"
LoadLanguageFile "${NSISDIR}\Contrib\Language files\Spanish.nlf"

;LangString application_name ${LANG_ENGLISH} "${APP_NAME}"
LangString application_name ${LANG_SPANISH} "${APP_NAME}"

;LangString application_dir ${LANG_ENGLISH} "${APP_SHORT_NAME}"
LangString application_dir ${LANG_SPANISH} "${APP_SHORT_NAME}"

;LangString message ${LANG_ENGLISH} "This will install $(application_name) software. Continue?"
LangString message ${LANG_SPANISH} "Este programa instalará el $(application_name). ¿Desea continuar?"

;LangString message ${LANG_ENGLISH} "$(application_name) is already installed. $\n$\nClick `OK` to remove the current version or `Cancel` to cancel this upgrade."
LangString uninstall_message ${LANG_SPANISH} "Existe una version de $(application_name) instalada. $\n$\n Presione 'Aceptar' para desinstalar la version actual o 'Cancelar' para cancelar la instalación."

;LangString message_delete_usr_data ${LANG_ENGLISH} "Do you want to delete PATs's directory?"
LangString message_delete_usr_data ${LANG_SPANISH} "El $(application_name) ha detectado la existencia del directorio de trabajo de los usuarios. ¿Desea eliminar directorio de PATs?"

;LangString message_delete_pats_dir ${LANG_ENGLISH} "Uninstalling"
LangString message_delete_pats_dir ${LANG_SPANISH} "Desinstalando"

;LangString message_checkbox_pats ${LANG_ENGLISH} "Delete PATs's directory"
LangString message_checkbox_pats ${LANG_SPANISH} "Eliminar directorio de PATs"

;LangString uninstalllink ${LANG_ENGLISH} "Uninstall.lnk"
LangString uninstalllink ${LANG_SPANISH} "Desinstalar $(application_name).lnk"

;LangString message_readme ${LANG_ENGLISH} "Installation succesful. Would you like to view $(readme_file)?"
LangString message_readme ${LANG_SPANISH} "Felicitaciones, la instalación ha concluído. ¿Desea ver el archivo $(readme_file)?"

;LangString readme_file ${LANG_ENGLISH} "readme.txt"
LangString readme_file ${LANG_SPANISH} "leame.txt"

;LangString license_file ${LANG_ENGLISH} "license.txt"
LangString license_file ${LANG_SPANISH} "licencia.txt"

;LangString message_pat_dir ${LANG_ENGLISH} "${USER_DATA_DIR}"
LangString message_pat_dir ${LANG_SPANISH} "${USER_DATA_DIR}"

;LangString message_file_description ${LANG_ENGLISH} "PAT File"
LangString message_file_description ${LANG_SPANISH} "Archivo PAT"

;LangString message_warn_del_pat ${LANG_ENGLISH} "Warning: all the working files of users are gonna be deleted"
LangString message_warn_del_pat ${LANG_SPANISH} "Advertencia: se perderón todos los archivos de trabajo de los usuarios."

;--------------------------------
; The name of the installer
Name $(application_name)

; The file to write
OutFile "${INSTALLER_FILE}"

; Cuando se generen los iconos tener en cuenta la restriccion de que deben
; coincidir,  el icono  del instalador y el del desinstalador, en tamanio de
; bytes y pixel.
;Icon ${INSTALLER_ICON}
;UninstallIcon ${UNINSTALLER_ICON}

CRCCheck on
; The default installation directory
InstallDir "$PROGRAMFILES\$(application_dir)"

; Archivo de licencia.
LicenseData $(license_file)

;--------------------------------
; Páginas del instalador
; Page License
Page directory
Page instfiles

; Páginas del desinstalador
UninstPage uninstConfirm
UninstPage custom un.CreateCustomPage un.LeaveCustomPage
UninstPage instfiles
;--------------------------------

Var uninstallDir ; Variable para obtener el directorio del desinstalador
Function .onInit
	; intento leer la clave del registro, para obtener el path del desinstalador.
   ClearErrors
   ReadRegStr $uninstallDir HKLM "${REG_KEY}\Uninstall" "UninstallString"
   ifErrors OnError Uninstall

Uninstall:
   MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION $(uninstall_message) IDOK +2
      Abort

   ClearErrors
   ; el parametro _? hace que ExecWait ejecute el desinstalador frenando las instrucciones del script hasta que termine
	; el desinstalador.
   ReadRegStr $INSTDIR HKLM "${REG_KEY}\Uninstall" "InstallLocation"
   ExecWait '"$uninstallDir" _?=$INSTDIR'
   IfErrors NoRemoveUninstaller Done

OnError:
   ; si hubo error de lectura del registro puede ser xq la clave no existe => uninstallDir==""
   StrCmp $uninstallDir "" Done
   Abort ; cualquier otro error aborto la desinstalacion

;si hubo errores en la desinstalacion aborto la ejecucion.
NoRemoveUninstaller:
   Abort

Done:
   MessageBox MB_OKCANCEL|MB_ICONQUESTION $(message) IDOK Continue
      Abort

Continue:
FunctionEnd


Function .onInstSuccess
	MessageBox MB_YESNO $(message_readme) IDNO NoReadme
		Exec "notepad.exe $INSTDIR\$(readme_file)"
NoReadme:
FunctionEnd

; declaro variables utilizadas en funciones custom para crear controles
Var dialogCtrl
Var labelTitleCtrl
Var labelDirPatCtrl
Var textDirPatCtrl
Var checkboxCtrl
Var checkboxState
Var labelWarnCtrl

; funcion para crear pagina personalizada.
Function un.CreateCustomPage
	; valido que exista el directorio de pats del usuario. Solo si existe muestro la pagina con checkbox para desinstalar directorio de pats.
   ${DirState} "${USER_DATA_DIR}" $0
	IntCmp $0 0 found_directory not_found_directory found_directory
found_directory:
	GoTo Done
not_found_directory:
	Return

Done:
   ; creo pagina personalizada.
   nsDialogs::Create /NOUNLOAD 1018
   Pop $dialogCtrl

   ; verifico que se haya creado sin errores.
   ${If} $dialogCtrl == error
      Abort
   ${EndIf}

   ; creo label con mensaje para usuario.
   ${NSD_CreateLabel} 0u 0u 100% 24u $(message_delete_usr_data)
   Pop $labelTitleCtrl

   ${NSD_CreateLabel} 0u 50u 50u 12u $(message_delete_pats_dir)
   Pop $labelDirPatCtrl

   ; creo una text deshabilitada para mostrar el path de la carpeta de PATs a eliminar.
   ${NSD_CreateText} 50u 48u 80% 12u $(message_pat_dir)
   Pop $textDirPatCtrl
   EnableWindow $textDirPatCtrl 0

	; creo label con mensaje de advertencia para el usuario.
   ${NSD_CreateLabel} 50u -30u 80% 48u $(message_warn_del_pat)
   Pop $labelWarnCtrl
	ShowWindow $labelWarnCtrl ${SW_HIDE}

   ; creo checkbox para saber si hay que eliminar carpeta de PATs.
   ${NSD_CreateCheckbox} 0u -50u 100% 8u $(message_checkbox_pats)
   Pop $checkboxCtrl
	${NSD_OnClick} $checkboxCtrl un.OnCheckbox

   nsDialogs::Show

Continue:
FunctionEnd

Function un.LeaveCustomPage
   ; obtengo el valor del checkbox
   ${NSD_GetState} $checkboxCtrl $checkboxState
FunctionEnd

; funcion que responde al checkeo del checkbox.
Function un.OnCheckbox
	Pop $checkboxCtrl
	${NSD_GetState} $checkboxCtrl $checkboxState
	${If} $checkboxState == ${BST_CHECKED}
      ShowWindow $labelWarnCtrl ${SW_SHOW}
	${Else}
	   ShowWindow $labelWarnCtrl ${SW_HIDE}
   ${EndIf}
FunctionEnd
;--------------------------------

; The stuff to install
Section "" ;No components page, name is not important
	; Le pido que reserve 100Mb más de los necesarios para los archivos
	; que trae el instalador
	AddSize 100000
	; Set output path to the installation directory.
	SetOutPath $INSTDIR

	; Put file there
	File ${SRC_DIR}${EXEFILE}
	; Copio la configuracion
	File ${SRC_DIR}configuracion.xml
	; Copio los datos, ignoro los directorios del svn
	File /r /x .svn ${SRC_DIR}data
	; Copio el archivo leame
	File /nonfatal ${SRC_DIR}readme.txt
	File /nonfatal ${SRC_DIR}leame.txt

	; Copio el archivo licencia
	File /nonfatal ${SRC_DIR}licencia.txt
	File /nonfatal ${SRC_DIR}license.txt

	; dlls de soporte
	File ${DLLS_DIR}libpng-3.dll
	File ${DLLS_DIR}libtiff-3.dll
	File ${DLLS_DIR}libpng12-0.dll
	File ${DLLS_DIR}libproj-0.dll
   File ${GDAL_DLL_NAME}
   File ${GEOS_DLL_NAME}
	File ${DLLS_DIR}libgeos_c-1.dll
   File /nonfatal ${DLLS_DIR}libcurl-4.dll
   File /nonfatal ${DLLS_DIR}libexpat-1.dll
   File /nonfatal ${DLLS_DIR}libgeotiff-2.dll
   File /nonfatal ${DLLS_DIR}libhdf5.dll
   File /nonfatal ${DLLS_DIR}libjpeg-8.dll
   File /nonfatal ${DLLS_DIR}libkml.dll
   File /nonfatal ${DLLS_DIR}libtiffxx-3.dll
   File /nonfatal ${DLLS_DIR}libz.dll
   File /nonfatal ${DLLS_DIR}libgcc_s_dw2-1.dll
	; dlls wx
	File ${WX_DLLS_DIR}wxbase28_gcc.dll
	File ${WX_DLLS_DIR}wxbase28_xml_gcc.dll
	File ${WX_DLLS_DIR}wxmsw28_adv_gcc.dll
	File ${WX_DLLS_DIR}wxmsw28_core_gcc.dll
	File ${WX_DLLS_DIR}wxmsw28_gl_gcc.dll
	File ${WX_DLLS_DIR}wxmsw28_html_gcc.dll
	File ${WX_DLLS_DIR}wxmsw28_xrc_gcc.dll
   ; suri
   !ifdef SURI_DLL
      File ${SURI_DLL}
   !endif
   ; Bitmaps
   ; Incluyo el icono de los archivos PAT.
   File ${MIME_ICON_PATH}

	; creo el desinstalador
	WriteUninstaller .\${UNINSTALLER_NAME}
	; renombro el ejecutable
	Rename "$INSTDIR\${EXEFILE}" "$INSTDIR\$(application_name).exe"

	CreateShortcut "$DESKTOP\$(application_name).lnk" "$INSTDIR\$(application_name).exe"

	CreateDirectory "$SMPROGRAMS\$(application_dir)"
	CreateShortcut "$SMPROGRAMS\$(application_dir)\$(application_name).lnk" "$INSTDIR\$(application_name).exe"
	CreateShortcut "$SMPROGRAMS\$(application_dir)\$(uninstalllink)" "$INSTDIR\${UNINSTALLER_NAME}"
	IfFileExists "$INSTDIR\$(license_file)" 0 +2
		CreateShortcut "$SMPROGRAMS\$(application_dir)\$(license_file).lnk" "$INSTDIR\$(license_file)"
	IfFileExists "$INSTDIR\$(readme_file)" 0 +2
		CreateShortcut "$SMPROGRAMS\$(application_dir)\$(readme_file).lnk" "$INSTDIR\$(readme_file)"

  ; creo clave en el registro de windows.
   WriteRegStr HKLM "${REG_KEY}\Uninstall" "DisplayName" "${APP_NAME}"
   WriteRegStr HKLM "${REG_KEY}\Uninstall" "UninstallString" "$INSTDIR\${UNINSTALLER_NAME}"
   WriteRegStr HKLM "${REG_KEY}\Uninstall" "InstallLocation" "$INSTDIR"
   WriteRegStr HKLM "${REG_KEY}\Uninstall" "DisplayVersion" "${VERSION}"

   ; asorio la extension .pat a la aplicacion.
	WriteRegStr HKCR "${MIME_EXTENSION}" "" "$(message_file_description)"
	WriteRegStr HKCR "$(message_file_description)" "" "$(message_file_description)"
   WriteRegStr HKCR "$(message_file_description)\DefaultIcon" "" "$INSTDIR\${MIME_ICON},0"
	WriteRegStr HKCR "$(message_file_description)\shell" "" "open"
	WriteRegStr HKCR "$(message_file_description)\shell\edit" "" "Edit $(message_file_description)"
	WriteRegStr HKCR "$(message_file_description)\shell\edit\command" "" '"$INSTDIR\$(application_name).exe" "%1"'
	WriteRegStr HKCR "$(message_file_description)\shell\open" "" ""
	WriteRegStr HKCR "$(message_file_description)\shell\open\command" "" '"$INSTDIR\$(application_name).exe" "%1"'

	; invoco la funcion RefreshShellIcons para que se actualizen los iconos de los PATs. Fuerza una lectura del registro de WIN  y evito que se tenga que reiniciar la pc.
	${RefreshShellIcons}
SectionEnd ; end the section

Section "Uninstall"
	Delete "$INSTDIR\${UNINSTALLER_NAME}" ; delete self (see explanation below why this works)

	; Borra el directorio de instalacion
	RMDir /r "$INSTDIR"

  ; Verifico valor del checkbox para saber si debo eliminar carpeta de PATS
  ${If} $checkboxState == ${BST_CHECKED}
		DetailPrint "Eliminar carpeta de PATs"
		RMDir /r "${USR_DIR}" ; elimino todo el directorio 2Mp que se encuentra dentro del directorio de datos del usuario.
  ${EndIf}

	; Borra los links del menu START
	RMDir /r "$SMPROGRAMS\$(application_dir)"
	Delete "$DESKTOP\$(application_name).lnk"

   ; elimino clave en el registro de windows.
   DeleteRegKey HKLM "${SUR_REG_KEY_ROOT}"

   ; desasorio la extension .pat de la aplicacion.
   DeleteRegKey HKCR "${MIME_EXTENSION}"
   DeleteRegKey HKCR "$(message_file_description)"
	${RefreshShellIcons}
SectionEnd
