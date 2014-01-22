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
#include<opencv\cv.h>
using namespace std;
using namespace cv;

//-----Entete des méthodes/fonctions-----------------------------------------
class LisseurPeau{

public:
	LisseurPeau(void);
	~LisseurPeau(void);

	Mat lisserImageFiltreGaussien(Mat);
	Mat lisserImageMethodeLee(Mat, Mat);

};