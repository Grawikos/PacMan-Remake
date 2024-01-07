// mainwindow.cpp
#include "mainwindow.h"
#include <QHBoxLayout>
#include <iostream>
#include <QDebug>


#define SIZE_MODIFIER 2
#define SPRITE_SIZE 14 * SIZE_MODIFIER
#define TILE_SIZE 8 * SIZE_MODIFIER
#define ROWS 28
#define COLUMNS 31
#define PADDING 4
#define SPACE_SCORE 50
#define PACE 200 //ms per tile

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), tileSize(TILE_SIZE) {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    setFixedSize(ROWS * TILE_SIZE, COLUMNS * TILE_SIZE + SPACE_SCORE);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setAlignment(Qt::AlignTop);


    QHBoxLayout* scoreLayout = new QHBoxLayout();
	scoreLabel = new QLabel("Score: 0", centralWidget);
	scoreLayout->addWidget(scoreLabel);
	highScoreLabel = new QLabel("High Score: 0", centralWidget);
	scoreLayout->addWidget(highScoreLabel, Qt::AlignCenter);

    QHBoxLayout* centralLayout = new QHBoxLayout(centralWidget);
    centralLayout->setAlignment(Qt::AlignCenter);
    
    pacman = new Player(centralWidget);
    pacman->move(14, 23);
    blinky = new Blinky(centralWidget);
	blinky->x = 1;
	blinky->y = 1;
	pinky = new Pinky(centralWidget);
	pinky->x = 2;
	pinky->y = 1;
	inky = new Inky(centralWidget);
	inky->x = 3;
	inky->y = 1;
    centralLayout->addWidget(pacman->getLabel());
    centralLayout->addWidget(blinky->getLabel());
    centralLayout->addWidget(pinky->getLabel());
    centralLayout->addWidget(inky->getLabel());
    
    
    mainLayout->addLayout(scoreLayout);
    mainLayout->addLayout(centralLayout);
    
    
    food.load("pacman_sprites/Food_small.png");
	bigFood.load("pacman_sprites/Food_big.png");
	backgroundImage.load("pacman_sprites/Board.png");
	backgroundImage = backgroundImage.scaled(ROWS * TILE_SIZE, COLUMNS * TILE_SIZE);
	
    lastKeyPressed = Qt::Key_unknown;
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

    continuousMoveTimer = new QTimer(this);
    ghostMoveTimer = new QTimer(this);
    ghostMoveTimer->start(200);
    
	connect(ghostMoveTimer, &QTimer::timeout, this, &MainWindow::ghostMove);
	connect(continuousMoveTimer, &QTimer::timeout, this, &MainWindow::continuousMove);
	
}

void MainWindow::ghostMove(){
	blinky->findTarget(pacman->x, pacman->y);
	blinky->move();
	pinky->PacDirection = pacman->direction;
	pinky->findTarget(pacman->x, pacman->y);
	pinky->move();
	inky->PacDirection = pacman->direction;
	inky->BlinkyX = blinky->x;
	inky->BlinkyY = blinky->y;
	inky->findTarget(pacman->x, pacman->y);
	inky->move();
	
}


void MainWindow::keyPressEvent(QKeyEvent *event) {
	int key = event->key();
    switch (key) {
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Left:
        case Qt::Key_Right:
			lastKeyPressed = key;
			if (!continuousMoveTimer->isActive()) {
				continuousMove();
				continuousMoveTimer->start(PACE);
			} 
            break;
    }
}


void MainWindow::continuousMove() {
    int newX = pacman->x;
    int newY = pacman->y;
    char direction = 'x';
    //qDebug() << newX << newY;
    switch (lastKeyPressed) {
        case Qt::Key_Up:
			direction = 'w';
            newY--;
            break;
        case Qt::Key_Down:
			direction = 's';
            newY++;
            break;
        case Qt::Key_Left:
			direction = 'a';
            newX--;
            break;
        case Qt::Key_Right:
			direction = 'd';
            newX++;
            break;
    }

    // Check wall
    if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLUMNS &&
		map[newY][newX] != 1) {
		pacman->direction = direction;
		pacman->move(newX, newY);
		if(map[newY][newX] == 2){
			score++;
			updateScore();
			map[newY][newX] = 0;
		}
		if(map[newY][newX] == 3){
			score += 10;
			updateScore();
			map[newY][newX] = 0;
		}
		
	}
	else if (pacman->x == 27 && pacman->y == 14 && newX == 28) {
		pacman->direction = 'd';
		pacman->jump(0);
	}
	else if (pacman->x == 0 && pacman->y == 14 && newX == -1) {
		pacman->direction = 'a';
		pacman->jump(27);
	}
	else{
		directionalMove();
	}

}


void MainWindow::directionalMove(){
	int newX = pacman->x;
    int newY = pacman->y;
    //qDebug() << newX << newY;
    switch (pacman->direction) {
        case 'w':
            newY--;
            break;
        case 's':
			newY++;
            break;
        case 'a':
            newX--;
            break;
        case 'd':
            newX++;
            break;
    }
    if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLUMNS &&
		map[newY][newX] != 1) {
		pacman->move(newX, newY);
		if(map[newY][newX] == 2){
			score++;
			updateScore();
			map[newY][newX] = 0;
		}
		if(map[newY][newX] == 3){
			score += 10;
			updateScore();
			map[newY][newX] = 0;
		}
	}
	else if (pacman->x == 27 && pacman->y == 14 && newX == 28) {
		pacman->jump(0);
	}
	else if (pacman->x == 0 && pacman->y == 14 && newX == -1) {
		pacman->jump(27);
	}
	else{
//		continuousMoveTimer->stop();
		pacman->direction = 'x';
	}
    
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);
	
	painter.drawPixmap(0, SPACE_SCORE, backgroundImage);

	for (int row = 0; row <= ROWS + 1; ++row)
	{
		for (int col = 0; col < COLUMNS; ++col)
		{
			if(map[row][col] == 2)
				painter.drawPixmap(col * TILE_SIZE - PADDING, row * TILE_SIZE + SPACE_SCORE - PADDING, SPRITE_SIZE, SPRITE_SIZE, food);
			else if(map[row][col] == 3)
				painter.drawPixmap(col * TILE_SIZE - PADDING, row * TILE_SIZE + SPACE_SCORE - PADDING, SPRITE_SIZE, SPRITE_SIZE, bigFood);
			
		}
	}
	/*
	for (int row = 10; row < 12; ++row)
	{
		for (int col = 10; col < 12; ++col)
		{
			int spriteIndex = (row * 2 + col) % 4;
			painter.drawPixmap(col * SPRITE_SIZE, row * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE, sprites[spriteIndex]);
		}
	}*/
}

void MainWindow::updateScore() {
    scoreLabel->setText("Score: " + QString::number(score));
}

void MainWindow::updateHighScore() {
    highScoreLabel->setText("High Score: " + QString::number(highScore));
}


