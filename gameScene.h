#ifndef GAMESCENE_H
#define GAMESCENE_H
#include "cocos2d.h"
#include "hero.h"
#include "block.h"
USING_NS_CC;

//enum blockType;
class block;
class blockManager;
class hero;

class gameScene: public Layer
{
public:
	gameScene();
	~gameScene();
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(gameScene);

	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
	void callBackAfterAnime(Node* mNode,block* mBlock);
	void runBlockActionByType(block* mBlock);
	void playEffectByType(block* mBlock);
	void popRestartWindow();
	void update(float dt);
	void nextStageCallback(Ref* pSender);
	bool musicIsOn();

private:
	bool GAMEOVER;
	bool playerWin;
	bool GAMEBEGIN;
	bool MUSIC_ON;
	hero* mHero;
	hero* mMonster;
	block* mBlock_1st;
	block* mBlock_2nd;
	blockManager *blockStore;
	Menu* UIMenu;

	static int count;
};

#endif // 

