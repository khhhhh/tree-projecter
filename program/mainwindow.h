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

private slots:
    void slider_valueChanged();
    void growTree();
    void changeGrowthTree(int procent);
    void loadTexWindow();

    void on_actionOpen_triggered();
    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_slSeason_valueChanged(int value);

    void on_listWidget_currentRowChanged(int currentRow);
    void deleteTree();

    void on_actionOpen_building_texture_2_triggered();

private:
    std::vector<QSlider *> sliders;
    std::vector<Tree *> trees;
    QString opened_file_path;
    Ui::MainWindow *ui;
    OpenGlWidget *openGlWidget;
    json j;
    void set_sliders();
    void delay(int secs);
    void setPropsAtIndex(int index, json j);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    //void mousePressEvent(QMouseEvent* event);
};

#endif // MAINWINDOW_H
