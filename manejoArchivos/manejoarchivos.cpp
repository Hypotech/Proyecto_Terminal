#include "manejoarchivos.hpp"

manejoArchivos::manejoArchivos()
{
}

std::vector<std::string> manejoArchivos::listarCarpeta(const std::string &ruta)
{
    DIR* Directorio;
    struct dirent* InfoArchivo;
    struct stat InfoNodo;
    std::vector<std::string> ListaCarpetas;

    if( ( Directorio = opendir( ruta.c_str() ) ) == NULL ){
        perror("No se pudo abrir el directorio");
        return std::vector<std::string>();
    }

    std::string Ruta_a_Archivo;

    while((InfoArchivo = readdir(Directorio)) != NULL){ // Leemos la info de los elementos contenidos en el directorio

        if ( strcmp (InfoArchivo->d_name, ".") == 0 || strcmp(InfoArchivo->d_name, "..") == 0 ) //omitimos . y ..
            continue;

        Ruta_a_Archivo.assign(ruta); //agregamos el ruta del dir. actual al string

        if( Ruta_a_Archivo.compare(Ruta_a_Archivo.length()-1, 1, "/") != 0 )
            Ruta_a_Archivo.append("/"); //si no tiene un "/" al final se le agrega

        Ruta_a_Archivo.append(InfoArchivo->d_name); //ruta completa al archivo en el string
        int StatusNodo = stat(Ruta_a_Archivo.c_str(), &InfoNodo);

        if( StatusNodo != -1 && (InfoNodo.st_mode & S_IFMT) == S_IFDIR ){ //Si no fallo llamada a stat y sea un directorio
            ListaCarpetas.push_back(InfoArchivo->d_name);
        }
    }

    closedir(Directorio);
    std::sort( ListaCarpetas.begin(), ListaCarpetas.end() );
    return ListaCarpetas;
}

std::vector<std::string> manejoArchivos::listarArchivos(const std::string &ruta)
{
    DIR* Directorio;
    struct dirent* InfoArchivo;
    struct stat InfoNodo;
    std::vector<std::string> ListaArchivos;

    if( ( Directorio = opendir( ruta.c_str() ) ) == NULL ){
        perror("No se pudo abrir el directorio");
        return std::vector<std::string>();
    }

    std::string Ruta_a_Archivo;

    while((InfoArchivo = readdir(Directorio)) != NULL){ // Leemos la info de los elementos contenidos en el directorio

        if ( strcmp (InfoArchivo->d_name, ".") == 0 || strcmp(InfoArchivo->d_name, "..") == 0 ) //omitimos . y ..
            continue;

        Ruta_a_Archivo.assign(ruta); //agregamos la ruta del dir. actual al string

        if( Ruta_a_Archivo.compare(Ruta_a_Archivo.length()-1, 1, "/") != 0 )
            Ruta_a_Archivo.append("/"); //si no tiene un "/" al final se le agrega

        Ruta_a_Archivo.append(InfoArchivo->d_name); //ruta completa al archivo en el string
        int StatusNodo = stat(Ruta_a_Archivo.c_str(), &InfoNodo);

        if( StatusNodo != -1 && (InfoNodo.st_mode & S_IFMT) == S_IFREG ){ //Si no fallo llamada a stat y sea un archivo
            ListaArchivos.push_back(InfoArchivo->d_name);
        }
    }

    closedir(Directorio);
    std::sort( ListaArchivos.begin(), ListaArchivos.end() );
    return ListaArchivos;
}

bool manejoArchivos::crearCarpeta(const std::string &ruta)
{
    if (mkdir( ruta.c_str(), S_IRWXU) == 0)
        return true;

    perror("No se creo Carpeta");
    return false;
}

bool manejoArchivos::quitarCarpeta(const std::string &ruta)
{
    DIR* Directorio;
    struct dirent* InfoArchivo;

    if( ( Directorio = opendir( ruta.c_str() ) ) == NULL ){
        perror("No se pudo abrir el directorio");
        return false;
    }

    std::string Ruta_a_Archivo;
    struct stat InfoNodo;
    int estado = true;

    while((InfoArchivo = readdir(Directorio)) != NULL && estado){ // primero elimina el contenido del directorio

        if ( strcmp (InfoArchivo->d_name, ".") == 0 || strcmp(InfoArchivo->d_name, "..") == 0 ) //omitimos . y ..
            continue;

        Ruta_a_Archivo.assign(ruta); //agregamos el ruta del dir. actual al string

        if( Ruta_a_Archivo.compare(Ruta_a_Archivo.length()-1, 1, "/") != 0 )
            Ruta_a_Archivo.append("/"); //si no tiene un "/" al final se le agrega

        Ruta_a_Archivo.append(InfoArchivo->d_name); //ruta completa al archivo en el string

        if (stat(Ruta_a_Archivo.c_str(), &InfoNodo) != -1) //mientras podemos aceder a su info
        {
           if ( S_ISDIR(InfoNodo.st_mode & S_IFMT) == S_IFDIR )
              estado = quitarCarpeta(Ruta_a_Archivo.c_str()); //si es un directorio, se hace una llamada recursiva
           else
              estado = remove(Ruta_a_Archivo.c_str()) == 0 ? true : false; //si es un archivo, simplemente se elimina
        }
        else
            estado = false;
    }

    if(estado){
        estado = remove(ruta.c_str()) == 0 ? true : false; //luego se elimina la carpeta una vez vacia
        if( !estado ) //hubo un problema
            perror("No se pudo eliminar directorio");
    }
    else
        perror("Imposible eliminar archivos internos");

    closedir(Directorio);
    return estado;
}
