echo OFF

set ROOT="C:\Users\Saman\RougeLike"

if not exist %ROOT% (
	echo Game root directory at '%ROOT%' does not exist, open batch file and set correct directory
	echo Failed generating installer, exiting...
	PASUE
	exit
)

echo Updating game files from root at '%ROOT%\RougeLike\Resources'

if exist "%~dp0Resources" rmdir /s /q "%~dp0Resources"

if exist "%~dp0Resources" echo resources folder was not deleted.

xcopy /S /I /Y "%ROOT%\RougeLike\Resources" "%~dp0Resources"
copy "%ROOT%\Release\RougeLike.exe" "%~dp0DungeonDash.exe"
copy "%ROOT%\RougeLike\zlib1.dll" "%~dp0"

echo.
echo Compiling NSIS script 'DungeonDashInstallerScript'...

"C:\Program Files (x86)\NSIS\makensisw.exe" %~dp0DungeonDashInstallerScript.nsi

echo.
echo Dungeon Dash Installer generated.