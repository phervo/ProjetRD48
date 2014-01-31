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
void determinerEtiquetes(Mat * matriceEtiquettage, int i, int j, int *numeroEtiquette);
int inf(int etiquetteCourante, int etiquetteAdjacente);
Rect detecterCadreVisage(Mat monImage);
Mat changerMasquePeau(Mat masqueOriginal, set<int> ensembleEtiquette, Mat matriceEtiquette);
set<int> getEnsembleEtiquette(Mat etiquettes, Rect cadreVisage);


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
	Mat monImageCible = imread("Images_Test/o1.jpg", CV_LOAD_IMAGE_UNCHANGED);

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
	imshow("Masque des pixels de peau", masquePeau);
	Mat etiquettage = retournerEtiquettage(masquePeau);
	Rect cadreEnglobant = detecterCadreVisage(monImageCible);
	/*set<int> ensembleEtiquette = getEnsembleEtiquette(etiquettage, cadreEnglobant);
	Mat resultat = changerMasquePeau(masquePeau, ensembleEtiquette, etiquettage);
	imshow("Image après Lissage", resultat);*/
	waitKey(0);
	cvDestroyAllWindows();
	//Ecriture de l'image résultante
	//imwrite(argv[2], resultat);

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
	
	int numero = 1;
	Mat matriceEtiquettage = Mat(masquePeau.rows, masquePeau.cols, CV_8SC1);
	for (int i = 0; i < masquePeau.rows; i++){
		for (int j = 0; j < masquePeau.cols; j++){
			matriceEtiquettage.at<int>(i, j) = 0;
		}
	}
	cout << matriceEtiquettage .rows<< endl;
	cout << matriceEtiquettage.cols << endl;
	

	for (int i = 0; i < masquePeau.rows; i++){
		for (int j = 0; j < masquePeau.cols; j++){
			if ((float)masquePeau.at<int>(i, j) == 255){ //si pixel est blanc
				determinerEtiquetes(&matriceEtiquettage, i, j, &numero);
			}
			else{
				matriceEtiquettage.at<int>(i, j) = 0;
			}
		}
	}
	imshow("etiquettage", matriceEtiquettage);
	return matriceEtiquettage;
}

void determinerEtiquetes(Mat * matriceEtiquettage, int i, int j, int *numeroEtiquette){
	matriceEtiquettage->at<int>(i, j) = *numeroEtiquette;
	int nbLigne = matriceEtiquettage->rows-1;
	int nbCol = matriceEtiquettage->cols-1;
	
	cout << "i" <<i << endl;
	cout << "j"<<j << endl;

	if (j>=1){
		matriceEtiquettage->at<int>(i, j) = inf(matriceEtiquettage->at<int>(i, j), matriceEtiquettage->at<int>(i, j - 1));
	}
	
	if (i>=1){
		matriceEtiquettage->at<int>(i, j) = inf(matriceEtiquettage->at<int>(i, j), matriceEtiquettage->at<int>(i - 1, j));
	}
	
	if (1<=i<nbLigne && 1<=j<nbCol){
		matriceEtiquettage->at<int>(i, j) = inf(matriceEtiquettage->at<int>(i, j), matriceEtiquettage->at<int>(i - 1, j + 1));
	}
	
	if (i>=1 && j>=1){
		matriceEtiquettage->at<int>(i, j) = inf(matriceEtiquettage->at<int>(i, j), matriceEtiquettage->at<int>(i - 1, j - 1));
	}

	if (matriceEtiquettage->at<int>(i, j) == *numeroEtiquette){
		(*numeroEtiquette)++;
		cout << "nouvelle etiquette" << *numeroEtiquette << endl;
	}
}
 
int inf(int etiquetteCourante,int etiquetteAdjacente){
	int retour=0;
	cout << "etiquetteCourante" <<etiquetteCourante << endl;
	cout << "etiquetteAdjacente" << etiquetteAdjacente << endl;
	if (etiquetteAdjacente == 0){
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
	cout << "valeure de retour" << retour << endl;
	return retour;
}

/*Fonction qui recupere les etiquettes au pixel*/
set<int> getEnsembleEtiquette(Mat etiquettes, Rect cadreVisage){
	std::set<int> monEnsemble;

	for (int i = cadreVisage.x; i < cadreVisage.x + cadreVisage.width;i++){
		for (int j = cadreVisage.y; j < cadreVisage.y + cadreVisage.height; j++){
			//
			if (etiquettes.at<int>(i, j) != 0 &&  monEnsemble.count(etiquettes.at<int>(i, j)) == 0){
				cout << etiquettes.at<int>(i, j) << endl;
				monEnsemble.insert(etiquettes.at<int>(i, j));
			}
			
		}
	}
	return monEnsemble;
}

/*Fonction qui change le masque de peau en tenant compte de la presence ou non de l'etiquette dans le masque englobant.
Si l'etiquette n'est pas dans le cadre englobant alors pas besoin de conserver la composante connexe dans le masque de peau.*/
Mat changerMasquePeau(Mat masqueOriginal, set<int> ensembleEtiquette, Mat matriceEtiquette){
	Mat nouveauMasquePeau = Mat::zeros(masqueOriginal.size(), CV_8UC1);;
	for (int i = 0; i < masqueOriginal.rows; i++){
		for (int j = 0; j < masqueOriginal.cols; j++){
			//cout << ensembleEtiquette.count(matriceEtiquette.at<int>(i, j)) << endl;
			if (ensembleEtiquette.count(matriceEtiquette.at<int>(i, j)) == 0){
				nouveauMasquePeau.at<unsigned char>(i, j) = 0;
			}
			else{
				nouveauMasquePeau.at<unsigned char>(i, j) = (int)masqueOriginal.at<unsigned char>(i, j);
			}
		}
	}
	return nouveauMasquePeau;
}