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
    QStringList woodPaths;
    QStringList twigPaths;

    bool LoadPaths(QString path, QListWidget* widget, QStringList& filepaths);

private slots:
    void sendTexturePath();
    void closeDialog();
};

#endif // CHANGETEXWINDOW_H
