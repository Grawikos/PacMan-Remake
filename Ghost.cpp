#include "Ghost.h"
#include <QPropertyAnimation>
#include <QDebug>
#include <QMap>
#include <QRandomGenerator>
#include <QVector>

#define SIZE_MODIFIER 2
#define SPRITE_SIZE 14 * SIZE_MODIFIER
#define PADDING 12
#define SIZE_MODIFIER 2
#define TILE_SIZE 8 * SIZE_MODIFIER
#define ROWS 28
#define COLUMNS 31
#define PACE 180
#define JUMPPACE PACE/2
#define SPACE_SCORE 50
#define WALL 1
#define FRAMES 4
#define X 0
#define Y 1


Ghost::Ghost(QObject *parent) : QObject(parent) {
    mode = CHASE;
    isEaten = false;
    direction = {1, 0};
    setOffset(-SPRITE_SIZE / 2, -SPRITE_SIZE / 2);  // offset
    pace = PACE;
	
    animTimer = new QTimer(this);
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
}



void Ghost::setPosition(int _x, int _y){
	x = _x;
	y = _y;
    setPos(x * TILE_SIZE + PADDING, y * TILE_SIZE + PADDING);
}

void Ghost::setSpritePos(float _x, float _y) {
    setPos(_x * TILE_SIZE + PADDING, _y * TILE_SIZE + PADDING);
}



void Ghost::getFrightened(){
	pace = PACE * 1.5;
	mode = FRIGHTENED;
	direction = {-direction[X], -direction[Y]};
	loadSprite("pacman_sprites/ScaredGhost.png");
}


void Ghost::eaten(){
	isEaten = true;
	loadSprite("pacman_sprites/Eyes.png");
	pace = PACE/3;
}

void Ghost::backHome(){
	if(x == 13 && y == 11){
		pace = PACE;
		isEaten = false;
		mode = CHASE;
		pace = PACE;
		loadSprite(sprite);
		move();
	}else if(!eatenLoop){
		eatenLoop = true;
		targetX = 13;
		targetY = 11;
		pace = PACE / 3;
		chooseTargetDirection();
		int newX = x + direction[X], newY = y + direction[Y];
		animate(newX, newY);
		QTimer::singleShot(pace, this, [=]() {
			setPosition(newX, newY);
			animTimer->stop();
			eatenLoop = false;
			backHome();
		});
	}else{
		
	}
}

void Ghost::nextMove(){	
	if(isEaten){
		backHome();
		return;
	}
	switch(mode){
		case CHASE:
			move();
			break;
		case SCATTER:
			break;
		case FRIGHTENED:
			runAway();
	}
}

void Ghost::chooseRandomDirection(){
	QVector<QVector<int>> possibleDirections = {
		{-1, 0}, 
		{1, 0},  
		{0, -1}, 
		{0, 1}   
	};
	QVector<int> reverse = {-direction[X], -direction[Y]};
	QVector<QVector<int>> validDirections;
	for (const auto& dir : possibleDirections) {
		if(dir == reverse) continue;
		int newX = x + dir[X];
		int newY = y + dir[Y];

		if (map[newY][newX] != 1) {
			validDirections.push_back(dir);
		}
	}
	int randomIndex = QRandomGenerator::global()->bounded(validDirections.size());
	direction = validDirections[randomIndex];
}

void Ghost::runAway(){
	chooseRandomDirection();
	int newX = x + direction[X], newY = y + direction[Y];
    if (x == 27 && y == 14 && newX == 28) {
		setPosition(0, 14);
		newX = 0;
		return;
	}
	else if (x == 0 && y == 14 && newX == -1) {
		setPosition(27, 14);
		newX = 27;
		return;
	}
	animate(newX, newY);
    QTimer::singleShot(pace, this, [=]() {
        setPosition(newX, newY);
        animTimer->stop();
    });
}

void Ghost::chooseTargetDirection(){
	QVector<QVector<int>> possibleDirections = {
		{-1, 0}, 
		{1, 0},  
		{0, -1}, 
		{0, 1}   
	};
	QVector<float> distances;
	QVector<int> reverse = {-direction[X], -direction[Y]};

	QVector<QVector<int>> validDirections;
	for (const auto& dir : possibleDirections) {
		if(dir == reverse) continue;
		int newX = x + dir[X]; // X coordinate
		int newY = y + dir[Y]; // Y coordinate

		if (map[newY][newX] != 1) {
			validDirections.push_back(dir);
			distances.push_back((newX - targetX) * (newX - targetX) + (newY - targetY) * (newY - targetY));
		}
	}
	float minDst = 999999;
	for(int i = 0; i < validDirections.size(); i++){
		if(distances[i] < minDst){
			direction = validDirections[i];
			minDst = distances[i];
		}
	}
}

void Ghost::move(){
	chooseTargetDirection();
	int newX = x + direction[X], newY = y + direction[Y];
    if (x == 27 && y == 14 && newX == 28) {
		setPosition(0, 14);
		newX = 0;
		return;
	}
	else if (x == 0 && y == 14 && newX == -1) {
		setPosition(27, 14);
		newX = 27;
		return;
	}
	animate(newX, newY);
    QTimer::singleShot(pace, this, [=]() {
        setPosition(newX, newY);
        animTimer->stop();
    });
}

void Ghost::animate(int toX, int toY){
	
	animTimer->start((pace - 5)/FRAMES);
	float dx = x - toX;
	float dy = y - toY;
	frame = 0;
	disconnect(animTimer, &QTimer::timeout, this, nullptr);
	connect(animTimer, &QTimer::timeout, this, [=]() {
        setSpritePos(x - frame*dx/FRAMES, y - frame*dy/FRAMES);
        frame++;
        if(frame >= FRAMES){
			animTimer->stop();
		}
    });
	
}


void Ghost::loadSprite(QString s){
	setPixmap(QPixmap(s).scaled(SPRITE_SIZE, SPRITE_SIZE));
}

void Ghost::reset(){
	mode = CHASE;
	pace = PACE;
	QVector<int> direction = {0, 0};
	setPosition(12 + (homeX + homeY) % 4, 11);
	loadSprite(sprite);
	isEaten = false;
	eatenLoop = false;
}

