#ifndef VENTANA_CONFIG_H
#define VENTANA_CONFIG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class ventana_config;
}

class ventana_config : public QDialog
{
    Q_OBJECT

public:
    enum metodo {eigen,fisher, LBPH};
    enum tamanyo {_200,_150,_100};
    explicit ventana_config( QWidget *parent = 0 ,metodo metdo= LBPH, tamanyo tam = _200);
    ~ventana_config();

private slots:
    void on_Btn_Cancelar_clicked();
    void on_Btn_Aceptar_clicked();

private:
    Ui::ventana_config *ui;
};

#endif // VENTANA_CONFIG_H
