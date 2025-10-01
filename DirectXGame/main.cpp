#include "KamataEngine.h"
#include <Windows.h>
#include <memory>

#include "GameClear.h"
#include "GameOver.h"
#include "GameScene.h"
#include "Rule.h"
#include "SceneState.h"
#include "Title.h"

using namespace KamataEngine;

static std::unique_ptr<Title> gTitle;
static std::unique_ptr<Rule> gRule;
static std::unique_ptr<GameScene> gGame;
static std::unique_ptr<GameClear> gClear;
static std::unique_ptr<GameOver> gOver;

static SceneState gState = SceneState::Title;

static void CreateAndInit(SceneState s) {
	switch (s) {
	case SceneState::Title:
		gTitle = std::make_unique<Title>();
		gTitle->Initialize();
		break;
	case SceneState::Rule:
		gRule = std::make_unique<Rule>();
		gRule->Initialize();
		break;
	case SceneState::Game:
		gGame = std::make_unique<GameScene>();
		gGame->Initialize();
		break;
	case SceneState::GameClear:
		gClear = std::make_unique<GameClear>();
		gClear->Initialize();
		break;
	case SceneState::GameOver:
		gOver = std::make_unique<GameOver>();
		gOver->Initialize();
		break;
	}
}

static bool CheckAndTransit() {
	switch (gState) {
	case SceneState::Title:
		if (gTitle->IsNextSceneRequested()) {
			gState = gTitle->GetNextScene();
			CreateAndInit(gState);
			return true;
		}
		break;
	case SceneState::Rule:
		if (gRule->IsNextSceneRequested()) {
			gState = gRule->GetNextScene();
			CreateAndInit(gState);
			return true;
		}
		break;
	case SceneState::Game:
		if (gGame->IsNextSceneRequested()) {
			gState = gGame->GetNextScene();
			CreateAndInit(gState);
			return true;
		}
		break;
	case SceneState::GameClear:
		if (gClear->IsNextSceneRequested()) {
			gState = gClear->GetNextScene();
			CreateAndInit(gState);
			return true;
		}
		break;
	case SceneState::GameOver:
		if (gOver->IsNextSceneRequested()) {
			gState = gOver->GetNextScene();
			CreateAndInit(gState);
			return true;
		}
		break;
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
		switch (gState) {
		case SceneState::Title:
			gTitle->Update();
			break;
		case SceneState::Rule:
			gRule->Update();
			break;
		case SceneState::Game:
			gGame->Update();
			break;
		case SceneState::GameClear:
			gClear->Update();
			break;
		case SceneState::GameOver:
			gOver->Update();
			break;
		}

		// Draw
		dx->PreDraw();
		switch (gState) {
		case SceneState::Title:
			gTitle->Draw();
			break;
		case SceneState::Rule:
			gRule->Draw();
			break;
		case SceneState::Game:
			gGame->Draw();
			break;
		case SceneState::GameClear:
			gClear->Draw();
			break;
		case SceneState::GameOver:
			gOver->Draw();
			break;
		}
		dx->PostDraw();

		// Transit
		CheckAndTransit();
	}

	KamataEngine::Finalize();
	return 0;
}
