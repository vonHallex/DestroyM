#pragma once
#include "cocos2d.h"
#include "gameScene.h"
USING_NS_CC;

class gameScene;

class loginScene: public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(loginScene);

	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);

	void addCaty(int x,int y);
	int changeCatyState(int preIndex);
	void updateSlider(float dt);
	void slideToUnlock(Vec2 prePos, Vec2 finalPos);

private:
	int catyIndex;
	std::map<int,Sprite*> Caty;
	Sprite* sliderA;
	Sprite* sliderB;
	Sprite* bottom;
	Rect slideArea;
};
