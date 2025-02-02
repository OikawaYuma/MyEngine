/**
* @ file Loder.h
* @ brief Blenderで出力したObject情報を読み込むクラス
* * @ author 及川　優麿　（オイカワ　ユウマ）
*/

#pragma once
#include <string>
#include <vector>
#include "Transform.h"
#include "Model.h"
#include "Object3d.h"
#include "Camera.h"
#include <list>



class LevelData {
public:
	struct ObjectData {
		std::string filename;
		Transform transform;
	};
	std::vector<ObjectData> objects;

private:
	
	

};
class Player;
class BaseEnemy;
class Ground;
class PlayerItem;
class WorldDesign;
class Loder
{
public:
	static void LoadJsonFileBase(const std::string kDefaultBaseDirectory, const std::string fileName,std::vector<Object3d*>& objects,Camera* camera);
	static void LoadJsonFile(const std::string kDefaultBaseDirectory, const std::string fileName,Player *player,std::list<std::unique_ptr<BaseEnemy>>& enemys,std::list<std::unique_ptr<PlayerItem>>& items, std::list<std::unique_ptr<WorldDesign>>& worldDesigns,Ground * ground);
};


