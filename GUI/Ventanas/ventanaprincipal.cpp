#include "ventanaprincipal.h"
#include "ui_ventanaprincipal.h"

#define ESCALA_TEXTO 2 //Tamanyo o escala del texto

VentanaPrincipal::VentanaPrincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VentanaPrincipal),
    validorIP("((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\\.)){3}((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)($))")
{
    ui->setupUi(this);
    this->setFixedSize(800,600);
    ui->Btn_Desconectar->hide();
    ui->statusbar->showMessage("Desconectado");

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
        MnsjNoParametros.setText("Error: Archivo Parametros.conf no encontrado");
    }

    ReconocedorServidor = new ReconocerdordePersona(Neigen,rangoConfianza,
                                                    radio,regX,regY,vecinos,modelo);
}

VentanaPrincipal::~VentanaPrincipal()
{
    delete ui;
    delete camLap;
    delete Timer;
    delete ReconocedorServidor;
}

bool VentanaPrincipal::LeerArchivoConfig()
{
    QFile archivo ("config");
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
    else
        return false;

    for(int i=0; i < parametros.length(); i++){
        if(parametros[i] == "minimoDetectar")
            minRostro = valores[i].toInt();
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
        else if(parametros[i] == "vecinos")
            vecinos = valores[i].toInt();
        else if(parametros[i] == "modelo"){
            if (valores[i] == "eigen")
                modelo = eigen;
            else if (valores[i] == "fisher")
                modelo = fisher;
            else if (valores[i] == "LBPH")
                modelo = LBPH;
        }
    }
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
    DectorRostrosServidor.EscanearImagen(FrameRecibido, MarcosRostrosDetec,minRostro);

    cv::Mat ImagenRostro;

    for (vector<Rect>::const_iterator Rostro = MarcosRostrosDetec.begin() ; Rostro != MarcosRostrosDetec.end() ; Rostro++){

        rectangle(FrameRecibido, *Rostro, cv::Scalar(255,255,255)); //se dibuja un rectangulo que enmarca el rostro detectado
        ImagenRostro = FrameRecibido(*Rostro); //recortamos solo el rostro y lo almacenamos
        cv::Mat RostroNormalizado(anchImag, altImag ,ImagenRostro.type() ); //aqui se almacenara el rostro ya normalizada en tamanyo

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
    ver_raspicam ventanaVerRaspicam(this);

    connect( this,SIGNAL( RaspicamLista(cv::Mat&) ),&ventanaVerRaspicam,SLOT( DesplegarFrame(cv::Mat&) ) );
    connect(&ventanaVerRaspicam,SIGNAL(abrirPuerta(), this,SLOT(SL_abrirPuerta());
    ventanaVerRaspicam.setModal(false);
    this->hide();
    ventanaVerRaspicam.exec();
    this->setVisible(true);
}

void VentanaPrincipal::on_Btn_Agregar_Usr_clicked()
{
    Agregar_Usuario ventanaAgregarUsuario;
    ventanaAgregarUsuario.setModal(false);
    this->hide();

    if(ventanaAgregarUsuario.exec() == QDialog::Accepted){
        agregar_usur_Foto ventanaAgregarUsrFoto(this);
        ventanaAgregarUsrFoto.setModal(false);

        connect( this,SIGNAL( RaspicamLista(cv::Mat&) ),&ventanaAgregarUsrFoto, SIGNAL(RaspicamLista(cv::Mat&) ) );
        connect( this,SIGNAL( RostroDectado(cv::Mat&) ), &ventanaAgregarUsrFoto, SIGNAL( RaspicamRostroDectado(cv::Mat&) ) );
        connect( this,SIGNAL( RostroNODectado() ), &ventanaAgregarUsrFoto, SIGNAL( RaspicamRostroNODectado() ) );

        if(ventanaAgregarUsrFoto.exec() == QDialog::Accepted){
            //Ventana de procesado
        }
    }

    this->setVisible(true);

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
        ui->statusbar->showMessage( "Conectado a " + ui->LEditIP->text() );
        ui->LEditIP->setEnabled(false);

        ui->Btn_Agregar_Usr->setEnabled(true);
        ui->Btn_Cofiguracion->setEnabled(true);
        ui->Btn_Quitar_Usr->setEnabled(true);
        ui->Btn_VerRaspcam->setEnabled(true);
    }
}

void VentanaPrincipal::on_LEditIP_textEdited(const QString &str) //habilita el boton de conectar
{
    if( validorIP.exactMatch(str) )
        ui->Btn_Conectar->setEnabled(true);
    else
        ui->Btn_Conectar->setEnabled(false);
}

void VentanaPrincipal::on_Btn_Desconectar_clicked()
{
    Timer->stop(); /* Se detiene el Timer */
    Servidor.cerrarConexion();
    disconnect( this, SIGNAL( FrameListo() ),this,SLOT( SL_ReconocerRostros() ) );
    ui->Btn_Desconectar->hide();
    ui->Btn_Conectar->setVisible(true);
    ui->LEditIP->setEnabled(true);
    ui->statusbar->showMessage("Desconectado");

    ui->Btn_Agregar_Usr->setEnabled(false);
    ui->Btn_Cofiguracion->setEnabled(false);
    ui->Btn_Quitar_Usr->setEnabled(false);
    ui->Btn_VerRaspcam->setEnabled(false);
}

void VentanaPrincipal::on_Btn_Cofiguracion_clicked()
{
    ventana_config ventanaConfig ( this, modelo, minRostro);
    ventanaConfig.setModal(false);
    this->setVisible(false);
    ventanaConfig.exec();
    this->setVisible(true);
}

void VentanaPrincipal::on_Btn_Quitar_Usr_clicked()
{
    ventanaQuitar ventanaQuit;
    ventanaQuit.setModal(false);
    this->setVisible(false);
    ventanaQuit.exec();
    this->setVisible(false);
}

void VentanaPrincipal::on_LEditIP_returnPressed()
{
    if( ui->Btn_Conectar->isEnabled() )
        on_Btn_Conectar_clicked();
}

void VentanaPrincipal::SL_abrirPuerta()
{
    Servidor.MandarApertura(SI);
}
