/*
#include "FbxLoader.h"
#include <cassert>
#include "TexManager.h"

using namespace DirectX;

const std::string FbxLoader::baseDirectory = "Resource/";
const std::string FbxLoader::defaultTextureFileName = "ErrorTexture.png";

FbxLoader* FbxLoader::GetInstance()
{
    static FbxLoader instance;
    return &instance;
}

void FbxLoader::Initialize() {
    assert(fbxManager == nullptr);
    fbxManager = FbxManager::Create();
    FbxIOSettings *ios = FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(ios);
    fbxImporter = FbxImporter::Create(fbxManager, "");
}

void FbxLoader::Finalize() {
    fbxImporter->Destroy();
    fbxManager->Destroy();
}

Model FbxLoader::LoadModelFromFile(const string &modelName) {
    //モデルと同じ名前のフォルダから読み込む
    const string directoryPath = baseDirectory + modelName + "/";
    //拡張子.fbxを付加
    const string fileName = modelName + ".fbx";
    //連結してフルパスを得る
    const string fullpath = directoryPath + fileName;
    //ファイル名を指定してFBXファイルを読み込む
    if (!fbxImporter->Initialize(fullpath.c_str(), -1, fbxManager->GetIOSettings())) {
        assert(0);
    }
    //シーン生成
    FbxScene *fbxScene = FbxScene::Create(fbxManager, "fbxScene");

    //ファイルからロードしたFBX情報をシーンにインポート
    fbxImporter->Import(fbxScene);

    //モデル生成
    Model *model = new Model();
    model->name = modelName;
    model->isMaterial = true;
    //FBXノードの数を取得
    int nodeCount = fbxScene->GetNodeCount();
    //あらかじめ必要数分のメモリを確保することで、アドレスがずれるのを予防
    model->nodes.reserve(nodeCount);
    //ルートノードから順に解析してモデルに流し込む
    ParseNodeRecursive(model, fbxScene->GetRootNode());

    //FBXシーン保存
    model->fbxScene = fbxScene;

    return *model;
}

void FbxLoader::ParseNodeRecursive(Model *model, FbxNode *fbxNode, FBXNode *parent) {
    //ノード名を取得
    string name = fbxNode->GetName();
    //モデルにノードを追加
    model->nodes.emplace_back();
    FBXNode &node = model->nodes.back();
    //ノード名を取得
    node.name = fbxNode->GetName();
    //FBXノードのローカル移動情報
    FbxDouble3 rotation = fbxNode->LclRotation.Get();
    FbxDouble3 scaling = fbxNode->LclScaling.Get();
    FbxDouble3 translation = fbxNode->LclTranslation.Get();
    //形式変換して代入
    node.rotation = { (float)rotation[0], (float)rotation[1], (float)rotation[2], 0.0f };
    node.scaling = { (float)scaling[0], (float)scaling[1], (float)scaling[2], 0.0f };
    node.translation = { (float)translation[0], (float)translation[1], (float)translation[2], 1.0f };
    //スケール、回転、平行移動行列の計算
    XMMATRIX matScaling, matRotation, matTranslation;
    matScaling = XMMatrixScalingFromVector(node.scaling);
    matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
    matTranslation = XMMatrixTranslationFromVector(node.translation);
    //ローカル変形行列の計算
    node.transform = XMMatrixIdentity();
    node.transform *= matScaling; //ワールド行列にスケーリングを反映
    node.transform *= matRotation; //ワールド行列に回転を反映
    node.transform *= matTranslation; //ワールド行列に平行移動を反映
    //グローバル変形行列の計算
    node.globalTransform = node.transform;
    if (parent) {
        node.parent = parent;
        //親の変形を乗算
        node.globalTransform *= parent->globalTransform;
    }
    //FBXノードの情報を解析してノードに記録
    //FBXノードのメッシュ情報を解析
    FbxNodeAttribute *fbxNodeAttribute = fbxNode->GetNodeAttribute();
    if (fbxNodeAttribute) {
        if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
            model->meshNode = &node;
            ParseMesh(model, fbxNode);
        }
    }

    //子ノードに対して再起呼び出し
    for (int i = 0; i < fbxNode->GetChildCount(); i++) {
        ParseNodeRecursive(model, fbxNode->GetChild(i), &node);
    }
}

void FbxLoader::ParseMesh(Model *model, FbxNode *fbxNode) {
    //ノードのメッシュを取得
    FbxMesh *fbxMesh = fbxNode->GetMesh();
    //頂点座標読み取り
    ParseMeshVertices(model, fbxMesh);
    //面を構成するデータの読み取り
    ParseMeshFaces(model, fbxMesh);
    //マテリアルの読み取り
    ParseMaterial(model, fbxNode);
    //スキニングの情報読み取り
    ParseSkin(model, fbxMesh);
}

void FbxLoader::ParseMeshVertices(Model *model, FbxMesh *fbxMesh) {
    auto &vertices = model->vertices;
    //頂点座標データの数
    const int controlPointsCount = fbxMesh->GetControlPointsCount();
    //必要数だけ頂点データ配列を確保
    Vertex vert{};
    model->vertices.resize(controlPointsCount, vert);
    //FBXメッシュの頂点座標配列を取得
    FbxVector4 *pCoord = fbxMesh->GetControlPoints();
    //FBXメッシュの全頂点座標をモデル内の配列にコピーする。
    for (int i = 0; i < controlPointsCount; i++) {
        Vertex &vertex = vertices[i];
        //座標のコピー
        vertex.pos.x = (float)pCoord[i][0];
        vertex.pos.y = (float)pCoord[i][1];
        vertex.pos.z = (float)pCoord[i][2];
    }
}

void FbxLoader::ParseMeshFaces(Model *model, FbxMesh *fbxMesh) {
    auto &vertices = model->vertices;
    auto &indices = model->indices;
    //1ファイルに複数メッシュのモデルは非対応
    assert(indices.size() == 0);
    //面の数
    const int polygonCount = fbxMesh->GetPolygonCount();
    //UVデータの数
    const int textureUVCount = fbxMesh->GetTextureUVCount();
    //UV名リスト
    FbxStringList uvNames;
    fbxMesh->GetUVSetNames(uvNames);
    //面ごとの情報読み取り
    for (int i = 0; i < polygonCount; i++) {
        //面を構成する頂点の数を取得（3なら三角形ポリゴン)
        const int polygonSize = fbxMesh->GetPolygonSize(i);
        assert(polygonSize <= 4);
        //1頂点ずつ処理
        for (int j = 0; j < polygonSize; j++) {
            //FBX頂点配列のインデックス
            int index = fbxMesh->GetPolygonVertex(i, j);
            assert(index >= 0);
            //頂点法線読込
            Vertex &vertex = vertices[index];
            FbxVector4 normal;
            if (fbxMesh->GetPolygonVertexNormal(i, j, normal)) {
                vertex.normal.x = (float)normal[0];
                vertex.normal.y = (float)normal[1];
                vertex.normal.z = (float)normal[2];
            }
            //テクスチャUV読込
            if (textureUVCount > 0) {
                FbxVector2 uvs;
                bool lUnmappedUV;
                //0番決め打ちで読込
                if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV)) {
                    vertex.uv.x = (float)uvs[0];
                    vertex.uv.y = 1.0f - (float)uvs[1];
                }
            }
            //インデックス配列に頂点インデックス追加
            //3頂点目までなら
            if (j < 3) {
                //1点追加し、他の2点と三角形を構築する
                indices.push_back(index);
            }
            //4頂点目
            else {
                //3点追加し、四角形の0,1,2,3の内 2,3,0で三角形を構築する
                int index2 = indices[indices.size() - 1];
                int index3 = index;
                int index0 = indices[indices.size() - 3];
                indices.push_back(index2);
                indices.push_back(index3);
                indices.push_back(index0);
            }
        }
    }
    model->_indices = indices.size();
}

void FbxLoader::ParseMaterial(Model *model, FbxNode *fbxNode) {
    const int materialCount = fbxNode->GetMaterialCount();
    if (materialCount > 0) {
        //先頭のマテリアルを取得
        FbxSurfaceMaterial *material = fbxNode->GetMaterial(0);
        bool textureLoaded = false;
        if (material) {
            //FbxSurfaceLambertクラスかどうかを調べる
            if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
            {
                FbxSurfaceLambert *lambert = static_cast<FbxSurfaceLambert *>(material);
                //環境光係数
                FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
                model->material.ambient.x = (float)ambient.Get()[0];
                model->material.ambient.y = (float)ambient.Get()[1];
                model->material.ambient.z = (float)ambient.Get()[2];

                //拡散反射光係数
                FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
                model->material.diffuse.x = (float)diffuse.Get()[0];
                model->material.diffuse.y = (float)diffuse.Get()[1];
                model->material.diffuse.z = (float)diffuse.Get()[2];
            }
            //ディフューズテクスチャを取り出す
            const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
            if (diffuseProperty.IsValid())
            {
                const FbxFileTexture *texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
                if (texture) {
                    const char *filepath = texture->GetFileName();
                    //ファイルパスからファイル名抽出
                    string path_str(filepath);
                    string name = ExtractFileName(path_str);
                    //テクスチャ読み込み
                    LoadTexture(model, baseDirectory + model->name + "/" + name);
                    textureLoaded = true;
                }
            }
        }
        //テクスチャがない場合は白テクスチャを貼る
        if (!textureLoaded) {
            LoadTexture(model, baseDirectory + defaultTextureFileName);
        }
    }
    else {
        LoadTexture(model, baseDirectory + defaultTextureFileName);
    }
}

void FbxLoader::ParseSkin(Model *model, FbxMesh *fbxMesh) {
    FbxSkin *fbxSkin = static_cast<FbxSkin *>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));
    //スキニング情報が無ければ終了
    if (fbxSkin == nullptr) {
        //各頂点について処理
        for (int i = 0; i < model->vertices.size(); i++) {
            //最初のボーン(単位行列)の影響100%にする
            model->vertices[i].boneIndex[0] = 0;
            model->vertices[i].boneWeight[0] = 1.0f;
        }
        return;
    }
    //ボーン配列の参照
    std::vector<Model::Bone> &bones = model->bones;
    //ボーンの数
    int clusterCount = fbxSkin->GetClusterCount();
    bones.reserve(clusterCount);
    //全てのボーンについて
    for (int i = 0; i < clusterCount; i++) {
        //FBXボーン情報
        FbxCluster *fbxCluster = fbxSkin->GetCluster(i);
        //ボーン自体のノードの名前を取得
        const char *boneName = fbxCluster->GetLink()->GetName();
        //新しくボーンを追加し、追加したボーンの参照を得る
        bones.emplace_back(Model::Bone(boneName));
        Model::Bone &bone = bones.back();
        //自作ボーンとFBXのボーンを紐づける
        bone.fbxCluster = fbxCluster;
        //FBXから初期姿勢行列を取得する
        FbxAMatrix fbxMat;
        fbxCluster->GetTransformLinkMatrix(fbxMat);
        //XMMatrix型に変換する
        XMMATRIX initialPose;
        ConvertMatrixFromFbx(&initialPose, fbxMat);
        //初期姿勢行列の逆行列を得る
        bone.invInitialPose = XMMatrixInverse(nullptr, initialPose);
    }
    //ボーン番号とスキンウェイトのペア
    struct WeightSet {
        UINT index;
        float weight;
    };
    //二次元配列(ジャグ配列)
    //list:頂点が影響を受けるボーンの全リスト
    //vector:それを全頂点分
    std::vector<std::list<WeightSet>> weightLists(model->vertices.size());
    //全てのボーンについて
    for (int i = 0; i < clusterCount; i++) {
        //FBXボーン情報
        FbxCluster *fbxCluster = fbxSkin->GetCluster(i);
        //このボーンに影響を受ける頂点の数
        int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();
        //このボーンに影響を受ける頂点の配列
        int *controlPointIndices = fbxCluster->GetControlPointIndices();
        double *controlPointWeights = fbxCluster->GetControlPointWeights();
        //影響を受ける全頂点について
        for (int j = 0; j < controlPointIndicesCount; j++) {
            //頂点番号
            int vertIndex = controlPointIndices[j];
            //スキンウェイト
            float weight = (float)controlPointWeights[j];
            //その頂点の影響を受けるボーンリストに、ボーンとウェイトのペアを追加
            weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i, weight });
        }
    }
    //頂点配列書き換え用の参照
    auto &vertices = model->vertices;
    //各頂点について処理
    for (int i = 0; i < vertices.size(); i++) {
        //頂点のウェイトから最も大きい4つを選択
        auto &weightList = weightLists[i];
        //大小比較用のラムダ式を指定して降順にソート
        weightList.sort([](auto const &lhs, auto const &rhs) {
            //左の要素の方が大きければtrue　そうでなければfalseを返す
            return lhs.weight > rhs.weight;
            });
        int weightArrayIndex = 0;
        //降順ソート済みのウェイトリストから
        for (auto &WeightSet : weightList) {
            //頂点データに書き込み
            vertices[i].boneIndex[weightArrayIndex] = WeightSet.index;
            vertices[i].boneWeight[weightArrayIndex] = WeightSet.weight;
            //4つに達したら終了
            if (++weightArrayIndex >= MAX_BONE_INDICES) {
                float weight = 0.0f;
                //2番目以降のウェイトを合計
                for (int j = i; j < MAX_BONE_INDICES; j++) {
                    weight += vertices[i].boneWeight[j];
                }
                //合計で1.0f(100%)になるように調整
                vertices[i].boneWeight[0] = 1.0f - weight;
                break;
            }
        }
    }
}

void FbxLoader::LoadTexture(Model *model, const std::string &fullpath) {
    model->material.texNumber = TexManager::LoadTexture3(fullpath);
}

std::string FbxLoader::ExtractFileName(const std::string &path) {
    size_t pos1;
    //区切り文字'\\'が出てくる一番最後の部分を検索
    pos1 = path.rfind('\\');
    if (pos1 != string::npos) {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }
    //区切り文字'/'が出てくる一番最後の部分を検索
    pos1 = path.rfind('/');
    if (pos1 != string::npos) {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }
    return path;
}

void FbxLoader::ConvertMatrixFromFbx(DirectX::XMMATRIX *dst, const FbxAMatrix &src) {
    //行
    for (int i = 0; i < 4; i++) {
        //列
        for (int j = 0; j < 4; j++) {
            //1要素コピー
            dst->r[i].m128_f32[j] = (float)src.Get(i, j);
        }
    }
}
*/
#include "FbxLoader.h"
#include <cassert>
#include "TexManager.h"
#include "ModelManager.h"

