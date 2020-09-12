REM %1 : Folder to copy headers from (recursive)
REM %2 : Folder to copy headers to
REM %3 : Set to anything to delete %2 before copying

set "condition=%3"

if defined condition (
	rmdir /s /q "%2"
	mkdir "%2"
)

robocopy "%1" "%2" *.h /IT /IM /S
if %ERRORLEVEL% GEQ 8 goto failed

goto eof
:failed
exit %ERRORLEVEL%

:eof
exit /B 0