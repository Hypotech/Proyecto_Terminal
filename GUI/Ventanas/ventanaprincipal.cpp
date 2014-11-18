#include "ventanaprincipal.h"
#include "ui_ventanaprincipal.h"

#define ESCALA_TEXTO 2 //Tamanyo o escala del texto

VentanaPrincipal::VentanaPrincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VentanaPrincipal),
    iPRegEx("((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\\.)){3}((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)($))")
{
    ui->setupUi(this);
    this->setFixedSize(800,600);
    ui->Btn_Desconectar->hide();
    ui->statusBar->showMessage("Desconectado");

    QRegExpValidator* validadorIP = new QRegExpValidator(iPRegEx,this);
    ui->LEditIP->setValidator(validadorIP);
//    ui->LEditIP->setInputMask("000.000.000.000; ");

    const int PeriodoCaptura = 1000/25;   //ms correspondientes para obtener 25 fps
    Timer = new QTimer(this);
    Timer->setInterval(PeriodoCaptura);

    //################################ Conecciones entre slots y signals ###################################//
    connect( Timer, SIGNAL(timeout()), this, SLOT(SL_ObtenerFrame() ) );
    connect( this,SIGNAL( FrameListo() ),this,SLOT( SL_ReconocerRostros() ) );
    //######################################################################################################//

    if(DectorRostrosServidor.clasificadorCargado() == false){
        QMessageBox MnsjHaarCasNoCargado;
        MnsjHaarCasNoCargado.setIcon(QMessageBox::Critical);
        MnsjHaarCasNoCargado.setText("Error: Falta el archivo o archivo dañado haarcascade.xml");
        MnsjHaarCasNoCargado.exec();
    }

    if( LeerArchivoConfig() == false){
        QMessageBox MnsjNoParametros;
        MnsjNoParametros.setIcon(QMessageBox::Critical);
        MnsjNoParametros.setText("Error: Archivo \"configuracion\" no encontrado");
        MnsjNoParametros.exec();
    }

    ReconocedorServidor = new ReconocerdordePersona("./BD", Neigen,rangoConfianza,
                                                    radio,regX,regY,modelo);
}

VentanaPrincipal::~VentanaPrincipal()
{
    delete Timer;
    delete ReconocedorServidor;
    delete ui;
}

bool VentanaPrincipal::LeerArchivoConfig()
{
    QFile archivo ("configuracion");
    QStringList parametros, valores;

    if( archivo.open(QIODevice::ReadOnly| QIODevice::Text) )
    {
        QTextStream flujoArchivo(&archivo);
        while( !flujoArchivo.atEnd() )
        {
            QString linea = flujoArchivo.readLine();
            QStringList partes = linea.split("=", QString::SkipEmptyParts);

            if (partes.length() == 2){
                parametros.push_back(partes[0]);
                valores.push_back(partes[1]);
            }
        }
    }
    else{
        archivo.close();
        return false;
    }

    for(int i=0; i < parametros.length(); i++){
        if(parametros[i] == "minimoDetectar"){
            switch (valores[i].toInt()){
            case 200:
                minRostro = ventana_config::_200;
                break;
            case 150:
                minRostro = ventana_config::_150;
                break;
            case 100:
                minRostro = ventana_config::_100;
                break;
            }
        }
        else if(parametros[i] == "NumEigen")
            Neigen = valores[i].toInt();
        else if(parametros[i] == "rangoCofianza")
            rangoConfianza = valores[i].toFloat();
        else if(parametros[i] == "radio")
            radio = valores[i].toInt();
        else if(parametros[i] == "celdaX")
            regX = valores[i].toInt();
        else if(parametros[i] == "celdaY")
            regY = valores[i].toInt();
//        else if(parametros[i] == "vecinos")
//            vecinos = valores[i].toInt();
        else if(parametros[i] == "modelo"){
            if (valores[i] == "eigen")
                modelo = ReconocerdordePersona::eigen;
            else if (valores[i] == "fisher")
                modelo = ReconocerdordePersona::fisher;
            else if (valores[i] == "LBPH")
                modelo = ReconocerdordePersona::LBPH;
        }
    }

    archivo.close();
    return true;
}