using namespace DirectX;

const std::string FbxLoader::baseDirectory = "Resource/fbx/";
const std::string FbxLoader::defaultTextureFileName = "ErrorTexture.png";

FbxLoader *FbxLoader::GetInstance()
{
    static FbxLoader instance;
    return &instance;
}

void FbxLoader::Initialize() {
    assert(fbxManager == nullptr);
    fbxManager = FbxManager::Create();
    FbxIOSettings *ios = FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(ios);
    fbxImporter = FbxImporter::Create(fbxManager, "");
}

void FbxLoader::Finalize() {
    //fbxImporter->Destroy();
    //fbxManager->Destroy();
}

Model FbxLoader::LoadModelFromFile(const string &modelName) {
    //モデルと同じ名前のフォルダから読み込む
    const string directoryPath = baseDirectory + modelName + "/";
    //拡張子.fbxを付加
    const string fileName = modelName + ".fbx";
    //連結してフルパスを得る
    const string fullpath = directoryPath + fileName;
    //ファイル名を指定してFBXファイルを読み込む
    if (!fbxImporter->Initialize(fullpath.c_str(), -1, fbxManager->GetIOSettings())) {
        assert(0);
    }
    //シーン生成
    FbxScene *fbxScene = FbxScene::Create(fbxManager, "fbxScene");

    //ファイルからロードしたFBX情報をシーンにインポート
    fbxImporter->Import(fbxScene);

    //シーン内のノードのポリゴンを全て三角形にする
    FbxGeometryConverter converter(fbxManager);
    //全FbxMeshをマテリアル単位で分割
    converter.SplitMeshesPerMaterial(fbxScene, true);
    //ポリゴンを三角形にする
    converter.Triangulate(fbxScene, true);
    //シーンを流し込んだらImporterは開放してOK
    //fbxImporter->Destroy();

    // std::vector<FbxMesh *>meshes;
    // GetMesh(fbxScene->GetRootNode(), &meshes);

    // ModelManager::Instance();

     //モデル生成
    Model *model = new Model();
    model->name = modelName;
    model->isMaterial = true;
    //FBXノードの数を取得
    int nodeCount = fbxScene->GetNodeCount();
    //あらかじめ必要数分のメモリを確保することで、アドレスがずれるのを予防
    model->nodes.reserve(nodeCount);
    std::string oyaName = fbxScene->GetRootNode()->GetName();
    //ルートノードから順に解析してモデルに流し込む
    ParseNodeRecursive(model, fbxScene->GetRootNode(), nullptr, oyaName);
    //FBXシーン保存
    model->fbxScene = fbxScene;

    return *model;
}

