#include "FloutteurPeau.h"
#include "opencv2\opencv.hpp"
#include<stdio.h>

/*Il existe 4 types de flous sous openCv, pour voir l'effet de chacun de ses flous, décommenter le code dans EssaiFlous et
le lancer. Les 4 types de flous sont flou homogene, flou gaussien, flou médian, flou bilatéral.
Nous avons choisi d'utiliser le flou gaussien car c'est celui qui montre les meilleurs résultats en ne détériorant pas trop l'image ou
en donnant un aspet aquarel a la photo*/

//constructeur
FloutteurPeau::FloutteurPeau(void){
}

//Destructeur
FloutteurPeau::~FloutteurPeau(void){
}

Mat FloutteurPeau::flouterImageTotale(Mat matriceImageBase){
	Mat matriceFlou;
	//imshow("Image Originale", monImage);
	GaussianBlur(matriceImageBase, matriceFlou, cv::Size(7, 7), 2, 2); //plus tard deviation standart hauteur de face/100 voir article
	return matriceFlou;
}

Mat FloutteurPeau::Opacite50prcent(Mat matriceImageBase){

	Mat opacite50;
	/*Mat monImage = imread(imagename, CV_LOAD_IMAGE_UNCHANGED);
	imshow("Image Originale", monImage);
	std::cout << monImage.rows << std::endl;
	std::cout << monImage.cols << std::endl;
	Mat matriceFlou = flouterImageTotale(imagename);
	std::cout << matriceFlou.rows << std::endl;
	std::cout << matriceFlou.cols << std::endl;
	imshow("image floutée", matriceFlou);
	opacite50 = monImage * 0.50;
	std::cout << opacite50.rows << std::endl;
	std::cout << opacite50.cols << std::endl;
	imshow("apres flouttage", opacite50);
	char * imagename2 = "images/Patoch.jpg";
	Mat monImage2 = imread(imagename2, CV_LOAD_IMAGE_UNCHANGED);
	Mat produit = monImage2 * monImage;
	imshow("image floutée", produit);
	waitKey(0);
	cvDestroyAllWindows();*/
	return opacite50;
}

Mat FloutteurPeau::combinaisonMasqueImageOriginale(Mat matriceImageBase, Mat masquePeau){
	Mat matriceSortie;
	Mat matriceFlou = flouterImageTotale(matriceImageBase);
	matriceSortie = Mat::zeros(matriceImageBase.size(), CV_8UC3);
	for (int i = 0; i < masquePeau.rows; i++){
		for (int j = 0; j < masquePeau.cols; j++){
			if ((float)masquePeau.at<unsigned char>(i,j) == 255){
				matriceSortie.at<Vec3b>(i, j) = matriceFlou.at<Vec3b>(i, j)*0.5 + matriceImageBase.at<Vec3b>(i, j)*0.5;
			}else{
				matriceSortie.at<Vec3b>(i, j) = matriceImageBase.at<Vec3b>(i, j);
			}
		}
	}
	return matriceSortie;
}