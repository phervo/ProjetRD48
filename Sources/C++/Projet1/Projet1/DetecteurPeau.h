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

#pragma once
//Inspiré du travail de http://razibdeb.wordpress.com/2013/09/10/skin-detection-in-c-using-opencv/

#include<opencv\cv.h>
using namespace std;
using namespace cv;

//-----Entete des méthodes/fonctions-----------------------------------------
class DetecteurPeau{

public:
	DetecteurPeau(void);
	~DetecteurPeau(void);

	Mat getMasquePeau(Mat);
	Mat getMasquePeauFiltré(Mat);
	Mat getMatriceEtiquetage(Mat);
	Mat filtrerMasquePeauSelonComposantesConnexes(Mat, set<unsigned char>, Mat);
	Rect detecterCadreVisage(Mat);
	set<unsigned char> getEtiquettesDansCadreVisage(Mat, Rect);
	unsigned char getEtiquetteInferieure(unsigned char, unsigned char);
	void choisirEtiquette(Mat*, int, int, unsigned char*);
	
	
private:
	int Y_MIN;
	int Y_MAX;
	int Cb_MIN;
	int Cb_MAX;
	int Cr_MIN;
	int Cr_MAX;
};