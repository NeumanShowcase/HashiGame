#include "hashlife.h"
#include "ui_hashlife.h"

Hashlife::Hashlife(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Hashlife)
{
    ui->setupUi(this);
    ui->_graphicsView->setStyleSheet("background: url(:/images/background_image.png);");
    this->_mainScene = new QGraphicsScene();
    this->_mainScene->setSceneRect(QRectF(0, 0, WINDOWSIZE, WINDOWSIZE));
    this->_levelFromWeb = "";
    ui->_graphicsView->setScene(_mainScene);
    ui->_graphicsView->show();
    ui->_graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    ui->menuBar->hide();
    checkInternetConnection();
    checkWebLoadingEngine();
    addMenuGraphics();
    autoScaleView();
}

//Add menu graphics at start of application or after user ends a game
void Hashlife::addMenuGraphics()
{
    ui->menuBar->hide();
    this->_btnNewGame = new QPushButton;
    this->_btnNewGame->setGeometry(QRect(120, 30, 360, 150));
    this->_btnNewGame->setText("New Game");
    this->_btnNewGame->setFont(QFont("Gill Sans MT", 18, QFont::Bold, true));
    this->_proxyNewGame = new QGraphicsProxyWidget;
    this->_proxyNewGame = _mainScene->addWidget(_btnNewGame);
    connect(_btnNewGame, SIGNAL(released()), this, SLOT(setupGame()));
    this->_btnHelp = new QPushButton;
    this->_btnHelp->setGeometry(QRect(120, 225, 360, 150));
    this->_btnHelp->setText("Help");
    this->_btnHelp->setFont(QFont("Gill Sans MT", 18, QFont::Bold, true));
    this->_proxyHelp = new QGraphicsProxyWidget;
    this->_proxyHelp = _mainScene->addWidget(_btnHelp);
    connect(_btnHelp, SIGNAL(released()), this, SLOT(help()));
    this->_btnExit = new QPushButton;
    this->_btnExit->setGeometry(QRect(120, 420, 360, 150));
    this->_btnExit->setText("Exit");
    this->_btnExit->setFont(QFont("Gill Sans MT", 18, QFont::Bold, true));
    this->_proxyExit = new QGraphicsProxyWidget;
    this->_proxyExit = _mainScene->addWidget(_btnExit);
    connect(_btnExit, SIGNAL(released()), this, SLOT(exit()));
}

//Removes menu and creates the game
void Hashlife::newGame()
{
    this->_logic = new GameLogic(_gridSizeSelection, _mainScene);
    connect(_logic, SIGNAL(endGameButtonClickedSignal()), this, SLOT(on_actionEnd_Game_triggered()));
    this->_mainScene->removeItem(_btnGrid10->graphicsProxyWidget());
    this->_mainScene->removeItem(_btnGrid15->graphicsProxyWidget());
    this->_mainScene->removeItem(_btnGrid20->graphicsProxyWidget());
    this->_mainScene->removeItem(_btnEasy->graphicsProxyWidget());
    this->_mainScene->removeItem(_btnMedium->graphicsProxyWidget());
    this->_mainScene->removeItem(_btnHard->graphicsProxyWidget());
    this->_mainScene->removeItem(_btnStart->graphicsProxyWidget());
    this->_mainScene->removeItem(_btnReturn->graphicsProxyWidget());
    this->_mainScene->removeItem(_difficultyLabel->graphicsProxyWidget());
    this->_mainScene->removeItem(_gridLabel->graphicsProxyWidget());
    this->_mainScene->removeItem(_boardLabel->graphicsProxyWidget());
    this->_mainScene->removeItem(_comboGridLabel->graphicsProxyWidget());
    this->_mainScene->removeItem(_comboDifficultyLabel->graphicsProxyWidget());
    this->_mainScene->removeItem(_boardSelector->graphicsProxyWidget());
    ui->menuBar->show();
    ui->_graphicsView->fitInView( _mainScene->sceneRect(), Qt::KeepAspectRatio);
    try
    {
        chosenMap();    
    }
    catch(QString ERRORCODE)
    {
        QErrorMessage fileProblem(this); fileProblem.showMessage(ERRORCODE); fileProblem.exec();
    }
    setupGameGraphics();
}

