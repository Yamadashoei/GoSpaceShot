#pragma once
#include "IGamePlayState.h"

class GamePlayingState : public IGamePlayState {
public:
	void Enter(GameScene& gameScene) override;
	void Update(GameScene& gameScene) override;
	void DrawOverlay(GameScene& gameScene) override;
};