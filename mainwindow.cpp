#include "mainwindow.h"

#include <iostream>

MainWindow::MainWindow()
{
    createActions();
    createConnections();
    createMenuBar();
    createToolBars();
}

void MainWindow::createActions(){
    controlAction = new QAction(tr("Control objects"), this);
    controlAction->setData(int(Scene::ControlObjects));
    controlAction->setIcon(QIcon(":/icons/cursor.png"));
    controlAction->setCheckable(true);

    lineAction = new QAction(tr("Draw line"), this);
    lineAction->setData(int(Scene::DrawLine));
    lineAction->setIcon(QIcon(":/icons/line.png"));
    lineAction->setCheckable(true);

    selectAction = new QAction(tr("Select objects"), this);
    selectAction->setData(int(Scene::SelectObject));
    selectAction->setIcon(QIcon(":/icons/selective.png"));
    selectAction->setCheckable(true);

    rectAction = new QAction(tr("Draw rectangular"), this);
    rectAction->setData(int(Scene::DrawRect));
    rectAction->setIcon(QIcon(":/icons/rect.png"));
    rectAction->setCheckable(true);

    ellipseAction = new QAction(tr("Draw ellipse"), this);
    ellipseAction->setData(int(Scene::DrawEllipse));
    ellipseAction->setIcon(QIcon(":/icons/ellipse.png"));
    ellipseAction->setCheckable(true);

    textAction = new QAction(tr("Write text"), this);
    textAction->setData(int(Scene::WriteText));
    textAction->setIcon(QIcon(":/icons/text.png"));
    textAction->setCheckable(true);

    drawingActionGroup = new QActionGroup(this);
    drawingActionGroup->setExclusive(true);
    drawingActionGroup->addAction(controlAction);
    drawingActionGroup->addAction(lineAction);
    drawingActionGroup->addAction(selectAction);
    drawingActionGroup->addAction(rectAction);
    drawingActionGroup->addAction(ellipseAction);
    drawingActionGroup->addAction(textAction);

    penColorAction = new QAction(tr("Choose Color"), this);
    penColorAction->setIcon(QIcon(":/icons/color_circle.png"));

    penStyle = new QComboBox;
    penStyle->addItem(tr("Solid Line"), 1);
    penStyle->addItem(tr("Dash Line"), 2);
    penStyle->addItem(tr("Dot Line"), 3);
    penStyle->addItem(tr("Dash Dot Line"), 4);
    penStyle->addItem(tr("Dash Dot Dot Line"), 5);

    penCap = new QComboBox;
    penCap->addItem(tr("Flat Cap"), 0x00);
    penCap->addItem(tr("Square Cap"), 0x10);
    penCap->addItem(tr("Round Cap"), 0x20);

    penJoinStyle = new QComboBox;
    penJoinStyle->addItem(tr("Miter Join"), 0x00);
    penJoinStyle->addItem(tr("Bevel Join"), 0x40);
    penJoinStyle->addItem(tr("Round Join"), 0x80);
    penJoinStyle->addItem(tr("SvgMiter Join"), 0x100);

    penWidth = new QLineEdit;
    penWidth->setValidator(new QIntValidator(1, 75, this));
    penWidth->setText(tr("3"));

    newFileAction = new QAction(tr("&New file"), this);
    openFileAction = new QAction(tr("&Open existing file"), this);
    saveFileAction = new QAction(tr("Save file"), this);
    exitAction = new QAction(tr("&Quit"), this);
}

void MainWindow::createConnections(){
    connect(drawingActionGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(drawingActionGroupClicked(QAction*)));
    connect(penColorAction, SIGNAL(triggered()),
            this, SLOT(penColorActionClicked()));
    connect(penStyle, SIGNAL(currentIndexChanged(int)),
            this, SLOT(penStyleComboBoxChanged(int)));
    connect(penCap, SIGNAL(currentIndexChanged(int)),
            this, SLOT(penCapComboBoxChanged(int)));
    connect(penJoinStyle, SIGNAL(currentIndexChanged(int)),
            this, SLOT(penJoinStyleComboBoxChanged(int)));
    connect(penWidth, SIGNAL(textChanged(QString)),
            this, SLOT(penWidthChanged(QString)));
    connect(newFileAction, SIGNAL(triggered()),
            this, SLOT(createNewScene()));
    connect(exitAction, SIGNAL(triggered()),
            this, SLOT(exit()));
    connect(saveFileAction, SIGNAL(triggered()),
            this, SLOT(saveFile()));
    connect(openFileAction, SIGNAL(triggered()),
            this, SLOT(openFile()));
}

