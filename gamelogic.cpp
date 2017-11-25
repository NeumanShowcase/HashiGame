#include "gamelogic.h"

GameLogic::GameLogic(int size, QGraphicsScene* aScene):_won(false), _gameGridSize(size), _gameScene(aScene)
{
    this->_islandText = new QGraphicsTextItem("All nodes must form one single connection!");
    this->_islandText->setZValue(400);
    this->_islandText->setPos(0, 250);
    this->_islandText->setFont(QFont("Gill Sans MT", 22, QFont::Bold, false));
    QGraphicsDropShadowEffect * dropShadow = new QGraphicsDropShadowEffect();
    dropShadow->setBlurRadius(12);
    dropShadow->setColor(QColor(255,255,255,200));
    dropShadow->setOffset(QPoint(2,2));
    this->_islandText->setGraphicsEffect(dropShadow);
    this->_islandText->setDefaultTextColor(Qt::red);
    this->_gameScene->addItem(_islandText);
    this->_islandText->hide();
}

//Reads data from a file and turns it into a QByteArray
void GameLogic::loadGameBoardFromFile(QString pathToBoard)
{
    QByteArray nodePositionData;
    QByteArray nodePositionDataFormatted;
    QFile boardFile(pathToBoard);

    if (!boardFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw (QString("board info file cant be opened! I looked here: "+pathToBoard));
    }
    else
    {
        while (!boardFile.atEnd())
        {
            nodePositionData.append(boardFile.readLine());
        }
        if (boardFile.atEnd())
        {
            for (int i = 0; i < nodePositionData.length(); ++i)
            {
                if (nodePositionData[i] != '\n')
                {
                    nodePositionDataFormatted.append(nodePositionData[i]);
                }
            }
            boardFile.close();
            if(nodePositionDataFormatted.length() != (this->_gameGridSize* this->_gameGridSize))
            {
                throw (QString("Board file data is wrong/corrupt please fix"));
            }
            else
            {
                loadLevel(nodePositionDataFormatted);
                connectNodes(nodePositionDataFormatted, this->_allGameObjects);
            }
        }
    }
    _islandText->hide();
}