void FbxLoader::GetMesh(FbxNode *Node, std::vector<FbxMesh *> *Mesh)
{
    int childCount = Node->GetChildCount();
    for (int i = 0; i < childCount; i++)
    {
        FbxNode *child = Node->GetChild(i);	//子ノードを取得
        int meshCount = child->GetNodeAttributeCount();
        for (int num = 0; num < meshCount; num++)
        {
            FbxNodeAttribute *info = child->GetNodeAttributeByIndex(num);
            FbxNodeAttribute::EType type = info->GetAttributeType();

            if (type == FbxNodeAttribute::EType::eMesh)
            {
                Mesh->emplace_back((FbxMesh *)info);
            }
        }
        GetMesh(child, Mesh);
    }
}

void FbxLoader::ParseNodeRecursive(Model *model, FbxNode *fbxNode, FBXNode *parent, std::string OyaName) {
    //ノード名を取得
    string name = fbxNode->GetName();
    //モデルにノードを追加
    model->nodes.emplace_back();
    FBXNode &node = model->nodes.back();
    //ノード名を取得
    node.name = fbxNode->GetName();
    //FBXノードのローカル移動情報
    FbxDouble3 rotation = fbxNode->LclRotation.Get();
    FbxDouble3 scaling = fbxNode->LclScaling.Get();
    FbxDouble3 translation = fbxNode->LclTranslation.Get();
    //形式変換して代入
    node.rotation = { (float)rotation[0], (float)rotation[1], (float)rotation[2], 0.0f };
    node.scaling = { (float)scaling[0], (float)scaling[1], (float)scaling[2], 0.0f };
    node.translation = { (float)translation[0], (float)translation[1], (float)translation[2], 1.0f };
    //スケール、回転、平行移動行列の計算
    XMMATRIX matScaling, matRotation, matTranslation;
    matScaling = XMMatrixScalingFromVector(node.scaling);
    matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
    matTranslation = XMMatrixTranslationFromVector(node.translation);
    //ローカル変形行列の計算
    node.transform = XMMatrixIdentity();
    node.transform *= matScaling; //ワールド行列にスケーリングを反映
    node.transform *= matRotation; //ワールド行列に回転を反映
    node.transform *= matTranslation; //ワールド行列に平行移動を反映
    //グローバル変形行列の計算
    node.globalTransform = node.transform;
    if (parent) {
        node.parent = parent;
        //親の変形を乗算
        node.globalTransform *= parent->globalTransform;
    }
    //FBXノードの情報を解析してノードに記録
    //FBXノードのメッシュ情報を解析
    FbxNodeAttribute *fbxNodeAttribute = fbxNode->GetNodeAttribute();
    if (fbxNodeAttribute) {
        if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
            model->meshNode = &node;
            ParseMesh(model, fbxNode);
        }
    }

    //子ノードに対して再起呼び出し
    for (int i = 0; i < fbxNode->GetChildCount(); i++) {
        /*Model subModel;
        ModelManager::Instance()->models.push_back(subModel);
        int Num = ModelManager::Instance()->models.size() - 1;
        ModelManager::Instance()->NowLoadNum++;
        ModelManager::Instance()->models[Num].name = model->name;
        ParseNodeRecursive(&ModelManager::Instance()->models[Num], fbxNode->GetChild(i), &node, OyaName);
        ModelManager::Instance()->models[Num].parent = model;*/
        ParseNodeRecursive(model, fbxNode->GetChild(i), &node, OyaName);
    }
    // int a = 0;
}

