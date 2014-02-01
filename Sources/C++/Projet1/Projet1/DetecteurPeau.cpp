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
opérateurs morphologiques pour rendre le masque plus net et on filtre selon les composantes connexes débutant dans la
boite englobante du visage.
*/
//Methode qui va simplifier le masque de peau obtenu normalement avec des operations morphologiques
Mat DetecteurPeau::getMasquePeauFiltré(Mat imageEnEntree){
	Mat masquePeauNormal = DetecteurPeau::getMasquePeau(imageEnEntree);
	Mat masquePeauDilaté, masquePeauErodé;

	//Nous obtenons de bons résultats avec une dilatation et deux érosions
	dilate(masquePeauNormal, masquePeauDilaté, Mat(), Point(-1, -1), 1, 1, 1);
	erode(masquePeauDilaté,  masquePeauErodé,  Mat(), Point(-1, -1), 2, 1, 1);
	
	//Nous allons maintenant filtrer ce masque de peau en ne conservant que les pixels qui appartiennent aux composantes connexes
	//qui commencent dans la boite englobante du visage.

	//On commence par chercher la matrice des composantes connexes
	Mat composantesConnexes               = getMatriceEtiquetage(masquePeauErodé);
	
	Rect cadreEnglobant                   = detecterCadreVisage(imageEnEntree);
	set<unsigned char>  etiquettes        = getEtiquettesDansCadreVisage(composantesConnexes, cadreEnglobant);

	//Et on élimine par filtrage une grande partie des faux positifs restant
	Mat nouveauMasque = filtrerMasquePeauSelonComposantesConnexes(masquePeauErodé, etiquettes, composantesConnexes);

	return masquePeauErodé;
}

/*
Methode getMatriceEtiquetage
Paramètre : Mat masquePeau : la matrice correspondant au masque des pixels de peau de l'image originale.
Retourne  : Mat            : la matrice qui contient les étiquettes des composantes connexes du masque des pixels de peau.
Principe  : On prend en entrée la matrice du masque des pixels peau. On cherche ensuite à affecter chaque pixel à une composante
connexe via une étiquette. La matrice finale est donc une matrice d'étiquettes correspondant aux composantes connexes
*/
Mat DetecteurPeau::getMatriceEtiquetage(Mat masquePeau){

	unsigned char numero = 1;
	//Tous les pixels sont étiquetés à 0 (noir) au départ
	Mat composantesConnexes = Mat::zeros(masquePeau.size(), CV_8UC1);

	for (int i = 0; i < masquePeau.rows; i++){
		for (int j = 0; j < masquePeau.cols; j++){
			if ((int)masquePeau.at<unsigned char>(i, j) == 255){ 
				//Cas d'un pixel de peau
				choisirEtiquette(&composantesConnexes, i, j, &numero);
			}
		}
	}
	return composantesConnexes;
}

/*
Methode choisirEtiquette
Paramètre : Mat matriceEtiquetage          : la matrice qui contient les etiquettes des composantes connexes.
Paramètre : int numeroLigne                : le numero de ligne de la matrice du masque de peau pour laquelle il faut calculer l'etiquette.
Paramètre : int numeroColonne              : le numeroColonne de la matrice du masque de peau pour laquelle il faut calculer l'etiquette.
Paramètre : unsigned char *numeroEtiquette : le pointeur vers le numero d'étiquette courant.
Principe  : Sert à déterminer la valeur d'étiquette d'une cellule précise.On affecte le nouveau numéro d'étiquette par défaut. Puis on compare 
l'étiquette courante avec les valeurs des étiquettes des pixels précédemment étiquetés selon un 8-voisinage.
Le but est de rattacher notre pixel courant à l'une des composantes connexes qu'il touche.
S'il n'y a pas de pixel de ce type à proximité alors on conserve le numéro d'étiquette courant.
*/
void DetecteurPeau::choisirEtiquette(Mat * matriceEtiquetage, int numeroLigne, int numeroColonne, unsigned char *numeroEtiquette){
	matriceEtiquetage->at<unsigned char>(numeroLigne, numeroColonne) = *numeroEtiquette;
	int nbLigne = matriceEtiquetage->rows - 1;
	int nbCol   = matriceEtiquetage->cols - 1;

	//Comparaison de l'étiquette courante avec les 4 pixels voisins potentiels précédemments évalués

	if (numeroColonne >= 1){
		matriceEtiquetage->at<unsigned char>(numeroLigne, numeroColonne) = getEtiquetteInferieure(matriceEtiquetage->at<unsigned char>(numeroLigne, numeroColonne), matriceEtiquetage->at<unsigned char>(numeroLigne, numeroColonne - 1));
	}

	if (numeroLigne >= 1){
		matriceEtiquetage->at<unsigned char>(numeroLigne, numeroColonne) = getEtiquetteInferieure(matriceEtiquetage->at<unsigned char>(numeroLigne, numeroColonne), matriceEtiquetage->at<unsigned char>(numeroLigne - 1, numeroColonne));
	}

	if ((1 <= numeroLigne) && (numeroLigne<nbLigne) && (1 <= numeroColonne) && (numeroColonne<nbCol)){
		matriceEtiquetage->at<unsigned char>(numeroLigne, numeroColonne) = getEtiquetteInferieure(matriceEtiquetage->at<unsigned char>(numeroLigne, numeroColonne), matriceEtiquetage->at<unsigned char>(numeroLigne - 1, numeroColonne + 1));
	}

	if (numeroLigne >= 1 && numeroColonne >= 1){
		matriceEtiquetage->at<unsigned char>(numeroLigne, numeroColonne) = getEtiquetteInferieure(matriceEtiquetage->at<unsigned char>(numeroLigne, numeroColonne), matriceEtiquetage->at<unsigned char>(numeroLigne - 1, numeroColonne - 1));
	}

	if (matriceEtiquetage->at<unsigned char>(numeroLigne, numeroColonne) == *numeroEtiquette){
		(*numeroEtiquette)++;
	}
}

