#pragma once
#include "IGamePlayState.h"

class GamePausedState : public IGamePlayState {
public:
	void Enter(GameScene& gameScene) override;
	void Update(GameScene& gameScene) override;
	void DrawOverlay(GameScene& gameScene) override;

private:
	int pauseIndex_ = 0;
};