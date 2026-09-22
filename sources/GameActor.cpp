#include "GameActor.h"

std::vector<GameActor*> GameActor::mActors;

void GameActor::killPendingsActors()
{
	if (mActors.empty())
	{
		return;
	}

	for (size_t i = 0; i < mActors.size(); ++i)
	{
		if (mActors[i]->ShouldBeDestroyed())
		{
			delete mActors[i];
			mActors.erase(mActors.begin() + i);
		}
	}
}

void GameActor::killa()
{
	if (!mActors.empty())
	{
		for (const auto* actor : mActors)
		{
			delete actor;
		}
	}
	
	mActors.clear();
}

std::vector<GameActor*> GameActor::GetActorsByTag(Tag tag)
{
	std::vector<GameActor*> actors;
	
	for (auto* actor : mActors)
	{
		if (actor->GetTag() == tag) actors.emplace_back(actor);
	}
	
	return actors;
}

GameActor::GameActor() :
	mLogicPriority{ 0 },
	mRenderPriority{ 0 },
	mTag{ TagDefault },
	mTransform{},
	mPendingDestroy{ false }
{
	AddActor(this);
}

GameActor::GameActor(const short logicPriority, const short renderPriority, const Transform2D& transform, const Tag tag) :
	mLogicPriority{ logicPriority },
	mRenderPriority{ renderPriority },
	mTag{ tag },
	mTransform{ transform },
	mPendingDestroy{ false }
{
	AddActor(this);
}