void MainWindow::createNewScene() {
    NewSceneDialog* d = new NewSceneDialog;
    if(d->exec() == QDialog::Accepted) {
        createNewScene(d->getHeight(), d->getWidth());
    }
}

void MainWindow::createNewScene(int h, int w) {
    scene = new Scene(this);
    scene->setSceneRect(0, 0, h, w);
    view = new QGraphicsView(scene);
    view->setRenderHints(QPainter::Antialiasing);
    setCentralWidget(view);
}

void MainWindow::drawingActionGroupClicked(QAction *action){
    scene->setMode(Scene::Mode(action->data().toInt()));
}

void MainWindow::penColorActionClicked() {
    scene->pen.setColor(QColorDialog::getColor());;
}

void MainWindow::penStyleComboBoxChanged(int index) {
    scene->pen.setStyle(static_cast<Qt::PenStyle>(penStyle->itemData(index).toInt()));
}

void MainWindow::penCapComboBoxChanged(int index) {
    scene->pen.setCapStyle(static_cast<Qt::PenCapStyle>(penStyle->itemData(index).toInt()));
}

void MainWindow::penJoinStyleComboBoxChanged(int index) {
    scene->pen.setJoinStyle(static_cast<Qt::PenJoinStyle>(penStyle->itemData(index).toInt()));
}

void MainWindow::penWidthChanged(QString num) {
    scene->pen.setWidth(num.toInt());
}

void MainWindow::exit() {
    if(QMessageBox::question(this,
                                 "Exit",
                                 "Do you really want to quit?",
                                 QMessageBox::Yes | QMessageBox::Cancel,
                                 QMessageBox::Cancel) == QMessageBox::Yes)
            qApp->quit();
}

void MainWindow::createToolBars(){
    drawingToolBar = new QToolBar;
    addToolBar(Qt::LeftToolBarArea, drawingToolBar);
    drawingToolBar->addSeparator();
    drawingToolBar->addAction(controlAction);
    drawingToolBar->addAction(selectAction);
    drawingToolBar->addAction(lineAction);
    drawingToolBar->addAction(rectAction);
    drawingToolBar->addAction(ellipseAction);
    drawingToolBar->addAction(textAction);

    penStyleToolBar = new QToolBar;
    addToolBar(Qt::TopToolBarArea, penStyleToolBar);
    penStyleToolBar->addSeparator();
    penStyleToolBar->addAction(penColorAction);
    penStyleToolBar->addWidget(penStyle);
    penStyleToolBar->addWidget(penCap);
    penStyleToolBar->addWidget(penJoinStyle);
    penStyleToolBar->addWidget(penWidth);
}

void MainWindow::createMenuBar() {
    menuBar = new QMenuBar(this);
    QMenu *fileMenu = menuBar->addMenu(tr("&File"));
    fileMenu->addAction(newFileAction);
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(saveFileAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    setMenuBar(menuBar);
}

void MainWindow::saveFile() {
    QString newPath = QFileDialog::getSaveFileName(this, tr("Save svg"),
            path, tr("SVG files (*.svg)"));

    path = newPath;

    QSvgGenerator generator;
    generator.setFileName(path);
    generator.setSize(QSize(scene->width(), scene->height()));
    generator.setViewBox(QRect(0, 0, scene->width(), scene->height()));
    generator.setTitle(trUtf8("SVG Example"));
    generator.setDescription(trUtf8("File created by SVG Example"));

    QPainter painter;
    painter.begin(&generator);
    scene->render(&painter);
    painter.end();
}

void MainWindow::openFile() {
    QString path = QFileDialog::getOpenFileName(this, tr("Open svg"), QString(), tr("SVG files (*svg)"));
    QGraphicsSvgItem *item = new QGraphicsSvgItem(path);
    createNewScene(item->boundingRect().height(), item->boundingRect().width());
    scene->addItem(item);
}
