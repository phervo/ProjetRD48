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


%Fonction Redimensionnement
%Paramètre   : nomImage       : nom de l'image que l'on va redimensionner
%			 : taille         : taille maximale qu'aura l'image en hauteur
%                               ou en largeur
%Retourne    : imageRetour    : la matrice de l'image de retour
%Principe    : Calcule un nouveau ratio pour la taille de l'image par
%rapport à la nouvelle taille et à la dimension de plus grande valeur de
%l'image.

function [imageRetour] = Redimensionnement(nomImage,taille)

    %dimension ideale
    dimension_maximale = taille; 

    image = imread(nomImage);
    
    s = size(image);
    indice = find(s == max(s));
    
    %Redimensionnement de l'image
    if (s(indice) > dimension_maximale)
        ratio = dimension_maximale / s(indice);
        imageRetour = imresize(image, ratio);
    end;
    
end