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

#include "DetecteurPeau.h"
#include "opencv2\opencv.hpp"

//-----Corps des méthodes/fonctions------------------------------------------
//Constructeur
DetecteurPeau::DetecteurPeau(void){
	
	//On fixe l'intervalle des couleurs de la peau en YCrCb
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


/*
Methode getMasquePeau
Paramètre : Mat imageEnEntree : la matrice de l'image dont on veut le masque des pixels de peau
Retourne  : Mat               : la matrice qui contient le masque de pixels de peau
Principe  : On passe l'image dans le domaine de couleur YCrCb puis on détecte tous les pixels qui sont
dans les intervalles définis comme caractéristiques des piels de peau.
*/
Mat DetecteurPeau::getMasquePeau(Mat imageEnEntree){
	Mat masquePeau ;
	//Passage de l'image de l'espace de couleurs BGR vers YCrCb pour les traitements
	cvtColor(imageEnEntree, masquePeau, COLOR_BGR2YCrCb);
	//Filtrage des pixels appartenant à l'intervalle des pixels de peau  
	inRange(masquePeau, Scalar(Y_MIN, Cr_MIN, Cb_MIN), Scalar(Y_MAX, Cr_MAX, Cb_MAX), masquePeau);
	return masquePeau;
}


/*
Methode getMasquePeauFiltré
Paramètre : Mat imageEnEntree : la matrice de l'image dont on veut le masque des pixels de peau après optimisation
Retourne  : Mat               : la matrice qui contient le masque de pixels de peau après traitements morphologiques
Principe  : On appelle la méthode getMasquePeau, et à partir du masque de pixels de peau, on applique des
opérateurs morphologiques pour rendre le masque plus net.
*/
//Methode qui va simplifier le masque de peau obtenu normalement avec des operations morphologiques
Mat DetecteurPeau::getMasquePeauFiltré(Mat imageEnEntree){
	Mat masquePeauNormal = DetecteurPeau::getMasquePeau(imageEnEntree);
	Mat masquePeauDilaté, masquePeauErodé;

	//Nous obtenons de bons résultats avec une dilatation et deux érosions
	dilate(masquePeauNormal, masquePeauDilaté, Mat(), Point(-1, -1), 1, 1, 1);
	erode(masquePeauDilaté,  masquePeauErodé,  Mat(), Point(-1, -1), 2, 1, 1);

	return masquePeauErodé;
}

/*
Methode getMatriceEtiquettage
Paramètre : Mat masquePeau : la matrice correspondant au masque de peau de l'image originale.
Retourne  : Mat matriceEtiquettage : la matrice qui contient l'etiquette correspondant a chaque pixel de la matrice de départ.
Principe  : On prend en entrée la matrice du masque de peau et on créer une matrice de la même taille. Chaque cellule contiendra
l'étiquette correspondant à la composante connexe à laquelle appartient le pixel situé au même indice dans le masque de peau.
Nous utilisons cette matrice etiquette dans d'autres fonctions afin de pouvoir déterminer la composante connexe correspondant
reelement au visage et eliminer par filtrage les faux positifs fournis par la méthode getMasquePeauFiltré.
*/
Mat DetecteurPeau::getMatriceEtiquettage(Mat masquePeau){

	unsigned char numero = 1;
	Mat matriceEtiquettage = Mat::zeros(masquePeau.size(), CV_8UC1);

	for (int i = 0; i < masquePeau.rows; i++){
		for (int j = 0; j < masquePeau.cols; j++){
			if ((int)masquePeau.at<unsigned char>(i, j) == 255){ //si pixel est blanc
				determinerEtiquette(&matriceEtiquettage, i, j, &numero);
			}
		}
	}
	imshow("etiquettage", matriceEtiquettage);
	return matriceEtiquettage;
}

/*
Methode determinerEtiquette
Paramètre : Mat matriceEtiquettage : la matrice qui contiendra les etiquettes.
Paramètre : int numeroLigne : le numero de ligne de la matrice du masque de peau pour laquelle il faut calculer l'etiquette.
Paramètre : int numeroColonne : le numeroColonne de la matrice du masque de peau pour laquelle il faut calculer l'etiquette.
Paramètre : char *numeroEtiquette : le pointeur vers le numero d'étiquette courant.
Principe  : Sert à déterminer la valeure d'etiquette d'une cellule précise. On regarde les valeures d'étiquettes sur les 3 pixels supérieurs et le pixel imédiatement à gauche et on prend la plus petite.
Le but est de rattacher notre pixel courant à l'une des composantes connexes qu'il touche.
Si il n'y a pas de pixel de ce type à proximité alors on affecte le numéro d'étiquette courant (paramètre numeroEtiquette).
*/
void DetecteurPeau::determinerEtiquette(Mat * matriceEtiquettage, int numeroLigne, int numeroColonne, unsigned char *numeroEtiquette){
	matriceEtiquettage->at<unsigned char>(numeroLigne, numeroColonne) = *numeroEtiquette;
	int nbLigne = matriceEtiquettage->rows - 1;
	int nbCol = matriceEtiquettage->cols - 1;

	if (numeroColonne >= 1){
		matriceEtiquettage->at<unsigned char>(numeroLigne, numeroColonne) = inf(matriceEtiquettage->at<unsigned char>(numeroLigne, numeroColonne), matriceEtiquettage->at<unsigned char>(numeroLigne, numeroColonne - 1));
	}

	if (numeroLigne >= 1){
		matriceEtiquettage->at<unsigned char>(numeroLigne, numeroColonne) = inf(matriceEtiquettage->at<unsigned char>(numeroLigne, numeroColonne), matriceEtiquettage->at<unsigned char>(numeroLigne - 1, numeroColonne));
	}

	if ((1 <= numeroLigne) && (numeroLigne<nbLigne) && (1 <= numeroColonne) && (numeroColonne<nbCol)){
		matriceEtiquettage->at<unsigned char>(numeroLigne, numeroColonne) = inf(matriceEtiquettage->at<unsigned char>(numeroLigne, numeroColonne), matriceEtiquettage->at<unsigned char>(numeroLigne - 1, numeroColonne + 1));
	}

	if (numeroLigne >= 1 && numeroColonne >= 1){
		matriceEtiquettage->at<unsigned char>(numeroLigne, numeroColonne) = inf(matriceEtiquettage->at<unsigned char>(numeroLigne, numeroColonne), matriceEtiquettage->at<unsigned char>(numeroLigne - 1, numeroColonne - 1));
	}

	if (matriceEtiquettage->at<unsigned char>(numeroLigne, numeroColonne) == *numeroEtiquette){
		(*numeroEtiquette)++;
	}
}

/*
Methode inf
Paramètre : unsigned char etiquetteCourante : la valeure d etiquette actuellement affecté à la cellule.
Paramètre : unsigned char etiquetteAdjacente : la valeure de l'étiquette adajacente à tester.
Retourne  : unsigned char retour : la valeure la plus petite entre etiquetteCourante et etiquetteAdjacente.
Principe  : On va comparer la valeure de etiquetteCourante et etiquetteAdjacente. Si etiquetteAdjacente est plus petite alors
on on retourne cette valeure sinon on garde la valeure actuelle.
Le but est de déterminer la composante connexe la plus petite en terme de numéro d'étiquette à laquelle rattacher notre objet.
*/
unsigned char DetecteurPeau::inf(unsigned char etiquetteCourante, unsigned char etiquetteAdjacente){
	unsigned char retour = 0;

	if (etiquetteAdjacente == (unsigned char)0){
		retour = etiquetteCourante;
	}
	else{
		if (etiquetteCourante <= etiquetteAdjacente){
			retour = etiquetteCourante;
		}
		else{
			retour = etiquetteAdjacente;
		}
	}
	return retour;
}

/*
Methode inf
Paramètre : Mat monImage : la matrice correpondant a l'image originale.
Retourne  : Rect cadreEnglobant : le cadre englobant le visage déterminer selon la méthode de viola et Jones.
Principe  : On va utiliser la méthode de Haar pour détecter automatiquement le visage et retourner le cadre englobant.

*/
Rect DetecteurPeau::detecterCadreVisage(Mat monImage){
	Rect cadreEnglobant = Rect();
	//Chargement du classifieur de Haar
	String classifieur_visage_fichier = "haarcascade_frontalface_default.xml";
	CascadeClassifier classifieur_visage;

	if (!classifieur_visage.load(classifieur_visage_fichier)){
		cerr << "Erreur: Impossible de charger le Detecteur de Haar de visages" << endl;
		exit(-4);
	}
	//Recherche de visage(s) dans l'image
	vector<Rect> visages;
	Mat monImage_niveauxGris;

	cvtColor(monImage, monImage_niveauxGris, CV_BGR2GRAY);
	equalizeHist(monImage_niveauxGris, monImage_niveauxGris);

	classifieur_visage.detectMultiScale(monImage_niveauxGris, visages, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	if (!visages.empty()){
		cadreEnglobant = visages[0]; //on recupere le premier uniquement

		for (Rect cadre : visages){
			if ((cadreEnglobant.height*cadreEnglobant.width) < (cadre.height*cadre.width))cadreEnglobant = cadre;
			//rectangle(monImage, cadre, 'r');
		}
	}



	//Retour du résultat de la recherche
	return cadreEnglobant;
}

/*
Methode getEnsembleEtiquette
Paramètre : Mat etiquettes : la matrice contenant toutes les étiquettes.
Paramètre : Rect cadreVisage : le rectangle englobant le visage obtenu par la methode de Viola et Jones.
Retourne  : set<unsigned char> ensemble: la liste des etiquettes figurant dans le cadre englobant.
Principe  : On recupère toutes les étiquettes présentes dans la masque englobant. Ainsi par la suite on pourra faire un traitement
afin de ne conserver dans notre masque de peau que les pixels correspondant à ces étiquettes et supprimer les faux positifs.
*/
set<unsigned char> DetecteurPeau::getEnsembleEtiquette(Mat etiquettes, Rect cadreVisage){
	std::set<unsigned char> ensemble;

	for (int i = cadreVisage.x; i < cadreVisage.x + cadreVisage.width; i++){
		for (int j = cadreVisage.y; j < cadreVisage.y + cadreVisage.height; j++){

			if ((int)etiquettes.at<unsigned char>(i, j) != 0 && ensemble.count(etiquettes.at<unsigned char>(i, j)) == 0){
				ensemble.insert(etiquettes.at<unsigned char>(i, j));
			}

		}
	}
	return ensemble;
}

/*
Methode changerMasquePeau
Paramètre : Mat masqueOriginal : la matrice contenant le masque de peau.
Paramètre : set<unsigned char> ensembleEtiquette : l'ensemble des etiquettes présentes dans le cadre englobant. 
On l'obtient avec getEnsembleEtiquette.
Paramètre  : Mat matriceEtiquette: la matrice contenant les étiquettes.
Retourne  : Mat nouveauMasquePeau: la matrice contenant le masque de peau épuré des faux positifs.
Principe  : On va supprimer tous les faux positifs dans la matrice masqueOriginal en mettant à 0(noir) tous les pixels
qui ne correpondent pas à une étiquette présente dans le cadre englobant (présent dans ensembleEtiquette).
 
*/
Mat DetecteurPeau::changerMasquePeau(Mat masqueOriginal, set<unsigned char> ensembleEtiquette, Mat matriceEtiquette){

	Mat nouveauMasquePeau = Mat::zeros(masqueOriginal.size(), CV_8UC1);;

	for (int i = 0; i < masqueOriginal.rows; i++){
		for (int j = 0; j < masqueOriginal.cols; j++){

			unsigned char etiquette = matriceEtiquette.at<unsigned char>(i, j);

			if (ensembleEtiquette.count(etiquette)){
				nouveauMasquePeau.at<unsigned char>(i, j) = masqueOriginal.at<unsigned char>(i, j);
			}

		}
	}

	return nouveauMasquePeau;
}