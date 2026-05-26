#pragma once

class GameScene;

class IGamePlayState {
public:
	virtual ~IGamePlayState() = default;

	virtual void Enter(GameScene& gameScene) { (void)gameScene; }
	virtual void Update(GameScene& gameScene) = 0;
	virtual void DrawOverlay(GameScene& gameScene) { (void)gameScene; }
};