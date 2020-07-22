robocopy "../#in/Import/Bin" "%2" *.%1 /IT /IM
if %ERRORLEVEL% GEQ 8 goto failed

exit 0
:failed
exit %ERRORLEVEL%