void FbxLoader::ParseMesh(Model *model, FbxNode *fbxNode) {
    //ノードのメッシュを取得
    FbxMesh *fbxMesh = fbxNode->GetMesh();
    std::map<int, std::forward_list<BoneAffectInfo>>boneAffectTable;
    //スキニングの情報読み取り
    ParseSkin(model, fbxMesh, boneAffectTable);
    //頂点座標読み取り
    ParseMeshVertices(model, fbxMesh, boneAffectTable);
    //面を構成するデータの読み取り
    ParseMeshFaces(model, fbxMesh);
    //マテリアルの読み取り
    ParseMaterial(model, fbxNode);
}

void FbxLoader::ParseMeshVertices(Model *model, FbxMesh *fbxMesh, std::map<int, std::forward_list<BoneAffectInfo>> &BoneAffectTable) {
    auto &vertices = model->vertices;
    //頂点座標データの数
    //const int controlPointsCount = fbxMesh->GetControlPointsCount();
    const int controlPointsCount = fbxMesh->GetPolygonVertexCount();
    //必要数だけ頂点データ配列を確保
    Vertex vert{};
    model->vertices.resize(controlPointsCount, vert);
    //FBXメッシュの頂点座標配列を取得
    FbxVector4 *pCoord = fbxMesh->GetControlPoints();
    //インデックスバッファの取得
    int *indices = fbxMesh->GetPolygonVertices();
    //FBXメッシュの全頂点座標をモデル内の配列にコピーする。
    for (int i = 0; i < controlPointsCount; i++) {
        Vertex &vertex = vertices[i];
        //インデックスバッファから頂点番号を取得
        int index = indices[i];
        //座標のコピー
        vertex.pos.x = (float)pCoord[index][0];
        vertex.pos.y = (float)pCoord[index][1];
        vertex.pos.z = (float)pCoord[index][2];
        //保存しておいたボーンの対応表から頂点に対する影響データを取得
        SetBoneAffectToVertex(BoneAffectTable, &vertex, index);
    }
}

