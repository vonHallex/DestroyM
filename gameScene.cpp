#include "gameScene.h"
USING_NS_CC;

int gameScene::count = 0;

gameScene::gameScene()
{
	GAMEOVER = false;
	GAMEBEGIN = false;
	playerWin = false;
	MUSIC_ON = false;
}

gameScene::~gameScene()
{

}

Scene* gameScene::createScene()
{
	Scene* scene = Scene::create();
	gameScene* Layer = gameScene::create();

	scene->addChild(Layer);
	return scene;
}

bool gameScene::init()
{
	Size vSize = Director::getInstance()->getVisibleSize();
	Sprite* bgPic = Sprite::create("background1.png");
	bgPic->setPosition(vSize.width/2, vSize.height/2);
	this->addChild(bgPic,1);

	blockStore = blockManager::createBlockMangaer(this);
	srand(time(NULL));
	for(int i = 0; i< 6; i++)
	{
		for(int j = 0; j< 6; j++)
		{
			block* tmpBlock = block::createRandomTypeWithID(i*6+j);
			tmpBlock->setGameScene(this);
			tmpBlock->setScale(0.5f);
			tmpBlock->setAnchorPoint(CCPointZero);
			tmpBlock->setPosition(i* 60 - 360,j* 60);
			this->addChild(tmpBlock,2);
			
			MoveTo* moveToAction = MoveTo::create(0.5f,Vec2(i* 60,j* 60));
			tmpBlock->runAction(EaseBounceOut::create(moveToAction));
			blockStore->insertIntoStore(tmpBlock);
		}
	}


	/*while(blockStore->checkRowAndColumn() != 0)
	{
		blockStore->fillWithRandomBlock();
	}
*/
	mHero = hero::createWithLayerAndPic(this,"head_yuanAngle.png");
	mHero->setScale(0.4f);
	mHero->setPosition(mHero->getContentSize().width/2 -70, 360 +mHero->getContentSize().height/2 -60);
	this->addChild(mHero,3);

	mMonster = hero::createWithLayerAndPic(this,"saburai.png",MONSTER);
	mMonster->setScale(1.2f);
	mMonster->setPosition(270,550);
	mMonster->setTag(999);
	this->addChild(mMonster,3);

	Sprite* tagetBlock = Sprite::create("Point.png");
	tagetBlock->setTag(101);
	tagetBlock->setAnchorPoint(CCPointZero);
	tagetBlock->setScale(0.5f);
	tagetBlock->setVisible(false);
	this->addChild(tagetBlock,99);

	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	this->scheduleUpdate();
	GAMEBEGIN = true;
	MUSIC_ON = true;

	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("musicEffect/bk.mp3");

	return true;
}

bool gameScene::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPos = pTouch->getLocation();
	
	if(!GAMEOVER)
	{
		count++;
		if(count == 1)
		{
			mBlock_1st = blockStore->getCurrentBlockByPos(touchPos);
			mBlock_1st->setZOrder(66);
			//mBlock_1st->setScale(0.7f);
			auto spTarget = (Sprite*)this->getChildByTag(101);
			spTarget->setVisible(true);
			spTarget->setPosition(blockStore->getCurrentBlockByPos(touchPos)->getPosition());
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("musicEffect/exchange1.wav");
		}
		else if (count == 2)
		{
			mBlock_2nd = blockStore->getCurrentBlockByPos(touchPos);
			mBlock_2nd->setZOrder(67);
			
			blockStore->swapBlocks(mBlock_1st,mBlock_2nd);
			((Sprite*)this->getChildByTag(101))->setVisible(false);
			count = 0;

			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("musicEffect/exchange2.wav");
		}
	}

	return true;
}

void gameScene::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	
}

