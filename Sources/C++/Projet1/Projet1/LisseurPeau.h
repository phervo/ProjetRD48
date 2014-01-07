#include<opencv\cv.h>
using namespace std;
using namespace cv;

class LisseurPeau{

public:
	LisseurPeau(void);
	~LisseurPeau(void);

	Mat lisserImageFiltreGaussien(Mat);
	Mat lisserImageMethodeLee(Mat, Mat);

};