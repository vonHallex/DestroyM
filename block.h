#ifndef BLOCK_H
#define BLOCK_H
#include "cocos2d.h"
#include "blockSkill.h"
#include "globalDefine.h"
//#include "Box2D/Box2D.h"
#include "gameScene.h"
USING_NS_CC;

typedef enum blockType
{
	RED = 1,
	GREEN,
	BLUE,
	YELLOW,
	PURPLE,
	CYAN
}blockType;

class gameScene;

class block: public Sprite
{
public:
	block();
	~block();
	static block* createRandomTypeWithID(const int& mIndex);
	static block* createBlockWithType(const int& mIndex,const blockType& type);
	bool initRandomTypeWithID(const int& mIndex);
	bool initBlockWithType(const int& mIndex,const blockType& type);
	//block* combineTwoBlcoks(block* blockA, block* blockB);
	blockType getRandomType() const;
	//Map<blockType,block> blockMap;	
	//Animate* releaseAnimation();
	blockType getCurrentType() const;
	void setGameScene(gameScene* scene);
	void setIndex(int ind);
	int getIndex() const;
	void setBlockType(blockType type);
	//void runSkillToPos(Vec2 pos);
	Sprite* getSkillSprite() const;
	block* operator + (block* blockB);
private:
	Sprite* skillSprite;
	blockType mType;
	gameScene* layer;	
	int index;
};


class blockManager:public Node
{
public:
	blockManager();
	~blockManager();
	//CREATE_FUNC(blockManager);
	static blockManager* createBlockMangaer(gameScene* mLayer);
	bool initBlockManager(gameScene* mLayer);
	//若存在消除过的便创建新block补位
	bool insertIntoStore(block* mBlock);
	block* getCurrentBlockByPos(Vec2 pos);
	block* getBlockByIndex(int mIndex);
	void addBlockAtIndex(block* mBlock,int X, int Y);
	//消除
	int checkRowAndColumn();
	//填充
	void fillWithRandomBlock();
	//填充时落下
	//void blockFallDown(int X,int Y);
	void reCreate(Node* mNode,int X,int Y);
	void setRunningLayer(gameScene* layer);
	void swapBlocks(block* blockA, block* blockB,Vec2 pos = 0);
	void swapByPosition(Vec2 preLocation,Vec2 finLocation);

private:
	//静态容器储存所有的block
	Vector<block*> blockStore;
	gameScene* layer;
	int empty[6];

};

#endif