void gameScene::runBlockActionByType(block* mBlock)
{
	blockType type = mBlock->getCurrentType();
	const char* spName = String::createWithFormat("block/skill_%d.png", mBlock->getCurrentType())->getCString();
	Sprite* skillSprite = Sprite::create(spName);
	skillSprite->setScale(0.5f);
	skillSprite->setTag(1024);
	skillSprite->setPosition(mBlock->getPosition());
	Sprite* sp = (Sprite*)getChildByTag(999);

	switch (type)
	{
	case RED:
		{
			this->addChild(skillSprite,50);

			ccBezierConfig bConfig;
			bConfig.controlPoint_1 = Vec2(30,50);
			bConfig.controlPoint_2 = Vec2(10,10);
			bConfig.endPosition = sp->getPosition();
			BezierTo* bezier = BezierTo::create(0.5f,bConfig);
			CallFuncN* action2 = CallFuncN::create(CC_CALLBACK_1(gameScene::callBackAfterAnime,this,mBlock));
			Spawn* action3 = Spawn::create(RotateBy::create(0.5f,Vec3(0,0,360)),bezier,NULL);
			Sequence* action = Sequence::create(action3,action2,NULL);
			skillSprite->runAction(action);
			
			mHero->attackCharacter(mMonster,1);
			
			if(mMonster->getHeartPoint() == 0 && mHero->getHeartPoint() > 0)
			{
				GAMEBEGIN = false;
				GAMEOVER = true;
				playerWin = true;
			}
			break;
		}
	case GREEN:
		{
			skillSprite->setScale(1.2f);
			skillSprite->setPosition(this->getChildByTag(999)->getPosition());
			this->addChild(skillSprite,50);
			FadeOut* action1 = FadeOut::create(1.0f);
			CallFuncN* action2 = CallFuncN::create(CC_CALLBACK_1(gameScene::callBackAfterAnime,this,mBlock));
			Sequence* action = Sequence::create(action1,action2,NULL);
			skillSprite->runAction(action);

			break;
		}
	case BLUE:
		{
			this->addChild(skillSprite,50);
			MoveTo* action1 = MoveTo::create(0.5f,mHero->getPosition());
			CallFuncN* action2 = CallFuncN::create(CC_CALLBACK_1(gameScene::callBackAfterAnime,this,mBlock));
			Sequence* action = Sequence::create(action1,action2,NULL);
			skillSprite->runAction(action);
			
			mHero->addDefencePoint();
			break;
		}
	case YELLOW:
		{

			//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("musicEffect/thunder.wav",false);
			mHero->attackCharacter(mMonster,2);
			break;
		}
	case PURPLE:
		{

			mHero->addHeartPoint();
			break;
		}
	case CYAN:
		{


			break;
		}
	}

	playEffectByType(mBlock);
}

void gameScene::popRestartWindow()
{
	this->unscheduleUpdate();
	this->setTouchEnabled(false);
	//
	Size VSize = Director::getInstance()->getVisibleSize();
	Sprite* shadowBK = Sprite::create("shadow.png");
	shadowBK->setPosition(VSize.width/2,VSize.height/2);
	this->addChild(shadowBK,104);

	Sprite* window = Sprite::create("hintWindow.png");
	window->setScale(0.55f);
	window->setPosition(VSize.width/2 - window->getContentSize().width/2, VSize.height/2);
	this->addChild(window,105);
	Size winSize = window->getContentSize();
	MenuItemImage* nextStageButton = MenuItemImage::create("btn/nextStage_1.png","btn/nextStage_0.png",
															 CC_CALLBACK_1(gameScene::nextStageCallback, this));
	nextStageButton->setPosition(winSize.width - 160, winSize.height - 260);

	auto menu = Menu::create(nextStageButton, NULL);
	menu->setPosition(Vec2::ZERO);
	window->addChild(menu, 106);

	MoveBy* moveAction = MoveBy::create(1.0f,Vec2(window->getContentSize().width/2 - 38,0));
	EaseBounceOut* easeBounce = EaseBounceOut::create(moveAction);

	shadowBK->runAction(FadeIn::create(2.0f));
	window->runAction(easeBounce);
}	

void gameScene::nextStageCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(gameScene::createScene());
}

void gameScene::playEffectByType(block* mBlock)
{
	int index = (int)mBlock->getCurrentType();
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(String::createWithFormat("musicEffect/%d.wav",index)->getCString(),false);
	CCLOG(" type = %d", index);
}

bool gameScene::musicIsOn()
{
	return MUSIC_ON;
}

void gameScene::callBackAfterAnime(Node* mNode,block* mBlock)
{
	removeChildByTag(1024);
//	playEffectByType(mBlock);
}

void gameScene::update(float dt)
{
	if(GAMEBEGIN)
		if(blockStore->checkRowAndColumn()!=0)
			blockStore->fillWithRandomBlock();		

	if(GAMEOVER == true)
		popRestartWindow();
}
