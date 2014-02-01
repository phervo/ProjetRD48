setlocal enableDelayedExpansion

set vari=%1
set varj=%2

for %%f in (%vari%"\*") do (
	del %varj%\%%~nf"_contraste"%%~xf
	cd "scripts_matlab"
	matlab.exe -nodisplay -nosplash -nodesktop -r "CorrectionContrasteLuminosite('\"..\\"%%f','\"..\\"%varj%\%%~nf\"_contraste\"%%~xf');"
	cd ..
	taskkill /im matlab.exe
)