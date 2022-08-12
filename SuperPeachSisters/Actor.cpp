// Name: Annie Tran
// UID: 905717479
// Project 4: Super Peach Sisters
// Actor.cpp

#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//-------------------------------------PEACH-------------------------------------
void Peach::doSomething()
{
    int actorType = 0;
    Actor *temp;
    //1. check if Peach is alive
    if (!isAlive())
        return;
    //2. check if currently invincible (aka has star power)
    if (m_starPower)
    {
        if (m_starPower)
            gameticks--;
        if (gameticks==0)
            m_starPower=false;
    }
    //3.check if currently temporarily invincible
    if (invincibilityTicks>0)
        invincibilityTicks--;
    if (invincibilityTicks==0)
        m_isTempInvincible=false;
    
    //4. check if in recharge mode (Shoot power)
    if (m_shootPower)
    {
        if (rechargeTime > 0)
            rechargeTime--;
    }

    
    //5. check for overlap with other object
    {
        temp = getWorld()->overlap(getX(), getY(), actorType, this);
        if (actorType != 0)    //if Peach currently overlaps with something
            temp->bonk();
    }
    
    //6. is Peach jumping?
    if (activeJump && remaining_jump_distance>0)
    {
        temp = getWorld()->overlap(getX(), getY()+4, actorType, this);
        if (temp->blocksActor())
        {
            temp->bonk();
            remaining_jump_distance = 0;
            activeJump = false;
        }
        else
        {
                moveTo(getX(), getY()+4);
                remaining_jump_distance--;
        }
    }
    //7. if Peach is not actively jumping - then is she falling?
    else
    {
        if (getWorld()->blockedMovement(getX(),getY()) || getWorld()->blockedMovement(getX(),getY()-1) || getWorld()->blockedMovement(getX(),getY()-2) || getWorld()->blockedMovement(getX(),getY()-3));
        else
            moveTo(getX(), getY()-4);
    }
    
    //8. check for keystroke from user
    int key;
    if (getWorld()->getKey(key)) //if user hits a valid key
    {
        switch (key)
        {
            case KEY_PRESS_LEFT:
            {
                setDirection(180);
                temp = getWorld()->overlap(getX()-4, getY(), actorType, this);
                if (temp->blocksActor())
                {
                    temp->bonk();
                    break;
                }
                else
                    moveTo(getX()-4, getY());
                break;
            }
            case KEY_PRESS_RIGHT:
            {
                setDirection(0);
                temp = getWorld()->overlap(getX()+4, getY(), actorType, this);
                if (temp->blocksActor())
                {
                    temp->bonk();
                    break;
                }
                else
                    moveTo(getX()+4, getY());
                break;
            }
            case KEY_PRESS_UP:
            {
                if (getWorld()->blockedMovement(getX(), getY()-1)) //means that there is an object to support Peach's jump
                {
                    activeJump=true;
                    if (m_jumpPower!=true)
                        remaining_jump_distance = 8;
                    else
                        remaining_jump_distance = 12;
                    getWorld()->playSound(SOUND_PLAYER_JUMP);
                }
                break;
            }
            case KEY_PRESS_SPACE:
            {
                if (!m_shootPower)
                    break;
                else if (rechargeTime > 0)
                    break;
                else
                {
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    rechargeTime=8;
                    getWorld()->addPeachFireball(this);
                }
                break;
            }
            default:
                break;
        }
    }
}

void Peach::bonk()
{
    if (hasStarPower()||(invincibilityTicks>0))
        return;
    else
    {
        m_hitPoint--;
        invincibilityTicks=10;
        if (hasShootPower())
            setShootPower(false);
        if (hasJumpPower())
            setJumpPower(false);
        if (m_hitPoint > 0)
            getWorld()->playSound(SOUND_PLAYER_HURT);
        else
            setAliveStatus(false);
    }
}

void Peach::damage()
{
    if (hasStarPower()||(invincibilityTicks>0))
        return;
    else
    {
        m_hitPoint--;
        invincibilityTicks=10;
        if (hasShootPower())
            setShootPower(false);
        if (hasJumpPower())
            setJumpPower(false);
        if (m_hitPoint==0)
            setAliveStatus(false);
    }
}

