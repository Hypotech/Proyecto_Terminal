#ifndef AGREGAR_USUARIO_H
#define AGREGAR_USUARIO_H

#include "agregar_usur_foto.h"
#include <QDialog>

namespace Ui {
class Agregar_Usuario;
}

class Agregar_Usuario : public QDialog
{
    Q_OBJECT

public:
    explicit Agregar_Usuario(QWidget *parent = 0);
    ~Agregar_Usuario();

private slots:
    void on_Btn_Continuar_clicked();

    void on_Btn_Cancelar_clicked();

private:
    Ui::Agregar_Usuario *ui;
};

#endif // AGREGAR_USUARIO_H
