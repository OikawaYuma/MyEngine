#include "GlobalVariables.h"
#include <cassert>
#include "ImGuiCommon.h"
#include <fstream>
#include <WinUser.h>
#include <iostream>

GlobalVariables* GlobalVariables::GetInstance()
{
	static GlobalVariables instance;
	return &instance;
}

void GlobalVariables::Update()
{
	if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar()) return;

	// 各グループについて
	for (std::map<std::string, Group>::iterator itGroup = datas_.begin();
		itGroup != datas_.end(); ++itGroup) {
		// グループ名を取得
		const std::string& groupName = itGroup->first;
		// グループの参照を取得
		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str()))
			continue;

		// 各項目について
		for (std::map<std::string, Item>::iterator itItem = group.items.begin();
			itItem != group.items.end(); ++itItem) {

			// 項目名を取得
			const std::string& itemName = itItem->first;
			// 項目の参照を取得
			Item& item = itItem->second;

			// int32_t型の値を保持してれば
			if (std::holds_alternative<int32_t>(item.value)) {
				int32_t* ptr = std::get_if<int32_t>(&item.value);
				ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
			}
			// float型の値を保持してれば
			else if (std::holds_alternative<float>(item.value)) {
				float* ptr = std::get_if<float>(&item.value);
				ImGui::SliderFloat(itemName.c_str(), ptr, 0, 100);
			}
			// Vector3型の値を保持してれば
			else if (std::holds_alternative<Vector3>(item.value)) {
				Vector3* ptr = std::get_if<Vector3>(&item.value);
				ImGui::SliderFloat3(itemName.c_str(), &ptr->x, -10.0f, 10.0f);
			}
		}
		ImGui::Text("\n");

		if (ImGui::Button("Save")) {
			SaveFile(groupName);
			std::string message = std::format("{}.json saved.", groupName);
			MessageBoxA(nullptr, message.c_str(), "Globavariables", 0);
		}

		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
	ImGui::End();
}

void GlobalVariables::CreateGroup(const std::string& groupName)
{
	// 指定名のオブジェクトがなければ追加する
	datas_[groupName];
}

void GlobalVariables::SaveFile(const std::string& groupName)
{
	// グループを検索
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

	// 未登録チェック
	assert(itGroup != datas_.end());

	json root;
	root = json::object();

	// jsonオブジェクト登録
	root[groupName] = json::object();

	// 各項目について
	for (std::map<std::string, Item>::iterator itItem = itGroup->second.items.begin();
		itItem != itGroup->second.items.end(); ++ itItem) {

		// 項目名を取得
		const std::string& itemName = itItem->first;
		// 項目の参照を取得
		Item& item = itItem->second;
		// int32_t型の値を保持していれば
		if (std::holds_alternative<int32_t>(item.value)) {
			// int32_t型の値を登録
			root[groupName][itemName] = std::get<int32_t>(item.value);
		}
		else if (std::holds_alternative<float>(item.value)) {
			// float型の値を登録
			root[groupName][itemName] = std::get<float>(item.value);
		}
		else if (std::holds_alternative<Vector3>(item.value)) {
			// Vector3型の値を登録
			Vector3 value = std::get<Vector3>(item.value);
			root[groupName][itemName] = json::array({ value.x , value.y, value.z });
		}
	}
	// ディレクトリがなければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(groupName)) {
		std::filesystem::create_directories(groupName);
	}
	// 書き込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 書き込み用ファイルストリーム
	std::ofstream ofs;
	// ファイルを書き込むように開く
	ofs.open(filePath);
	// ファイルオープン失敗？
	if (ofs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}
	// ファイルにjson文字列を書き込む（インデント幅4）
	ofs << std::setw(4) << root << std::endl;
	// ファイルを閉じる
	ofs.close();
}

void GlobalVariables::LoadFiles()
{
	// ディレクトリがなければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(kDirectoryPath)) {
		return;
	}
	std::filesystem::directory_iterator dir_it(kDirectoryPath);
	for (const std::filesystem::directory_entry& entry : dir_it) {
		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		// ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		// .jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}
		// ファイル読み込み
		LoadFile(filePath.stem().string());
	}
	
}

