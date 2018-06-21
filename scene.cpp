#include "scene.h"
#include <iostream>

Scene::Scene(QObject* parent): QGraphicsScene(parent)
{
    sceneMode = NoMode;
    itemToDraw = 0;
    pen = QPen(Qt::black, 3, Qt::SolidLine);
}

void Scene::makeItemsControllable(bool areControllable){
    for(QGraphicsItem* item: items()){
        item->setFlag(QGraphicsItem::ItemIsSelectable, areControllable);
        item->setFlag(QGraphicsItem::ItemIsMovable, areControllable);
    }
}

qreal distance(QPointF p1, QPointF p2) {
    return sqrt(pow(p1.x()-p2.x(), 2) + pow(p1.y() - p2.y(), 2));
}

QPointF* nearSomeCorner(QPointF point, QGraphicsItem* item) {
    QRectF rect = item->mapRectToScene(item->boundingRect());
    if(rect.contains(point))
        return nullptr;
    QVector<QPointF> points = {rect.topLeft(), rect.topRight(),
                               rect.bottomLeft(), rect.bottomRight()};
    for(QPointF p: points) {
        std::cout << sqrt(pow(p.x()-point.x(), 2) + pow(p.y() - point.y(), 2)) << std::endl;
        if(distance(p, point) < 60) {
            std::cout << std::endl;
            return new QPointF(p);
        }
    }
    std::cout << std::endl;
    return nullptr;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(sceneMode == DrawLine || sceneMode == DrawRect ||
       sceneMode == DrawEllipse) {
        origPoint = event->scenePos();
    } else if(sceneMode == WriteText) {
        origPoint = event->scenePos();
        itemToDraw = new QGraphicsTextItem(tr(""));
        itemToDraw->setPos(origPoint);
        itemToDraw->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
//        static_cast<QGraphicsTextItem*>(itemToDraw)->setTextInteractionFlags(Qt::NoTextInteraction);
        static_cast<QGraphicsTextItem*>(itemToDraw)->setTextInteractionFlags(Qt::TextEditorInteraction);
        static_cast<QGraphicsTextItem*>(itemToDraw)->setFocus();
        addItem(itemToDraw);
    } else if(sceneMode == ControlObjects) {
        origPoint = event->scenePos();
        QGraphicsItem *item = nullptr;
        if(selectedItems().length() == 0) {
            item = itemAt(event->scenePos().x(),
                          event->scenePos().y(), QTransform());
            if(item) {
                item->setSelected(true);
            }
        }
        if(selectedItems().length() == 1) {
            item = selectedItems()[0];
        }
        if(item && nearSomeCorner(origPoint, item)) {
            setMode(RotateObjects);
            itemToDraw = item;
            auxPoint = *nearSomeCorner(origPoint, item);
        } else {
            setMode(MoveObjects);
        }
    } else if(sceneMode == RotateObjects) {
        origPoint = event->scenePos();
    } else if(sceneMode == MoveObjects) {
        origPoint = event->scenePos();
    } else {
        QGraphicsScene::mousePressEvent(event);
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(sceneMode == DrawLine){
        if(!itemToDraw) {
            itemToDraw = new QGraphicsLineItem;
            static_cast<QGraphicsLineItem*>(itemToDraw)->setPen(pen);
            itemToDraw->setPos(origPoint);
            addItem(itemToDraw);
        }
        static_cast<QGraphicsLineItem*>(itemToDraw)->setLine(0,0,
                            event->scenePos().x() - origPoint.x(),
                            event->scenePos().y() - origPoint.y());
    } else if(sceneMode == DrawRect) {
        if(!itemToDraw) {
            itemToDraw = new QGraphicsRectItem;
            static_cast<QGraphicsRectItem*>(itemToDraw)->setPen(pen);
            itemToDraw->setPos(origPoint);
            addItem(itemToDraw);
        }
        static_cast<QGraphicsRectItem*>(itemToDraw)->setRect(0,0,
                            event->scenePos().x() - origPoint.x(),
                            event->scenePos().y() - origPoint.y());
    } else if(sceneMode == DrawEllipse) {
        if(!itemToDraw) {
            itemToDraw = new QGraphicsEllipseItem;
            static_cast<QGraphicsEllipseItem*>(itemToDraw)->setPen(pen);
            itemToDraw->setPos(origPoint);
            addItem(itemToDraw);
        }
        static_cast<QGraphicsEllipseItem*>(itemToDraw)->setRect(0,0,
                            event->scenePos().x() - origPoint.x(),
                            event->scenePos().y() - origPoint.y());
    } else if(sceneMode == MoveObjects){
        for(QGraphicsItem* item: selectedItems()) {
            item->moveBy(event->scenePos().x() - origPoint.x(), event->scenePos().y() - origPoint.y());
        }
        origPoint = event->scenePos();
    } else if(sceneMode == RotateObjects) {
        qreal a = distance(auxPoint, event->scenePos());
        qreal b = distance(auxPoint, origPoint);
        qreal c = distance(event->scenePos(), origPoint);
        qreal angle = acos((a*a+b*b-c*c)/(2*a*b));
        angle *= 180/M_PI;
        //TODO: Find normal way to choose negative angle
//        qreal x;
//        if(origPoint.y() == itemToDraw->y())
//            x = event->scenePos().x();
//        else
//            x = (origPoint.x()-itemToDraw->x())*(auxPoint.y() - itemToDraw->y())/(origPoint.y()-itemToDraw->y());
//        if ( x > event->scenePos().x())
//            angle *= -1;
//        std::cout << angle << std::endl;
        itemToDraw->setTransformOriginPoint(auxPoint.x() - itemToDraw->x(), auxPoint.y() - itemToDraw->y());
        itemToDraw->setRotation(angle);
    }
    else {
        QGraphicsScene::mouseMoveEvent(event);
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(sceneMode != WriteText) {
        itemToDraw = nullptr;
    }
    if(sceneMode == SelectObject) {
//        setMode(ControlObjects);
    } else if(sceneMode == MoveObjects || sceneMode == RotateObjects) {
        setMode(ControlObjects);
    } else if(sceneMode == WriteText) {
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::setMode(Mode mode){
    sceneMode = mode;
    QGraphicsView::DragMode vMode =
            QGraphicsView::NoDrag;
    if(mode == DrawLine || mode == DrawEllipse || mode == DrawRect){
        makeItemsControllable(false);
        vMode = QGraphicsView::NoDrag;
    }
    else if(mode == SelectObject){
        makeItemsControllable(true);
        vMode = QGraphicsView::RubberBandDrag;

    } else if(mode == ControlObjects || mode == WriteText) {
        makeItemsControllable(true);
        vMode = QGraphicsView::NoDrag;
    } else if(mode == MoveObjects) {
    } else if(mode == RotateObjects) {
    }
    QGraphicsView* mView = views().at(0);
    if(mView) {
        mView->setDragMode(vMode);
    }
}

void Scene::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Delete) {
        for(QGraphicsItem* item: selectedItems()){
            removeItem(item);
            delete item;
        }
    } if(event->key() == Qt::Key_Escape) {
        if(itemToDraw) {
            itemToDraw->clearFocus();
            itemToDraw = nullptr;
        }
    } else {
        QGraphicsScene::keyPressEvent(event);
    }
}
