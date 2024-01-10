// mainwindow.cpp
#include "mainwindow.h"
#include "defines.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), gameStarted(false) {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    setFixedSize(ROWS * TILE_SIZE + 30, COLUMNS * TILE_SIZE + SPACE_SCORE + 20);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setAlignment(Qt::AlignTop);


    QHBoxLayout* scoreLayout = new QHBoxLayout();
	scoreLabel = new QLabel("Score: 0", centralWidget);
	scoreLabel->setFixedHeight(SPACE_SCORE);
	scoreLayout->addWidget(scoreLabel);
	highScoreLabel = new QLabel("High Score: 0", centralWidget);
	scoreLayout->addWidget(highScoreLabel, Qt::AlignCenter);

	scene = new QGraphicsScene();
    QGraphicsView* graphicsView = new QGraphicsView(scene, centralWidget);

    QGraphicsPixmapItem* backgroundImageItem = scene->addPixmap(QPixmap("pacman_sprites/Board.png"));
    backgroundImageItem->setScale(2);
    backgroundImageItem->setPos(0, 0);
    
    graphicsView->setFixedSize(ROWS * TILE_SIZE + 3, COLUMNS * TILE_SIZE + 3);
    graphicsView->setSceneRect(0, 0, ROWS * TILE_SIZE, COLUMNS * TILE_SIZE);
    //
    pacman = new Player(centralWidget);
	scene->addItem(pacman);
	pacman->setPosition(14, 23);

    blinky = new Blinky(centralWidget);

	scene->addItem(blinky);
    ghosts.push_back(blinky);
/*
	pinky = new Pinky(centralWidget);
	inky = new Inky(centralWidget);
	clyde = new Clyde(centralWidget);
    
	scene->addItem(pinky);
	scene->addItem(inky);
	scene->addItem(clyde);

	ghosts.push_back(pinky);
	ghosts.push_back(inky);
	ghosts.push_back(clyde);
*/
    mainLayout->addLayout(scoreLayout);
    mainLayout->addWidget(graphicsView);
    
    food.load("pacman_sprites/Food_small.png");
	bigFood.load("pacman_sprites/Food_big.png");
	
    lastKeyPressed = Qt::Key_unknown;
    
	initialiseMap();
    continuousMoveTimer = new QTimer(this);
    ghostMoveTimer = new QTimer(this);
    
	connect(ghostMoveTimer, &QTimer::timeout, this, &MainWindow::ghostMove);
	connect(continuousMoveTimer, &QTimer::timeout, this, &MainWindow::continuousMove);
    startScreen();
}


