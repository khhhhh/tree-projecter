#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widget.h"
#include "json.hpp"

using json = nlohmann::json;
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

    void on_actionOpen_triggered();
    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();


private:
    QString opened_file_path;
    Ui::MainWindow *ui;
    OpenGlWidget *openGlWidget;
    json j;
    void set_sliders();
};

#endif // MAINWINDOW_H