void VentanaPrincipal::SL_ObtenerFrame()
{
    if ( Servidor.Recibir(FrameRecibido) == true)
        emit FrameListo();
    else{
        on_Btn_Desconectar_clicked();
        QMessageBox::warning(this,"Conexión Perdida",
                             "Se ha perdido la conexión con la Raspberry");
    }

}

void VentanaPrincipal::SL_ReconocerRostros()
{
    vector<Rect> MarcosRostrosDetec;
    int Respuesta = NO;

    int tamRostroDeteccion;

    switch (minRostro ) {
    case ventana_config::_100:
        tamRostroDeteccion = 100;
        break;
    case ventana_config::_150:
        tamRostroDeteccion = 150;
        break;
      default:
        tamRostroDeteccion = 200;
        break;
    }

    DectorRostrosServidor.EscanearImagen(FrameRecibido, MarcosRostrosDetec,tamRostroDeteccion);

    cv::Mat ImagenRostro;

    for (vector<Rect>::const_iterator Rostro = MarcosRostrosDetec.begin() ;
         Rostro != MarcosRostrosDetec.end() ; Rostro++){

        rectangle(FrameRecibido, *Rostro, cv::Scalar(255,255,255)); //se dibuja un rectangulo que enmarca el rostro detectado
        ImagenRostro = FrameRecibido(*Rostro); //recortamos solo el rostro y lo almacenamos
        cv::Mat RostroNormalizado(300, 300 ,ImagenRostro.type() ); //aqui se almacenara el rostro ya normalizada en tamanyo

        double confianza;
        cv::resize(ImagenRostro, RostroNormalizado, RostroNormalizado.size(), 0, 0, INTER_LINEAR); //normalizamos la imagen en tamanyo
        int IDpersona = ReconocedorServidor->consulatarBD(RostroNormalizado,confianza); //se busca en la BD el rostro

        Point P(Rostro->x, Rostro->y); //obtenemos la coordenada inicial del rectangulo que enmarca el rostro (esquina superior izquierda)

        if(IDpersona == -1 || confianza < RangoConfianza){
            putText(FrameRecibido,"Rostro desconocido" ,P ,FONT_HERSHEY_PLAIN, ESCALA_TEXTO, cv::Scalar(255,255,255));
            Respuesta = NO;
        }
        else{
            putText(FrameRecibido,format("Subjeto ID %d", IDpersona),P ,FONT_HERSHEY_PLAIN, ESCALA_TEXTO, cv::Scalar(255,255,255));
            Respuesta = SI;
            std::cout<< "Rostro encontrado, persona con ID: " << IDpersona << std::endl;
        }
    }
    Servidor.MandarApertura(Respuesta); //mandamos a la raspberry apertura si o no

    emit RaspicamLista(FrameRecibido);

    if(MarcosRostrosDetec.empty())
        emit RostroNODectado();
    else
        emit RostroDectado(ImagenRostro);
}

void VentanaPrincipal::on_Menu_actionSalir_triggered()
{
    qApp->quit(); //quitamos app
}

void VentanaPrincipal::on_Btn_VerRaspcam_clicked()
{
    ver_raspicam ventanaVerRaspicam;

    connect( this,SIGNAL( RaspicamLista(const cv::Mat&) ),&ventanaVerRaspicam,SLOT( DesplegarFrame(const cv::Mat&) ) );
    connect( &ventanaVerRaspicam,SIGNAL(abrirPuerta()), this,SLOT(SL_abrirPuerta()) );
    this->setEnabled(false);
    ventanaVerRaspicam.exec();
    this->setEnabled(true);
}

