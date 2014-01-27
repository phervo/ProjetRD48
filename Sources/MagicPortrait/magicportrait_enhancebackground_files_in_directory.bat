setlocal enableDelayedExpansion

set vari=%1
set varj=%2

for %%f in (%vari%"\*") do (
	del %varj%\%%~nf"_profondeur"%%~xf	
	cd "scripts_matlab"
	matlab.exe -nodisplay -nosplash -nodesktop -r "AugmentationProfondeurChamp('\"..\\"%%f','\"..\\"%varj%\%%~nf\"_profondeur\"%%~xf');"
	cd ..
	taskkill /im matlab.exe
)