//Passes a QByteArray to initiate a game board with nodes and empty slots
void GameLogic::loadLevel(QByteArray infoFromFile)
{
    vector<vector<GridObject *>> tempBackwardsGameObjects;
    GridObject* pushThisToVector;
    QPoint passVectorElementIDAsPOS;
    int currentNumber;

    for (int i = 0; i < this->_gameGridSize; ++i)
    {
        int zValue;
        vector<GridObject*> temp;
        for (int j = 0; j < this->_gameGridSize; j++)
        {
            currentNumber = (infoFromFile[(j * this->_gameGridSize) + i]-'0');

            if(currentNumber == EMPTY)
            {
                switch(this->_gameGridSize)
                {
                case 9:
                    passVectorElementIDAsPOS.setX(i*(WINDOWSIZE/ this->_gameGridSize) + 1);
                    passVectorElementIDAsPOS.setY(j*(WINDOWSIZE/ this->_gameGridSize) + 1);
                    break;
                case 13:
                    passVectorElementIDAsPOS.setX(i*(WINDOWSIZE/ this->_gameGridSize) + 3);
                    passVectorElementIDAsPOS.setY(j*(WINDOWSIZE/ this->_gameGridSize) + 3);
                    break;
                case 17:
                    passVectorElementIDAsPOS.setX(i*(WINDOWSIZE/ this->_gameGridSize) + 6);
                    passVectorElementIDAsPOS.setY(j*(WINDOWSIZE/ this->_gameGridSize) + 6);
                    break;
                }

                pushThisToVector = new Empty(i, j, passVectorElementIDAsPOS, WINDOWSIZE/ this->_gameGridSize);
                connect(pushThisToVector, SIGNAL(clickedEmpty(Empty*)), this, SLOT(clickedEmpty(Empty*)));
            }
            else
            {
                switch(this->_gameGridSize)
                {
                case 9:
                    passVectorElementIDAsPOS.setX(i*(WINDOWSIZE/ this->_gameGridSize) + 1);
                    passVectorElementIDAsPOS.setY(j*(WINDOWSIZE/ this->_gameGridSize) + 1);
                    break;
                case 13:
                    passVectorElementIDAsPOS.setX(i*(WINDOWSIZE/ this->_gameGridSize) + 3);
                    passVectorElementIDAsPOS.setY(j*(WINDOWSIZE/ this->_gameGridSize) + 3);
                    break;
                case 17:
                    passVectorElementIDAsPOS.setX(i*(WINDOWSIZE/ this->_gameGridSize) + 6);
                    passVectorElementIDAsPOS.setY(j*(WINDOWSIZE/ this->_gameGridSize) + 6);
                    break;
                }
                pushThisToVector = new Node(i, j, (currentNumber), passVectorElementIDAsPOS, WINDOWSIZE/ this->_gameGridSize);
                connect(pushThisToVector, SIGNAL(mouseEnter(Node*)), this, SLOT(enterMouseNode(Node*)));
                connect(pushThisToVector, SIGNAL(mouseClicked(Node*)), this, SLOT(clickedNode(Node*)));
                connect(pushThisToVector, SIGNAL(mouseReleased(Node*)), this, SLOT(releasedMouseNode(Node*)));
                zValue = 100 - (i + j);
                pushThisToVector->setZValue(zValue);

            }
            connect(pushThisToVector, SIGNAL(hoverEnter(GridObject*)), this, SLOT(enterMouseGridObj(GridObject*)));
            connect(pushThisToVector, SIGNAL(hoverLeft(GridObject*)), this, SLOT(exitMouseGridObj(GridObject*)));
            temp.push_back(pushThisToVector);
        }
        this->_allGameObjects.push_back(temp);
    }
    connectNodes(infoFromFile, this->_allGameObjects);
    addGameGraphics(this->_gameScene);
}

//Adds all the items graphically to the QGraphicsScene
void GameLogic::addGameGraphics(QGraphicsScene* aScene)
{
    this->_gameScene = aScene;
    this->_gameScene->setBackgroundBrush(QBrush(QColor(227,227,227)));
    for (int i = 0; i < this->_allGameObjects.size(); ++i)
    {
        for (int j = 0; j < this->_allGameObjects.size(); j++)
        {
            this->_gameScene->addItem(this->_allGameObjects[i][j]);
        }
    }
}

//Checks if a left clicked line is a valid placement, also checks if the added line finishes the game
void GameLogic::clickedLine(Line *line)
{
    if(line->isPotentialLine() && !line->checkIsDouble())
    {
        //adds double line
        enterMouseGridObjLine(dynamic_cast<GridObject *>(line));

        line->getFirstConnection()->addBridge(this->_currentDirection);
        line->getSecondConnection()->addBridge(DirConections::getOppositeDirection(this->_currentDirection));
        for(GridObject * item : this->_highLightedObjects[this->_currentDirection])
        {
            if(this->_gridTypeIndicator[item->getXPos()][item->getYPos()] == LINE)
            {
                Line * highlightedLine = dynamic_cast<Line *>(item);
                highlightedLine->addSecondLine();
            }
        }
    }
    checkGameFinished();
}

