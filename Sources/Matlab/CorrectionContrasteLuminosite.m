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


%Fonction CorrectionContrasteLuminosite
%Paramètre   : nomImage       : nom de l'image dont on va corriger le contraste et la luminosité
%			 : nomImageRetour : nom de l'image de retour
%Retourne    : imageRetour    : la matrice de l'image de retour
%Principe    : Ajuste les limites des histogrammes et écrit l'image de retour

function [imageRetour] = CorrectionContrasteLuminosite(nomImage,nomImageRetour)
    
    monImage = imread(nomImage);
    sl = stretchlim(monImage);
    imageRetour = imadjust(monImage,sl,[]);
    imwrite(imageRetour,nomImageRetour,'Quality',100);
end