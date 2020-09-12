REM %1 : Import folder for binaries
REM %2 : Export folder where the .exe is located
REM %3 : File extension to copy

robocopy "%1" "%2" *.%3 /IT /IM
if %ERRORLEVEL% GEQ 8 goto failed

goto eof
:failed
exit %ERRORLEVEL%

:eof
exit /B 0