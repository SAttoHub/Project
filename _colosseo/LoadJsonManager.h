#include "nlohmann/json.hpp"
#include "Singleton.h"
#include "DirectXMath.h"
#include <vector>
#include "Engine//Objects//3DObject.h"

// JSONオブジェクト配置データ読み取りクラス
class LoadJsonManager : public Singleton<LoadJsonManager> {
	friend class Singleton <LoadJsonManager>;
private:
	// エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	struct ObjectData {
		std::string FileName = "";
		std::string Name = "";
		std::string ShaderName = "";
		XMFLOAT3 Position = XMFLOAT3();
		XMFLOAT3 Rotate = XMFLOAT3();
		XMFLOAT3 Scale = XMFLOAT3();
	 };
	//std::vector<ObjectData> ObjectsData;

	struct LevelData {
		std::string FileName = "";
		std::vector<ObjectData> ObjectsData;
	};
	std::vector<LevelData> LevelsData;

public:
	void LoadJson(const char* _FileName);

	/*XMFLOAT3 GetPosition(const char* Name) {
		for (auto& data : ObjectsData) {
			if (Name == data.Name) {
				return data.Position;
			}
		}
		return XMFLOAT3();
	};

	XMFLOAT3 GetRotate(const char* Name) {
		for (auto& data : ObjectsData) {
			if (Name == data.Name) {
				return data.Rotate;
			}
		}
		return XMFLOAT3();
	};

	XMFLOAT3 GetScale(const char* Name) {
		for (auto& data : ObjectsData) {
			if (Name == data.Name) {
				return data.Scale;
			}
		}
		return XMFLOAT3(1.0f, 1.0f, 1.0f);
	};*/

	std::vector<Object3d*> LoadScene(const char* _FileName);
};