void MainWindow::initialiseMap(){
	map = { // 0 - empty 1 - wall, 2 - food, 3 - powerup
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
	{1,3,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,3,1},
	{1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
	{1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
	{1,2,2,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,2,2,1},
	{1,1,1,1,1,1,2,1,1,1,1,1,0,1,1,0,1,1,1,1,1,2,1,1,1,1,1,1},
	{1,1,1,1,1,1,2,1,1,1,1,1,0,1,1,0,1,1,1,1,1,2,1,1,1,1,1,1},
	{0,0,0,0,1,1,2,1,1,0,0,0,0,0,0,0,0,0,0,1,1,2,1,1,0,0,0,0},
	{1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
	{1,1,1,1,1,1,2,1,1,0,1,1,0,0,0,0,1,1,0,1,1,2,1,1,1,1,1,1},
	{0,0,0,0,0,0,2,0,0,0,1,1,0,0,0,0,1,1,0,0,0,2,0,0,0,0,0,0},
	{1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
	{1,0,0,0,0,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,0,0,0,0,1},
	{0,0,0,0,0,1,2,1,1,0,0,0,0,0,0,0,0,0,0,1,1,2,1,0,0,0,0,0},
	{0,0,0,0,0,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,0,0,0,0,0},
	{1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
	{1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
	{1,3,2,2,1,1,2,2,2,2,2,2,2,0,0,2,2,2,2,2,2,2,1,1,2,2,3,1},
	{1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
	{1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
	{1,2,2,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,2,2,1},
	{1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1},
	{1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	
	
	foodItemsGrid.resize(ROWS + 2);
	for (int i = 0; i < foodItemsGrid.size(); ++i) {
		foodItemsGrid[i].resize(COLUMNS + 2);
	}
	for (int row = 0; row <= ROWS + 1; ++row)
	{
		for (int col = 0; col < COLUMNS; ++col)
		{
			if (map[row][col] == 2)
			{
				QGraphicsPixmapItem *foodItem = scene->addPixmap(food);
				foodItem->setScale(SIZE_MODIFIER);
				foodItem->setPos(col * TILE_SIZE - PADDING, row * TILE_SIZE - PADDING);
				foodItemsGrid[row][col] = foodItem;
			}
			else if (map[row][col] == 3)
			{
				QGraphicsPixmapItem *bigFoodItem = scene->addPixmap(bigFood);
				bigFoodItem->setScale(SIZE_MODIFIER);
				bigFoodItem->setPos(col * TILE_SIZE - 4, row * TILE_SIZE - 4);
				foodItemsGrid[row][col] = bigFoodItem;
			}
		}
	}
}

int MainWindow::readHighScore() {
    int highScore = 0;

    QFile file("highScore.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in >> highScore;
        file.close();
    } else {
        qDebug() << "Failed to open highScore.txt for reading";
    }

    return highScore;
}

void MainWindow::writeHighScore(){
	QFile file("highScore.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << highScore;
        file.close();
    } else {
        qDebug() << "Failed to open highScore.txt for writing";
    }
	
}

void MainWindow::startScreen(){
	score = 0;
	highScore = readHighScore();
	updateHighScore();
	
    for(auto g : ghosts){
		g->reset();
	}

}



void MainWindow::ghostMove(){
/*	pinky->PacDirection = pacman->direction;
	inky->PacDirection = pacman->direction;
	inky->BlinkyX = blinky->x;
	inky->BlinkyY = blinky->y;
*/	checkDeathColision();
	
	for(Ghost* ghost : ghosts){
		ghost->findTarget(pacman->x, pacman->y);
		ghost->nextMove();
	}
}

void MainWindow::restart(){
	gameStarted = false;
	pacman->reset();
	for(auto r : foodItemsGrid){
		for(auto c : r){
			if(c && c->scene() == scene)
				c->show();
		}
	}

	map = { // 0 - empty 1 - wall, 2 - food, 3 - powerup
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
	{1,3,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,3,1},
	{1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
	{1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
	{1,2,2,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,2,2,1},
	{1,1,1,1,1,1,2,1,1,1,1,1,0,1,1,0,1,1,1,1,1,2,1,1,1,1,1,1},
	{1,1,1,1,1,1,2,1,1,1,1,1,0,1,1,0,1,1,1,1,1,2,1,1,1,1,1,1},
	{0,0,0,0,1,1,2,1,1,0,0,0,0,0,0,0,0,0,0,1,1,2,1,1,0,0,0,0},
	{1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
	{1,1,1,1,1,1,2,1,1,0,1,1,0,0,0,0,1,1,0,1,1,2,1,1,1,1,1,1},
	{0,0,0,0,0,0,2,0,0,0,1,1,0,0,0,0,1,1,0,0,0,2,0,0,0,0,0,0},
	{1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
	{1,0,0,0,0,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,0,0,0,0,1},
	{0,0,0,0,0,1,2,1,1,0,0,0,0,0,0,0,0,0,0,1,1,2,1,0,0,0,0,0},
	{0,0,0,0,0,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,0,0,0,0,0},
	{1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
	{1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
	{1,3,2,2,1,1,2,2,2,2,2,2,2,0,0,2,2,2,2,2,2,2,1,1,2,2,3,1},
	{1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
	{1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
	{1,2,2,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,2,2,1},
	{1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1},
	{1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	startScreen();
	
}

void MainWindow::death(){
	if(score > highScore){
		highScore = score;
		updateHighScore();
	}
	ghostMoveTimer->stop();
	//pacman->animTimer->stop();
	continuousMoveTimer->stop();
	
	QDialog gameOverDialog;
    gameOverDialog.setWindowTitle("Game Over");
    gameOverDialog.resize(200, 100);

    QPushButton resetButton("Restart");
    connect(&resetButton, &QPushButton::clicked, &gameOverDialog, &QDialog::accept);

    QPushButton quitButton("Quit");
    connect(&quitButton, &QPushButton::clicked, &gameOverDialog, &QDialog::reject);

    QVBoxLayout layout(&gameOverDialog);
    layout.addWidget(&resetButton);
    layout.addWidget(&quitButton);

    int result = gameOverDialog.exec();

    if (result == QDialog::Accepted) {
        restart();
    } else if (result == QDialog::Rejected) {
		writeHighScore();
		qApp->quit();
    }
}


void MainWindow::keyPressEvent(QKeyEvent *event) {
	if (!gameStarted) {
		ghostMoveTimer->start(MAIN_PACE); 
		gameStarted = true;
    }
	int key = event->key();
    switch (key) {
        case Qt::Key_W:
        case Qt::Key_S:
        case Qt::Key_A:
        case Qt::Key_D:
			lastKeyPressed = key;
			if (!continuousMoveTimer->isActive()) {
				continuousMove();
				continuousMoveTimer->start(MAIN_PACE);
			} 
            break;
    }
}

void MainWindow::powerUP(){
	score += 10;
	pacman->isPoweredUP = true;
	for(Ghost* g : ghosts){
		g->getFrightened();
	}
	ghostMoveTimer->setInterval(MAIN_PACE * 1.5);
	QTimer::singleShot(8000, this, [&](){ 	// 8s ?
		pacman->isPoweredUP = false;
		for(Ghost* g : ghosts){
			if(!g->isEaten)
				g->newChase();
		}
		ghostMoveTimer->setInterval(MAIN_PACE);	
	}); 
	scoreMulitiplier = 1;
	updateScore();
}

bool MainWindow::checkDeathColision(){
	for(Ghost* ghost : ghosts){
		if(((ghost->x == pacman->x) && (ghost->y == pacman->y)) ||
			((pacman->x + pacman->direction[X] == ghost->x) &&
			(pacman->y + pacman->direction[Y] == ghost->y) &&
			(ghost->x + ghost->direction[X] == pacman->x)&&
			(ghost->y + ghost->direction[Y] == pacman->y)))
		{
			if(pacman->isPoweredUP){
					
				if(!ghost->isEaten){	
					score += 20 * scoreMulitiplier;	
					ghost->eaten();
					scoreMulitiplier *= 2;
				}
			}
			else if(!ghost->isEaten){
				return true;
			}
		}
	}
	return false;
}

void MainWindow::continuousMove() {
    int newX = pacman->x;
    int newY = pacman->y;
    QVector direction = {0, 0};
    switch (lastKeyPressed) {
        case Qt::Key_W:
			direction = {0, -1};
            newY--;
            break;
        case Qt::Key_S:
			direction = {0, 1};
            newY++;
            break;
        case Qt::Key_A:
			direction = {-1, 0};
            newX--;
            break;
        case Qt::Key_D:
			direction = {1, 0};
            newX++;
            break;
    }

    // Check wall
    if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLUMNS &&
		map[newY][newX] != 1) {
		if(checkDeathColision()){
			death();
			return;
		}
		pacman->direction = direction;
		pacman->move(newX, newY);
		if(map[newY][newX] == 2){
			score++;
			updateScore();
			map[newY][newX] = 0;
			foodItemsGrid[newY][newX]->hide();
		}
		if(map[newY][newX] == 3){
			powerUP();
			map[newY][newX] = 0;
			foodItemsGrid[newY][newX]->hide();
		}
		
	}
	else if (pacman->x == 27 && pacman->y == 14 && newX == 28) {
		pacman->direction = {1, 0};
		pacman->jump(0);
	}
	else if (pacman->x == 0 && pacman->y == 14 && newX == -1) {
		pacman->direction = {-1, 0};
		pacman->jump(27);
	}
	else{
		directionalMove();
	}

}


void MainWindow::directionalMove(){
	int newX = pacman->x;
    int newY = pacman->y;
    newX += pacman->direction[0];
    newY += pacman->direction[1];
    
    if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLUMNS &&
		map[newY][newX] != 1) {
		if(checkDeathColision()){
			death();
			return;
		}
		pacman->move(newX, newY);
		if(map[newY][newX] == 2){
			score++;
			updateScore();
			map[newY][newX] = 0;
			foodItemsGrid[newY][newX]->hide();
		}
		if(map[newY][newX] == 3){
			powerUP();
			map[newY][newX] = 0;
			foodItemsGrid[newY][newX]->hide();
		}
	}
	else if (pacman->x == 27 && pacman->y == 14 && newX == 28) {
		pacman->jump(0);
	}
	else if (pacman->x == 0 && pacman->y == 14 && newX == -1) {
		pacman->jump(27);
	}
	else{
		pacman->direction = {0, 0};
	}
    
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QPainter painter(this);
}

void MainWindow::updateScore() {
    scoreLabel->setText("Score: " + QString::number(score));
}

void MainWindow::updateHighScore() {
    highScoreLabel->setText("High Score: " + QString::number(highScore));
}