//A line is rightclicked and checked if double or single, updates the nodes it connects to and the graphical interface
void GameLogic::rightClickedLine(Line *line)
{
    Node * firstNode = line->getFirstConnection();
    Node * secondNode = line->getSecondConnection();

    if(line->getYPos() < firstNode->getYPos())  //top
    {
        this->_currentDirection = Direction::top;
    }
    else if(line->getXPos() > firstNode->getXPos())  //right
    {
        this->_currentDirection = Direction::right;
    }
    else if(line->getYPos() > firstNode->getYPos())  //bottom
    {
        this->_currentDirection = Direction::bottom;
    }
    else if(line->getXPos() < firstNode->getXPos())  //left
    {
        this->_currentDirection = Direction::left;
    }


    firstNode->removeBridge(this->_currentDirection);
    secondNode->removeBridge(DirConections::getOppositeDirection(this->_currentDirection));

    //get items between nodes
    //WARNING THIS VECTOR DOES NOT ALWAYS HOLD ITEMS BROOOO (maybe?) vvvvv
    vector<GridObject*> directionObjects = firstNode->getAllPotentialLines(&_allGameObjects)[this->_currentDirection];

    if(line->checkIsDouble())
    {

        for(GridObject * item : directionObjects)
        {
            if(this->_gridTypeIndicator[item->getXPos()][item->getYPos()] == LINE)
            {
                Line * selectedLine = dynamic_cast<Line *>(item);
                selectedLine->removeSecondLine();
            }
        }
        enterMouseGridObj(dynamic_cast<GridObject *>(line));
    }
    else
    {
        for(GridObject * item : directionObjects)
        {
            if(this->_gridTypeIndicator[item->getXPos()][item->getYPos()] == LINE)
            {
                Line * selectedLine = dynamic_cast<Line *>(item);
                linetoEmpty(selectedLine);
            }
        }
        clearHighlighted();
        updateHighlighted();
    }
    this->_islandText->hide();
}

//Checks if the empty spot clicked is a potential line and if a line can be added
void GameLogic::clickedEmpty(Empty *empty)
{
    if(empty->isPotentialLine())
    {
        vector<GridObject*> directionObjects = this->_highLightedObjects[this->_currentDirection];
        Node * node1 = this->_activeNode;
        Node * node2 = this->_activeNode->getConnectedNodes()[this->_currentDirection];

        node1->addBridge(this->_currentDirection);
        node2->addBridge(DirConections::getOppositeDirection(this->_currentDirection));

        for(GridObject * object : directionObjects)
        {
            emptyToLine(dynamic_cast<Empty *>(object), node1, node2);
            delete object;
            updateHighlighted();

            checkGameFinished();
        }
    }
}

//Convert empty to a line
void GameLogic::emptyToLine(Empty *empty, Node* conn1, Node* conn2)
{
    int x = empty->getXPos();
    int y = empty->getYPos();
    //QPoint point(x*(WINDOWSIZE/_gameGridSize), y*(WINDOWSIZE/_gameGridSize));
    QPoint point = empty->getPoint();
    Orientation ori;
    if(this->_currentDirection == Direction::right || this->_currentDirection == Direction::left)
    {
        ori = Orientation::horizontal;
    }
    else if(this->_currentDirection == Direction::top || this->_currentDirection == Direction::bottom)
    {
        ori = Orientation::vertical;
    }
    else
    {
        ori = Orientation::vertical;
    }

    GridObject* line = new Line(x, y, ori, point, WINDOWSIZE/ this->_gameGridSize, conn1, conn2);

    connect(line, SIGNAL(clickedLine(Line*)), this, SLOT(clickedLine(Line*)));
    connect(line, SIGNAL(rightClickedLine(Line*)), this, SLOT(rightClickedLine(Line*)));

    connect(line, SIGNAL(hoverEnter(GridObject*)), this, SLOT(enterMouseGridObj(GridObject*)));
    connect(line, SIGNAL(hoverLeft(GridObject*)), this, SLOT(exitMouseGridObj(GridObject*)));

    this->_allGameObjects[x][y] = line;
    this->_gridTypeIndicator[x][y] = LINE;
    this->_gameScene->removeItem(empty);
    this->_gameScene->addItem(this->_allGameObjects[x][y]);

    if(conn1->isFull() || conn2->isFull())
    {
        clearHighlighted();
    }
    else
    {
        this->_highLightedObjects.clear();
        this->_highLightedObjects[this->_currentDirection].push_back(line);
        line->setPotentialLine(true);
    }
}

