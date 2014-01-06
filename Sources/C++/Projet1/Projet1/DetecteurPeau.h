#pragma once
//Inspiré du travail de http://razibdeb.wordpress.com/2013/09/10/skin-detection-in-c-using-opencv/

#include<opencv\cv.h>
using namespace std;
using namespace cv;

class DetecteurPeau{

public:
	DetecteurPeau(void);
	~DetecteurPeau(void);

	Mat getMasquePeau(Mat imageEnEntree);
	Mat getMasquePeauFiltré(Mat imageEnEntree);

private:
	int Y_MIN;
	int Y_MAX;
	int Cb_MIN;
	int Cb_MAX;
	int Cr_MIN;
	int Cr_MAX;
};