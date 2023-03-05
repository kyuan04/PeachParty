#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Avatar::findPossibleDirections() {
    possibleDirections.clear();
    numPossibleDirections = 0;
    int newX;
    int newY;
    
    getPositionInThisDirection(up, 16, newX, newY);
    if (getWorld()->isValidPosition(newX, newY)) {
        numPossibleDirections++;
        possibleDirections.insert(90);
    }
    
    newX = 0;
    newY = 0;
    
    getPositionInThisDirection(down, 16, newX, newY);
    if (getWorld()->isValidPosition(newX, newY)) {
        numPossibleDirections++;
        possibleDirections.insert(270);
    }
    
    newX = 0;
    newY = 0;
    
    getPositionInThisDirection(left, 16, newX, newY);
    if (getWorld()->isValidPosition(newX, newY)) {
        numPossibleDirections++;
        possibleDirections.insert(180);
    }
    
    newX = 0;
    newY = 0;
    
    getPositionInThisDirection(right, 16, newX, newY);
    if (getWorld()->isValidPosition(newX, newY)) {
        numPossibleDirections++;
        possibleDirections.insert(0);
    }
    
}

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
        int newX;
        int newY;
        
        if (ticksToMove % 8 == 0) {
            findPossibleDirections();
            
            //if player is on directional square
            if (getForcingDir() != -1) {
                setMoveDirection(getForcingDir());
                if (getMoveDirection() == left) {
                    setDirection(left);
                } else {
                    setDirection(right);
                }
                setForcingDir(-1);
            } else if (numPossibleDirections >= 3) {
                int forkAction = getWorld()->getAction(playerNumber);
                int originalDir = getMoveDirection();
                if ((forkAction == ACTION_UP) && (possibleDirections.find(90) != possibleDirections.end()) && (originalDir != down)) {
                    setMoveDirection(up);
                } else if ((forkAction == ACTION_DOWN) && (possibleDirections.find(270) != possibleDirections.end()) && (originalDir != up)) {
                    setMoveDirection(down);
                } else if ((forkAction == ACTION_LEFT) && (possibleDirections.find(180) != possibleDirections.end()) && (originalDir != right)) {
                    setMoveDirection(left);
                } else if ((forkAction == ACTION_RIGHT) && (possibleDirections.find(0) != possibleDirections.end()) && (originalDir != left)) {
                    setMoveDirection(right);
                } else {
                    return;
                }
                
                //adjust direction the sprite is facing
                if (getMoveDirection() == left) {
                    setDirection(left);
                } else {
                    setDirection(right);
                }
                
                
            }
            
            //if avatar can't continue moving forward in current direction
            newX = 0;
            newY = 0;
            getPositionInThisDirection(getMoveDirection(), 16, newX, newY);
            findPossibleDirections();
            if (!getWorld()->isValidPosition(newX, newY) && numPossibleDirections < 3) {
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
        if (p->getNumCoins() < 3) {
            p->resetNumCoins(p->getNumCoins() * -1);
        } else {
            p->resetNumCoins(-3);
        }
        getWorld()->playSound(SOUND_TAKE_COIN);
        p->setActivatedSquare(true);
    }
    
    if (y->hasLanded() && y->getX() == this->getX() && y->getY() == this->getY() && !y->hasActivatedSquare()) {
        if (y->getNumCoins() < 3) {
            y->resetNumCoins(y->getNumCoins() * -1);
        } else {
            y->resetNumCoins(-3);
        }
        getWorld()->playSound(SOUND_TAKE_COIN);
        y->setActivatedSquare(true);
    }
}

void StarSquare::doSomething() {
    Peach* p = getWorld()->getPeach();
    Yoshi* y = getWorld()->getYoshi();
    
    if (!p->hasLanded()) {
        p->setActivatedSquare(false);
    }
    
    if (!y->hasLanded()) {
        y->setActivatedSquare(false);
    }
    
    if (p->getX() == this->getX() && p->getY() == this->getY() && !p->hasActivatedSquare()) {
        if (p->getNumCoins() < 20) {
            return;
        } else {
            p->resetNumCoins(-20);
            p->addStar();
            getWorld()->playSound(SOUND_GIVE_STAR);
            p->setActivatedSquare(true);
        }
    }
    
    if (y->getX() == this->getX() && y->getY() == this->getY() && !y->hasActivatedSquare()) {
        if (y->getNumCoins() < 20) {
            return;
        } else {
            y->resetNumCoins(-20);
            y->addStar();
            getWorld()->playSound(SOUND_GIVE_STAR);
            y->setActivatedSquare(true);
        }
    }
}

