%
%-----------------------------------------------------------------------------
%Polytech Nantes
%2013-2014 INFO5
%-----------------------------------------------------------------------------
%Projet de Recherche et développement
%Sujet 48 Magic Portrait
%-----------------------------------------------------------------------------
%Etudiants:
%Pierre-Yves HERVO
%Paul-François JEAU
%-----------------------------------------------------------------------------
%


%Fonction TraitementDefocusPuisContrasteLuminosite
%Paramètre   : nomImage       : nom de l'image dont on veut améliorer la
%                               profondeur de champ et le 
%                               contraste/luminosité
%			 : nomImageApresAugmentationDefocus : nom de l'image de retour
%			 après l'augmentation de la profondeur de champ
%			 : nomImageApresCorrectionContLum : nom de l'image de retour
%			 après la correction du contraste et de la luminosité
%			 : redimensionner : Y ou N pour indiquer s'il faut
%			 redimensionner l'image de base ou non
%			 : taille : taille de la dimension maximale de l'image

%Principe    : Cette fonction lance successivement tous les traitements
%Matlab pour améliorer une image.

function TraitementDefocusPuisContrasteLuminosite(nomImageOriginale,nomImagePeauLisse,nomImageApresAugmentationDefocus,nomImageApresCorrectionContLum,redimensionner,taille)


imageOriginale = imread(nomImageOriginale);

%Redimensionnement si précisé
if(redimensionner=='Y')
    imwrite(Redimensionnement(nomImagePeauLisse,taille),nomImagePeauLisse,'Quality',100); 
	imageOriginale = Redimensionnement(nomImageOriginale,taille);
end;

subplot(1,2,1);
imshow(imageOriginale);


%Augmentation de la profondeur de champ
AugmentationProfondeurChamp(nomImagePeauLisse,nomImageApresAugmentationDefocus);

%Puis correction de la luminosité et du contraste
CorrectionContrasteLuminosite(nomImageApresAugmentationDefocus,nomImageApresCorrectionContLum);
subplot(1,2,2);
imageApresCorrectionContLum = imread(nomImageApresCorrectionContLum);
imshow(imageApresCorrectionContLum);
end