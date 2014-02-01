/*
-----------------------------------------------------------------------------
Polytech Nantes
2013-2014 INFO5
-----------------------------------------------------------------------------
Projet de Recherche et développement
Sujet 48 Magic Portrait
-----------------------------------------------------------------------------
Etudiants:
Pierre-Yves HERVO
Paul-François JEAU
-----------------------------------------------------------------------------
*/

//-----Librairies et Headers-------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "DetecteurPeau.h"
#include "LisseurPeau.h"
#include <stdlib.h>
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <set>

using namespace cv;
using namespace std;

//-----Entete des méthodes/fonctions-----------------------------------------
bool detecterVisage(Mat);
Mat  lisserPeau(Mat); 
void lisserPeauTest(char*);
Mat retournerEtiquettage(Mat masquePeau);
void determinerEtiquetes(Mat * matriceEtiquettage, int i, int j, unsigned char *numeroEtiquette);
unsigned char inf(unsigned char etiquetteCourante, unsigned char etiquetteAdjacente);
Rect detecterCadreVisage(Mat monImage);
Mat changerMasquePeau(Mat masqueOriginal, set<unsigned char> ensembleEtiquette, Mat matriceEtiquette);
set<unsigned char> getEnsembleEtiquette(Mat etiquettes, Rect cadreVisage);


//-----Main du programme-----------------------------------------------------
int main(int argc, char *argv[])
{
	//Vérification de l'argument
	/*if (argc < 3) {
		cerr << "Utilisation de : " << argv[0] << endl;
		cerr << "Argument 1 : nom_image_cible" << endl;
		cerr << "Argument 2 : nom_image_resultat" << endl;
		cerr << "Exemple d'utilisation : nom_executable <nom_fichier_cible> <nom_fichier_resultat>" << endl;
		return -1;
	}*/

	//Chargement de l'image cible
	//Mat monImageCible = imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);
	Mat monImageCible = imread("Images_Test/v4.jpg", CV_LOAD_IMAGE_UNCHANGED);

	if (monImageCible.empty()){
		cerr << "Erreur: l'image " << argv[1] << " n'a pas pu être chargée" << endl;
		return -2;
	}

	//Vérification du type de l'image
	if (monImageCible.channels()!= 3){
		cerr << "Erreur: l'image " << argv[1] << " n'est pas en couleur en ne comporte pas 3 composantes" << endl;
		return -3;
	}

	//Vérification de la présence de visage dans l'image
	if (detecterVisage(monImageCible)){
		cerr << "Erreur: l'image " << argv[1] << " ne contient aucun visage" << endl;
		return -4;
	}

	//Lancement du traitement en tant que tel
	//Mat resultat = lisserPeau(monImageCible);
	DetecteurPeau monDetecteurPeau = DetecteurPeau();
	Mat masquePeau;
	masquePeau = monDetecteurPeau.getMasquePeauFiltré(monImageCible);

	Mat etiquettage = retournerEtiquettage(masquePeau);
	
	
	Rect cadreEnglobant = detecterCadreVisage(monImageCible);

	set<unsigned char> ensembleEtiquette = getEnsembleEtiquette(etiquettage, cadreEnglobant);
	Mat resultat = changerMasquePeau(masquePeau, ensembleEtiquette, etiquettage);

	imshow("Image après Lissage", resultat);
	imshow("Masque des pixels de peau", masquePeau);

	waitKey(0);
	cvDestroyAllWindows();
	//Ecriture de l'image résultante
	imwrite("Images_Test/v4_res_2.jpg", resultat);

	return 0;
}


//-----Corps des méthodes/fonctions------------------------------------------

