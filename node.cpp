#include "node.h"

Node::Node(int x, int y, int nodeSize, QPoint point, int size): GridObject(x, y),_connectionHandler(nodeSize)
{
    this->_pos = point;
    this->setRect(_pos.x(), _pos.y(), size, size);
    this->_rectBottom = new QGraphicsRectItem(_pos.x(), _pos.y() + (size/2), size, size/2 + 1);
    this->_rectTop = new QGraphicsRectItem(_pos.x(), _pos.y(), size, size/2);
    this->_rectLeft = new QGraphicsRectItem(_pos.x(), _pos.y(), size/2, size);
    this->_rectRight = new QGraphicsRectItem(_pos.x() + (size/2), _pos.y(), size/2 + 1, size);
    QBrush brush(Qt::transparent);
    this->setBrush(brush);
    QPen pen(Qt::transparent);
    this->setPen(pen);
    this->_rectBottom->setPen(pen);
    this->_rectBottom->setBrush(brush);
    this->_rectBottom->setZValue(-1);
    this->_rectTop->setPen(pen);
    this->_rectTop->setBrush(brush);
    this->_rectTop->setZValue(-1);
    this->_rectLeft->setPen(pen);
    this->_rectLeft->setBrush(brush);
    this->_rectLeft->setZValue(-1);
    this->_rectRight->setPen(pen);
    this->_rectRight->setBrush(brush);
    this->_rectRight->setZValue(-1);
    this->_nodeCircle = new QGraphicsEllipseItem(_pos.x(), _pos.y(), size, size);
    QGraphicsDropShadowEffect * dropShadow = new QGraphicsDropShadowEffect();
    dropShadow->setBlurRadius(12);
    dropShadow->setColor(QColor(0,0,0,200));
    dropShadow->setOffset(QPoint(2,2));
    this->_nodeCircle->setGraphicsEffect(dropShadow);
    QString nodeString = QString::number(nodeSize);
    QFont serifFont("Gill Sans MT", 38);
    switch(size)
    {
    case 66: //9x9
        serifFont.setPixelSize(38);
        break;
    case 46: //13x13
        serifFont.setPixelSize(32);
        break;
    case 35: //17x17
        serifFont.setPixelSize(26);
        break;
    }
    this->_nodeText = new QGraphicsTextItem(nodeString);
    this->_nodeText->setDefaultTextColor(Qt::white);
    this->_nodeText->setFont(serifFont);
    switch(size)
    {
    case 66: //9x9
        this->_nodeText->setPos(point.x() + (23 - this->_nodeText->boundingRect().width()/8)  , point.y() + 2);;
        break;
    case 46: //13x13
        this->_nodeText->setPos(point.x() + (14 - this->_nodeText->boundingRect().width()/8)  , point.y() - 5);;
        break;
    case 35: //17x17
        this->_nodeText->setPos(point.x() + (10 - this->_nodeText->boundingRect().width()/8)  , point.y() - 6);;
        break;
    }
    this->_nodeText->setAcceptHoverEvents(false);
    this->_itemGroup = new QGraphicsItemGroup(this);
    this->_itemGroup->addToGroup(this->_rectTop);
    this->_itemGroup->addToGroup(this->_rectBottom);
    this->_itemGroup->addToGroup(this->_rectLeft);
    this->_itemGroup->addToGroup(this->_rectRight);
    this->_itemGroup->addToGroup(this->_nodeCircle);
    this->_itemGroup->addToGroup(this->_nodeText);
    updateColor();
    this->_nodeCircle->setPen(pen);
}

void Node::connectNodes(Direction side, Node *nodePtr)
{
    this->_connectionHandler.selectConnection(side)->connect(nodePtr);
}

void Node::addBridge(Direction side)
{
    if(this->_connectionHandler.selectConnection(side)->getAmountOfConnects() < 2)
    {
        this->_connectionHandler.selectConnection(side)->addBridge();
        this->_connectionHandler.setConnections(this->_connectionHandler.getConnections() + 1);
        this->updateColor();
    }
}

void Node::removeBridge(Direction side)
{
    if(this->_connectionHandler.selectConnection(side)->getAmountOfConnects() > 0)
    {
        this->_connectionHandler.selectConnection(side)->removeBridge();
        this->_connectionHandler.setConnections(this->_connectionHandler.getConnections() - 1);
        this->updateColor();
    }
}

int Node::getRemaining()
{
    return this->_connectionHandler.getRemaining();
}

std::map<Direction, int> Node::getSlotMap()
{
    return this->_connectionHandler.getSlotMap();
}

bool Node::isFull()
{
    if(this->_connectionHandler.getRemaining()==0) return true;
    return false;
}

