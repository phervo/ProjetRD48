setlocal enableDelayedExpansion

set cheminImageOriginale=%1
set cheminImagePeauLisse=%2
set cheminImageDefocus=%3
set cheminImageContraste=%4

del %cheminImagePeauLisse%
del %cheminImageDefocus%
del %cheminImageContraste%


magicportrait.exe %cheminImageOriginale% %cheminImagePeauLisse%
cd "scripts_matlab"
matlab.exe -nodisplay -nosplash -nodesktop -r "TraitementDefocusPuisContrasteLuminosite('\"..\\"%cheminImageOriginale%','\"..\\"%cheminImagePeauLisse%','\"..\\"%cheminImageDefocus%','\"..\\"%cheminImageContraste%','Y',400);"

