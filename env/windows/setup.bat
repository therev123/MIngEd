:: BatchGotAdmin
:-------------------------------------
REM  --> Check for permissions
>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"

SET ENV_DIR=%~dp0

::Try and guess where we want this installed
::NOTE: this script would need write access to wherever it is
IF "%1"=="" (
	SET INSTALL_DIR=C:%HOMEPATH%
) ELSE (
	SET INSTALL_DIR="%1"
)


REM --> If error flag set, we do not have admin.
if '%errorlevel%' NEQ '0' (
    echo Requesting administrative privileges...
    goto UACPrompt
) else ( goto gotAdmin )

:UACPrompt
    echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
    echo UAC.ShellExecute "%~s0", "", "", "runas", 1 >> "%temp%\getadmin.vbs"

    "%temp%\getadmin.vbs"
    exit /B

:gotAdmin
    if exist "%temp%\getadmin.vbs" ( del "%temp%\getadmin.vbs" )
    pushd "%CD%"
    CD /D "%~dp0"
	SET DIR=%~dp0
	
	::Copy the wrapper scripts to the install directory
	XCOPY %ENV_DIR%\bin %INSTALL_DIR%\bin /S /Y

	::add our environment variables
	SETX /M MSDKDIR %INSTALL_DIR%\bin\.MSDK
	SETX /M PATH "%PATH%;%INSTALL_DIR%\bin;%INSTALL_DIR%\bin\.MSDK\Tools"
	PAUSE 

	::Link the prepared output directory to somewhere our
	::development environment can easily find
	MKLINK /J %INSTALL_DIR%\MIngEd %ENV_DIR%\..\..\pkg\Maratis

	::set up the internal symlink
	%INSTALL_DIR%\bin\MSDK minged

	UNSET ENV_DIR
	UNSET ARGC
	UNSET INSTALL_DIR
	PAUSE