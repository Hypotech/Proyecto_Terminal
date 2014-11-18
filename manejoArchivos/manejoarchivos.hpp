#ifndef MANEJOARCHIVOS_HPP
#define MANEJOARCHIVOS_HPP

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

#include <ostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>

class manejoArchivos
{
public:
    manejoArchivos();
    static std::vector<std::string> listarCarpeta(const std::string& ruta);
    static std::vector<std::string> listarArchivos(const std::string& ruta);
    static bool crearCarpeta(const std::string& ruta);
    static bool quitarCarpeta(const std::string& ruta);
};

#endif // MANEJOARCHIVOS_HPP
