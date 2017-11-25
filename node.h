#pragma once

#include "gridobject.h"
#include "dirconnections.h"
#include <QFont>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsSceneMouseEvent>

class Node : public GridObject
{

    Q_OBJECT

public:

    Node(int x, int y, int nodeSize, QPoint point, int size);

    void connectNodes(Direction side,Node* nodePtr);

    void addBridge(Direction side);

    void removeBridge(Direction side);

    std::map<Direction,int> getSlotMap();

    bool isFull();

    int getRemaining();

    std::map<Direction, std::vector<GridObject *> > getAllPotentialLines(std::vector<std::vector<GridObject*>> *board);

    bool isHovered();

    std::map<Direction, Node *> getConnectedNodes();

    void updateColor();

    void setEllipseColor(QColor color);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void fillBackground(Direction dir);

    void clearBackground();

    ~Node();

signals:

    void mouseEnter(Node*);

    void mouseLeave(Node*);

    void mouseClicked(Node*);

    void mouseReleased(Node *);

private:

    std::vector<GridObject *> getPotentialLinesDir(Direction direction,std::vector<std::vector<GridObject*>> *board);

    DirConections _connectionHandler;

    QGraphicsEllipseItem *_nodeCircle;

    QGraphicsTextItem *_nodeText;

    QGraphicsRectItem *_rectTop;

    QGraphicsRectItem *_rectBottom;

    QGraphicsRectItem *_rectLeft;

    QGraphicsRectItem *_rectRight;

};
