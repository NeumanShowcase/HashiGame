#pragma once

#include <QGraphicsItem>
#include <QObject>
#include <QBrush>
#include <QPen>
#include <QPoint>
#include <QGraphicsTextItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>

class Button: public QObject, public QGraphicsRectItem
{

    Q_OBJECT

public:

    Button(int width, int height, QString btnText);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    ~Button();

signals:

    void clicked();

protected:

    QPoint _pos;

    int _width;

    int _height;

    QGraphicsTextItem *_btnText;

};
