#ifndef HERO_H
#define HERO_H
#include "cocos2d.h"
#include "block.h"
#include "gameScene.h"
#include "attackPattern.h"
USING_NS_CC;

typedef enum heroType
{
	HERO,MONSTER
}heroType;

#define HPUPLIMIT_HERO 100
#define HPUPLIMIT_MONSTER 1000

class block;
class gameScene;
class attackPattern;

class hero: public Sprite
{
public:
	hero();
	~hero();
	static hero* createWithLayerAndPic(Layer* scene,const std::string &picName,heroType type = HERO);
	bool initWithLayerAndPic(Layer* scene,const std::string &picName,heroType type = HERO);
	CC_SYNTHESIZE(int,heartPoint,HeartPoint);
	CC_SYNTHESIZE(int,magicPoint,MagicPoint);
	CC_SYNTHESIZE(int,attackPoint,AttackPoint);
	CC_SYNTHESIZE(int,defencePoint,DefencePoint);
	heroType getType() const;
	//attackMode
	bool changeMonsterActionPattern(attackPattern* newPattern);
	void attackByPattern();
	//
	void initHeartPointShower();
	void updateHeartPoint(int pre);
	void updateMonsterStatus(float dt);
	void attackCharacter(hero* chara,int mode = 1);
	void addDefencePoint(int point = 5);
	void addHeartPoint(int hpoint = 5);
	void damageOverTime(hero* attacker);
	void reset();

protected:
	Layer* mLayer;
	LabelTTF* defencePointShower;
	heroType currentType;
	ProgressTimer* heartPointShower;
	float hurtingTime;

};


#endif
