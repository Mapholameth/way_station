@ECHO OFF
CD lexer
ECHO Testing scanner: > scanner_test.log
ECHO. >> scanner_test.log
FOR %%V IN (*.c) DO (
ECHO -----------------------------------------------
ECHO testing: "%%~nV"
..\..\bin\Compiler.debug.exe -l %%~nV.c > %%~nV.out
cmp -c %%~nV.out %%~nV.ans
IF ERRORLEVEL 1 (ECHO ******* FAILED *******) ELSE (ECHO ******* PASSED *******)
IF ERRORLEVEL 1 (
ECHO test "%%~nV":    FAILED: >>scanner_test.log & cmp -c %%~nV.out %%~nV.ans >> scanner_test.log) ELSE (ECHO test "%%~nV":    PASSED >>scanner_test.log)
) 
cd ..\
PAUSE

rem type %%V.out.txt