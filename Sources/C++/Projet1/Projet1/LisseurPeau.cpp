#include "LisseurPeau.h"
#include "opencv2\opencv.hpp"
#include<stdio.h>

/*Il existe 4 types de flous sous openCv, pour voir l'effet de chacun de ses flous, décommenter le code dans EssaiFlous et
le lancer. Les 4 types de flous sont flou homogene, flou gaussien, flou médian, flou bilatéral.
Nous avons choisi d'utiliser le flou gaussien car c'est celui qui montre les meilleurs résultats en ne détériorant pas trop l'image ou
en donnant un aspet aquarel a la photo*/

//Constructeur
LisseurPeau::LisseurPeau(void){
}

//Destructeur
LisseurPeau::~LisseurPeau(void){
}

Mat LisseurPeau::lisserImageFiltreGaussien(Mat monImageEnEntree){
	Mat imageLissée;
	GaussianBlur(monImageEnEntree, imageLissée, cv::Size(7, 7), 2, 2); //plus tard deviation standart hauteur de face/100 voir article
	return imageLissée;
}

Mat LisseurPeau::lisserImageMethodeLee(Mat monImageEnEntree, Mat masquePeau){
	Mat imageFinale;
	imageFinale = Mat::zeros(monImageEnEntree.size(), CV_8UC3);
	Mat imageLissée = lisserImageFiltreGaussien(monImageEnEntree);

	for (int i = 0; i < masquePeau.rows; i++){
		for (int j = 0; j < masquePeau.cols; j++){
			if ((float)masquePeau.at<unsigned char>(i, j) == 255){
				imageFinale.at<Vec3b>(i, j) = imageLissée.at<Vec3b>(i, j)*0.5 + monImageEnEntree.at<Vec3b>(i, j)*0.5;
			}
			else{
				imageFinale.at<Vec3b>(i, j) = monImageEnEntree.at<Vec3b>(i, j);
			}
		}
	}
	return imageFinale;
}