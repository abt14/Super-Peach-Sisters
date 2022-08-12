// Name: Annie Tran
// UID: 905717479
// Project 3: Super Peach Sisters
// StudentWorld.cpp

#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_peach = nullptr; //set to nullptr because peach will be initailize in init()
    v_actors.clear();
}

StudentWorld:: ~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    //must load up first level data file using provided Level class & create block, peach obejcts, and inert them into data structures
    //positions of objects must be set based on contents of the Level data file
    finishedlevel=false;
    finishedGame=false;
    
    Level lev(assetPath());
    int levelNumber = getLevel();
    ostringstream leveloss;
    leveloss.fill('0');
    leveloss << "level" << setw(2) << levelNumber << ".txt";
    string level_file = leveloss.str();
    
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found || levelNumber>99)
    {
        cerr << "Could not find data file" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_fail_bad_format)
    {
        cerr << "file is improperly formatted" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level" << endl;
        Level::GridEntry ge;
        for (int x=0; x < GRID_WIDTH; x++)
        {
            for (int y=0; y<GRID_HEIGHT; y++)
            {
                ge = lev.getContentsOf(x, y);
                switch (ge)
                {
                    case Level::peach:
                    {
                        m_peach = new Peach(this,x*SPRITE_WIDTH,y*SPRITE_HEIGHT); //initialize Peach
                        break;
                    }
                    case Level::block:
                        v_actors.push_back(new Block(this,x*SPRITE_WIDTH,y*SPRITE_HEIGHT, false, false, false)); //initialize Block
                        break;
                    case Level::star_goodie_block:
                        v_actors.push_back(new Block(this,x*SPRITE_WIDTH,y*SPRITE_HEIGHT, true, false, false)); //initialize Star Block
                        break;
                    case Level::flower_goodie_block:
                        v_actors.push_back(new Block(this,x*SPRITE_WIDTH,y*SPRITE_HEIGHT, false, false, true)); //initialize flower block
                        break;
                    case Level::mushroom_goodie_block:
                        v_actors.push_back(new Block(this,x*SPRITE_WIDTH,y*SPRITE_HEIGHT, false, true, false)); //initialize mushroom block
                        break;
                    case Level:: flag:
                        v_actors.push_back(new Flag(this,x*SPRITE_WIDTH,y*SPRITE_HEIGHT)); //flag
                        break;
                    case Level::pipe:
                        v_actors.push_back(new Pipe(this,x*SPRITE_WIDTH,y*SPRITE_HEIGHT)); //initialize pipes
                        break;
                    case Level::mario:
                        v_actors.push_back(new Mario(this,x*SPRITE_WIDTH,y*SPRITE_HEIGHT)); //initialize pipes
                        break;
                    case Level::goomba:
                        v_actors.push_back(new Goomba(this,x*SPRITE_WIDTH,y*SPRITE_HEIGHT,randDirection())); //initialize pipes
                        break;
                    case Level::koopa:
                        v_actors.push_back(new Koopa(this,x*SPRITE_WIDTH,y*SPRITE_HEIGHT, randDirection())); //initialize pipes
                        break;
                    case Level::piranha:
                        v_actors.push_back(new Piranha(this,x*SPRITE_WIDTH,y*SPRITE_HEIGHT, randDirection())); //initialize pipes
                        break;
                    default:
                        break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    m_peach->doSomething();
    vector<Actor*>::iterator it;
    for (it = v_actors.begin(); it != v_actors.end(); it++)
    {
        if ((*it)->isAlive())
            (*it) -> doSomething();
        
        if (!(m_peach->isAlive()))
        {
            playSound(SOUND_PLAYER_DIE);
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        if (finishedlevel)
        {
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
        if (finishedGame)
        {
            playSound(SOUND_GAME_OVER);
            return GWSTATUS_PLAYER_WON;
        }
    }
    //delete actors that are dead
    for (it = v_actors.begin(); it != v_actors.end(); it++)
    {
        if(!(*it)->isAlive())
        {
            Actor *temp = *it;
            delete temp;
            it = v_actors.erase(it);
            it = v_actors.begin();
        }
            
    }
    //update the status text
    ostringstream oss;
    oss << "Lives: " << getLives() << "  Level: " << getLevel() << "  Points: " << getScore();
    if (getPeach()->hasStarPower())
        oss << " StarPower!";
    if (getPeach()->hasShootPower())
        oss << " ShootPower!";
    if (getPeach()->hasJumpPower())
        oss << " JumpPower!";
    string s = oss.str();
    setGameStatText(s);

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_peach;
    for (int i=0; i<v_actors.size(); i++)
        delete v_actors[i];
    v_actors.clear();
}

//OTHER FUNCTIONS
Actor* StudentWorld::overlap(int x, int y, int& actorType, Actor* a)
{
    if (a!=m_peach)
    {
        if (abs(m_peach->getX()-x) < SPRITE_WIDTH && abs(m_peach->getY()-y) < SPRITE_HEIGHT) //overlap with peach
        {
            actorType = 1;    //1 is a peach type
            return m_peach;
        }
    }
    vector<Actor*>::iterator it;
    for (it = v_actors.begin(); it != v_actors.end(); it++)
    {
        if (abs((*it)->getX()-x) < SPRITE_WIDTH && abs((*it)->getY()-y) < SPRITE_HEIGHT)    //return object that is overlapping with caller
        {
            if ((*it)->damageable())
                actorType = 2;  //2 is a damageable actor
            else
                actorType = 3;  //3 is any other actor that overlaps with
            return *it;
        }
    }
    return a;   //if not overlapping then return the original caller; 0 is original
}

bool StudentWorld::blockedMovement(int x, int y)
{
    vector<Actor*>::iterator it;
    for (it = v_actors.begin(); it != v_actors.end(); it++)
    {
        if ((*it)->blocksActor())
        {
            if (abs((*it)->getX()-x) < SPRITE_WIDTH && abs((*it)->getY()-y) < SPRITE_HEIGHT)
                return true;
        }
    }
    return false;
}

//PIRANHA FUNCTION

bool StudentWorld::peachIsLevel(int y) //piranha function
{
    if(abs(m_peach->getY()-y) <= 1.5*SPRITE_HEIGHT)
        return true;
    else
        return false;
}

bool StudentWorld::peachInRange(int x)
{
    if (abs(m_peach->getX()-x) < 8*SPRITE_WIDTH)
        return true;
    else
        return false;
}
    

//NEWLY CREATED OBJECTS
void StudentWorld::addStarGoodie(Actor *block)
{
    v_actors.push_back(new Star(this, block->getX(),block->getY()+8));
}

void StudentWorld::addMushroomGoodie(Actor *block)
{
    v_actors.push_back(new Mushroom(this, block->getX(),block->getY()+8));
}

void StudentWorld::addFlowerGoodie(Actor *block)
{
    v_actors.push_back(new Flower(this, block->getX(),block->getY()+8));
}

void StudentWorld::addPeachFireball(Actor *shooter)
{
    if (shooter->getDirection() == 0)
        v_actors.push_back(new Peach_Fired(this, (shooter->getX()+4), shooter->getY(), shooter->getDirection()));
    else
        v_actors.push_back(new Peach_Fired(this, shooter->getX()-4, shooter->getY(), shooter->getDirection()));
}

void StudentWorld::addPiranhaFireball(Actor* shooter)
{
    v_actors.push_back(new Piranha_Fired(this, shooter->getX(), shooter->getY(), shooter->getDirection()));
}
void StudentWorld::addShell(Actor *block)
{
    v_actors.push_back(new Shell(this, block->getX(), block->getY(),block->getDirection()));
}

//NON MEMBER FUNCTION
//modify the int direciton
int randDirection()
{
    int n = randInt(0, 1);
    if (n==0)
        return 0;
    else
        return 180;
}
