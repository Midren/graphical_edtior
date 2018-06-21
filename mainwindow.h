#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCoreApplication>
#include <QMainWindow>
#include <QGraphicsView>
#include <QAction>
#include <QActionGroup>
#include <QToolBar>
#include <QColorDialog>
#include <QColor>
#include <QComboBox>
#include <QLineEdit>
#include <QString>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QFileDialog>
#include <QSvgGenerator>
#include <QGraphicsSvgItem>

#include "newscenedialog.h"
#include "scene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
public slots:
    void drawingActionGroupClicked(QAction*);
    void penColorActionClicked();
    void penStyleComboBoxChanged(int);
    void penCapComboBoxChanged(int);
    void penJoinStyleComboBoxChanged(int);
    void penWidthChanged(QString);
    void createNewScene();
    void saveFile();
    void openFile();
    void exit();
private:
    QGraphicsView* view;
    Scene* scene;
    QString path;

    void createActions();
    void createConnections();
    void createToolBars();
    void createMenuBar();
    void createNewScene(int, int);

    QAction* newFileAction;
    QAction* openFileAction;
    QAction* saveFileAction;
    QAction* exitAction;

    QAction* controlAction;
    QAction* selectAction;
    QAction* lineAction;
    QAction* rectAction;
    QAction* ellipseAction;
    QAction* textAction;

    QAction* penColorAction;

    QComboBox* penStyle;
    QComboBox* penCap;
    QComboBox* penJoinStyle;
    QLineEdit* penWidth;

    QActionGroup *drawingActionGroup;

    QToolBar* penStyleToolBar;
    QToolBar* drawingToolBar;

    QMenuBar* menuBar;
};

#endif // MAINWINDOW_H