void GlobalVariables::LoadFile(const std::string& groupName)
{
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 
	std::ifstream ifs;
	ifs.open(filePath);
	// ファイルオープン失敗？
	if (ifs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}
	json root;

	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	// グループを検索
	json::iterator itGroup = root.find(groupName);

	// 未登録チェック
	assert(itGroup != root.end());

	// 各アイテムについて
	for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
		// アイテム名を取得
		const std::string& itemName = itItem.key();
		// int32_t型の値を保持していれば
		if (itItem->is_number_integer()) {
			int32_t value = itItem->get<int32_t>();
			SetValue(groupName, itemName, value);
		}
		// float型の値を保持していれば
		else if (itItem->is_number_integer()) {
			double value = itItem->get<double>();
			SetValue(groupName, itemName, static_cast<float>(value));
		}
		// Vector3型の値を保持していれば
		else if (itItem->is_array() && itItem->size() == 3) {
			Vector3 value = { itItem->at(0), itItem->at(1) ,itItem->at(2) };
			SetValue(groupName, itemName, value);
		}
	}
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, int32_t value)
{
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value)
{
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector3& value)
{
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void GlobalVariables::AddItme(const std::string& groupName, const std::string& key, const int32_t& value)
{
	if (!std::filesystem::exists(key)) {
		SetValue(groupName,key,value);
	}
}

void GlobalVariables::AddItme(const std::string& groupName, const std::string& key, const float& value)
{
	if (!std::filesystem::exists(key)) {
		SetValue(groupName, key, value);
	}
}

void GlobalVariables::AddItme(const std::string& groupName, const std::string& key, const Vector3& value)
{
	if (!std::filesystem::exists(key)) {
		SetValue(groupName, key, value);
	}
}

int32_t GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key) const
{
	
	// グループが存在するか確認
	auto itGroup = datas_.find(groupName);
	if (itGroup == datas_.end()) {
		// グループが存在しない場合はエラー処理または適切なデフォルト値を返す
		throw std::runtime_error("Group not found");
	}

	const Group& group = itGroup->second;

	// アイテムが存在するか確認
	auto itItem = group.items.find(key);
	if (itItem == group.items.end()) {
		// アイテムが存在しない場合はエラー処理または適切なデフォルト値を返す
		throw std::runtime_error("Item not found");
	}

	// アイテムの値を取得
	const Item& item = itItem->second;

	// int32_t 型の値を保持していれば返す
	if (std::holds_alternative<int32_t>(item.value)) {
		return std::get<int32_t>(item.value);
	}
	else {
		// 型が違う場合はエラー処理または適切なデフォルト値を返す
		throw std::runtime_error("Item is not of type int32_t");
	}
}


float GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key) const
{
	// グループが存在するか確認
	auto itGroup = datas_.find(groupName);
	if (itGroup == datas_.end()) {
		throw std::runtime_error("Group not found");
	}

	const Group& group = itGroup->second;

	// アイテムが存在するか確認
	auto itItem = group.items.find(key);
	if (itItem == group.items.end()) {
		throw std::runtime_error("Item not found");
	}

	// アイテムの値を取得
	const Item& item = itItem->second;

	// float 型の値を保持していれば返す
	if (std::holds_alternative<float>(item.value)) {
		return std::get<float>(item.value);
	}
	else {
		throw std::runtime_error("Item is not of type float");
	}
}

Vector3 GlobalVariables::GetVector3Value(const std::string& groupName, const std::string& key) const
{
	// グループが存在するか確認
	auto itGroup = datas_.find(groupName);
	if (itGroup == datas_.end()) {
		throw std::runtime_error("Group not found");
	}

	const Group& group = itGroup->second;

	// アイテムが存在するか確認
	auto itItem = group.items.find(key);
	if (itItem == group.items.end()) {
		throw std::runtime_error("Item not found");
	}

	// アイテムの値を取得
	const Item& item = itItem->second;

	// Vector3 型の値を保持していれば返す
	if (std::holds_alternative<Vector3>(item.value)) {
		return std::get<Vector3>(item.value);
	}
	else {
		throw std::runtime_error("Item is not of type Vector3");
	}
}

void GlobalVariables::AddScore(int32_t score)
{
	scores_.push_back(score);
	json j;
	// JSONオブジェクトに新しいスコアを追加
	j["scores"] = scores_;
	std::string filePath = kDirectoryPath + "score.json";
	// JSONファイルに書き出し（上書き）
	std::ofstream file(filePath);
	if (file.is_open()) {
		file << j.dump(4);  // 4はインデントのスペース数
		file.close();
		std::cout << "JSONファイルに新しいスコアを書き込みました。" << std::endl;
	}
	else {
		std::cerr << "ファイルを開けませんでした。" << std::endl;
	}

	// 今回のスコアを更新
	nowScore_ = score;
}

void GlobalVariables::LoadFileScore()
{
	// JSONオブジェクトに新しいスコアを追加
	//j["scores"] = times_;
	std::string filePath = kDirectoryPath + "score.json";
	// JSONファイルを開く
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "ファイルを開けませんでした。" << std::endl;
		return;
	}

	// JSONオブジェクトにパース
	json j;
	file >> j;
	file.close();

	// JSONからスコアを読み込む
	try {
		if (j.contains("scores") && j["scores"].is_array()) {
			scores_ = j["scores"].get<std::vector<int32_t>>();
		}
		else {
			std::cerr << "JSONに 'scores' キーが含まれていないか、配列ではありません。" << std::endl;
			return;
		}
	}
	catch (const json::exception& e) {
		std::cerr << "JSONのパースエラー: " << e.what() << std::endl;
		return;
	}

	// スコアを表示
	std::cout << "読み込んだスコア:" << std::endl;
	for (int score : scores_) {
		std::cout << score << std::endl;
	}
}



