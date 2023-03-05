#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
    Actor(StudentWorld* world, int imageID, int startX, int startY, int startDir, int depth) : GraphObject(imageID, startX, startY, startDir, depth), m_world(world), forcingDir(-1) {}
    virtual void doSomething() = 0;
    StudentWorld* getWorld() { return m_world; }
    void setForcingDir(int dir) { forcingDir = dir; }
    int getForcingDir() { return forcingDir; }
private:
    StudentWorld* m_world;
    int forcingDir;
};

class MovingActor : public Actor {
public:
    MovingActor(StudentWorld* world, int imageID, int startX, int startY, int startDir, int depth) : Actor(world, imageID, startX, startY, startDir, depth), ticksToMove(0), numPossibleDirections(0), walking(false) {}
    int getTicksToMove() { return ticksToMove; }
    void setTicksToMove(int i) {ticksToMove = i; }
    void setMoveDirection(int dir) { moveDirection = dir; }
    int getMoveDirection() { return moveDirection; }
    bool isWalking() { return walking; }
    void setWalking(bool b) { walking = b; }
    void checkDirection(int dir, int value);
    void findPossibleDirections();
    std::set<int> getPossibleDirections() { return possibleDirections; }
    void adjustSpriteDirection();
    void doActionAtCorner();
    void teleport();
    void swap(MovingActor* y);
    void setRandomValidDir();
    virtual void doSomething();
    virtual void doActionWhileWaiting() = 0;
    virtual bool canDoActionAtFork() = 0;
    virtual void doMove() = 0;
    virtual void decideMove() = 0;
private:
    int ticksToMove;
    bool walking;
    int moveDirection;
    int numPossibleDirections;
    std::set<int> possibleDirections;
};

class Avatar : public MovingActor {
public:
    Avatar(StudentWorld* world, int imageID, int startX, int startY, int number) : MovingActor(world, imageID, startX, startY, 0, 0), playerNumber(number), numCoins(0), numStars(0), activatedSquare(false), activatedBaddie(false), hasVortex(false) { setForcingDir(-1); }
    int getPlayerNumber();
    void addStar() { numStars++; }
    int getNumStars() { return numStars; }
    void setNumStars(int stars) { numStars = stars; }
    void resetNumCoins(int c) { numCoins += c; }
    int getNumCoins() { return numCoins; }
    void setNumCoins(int c) { numCoins = c; }
    void setActivatedSquare(bool b) { activatedSquare = b; }
    bool hasActivatedSquare() { return activatedSquare; }
    void setActivatedBaddie(bool b) { activatedBaddie = b; }
    bool hasActivatedBaddie() { return activatedBaddie; }
    std::string vortexMessage();
    void giveVortex() { hasVortex = true; }
    //bool getHasVortex() { return hasVortex; }
    virtual void doActionWhileWaiting();
    virtual bool canDoActionAtFork();
    virtual void doMove();
    virtual void decideMove();
private:
    int playerNumber;
    int numCoins;
    int numStars;
    bool hasVortex;
    bool activatedSquare;
    bool activatedBaddie;
};

class Peach : public Avatar {
public:
    Peach(StudentWorld* world, int startX, int startY) : Avatar(world, IID_PEACH, SPRITE_WIDTH * startX, SPRITE_HEIGHT * startY, 1) {};
private:
};

class Yoshi : public Avatar {
public:
    Yoshi(StudentWorld* world, int startX, int startY) : Avatar(world, IID_YOSHI, SPRITE_WIDTH * startX, SPRITE_HEIGHT * startY, 2){};
private:
};

class Baddie : public MovingActor {
public:
    Baddie(StudentWorld* world, int imageID, int startX, int startY) : MovingActor(world, imageID, startX, startY, 0, 0), pauseCounter(180), travelDistance(0) {}
    int getPauseCounter() { return pauseCounter; }
    void setPauseCounter(int i) {pauseCounter = i; }
    void decrementPauseCounter() { pauseCounter--; }
    virtual void decideMove();
    virtual bool canDoActionAtFork();
private:
    int pauseCounter;
    int travelDistance;
};

