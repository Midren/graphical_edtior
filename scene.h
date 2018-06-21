#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsItem>

#include <QPen>
#include <QKeyEvent>
#include <QTransform>
#include <QRectF>
#include <QVector>
#include <QPointF>

#include <math.h>

#ifndef SCENE_H
#define SCENE_H


class Scene : public QGraphicsScene
{
public:
    enum Mode {NoMode, SelectObject,
               DrawLine, DrawRect, DrawEllipse, WriteText,
               ControlObjects, MoveObjects, RotateObjects};
    Scene(QObject* parent = 0);
    QPen pen;
    void setMode(Mode mode);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    Mode sceneMode;
    QPointF origPoint;
    QPointF auxPoint;
    QGraphicsItem* itemToDraw;
    void makeItemsControllable(bool areControllable);
};

#endif // SCENE_H
