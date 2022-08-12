// Name: Annie Tran
// UID: 905717479
// Project 4: Super Peach Sisters
// Actor.cpp

#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    //check overlap and block
    Actor* overlap(int x, int y, int& actorType, Actor* a); //check for overlap
    bool blockedMovement(int x, int y); //check for blocking
    bool peachIsLevel(int y); //piranha function
    bool peachInRange(int x); //piranha function
    //create objects
    void addStarGoodie(Actor* block);
    void addMushroomGoodie(Actor* block);
    void addFlowerGoodie(Actor* block);
    void addPeachFireball(Actor* shooter);
    void addPiranhaFireball(Actor* shooter);
    void addShell(Actor* block);
    //check for portal (aka flags and mario)
    void levelCompleted(bool status) {finishedlevel = status;}
    void GameCompleted(bool status) {finishedGame = status;}

//return a pointer to peach to access peach public functions
    Peach* getPeach() {return m_peach;}
    
private:
    Peach *m_peach;
    std::vector<Actor*> v_actors;
    bool finishedlevel;
    bool finishedGame;
};

int randDirection();

#endif // STUDENTWORLD_H_
