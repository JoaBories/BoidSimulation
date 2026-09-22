#include "GameActor.h"

std::map<short, std::vector<GameActor*>> GameActor::mActorLogicList;
std::map<short, std::vector<GameActor*>> GameActor::mActorRenderList;
std::unordered_map<Tag, std::vector<GameActor*>> GameActor::mActorTagMap;

void GameActor::AddActorToLogicList(short logicPriority, GameActor* actor)
{
	if (!mActorLogicList.count(logicPriority))
	{
		mActorLogicList[logicPriority] = std::vector<GameActor*>{ actor };
	}
	else
	{
		mActorLogicList.at(logicPriority).push_back(actor);
	}
}

void GameActor::AddActorToRenderList(short renderPriority, GameActor* actor)
{
	if (!mActorRenderList.count(renderPriority)) 
	{
		mActorRenderList[renderPriority] = std::vector<GameActor*>{ actor };
	}
	else
	{
		mActorRenderList.at(renderPriority).push_back(actor);
	}
}

void GameActor::AddActorToTagMap(Tag tag, GameActor* actor)
{
	if (!mActorTagMap.count(tag))
	{
		mActorTagMap[tag] = std::vector<GameActor*>{ actor };
	}
	else
	{
		mActorTagMap.at(tag).push_back(actor);
	}
}

void GameActor::KillPendingsActors()
{
	if (mActorLogicList.empty())
	{
		return;
	}

	for (auto& actorList : mActorLogicList)
	{
		for (size_t i = 0; i < actorList.second.size(); i++)
		{
			if (actorList.second[i]->ShouldBeDestroyed())
			{
				delete actorList.second[i];

				actorList.second.erase(actorList.second.begin() + i);
			};
		}

		if (actorList.second.empty()) mActorLogicList.erase(actorList.first);
	}

	for (auto& actorList : mActorRenderList)
	{
		for (size_t i = 0; i < actorList.second.size(); i++)
		{
			if (actorList.second[i]->ShouldBeDestroyed())
			{
				delete actorList.second[i];

				actorList.second.erase(actorList.second.begin() + i);
			};
		}

		if (actorList.second.empty()) mActorRenderList.erase(actorList.first);
	}

	for (auto& actorList : mActorTagMap)
	{
		for (size_t i = 0; i < actorList.second.size(); i++)
		{
			if (actorList.second[i]->ShouldBeDestroyed())
			{
				delete actorList.second[i];

				actorList.second.erase(actorList.second.begin() + i);
			};
		}

		if (actorList.second.empty()) mActorTagMap.erase(actorList.first);
	}
}

void GameActor::Killa()
{
	if (!mActorLogicList.empty())
	{
		for (auto& actorList : mActorLogicList)
		{
			for (auto* actor : actorList.second)
			{
				delete actor;
			}
		}

	}

	mActorLogicList.clear();
	mActorRenderList.clear();
	mActorTagMap.clear();
}

std::vector<GameActor*> GameActor::GetActorsByTag(Tag tag)
{
	if (mActorTagMap.find(tag) == mActorTagMap.end())
	{
		return std::vector<GameActor*>{};
	}

	return mActorTagMap[tag];
}

GameActor::GameActor() :
	mLogicPriority{ 0 },
	mRenderPriority{ 0 },
	mPendingDestroy{ false },
	mTransform{},
	mTag{ TagDefault },
	mActive{ true }
{
	AddActorToLogicList(mLogicPriority, this);
	AddActorToRenderList(mRenderPriority, this);
	AddActorToTagMap(mTag, this);
}

GameActor::GameActor(short logicPriority, short renderPriority, Transform2D transform, Tag tag):
	mLogicPriority{ logicPriority },
	mRenderPriority{ renderPriority },
	mPendingDestroy{ false },
	mTransform{ transform },
	mTag{ tag },
	mActive{ true }
{
	AddActorToLogicList(logicPriority, this);
	AddActorToRenderList(renderPriority, this);
	AddActorToTagMap(mTag, this);
}

