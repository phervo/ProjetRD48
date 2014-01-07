#pragma once
#include<opencv\cv.h>
using namespace std;
using namespace cv;

class CorrectionGenerales{

public:
	CorrectionGenerales(void);
	~CorrectionGenerales(void);

	Mat corrigerContrasteEtLuminosité(Mat imageEnEntree);
	Mat corrigerLuminosité(Mat imageEnEntree);
	Mat corrigerBruit(Mat imageEnEntree);

};