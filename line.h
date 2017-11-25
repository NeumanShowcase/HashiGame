#pragma once

#include "gridobject.h"
#include "node.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QGraphicsItemGroup>
#include <QGraphicsSceneMouseEvent>

enum class Orientation
{
    horizontal = 0, vertical
};

class Line : public GridObject
{

    Q_OBJECT

public:

    Line(int x, int y, Orientation orientation, QPoint point, int size, Node* conn1, Node* conn2 );

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    Orientation getOrientation();

    Node *getFirstConnection();

    Node *getSecondConnection();

    bool checkIsDouble();

    void addSecondLine();

    void removeSecondLine();

    ~Line();

signals:

    void clickedLine(Line*);

    void rightClickedLine(Line*);

private:

    Node * _firstConnection;

    Node * _secondConnection;

    bool _isDouble = false;

    Orientation _orientation;

    QGraphicsRectItem *_lineRect = nullptr;

    QGraphicsRectItem *_secondLineRect = nullptr;

    int _gridSize;
};

