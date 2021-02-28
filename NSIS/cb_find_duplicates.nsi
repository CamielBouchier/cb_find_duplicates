; vim: syntax=nsis ts=2 sw=2 sts=2 sr et columns=100 lines=45

;---------------------------------------------------------------------------------------------------
;
; $BeginLicense$
;
; (C) 2015-2021 by Camiel Bouchier (camiel@bouchier.be)
;
; This file is part of cb_find_duplicates.
; All rights reserved.
; You are granted a non-exclusive and non-transferable license to use this
; software for personal or internal business purposes.
;
; THIS SOFTWARE IS PROVIDED "AS IS" AND
; ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
; WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
; DISCLAIMED. IN NO EVENT SHALL Camiel Bouchier BE LIABLE FOR ANY
; DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
; (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
; ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
; (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
; SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
; $EndLicense$
;
;---------------------------------------------------------------------------------------------------

!define ProgName    "cb_find_duplicates"
!define RegKey      "Software\${ProgName}"
!define StartMenu   "$SMPROGRAMS\${ProgName}"
!define UnInstall   "uninstall.exe"
!define UnInstKey   "Software\Microsoft\Windows\CurrentVersion\Uninstall\${ProgName}"

XPStyle on

Name       "${ProgName}"
Caption    "${ProgName}"
OutFile    "../${ProgName}_installer.exe"
InstallDir "$PROGRAMFILES64\${ProgName}"

;---------------------------------------------------------------------------------------------------

Page license
Page directory
Page instfiles

;---------------------------------------------------------------------------------------------------

; First is default
LoadLanguageFile "${NSISDIR}\Contrib\Language files\English.nlf"
LoadLanguageFile "${NSISDIR}\Contrib\Language files\Dutch.nlf"

LicenseLangString TheLicenseData ${LANG_ENGLISH} "..\about\license_english.txt"

LangString TheLicenseText ${LANG_ENGLISH} "License"

LangString TheDirText ${LANG_ENGLISH} "The installer will install ${ProgName}"

LangString TheUninstallText ${LANG_ENGLISH} "The installer will uninstall ${ProgName}"

LicenseData   "$(TheLicenseData)"
LicenseText   "$(TheLicenseText)"
DirText       "$(TheDirText)"
UninstallText "$(TheUninstallText)"

;---------------------------------------------------------------------------------------------------

UninstPage uninstConfirm
UninstPage instfiles

;---------------------------------------------------------------------------------------------------

Section
  WriteRegStr HKLM "${RegKey}"    "Install_Dir" "   $INSTDIR"
  WriteRegStr HKLM "${UnInstKey}" "DisplayName"     "${ProgName} (remove only)"
  WriteRegStr HKLM "${uninstkey}" "UninstallString" '"$INSTDIR\${UnInstall}"'

  SetOutPath $INSTDIR
  File /r /x ".*" "..\dist_windows\*"
  File /r /x ".*" "..\dist_windows\*.*"

  WriteUninstaller "${UnInstall}"

SectionEnd

;---------------------------------------------------------------------------------------------------

Section

  SetOutPath $INSTDIR
  CreateDirectory "${StartMenu}"
  CreateShortCut  "${StartMenu}\${ProgName}.lnk" "$INSTDIR\${ProgName}.exe"

SectionEnd

;---------------------------------------------------------------------------------------------------

Section "Uninstall"

  DeleteRegKey HKLM "${UnInstKey}"
  DeleteRegKey HKLM "${RegKey}"

  Delete "${StartMenu}\*.*"
  Delete "${StartMenu}"
  RMDir /r $INSTDIR

SectionEnd

;---------------------------------------------------------------------------------------------------

Function .onInit

	;Language selection dialog

	Push ""
	Push ${LANG_ENGLISH}
	Push English
	;Push ${LANG_DUTCH}
	;Push Dutch
	Push A ; A means auto count languages
	       ; for the auto count to work the first empty push (Push "") must remain
	LangDLL::LangDialog "Installer Language" "Please select the language of the installer"

	Pop $LANGUAGE
	StrCmp $LANGUAGE "cancel" 0 +2
		Abort
FunctionEnd

;---------------------------------------------------------------------------------------------------