void FbxLoader::ParseMeshFaces(Model *model, FbxMesh *fbxMesh) {
    auto &vertices = model->vertices;
    auto &indices = model->indices;
    ////1ファイルに複数メッシュのモデルは非対応
    //assert(indices.size() == 0);
    ////面の数
    //const int polygonCount = fbxMesh->GetPolygonCount();
    ////UVデータの数
    //const int textureUVCount = fbxMesh->GetTextureUVCount();
    ////UV名リスト
    //FbxStringList uvNames;
    //fbxMesh->GetUVSetNames(uvNames);
    ////面ごとの情報読み取り
    //for (int i = 0; i < polygonCount; i++) {
    //    //面を構成する頂点の数を取得（3なら三角形ポリゴン)
    //    const int polygonSize = fbxMesh->GetPolygonSize(i);
    //    assert(polygonSize <= 4);
    //    //1頂点ずつ処理
    //    for (int j = 0; j < polygonSize; j++) {
    //        //FBX頂点配列のインデックス
    //        int index = fbxMesh->GetPolygonVertex(i, j);
    //        assert(index >= 0);
    //        //頂点法線読込
    //        Vertex &vertex = vertices[index];
    //        FbxVector4 normal;
    //        if (fbxMesh->GetPolygonVertexNormal(i, j, normal)) {
    //            vertex.normal.x = (float)normal[0];
    //            vertex.normal.y = (float)normal[1];
    //            vertex.normal.z = (float)normal[2];
    //        }
    //        //テクスチャUV読込
    //        if (textureUVCount > 0) {
    //            FbxVector2 uvs;
    //            bool lUnmappedUV;
    //            //0番決め打ちで読込
    //            if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV)) {
    //                vertex.uv.x = (float)uvs[0];
    //                vertex.uv.y = 1.0f - (float)uvs[1];
    //            }
    //        }
    //        //インデックス配列に頂点インデックス追加
    //        //3頂点目までなら
    //        if (j < 3) {
    //            //1点追加し、他の2点と三角形を構築する
    //            indices.push_back(index);
    //        }
    //        //4頂点目
    //        else {
    //            //3点追加し、四角形の0,1,2,3の内 2,3,0で三角形を構築する
    //            int index2 = indices[indices.size() - 1];
    //            int index3 = index;
    //            int index0 = indices[indices.size() - 3];
    //            indices.push_back(index2);
    //            indices.push_back(index3);
    //            indices.push_back(index0);
    //        }
    //    }
    //}

    //頂点バッファの取得
    //FbxVector4 *vertices = fbxMesh->GetControlPoints();
    //インデックスバッファの取得
    //int *indices = fbxMesh->GetPolygonVertices();
    //頂点座標の数の取得
    //int polygonVertexCount = fbxMesh->GetPolygonVertexCount();

    //頂点情報の取得
    //for (int i = 0; i < polygonVertexCount; i++)
    //{
    //    //FBX頂点配列のインデックス
    //    int index = i;
    //    assert(index >= 0);
    //    //頂点法線読込
    //    Vertex &vertex = vertices[index];
    //    /*FbxVector4 normal;
    //    if (fbxMesh->GetPolygonVertexNormal(i, normal)) {
    //        vertex.normal.x = (float)normal[0];
    //        vertex.normal.y = (float)normal[1];
    //        vertex.normal.z = (float)normal[2];
    //    }*/
    //    ////テクスチャUV読込
    //    //if (textureUVCount > 0) {
    //    //    FbxVector2 uvs;
    //    //    bool lUnmappedUV;
    //    //    //0番決め打ちで読込
    //    //    if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV)) {
    //    //        vertex.uv.x = (float)uvs[0];
    //    //        vertex.uv.y = 1.0f - (float)uvs[1];
    //    //    }
    //    //}
    //    //インデックス配列に頂点インデックス追加
    //    //3頂点目までなら
    //    //if (j < 3) {
    //        //1点追加し、他の2点と三角形を構築する
    //        indices.push_back(index);
    //    //}
    //    ////4頂点目
    //    //else {
    //    //    //3点追加し、四角形の0,1,2,3の内 2,3,0で三角形を構築する
    //    //    int index2 = indices[indices.size() - 1];
    //    //    int index3 = index;
    //    //    int index0 = indices[indices.size() - 3];
    //    //    indices.push_back(index2);
    //    //    indices.push_back(index3);
    //    //    indices.push_back(index0);
    //    //}
    //}
    //ポリゴンの数だけ連番として保存する
    for (int i = 0; i < fbxMesh->GetPolygonVertexCount(); i++)
    {
        //左手系（右周り）
        indices.emplace_back(i * 3 + 1);
        indices.emplace_back(i * 3 + 2);
        indices.emplace_back(i * 3);
    }


    FbxArray<FbxVector4> normals;
    //法線リストの取得
    fbxMesh->GetPolygonVertexNormals(normals);
    //auto &mesh = ModelMesh->GetMesh();
    //法線設定
    for (int i = 0; i < normals.Size(); i++)
    {
        //Vec3<float>normal = { (float)normals[i][0],(float)normals[i][2],(float)normals[i][1] };
        //Vec3<float>normal = { (float)normals[i][0],(float)normals[i][1],(float)normals[i][2] };
        vertices[i].normal.x = (float)normals[i][0];
        vertices[i].normal.y = (float)normals[i][1];
        vertices[i].normal.z = (float)normals[i][2];
        //ModelMesh->vertices[i + offset].normal = normal;
    }

    //UVデータの数
    int uvCount = fbxMesh->GetTextureUVCount();
    //uvsetの名前保存用
    FbxStringList uvsetNames;
    //UVSetの名前リストを取得
    fbxMesh->GetUVSetNames(uvsetNames);
    FbxArray<FbxVector2>uvBuffers;
    fbxMesh->GetPolygonVertexUVs(uvsetNames.GetStringAt(0), uvBuffers);

    int size = uvBuffers.Size();
    for (int i = 0; i < size; ++i)
    {
        vertices[i].uv.x = (float)uvBuffers[i][0];
        vertices[i].uv.y = (1.0f - (float)uvBuffers[i][1]);
    }

    model->_indices = int(indices.size());
}

