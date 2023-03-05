#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void MovingActor::adjustSpriteDirection() {
    if (getMoveDirection() == left) {
        setDirection(left);
    } else {
        setDirection(right);
    }
}

void MovingActor::checkDirection(int dir, int value) {
    int newX;
    int newY;
    
    getPositionInThisDirection(dir, 16, newX, newY);
    if (getWorld()->isValidPosition(newX, newY)) {
        numPossibleDirections++;
        possibleDirections.insert(value);
    }
}

void MovingActor::findPossibleDirections() {
    possibleDirections.clear();
    numPossibleDirections = 0;
    
    checkDirection(up, 90);
    checkDirection(down, 270);
    checkDirection(left, 180);
    checkDirection(right, 0);
}

void MovingActor::doSomething() {
    if (!isWalking()) {
        doActionWhileWaiting();
    }
    
    if (isWalking()) {
        if (getTicksToMove() % 8 == 0) {
            findPossibleDirections();
            
            //if moving actor is on directional square
            if (getForcingDir() != -1) {
                setMoveDirection(getForcingDir());
                adjustSpriteDirection();
                setForcingDir(-1);
            } else if (numPossibleDirections >= 3) { //if moving actor is at fork
                if (!canDoActionAtFork()) {
                    return;
                }
                //adjust direction the sprite is facing
                adjustSpriteDirection();
            }
            //if avatar can't continue moving forward in current direction
            doActionAtCorner();
        }
        //make sprite face the right direction
        adjustSpriteDirection();
        
        doMove();
    }
}

void MovingActor::teleport() {
    while (true) {
        int randX = randInt(1, 16);
        int randY = randInt(1, 16);
        
        if (getWorld()->isValidPosition(randX * SPRITE_WIDTH, randY * SPRITE_HEIGHT)) {
            moveTo(randX * SPRITE_WIDTH, randY * SPRITE_HEIGHT);
            break;
        }
    }
}

void MovingActor::swap(MovingActor* y) {
    //swap x, y coordinates
    int tempX = getX();
    int tempY = getY();
    moveTo(y->getX(), y->getY());
    y->moveTo(tempX, tempY);
    
    //swap ticksToMove
    int tempTicksToMove = getTicksToMove();
    setTicksToMove(y->getTicksToMove());
    y->setTicksToMove(tempTicksToMove);
    
    //swap move directions
    int tempMoveDirection = getMoveDirection();
    setMoveDirection(y->getMoveDirection());
    y->setMoveDirection(tempMoveDirection);
    
    //swap sprite directions
    int tempDirection = getDirection();
    setDirection(y->getDirection());
    y->setDirection(tempDirection);
    
    //swap walk/roll state
    int tempState = isWalking();
    setWalking(y->isWalking());
    y->setWalking(tempState);
}

