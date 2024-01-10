#include "Ghost.h"

#ifndef INKY_H
#define INKY_H

#include <QObject>
#include <QLabel>
#include <QVector>


class Inky : virtual public Ghost {
    Q_OBJECT

public:
	int BlinkyX, BlinkyY;
	QVector<int> PacDirection;
	Inky(QObject *parent = nullptr);
	void findTarget(int pX, int pY) override;   
	void newChase() override; 
};

#endif // INKY_H