//**********************************BASE CLASS: OBSTACLE***********************************
//-------------------------------------BLOCK-------------------------------------
bool Block::hasGoodie()
{
    if (starBlock==true || flowerBlock ==true || mushroomBlock==true)
        return true;
    else
        return false;
}

void Block::bonk()
{
    if (hasGoodie()==false)
    {
        getWorld()->playSound(SOUND_PLAYER_BONK);
        return;
    }
    else
    {
        getWorld()->playSound(SOUND_POWERUP_APPEARS);
        //introduce goodie - must be exactly 8 pixels above block that was bonked so FIX THIS!!!!!
        if (starBlock)
        {
            getWorld()->addStarGoodie(this);
            isStarBlock(false);
        }
        else if (mushroomBlock)
        {
            getWorld()->addMushroomGoodie(this);
            isMushroomBlock(false);
        }
        else if (flowerBlock)
        {
            getWorld()->addFlowerGoodie(this);
            isFlowerBlock(false);
        }
    }
}

//**********************************BASE CLASS: LEVELENDER********************************** for flag and Mario
void LevelEnder::doSomething()
{
    if (!isAlive())
        return;
    else
    {
        int actorType = 0;
        getWorld()->overlap(getX(), getY(), actorType, this);
        if (actorType==1) //overlaps with peach
        {
            getWorld()->increaseScore(1000);
            setAliveStatus(false);
            doSomethingLevelEnderSubClass();
        }
    }
    
}
//-------------------------------------FLAG-------------------------------------
void Flag:: doSomethingLevelEnderSubClass()
{
    getWorld()->levelCompleted(true);
}

//-------------------------------------MARIO------------------------------------
void Mario:: doSomethingLevelEnderSubClass()
{
    getWorld()->GameCompleted(true);
}

