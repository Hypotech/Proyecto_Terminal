#ifndef VENTANAQUITAR_H
#define VENTANAQUITAR_H

#include <QDialog>

namespace Ui {
class ventanaQuitar;
}

class ventanaQuitar : public QDialog
{
    Q_OBJECT

public:
    explicit ventanaQuitar(QWidget *parent = 0);
    ~ventanaQuitar();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ventanaQuitar *ui;
};

#endif // VENTANAQUITAR_H
