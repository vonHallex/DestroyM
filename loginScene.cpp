#include "loginScene.h"
#include <ctime>
#include <cstdlib>

Scene* loginScene::createScene()
{
	Scene* scene = Scene::create();
	loginScene* mLayer = loginScene::create();

	scene->addChild(mLayer);
	return scene;
}

bool loginScene::init()
{
	Size vSize = Director::getInstance()->getVisibleSize();
	Sprite* bk = Sprite::create("login/loginBk.png");
	bk->setPosition(vSize.width/2, vSize.height/2);
	this->addChild(bk);
	//添加滑动开启
	bottom = Sprite::create("login/slideBottom.png");
	bottom->setPosition(vSize.width/2, vSize.height/4);
	this->addChild(bottom);
	
	sliderA = Sprite::create("login/slider_1.png");
	sliderA->setPosition(bottom->getPosition());
	this->addChild(sliderA);
	
	sliderB = Sprite::create("login/slider_2.png");
	sliderB->setPosition(bottom->getPositionX() -vSize.width, bottom->getPositionY());
	this->addChild(sliderB);
	sliderB->setVisible(true);
	//
	this->addCaty(vSize.width, 0);
	//
	slideArea = CCRectMake(0,vSize.height/4 - bottom->getContentSize().height/2, vSize.width, bottom->getContentSize().height);


	srand(time(0));
	catyIndex = rand()%3+1;
	CCLOG("%d", catyIndex);
	this->changeCatyState(catyIndex);
	
	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	this->schedule(schedule_selector(loginScene::updateSlider,0.0001f));

	return true;
}

void loginScene::addCaty(int x, int y)
{
	for(int i = 1; i<= 3 ; i++)
	{
		Caty[i] = Sprite::create(String::createWithFormat("login/caty%d.png",i)->getCString());
		Caty[i]->setAnchorPoint(Vec2(1,0));
		Caty[i]->setPosition(x ,y);
		this->addChild(Caty[i],10+i);
		Caty[i]->setVisible(false);
	}
}

int loginScene::changeCatyState(int preIndex)
{
	int index = rand()%3 +1;
	while(index == preIndex)
	{
		index = rand()%3 +1;
	}
	catyIndex = index;
	Caty[catyIndex]->setVisible(true);
	Caty[preIndex]->setVisible(false);
	return catyIndex;
}

void loginScene::slideToUnlock(Vec2 prePos, Vec2 finalPos)
{
	if(slideArea.containsPoint(prePos) && slideArea.containsPoint(finalPos)
		 &&  finalPos.x - prePos.x > 0)
	{
		Director::getInstance()->replaceScene(gameScene::createScene());
	}
}

bool loginScene::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = pTouch->getLocation();
	if(Caty[2]->getBoundingBox().containsPoint(touchPoint))
		changeCatyState(catyIndex);

	return true;
}

void loginScene::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	this->slideToUnlock(pTouch->getPreviousLocation(),pTouch->getLocation());
}

void loginScene::onTouchEnded(Touch *pTouch, Event *pEvent)
{

}

void loginScene::updateSlider(float dt)
{
	float saX = sliderA->getPositionX();
	float sbX = sliderB->getPositionX();
	saX++;
	sbX++;

	if(saX >= 540)
	{
		saX = -180;
	}
	if(sbX >= 540)
	{
		sbX = -180;
	}

	sliderA->setPositionX(saX);
	sliderB->setPositionX(sbX);
}
