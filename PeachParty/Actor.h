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
    Avatar(StudentWorld* world, int imageID, int startX, int startY, int number) : Actor(world, imageID, startX, startY, 0, 0), playerNumber(number), ticks_to_move(0), waitingToRoll(true) {};
    virtual void doSomething() {std::cerr << "do something\n";};
    int getPlayerNumber();
private:
    int playerNumber;
    int ticks_to_move;
    bool waitingToRoll;
};

class Peach : public Avatar {
public:
    Peach(StudentWorld* world, int startX, int startY) : Avatar(world, IID_PEACH, SPRITE_WIDTH * startX, SPRITE_HEIGHT * startY, 1){};
    virtual void doSomething() {std::cerr << "do something\n";};
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
    virtual void doSomething() {std::cerr << "do something\n";};
private:
};

class CoinSquare: public Square {
public:
    CoinSquare(StudentWorld* world, int startX, int startY) : Square(world, IID_BLUE_COIN_SQUARE, startX, startY, 0) {}
    virtual void doSomething() {std::cerr << "do something\n";};
private:
};

#endif // ACTOR_H_