void FbxLoader::ParseMaterial(Model *model, FbxNode *fbxNode) {
    const int materialCount = fbxNode->GetMaterialCount();
    if (materialCount > 0) {
        //先頭のマテリアルを取得
        FbxSurfaceMaterial *material = fbxNode->GetMaterial(0);
        bool textureLoaded = false;
        if (material) {
            //FbxSurfaceLambertクラスかどうかを調べる
            if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
            {
                FbxSurfaceLambert *lambert = static_cast<FbxSurfaceLambert *>(material);
                //環境光係数
                FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
                model->material.ambient.x = (float)ambient.Get()[0];
                model->material.ambient.y = (float)ambient.Get()[1];
                model->material.ambient.z = (float)ambient.Get()[2];

                //拡散反射光係数
                FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
                model->material.diffuse.x = (float)diffuse.Get()[0];
                model->material.diffuse.y = (float)diffuse.Get()[1];
                model->material.diffuse.z = (float)diffuse.Get()[2];
            }
            //ディフューズテクスチャを取り出す
            const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
            if (diffuseProperty.IsValid())
            {
                const FbxFileTexture *texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
                if (texture) {
                    const char *filepath = texture->GetFileName();
                    //ファイルパスからファイル名抽出
                    string path_str(filepath);
                    string name = ExtractFileName(path_str);
                    //テクスチャ読み込み
                    LoadTexture(model, baseDirectory + model->name + "/" + name);
                    textureLoaded = true;
                }
            }
        }
        //テクスチャがない場合は白テクスチャを貼る
        if (!textureLoaded) {
            LoadTexture(model, baseDirectory + defaultTextureFileName);
        }
    }
    else {
        LoadTexture(model, baseDirectory + defaultTextureFileName);
    }
}

