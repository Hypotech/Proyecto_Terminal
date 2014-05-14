#include "RostrosDector.hpp"

#define MI_PATH "haarcascade_frontalface_default.xml"

using namespace cv;

DectorRostros::DectorRostros(float escl, double min, double max, int vecinos)
{
    Escala = escl;
    minimoTamanyo = min;
    maximoTamnyo = max;
    Detector_en_Cascada.load(MI_PATH);
}

bool DectorRostros::EscanearImagen(const Mat& ImagenEntrada, vector<Rect>& UbicacionRostros) 
{
   
if( Detector_en_Cascada.empty() )
    return false;

Mat ImagenByN;
    int width  = ImagenEntrada.size().width, height = ImagenEntrada.size().height;

    // Size minScaleSize = Size(minimoTamanyo  * width, minimoTamanyo  * height),
    // 	maxScaleSize = Size(maximoTamnyo  * width, maximoTamnyo  * height);

    //Obtenemos una copia del frame en blanco/negro y con histrograma normalizado
   cvtColor(ImagenEntrada,ImagenByN,CV_BGR2GRAY);
   equalizeHist(ImagenByN, ImagenByN);

//Funcion para detectar objetos de distinto tamanyo, los objetos detectados son retornados como una lista de rectangulos
   Detector_en_Cascada.detectMultiScale(ImagenByN, UbicacionRostros,1.1, 4, 0
        |CV_HAAR_FIND_BIGGEST_OBJECT
        //|CV_HAAR_DO_ROUGH_SEARCH
        //|CV_HAAR_SCALE_IMAGE
        ,
        Size(30, 30) ); // Escala, Vecinos, 0, minScaleSize, maxScaleSize); 
   return true;
}
