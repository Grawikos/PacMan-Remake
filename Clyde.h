#include "Ghost.h"

#ifndef CLYDE_H
#define CLYDE_H

#include <QObject>
#include <QLabel>

class Clyde : virtual public Ghost {
    Q_OBJECT

public:
	Clyde(QObject *parent = nullptr);
	void findTarget(int pX, int pY) override;    
	void newChase() override;
};

#endif // Clyde_H