//Convert line to an empty
void GameLogic::linetoEmpty(Line *line)
{
    int x = line->getXPos();
    int y = line->getYPos();
    //QPoint point(x*(WINDOWSIZE/ this->_gameGridSize), y*(WINDOWSIZE/ this->_gameGridSize));
    QPoint point = line->getPoint();
    GridObject* empty = new Empty(x, y, point, WINDOWSIZE/ this->_gameGridSize);
    connect(empty, SIGNAL(clickedEmpty(Empty*)), this, SLOT(clickedEmpty(Empty*)));
    connect(empty, SIGNAL(hoverEnter(GridObject*)), this, SLOT(enterMouseGridObj(GridObject*)));
    connect(empty, SIGNAL(hoverLeft(GridObject*)), this, SLOT(exitMouseGridObj(GridObject*)));
    this->_allGameObjects[x][y] = empty;
    this->_gridTypeIndicator[x][y] = EMPTY;
    this->_gameScene->removeItem(line);
    this->_gameScene->addItem(_allGameObjects[x][y]);
    QApplication::restoreOverrideCursor();
}

//Look for alignment between two nodes
bool GameLogic::isDirectionOrientationAligned(Direction dir, Orientation ori)
{
    switch(ori)
    {
    case Orientation::vertical:
        if(dir == Direction::top || dir == Direction::bottom)
        {
            return true;
        }
        else
            return false;
    case Orientation::horizontal:
        if(dir == Direction::right || dir == Direction::left)
        {
            return true;
        }
        else
            return false;
    }
}

//Checks if the mouse is over a potential line, updates the cursor to indicate if clickable or not
void GameLogic::enterMouseGridObj(GridObject *gridObj)
{
    if(this->_gridTypeIndicator[gridObj->getXPos()][gridObj->getYPos()] == LINE)  //hover line
    {
        QApplication::setOverrideCursor(Qt::PointingHandCursor);
        enterMouseGridObjLine(gridObj);
    }
    else if(gridObj->isPotentialLine()) //hover empty
    {
        QApplication::setOverrideCursor(Qt::PointingHandCursor);
        updateCurrentDirection(gridObj);
    }
    else
    {
        QApplication::restoreOverrideCursor();
        clearHighlighted();
        this->_highLightedObjects.clear();
    }
}

void GameLogic::exitMouseGridObj(GridObject *gridObj)
{
    QApplication::restoreOverrideCursor();
}

//Highlights all the potential lines a user can place from the node that has a mouseover
void GameLogic::enterMouseNode(Node *node)
{
    QApplication::restoreOverrideCursor();
    if(!node->isFull())
    {
        this->_activeNode = node;

        this->_activeNodeCoords = QPoint(node->getXPos(), node->getYPos());

        clearHighlighted();

        node->setPotentialLine(true);

        for(int i = 0; i < node->getConnectedNodes().size(); i++)
        {
            if(node->getConnectedNodes()[static_cast<Direction>(i+1)] != nullptr)
            {
                node->getConnectedNodes()[static_cast<Direction>(i+1)]->setPotentialLine(true);
            }
        }

        //highlight spaces inbetween
        std::map<Direction, std::vector<GridObject *> > objectsToHighlight = node->getAllPotentialLines(&_allGameObjects);
        for(int i = 0; i < objectsToHighlight.size(); i++)
        {
            Direction localCurrentDirection = static_cast<Direction>(i+1);

            //clears direction if direction's node target is full
            if(node->getConnectedNodes()[localCurrentDirection] != nullptr)
            {
                if(node->getConnectedNodes()[localCurrentDirection]->isFull())
                {
                    objectsToHighlight[localCurrentDirection].clear();
                    node->getConnectedNodes()[localCurrentDirection]->setPotentialLine(false);
                }
            }

            vector<GridObject *> objectVector = objectsToHighlight[localCurrentDirection];
            vector<GridObject *> objectsToAdd;
            for(GridObject * object : objectVector)
            {

                bool isLine = (this->_gridTypeIndicator[object->getXPos()][object->getYPos()] == LINE);

                if(isLine)
                {
                    GridObject * selectedObject = this->_allGameObjects[object->getXPos()][object->getYPos()];
                    Line * objectToTest = dynamic_cast<Line * >(selectedObject);
                    bool isCorrectOrient = this->isDirectionOrientationAligned(localCurrentDirection, objectToTest->getOrientation());

                    if(!isCorrectOrient)
                    {
                        node->getConnectedNodes()[localCurrentDirection]->setPotentialLine(false);
                        objectsToAdd.clear();
                        break;
                    }
                }
                if(object == nullptr)
                {
                    break;
                }

                objectsToAdd.push_back(object);
            }

            for(GridObject * object : objectsToAdd)
            {
                object->setPotentialLine(true);
            }

            this->_highLightedObjects[localCurrentDirection] = objectsToAdd;
        }



        updateHighlighted();
    }
}

