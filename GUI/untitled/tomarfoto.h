#ifndef TOMARFOTO_H
#define TOMARFOTO_H

#include <QMainWindow>

namespace Ui {
class TomarFoto;
}

class TomarFoto : public QMainWindow
{
    Q_OBJECT

public:
    explicit TomarFoto(QWidget *parent = 0);
    ~TomarFoto();

private:
    Ui::TomarFoto *ui;
};

#endif // TOMARFOTO_H