void FbxLoader::ParseSkin(Model *model, FbxMesh *fbxMesh, std::map<int, std::forward_list<BoneAffectInfo>> &BoneAffectTable) {
    //FbxSkin *fbxSkin = static_cast<FbxSkin *>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));
    ////スキニング情報が無ければ終了
    //if (fbxSkin == nullptr) {
    //    //各頂点について処理
    //    for (int i = 0; i < model->vertices.size(); i++) {
    //        //最初のボーン(単位行列)の影響100%にする
    //        model->vertices[i].boneIndex[0] = 0;
    //        model->vertices[i].boneWeight[0] = 1.0f;
    //    }
    //    return;
    //}
    ////ボーン配列の参照
    //std::vector<Model::Bone> &bones = model->bones;
    ////ボーンの数
    //int clusterCount = fbxSkin->GetClusterCount();
    //bones.reserve(clusterCount);
    ////全てのボーンについて
    //for (int i = 0; i < clusterCount; i++) {
    //    //FBXボーン情報
    //    FbxCluster *fbxCluster = fbxSkin->GetCluster(i);
    //    //ボーン自体のノードの名前を取得
    //    const char *boneName = fbxCluster->GetLink()->GetName();
    //    //新しくボーンを追加し、追加したボーンの参照を得る
    //    bones.emplace_back(Model::Bone(boneName));
    //    Model::Bone &bone = bones.back();
    //    //自作ボーンとFBXのボーンを紐づける
    //    bone.fbxCluster = fbxCluster;
    //    //FBXから初期姿勢行列を取得する
    //    FbxAMatrix fbxMat;
    //    fbxCluster->GetTransformLinkMatrix(fbxMat);
    //    //XMMatrix型に変換する
    //    XMMATRIX initialPose;
    //    ConvertMatrixFromFbx(&initialPose, fbxMat);
    //    //初期姿勢行列の逆行列を得る
    //    bone.invInitialPose = XMMatrixInverse(nullptr, initialPose);
    //}
    ////ボーン番号とスキンウェイトのペア
    //struct WeightSet {
    //    UINT index;
    //    float weight;
    //};
    ////二次元配列(ジャグ配列)
    ////list:頂点が影響を受けるボーンの全リスト
    ////vector:それを全頂点分
    //std::vector<std::list<WeightSet>> weightLists(model->vertices.size());
    ////全てのボーンについて
    //for (int i = 0; i < clusterCount; i++) {
    //    //FBXボーン情報
    //    FbxCluster *fbxCluster = fbxSkin->GetCluster(i);
    //    //このボーンに影響を受ける頂点の数
    //    int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();
    //    //このボーンに影響を受ける頂点の配列
    //    int *controlPointIndices = fbxCluster->GetControlPointIndices();
    //    double *controlPointWeights = fbxCluster->GetControlPointWeights();
    //    //影響を受ける全頂点について
    //    for (int j = 0; j < controlPointIndicesCount; j++) {
    //        //頂点番号
    //        int vertIndex = controlPointIndices[j];
    //        //スキンウェイト
    //        float weight = (float)controlPointWeights[j];
    //        //その頂点の影響を受けるボーンリストに、ボーンとウェイトのペアを追加
    //        weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i, weight });
    //    }
    //}
    ////頂点配列書き換え用の参照
    //auto &vertices = model->vertices;
    ////各頂点について処理
    //for (int i = 0; i < vertices.size(); i++) {
    //    //頂点のウェイトから最も大きい4つを選択
    //    auto &weightList = weightLists[i];
    //    //大小比較用のラムダ式を指定して降順にソート
    //    weightList.sort([](auto const &lhs, auto const &rhs) {
    //        //左の要素の方が大きければtrue　そうでなければfalseを返す
    //        return lhs.weight > rhs.weight;
    //        });
    //    int weightArrayIndex = 0;
    //    //降順ソート済みのウェイトリストから
    //    for (auto &WeightSet : weightList) {
    //        //頂点データに書き込み
    //        vertices[i].boneIndex[weightArrayIndex] = WeightSet.index;
    //        vertices[i].boneWeight[weightArrayIndex] = WeightSet.weight;
    //        //4つに達したら終了
    //        if (++weightArrayIndex >= MAX_BONE_INDICES) {
    //            float weight = 0.0f;
    //            //2番目以降のウェイトを合計
    //            for (int j = i; j < MAX_BONE_INDICES; j++) {
    //                weight += vertices[i].boneWeight[j];
    //            }
    //            //合計で1.0f(100%)になるように調整
    //            vertices[i].boneWeight[0] = 1.0f - weight;
    //            break;
    //        }
    //    }
    //}
    //スキンの数を取得
    int skinCount = fbxMesh->GetDeformerCount(FbxDeformer::eSkin);
    if (!skinCount)return;

    //auto skel = Model->GetComponent<Skeleton>();

    for (int i = 0; i < skinCount; ++i)
    {
        //i番目のスキンを取得
        FbxSkin *skin = (FbxSkin *)fbxMesh->GetDeformer(i, FbxDeformer::eSkin);

        //クラスターの数を取得
        int clusterNum = skin->GetClusterCount();

            //ボーン配列の参照
        std::vector<Model::Bone> &bones = model->bones;
        //ボーンの数
        int clusterCount = skin->GetClusterCount();
        bones.reserve(clusterCount);
        //全てのボーンについて
        for (int i = 0; i < clusterCount; i++) {
            //FBXボーン情報
            FbxCluster *fbxCluster = skin->GetCluster(i);
            //ボーン自体のノードの名前を取得
            const char *boneName = fbxCluster->GetLink()->GetName();
            //新しくボーンを追加し、追加したボーンの参照を得る
            bones.emplace_back(Model::Bone(boneName));
            Model::Bone &bone = bones.back();
            //自作ボーンとFBXのボーンを紐づける
            bone.fbxCluster = fbxCluster;
            //FBXから初期姿勢行列を取得する
            FbxAMatrix fbxMat;
            fbxCluster->GetTransformLinkMatrix(fbxMat);
            //XMMatrix型に変換する
            XMMATRIX initialPose;
            ConvertMatrixFromFbx(&initialPose, fbxMat);
            //初期姿勢行列の逆行列を得る
            bone.invInitialPose = XMMatrixInverse(nullptr, initialPose);
        }

        for (int j = 0; j < clusterNum; ++j)
        {
            //j番目のクラスタを取得
            FbxCluster *cluster = skin->GetCluster(j);
            std::string clusterName = cluster->GetName();

            //ボーン
           /* if (skel->bones.empty() || skel->bones.size() - 1 < j || skel->bones[j].name != clusterName)
            {
                skel->bones.emplace_back();
                skel->bones[j].name = clusterName;
                printf("	%d本目のボーン：%s\n", j, clusterName.c_str());
            }*/

            //クラスターをボーンの情報として保存
            LoadConvertFBXBone(cluster, j, BoneAffectTable);

            //ボーンのオフセット行列
           // FbxAMatrix globalTransform;
           // cluster->GetTransformLinkMatrix(globalTransform);
            //skel->bones[j].offsetMat = ConvertToXMMATRIX(globalTransform);
        }
    }

    //skel->CreateBoneTree();
}

