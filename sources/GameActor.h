#pragma once

#include "GlobalVariables.h"
#include "AssetBank.h"
using Struct::Transform2D;

enum Tag
{
	TagDefault
};

class GameActor
{
private:
	static void AddActor(GameActor* actor) { mActors.emplace_back(actor); }
	static std::vector<GameActor*> mActors;

protected:
	short mLogicPriority;
	short mRenderPriority;

	Tag mTag;

	Transform2D mTransform;

	bool mPendingDestroy;
	bool mActive = true;

public:
	//Static for all GameActors
	static void killPendingsActors();
	static void killa();

	//Used for iterating trough actors
	static const std::vector<GameActor*>& actors() { return mActors; }
	
	static std::vector<GameActor*> GetActorsByTag(Tag tag);

	//Public for object only
	GameActor();
	virtual ~GameActor() {}

	GameActor(short logicPriority, short renderPriority, const Transform2D& transform, Tag tag);

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	Transform2D GetTransform() const									{ return mTransform; }
	void SetTransform(const Transform2D& transform)						{ mTransform = transform; }

	Tag GetTag() const													{ return mTag; }

	bool IsActive() const												{ return mActive; }
	void SetActive(bool active)											{ mActive = active; }

	bool ShouldBeDestroyed() const										{ return mPendingDestroy; }
	void Destroy()														{ mPendingDestroy = true; }
};