//Highlights all nodes that form a continuous connection with the node with an orange color
void GameLogic::clickedNode(Node *node)
{
    vector<Node *> * myIslandVector = new std::vector<Node *>();

    checkNodeConnected(node,myIslandVector);

    for(Node * item : (*myIslandVector))
    {
        item->setEllipseColor(QColor("orange"));
    }
}

//Removes orange color highlight
void GameLogic::releasedMouseNode(Node *node)
{
    vector<Node *> * myIslandVector = new std::vector<Node *>();

    checkNodeConnected(node,myIslandVector);

    for(Node * item : (*myIslandVector))
    {
        item->updateColor();
    }
}

// Form a connection between nodes that lie next to each other
void GameLogic::connectNodes(QByteArray infoFromFile, vector<vector<GridObject *>> board)
{
    for(int x = 0; x<this->_gameGridSize; x++)
    {
        vector<int> tempIntVec;
        for(int y = 0; y<this->_gameGridSize; y++)
        {
            tempIntVec.push_back(infoFromFile[y* this->_gameGridSize + x] - 48);
        }
        this->_gridTypeIndicator.push_back(tempIntVec);
    }
    for(int x = 0; x<_gameGridSize; x++)
    {
        for(int y = 0; y<_gameGridSize; y++)
        {
            if(this->_gridTypeIndicator[x][y] != EMPTY)
            {
                Node * currentNode = dynamic_cast<Node *>(board[x][y]);
                //right
                for(int i = x + 1; i<this->_gameGridSize; i++)
                {
                    if(this->_gridTypeIndicator[i][y] != EMPTY)
                    {
                        currentNode->connectNodes(Direction::right, dynamic_cast<Node *>(board[i][y]));
                        break;
                    }
                }
                //left
                for(int i = x - 1; i>=0; i--)
                {
                    if(this->_gridTypeIndicator[i][y] != EMPTY)
                    {
                        currentNode->connectNodes(Direction::left, dynamic_cast<Node *>(board[i][y]));
                        break;
                    }
                }
                //bottom
                for(int i = y + 1; i<this->_gameGridSize; i++)
                {
                    if(this->_gridTypeIndicator[x][i] != EMPTY)
                    {
                        currentNode->connectNodes(Direction::bottom, dynamic_cast<Node *>(board[x][i]));
                        break;
                    }
                }
                //top
                for(int i = y - 1; i>=0; i--)
                {
                    if(this->_gridTypeIndicator[x][i] != EMPTY)
                    {
                        currentNode->connectNodes(Direction::top, dynamic_cast<Node *>(board[x][i]));
                        break;
                    }
                }
            }
        }
    }
}

