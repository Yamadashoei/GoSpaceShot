#include "GamePausedState.h"
#include "GamePlayingState.h"
#include "GameScene.h"
#include <memory>

void GamePausedState::Enter(GameScene& gameScene) {
	(void)gameScene;
	pauseIndex_ = 0;
}

void GamePausedState::Update(GameScene& gameScene) {
	auto* input = gameScene.GetInput();

	if (input->TriggerKey(DIK_ESCAPE)) {
		gameScene.ChangePlayState(std::make_unique<GamePlayingState>());
		return;
	}

	if (input->TriggerKey(DIK_W) || input->TriggerKey(DIK_UP)) {
		pauseIndex_ = (pauseIndex_ + 3 - 1) % 3;
	}
	if (input->TriggerKey(DIK_S) || input->TriggerKey(DIK_DOWN)) {
		pauseIndex_ = (pauseIndex_ + 1) % 3;
	}

	if (input->TriggerKey(DIK_RETURN) || input->TriggerKey(DIK_SPACE)) {
		if (pauseIndex_ == 0) {
			gameScene.ChangePlayState(std::make_unique<GamePlayingState>());
		} else if (pauseIndex_ == 1) {
			gameScene.Initialize();
		} else {
			gameScene.RequestTitleScene();
		}
	}
}

void GamePausedState::DrawOverlay(GameScene& gameScene) { gameScene.DrawPauseOverlay(pauseIndex_); }