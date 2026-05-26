#include "GamePlayingState.h"
#include "GamePausedState.h"
#include "GameScene.h"
#include <memory>

void GamePlayingState::Enter(GameScene& gameScene) { (void)gameScene; }

void GamePlayingState::Update(GameScene& gameScene) {
	if (gameScene.GetInput()->TriggerKey(DIK_ESCAPE)) {
		gameScene.ChangePlayState(std::make_unique<GamePausedState>());
		return;
	}

	gameScene.UpdatePlayingCore();
}

void GamePlayingState::DrawOverlay(GameScene& gameScene) { (void)gameScene; }