//Opens a window with an instructions and a gif
void Hashlife::help()
{
    checkFirstRun();
    const int width = 520;
    const int height = 800;
    const int marginLeft = 10;
    const int marginRight = 20;
    QDialog* helpDialog = new QDialog(0,Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    helpDialog->setFixedSize(QSize(width,height));
    helpDialog->setStyleSheet("background-color:#ffffff;");

    QLabel gifExplain(helpDialog);
    QMovie *pictureGif = new QMovie(":/images/tutorial.gif");
    gifExplain.setMovie(pictureGif);
    pictureGif->setScaledSize(QSize(width-marginRight,width-marginRight));
    pictureGif->start();
    gifExplain.setGeometry(marginLeft,0,width-marginRight,width-marginRight);

    QLabel ruleLabel("Rules:\n"
                     "- The goal of the game is to connect nodes together via bridges.\n"
                     "- The game ends when all nodes have as many bridges going out from \n"
                     "     them as their own value.\n"
                     "- All nodes must form one single connection.\n"
                     "- Each node may contain 0, 1 or 2 bridges in any given direction.\n"
                     "- You place a bridge by hovering over a node and selecting\n"
                     "     a valid bridge by left clicking with the mouse.\n"
                     "- You can remove a bridge by right clicking on it.\n"
                     "- Bridges may not cross each other.\n"
                     "- Bridges can only be vertical or horizontal.",
                     helpDialog);
    ruleLabel.setFont(QFont("Gill Sans MT", 11, QFont::Bold, false));
    ruleLabel.setGeometry(marginLeft,width+10,width-marginRight,height-width);
    helpDialog->exec();
}

void Hashlife::exit()
{
    qApp->exit();
}

//Called from the game initiating state if the user clicks the return button
void Hashlife::returnToMenu()
{
    this->_mainScene->removeItem(this->_btnGrid10->graphicsProxyWidget());
    this->_mainScene->removeItem(this->_btnGrid15->graphicsProxyWidget());
    this->_mainScene->removeItem(this->_btnGrid20->graphicsProxyWidget());
    this->_mainScene->removeItem(this->_btnEasy->graphicsProxyWidget());
    this->_mainScene->removeItem(this->_btnMedium->graphicsProxyWidget());
    this->_mainScene->removeItem(this->_btnHard->graphicsProxyWidget());
    this->_mainScene->removeItem(this->_btnStart->graphicsProxyWidget());
    this->_mainScene->removeItem(this->_btnReturn->graphicsProxyWidget());
    this->_mainScene->removeItem(this->_difficultyLabel->graphicsProxyWidget());
    this->_mainScene->removeItem(this->_gridLabel->graphicsProxyWidget());
    this->_mainScene->removeItem(this->_boardLabel->graphicsProxyWidget());
    this->_mainScene->removeItem(this->_comboGridLabel->graphicsProxyWidget());
    this->_mainScene->removeItem(this->_comboDifficultyLabel->graphicsProxyWidget());
    this->_mainScene->removeItem(this->_boardSelector->graphicsProxyWidget());
    addMenuGraphics();
}

//User selects difficulty in the game initiating state
void Hashlife::difficultyClicked()
{
    this->_boardSelector->setCurrentIndex(0);
    switch(this->_grpDifficulty->checkedId())
    {
    case 0:
        this->_comboDifficultyLabel->setText("Easy:");
        break;
    case 1:
        this->_comboDifficultyLabel->setText("Medium:");;
        break;
    case 2:
        this->_comboDifficultyLabel->setText("Hard:");;
        break;
    default:
        break;
    }
}

//User selects game board size in the game initiating state
void Hashlife::gridClicked()
{
    this->_boardSelector->setCurrentIndex(0);
    switch(this->_grpGridSize->checkedId())
    {
    case 0:
        this->_gridSizeSelection = 9;
        this->_comboGridLabel->setText("9 x 9:");
        break;
    case 1:
        this->_gridSizeSelection = 13;
        this->_comboGridLabel->setText("13 x 13:");;
        break;
    case 2:
        this->_gridSizeSelection = 17;
        this->_comboGridLabel->setText("17 x 17:");;
        break;
    default:
        break;
    }
}

void Hashlife::setupGameGraphics()
{
    this->_logic->addGameGraphics(this->_mainScene);
}

void Hashlife::resizeEvent(QResizeEvent *event)
{
    autoScaleView();
}

void Hashlife::showEvent(QShowEvent *event)
{
    ui->_graphicsView->fitInView( this->_mainScene->sceneRect(), Qt::KeepAspectRatio);
}

void Hashlife::autoScaleView()
{
    ui->_graphicsView->fitInView( this->_mainScene->sceneRect(), Qt::KeepAspectRatio);
}

Hashlife::~Hashlife()
{

}

//Game initiating state called when the user clicks new game from main menu
void Hashlife::setupGame()
{
    this->_mainScene->removeItem(this->_btnNewGame->graphicsProxyWidget());
    this->_mainScene->removeItem(this->_btnHelp->graphicsProxyWidget());
    this->_mainScene->removeItem(this->_btnExit->graphicsProxyWidget());
    this->_gridSizeSelection = 9;
    this->_btnReturn = new QPushButton;
    this->_btnReturn->setGeometry(QRect(480, 0, 120, 45));
    this->_btnReturn->setText("Return");
    this->_btnReturn->setFont(QFont("Gill Sans MT", 18, QFont::Bold, true));;
    this->_btnReturn->setMaximumSize(120, 45);
    this->_proxyReturn = new QGraphicsProxyWidget;
    this->_proxyReturn = this->_mainScene->addWidget(this->_btnReturn);
    this->connect(this->_btnReturn, SIGNAL(clicked()), this, SLOT(returnToMenu()));
    this->_gridLabel = new QLabel;
    this->_gridLabel->setText("Select Grid Size:");
    this->_gridLabel->setFont(QFont("Gill Sans MT", 12, QFont::Bold, false));
    this->_gridLabel->setGeometry(QRect(237, 30, 150, 30));
    this->_gridLabel->setStyleSheet("background-color: rgba(0,0,0,0%); color: white;");
    this->_proxyGridLabel = new QGraphicsProxyWidget;
    this->_proxyGridLabel = this->_mainScene->addWidget(this->_gridLabel);
    this->_btnGrid10 = new QPushButton;
    this->_btnGrid10->setGeometry(QRect(90, 60, 120, 120));
    this->_btnGrid10->setText("9 x 9");
    this->_btnGrid10->setFont(QFont("Gill Sans MT", 18, QFont::Bold, true));
    this->_btnGrid10->setCheckable(true);
    this->_btnGrid10->setChecked(true);
    this->_btnGrid10->setMaximumSize(120, 120);
    this->_proxyGrid10 = new QGraphicsProxyWidget;
    this->_proxyGrid10 = this->_mainScene->addWidget(this->_btnGrid10);
    this->_btnGrid15 = new QPushButton;
    this->_btnGrid15->setGeometry(QRect(240, 60, 120, 120));
    this->_btnGrid15->setText("13 x 13");
    this->_btnGrid15->setFont(QFont("Gill Sans MT", 18, QFont::Bold, true));
    this->_btnGrid15->setCheckable(true);
    this->_btnGrid15->setMaximumSize(120, 120);
    this->_proxyGrid15 = new QGraphicsProxyWidget;
    this->_proxyGrid15 = this->_mainScene->addWidget(this->_btnGrid15);
    this->_btnGrid20 = new QPushButton;
    this->_btnGrid20->setGeometry(QRect(390, 60, 120, 120));
    this->_btnGrid20->setText("17 x 17");
    this->_btnGrid20->setFont(QFont("Gill Sans MT", 18, QFont::Bold, true));
    this->_btnGrid20->setCheckable(true);
    this->_btnGrid20->setMaximumSize(120, 120);
    this->_proxyGrid20 = new QGraphicsProxyWidget;
    this->_proxyGrid20 = this->_mainScene->addWidget(this->_btnGrid20);
    this->_difficultyLabel = new QLabel;
    this->_difficultyLabel->setText("Select Difficulty:");
    this->_difficultyLabel->setFont(QFont("Gill Sans MT", 12, QFont::Bold, false));
    this->_difficultyLabel->setGeometry(QRect(234, 180, 150, 30));
    this->_difficultyLabel->setStyleSheet("background-color: rgba(0,0,0,0%); color: white;");
    this->_proxyDifficultyLabel = new QGraphicsProxyWidget;
    this->_proxyDifficultyLabel = this->_mainScene->addWidget(this->_difficultyLabel);
    this->_btnEasy = new QPushButton;
    this->_btnEasy->setGeometry(QRect(90, 210, 120, 120));
    this->_btnEasy->setText("Easy");
    this->_btnEasy->setFont(QFont("Gill Sans MT", 18, QFont::Bold, true));
    this->_btnEasy->setCheckable(true);
    this->_btnEasy->setChecked(true);
    this->_btnEasy->setMaximumSize(120, 120);
    this->_proxyEasy = new QGraphicsProxyWidget;
    this->_proxyEasy = this->_mainScene->addWidget(this->_btnEasy);
    this->_btnMedium = new QPushButton;
    this->_btnMedium->setGeometry(QRect(240, 210, 120, 120));
    this->_btnMedium->setText("Medium");
    this->_btnMedium->setFont(QFont("Gill Sans MT", 18, QFont::Bold, true));
    this->_btnMedium->setCheckable(true);
    this->_btnMedium->setMaximumSize(120, 120);
    this->_proxyMedium = new QGraphicsProxyWidget;
    this->_proxyMedium = this->_mainScene->addWidget(this->_btnMedium);
    this->_btnHard = new QPushButton;
    this->_btnHard->setGeometry(QRect(390, 210, 120, 120));
    this->_btnHard->setText("Hard");
    this->_btnHard->setFont(QFont("Gill Sans MT", 18, QFont::Bold, true));
    this->_btnHard->setCheckable(true);
    this->_btnHard->setMaximumSize(120, 120);
    this->_proxyHard = new QGraphicsProxyWidget;
    this->_proxyHard = this->_mainScene->addWidget(this->_btnHard);
    this->_btnStart = new QPushButton;
    this->_btnStart->setGeometry(QRect(120, 420, 360, 150));
    this->_btnStart->setText("Start Game");
    this->_btnStart->setFont(QFont("Gill Sans MT", 18, QFont::Bold, true));;
    this->_proxyStart = new QGraphicsProxyWidget;
    this->_proxyStart = this->_mainScene->addWidget(this->_btnStart);
    connect(_btnStart, SIGNAL(clicked()), this, SLOT(newGame()));
    this->_boardLabel = new QLabel;
    this->_boardLabel->setText("Select Game Board:");
    this->_boardLabel->setFont(QFont("Gill Sans MT", 12, QFont::Bold, false));
    this->_boardLabel->setGeometry(QRect(228, 330, 160, 30));
    this->_boardLabel->setStyleSheet("background-color: rgba(0,0,0,0%); color: white;");
    this->_proxyBoardLabel = new QGraphicsProxyWidget;
    this->_proxyBoardLabel = this->_mainScene->addWidget(this->_boardLabel);
    this->_comboDifficultyLabel = new QLabel;
    this->_comboDifficultyLabel->setText("Easy:");
    this->_comboDifficultyLabel->setFont(QFont("Gill Sans MT", 11, QFont::Bold, false));
    this->_comboDifficultyLabel->setGeometry(QRect(156, 363, 90, 30));
    this->_comboDifficultyLabel->setStyleSheet("background-color: rgba(0,0,0,0%); color: white;");
    this->_proxyComboDifficultyLabel = new QGraphicsProxyWidget;
    this->_proxyComboDifficultyLabel = this->_mainScene->addWidget(this->_comboDifficultyLabel);
    this->_comboGridLabel = new QLabel;
    this->_comboGridLabel->setText("9 x 9:");
    this->_comboGridLabel->setFont(QFont("Gill Sans MT", 11, QFont::Bold, false));
    this->_comboGridLabel->setGeometry(QRect(90, 363, 90, 30));
    this->_comboGridLabel->setStyleSheet("background-color: rgba(0,0,0,0%); color: white;");
    this->_proxyComboGridLabel = new QGraphicsProxyWidget;
    this->_proxyComboGridLabel = this->_mainScene->addWidget(this->_comboGridLabel);
    this->_grpGridSize = new QButtonGroup;
    this->_grpGridSize->addButton(this->_btnGrid10, 0);
    this->_grpGridSize->addButton(this->_btnGrid15, 1);
    this->_grpGridSize->addButton(this->_btnGrid20, 2);
    connect(_grpGridSize, SIGNAL(buttonClicked(int)), this, SLOT(gridClicked()));
    this->_grpDifficulty = new QButtonGroup;
    this->_grpDifficulty->addButton(this->_btnEasy, 0);
    this->_grpDifficulty->addButton(this->_btnMedium, 1);
    this->_grpDifficulty->addButton(this->_btnHard, 2);
    connect(_grpDifficulty, SIGNAL(buttonClicked(int)), this, SLOT(difficultyClicked()));
    this->_boardSelector = new QComboBox;
    this->_boardSelector->setGeometry(QRect(225, 360, 285, 36));
    this->_boardSelector->setFont(QFont("Gill Sans MT", 12, QFont::Bold, false));
    this->_boardSelector->setMaximumSize(285, 36);
    if (!_levelFromWeb.isEmpty())
    {
        _boardSelector->insertItem(0, "Random Online");
    }

    this->_boardSelector->insertItem(1, "Level 1");
    this->_boardSelector->insertItem(2, "Level 2");
    this->_boardSelector->insertItem(3, "Level 3");
    this->_boardSelector->insertItem(4, "Level 4");
    this->_boardSelector->insertItem(5, "Level 5");
    this->_boardSelector->insertItem(6, "Level 6");
    this->_boardSelector->insertItem(7, "Level 7");
    this->_proxyBoardSelector = new QGraphicsProxyWidget;
    this->_proxyBoardSelector = _mainScene->addWidget(_boardSelector);
}

//Called when user decides to end game early or after a finished game
void Hashlife::on_actionEnd_Game_triggered()
{
    _mainScene->clear();
    if(this->_logic != nullptr)
    {
        _logic->endGame();
        this->_logic = nullptr;
    }
    addMenuGraphics();
}

void Hashlife::on_actionHelp_triggered()
{
    help();
}

//Creates a string to the location of the target file based on player choices
void Hashlife::chosenMap()
{
    QString loadThisTypeOfBoard = ROOTDIR;
    QString loadThisFromWeb = WEBDEFAULTURL;
    switch (_grpGridSize->checkedId())
    {
    case 0:
        loadThisTypeOfBoard.append("9x9/");
        loadThisFromWeb.append(WEB9X9URL);
        break;
    case 1:
        loadThisTypeOfBoard.append("13x13/");
        loadThisFromWeb.append(WEB13X13URL);
        break;
    case 2:
        loadThisTypeOfBoard.append("17x17/");
        loadThisFromWeb.append(WEB17X17URL);
        break;
        // Never end up here //
    default:
        throw(QString ("something went wrong while computing user lever choice"));
    }
    switch (_grpDifficulty->checkedId())
    {
    case 0:
        loadThisTypeOfBoard.append("easy/");
        loadThisFromWeb.append(WEBDIFFICULTYURL);
        loadThisFromWeb.append('1');
        break;
    case 1:
        loadThisTypeOfBoard.append("medium/");
        loadThisFromWeb.append(WEBDIFFICULTYURL);
        loadThisFromWeb.append('3');
        break;
    case 2:
        loadThisTypeOfBoard.append("hard/");
        loadThisFromWeb.append(WEBDIFFICULTYURL);
        loadThisFromWeb.append('4');
        break;
        // Never end up here //
    default:
        throw(QString ("something went wrong while computing user lever choice"));
    }

     if (!loadThisTypeOfBoard.isEmpty())
    {
        switch(this->_boardSelector->currentIndex())
        {
        case 0: //Load random level

            if(! this->_levelFromWeb.isEmpty())
            {
                try
                {
                    if (checkFirstRun())
                    {
                        help();
                    }
                    onlineLoad(QUrl(loadThisFromWeb));
                }catch(QString ERRORCODE)
                {
                }
            }
            // never end up here but just in case... //
            else
            {
                loadThisTypeOfBoard.append("level1.hashiboard");
                this->_logic->loadGameBoardFromFile(loadThisTypeOfBoard);
            }

            break;
        case 1:
            loadThisTypeOfBoard.append("level1.hashiboard");
            if (checkFirstRun())
            {
                help();
            }
            this->_logic->loadGameBoardFromFile(loadThisTypeOfBoard);
            break;
        case 2:
            loadThisTypeOfBoard.append("level2.hashiboard");
            if (checkFirstRun())
            {
                help();
            }
            this->_logic->loadGameBoardFromFile(loadThisTypeOfBoard);
            break;
        case 3:
            loadThisTypeOfBoard.append("level3.hashiboard");
            if (checkFirstRun())
            {
                help();
            }
            this->_logic->loadGameBoardFromFile(loadThisTypeOfBoard);
            break;
        case 4:
            loadThisTypeOfBoard.append("level4.hashiboard");
            if (checkFirstRun())
            {
                help();
            }
            this->_logic->loadGameBoardFromFile(loadThisTypeOfBoard);
            break;
        case 5:
            loadThisTypeOfBoard.append("level5.hashiboard");
            if (checkFirstRun())
            {
                help();
            }
            this->_logic->loadGameBoardFromFile(loadThisTypeOfBoard);
            break;
        case 6:
            loadThisTypeOfBoard.append("level6.hashiboard");
            if (checkFirstRun())
            {
                help();
            }
            this->_logic->loadGameBoardFromFile(loadThisTypeOfBoard);
            break;
        case 7:
            loadThisTypeOfBoard.append("level7.hashiboard");
            if (checkFirstRun())
            {
                help();
            }
            this->_logic->loadGameBoardFromFile(loadThisTypeOfBoard);
            break;
        default:
            break;
        }
    }
}

void Hashlife::onlineLoad(QUrl urlToSite)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    if (urlToSite != QUrl(IPTESTURL))
    {
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));
    }
    else
    {
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFromInternetTesting(QNetworkReply*)));
    }
    QNetworkRequest request;
    request.setUrl(urlToSite);
    request.setRawHeader("Connection", "close");
    manager->get(request);

}

