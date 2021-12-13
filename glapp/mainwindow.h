#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widget.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slider_valueChanged();


private:
    Ui::MainWindow *ui;
    OpenGlWidget *openGlWidget;
};

#endif // MAINWINDOW_H
