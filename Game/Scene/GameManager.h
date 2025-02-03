/**
* @ file GameManager.h
* @ brief ゲームに関する全ての設定を行う　Singleton Patternで作成
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include <memory>
#include <unordered_map>
#include <functional>
#include "IScene.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "ClearScene.h"
#include "DemoScene.h"
#include "GameOverScene.h"
#include "Input.h"
//#include "Input.h"
class GameManager
{
private:
	// シーンを保持するメンバ変数
	std::unique_ptr<IScene> sceneArr_;
	std::unordered_map<int, std::function<std::unique_ptr<IScene>()>> sceneMap_;  // ラムダ式でシーンを登録
	// どのステージを呼び出すかを管理する変数
	int currentSceneNo_ = 0;
	int prevSceneNo_ = 0;
public:
	GameManager();
	~GameManager();

	int Run();

};