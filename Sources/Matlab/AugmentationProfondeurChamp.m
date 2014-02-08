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


%Fonction AugmentationProfondeurChamp
%Paramètre   : nomImage       : nom de l'image dont on veut améliorer la profondeur de champ
%			 : nomImageRetour : nom de l'image de retour
%Retourne    : imageRetour    : la matrice de l'image de retour
%Principe    : Cette fonction va calculer une version floue de l'image originale et calculer
%la carte de profondeur. Ensuite grâce à la carte, on peut construire une image dont les pixels
%qui sont supérieurs à un seuil de profondeur sont flous.

function [imageRetour] = AugmentationProfondeurChamp(nomImage,nomImageRetour)

	addpath('DefocusEstimation_Sources\');
	
    %Image Floue
    imageOriginale=imread(nomImage);
    %w = fspecial('disk',16);
    w = fspecial('gaussian',[21 21],7);
    imageFloue = imfilter(imageOriginale,w,'replicate');
    
    
    %Carte de profondeur
    imageOriginale_Double=im2double(imread(nomImage));
    eth=0.1;
    carteBordure=edge(rgb2gray(imageOriginale_Double),'canny',eth,1);
    std=1;
    lambda=0.001;
    flouMaximum=3;
    [sDMap, fDmap] = defocusEstimation(imageOriginale_Double,carteBordure,std,lambda,flouMaximum);

    %imshow(fDmap,[0 flouMaximum]);

    
    %Detection de la boite englobante du visage
    detecteurVisage = vision.CascadeObjectDetector; 		
    boitesEnglobantes = step(detecteurVisage,imageOriginale); 
    
    numColonne=boitesEnglobantes(1);
    numLigne=boitesEnglobantes(2);
    nbColonnesBoite=boitesEnglobantes(3);
    nbLignesBoite=boitesEnglobantes(4);
    
    %figure(1);
    %hold on;
    %imshow(imageOriginale);
    %%x, y , width ,height
    %rectangle('Position',[numColonne,numLigne,nbColonnesBoite,nbLignesBoite]);
    %hold off;
    
    %figure(2);
    %imshow(imageFloue);
    
    %Calcul du seuil de profondeur moyen pour le visage dans la boite
    SEUIL = mean(mean(fDmap(numLigne:numLigne+nbLignesBoite,numColonne:numColonne+nbColonnesBoite)));
    carteProfondeurFond = fDmap>SEUIL(1)+0.2;
    %carteProfondeurFond = fDmap>1.25;

    
    imageRetour = zeros(size(imageOriginale),class(imageOriginale));
    %Construction de l'image résultante, si à une position donnée, le pixel
    %est à une profondeur supérieure à la profondeur moyenne au niveau du
    %visage, on choisit la version floue sinon on prend la version
    %originale du pixel
    s = size(imageOriginale);
    for i=1:s(1)
        for j=1:s(2)
            if(carteProfondeurFond(i,j)==1)
                imageRetour(i,j,1:3) = imageOriginale(i,j,1:3)*0.5+imageFloue(i,j,1:3)*0.5;
            else
                imageRetour(i,j,1:3) = imageOriginale(i,j,1:3);
            end;
        end;
    end;
    
    %figure(3);
    %imshow(imageRetour);
    
    %Ecriture du résultat
    imwrite(imageRetour,nomImageRetour,'Quality',100); 
end