class Bowser : public Baddie {
public:
    Bowser(StudentWorld* world, int startX, int startY) : Baddie(world, IID_BOWSER, SPRITE_WIDTH * startX, SPRITE_HEIGHT * startY) {}
    virtual void doActionWhileWaiting();
    //virtual bool canDoActionAtFork();
    virtual void doMove();
private:
};

class Boo : public Baddie {
public:
    Boo(StudentWorld* world, int startX, int startY) : Baddie(world, IID_BOO, SPRITE_WIDTH * startX, SPRITE_HEIGHT * startY) {}
    virtual void doActionWhileWaiting();
    //virtual bool canDoActionAtFork(){return true;}
    virtual void doMove();
private:
};

class Square : public Actor {
public:
    Square(StudentWorld* world, int imageID, int startX, int startY, int depth) : Actor(world, imageID, SPRITE_WIDTH * startX, SPRITE_WIDTH * startY, 0, depth) {}
private:
};

class CoinSquare : public Square {
public:
    CoinSquare(StudentWorld* world, int imageID, int startX, int startY) : Square(world, imageID, startX, startY, 1), active(true), isNewPlayer(true) {}
    bool isActive() { return active; }
private:
    bool active;
    bool isNewPlayer;
};

class BlueCoinSquare : public CoinSquare {
public:
    BlueCoinSquare(StudentWorld* world, int startX, int startY) : CoinSquare(world, IID_BLUE_COIN_SQUARE, startX, startY) {}
    virtual void doSomething();
private:
};

class RedCoinSquare : public CoinSquare {
public:
    RedCoinSquare(StudentWorld* world, int startX, int startY) : CoinSquare(world, IID_RED_COIN_SQUARE, startX, startY) {}
    virtual void doSomething();
private:
};

class StarSquare : public Square {
public:
    StarSquare(StudentWorld* world, int startX, int startY) : Square(world, IID_STAR_SQUARE, startX, startY, 1) {}
    virtual void doSomething();
private:
};

class DirectionSquare : public Square {
public:
    DirectionSquare(StudentWorld* world, int imageID, int startX, int startY, int dir) : Square(world, imageID, startX, startY, 1) { setDirection(dir);
        setForcingDir(dir);
    }
    virtual void doSomething();
private:
};

class UpDirectionSquare : public DirectionSquare {
public:
    UpDirectionSquare(StudentWorld* world, int startX, int startY) : DirectionSquare(world, IID_DIR_SQUARE, startX, startY, up) {}
private:
};

class DownDirectionSquare : public DirectionSquare {
public:
    DownDirectionSquare(StudentWorld* world, int startX, int startY) : DirectionSquare(world, IID_DIR_SQUARE, startX, startY, down) {}
private:
};

class LeftDirectionSquare : public DirectionSquare {
public:
    LeftDirectionSquare(StudentWorld* world, int startX, int startY) : DirectionSquare(world, IID_DIR_SQUARE, startX, startY, left) {}
private:
};

class RightDirectionSquare : public DirectionSquare {
public:
    RightDirectionSquare(StudentWorld* world, int startX, int startY) : DirectionSquare(world, IID_DIR_SQUARE, startX, startY, right) {}
private:
};

class BankSquare : public Square {
public:
    BankSquare(StudentWorld* world, int startX, int startY) : Square(world, IID_BANK_SQUARE, startX, startY, 1), bankBalance(0) {}
    virtual void doSomething();
    void resetBankBalance(int c) { bankBalance += c; }
    int getBankBalance() { return bankBalance; }
private:
    int bankBalance;
};

class EventSquare : public Square {
public:
    EventSquare(StudentWorld* world, int startX, int startY) : Square (world, IID_EVENT_SQUARE, startX, startY, 1) {}
    virtual void doSomething();
private:
};

class Vortex : public MovingActor {
public:
    Vortex(StudentWorld* world, int startX, int startY, int destX, int destY) : MovingActor(world, IID_VORTEX, startX, startY, 0, 0), endX(destX), endY(destY) {}
    virtual void doSomething();
private:
    int endX;
    int endY;
};

#endif // ACTOR_H_
