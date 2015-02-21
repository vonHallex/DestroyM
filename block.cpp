#include "block.h"
#include <cstdlib>
#include <ctime>
using namespace std;
USING_NS_CC;

block::block()
{
}

block::~block()
{

}

block* block::createRandomTypeWithID(const int& mIndex)
{
	block* mblock = new block;

	if(mblock && mblock->initRandomTypeWithID(mIndex))
	{
		mblock->autorelease();
		return mblock;
	}
	else
	{
		delete mblock;
		mblock = NULL;
		return NULL;
	}
}

bool block::initRandomTypeWithID(const int& mIndex)
{
	index = mIndex;
	mType = this->getRandomType();
	const char* fileName = String::createWithFormat("block/%d.png",mType)->getCString();
	this->initWithFile(fileName);
		
	return true;
}

block* block::createBlockWithType(const int& mIndex,const blockType& type)
{
	block* mblock = new block;

	if(mblock && mblock->initBlockWithType(mIndex,type))
	{
		mblock->autorelease();
		return mblock;
	}
	else
	{
		delete mblock;
		mblock = NULL;
		return NULL;
	}
}

bool block::initBlockWithType(const int& mIndex,const blockType& type)
{
	index = mIndex;
	mType = type;
	const char* fileName = String::createWithFormat("block/%d.png",mType)->getCString();
	this->initWithFile(fileName);

	return true;
}

Sprite* block::getSkillSprite() const
{
	if(skillSprite)
		return skillSprite;
	else
		return NULL;
}

void block::setGameScene(gameScene* scene)
{
	layer = scene;
}

void block::setIndex(int ind)
{
	index = ind;
}

int block::getIndex() const
{
	return index;
}

blockType block::getRandomType() const
{
	blockType randomIndex = (blockType)(rand()%6+1);
	return randomIndex;
}

void block::setBlockType(blockType type)
{
	mType = type;
}

blockType block::getCurrentType() const
{
	return mType;
}

//block* block::combineTwoBlcoks(block* blockA, block* blockB)
//{
//
//}



//---------------------------------------------------------------------------------

blockManager::blockManager()
{
	layer = nullptr;
}

blockManager::~blockManager()
{

}

blockManager* blockManager::createBlockMangaer(gameScene* mLayer)
{
	blockManager* mBlockManager = new blockManager();

	if(mBlockManager && mBlockManager->initBlockManager(mLayer))
	{
		return mBlockManager;
	}
	else
	{
		delete mBlockManager;
		mBlockManager = NULL;
		return NULL;
	}
}

bool blockManager::initBlockManager(gameScene* mLayer)
{
	layer = mLayer;
	return true;
}

bool blockManager::insertIntoStore(block* mBlock)
{
	blockStore.pushBack(mBlock);
	
	return true;
}

block* blockManager::getBlockByIndex(int mIndex)
{
	return (block*)blockStore.at(mIndex);
}

block* blockManager::getCurrentBlockByPos(Vec2 pos)
{
	int X = pos.x / 60;
	int Y = pos.y / 60;
	
	if(blockStore.at(6* X+Y) != nullptr)
		return  blockStore.at(6* X+Y);
	else 
		return NULL;
}

void blockManager::addBlockAtIndex(block* mBlock,int X, int Y)
{
	int num = 6* X+Y;
	blockStore.replace(num,mBlock);
}

int blockManager::checkRowAndColumn()
{
	int rowSames = -1;
	int columnSames = -1;
	for (int i = 0; i< 6; i++) 
	{
		for (int j = 0; j < 6; j++) 
		{
			if (j == 0)
				rowSames = 1;
			else {
				if (blockStore.at(i*6+j-1)->getCurrentType() == blockStore.at(i*6+j)->getCurrentType())
					rowSames++;
				else 
				{
					if (rowSames >= 3) 
					{
						for (int n = 1; n <=rowSames; n++)
						{
							blockStore.at(i*6+ j - n)->setVisible(false);		
							//blockStore.at(i*6+ j - n)->runAction(Ro);
						}
					}
					rowSames = 1;
				}
				if (j == 5 && rowSames >= 3) 
				{
					for (int n =1; n <= rowSames; n++)
					{
						blockStore.at(i*6 + 6 - n)->setVisible(false);
					}
				}
			}

			if (j == 0 )
				columnSames = 1;
			else {
				if (blockStore.at(6*(j - 1)+i)->getCurrentType() == blockStore.at(j*6+ i)->getCurrentType())
					columnSames++;
				else {
					if (columnSames >= 3) 
					{
						for (int n = 1; n <= columnSames; n++)
						{
							blockStore.at((j - n)*6+ i)->setVisible(false);
						}
					}
					columnSames = 1;
				}
				if (j == 5 && columnSames >= 3) {
					for (int n = 1; n <=columnSames; n++)
					{
						blockStore.at((6 - n)*6+i)->setVisible(false);
					}
				}
			}	
			//END LOOP
		}
	}

	rowSames = 1;
	columnSames = 1;

	int temp = 0;					
	int result = 0;
	for(int a = 0; a< 6; a++)
	{
		for(int b = 0; b< 6; b++)
		{
			if(blockStore.at(a*6+b)->isVisible() == false)
			{
				temp++;
			}
		}
		empty[a] = temp;
		result += temp;
		temp = 0;
	}
	return result;
}

void blockManager::fillWithRandomBlock()
{
	for(int j = 0; j<6; j++)			
	{
		if(empty[j] > 0)					
		{
			for(int i = 0; i<6 ;i++)		
			{
				while(blockStore.at(j*6 +i)->isVisible() == false) 
				{  
					block* m_block = blockStore.at(6*j+ i);
					
					auto action1 = RotateTo::create(0.5f,0,180);
					auto action2 = CallFuncN::create(CC_CALLBACK_1(blockManager::reCreate,this, j, i));
					Sequence* action = Sequence::create(action1,action2,NULL);
					m_block->setVisible(true);
					m_block->runAction(action);					
				}
			}
		}
	}
}

void blockManager::reCreate(Node* mNode,int X ,int Y)
{
	auto tmpBlock = (block*)mNode;

	block* newBlock = block::createRandomTypeWithID(tmpBlock->getIndex());
	newBlock->setAnchorPoint(CCPointZero);
	newBlock->setScale(0.5f);
	newBlock->setPosition(tmpBlock->getPositionX(),tmpBlock->getPositionY());
	layer->addChild(newBlock,2);	
	
	layer->removeChild(tmpBlock);
	newBlock->setVisible(true);
	//newBlock->runAction(MoveTo::create(0.3f,Vec2(tmpBlock->getPositionX(),tmpBlock->getPositionY())));
	newBlock->setRotation3D(Vec3(0,180,0));
	newBlock->runAction(RotateTo::create(0.5f,0,180));
	blockStore.replace(X*6+Y, newBlock);

	Sprite* sp = (Sprite*)layer->getChildByTag(999);
	//tmpBlock->runSkillToPos(sp->getPosition());			
	layer->runBlockActionByType(tmpBlock);

}

void blockManager::swapBlocks(block* blockA, block* blockB)
{
	Vec2 posA = blockA->getPosition();
	Vec2 posB = blockB->getPosition();

	blockA->runAction(MoveTo::create(0.5f,posB));
	blockB->runAction(MoveTo::create(0.5f,posA));

	blockStore.swap(blockA,blockB);
}
