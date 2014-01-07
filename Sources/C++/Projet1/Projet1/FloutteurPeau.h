#include<opencv\cv.h>
using namespace std;
using namespace cv;

class FloutteurPeau{

public:
	FloutteurPeau(void);
	~FloutteurPeau(void);

	Mat flouterImageTotale(Mat);
	Mat Opacite50prcent(Mat);
	Mat combinaisonMasqueImageOriginale(Mat,Mat);

};