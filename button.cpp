#include "button.h"

Button::Button(int width, int height, QString btnText)
{
    QBrush brush(Qt::gray);
    this->setBrush(brush);
    _width = width;
    _height = height;
    setRect(0, 0, _width, _height);
    _btnText = new QGraphicsTextItem(btnText, this);
    int xTextPos = rect().width()/2 - _btnText->boundingRect().width() / 2;
    int yTextPos = rect().height()/2 - _btnText->boundingRect().height() / 2;
    _btnText->setPos(xTextPos, yTextPos);
    setAcceptHoverEvents(true);
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //QBrush brush(Qt::red);
    //this->setBrush(brush);
    emit clicked();
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{

}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{

}

Button::~Button()
{
    delete _btnText;
}
