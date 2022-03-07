#ifndef CHANGETEXWINDOW_H
#define CHANGETEXWINDOW_H

#include <QDialog>
#include <QListWidget>
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
    changeTexWindow *obj;

    bool LoadPaths(QString path, QListWidget* widget);

private slots:
    void sendTexturePath();
    void closeDialog();
};

#endif // CHANGETEXWINDOW_H