//Highlight all the potential lines with a graphical indicator
void GameLogic::updateHighlighted()
{
    //fill node current direction
    if(this->_activeNode != nullptr)
    {
        if(this->_activeNode->isPotentialLine())
        {
            map<Direction, Node *> nodeConnections = this->_activeNode->getConnectedNodes();
            for(int i = 0; i < this->_activeNode->getConnectedNodes().size(); i++)
            {
                Direction localCurrentDir = static_cast<Direction>(i+1);
                if(nodeConnections[localCurrentDir] != nullptr)
                {
                    if(nodeConnections[localCurrentDir]->isPotentialLine())
                    {
                        this->_activeNode->fillBackground(localCurrentDir);
                        nodeConnections[localCurrentDir]->fillBackground(DirConections::getOppositeDirection(localCurrentDir));
                    }
                }
            }
        }
    }
    for (int i = 0; i < this->_allGameObjects.size(); ++i)
    {
        for (int j = 0; j < this->_allGameObjects.size(); j++)
        {
            if(this->_allGameObjects[i][j]->isPotentialLine())
            {
                if(this->_gridTypeIndicator[i][j] < 1)    //if not node
                {
                    this->_allGameObjects[i][j]->setBrush(QBrush(QColor(229,215,135)));
                }
            }
            else
            {
                if(this->_gridTypeIndicator[i][j] > 0)    //if node
                {
                    dynamic_cast<Node *>(this->_allGameObjects[i][j])->clearBackground();
                }
                else
                {
                    this->_allGameObjects[i][j]->setBrush(QBrush(Qt::transparent));
                }
            }
        }
    }
}

//Remove the potential line from all nodes and update graphically
void GameLogic::clearHighlighted()
{

    for (int i = 0; i <this->_allGameObjects.size(); ++i)
    {
        for (int j = 0; j < this->_allGameObjects.size(); j++)
        {
             this->_allGameObjects[i][j]->setPotentialLine(false);
        }
    }

    updateHighlighted();
}

//Activate a specific direction
void GameLogic::activateDirection(Direction direction)
{
    this->_currentDirection = direction;
    vector<GridObject*> directionObjects = this->_highLightedObjects[direction];
    this->_highLightedObjects.clear();
    this->_highLightedObjects[direction] = directionObjects;
    clearHighlighted();
    this->_activeNode->setPotentialLine(true);
    this->_activeNode->getConnectedNodes()[direction]->setPotentialLine(true);
    for(GridObject* item: this->_highLightedObjects[direction])
    {
        item->setPotentialLine(true);
    }
    updateHighlighted();
}

//Update graphic highlighting when the user moves from a node to a gridobject
void GameLogic::enterMouseGridObjLine(GridObject *gridObj)
{
    clearHighlighted();
    this->_highLightedObjects.clear();

    Line * selectedLine = dynamic_cast<Line *>(gridObj);
    Node * firstNode = selectedLine->getFirstConnection();
    Node * secondNode = selectedLine->getSecondConnection();

    if(!(firstNode->isFull() || secondNode->isFull()) && !selectedLine->checkIsDouble())
    {
        selectedLine->setPotentialLine(true);
        for(int i = 0; i < firstNode->getConnectedNodes().size(); i++)
        {
            this->_currentDirection = static_cast<Direction>(i+1);
            if(firstNode->getConnectedNodes()[this->_currentDirection] == secondNode)
            {
                vector<GridObject*> directionObjects = firstNode->getAllPotentialLines(&_allGameObjects)[this->_currentDirection];
                clearHighlighted();
                firstNode->setPotentialLine(true);
                secondNode->setPotentialLine(true);
                for(GridObject* item: directionObjects)
                {
                    this->_highLightedObjects[this->_currentDirection].push_back(item);
                    item->setPotentialLine(true);
                }
                updateHighlighted();


                break;
            }
        }
    }
}

//Sets the direction of the current connection a gridobject has
void GameLogic::updateCurrentDirection(GridObject *gridObj)
{
    if(gridObj->getYPos() < this->_activeNodeCoords.y())  //top
    {
        activateDirection(Direction::top);
    }
    else if(gridObj->getXPos() > this->_activeNodeCoords.x())  //right
    {
        activateDirection(Direction::right);
    }
    else if(gridObj->getYPos() > this->_activeNodeCoords.y())  //bottom
    {
        activateDirection(Direction::bottom);
    }
    else if(gridObj->getXPos() < this->_activeNodeCoords.x())  //left
    {
        activateDirection(Direction::left);
    }
}