void MovingActor::doActionAtCorner() {
    int newX;
    int newY;
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

void MovingActor::setRandomValidDir() {
    int newX;
    int newY;
    bool check = true;
    while (check) {
        int randDir = randInt(1, 4);
        if (randDir == 1) {
            getPositionInThisDirection(up, 16, newX, newY);
            if (getWorld()->isValidPosition(newX, newY)) {
                setMoveDirection(up);
                break;
            }
        } else if (randDir == 2) {
            getPositionInThisDirection(right, 16, newX, newY);
            if (getWorld()->isValidPosition(newX, newY)) {
                setMoveDirection(right);
                break;
            }
        } else if (randDir == 3) {
            getPositionInThisDirection(down, 16, newX, newY);
            if (getWorld()->isValidPosition(newX, newY)) {
                setMoveDirection(down);
                break;
            }
        } else if (randDir == 4) {
            getPositionInThisDirection(left, 16, newX, newY);
            if (getWorld()->isValidPosition(newX, newY)) {
                setMoveDirection(left);
                break;
            }
        }
    }
    
    adjustSpriteDirection();
    setWalking(true);
}

void Avatar::doActionWhileWaiting() {
    
    int action = getWorld()->getAction(playerNumber);
    if (action == ACTION_ROLL) {
        decideMove();
    } else {
        return;
    }
}

bool Avatar::canDoActionAtFork() {
    int forkAction = getWorld()->getAction(playerNumber);
    int originalDir = getMoveDirection();
    if ((forkAction == ACTION_UP) && getPossibleDirections().count(90) && (originalDir != down)) {
        setMoveDirection(up);
        return true;
    } else if ((forkAction == ACTION_DOWN) && getPossibleDirections().count(270) && (originalDir != up)) {
        setMoveDirection(down);
        return true;
    } else if ((forkAction == ACTION_LEFT) && getPossibleDirections().count(180) && (originalDir != right)) {
        setMoveDirection(left);
        return true;
    } else if ((forkAction == ACTION_RIGHT) && getPossibleDirections().count(0) && (originalDir != left)) {
        setMoveDirection(right);
        return true;
    } else {
        return false;
    }
}

void Avatar::doMove() {
    moveAtAngle(getMoveDirection(), 2);
    setTicksToMove(getTicksToMove() - 1);
    if (getTicksToMove() == 0) {
        setWalking(false);
    }
}

void Avatar::decideMove() {
    int dieRoll = randInt(1, 10);
    setTicksToMove(dieRoll * 8);
    setWalking(true);
}

std::string Avatar::vortexMessage() {
    if (hasVortex) {
        return " VOR";
    } else {
        return "";
    }
}

void Baddie::decideMove() {
    setRandomValidDir();
}

void Bowser::doActionWhileWaiting() {
    Peach* p = getWorld()->getPeach();
    Yoshi* y = getWorld()->getYoshi();
    
    if (p->isWalking()) {
        p->setActivatedBaddie(false);
    }
    
    if (y->isWalking()) {
        y->setActivatedBaddie(false);
    }
    
    if (p->getX() == this->getX() && p->getY() == this->getY() && !p->isWalking() && !p->hasActivatedBaddie()) {
        int choice = randInt(1, 2);
        if (choice == 1) {
            p->setNumCoins(0);
            p->setNumStars(0);
            getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
            p->setActivatedBaddie(true);
        }
    }
    
    if (y->getX() == this->getX() && y->getY() == this->getY() && !y->isWalking() && !y->hasActivatedBaddie()) {
        int choice = randInt(1, 2);
        if (choice == 1) {
            y->setNumCoins(0);
            y->setNumStars(0);
            getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
            y->setActivatedBaddie(true);
        }
    }
    
    decrementPauseCounter();
    
    if (getPauseCounter() == 0) {
        int squaresToMove = randInt(1, 10);
        setTicksToMove(squaresToMove * 8);
        findPossibleDirections();
        
        setRandomValidDir();
    }
}

bool Baddie::canDoActionAtFork() {
    setRandomValidDir();
    return true;
}

void Bowser::doMove() {
    moveAtAngle(getMoveDirection(), 2);
    setTicksToMove(getTicksToMove() - 1);
    if (getTicksToMove() == 0) {
        setWalking(false);
        setPauseCounter(180);
        int rand = randInt(1, 4);
        if (rand == 3) {
            //        implement dropping square shit on page 39
            //        Bowser will deposit a dropping by asking the StudentWorld
            //        object to remove the square underneath him and insert a
            //        new Dropping Square in its place and play the
            //        SOUND_DROPPING_SQUARE_CREATED sound using
            //        playSound().
        }
    }
}

void Boo::doActionWhileWaiting() {
    Peach* p = getWorld()->getPeach();
    Yoshi* y = getWorld()->getYoshi();
    
    if (p->isWalking()) {
        p->setActivatedBaddie(false);
    }
    
    if (y->isWalking()) {
        y->setActivatedBaddie(false);
    }
    
    if (p->getX() == this->getX() && p->getY() == this->getY() && !p->isWalking() && !p->hasActivatedBaddie()) {
        int choice = randInt(1, 2);
        if (choice == 1) {
            int temp = p->getNumCoins();
            p->setNumCoins(y->getNumCoins());
            y->setNumCoins(temp);
        } else if (choice == 2) {
            int temp = p->getNumStars();
            p->setNumStars(y->getNumStars());
            y->setNumStars(temp);
        }
        getWorld()->playSound(SOUND_BOO_ACTIVATE);
        p->setActivatedBaddie(true);
    }
    
    if (y->getX() == this->getX() && y->getY() == this->getY() && !y->isWalking() && !y->hasActivatedBaddie()) {
        int choice = randInt(1, 2);
        if (choice == 1) {
            int temp = p->getNumCoins();
            p->setNumCoins(y->getNumCoins());
            y->setNumCoins(temp);
        } else if (choice == 2) {
            int temp = p->getNumStars();
            p->setNumStars(y->getNumStars());
            y->setNumStars(temp);
        }
        getWorld()->playSound(SOUND_BOO_ACTIVATE);
        y->setActivatedBaddie(true);
    }
    
    decrementPauseCounter();
    
    if (getPauseCounter() == 0) {
        int squaresToMove = randInt(1, 3);
        setTicksToMove(squaresToMove * 8);
        findPossibleDirections();
        
        setRandomValidDir();
    }
}

void Boo::doMove() {
    moveAtAngle(getMoveDirection(), 2);
    setTicksToMove(getTicksToMove() - 1);
    if (getTicksToMove() == 0) {
        setWalking(false);
        setPauseCounter(180);
    }
}

void BlueCoinSquare::doSomething() {
    if (!isActive()) {
        return;
    }
    
    Peach* p = getWorld()->getPeach();
    Yoshi* y = getWorld()->getYoshi();
    
    if (p->isWalking()) {
        p->setActivatedSquare(false);
    }
    
    if (y->isWalking()) {
        y->setActivatedSquare(false);
    }
    
    if (!p->isWalking() && p->getX() == this->getX() && p->getY() == this->getY() && !p->hasActivatedSquare()) {
        p->resetNumCoins(3);
        getWorld()->playSound(SOUND_GIVE_COIN);
        p->setActivatedSquare(true);
    }
    
    if (!y->isWalking() && y->getX() == this->getX() && y->getY() == this->getY() && !y->hasActivatedSquare()) {
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
    
    if (p->isWalking()) {
        p->setActivatedSquare(false);
    }
    
    if (y->isWalking()) {
        y->setActivatedSquare(false);
    }
    
    if (!p->isWalking() && p->getX() == this->getX() && p->getY() == this->getY() && !p->hasActivatedSquare()) {
        if (p->getNumCoins() < 3) {
            p->resetNumCoins(p->getNumCoins() * -1);
        } else {
            p->resetNumCoins(-3);
        }
        getWorld()->playSound(SOUND_TAKE_COIN);
        p->setActivatedSquare(true);
    }
    
    if (!y->isWalking() && y->getX() == this->getX() && y->getY() == this->getY() && !y->hasActivatedSquare()) {
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
    
    if (p->isWalking()) {
        p->setActivatedSquare(false);
    }
    
    if (y->isWalking()) {
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
    }
    if (y->getX() == this->getX() && y->getY() == this->getY()) {
        y->setForcingDir(this->getForcingDir());
    }
}

void BankSquare::doSomething() {
    Peach* p = getWorld()->getPeach();
    Yoshi* y = getWorld()->getYoshi();
    
    if (p->isWalking()) {
        p->setActivatedSquare(false);
    }
    
    if (y->isWalking()) {
        y->setActivatedSquare(false);
    }
    
    //if peach lands on bank square
    if (!p->isWalking() && p->getX() == this->getX() && p->getY() == this->getY() && !p->hasActivatedSquare()) {
        p->resetNumCoins(getBankBalance());
        resetBankBalance(getBankBalance() * -1);
        getWorld()->setBankBalance(getBankBalance());
        getWorld()->playSound(SOUND_WITHDRAW_BANK);
        p->setActivatedSquare(true);
    } else if (p->isWalking() && p->getX() == this->getX() && p->getY() == this->getY() && !p->hasActivatedSquare()) { //not landed but moves onto bank square
        int coinsToDeduct;
        if (p->getNumCoins() < 5) {
            coinsToDeduct = p->getNumCoins();
        } else {
            coinsToDeduct = 5;
        }
        p->resetNumCoins(coinsToDeduct * -1);
        resetBankBalance(coinsToDeduct);
        getWorld()->setBankBalance(getBankBalance());
        getWorld()->playSound(SOUND_DEPOSIT_BANK);
        p->setActivatedSquare(true);
    }
    
    //if yoshi lands on bank square
    if (!y->isWalking() && y->getX() == this->getX() && y->getY() == this->getY() && !y->hasActivatedSquare()) {
        y->resetNumCoins(getBankBalance());
        resetBankBalance(getBankBalance() * -1);
        getWorld()->setBankBalance(getBankBalance());
        getWorld()->playSound(SOUND_WITHDRAW_BANK);
        y->setActivatedSquare(true);
    } else if (y->isWalking() && y->getX() == this->getX() && y->getY() == this->getY() && !y->hasActivatedSquare()) { //not landed but moves onto bank square
        int coinsToDeduct;
        if (y->getNumCoins() < 5) {
            coinsToDeduct = y->getNumCoins();
        } else {
            coinsToDeduct = 5;
        }
        y->resetNumCoins(coinsToDeduct * -1);
        resetBankBalance(coinsToDeduct);
        getWorld()->setBankBalance(getBankBalance());
        getWorld()->playSound(SOUND_DEPOSIT_BANK);
        y->setActivatedSquare(true);
    }
}

void EventSquare::doSomething() {
    Peach* p = getWorld()->getPeach();
    Yoshi* y = getWorld()->getYoshi();
    
    if (p->isWalking()) {
        p->setActivatedSquare(false);
    }
    
    if (y->isWalking()) {
        y->setActivatedSquare(false);
    }
    
    if (!p->isWalking() && p->getX() == this->getX() && p->getY() == this->getY() && !p->hasActivatedSquare()) {
        int randAction = randInt(1, 3);
        if (randAction == 1) {
            p->teleport();
            getWorld()->playSound(SOUND_PLAYER_TELEPORT);
            p->setActivatedSquare(true);
        }
        else if (randAction == 2) {
            p->swap(y);
            getWorld()->playSound(SOUND_PLAYER_TELEPORT);
            p->setActivatedSquare(true);
            y->setActivatedSquare(true);
        }
        else if (randAction == 3) {
            p->giveVortex();
            p->setActivatedSquare(true);
        }
    }
    
    if (!y->isWalking() && y->getX() == this->getX() && y->getY() == this->getY() && !y->hasActivatedSquare()) {
        int randAction = randInt(1, 3);
        if (randAction == 1) {
            y->teleport();
            getWorld()->playSound(SOUND_PLAYER_TELEPORT);
            y->setActivatedSquare(true);
        }
        else if (randAction == 2) {
            y->swap(p);
            getWorld()->playSound(SOUND_PLAYER_TELEPORT);
            p->setActivatedSquare(true);
            y->setActivatedSquare(true);
        }
        else if (randAction == 3) {
            y->giveVortex();
            y->setActivatedSquare(true);
        }
    }
}

void Vortex::doSomething() {
    //isWalking() is the same as isActive() in this case
    if (!isWalking()) {
        return;
    }
}
