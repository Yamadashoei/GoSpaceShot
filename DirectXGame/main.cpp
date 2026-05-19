#include "KamataEngine.h"
#include <Windows.h>
#include <memory>

#include "GameClear.h"
#include "GameOver.h"
#include "GameScene.h"
#include "IScene.h"
#include "Rule.h"
#include "SceneState.h"
#include "Title.h"

using namespace KamataEngine;

static std::unique_ptr<IScene> currentScene;
static SceneState gState = SceneState::Title;

static void CreateAndInit(SceneState s) {
	switch (s) {
	case SceneState::Title:
		currentScene = std::make_unique<Title>();
		break;
	case SceneState::Rule:
		currentScene = std::make_unique<Rule>();
		break;
	case SceneState::Game:
		currentScene = std::make_unique<GameScene>();
		break;
	case SceneState::GameClear:
		currentScene = std::make_unique<GameClear>();
		break;
	case SceneState::GameOver:
		currentScene = std::make_unique<GameOver>();
		break;
	}
	currentScene->Initialize();
}

static bool CheckAndTransit() {
	if (currentScene->IsNextSceneRequested()) {
		gState = currentScene->GetNextScene();
		CreateAndInit(gState);
		return true;
	}
	return false;
}

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	KamataEngine::Initialize(L"LE3C_27_ヤマダ_ショウエイ");
	DirectXCommon* dx = DirectXCommon::GetInstance();

	// 最初のシーン
	CreateAndInit(gState);

	while (true) {
		if (KamataEngine::Update())
			break;

		// Update
		currentScene->Update();

		// Draw
		dx->PreDraw();
		currentScene->Draw();
		dx->PostDraw();

		// Transit
		CheckAndTransit();
	}

	KamataEngine::Finalize();
	return 0;
}