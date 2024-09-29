#pragma once
#include <variant>
#include "Vector3.h"
#include <map>
#include <string>
#include "json.hpp"
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include "function.h"
#include <format>
#include <wrl.h>
using json = nlohmann::json;
class GlobalVariables {
public: // base
	static GlobalVariables* GetInstance();

	void Update();

public: //使用構造体
	// 項目
	struct Item {
		// 項目の値
		std::variant<int32_t, float, Vector3> value;
	};
	// グループ
	struct Group {
		std::map<std::string, Item> items;
	};

public:
	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName"></param>
	void CreateGroup(const std::string& groupName);
	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName"></param>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// ディレクトリの全ファイル読み込み
	/// </summary>
	void LoadFiles();

	/// <summary>
	/// ファイルから読み込む読み込み
	/// </summary>
	void LoadFile(const std::string& groupName);
	// 値のセット(int)
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	// 値のセット(flaot)
	void SetValue(const std::string& groupName, const std::string& key, float value);
	// 値のセット(Vector3)
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

	// 項目の追加()
	void AddItme(const std::string& groupName, const std::string& key, const int32_t& value);
	// 項目の追加()
	void AddItme(const std::string& groupName, const std::string& key, const float& value);
	// 項目の追加()
	void AddItme(const std::string& groupName, const std::string& key, const Vector3& value);

	// 値の取得
	int32_t GetIntValue(const std::string& groupName, const std::string& key)const ;
	float GetFloatValue(const std::string& groupName, const std::string& key)const ;
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key)const;

public: // Timer記録関数
	void AddTime();

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	const GlobalVariables& operator=(const GlobalVariables&) = delete;

	// 全データ
	std::map<std::string, Group> datas_;
	// グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/GlobalVariables/";
};