void Hashlife::checkInternetConnection()
{
    onlineLoad(QUrl(IPTESTURL));
}

void Hashlife::checkWebLoadingEngine()
{
    QString makeDefaultURL = WEBDEFAULTURL;
    makeDefaultURL.append(WEB9X9URL);
    onlineLoad(QUrl(makeDefaultURL));
}

//Creates a QByteArray from online gameboard resource to be turned into a level
void Hashlife::parseSiteData(QString dataFromSite)
{
    QByteArray result = "";
    QString playingLevelID = "";
// regexp to make level string //
    QRegularExpression starthere("<div class=\"hashi\">");
    QRegularExpression checkForNode ("(ring)|(white)");
    QRegularExpression checkBoardID ("(puzzle number:)");
    QRegularExpressionMatchIterator match;
    QRegularExpressionMatch foundNode;
    QRegularExpressionMatch foundStart;
// regexp to validate quality of levelstring //
    QRegularExpression checkForDigit ("\\d");
    QRegularExpressionMatchIterator foundDigit;
    QRegularExpressionMatch matchDigit;

// extra info level id //
    QRegularExpressionMatch matchFoundBoardID;

    matchFoundBoardID = checkBoardID.match(dataFromSite);
    if (matchFoundBoardID.hasMatch())
    {
        for (int i = 1;i < 7;++i)
        {
            playingLevelID.append(dataFromSite[matchFoundBoardID.capturedEnd()+i]);
            qDebug() << playingLevelID << " level ID";
        }
    }

    foundStart = starthere.match(dataFromSite);
    if (foundStart.hasMatch())
    {
        qDebug() << "starting here" << foundStart.capturedEnd();
    }

    // ripping dirty clothes //
    for (int i=0; i < foundStart.capturedEnd();++i)
    {
        dataFromSite[i] = 'a';
    }



    /// node checking //




    match = checkForNode.globalMatch(dataFromSite);

    while (match.hasNext())
    {
        foundNode = match.next();
        if(foundNode.captured() == "white")
        {
            result.append("0");
            continue;
        }
        result.append(dataFromSite.at((foundNode.capturedEnd()+2)));
    }
    foundDigit = checkForDigit.globalMatch(result);
    int foundDigits = 0;
    while(foundDigit.hasNext())
    {
        matchDigit = foundDigit.next();
        ++foundDigits;
    }
    if ((foundDigits == result.length()) && ((result.length() == 81) || (result.length() == 169) || (result.length() == 289 )))
    {
        if (this->_levelFromWeb.isEmpty())
        {
             this->_levelFromWeb = result;
        }
        else
        {
              // make call //
            this->_logic->loadLevel(result);
        }
    }
    else
    {
        this->_levelFromWeb = "";
    }
}

void Hashlife::replyFinished(QNetworkReply *reply)
{
    QString dataFromSite = "";
    if(reply->error() == QNetworkReply::NetworkError::NoError)
    {
        if (this->_clientIP.isEmpty())
        {
        }
        QByteArray readSite = reply->readAll();
        parseSiteData(readSite);
        reply->deleteLater();
    }
}

void Hashlife::replyFromInternetTesting(QNetworkReply* reply)
{
    this->_clientIP = reply->readAll();
}
bool Hashlife::checkFirstRun()
{
    QString lookHere = QApplication::applicationDirPath();
   lookHere.append("/firstRun.isit");
   QFile firstRunfile(lookHere);
   if (!firstRunfile.open(QIODevice::ReadOnly | QIODevice::Text))
{
       firstRunfile.open(QIODevice::ReadWrite | QIODevice::Text);
       QTextStream stream(&firstRunfile);
       stream<<"i think therefore i am";
       firstRunfile.close();

       return true;
}

   else
   {
       return false;
   }
}
