if "%1" == "" goto EMPTY
FU /checksum %1
goto END

:EMPTY
.\tools\FU /checksum .\ROM\GS.bin

:END
pause