//Check if the user has finished the game with all nodes in one continuous connection
void GameLogic::checkGameFinished()
{
    vector<Node *> allNodes;

    for(int x = 0; x < this->_gameGridSize; x++)
    {
        for(int y = 0; y < this->_gameGridSize; y++)
        {
            if(this->_gridTypeIndicator[x][y] > 0)
            {
                Node * currentNode = dynamic_cast<Node *>(this->_allGameObjects[x][y]);
                if(!currentNode->isFull())
                {
                    return;
                }
                allNodes.push_back(currentNode);
            }
        }
    }

    vector<Node *> * myIslandVector = new std::vector<Node *>();

    checkNodeConnected(allNodes[0],myIslandVector);

    if(myIslandVector->size() == allNodes.size())
    {
        finishGame();
    }
    else
    {
        for(Node * item : (*myIslandVector))
        {
            item->setEllipseColor(Qt::red);
            this->_islandText->show();
        }
    }
}

//Fills a list with the node connections from the nodeToCheck
void GameLogic::checkNodeConnected(Node * nodeToCheck, vector<Node *> * nodesAdded)
{
    //lägg till aktiv nod i listan
    nodesAdded->push_back(nodeToCheck);

    //för varje direction
    for(int i = 0; i < 4; i++)
    {
        Direction directionToCheck = static_cast<Direction>(i+1);

        Node * currentNode = nodeToCheck->getConnectedNodes()[directionToCheck];
        int connectionInDirection = nodeToCheck->getSlotMap()[directionToCheck];

        //kolla nod om det finns en anslutning i den riktningen
        if(currentNode != nullptr && connectionInDirection)
        {
            //om noden vi kollar finns i de vi har gått genom
            if( !(find(nodesAdded->begin(), nodesAdded->end(), currentNode) != nodesAdded->end()))
            {
                checkNodeConnected(currentNode, nodesAdded);
            }
        }
    }
}

//Add a victory message and a button to return to main menu after the game is finished
void GameLogic::finishGame()
{
    this->_won = true;
    QGraphicsRectItem * overlay = new QGraphicsRectItem(0,0,600,600);
    overlay->setBrush(QBrush(QColor(255,255,255,150)));
    overlay->setZValue(200);
    this->_gameScene->addItem(overlay);
    for(vector<GridObject *> itemVector : this->_allGameObjects)
    {
        for(GridObject * item : itemVector)
        {
            item->setAcceptHoverEvents(false);
            item->setAcceptedMouseButtons(0);// was false RAG
        }
    }
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QGraphicsTextItem * victoryText = new QGraphicsTextItem("Congratulations, you won!");
    victoryText->setZValue(300);
    victoryText->setPos(25, 200);
    victoryText->setFont(QFont("Gill Sans MT", 34, QFont::Bold, true));
    this->_gameScene->addItem(victoryText);

    this->_btnReturnMenu = new QPushButton;
    this->_btnReturnMenu->setGeometry(QRect(120, 300, 360, 150));
    this->_btnReturnMenu->setText("Return To Menu");
    this->_btnReturnMenu->setFont(QFont("Gill Sans MT", 18, QFont::Bold, true));;
    this->_proxyBtnReturnMenu = new QGraphicsProxyWidget;
    this->_proxyBtnReturnMenu = _gameScene->addWidget(_btnReturnMenu);
    this->_proxyBtnReturnMenu->setParentItem(overlay);
    connect(this->_btnReturnMenu, SIGNAL(clicked()), this, SLOT(endGameButtonClicked()));
}

void GameLogic::endGameButtonClicked()
{
    this->_gameScene->removeItem(this->_btnReturnMenu->graphicsProxyWidget());
    emit endGameButtonClickedSignal();
}

void GameLogic::endGame()
{
    this->_gameScene->setBackgroundBrush(QBrush(Qt::transparent));
    delete this;
}

GameLogic::~GameLogic()
{
   this->_allGameObjects.erase(this->_allGameObjects.begin(),this->_allGameObjects.end());
}
