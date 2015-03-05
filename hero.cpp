#include"hero.h"

hero::hero()
{

}

hero::~hero()
{

}

hero* hero::createWithLayerAndPic(Layer* scene,const std::string &picName,heroType type/* = HERO*/)
{
	hero* mHero = new hero();

	if(mHero && mHero->initWithLayerAndPic(scene,picName,type))
	{
		return mHero;
	}
	else
	{
		delete mHero;
		mHero = NULL;
		return NULL;
	}
}

bool hero::initWithLayerAndPic(Layer* scene,const std::string &picName,heroType type/* = HERO*/)
{
    mLayer = scene;
	this->initWithFile(picName);
	currentType = type;

	if(currentType == HERO)	
	{
		heartPoint = HPUPLIMIT_HERO;
		magicPoint = 20;
		attackPoint = 20;
		defencePoint = 25;

		Size cSize = this->getContentSize();
		Sprite* shiledSprite = Sprite::create("block/skill_3.png");
		shiledSprite->setPosition(cSize.width/2+83,cSize.height/2-40);
		shiledSprite->setScale(1.2f);
		this->addChild(shiledSprite,98);
		defencePointShower = LabelTTF::create(StringUtils::format("%d",defencePoint),"fonts/Porto 400.ttf",65);
		defencePointShower->setPosition(cSize.width/2+83,cSize.height/2-40);
		this->addChild(defencePointShower,99);

	}
	if(currentType == MONSTER)
	{
		heartPoint = HPUPLIMIT_MONSTER;
		magicPoint = 15;
		attackPoint = 3;
		defencePoint = 10;		
	}

	hurtingTime = 5.0f;
	initHeartPointShower();
	return true;
}

void hero::initHeartPointShower()
{
	if(currentType == HERO)
	{
		Size contentSize = this->getContentSize();
		heartPointShower = ProgressTimer::create(Sprite::create("HeartPoint.png"));
		heartPointShower->setType(ProgressTimer::Type::BAR);
		heartPointShower->setPercentage(0);
		heartPointShower->setBarChangeRate(Vec2(0,1));
		heartPointShower->setMidpoint(Vec2(contentSize.width/2,0));
		heartPointShower->setPosition(Vec2(contentSize.width/2,contentSize.height/2));
		this->addChild(heartPointShower,97);
	}

	if(currentType == MONSTER)
	{
		Vec2 monsterPos = this->getPosition();
		Sprite* bottom = Sprite::create("HP_Bottom.png");
		bottom->setPosition(250,400);
		bottom->setScale(1.2f);
		mLayer->addChild(bottom,101);

		Sprite* tmp = Sprite::create("HP.png");
		heartPointShower = ProgressTimer::create(tmp);
		heartPointShower->setType(ProgressTimer::Type::BAR);
		heartPointShower->setPercentage(100);
		heartPointShower->setBarChangeRate(Vec2(1,0));
		heartPointShower->setMidpoint(Vec2(monsterPos.x - 90,monsterPos.y));
		heartPointShower->setPosition(250,400);
		heartPointShower->setScale(1.2f);
		mLayer->addChild(heartPointShower,102);
	}
}

void hero::updateHeartPoint(int pre)
{
	if(currentType == HERO)
	{
		ProgressTo* progressMove = ProgressTo::create(0.5f,heartPoint*100/HPUPLIMIT_HERO);
		heartPointShower->runAction(progressMove);

		if(heartPoint < 50)
			heartPointShower->runAction(Blink::create(1.0f,3));
	}
	else if(currentType == MONSTER)
	{
		ProgressTo* progressMove = ProgressTo::create(0.5f,heartPoint*100/HPUPLIMIT_MONSTER);
		heartPointShower->runAction(progressMove);
	}
}

void hero::damageOverTime(hero* attacker)
{

}

void hero::addDefencePoint(int point/* = 5*/)
{
	if(currentType == HERO)
	{
		defencePoint += point;
		defencePointShower->setString(StringUtils::format("%d",defencePoint));
	}
	else
	{
		return;
	}
}

void hero::addHeartPoint(int point/* = 5*/)
{
	if(currentType == HERO)
	{
		heartPoint += point;
		if(heartPoint >= 100)
			heartPoint = 100;
	}
	else
	{
		return;
	}
}

void hero::reset()
{
	this->setHeartPoint(100);
	this->setMagicPoint(100);
	this->setAttackPoint(100);
	this->setDefencePoint(30);
}

heroType hero::getType() const
{
	return currentType;
}

void hero::attackCharacter(hero* target,int mode /* = 1	*/)
{
	int targetHp = target->getHeartPoint();
	int targetMp = target->getMagicPoint();
	int targetAp = target->getAttackPoint();
	int targetDp = target->getDefencePoint();
	int preHp = targetHp;
	//physics attack
	if(mode == 1)
	{
		if(target->getType() == HERO)
		{
			int dpDamage = targetDp - attackPoint;
			if(dpDamage <= 0)
			{
				targetDp = 0;
				target->setDefencePoint(targetDp);
				//defencePointShower->setString(StringUtils::format("%d",targetDp));
				target->addDefencePoint(0);
				targetHp += dpDamage;
			}
			else if(dpDamage > 0)
			{
				target->addDefencePoint(-attackPoint);
			} 
		}
		else if(target->getType() == MONSTER)
		{
			int dpDamage = attackPoint - targetDp;
			if(dpDamage <= 0)
				dpDamage = 0;
			targetHp -= dpDamage;
		}
	}
	//magic attack
	else if(mode == 2)
		targetHp = targetHp - magicPoint;

	if(targetHp <= 0)
		targetHp = 0;

	target->setHeartPoint(targetHp);
	target->updateHeartPoint(preHp);
	if(target->currentType == MONSTER)
		target->runAction(EaseBounceInOut::create(Sequence::create(
											RotateBy::create(0.8f,Vec3(0,-15,0))
											,RotateBy::create(0.8f,Vec3(0,15,0))
											,NULL)));
}

void hero::updateMonsterStatus(float dt)
{	
	//Sequence* moveAround = Sequence::create(MoveBy::create(0.4,Vec2(20,0)),MoveBy::create(0.4f,Vec2(-20,0)),NULL);
	//hero* monster = (hero*)mLayer->getChildByTag(999);
	////mLayer->getChildByTag(999)->runAction(moveAround);
 //   if(monster != NULL)
	//{
	//	monster->runAction(moveAround);
	//	CCLOG("Run Successfully..");
	//}
	//else
	//{
	//	CCLOG("ERROR...");
	//	return;
	//}
}