//Return a vector with all GridObjects that can have a line connected to the node
std::map<Direction, std::vector<GridObject *> > Node::getAllPotentialLines(std::vector<std::vector<GridObject *> > *board)
{
    std::map<Direction, std::vector<GridObject *>> potentialLines = {
        {Direction::top,this->getPotentialLinesDir(Direction::top,board)},
        {Direction::right,this->getPotentialLinesDir(Direction::right,board)},
        {Direction::bottom,this->getPotentialLinesDir(Direction::bottom,board)},
        {Direction::left,this->getPotentialLinesDir(Direction::left,board)},
    };
    return potentialLines;
}

bool Node::isHovered()
{
    return this->_nodeCircle->isUnderMouse();
}

//Return nodes that can be connected to the node
std::map<Direction, Node *> Node::getConnectedNodes()
{
    std::map<Direction, Node *> nodeMap = {
        {Direction::top,this->_connectionHandler.selectConnection(Direction::top)->getTarget()},
        {Direction::right,this->_connectionHandler.selectConnection(Direction::right)->getTarget()},
        {Direction::bottom,this->_connectionHandler.selectConnection(Direction::bottom)->getTarget()},
        {Direction::left,this->_connectionHandler.selectConnection(Direction::left)->getTarget()}
    };
    return nodeMap;
}

//Update color based on amount of bridges needed for a node
void Node::updateColor()
{
    QColor mycolor;
    int connectionLeft = this->_connectionHandler.getRemaining();
    int H = 232;
    int S = int((14 + (8*connectionLeft)) * 2.55);
    int L = int((77 - (6.875*connectionLeft)) * 2.55);
    mycolor.setHsl(H,S,L);
    if(connectionLeft == 0)
    {
        mycolor.setHsl(123, int(60*2.55), int(62*2.55));
    }
    else if(connectionLeft < 0 || connectionLeft > (this->_connectionHandler.getRemaining() + this->_connectionHandler.getConnections()))
    {
        //debugging for wrong value
        mycolor.setRgb(255,0,0);
    }
    setEllipseColor(mycolor);
}

void Node::setEllipseColor(QColor color)
{
    this->_nodeCircle->setBrush(QBrush(color));
}

void Node::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    emit mouseEnter(this);
}

void Node::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    emit mouseLeave(this);
}

//Emit a signal to GameLogic class to highlight all nodes that form one continuous connection with the node
void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        emit mouseClicked(this);
    }
}

//Emit a signal to GameLogic to stop highlighting nodes
void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit mouseReleased(this);
}

//Fills the sides of the background of the node to show potential line positions
void Node::fillBackground(Direction dir)
{
    QBrush backColor = QBrush(QColor(229,215,135));
    switch(dir)
    {
        case Direction::top:
            this->_rectTop->setBrush(backColor);
            break;
        case Direction::right:
            this->_rectRight->setBrush(backColor);
            break;
        case Direction::bottom:
            this->_rectBottom->setBrush(backColor);
            break;
        case Direction::left:
            this->_rectLeft->setBrush(backColor);
            break;
        default:
            break;
    }
}

void Node::clearBackground()
{
    QBrush backColor = QBrush(Qt::transparent);
    this->_rectTop->setBrush(backColor);
    this->_rectRight->setBrush(backColor);
    this->_rectBottom->setBrush(backColor);
    this->_rectLeft->setBrush(backColor);
}

Node::~Node()
{
    delete _nodeCircle;
    delete _nodeText;
}

//Returns the potential lines in the specified direction from the node
std::vector<GridObject *> Node::getPotentialLinesDir(Direction direction, std::vector<std::vector<GridObject *> > *board)
{
    std::vector<GridObject *> potentialLines;

    switch(direction)
    {
        case Direction::right:
            if(this->_connectionHandler.selectConnection(Direction::right)->getTarget() != nullptr)
            {
                for(int i = this->getXPos() + 1 ;i < this->_connectionHandler.selectConnection(Direction::right)->getTarget()->getXPos();i++)
                {
                    potentialLines.push_back(board[0][i][this->getYPos()]);
                }
            }
            break;

        case Direction::left:
            if(this->_connectionHandler.selectConnection(Direction::left)->getTarget() != nullptr)
            {
                for(int i = this->getXPos() - 1 ;i > this->_connectionHandler.selectConnection(Direction::left)->getTarget()->getXPos();i--)
                {
                    potentialLines.push_back(board[0][i][this->getYPos()]);
                }
            }
            break;


        case Direction::bottom:
            if(this->_connectionHandler.selectConnection(Direction::bottom)->getTarget() != nullptr)
            {
                for(int i = this->getYPos() + 1 ;i < this->_connectionHandler.selectConnection(Direction::bottom)->getTarget()->getYPos(); i++)
                {
                    potentialLines.push_back(board[0][this->getXPos()][i]);
                }
            }
            break;

        case Direction::top:
            if(this->_connectionHandler.selectConnection(Direction::top)->getTarget() != nullptr)
            {
                for(int i = this->getYPos() - 1 ;i > this->_connectionHandler.selectConnection(Direction::top)->getTarget()->getYPos(); i--)
                {
                    potentialLines.push_back(board[0][this->getXPos()][i]);
                }
            }
            break;
    }
    return potentialLines;
}
