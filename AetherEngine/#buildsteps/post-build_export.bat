rmdir /s /q "%2"
mkdir "%2"

robocopy "../AetherEngine/Src" "%2" *.h /IT /IM /S
if %ERRORLEVEL% GEQ 8 goto failed

exit 0
:failed
exit %ERRORLEVEL%