/*
Methode getEtiquetteInferieure
Paramètre : unsigned char etiquetteCourante  : la valeur d'étiquette actuellement affectée à la cellule.
Paramètre : unsigned char etiquetteAdjacente : la valeur de l'étiquette adjacente à tester.
Retourne  : unsigned char                    : la valeur la plus petite entre etiquetteCourante et etiquetteAdjacente.
Principe  : On va comparer la valeur de etiquetteCourante et etiquetteAdjacente. Si etiquetteAdjacente est plus petite 
alors on retourne cette valeur sinon on garde la valeure actuelle.
Le but est de déterminer la composante connexe la plus petite en terme de numéro d'étiquette à laquelle rattacher notre objet.
*/
unsigned char DetecteurPeau::getEtiquetteInferieure(unsigned char etiquetteCourante, unsigned char etiquetteAdjacente){
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
Methode detecterCadreVisage
Paramètre : Mat monImage        : la matrice correspondant à l'image originale.
Retourne  : Rect cadreEnglobant : le cadre englobant le visage déterminé selon la méthode de viola et Jones.
Principe  : On va utiliser la méthode de Haar pour détecter automatiquement le visage et retourner le plus grand cadre englobant.
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
		//On commence par récupérer le premier cadre
		cadreEnglobant = visages[0];

		//Puis on cherche le plus grand cadre s'il y en a plusieurs
		for (Rect cadre : visages){
			if ((cadreEnglobant.height*cadreEnglobant.width) < (cadre.height*cadre.width))cadreEnglobant = cadre;
		}
	}

	//Retour du résultat de la recherche
	return cadreEnglobant;
}

/*
Methode getEtiquettesDansCadreVisage
Paramètre : Mat etiquettes              : la matrice contenant toutes les étiquettes des composantes connexes.
Paramètre : Rect cadreVisage            : le rectangle englobant le visage obtenu par la methode de Viola et Jones.
Retourne  : set<unsigned char> ensemble : la liste des étiquettes figurant dans le cadre englobant.
Principe  : On recupère toutes les étiquettes présentes dans le cadre englobant. Ainsi par la suite on pourra faire un traitement
afin de ne conserver dans notre masque de peau que les pixels correspondant à ces étiquettes et supprimer les faux positifs.
*/
set<unsigned char> DetecteurPeau::getEtiquettesDansCadreVisage(Mat etiquettes, Rect cadreVisage){
	std::set<unsigned char> ensemble;

	for (int i = cadreVisage.x; i < cadreVisage.x + cadreVisage.width; i++){
		for (int j = cadreVisage.y; j < cadreVisage.y + cadreVisage.height; j++){
			//On ne conserve que les étiquettes des composantes connexes qui apparaissent dans le cadre englobant
			if ((int)etiquettes.at<unsigned char>(i, j) != 0 && ensemble.count(etiquettes.at<unsigned char>(i, j)) == 0){
				ensemble.insert(etiquettes.at<unsigned char>(i, j));
			}

		}
	}
	return ensemble;
}

/*
Methode filtrerMasquePeauSelonComposantesConnexes
Paramètre                                         : Mat masqueOriginal : la matrice contenant le masque de peau.
Paramètre : set<unsigned char> ensembleEtiquettes : l'ensemble des etiquettes présentes dans le cadre englobant.
Paramètre : Mat matriceEtiquettes                 : la matrice contenant les étiquettes des composantes connexes.
Retourne  : Mat                                   : la matrice contenant le masque de peau épuré des faux positifs.
Principe  : On va supprimer tous les faux positifs dans la matrice masqueOriginal en mettant à 0 (noir) tous les pixels
qui ne correspondent pas à une étiquette présente dans le cadre englobant.
*/
Mat DetecteurPeau::filtrerMasquePeauSelonComposantesConnexes(Mat masqueOriginal, set<unsigned char> ensembleEtiquettes, Mat matriceEtiquettes){

	Mat nouveauMasquePeau = Mat::zeros(masqueOriginal.size(), CV_8UC1);;

	for (int i = 0; i < masqueOriginal.rows; i++){
		for (int j = 0; j < masqueOriginal.cols; j++){

			unsigned char etiquette = matriceEtiquettes.at<unsigned char>(i, j);

			if (ensembleEtiquettes.count(etiquette)){
				nouveauMasquePeau.at<unsigned char>(i, j) = masqueOriginal.at<unsigned char>(i, j);
			}

		}
	}

	return nouveauMasquePeau;
}
