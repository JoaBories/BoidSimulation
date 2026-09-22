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
	void AddActorToLogicList(short logicPriority, GameActor* actor);
	void AddActorToRenderList(short renderPriority, GameActor* actor);
	void AddActorToTagMap(Tag tag, GameActor* actor);

	static std::map<short, std::vector<GameActor*>> mActorLogicList;
	static std::map<short, std::vector<GameActor*>> mActorRenderList;
	static std::unordered_map<Tag, std::vector<GameActor*>> mActorTagMap;

protected:
	short mLogicPriority;
	short mRenderPriority;

	Tag mTag;

	Transform2D mTransform;

	bool mPendingDestroy;
	bool mActive = true;

public:
	//Static for all GameActors
	static void KillPendingsActors();
	static void Killa();

	static std::vector<GameActor*> GetActorsByTag(Tag tag);
	inline static std::map<short, std::vector<GameActor*>> GetActorsLogic()		{ return mActorLogicList; };
	inline static std::map<short, std::vector<GameActor*>> GetActorsRender()	{ return mActorRenderList; };

	//Public for object only
	GameActor();
	inline ~GameActor() {};

	GameActor(short logicPriority, short renderPriority, Transform2D transform, Tag tag);

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	inline Transform2D GetTransform() const										{ return mTransform; };
	inline void SetTransform(Transform2D transform)								{ mTransform = transform; };

	inline Tag GetTag() const													{ return mTag; };

	inline bool IsActive() const												{ return mActive; };
	inline void SetActive(bool active)											{ mActive = active; };

	inline bool ShouldBeDestroyed() const										{ return mPendingDestroy; };
	inline void Destroy()														{ mPendingDestroy = true; };
};