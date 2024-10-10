#include "Loder.h"
#include  "json.hpp"
#include <fstream>
#include <cassert>
#include "ModelManager.h"

#include "Character/Player/Player.h"
#include "Ground/Ground.h"
#include "Character/Enemy/Enemy.h"
#include "Item/PlayerItem.h"
#include "WorldDesign/WorldDesign.h"

void Loder::LoadJsonFile(const std::string kDefaultBaseDirectory, const std::string fileName, Player* player, std::list<std::unique_ptr<Enemy>>& enemys, std::list<PlayerItem*>& items, std::list<WorldDesign*>& worldDesigns)
{
	// 連結してフルパスを得る
	const std::string fullpath = kDefaultBaseDirectory + "/" + fileName + ".json";

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// Json文字列から回答したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name =
		deserialized["name"].get<std::string>();
	// 正しいレベルデータファイル化チェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();

		// MESH
		if (type.compare("MESH") == 0) {
			// 要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				// ファイル名
				objectData.filename = object["file_name"];
			}

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.transform.translate.x = (float)transform["translation"][0];
			objectData.transform.translate.y = (float)transform["translation"][2];
			objectData.transform.translate.z = (float)transform["translation"][1];
			// 回転角
			objectData.transform.rotate.x = -(float)transform["rotation"][0];
			objectData.transform.rotate.y = -(float)transform["rotation"][2];
			objectData.transform.rotate.z = -(float)transform["rotation"][1];
			// スケーリングa
			objectData.transform.scale.x = (float)transform["scaling"][0];
			objectData.transform.scale.y = (float)transform["scaling"][2];
			objectData.transform.scale.z = (float)transform["scaling"][1];
			ModelManager::GetInstance()->LoadModel("Resources/" + objectData.filename, objectData.filename + ".obj");

		}

		// TODO: オブジェクト走査を再帰関数にまとめ、再帰呼出で枝を走査する
		if (object.contains("children")) {

		}
		// MESH
		if (type.compare("CAMERA") == 0) {
			// 要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				// ファイル名
				objectData.filename = object["file_name"];
			}

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.transform.translate.x = (float)transform["translation"][0];
			objectData.transform.translate.y = (float)transform["translation"][2];
			objectData.transform.translate.z = (float)transform["translation"][1];
			// 回転角
			objectData.transform.rotate.x = -((float)transform["rotation"][0] - 3.141592f / 2.0f);
			objectData.transform.rotate.y = -(float)transform["rotation"][2];
			objectData.transform.rotate.z = -(float)transform["rotation"][1];
			// スケーリング
			objectData.transform.scale.x = (float)transform["scaling"][0];
			objectData.transform.scale.y = (float)transform["scaling"][2];
			objectData.transform.scale.z = (float)transform["scaling"][1];

			//camera->SetTranslate(objectData.transform.translate);
			//camera->SetRotate(objectData.transform.rotate);
			//ModelManager::GetInstance()->LoadModel("Resources/box/", objectData.filename + ".obj");

		}
	}

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {
		if (objectData.filename.compare("player") == 0) {
			ModelManager::GetInstance()->LoadModel("Resources/" + objectData.filename, objectData.filename + ".obj");
			player->Init(objectData.transform.translate, objectData.filename);
		}
		else if (objectData.filename.compare("floor") == 0) {
			//floor->Init(objectData.transform.scale, objectData.transform.translate);
		}
		else if (objectData.filename.compare("enemy") == 0) {
			std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
			//enemy->SetPlayer(player);
			enemy->Init(objectData.transform.translate, objectData.filename);
			enemy->SetPlayer(player);
			enemys.push_back(std::move(enemy));
		}

		else if (objectData.filename.compare("item") == 0) {
			PlayerItem* item = new PlayerItem();
			item->SetPlayer(player);
			item->Init(objectData.transform.translate, objectData.filename);
			items.push_back(item);
		}
		else if (objectData.filename.compare("worldDesign") == 0) {
			WorldDesign* wood = new WorldDesign();
			ModelManager::GetInstance()->LoadModel("Resources/" + objectData.filename, objectData.filename + ".obj");
			wood->Init(objectData.transform.scale, objectData.transform.translate, objectData.filename);
			worldDesigns.push_back(wood);
		}

	}

}



void Loder::LoadJsonFileBase(const std::string kDefaultBaseDirectory, const std::string fileName, std::vector<Object3d*>& objects, Camera* camera)
{
	// 連結してフルパスを得る
	const std::string fullpath = kDefaultBaseDirectory + "/" + fileName + ".json";

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// Json文字列から回答したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name =
		deserialized["name"].get<std::string>();
	// 正しいレベルデータファイル化チェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();

		// MESH
		if (type.compare("MESH") == 0) {
			// 要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				// ファイル名
				objectData.filename = object["file_name"];
			}

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.transform.translate.x = (float)transform["translation"][0];
			objectData.transform.translate.y = (float)transform["translation"][2];
			objectData.transform.translate.z = (float)transform["translation"][1];
			// 回転角
			objectData.transform.rotate.x = -(float)transform["rotation"][0];
			objectData.transform.rotate.y = -(float)transform["rotation"][2];
			objectData.transform.rotate.z = -(float)transform["rotation"][1];
			// スケーリングa
			objectData.transform.scale.x = (float)transform["scaling"][0];
			objectData.transform.scale.y = (float)transform["scaling"][2];
			objectData.transform.scale.z = (float)transform["scaling"][1];
			ModelManager::GetInstance()->LoadModel("Resources/box/", objectData.filename + ".obj");

		}

		// TODO: オブジェクト走査を再帰関数にまとめ、再帰呼出で枝を走査する
		if (object.contains("children")) {

		}
		// MESH
		if (type.compare("CAMERA") == 0) {
			// 要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				// ファイル名
				objectData.filename = object["file_name"];
			}

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.transform.translate.x = (float)transform["translation"][0];
			objectData.transform.translate.y = (float)transform["translation"][2];
			objectData.transform.translate.z = (float)transform["translation"][1];
			// 回転角
			objectData.transform.rotate.x = -((float)transform["rotation"][0] - 3.141592f / 2.0f);
			objectData.transform.rotate.y = -(float)transform["rotation"][2];
			objectData.transform.rotate.z = -(float)transform["rotation"][1];
			// スケーリング
			objectData.transform.scale.x = (float)transform["scaling"][0];
			objectData.transform.scale.y = (float)transform["scaling"][2];
			objectData.transform.scale.z = (float)transform["scaling"][1];

			camera->SetTranslate(objectData.transform.translate);
			camera->SetRotate(objectData.transform.rotate);
			ModelManager::GetInstance()->LoadModel("Resources/box/", objectData.filename + ".obj");

		}
	}

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {
		// モデルを指定して3Dオブジェクトを生成
		Object3d* newObject3d = new Object3d();
		newObject3d->Init();
		newObject3d->SetModel(objectData.filename + ".obj");
		newObject3d->SetTransform(objectData.transform);
		// 配列に登録
		objects.push_back(newObject3d);

	}

}

