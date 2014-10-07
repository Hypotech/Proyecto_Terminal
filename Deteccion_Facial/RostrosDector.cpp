#include "RostrosDector.hpp"

#define MI_PATH "haarcascade.xml"

using namespace cv;

DectorRostros::DectorRostros()
{
    // Escala = escl;
    // minimoTamanyo = min;
    // maximoTamnyo = max;
    Detector_en_Cascada.load(MI_PATH);
}

bool DectorRostros::clasificadorCargado()
{
    if( Detector_en_Cascada.empty() ){
	return false;
    }
    else
	return true;

}

void DectorRostros::EscanearImagen(const Mat& ImagenEntrada, vector<Rect>& UbicacionRostros,
                                   const int minEscala)
{
    Mat ImagenByN;
    // int width  = ImagenEntrada.size().width, height = ImagenEntrada.size().height;
    // Size minScaleSize = Size(minimoTamanyo  * width, minimoTamanyo  * height),
    // 	maxScaleSize = Size(maximoTamnyo  * width, maximoTamnyo  * height);

    //Obtenemos una copia del frame en blanco/negro y con histrograma normalizado
    // cvtColor(ImagenEntrada,ImagenByN,CV_BGR2GRAY); //ya no es necesario, pues ya se recibe en ByN
    ImagenByN = ImagenEntrada.clone(); //ahora simplemente se copia
    equalizeHist(ImagenByN, ImagenByN);

    //Funcion para detectar objetos, los objetos detectados son retornados como una lista de rectangulos
    Detector_en_Cascada.detectMultiScale(ImagenByN, UbicacionRostros,1.1, 4, 0
					 |CV_HAAR_FIND_BIGGEST_OBJECT
					 //|CV_HAAR_DO_ROUGH_SEARCH
					 //|CV_HAAR_SCALE_IMAGE
					 ,
                     Size(minEscala, minEscala) ); // Escala, Vecinos, 0, minScaleSize, maxScaleSize);
}