void FbxLoader::LoadConvertFBXBone(FbxCluster *Cluster, int BoneIndex, std::map<int, std::forward_list<BoneAffectInfo>> &BoneAffectTable)
{
    //該当ボーンが影響を与える頂点の数
    int pointNum = Cluster->GetControlPointIndicesCount();
    //影響を与える頂点のインデックス配列
    int *pointArray = Cluster->GetControlPointIndices();
    //ウェイト配列
    double *weightArray = Cluster->GetControlPointWeights();

    //ボーンが影響を与える頂点の情報を取得する
    for (int i = 0; i < pointNum; ++i)
    {
        //頂点インデックスとウェイトを取得
        int index = pointArray[i];
        float weight = (float)weightArray[i];

        //それらの情報を
        BoneAffectInfo info;
        info.index = BoneIndex;

        if (0.99 < weight)weight = 1.0f;
        if (weight < 0.01)weight = 0.0f;

        info.weight = weight;

        if (info.weight != 0.0f)
        {
            //頂点インデックス(添字)ごとにリストとして影響を受けるボーンは管理している。
            //ボーン情報をプッシュ
            BoneAffectTable[index].emplace_front(info);
        }
    }
}

bool FbxLoader::Test(Vertex *VertexPtr)
{
    for (int i = 0; i < 4; ++i)
    {
        if (VertexPtr->boneIndex[i] == -1) {
            return true;
        }
    }
    return false;
}

void FbxLoader::SetBoneAffectToVertex(std::map<int, std::forward_list<BoneAffectInfo>> &Info, Vertex *VertexPtr, int Index)
{
    //影響データ表が空じゃない
    if (!Info[Index].empty())
    {
        //適用されるボーンの数
        int count = 0;

        //該当インデックスの影響データ一覧を参照
        for (auto itr = Info[Index].begin(); itr != Info[Index].end(); ++itr)
        {
            for (int i = 0; i < 8; ++i)
            {
                if (VertexPtr->boneIndex[3] == -1) {
                    //対象の頂点のボーンデータで空な領域にデータを保存
                    if (VertexPtr->boneIndex[i] == -1)	//ボーン未登録
                    {
                        VertexPtr->boneIndex[i] = itr->index;
                        VertexPtr->boneWeight[i] = itr->weight;
                        break;
                    }
                }
                else {
                    //対象の頂点のボーンデータで空な領域にデータを保存
                    if (VertexPtr->boneIndexB[i - 4] == -1)	//ボーン未登録
                    {
                        VertexPtr->boneIndexB[i - 4] = itr->index;
                        VertexPtr->boneWeightB[i - 4] = itr->weight;
                        break;
                    }
                }
            }
            count++;
        }

        //１つの頂点につき最大４つまでしかボーンが適用出来ない
        if (8 < count)
        {
            int a = 0;
            //FBXLoaderの頂点読み込みにて５つ以上のボーンが適用されました。５つ目以降は無視されます
            //assert(0);
        }
    }
}

void FbxLoader::LoadTexture(Model *model, const std::string &fullpath) {
    model->material.texNumber = TexManager::LoadTexture3(fullpath);
}

std::string FbxLoader::ExtractFileName(const std::string &path) {
    size_t pos1;
    //区切り文字'\\'が出てくる一番最後の部分を検索
    pos1 = path.rfind('\\');
    if (pos1 != string::npos) {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }
    //区切り文字'/'が出てくる一番最後の部分を検索
    pos1 = path.rfind('/');
    if (pos1 != string::npos) {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }
    return path;
}

void FbxLoader::ConvertMatrixFromFbx(DirectX::XMMATRIX *dst, const FbxAMatrix &src) {
    //行
    for (int i = 0; i < 4; i++) {
        //列
        for (int j = 0; j < 4; j++) {
            //1要素コピー
            dst->r[i].m128_f32[j] = (float)src.Get(i, j);
        }
    }
}

FbxScene* FbxLoader::LoadFBXScene(const string &modelName)
{
    //モデルと同じ名前のフォルダから読み込む
    const string directoryPath = baseDirectory + modelName + "/";
    //拡張子.fbxを付加
    const string fileName = modelName + ".fbx";
    //連結してフルパスを得る
    const string fullpath = directoryPath + fileName;
    //ファイル名を指定してFBXファイルを読み込む
    if (!fbxImporter->Initialize(fullpath.c_str(), -1, fbxManager->GetIOSettings())) {
        assert(0);
    }
    //シーン生成
    FbxScene *fbxScene = FbxScene::Create(fbxManager, "fbxScene");

    //ファイルからロードしたFBX情報をシーンにインポート
    fbxImporter->Import(fbxScene);

    return fbxScene;
}
