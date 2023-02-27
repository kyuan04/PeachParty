#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
    Actor(StudentWorld* world, int imageID, int startX, int startY, int startDir, int depth) : GraphObject(imageID, startX, startY, startDir, depth), m_world(world) {};
    virtual void doSomething() = 0;
    StudentWorld* getWorld() { return m_world; };
    
private:
    StudentWorld* m_world;
};

class Avatar : public Actor {
public:
    Avatar(StudentWorld* world, int imageID, int startX, int startY, int number) : Actor(world, imageID, startX, startY, 0, 0), playerNumber(number), ticksToMove(0), waitingToRoll(true), numCoins(0), numStars(0) {};
    int getPlayerNumber();
    void setMoveDirection(int dir) { moveDirection = dir; }
    int getMoveDirection() { return moveDirection; }
    int getTicksToMove() { return ticksToMove; }
    int getNumStars() { return numStars; }
    int getNumCoins() { return numCoins; }
    virtual void doSomething();
private:
    int playerNumber;
    int ticksToMove;
    bool waitingToRoll;
    int moveDirection;
    int numCoins;
    int numStars;
    //make vortex projectile variable? p. 28
    bool hasVortex;
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

class Square : public Actor {
public:
    Square(StudentWorld* world, int imageID, int startX, int startY, int depth) : Actor(world, imageID, SPRITE_WIDTH * startX, SPRITE_WIDTH * startY, 0, depth) {}
private:
};

class CoinSquare : public Square {
public:
    CoinSquare(StudentWorld* world, int imageID, int startX, int startY) : Square(world, imageID, startX, startY, 1), isAlive(true) {}
    virtual void doSomething();
private:
    bool isAlive;
};

class BlueCoinSquare : public CoinSquare {
public:
    BlueCoinSquare(StudentWorld* world, int startX, int startY) : CoinSquare(world, IID_BLUE_COIN_SQUARE, startX, startY) {}
private:
};

class RedCoinSquare : public CoinSquare {
public:
    RedCoinSquare(StudentWorld* world, int startX, int startY) : CoinSquare(world, IID_RED_COIN_SQUARE, startX, startY) {}
private:
};

class StarSquare : public Square {
public:
    StarSquare(StudentWorld* world, int startX, int startY) : Square(world, IID_STAR_SQUARE, startX, startY, 1) {}
    virtual void doSomething() { return; };
private:
};

class DirectionSquare : public Square {
public:
    DirectionSquare(StudentWorld* world, int imageID, int startX, int startY, int dir) : Square(world, imageID, startX, startY, 1) { setDirection(dir); }
    virtual void doSomething() { return; };
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
    BankSquare(StudentWorld* world, int startX, int startY) : Square(world, IID_BANK_SQUARE, startX, startY, 1) {}
    virtual void doSomething() { return; };
private:
};

class EventSquare : public Square {
public:
    EventSquare(StudentWorld* world, int startX, int startY) : Square (world, IID_EVENT_SQUARE, startX, startY, 1) {}
    virtual void doSomething() { return; };
private:
};

class Baddie : public Actor {
public:
    Baddie(StudentWorld* world, int imageID, int startX, int startY) : Actor(world, imageID, startX, startY, 0, 0) {}
private:
};

class Bowser : public Baddie {
public:
    Bowser(StudentWorld* world, int startX, int startY) : Baddie(world, IID_BOWSER, SPRITE_WIDTH * startX, SPRITE_HEIGHT * startY) {}
    virtual void doSomething() { return; };
private:
};

class Boo : public Baddie {
public:
    Boo(StudentWorld* world, int startX, int startY) : Baddie(world, IID_BOO, SPRITE_WIDTH * startX, SPRITE_HEIGHT * startY) {}
    virtual void doSomething() { return; };
private:
};
#endif // ACTOR_H_
