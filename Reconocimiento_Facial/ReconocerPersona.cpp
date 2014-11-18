#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "ReconocerPersona.hpp"

using namespace cv;
using namespace std;

ReconocerdordePersona::ReconocerdordePersona(int Neigen, double ranConfia, int radio,
                                             int regX, int regY, metodo modelo)
{
    vector <Mat> imagenes;
    vector<int> etiquetas;

    cout << "Inicializando Reconocedor de rostros...";

    obtenerParametrosCSV(FILE_CSV,imagenes, etiquetas);
    cambiarModelo (Neigen, ranConfia, radio, regX, regY, modelo);

    cout<<"Entrenando...";
    Reconocedor->train(imagenes,etiquetas);

    cout << "(Listo)"<< endl;
}
ReconocerdordePersona::ReconocerdordePersona(std::string ruta, int Neigen, double ranConfia, int radio,
                                             int regX, int regY, metodo modelo)
{
    vector <Mat> imagenes;
    vector<int> etiquetas;

    cout << "Inicializando Reconocedor de rostros... "<< endl;

    obtenerParametrosDirectorio(ruta,imagenes,etiquetas);
    cambiarModelo (Neigen, ranConfia, radio, regX, regY, modelo);

    cout<<"Entrenando..."<<endl;
    Reconocedor->train(imagenes,etiquetas);

    cout << "(Listo)"<< endl;
}

int ReconocerdordePersona::consulatarBD(Mat& ImagenRostro) const
{
    return Reconocedor->predict(ImagenRostro); 
}

int ReconocerdordePersona::consulatarBD(Mat& ImagenRostro, double& confianza) const
{
    int label;
    Reconocedor->predict(ImagenRostro, label, confianza); 
    return label;
}

void ReconocerdordePersona::obtenerParametrosCSV(const string& ArchivoCSV,
                                                 vector<Mat>& imagenes, vector <int>& etiquetas)
{
    std::ifstream file(ArchivoCSV.c_str(), ifstream::in);

    char separator= ';';

    if (!file) {
        string error_message = "No valid input file was given, please check the given ArchivoCSV.";
        CV_Error(CV_StsBadArg, error_message);
    }

    string line, path, classlabel;
    while (getline(file, line))  //itera hasta que ya no haya lineas en file
	{
	    stringstream liness(line); //copia la linea actual en liness
	    getline(liness, path, separator); //determinamos el path de la linea actual
	    getline(liness, classlabel); //determinamos la etiqueta

	    cout<< path<< ";" << classlabel<<endl;

	    if(!path.empty() && !classlabel.empty()) //si no es un espacio en blanco almacena las imagenes y las etiquetas
		{
		    imagenes.push_back(imread(path, 0));
		    etiquetas.push_back(atoi(classlabel.c_str()));
		}
	}

}

void ReconocerdordePersona::cambiarModelo(int Neigen, double ranConfia, int radio, int regX,
                                          int regY, metodo modelo)
{
    if (modelo == LBPH)
        Reconocedor = createLBPHFaceRecognizer(radio,Neigen,regX, regY,ranConfia);
    else if (modelo == fisher)
        Reconocedor = createFisherFaceRecognizer(Neigen,ranConfia);
    else
        Reconocedor = createEigenFaceRecognizer(Neigen,ranConfia);
}

void ReconocerdordePersona::obtenerParametrosDirectorio(const string &ruta,
                                                        vector<Mat> &imagenes, vector<int> &etiquetas)
{
    manejoArchivos adminArchivos;
    std::vector<std::string> listaCarpetas;

    listaCarpetas = adminArchivos.listarCarpeta(ruta);

    if (listaCarpetas.empty())
        return;

    std::string rutaImagenes = ruta;

    if( ruta.compare(ruta.length()-1, 1, "/") != 0 )
        rutaImagenes.append("/");

    std::cout << "Lista de carpetas: "<< std::endl;

    for(std::vector<std::string>::const_iterator i = listaCarpetas.begin();
        i != listaCarpetas.end(); i++)
    {
        std::cout << *i << std::endl;

        std::vector<std::string> listaImagenes;

        listaImagenes = adminArchivos.listarArchivos(rutaImagenes + *i);

        if(listaImagenes.empty())
            continue;

        for(std::vector<std::string>::const_iterator j = listaImagenes.begin();
            j != listaImagenes.end(); j++)
        {
            std::cout << rutaImagenes << *i << "/" << *j << std::endl;

            imagenes.push_back( imread(rutaImagenes +  *i + "/" + *j, 0) );
            etiquetas.push_back( std::stoi(i->substr(1)) );
        }
    }
}
