robocopy "%1" "../#in/Import/Bin" *.dll /IT /IM
if %ERRORLEVEL% GEQ 8 goto failed
robocopy "%1" "../#in/Import/Lib" *.so /IT /IM
if %ERRORLEVEL% GEQ 8 goto failed
robocopy "%1" "../#in/Import/Lib" *.lib /IT /IM
if %ERRORLEVEL% GEQ 8 goto failed
robocopy "../AetherEngine" "../#in/Import/#RawInclude" *.h /IT /IM
if %ERRORLEVEL% GEQ 8 goto failed

exit 0
:failed
exit %ERRORLEVEL%