void DirectionSquare::doSomething() {
    Peach* p = getWorld()->getPeach();
    Yoshi* y = getWorld()->getYoshi();
    
    if (p->getX() == this->getX() && p->getY() == this->getY()) {
        p->setForcingDir(this->getForcingDir());
        //p->setMoveDirection(getForcingDir());
//        if (p->getMoveDirection() == left) {
//            p->setDirection(left);
//        } else {
//            p->setDirection(right);
//        }
    }
    if (y->getX() == this->getX() && y->getY() == this->getY()) {
        y->setForcingDir(this->getForcingDir());
        //y->setMoveDirection(getForcingDir());
//        if (y->getMoveDirection() == left) {
//            y->setDirection(left);
//        } else {
//            y->setDirection(right);
//        }
    }
}

void BankSquare::doSomething() {
    Peach* p = getWorld()->getPeach();
    Yoshi* y = getWorld()->getYoshi();
    
    if (!p->hasLanded()) {
        p->setActivatedSquare(false);
    }
    
    if (!y->hasLanded()) {
        y->setActivatedSquare(false);
    }
    
    //if peach lands on bank square
    if (p->hasLanded() && p->getX() == this->getX() && p->getY() == this->getY() && !p->hasActivatedSquare()) {
        p->resetNumCoins(getBankBalance());
        resetBankBalance(getBankBalance() * -1);
        getWorld()->playSound(SOUND_WITHDRAW_BANK);
        p->setActivatedSquare(true);
    } else if (!p->hasLanded() && p->getX() == this->getX() && p->getY() == this->getY() && !p->hasActivatedSquare()) { //not landed but moves onto bank square
        int coinsToDeduct;
        if (p->getNumCoins() < 5) {
            coinsToDeduct = p->getNumCoins();
        } else {
            coinsToDeduct = 5;
        }
        p->resetNumCoins(coinsToDeduct * -1);
        resetBankBalance(coinsToDeduct);
        getWorld()->playSound(SOUND_DEPOSIT_BANK);
        p->setActivatedSquare(true);
    }
    
    //if yoshi lands on bank square
    if (y->hasLanded() && y->getX() == this->getX() && y->getY() == this->getY() && !y->hasActivatedSquare()) {
        y->resetNumCoins(getBankBalance());
        resetBankBalance(getBankBalance() * -1);
        getWorld()->playSound(SOUND_WITHDRAW_BANK);
        y->setActivatedSquare(true);
    } else if (!y->hasLanded() && y->getX() == this->getX() && y->getY() == this->getY() && !y->hasActivatedSquare()) { //not landed but moves onto bank square
        int coinsToDeduct;
        if (y->getNumCoins() < 5) {
            coinsToDeduct = y->getNumCoins();
        } else {
            coinsToDeduct = 5;
        }
        y->resetNumCoins(coinsToDeduct * -1);
        resetBankBalance(coinsToDeduct);
        getWorld()->playSound(SOUND_DEPOSIT_BANK);
        y->setActivatedSquare(true);
    }
}

void Bowser::doSomething() {
    Peach* p = getWorld()->getPeach();
    Yoshi* y = getWorld()->getYoshi();
    
    if (!isWalking()) {
        if (p->getX() == this->getX() && p->getY() == this->getY() && p->hasLanded()) {
            int choice = randInt(1, 2);
            if (choice == 1) {
                p->resetNumCoins(p->getNumCoins() * -1);
                p->setNumStars(0);
                getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
            }
        }
        
        if (y->getX() == this->getX() && y->getY() == this->getY() && y->hasLanded()) {
            int choice = randInt(1, 2);
            if (choice == 1) {
                y->resetNumCoins(y->getNumCoins() * -1);
                y->setNumStars(0);
                getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
            }
        }
        
        decrementPauseCounter();
        
//        if (pauseCounter == 0) {
//            int squares_to_move = randInt(1, 10);
//            
//        }
    }
}
