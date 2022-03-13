#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
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
    void setTexture(QString path, TextureType type);
    void btTextureEnabled(bool enabled);
    void ungrowTree();

private slots:
    void slider_valueChanged();
    void growTree();
    void changeGrowthTree(int procent);
    void loadTexWindow();

    void on_actionOpen_triggered();
    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_slSeason_valueChanged(int value);

private:
    std::vector<QSlider *> sliders;
    QString opened_file_path;
    Ui::MainWindow *ui;
    OpenGlWidget *openGlWidget;
    json j;
    void set_sliders();
    void delay(int secs);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
