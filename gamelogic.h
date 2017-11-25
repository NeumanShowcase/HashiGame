#pragma once

#include <vector>
#include "gridobject.h"
#include "line.h"
#include "node.h"
#include "empty.h"
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QGraphicsScene>
#include <QObject>
#include <QCursor>
#include <QApplication>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPushButton>
#include <QGraphicsProxyWidget>

#define LINE -1
#define EMPTY 0

using namespace std;
class GameLogic : public QObject
{

    Q_OBJECT

public:

    GameLogic(int size, QGraphicsScene* aScene);

    void addNodeToGameObjects(void);

    void loadGameBoardFromFile(QString pathToBoard);

    void loadLevel(QByteArray infoFromFile);

    void addGameGraphics(QGraphicsScene* aScene);

    ~GameLogic();

public slots:

    void endGame();

    void endGameButtonClicked();

    void clickedLine(Line *line);

    void rightClickedLine(Line *line);

    void clickedEmpty(Empty *empty);

    void enterMouseGridObj(GridObject *gridObj);

    void exitMouseGridObj(GridObject *gridObj);

    void enterMouseNode(Node *node);

    void clickedNode(Node *node);

    void releasedMouseNode(Node *node);

signals:

    void endGameButtonClickedSignal();

private:

    void connectNodes(QByteArray infoFromFile,vector<vector<GridObject*>> board);

    void updateHighlighted();

    void clearHighlighted();

    void activateDirection(Direction direction);

    void emptyToLine(Empty *empty, Node *conn1, Node *conn2);

    void linetoEmpty(Line *line);

    static bool isDirectionOrientationAligned(Direction, Orientation);

    void enterMouseGridObjLine(GridObject *gridObj);

    void updateCurrentDirection(GridObject *gridObj);

    void checkGameFinished();

    void checkNodeConnected(Node * nodeToCheck, vector<Node *> * nodesAdded);

    void finishGame();

    int _gameGridSize;

    QPoint _activeNodeCoords;

    Node * _activeNode = nullptr;

    Direction _currentDirection;

    std::map<Direction, std::vector<GridObject *> > _highLightedObjects;

    vector<vector<GridObject*> > _allGameObjects;

    QGraphicsScene *_gameScene;

    QGraphicsTextItem *_islandText;

    QPushButton *_btnReturnMenu;

    QGraphicsProxyWidget *_proxyBtnReturnMenu;

    bool _won;

    vector<vector<int>> _gridTypeIndicator; //0 = empty, positive = node, negative = line
};
