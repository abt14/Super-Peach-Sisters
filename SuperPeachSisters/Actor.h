// Name: Annie Tran
// UID: 905717479
// Project 3: Super Peach Sisters
// Actor.h

#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

//=======================================PARENT BASE CLASS=======================================
class Actor : public GraphObject
{
public:
    //constructor
    Actor(StudentWorld *world, int imageID, int startX, int startY, int dir = 0, int depth = 0, double size = 1.0): GraphObject(imageID, startX, startY, dir, depth, size), m_studentWorld(world), m_alive(true){}
    
    //VIRTUAL FUNCTIONS
    virtual void doSomething() = 0;                         //each class should have its own unique doSomething method
    virtual void bonk()=0;                                  //each class should have its own unique bonk method
    virtual bool blocksActor() const {return false;}        //only block and pipe return true;
    virtual bool damageable() const {return false;}         //only peach and enemies return true
    virtual void damage()   {return;}                       //peach and enemies will have dif damage() method
    
    //NON VIRTUAL FUNCTIONS
    void setAliveStatus(bool alive) {m_alive = alive;}
    bool isAlive() const {return m_alive;}
    StudentWorld* getWorld() const {return m_studentWorld;} //allows class to access functions inside StudentWorld
    
private:
    StudentWorld *m_studentWorld;
    bool m_alive;
};

//-------------------------------------PEACH-------------------------------------
class Peach: public Actor
{
public:
    Peach(StudentWorld *world, int startX, int startY): Actor(world, IID_PEACH, startX, startY, 0, 0, 1.0), m_hitPoint(1), m_isTempInvincible(false), m_starPower(false), m_shootPower(false), m_jumpPower(false), remaining_jump_distance(0), activeJump(false), gameticks(0), rechargeTime(0), invincibilityTicks(0){}
    
    virtual void doSomething();
    virtual bool damageable() const {return true;}
    virtual void damage();
    virtual void bonk ();
    
        //set functions
    void setHitPoint(int health) {m_hitPoint=health;}
    void setTempInvincibility(bool status) {m_isTempInvincible=status;}
    void setStarPower(bool status, int duration) {m_starPower=status; gameticks = duration;}
    void setShootPower(bool status) {m_shootPower=status;}
    void setJumpPower(bool status) {m_jumpPower=status;}
        //get functions
    bool hasTempInvincibility() const {return m_isTempInvincible;}
    bool hasStarPower() const {return m_starPower;}
    bool hasShootPower() const {return m_shootPower;}
    bool hasJumpPower() const {return m_jumpPower;}
    
private:
    int m_hitPoint;
    bool m_isTempInvincible;
    bool m_starPower;
    bool m_shootPower;
    bool m_jumpPower;
    int remaining_jump_distance;
    bool activeJump;
    int gameticks;
    int rechargeTime;
    int invincibilityTicks;
};

//**********************************BASE CLASS: OBSTACLE***********************************
class Obstacle: public Actor
{
public:
    Obstacle(StudentWorld *world, int imageID, int startX, int startY): Actor(world, imageID, startX, startY, 0, 2, 1) {}
    virtual void doSomething() {return;}                //block & pipe do nothing
    virtual bool blocksActor() const {return true;}     //block & pipe block movement
};
//-------------------------------------BLOCK-----------------------------------
class Block: public Obstacle
{
public:
    Block(StudentWorld *world, int startX, int startY, bool star, bool mushroom, bool flower): Obstacle(world, IID_BLOCK, startX, startY), starBlock(star), mushroomBlock(mushroom), flowerBlock(flower){}

    virtual void bonk();
    
        //set functions
    void isStarBlock(bool hasPower) {starBlock = hasPower;}
    void isMushroomBlock(bool hasPower) {mushroomBlock = hasPower;}
    void isFlowerBlock(bool hasPower) {flowerBlock = hasPower;}
    bool hasGoodie();   //check for if block has released goodie
    
private:
    bool starBlock;
    bool mushroomBlock;
    bool flowerBlock;
};

//-------------------------------------PIPE-------------------------------------
class Pipe: public Obstacle //group this class with Block
{
public:
    Pipe (StudentWorld *world, int startX, int startY): Obstacle(world, IID_PIPE, startX, startY){}
    virtual void bonk () {return;}      //pipe does nothing when bonked - do I  move this to Obstacle class?
};

