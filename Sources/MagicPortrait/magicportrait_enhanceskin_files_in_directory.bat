setlocal enableDelayedExpansion

set vari=%1
set varj=%2

for %%f in (%vari%"\*") do (
	del %varj%\%%~nf"_lisse"%%~xf
	magicportrait.exe %%f %varj%\%%~nf"_lisse"%%~xf
)