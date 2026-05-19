#pragma once
#include "SceneState.h"

class IScene {
public:
	virtual ~IScene() = default;

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual bool IsNextSceneRequested() const = 0;
	virtual SceneState GetNextScene() const = 0;
};