//**********************************BASE CLASS: GOODIE**********************************
void Goodie::doSomething()
{
    if (!isAlive())
        return;
    //1. check if it overlaps with Peach
    int actorType = 0;
    getWorld()->overlap(getX(), getY(), actorType, this);
    if (actorType==1) //overlaps with peach
    {
        doSomethingGoodieSubClass();            //do something specific to the goodie called
        setAliveStatus(false);
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    //2. Check if object beneath it that would block it from falling 2 pixels down
    else
    {
        if (!getWorld()->blockedMovement(getX(), getY()-2)) //no blocking object
            moveTo(getX(), getY()-2);
    }
    //3. Determine what direction it is facing & try to move in that direction by 2 pixels
    if (getDirection()==0)          //facing right
    {
        if (getWorld()->blockedMovement(getX()+2, getY()))
        {
            setDirection(180);
            return;
        }
        else
            moveTo(getX()+2, getY());
    }
    else if (getDirection()==180)   //facing left
    {
        if (getWorld()->blockedMovement(getX()-2, getY()))
        {
            setDirection(0);
            return;
        }
        else
            moveTo(getX()-2, getY());
    }
}

//-------------------------------------FLOWER-------------------------------------
void Flower::doSomethingGoodieSubClass()
{
    getWorld()->increaseScore(50);
    getWorld()->getPeach()->setShootPower(true);
    getWorld()->getPeach()->setHitPoint(2);
}
//-------------------------------------MUSHROOMS-------------------------------------
void Mushroom::doSomethingGoodieSubClass()
{
    getWorld()->increaseScore(75);
    getWorld()->getPeach()->setJumpPower(true);
    getWorld()->getPeach()->setHitPoint(2);
}
//-------------------------------------STAR------------------------------------
void Star::doSomethingGoodieSubClass()
{
    getWorld()->increaseScore(100);
    getWorld()->getPeach()->setStarPower(true, 150);  //has star power for 150 game ticks
}

//**********************************BASE CLASS: PROJECTILE*********************************
void Projectile::doSomething()
{
    int actorType = 0;
    Actor* temp = getWorld()->overlap(getX(), getY(), actorType, this);
    //1. does it overlap a damageable object
    if (doSomethingProjectileSubClass())
    {
        if (temp->isAlive())
            temp->damage();
        setAliveStatus(false);
        return;
    }
    //2, can it fall downwards 2 pixels?
    else
    {
        if (!getWorld()->blockedMovement(getX(), getY()-2)) //no blocking object
            moveTo(getX(), getY()-2);
    }
    //3. Determine what direction it is facing & try to move in that direction by 2 pixels
    if (getDirection()==0)
    {
        if (getWorld()->blockedMovement(getX()+2, getY()))
        {
            setAliveStatus(false);
            return;
        }
        else
            moveTo(getX()+2, getY());
    }
    else if (getDirection()==180)
    {
        if (getWorld()->blockedMovement(getX()-2, getY()))
        {
            setAliveStatus(false);
            return;
        }
        else
            moveTo(getX()-2, getY());
    }
}

bool Projectile::doSomethingProjectileSubClass()            //for the peach-fired fireballs and the shells
{
    int actorType = 0;
    Actor* temp = getWorld()->overlap(getX(), getY(), actorType, this);
    if (actorType==2)
    {
        if (temp->isAlive())
            return true;
    }
        return false;
}

//-------------------------------------PIRANHA-FIRED FIREBALLS-------------------------------------
bool Piranha_Fired::doSomethingProjectileSubClass()
{
    int actorType = 0;
    getWorld()->overlap(getX(), getY(), actorType, this);
    if (actorType==1)
        return true;
    else
        return false;
}

//************************************BASE CLASS: ENEMIES**********************************
void Enemy::doSomething()
{
    //1. check if alive
    if (!isAlive())
        return;
    int actorType = 0;
    Actor* temp = getWorld()->overlap(getX(), getY(), actorType, this);
    //2. check if overlaps with peach
    if (actorType==1)
    {
        temp->bonk();
        return;
    }
    //3. can goomba/koopa move 1 pixel in current direction without bumping into an object
    if (getDirection()==0)
    {
        if (getWorld()->blockedMovement(getX()+1, getY()))
            setDirection(180);
        else    //will it fall off a ledge?
        {
            if(!getWorld()->blockedMovement(getX()+8, getY()-4))      //WHY IS IT 8 pixels instead of 4???
                setDirection(180);
        }
    }
    else if (getDirection()==180)
    {
        if (getWorld()->blockedMovement(getX()-1, getY()))
            setDirection(0);
        else
        {
            if(!getWorld()->blockedMovement(getX()-8, getY()-4))
                setDirection(0);
        }
    }
    //5. determine what direction and try to move
    if(getDirection()==0)
    {
        if (getWorld()->blockedMovement(getX()+1, getY()))
            return;
        else
            moveTo(getX()+1, getY());
    }
    else if (getDirection()==180)
    {
        if (getWorld()->blockedMovement(getX()-1, getY()))
            return;
        else
            moveTo(getX()-1, getY());
    }
}

void Enemy::bonk()
{
    int actorType = 0;
    getWorld()->overlap(getX(), getY(),actorType, this);
    if (actorType == 1)
    {
        if (getWorld()->getPeach()->hasStarPower())
        {
            getWorld()->playSound(SOUND_PLAYER_KICK);
            getWorld()->increaseScore(100);
            setAliveStatus(false);
            bonkEnemySubClass();
        }
    }
    else
        return;
}

void Enemy::damage()
{
    getWorld()->increaseScore(100);
    setAliveStatus(false);
    bonkEnemySubClass();  //will add a shell if it is a koopa
}

//-------------------------------------KOOPA-------------------------------------
void Koopa::bonkEnemySubClass()
{
    getWorld()->addShell(this);
}

//-------------------------------------PIRANHA------------------------------------
void Piranha::doSomething()
{
    //1. check if alive
    if (!isAlive())
        return;
    //2. cause Piranha's jaws to open and close in place
    increaseAnimationNumber();
    //3. check if it overlaps with Peach
    int actorType = 0;
    Actor* temp = getWorld()->overlap(getX(), getY(), actorType, this);
    if (actorType==1)
    {
        temp->bonk();   //ok
        return;
    }
    //4&5. determine if Peach's y coordinate is withint 1.5*SPRITE_HEIGHT of Piranha's y coordinate
    if (!getWorld()->peachIsLevel(getY()))
        return;
    //6.determine if peach is to left or right
    if (getWorld()->getPeach()->getX() > this->getX())
        setDirection(0);
    else
        setDirection (180);
    //7. check if has firing delay
    if (firingDelay>0)
    {
        firingDelay--;
        return;
    }
    else
    {
        if (getWorld()->peachInRange(getX()))
        {
            getWorld()->addPiranhaFireball(this);
            getWorld()->playSound(SOUND_PIRANHA_FIRE);
            firingDelay=40;
        }
    }
}
