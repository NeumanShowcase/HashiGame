#pragma once

#include "gridobject.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QGraphicsItemGroup>
#include <QGraphicsSceneMouseEvent>

class Empty : public GridObject
{

    Q_OBJECT

public:

    Empty(int x, int y, QPoint point, int size);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    ~Empty();

signals:

    void clickedEmpty(Empty*);

};
