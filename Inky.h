#include "Ghost.h"

#ifndef INKY_H
#define INKY_H

#include <QObject>
#include <QLabel>

class Inky : virtual public Ghost {
    Q_OBJECT

public:
	int BlinkyX, BlinkyY;
	char PacDirection;
	Inky(QObject *parent = nullptr);
	void findTarget(int pX, int pY) override;    
};

#endif // INKY_H

