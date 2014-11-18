#ifndef _RECONOCERPERSONA_HPP_
#define _RECONOCERPERSONA_HPP_

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "manejoarchivos.hpp"

#define FILE_CSV "./index.csv"

using namespace cv;
using namespace std;
class ReconocerdordePersona
{
public:
    enum metodo {eigen,fisher,LBPH};
    ReconocerdordePersona(int Neigen = 8, double ranConfia = 100, int radio = 1,
                          int regX = 8, int regY = 8, metodo modelo = LBPH);
    ReconocerdordePersona(std::string ruta = "BD", int Neigen = 8, double ranConfia = 100,
                          int radio = 1,int regX = 8, int regY = 8, metodo modelo = LBPH);

    int consulatarBD(Mat& ImagenRostro) const;
    int consulatarBD(Mat& ImagenRostro, double& confianza) const;

private:
    Ptr<FaceRecognizer> Reconocedor;

    void obtenerParametrosCSV(const string&, vector<Mat>& , vector <int>& );
    void cambiarModelo(int Neigen, double ranConfia, int radio, int regX,
                       int regY, metodo modelo);
    void obtenerParametrosDirectorio(const string& ruta, vector<Mat>& imagenes,
                                     vector<int> & etiquetas);
};


#endif /* _RECONOCERPERSONA_HPP_ */