/*
Methode detecterVisage
Paramètre : Mat monImage : la matrice de l'image que l'on veut tester
Retourne  : bool         : true si l'image contient au moins un visage, false sinon
Principe  : Avec un classifieur de Haar on cherche au moins une boite englobante d'un visage,
s'il y en a une, il y a au moins un visage à traiter.
*/
bool detecterVisage(Mat monImage){

	//Chargement du classifieur de Haar
	String classifieur_visage_fichier = "haarcascade_frontalface_default.xml";
	CascadeClassifier classifieur_visage;

	if (!classifieur_visage.load(classifieur_visage_fichier)){
		cerr << "Erreur: Impossible de charger le Detecteur de Haar de visages" << endl;
		exit(-4);
	}
	//Recherche de visage(s) dans l'image
	vector<Rect> visages;
	Mat monImage_niveauxGris;

	cvtColor(monImage, monImage_niveauxGris, CV_BGR2GRAY);
	equalizeHist(monImage_niveauxGris, monImage_niveauxGris);

	classifieur_visage.detectMultiScale(monImage_niveauxGris, visages, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	//Retour du résultat de la recherche
	return visages.empty();
}

/*
Methode detecterCadreVisage
*/
Rect detecterCadreVisage(Mat monImage){
	Rect cadreEnglobant= Rect();
	//Chargement du classifieur de Haar
	String classifieur_visage_fichier = "haarcascade_frontalface_default.xml";
	CascadeClassifier classifieur_visage;

	if (!classifieur_visage.load(classifieur_visage_fichier)){
		cerr << "Erreur: Impossible de charger le Detecteur de Haar de visages" << endl;
		exit(-4);
	}
	//Recherche de visage(s) dans l'image
	vector<Rect> visages;
	Mat monImage_niveauxGris;

	cvtColor(monImage, monImage_niveauxGris, CV_BGR2GRAY);
	equalizeHist(monImage_niveauxGris, monImage_niveauxGris);

	classifieur_visage.detectMultiScale(monImage_niveauxGris, visages, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	if (!visages.empty()){
		cadreEnglobant = visages[0]; //on recupere le premier uniquement

		for (Rect cadre : visages){
			if ((cadreEnglobant.height*cadreEnglobant.width) < (cadre.height*cadre.width))cadreEnglobant = cadre;
			//rectangle(monImage, cadre, 'r');
		}
	}



	//Retour du résultat de la recherche
	return cadreEnglobant;
}


/*
Methode lisserPeau
Paramètre : Mat monImage : la matrice de l'image à traiter
Retourne  : Mat          : la matrice de l'image après le lissage
Principe  : Cherche le masque des pixels de peau d'une image et à partir de celui-ci,
on lisse l'image en nous inspirant de la méthode de Lee et Al.
*/
Mat lisserPeau(Mat monImage){

	//Recherche du masque de peau 
	DetecteurPeau monDetecteurPeau;
	Mat           masquePeau = monDetecteurPeau.getMasquePeauFiltré(monImage);

	//Lissage de la peau avec la méthode de Lee et Al.
	LisseurPeau   lisseurPeau;
	Mat           resultat = lisseurPeau.lisserImageMethodeLee(monImage, masquePeau);

	return resultat;
}


/*
Methode lisserPeauTest
Paramètre : char* monImage : le nom de l'image à traiter
Retourne  : void           : la fonction affiche simplement les images issues des différentes étapes
Principe  : Cherche le masque des pixels de peau d'une image, l'affiche, et à partir de celui-ci,
on lisse l'image en nous inspirant de la méthode de Lee et Al. puis on affiche l'image résultat.
*/
void lisserPeauTest(char* nomImage){
	//Obtention de l'image originale
	Mat monImage = imread(nomImage, CV_LOAD_IMAGE_UNCHANGED);
	imshow("Image Originale", monImage);

	//Detection du masque des pixels de peau
	DetecteurPeau monDetecteurPeau;
	Mat masquePeau;
	masquePeau = monDetecteurPeau.getMasquePeauFiltré(monImage);
	imshow("Masque des pixels de peau", masquePeau);

	//Lissage de la peau selon la méthode de Lee et Al.
	LisseurPeau lisseurPeau;
	Mat resultat = lisseurPeau.lisserImageMethodeLee(monImage, masquePeau);
	imshow("Image après Lissage", resultat);

	waitKey(0);
	cvDestroyAllWindows();
}

/*Retourner matrice d equittage*/
Mat retournerEtiquettage(Mat masquePeau){
	
	unsigned char numero = 1;
	Mat matriceEtiquettage = Mat::zeros(masquePeau.size(), CV_8UC1);;
	

	for (int i = 0; i < masquePeau.rows; i++){
		for (int j = 0; j < masquePeau.cols; j++){
			if ((int)masquePeau.at<unsigned char>(i, j) == 255){ //si pixel est blanc
				determinerEtiquetes(&matriceEtiquettage, i, j, &numero);
			}
		}
	}
	cout << "Nombre étiquettes : " << numero<<endl;
	imshow("etiquettage", matriceEtiquettage);
	return matriceEtiquettage;
}

void determinerEtiquetes(Mat * matriceEtiquettage, int i, int j, unsigned char *numeroEtiquette){
	matriceEtiquettage->at<unsigned char>(i, j) = *numeroEtiquette;
	int nbLigne = matriceEtiquettage->rows-1;
	int nbCol = matriceEtiquettage->cols-1;
	
	if (j>=1){
		matriceEtiquettage->at<unsigned char>(i, j) = inf(matriceEtiquettage->at<unsigned char>(i, j), matriceEtiquettage->at<unsigned char>(i, j - 1));
	}
	
	if (i>=1){
		matriceEtiquettage->at<unsigned char>(i, j) = inf(matriceEtiquettage->at<unsigned char>(i, j), matriceEtiquettage->at<unsigned char>(i - 1, j));
	}
	
	if ((1<=i)&&(i<nbLigne) && (1<=j)&&(j<nbCol)){
		matriceEtiquettage->at<unsigned char>(i, j) = inf(matriceEtiquettage->at<unsigned char>(i, j), matriceEtiquettage->at<unsigned char>(i - 1, j + 1));
	}
	
	if (i>=1 && j>=1){
		matriceEtiquettage->at<unsigned char>(i, j) = inf(matriceEtiquettage->at<unsigned char>(i, j), matriceEtiquettage->at<unsigned char>(i - 1, j - 1));
	}

	if (matriceEtiquettage->at<unsigned char>(i, j) == *numeroEtiquette){
		(*numeroEtiquette)++;
	}
}
 
unsigned char inf(unsigned char etiquetteCourante, unsigned char etiquetteAdjacente){
	unsigned char retour = 0;
	
	if (etiquetteAdjacente == (unsigned char)0){
		retour = etiquetteCourante;
	}
	else{
		if (etiquetteCourante <= etiquetteAdjacente){
			retour = etiquetteCourante;
		}
		else{
			retour = etiquetteAdjacente;
		}
	}
	return retour;
}

/*Fonction qui recupere les etiquettes au pixel*/
set<unsigned char> getEnsembleEtiquette(Mat etiquettes, Rect cadreVisage){
	std::set<unsigned char> monEnsemble;

	for (int i = cadreVisage.x; i < cadreVisage.x + cadreVisage.width;i++){
		for (int j = cadreVisage.y; j < cadreVisage.y + cadreVisage.height; j++){
			
			if ((int)etiquettes.at<unsigned char>(i, j) != 0 && monEnsemble.count(etiquettes.at<unsigned char>(i, j)) == 0){
				monEnsemble.insert(etiquettes.at<unsigned char>(i, j));
			}
			
		}
	}
	return monEnsemble;
}

/*Fonction qui change le masque de peau en tenant compte de la presence ou non de l'etiquette dans le masque englobant.
Si l'etiquette n'est pas dans le cadre englobant alors pas besoin de conserver la composante connexe dans le masque de peau.*/
Mat changerMasquePeau(Mat masqueOriginal, set<unsigned char> ensembleEtiquette, Mat matriceEtiquette){

	Mat nouveauMasquePeau = Mat::zeros(masqueOriginal.size(), CV_8UC1);;
	
	for (int i = 0; i < masqueOriginal.rows; i++){
		for (int j = 0; j < masqueOriginal.cols; j++){
			
			unsigned char etiquette = matriceEtiquette.at<unsigned char>(i, j);

			if (ensembleEtiquette.count(etiquette)){
				nouveauMasquePeau.at<unsigned char>(i, j) = masqueOriginal.at<unsigned char>(i, j);
			}
			
		}
	}

	return nouveauMasquePeau;
}