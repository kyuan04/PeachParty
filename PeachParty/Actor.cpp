#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
void Avatar::doSomething() {
    if (waitingToRoll == true) {
        int action = getWorld()->getAction(playerNumber);
        if (action == ACTION_ROLL) {
            int dieRoll = randInt(1, 10);
            ticksToMove = dieRoll * 8;
            waitingToRoll = false;
        } else {
            return;
        }
    }
    
    if (waitingToRoll == false) {
        //if avatar can't continue moving forward in current direction
        int newX;
        int newY;
        if (ticksToMove % 8 == 0) {
            getPositionInThisDirection(getMoveDirection(), 16, newX, newY);
            if (!getWorld()->isValidPosition(newX, newY)) {
                if (getMoveDirection() == right || getMoveDirection() == left) {
                    getPositionInThisDirection(up, 16, newX, newY);
                    if (getWorld()->isValidPosition(newX, newY)) {
                        setMoveDirection(up);
                    } else {
                        setMoveDirection(down);
                    }
                } else if (getMoveDirection() == up || getMoveDirection() == down) {
                    getPositionInThisDirection(right, 16, newX, newY);
                    if (getWorld()->isValidPosition(newX, newY)) {
                        setMoveDirection(right);
                    } else {
                        setMoveDirection(left);
                    }
                }
            }
            
        }
        
        //make sprite face the right direction
        if (getMoveDirection() == left) {
            setDirection(left);
        } else {
            setDirection(right);
        }
        
        moveAtAngle(getMoveDirection(), 2);
        ticksToMove--;
        if (ticksToMove == 0) {
            waitingToRoll = true;
            
            }
    }
}

void BlueCoinSquare::doSomething() {
    if (!isActive()) {
        return;
    }
    
    Peach* p = getWorld()->getPeach();
    Yoshi* y = getWorld()->getYoshi();
    
    if (!p->hasLanded()) {
        p->setActivatedSquare(false);
    }
    
    if (!y->hasLanded()) {
        y->setActivatedSquare(false);
    }
    
    if (p->hasLanded() && p->getX() == this->getX() && p->getY() == this->getY() && !p->hasActivatedSquare()) {
        p->resetNumCoins(3);
        getWorld()->playSound(SOUND_GIVE_COIN);
        p->setActivatedSquare(true);
    }
    
    if (y->hasLanded() && y->getX() == this->getX() && y->getY() == this->getY() && !y->hasActivatedSquare()) {
        y->resetNumCoins(3);
        getWorld()->playSound(SOUND_GIVE_COIN);
        y->setActivatedSquare(true);
    }
}

void RedCoinSquare::doSomething() {
    if (!isActive()) {
        return;
    }
    
    Peach* p = getWorld()->getPeach();
    Yoshi* y = getWorld()->getYoshi();
    
    if (!p->hasLanded()) {
        p->setActivatedSquare(false);
    }
    
    if (!y->hasLanded()) {
        y->setActivatedSquare(false);
    }
    
    if (p->hasLanded() && p->getX() == this->getX() && p->getY() == this->getY() && !p->hasActivatedSquare()) {
        p->resetNumCoins(-3);
        getWorld()->playSound(SOUND_TAKE_COIN);
        p->setActivatedSquare(true);
    }
    
    if (y->hasLanded() && y->getX() == this->getX() && y->getY() == this->getY() && !y->hasActivatedSquare()) {
        y->resetNumCoins(-3);
        getWorld()->playSound(SOUND_TAKE_COIN);
        y->setActivatedSquare(true);
    }
}
