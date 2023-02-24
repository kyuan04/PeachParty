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
    Avatar(StudentWorld* world, int imageID, int startX, int startY, int number) : Actor(world, imageID, startX, startY, 0, 0), playerNumber(1), ticksToMove(0), waitingToRoll(true){};
    int getPlayerNumber();
    void setMoveDirection(int dir) { moveDirection = dir; };
    int getMoveDirection() { return moveDirection; };
    virtual void doSomething();
private:
    int playerNumber;
    int ticksToMove;
    bool waitingToRoll;
    int moveDirection;
};

class Peach : public Avatar {
public:
    Peach(StudentWorld* world, int startX, int startY) : Avatar(world, IID_PEACH, SPRITE_WIDTH * startX, SPRITE_HEIGHT * startY, 1) {};
private:
};

//class Yoshi : public Avatar {
//public:
//    Yoshi(StudentWorld* world, int startX, int startY) : Avatar(world, IID_YOSHI, SPRITE_WIDTH * startX, SPRITE_HEIGHT * startY, 1){};
//    virtual void doSomething() {std::cerr << "do something\n";};
//private:
//};

class Square : public Actor {
public:
    Square(StudentWorld* world, int imageID, int startX, int startY, int depth) : Actor(world, imageID, SPRITE_WIDTH * startX, SPRITE_WIDTH * startY, 0, depth) {}
private:
};

class CoinSquare: public Square {
public:
    CoinSquare(StudentWorld* world, int imageID, int startX, int startY) : Square(world, imageID, startX, startY, 0), isAlive(true) {}
    virtual void doSomething();
private:
    bool isAlive;
};

class BlueCoinSquare: public CoinSquare {
public:
    BlueCoinSquare(StudentWorld* world, int startX, int startY) : CoinSquare(world, IID_BLUE_COIN_SQUARE, startX, startY) {};
private:
};

#endif // ACTOR_H_
