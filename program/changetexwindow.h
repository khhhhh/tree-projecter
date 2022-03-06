#ifndef CHANGETEXWINDOW_H
#define CHANGETEXWINDOW_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class changeTexWindow;
}

class changeTexWindow : public QDialog
{
    Q_OBJECT

public:
    explicit changeTexWindow(QWidget *parent = nullptr);
    changeTexWindow(MainWindow *qMain);
    ~changeTexWindow();

private:
    Ui::changeTexWindow *ui;
    MainWindow *mainWin;
    QStringList pathLinks;

    void LoadPaths();

private slots:
    void sendTexturePath();
};

#endif // CHANGETEXWINDOW_H