//**********************************BASE CLASS: LEVEL ENDER**********************************
class LevelEnder: public Actor
{
public:
    LevelEnder(StudentWorld *world, int imageID, int startX, int startY): Actor(world, imageID, startX, startY, 0, 1, 1) {}
    virtual void doSomething();
    virtual void bonk () {return;}
    virtual void doSomethingLevelEnderSubClass() = 0; //pure virtual b/c flag and mario do different things
};
//-------------------------------------FLAG-------------------------------------
class Flag: public LevelEnder
{
public:
    Flag(StudentWorld *world, int startX, int startY): LevelEnder(world, IID_FLAG, startX, startY){}
private:
    virtual void doSomethingLevelEnderSubClass();
};

//-------------------------------------MARIO-------------------------------------
class Mario: public LevelEnder
{
public:
    Mario(StudentWorld *world, int startX, int startY): LevelEnder(world, IID_MARIO, startX, startY){}
private:
    virtual void doSomethingLevelEnderSubClass();
};

//**********************************BASE CLASS: GOODIE**********************************
class Goodie: public Actor
{
public:
    Goodie(StudentWorld *world, int imageID, int startX, int startY): Actor(world, imageID, startX, startY, 0, 1, 1){}
    virtual void doSomething();
    virtual void bonk () {return;}                      //nothing happens when bonked
    virtual void doSomethingGoodieSubClass() = 0;       //pure virtual b/c each goodie does something different
};
//-------------------------------------FLOWERS-------------------------------------
class Flower: public Goodie
{
public:
    Flower(StudentWorld *world, int startX, int startY): Goodie(world, IID_FLOWER, startX, startY){}
private:
    virtual void doSomethingGoodieSubClass();
};
//-------------------------------------MUSHROOMS-------------------------------------
class Mushroom: public Goodie
{
public:
    Mushroom(StudentWorld *world, int startX, int startY): Goodie(world, IID_MUSHROOM, startX, startY){}
private:
    virtual void doSomethingGoodieSubClass();
};
//-------------------------------------STAR-------------------------------------
class Star: public Goodie
{
public:
    Star(StudentWorld *world, int startX, int startY): Goodie(world, IID_STAR, startX, startY){}
private:
    virtual void doSomethingGoodieSubClass();
};

//**********************************BASE CLASS: PROJECTILE*********************************
class Projectile:public Actor
{
public:
    Projectile(StudentWorld *world, int imageID, int startX, int startY, int dir): Actor(world, imageID, startX, startY, dir, 1, 1) {}
    virtual void doSomething();
    virtual void bonk () {return;}
    virtual bool doSomethingProjectileSubClass();         //peach fire and shell behave the same -> define again for pirnaha fire
};
//-------------------------------------PIRANHA-FIRED FIREBALLS-------------------------------------
class Piranha_Fired: public Projectile
{
public:
    Piranha_Fired(StudentWorld *world, int startX, int startY, int dir): Projectile(world, IID_PIRANHA_FIRE, startX, startY, dir){}
private:
    virtual bool doSomethingProjectileSubClass();
};

//-------------------------------------PEACH-FIRED FIREBALLS-------------------------------------
class Peach_Fired: public Projectile
{
public:
    Peach_Fired(StudentWorld *world, int startX, int startY, int dir): Projectile(world, IID_PEACH_FIRE, startX, startY, dir){}
};
//-------------------------------------SHELL-------------------------------------
class Shell: public Projectile
{
public:
    Shell(StudentWorld *world, int startX, int startY, int dir): Projectile(world, IID_SHELL, startX, startY, dir){}
};

//************************************BASE CLASS: ENEMIES**********************************
class Enemy:public Actor
{
public:
    Enemy(StudentWorld *world, int imageID, int startX, int startY, int dir): Actor(world, imageID, startX, startY, dir, 0,1) {}
    virtual void doSomething();     //only works for goomba and koopa -> must define dif doSomething() for piranha
    virtual bool damageable() const { return true; }
    virtual void damage();
    virtual void bonk ();
    virtual void bonkEnemySubClass() {return;}
};

//-------------------------------------GOOMBA-------------------------------------
class Goomba: public Enemy
{
public:
    Goomba(StudentWorld *world, int startX, int startY,int dir): Enemy(world, IID_GOOMBA, startX, startY, dir){}
};
//-------------------------------------KOOPA-------------------------------------
class Koopa: public Enemy
{
public:
    Koopa(StudentWorld *world, int startX, int startY, int dir): Enemy(world, IID_KOOPA, startX, startY, dir){}
private:
    virtual void bonkEnemySubClass();

};
//-------------------------------------PIRANHA-------------------------------------
class Piranha: public Enemy
{
public:
    Piranha(StudentWorld *world, int startX, int startY, int dir): Enemy(world, IID_PIRANHA, startX, startY, dir), firingDelay(0){}
    virtual void doSomething();
private:
    int firingDelay;
};

#endif // ACTOR_H_
