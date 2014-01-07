#include "DetecteurPeau.h"
#include "opencv2\opencv.hpp"

//On fixe l'intervalle des couleurs de la peau en YCrCb
DetecteurPeau::DetecteurPeau(void){

	Y_MIN = 80;
	Y_MAX = 255;

	Cb_MIN = 85;
	Cb_MAX = 135;
	
	Cr_MIN = 135;
	Cr_MAX = 180;
}

//Destructeur
DetecteurPeau::~DetecteurPeau(void){
}

//Methode qui va chercher les pixels de peau de l'image en entrée
Mat DetecteurPeau::getMasquePeau(Mat imageEnEntree){
	Mat masquePeau;
	//Passage de l'image de l'espace de couleurs BGR vers YCrCb pour les traitements
	cvtColor(imageEnEntree, masquePeau, COLOR_BGR2YCrCb);
	//Filtrage des pixels appartenant à l'intervalle des pixels de peau  
	inRange(masquePeau, Scalar(Y_MIN, Cr_MIN, Cb_MIN), Scalar(Y_MAX, Cr_MAX, Cb_MAX), masquePeau);
	return masquePeau;
}

//Methode qui va simplifier le masque de peau obtenu normalement avec des operations morphologiques
Mat DetecteurPeau::getMasquePeauFiltré(Mat imageEnEntree){
	Mat masquePeauNormal = DetecteurPeau::getMasquePeau(imageEnEntree);
	Mat masquePeauDilaté, masquePeauErodé;

	//Nous obtenons de bons résultats avec une dilatation et deux érosions
	dilate(masquePeauNormal, masquePeauDilaté, Mat(), Point(-1, -1), 1, 1, 1);
	erode(masquePeauDilaté,  masquePeauErodé,  Mat(), Point(-1, -1), 2, 1, 1);

	return masquePeauErodé;
}

