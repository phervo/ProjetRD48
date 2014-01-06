#define _CRT_SECURE_NO_WARNINGS
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "DetecteurPeau.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace cv;
using namespace std;

// Entete de fonctions
void getPeauImage(char*);

int main(int argc, char *argv[])
{
	char *imagename = "images/o1.jpg";

	getPeauImage(imagename);
	return 0;
}

void getPeauImage(char* imagename){
	Mat monImage = imread(imagename, CV_LOAD_IMAGE_UNCHANGED);
	imshow("Image Originale", monImage);
	DetecteurPeau monDetecteurPeau;
	Mat masquePeau;
	masquePeau = monDetecteurPeau.getMasquePeauFiltré(monImage);
	imshow("Masque de peau", masquePeau);
	waitKey(0);
	cvDestroyAllWindows();
}