void VentanaPrincipal::on_Btn_Agregar_Usr_clicked()
{
    Agregar_Usuario ventanaAgregarUsuario;
    this->setEnabled(false);

    if(ventanaAgregarUsuario.exec() == QDialog::Accepted){
        agregar_usur_Foto ventanaAgregarUsrFoto;
//        ventanaAgregarUsrFoto.setModal(false);

        connect( this,SIGNAL( RaspicamLista(const cv::Mat&) ),&ventanaAgregarUsrFoto,
                 SIGNAL(RaspicamLista(const cv::Mat&) ) );

        connect( this,SIGNAL( RostroDectado(cv::Mat&) ), &ventanaAgregarUsrFoto,
                 SIGNAL( RaspicamRostroDectado(cv::Mat&) ) );

        connect( this,SIGNAL( RostroNODectado() ), &ventanaAgregarUsrFoto,
                 SIGNAL( RaspicamRostroNODectado() ) );

        if (ventanaAgregarUsrFoto.exec() == QDialog::Accepted){
            agregar_usur_finalizar ventanaFinalizar(ventanaAgregarUsrFoto.fotos);

            ventanaFinalizar.exec();
        }
    }

    this->setEnabled(true);

}


void VentanaPrincipal::on_Btn_Conectar_clicked()
{
    QByteArray IP =  ui->LEditIP->text().toStdString().c_str();

    if ( Servidor.conectar( IP.constData() ) == false ) { // Si no está vacía

        QMessageBox::warning(this,"Error de conexión",
                             "No se puede conectar con la Raspberry. Revise que haya escrito correctamente la IP");
    }
    else{
        Timer->start(); /*empieza a correr el timer*/
        ui->Btn_Conectar->hide(); //oculta el boton de conectar
        ui->Btn_Desconectar->setVisible(true);
        ui->statusBar->showMessage( "Conectado a " + ui->LEditIP->text() );
        ui->LEditIP->setEnabled(false);

        ui->Btn_Agregar_Usr->setEnabled(true);
        ui->Btn_Cofiguracion->setEnabled(true);
        ui->Btn_Quitar_Usr->setEnabled(true);
        ui->Btn_VerRaspcam->setEnabled(true);
    }
}

void VentanaPrincipal::on_LEditIP_textEdited(const QString &str) //habilita el boton de conectar
{
    if( ui->LEditIP->hasAcceptableInput() )
        ui->Btn_Conectar->setEnabled(true);
    else
        ui->Btn_Conectar->setEnabled(false);
}

void VentanaPrincipal::on_Btn_Desconectar_clicked()
{
    Timer->stop(); /* Se detiene el Timer */
    Servidor.cerrarConexion();
//    disconnect( this, SIGNAL( FrameListo() ),this,SLOT( SL_ReconocerRostros() ) );
    ui->Btn_Desconectar->hide();
    ui->Btn_Conectar->setVisible(true);
    ui->LEditIP->setEnabled(true);
    ui->statusBar->showMessage("Desconectado");

    ui->Btn_Agregar_Usr->setEnabled(false);
    ui->Btn_Cofiguracion->setEnabled(false);
    ui->Btn_Quitar_Usr->setEnabled(false);
    ui->Btn_VerRaspcam->setEnabled(false);
}

void VentanaPrincipal::on_Btn_Cofiguracion_clicked()
{
    ventana_config ventanaConfig (modelo, minRostro);
    this->setEnabled(false);
    ventanaConfig.exec();
    this->setEnabled(true);
}

void VentanaPrincipal::on_Btn_Quitar_Usr_clicked()
{
    ventanaQuitar ventanaQuit;
    this->setEnabled(false);
    ventanaQuit.exec();
    this->setEnabled(true);
}

void VentanaPrincipal::on_LEditIP_returnPressed()
{
        on_Btn_Conectar_clicked();
}

void VentanaPrincipal::SL_abrirPuerta()
{
    int respuesta = SI;
    Servidor.MandarApertura(respuesta);
}
