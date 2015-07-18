//-----------------------------------------------------------------------------
//!
//!	@file	gmAssetModelX.h
//!	@brief	3Dモデルアセット
//!	@author	Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	3Dアセットモデルクラス
//-----------------------------------------------------------------------------
class AssetModelX : public AssetBase
{
public:
	//! 型情報
	//GM_ASSET_TYPE(AssetModelX)

	class	Name;			//!< 名前文字列データ
	struct	Vertex;			//!< 頂点
	struct	GpuVertex;		//!< GPU頂点要素
	struct	Face;			//!< ポリゴン
	class	SkinWeight;		//!< スキンウェイト
	struct	Joint;			//!< ボーン関節
	class	Reader;			//!< 構文パーサー
	class	Material;		//!< マテリアル
	struct	Cluster;		//!< 描画グループクラスター
	class	Mesh;			//!< メッシュ
	class	Frame;			//!< モデルフレーム
	class	Animation;		//!< アニメーション


	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	AssetModelX(void);
	//! デストラクタ
	virtual ~AssetModelX(void);

	//@}

	//!	ロード
	//!	@param	[in]	fileName	ファイル名
	virtual bool					load(GM_CSTR fileName);
	//virtual bool					load(const c8* fileName);

	//!	フレームを名前で検索
	//!	@param	[in]	name		フレーム名
	AssetModelX::Frame*				searchFrame(GM_CSTR name);
	
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{


	//! フレームを移動させる
	//!	@param	[in]	frameName	適用するフレーム名
	//!	@param	[in]	offset		移動量
	bool							setOffsetByName(GM_CSTR frameName, Vector3& offset);

	//! フレームを回転させる
	//!	@param	[in]	frameName	適用するフレーム名
	//!	@param	[in]	rotate		回転量(Radian)
	bool							setRotateByName(GM_CSTR frameName, Vector3& rotate);

	//! フレームが動くかどうか設定
	//!	@param	[in]	frameName	適用するフレーム名
	//!	@param	[in]	isDynamic	動くかどうかフラグ
	bool							setDynamicByName(GM_CSTR frameName, bool& isDynamic);

	//! 最大最小値設定
	void							setMinMax(Float2 x, Float2 y, Float2 z){
		_minX = x._x;
		_maxX = x._y;
		_minY = y._x;
		_maxY = y._y;
		_minZ = z._x;
		_maxZ = z._y;
	}

	//! Xの最小値取得
	f32								getMinX() const { return _minX; }
	//! Xの最大値取得
	f32								getMaxX() const { return _maxX; }
	//! Yの最小値取得
	f32								getMinY() const { return _minY; }
	//! Yの最大値取得
	f32								getMaxY() const { return _maxY; }
	//! Zの最小値取得
	f32								getMinZ() const { return _minZ; }
	//! Zの最大値取得
	f32								getMaxZ() const { return _maxZ; }
	//! 動的フレームを持っているかどうか
	bool							haveDynamicFrame();
	//@}

public:
	//! フレームのポインタリスト
	//Array<AssetModelX::Frame*>		_frames;
	//Array<AssetModelX::Material*>	_materials;
	//Array<AssetModelX::Animation*>	_animations;		//!< アニメーション
	vector<AssetModelX::Frame*>		_frames;
	vector<AssetModelX::Material*>	_materials;
	vector<AssetModelX::Animation*>	_animations;		//!< アニメーション

private:
	//! フレームの読み込み
	AssetModelX::Frame*				_loadFrame(void);
	//! メッシュの読み込み
	AssetModelX::Mesh*				_loadMesh(void);
	//! マテリアル読み込み
	AssetModelX::Material*		 	_loadMaterial(void);
	//! アニメーション読み込み
	AssetModelX::Animation*		 	_loadAnimation(void);
private:
	AssetModelX::Reader*			_pReader;			//!< 構文パーサー
	GM_CHAR							_path[MAX_PATH];	//!< モデルのファイルパス名
	//c8								_path[MAX_PATH];	//!< モデルのファイルパス名
	f32								_minX;
	f32								_maxX;
	f32								_minY;
	f32								_maxY;
	f32								_minZ;
	f32								_maxZ;

	//IDirect3DVertexDeclaration9*	_pD3DVertexDecl;	//!< 頂点入力レイアウト
};

//-----------------------------------------------------------------------------
//!	名前文字列データ
//-----------------------------------------------------------------------------
class AssetModelX::Name
{
public:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	Name(GM_CSTR name="");

	//@}
	//-------------------------------------------------------------
	//! @name 取得/設定
	//-------------------------------------------------------------
	//@{

	//! 文字列を設定
	void							setName(GM_CSTR name);
	
	//! 文字列を取得
	GM_CSTR							getName(void) const;
	
	//@}
private:
	GM_CHAR							_name[MAX_PATH];		//!< 名前
};

//-----------------------------------------------------------------------------
//!	頂点データ
//-----------------------------------------------------------------------------
struct AssetModelX::Vertex {
	// スキニング情報の最大個数
	static const s32	SKIN_WEIGHT_COUNT_MAX = 4;

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	Vertex(void)
	//: _position(Vector3::ZERO)
	: _position(0.0f, 0.0f, 0.0f)
	{
		for( s32 i=0; i<SKIN_WEIGHT_COUNT_MAX; i++ ) {
			_skinFrame [i] = NULL;
			_skinWeight[i] = 0.0f;
		}
	}

	//@}

	//---- メンバー変数
	Vector3				_position;							//!< 座標
	Float2				_uv;								//!< UV

	Frame*				_skinFrame [SKIN_WEIGHT_COUNT_MAX];	//!< スキンフレーム
	f32					_skinWeight[SKIN_WEIGHT_COUNT_MAX];	//!< スキンウェイト
};

//-----------------------------------------------------------------------------
//!	頂点データ
//-----------------------------------------------------------------------------
struct AssetModelX::GpuVertex {
	Float3		_position;			//!< 位置座標　       [f32 x 3] 12 bytes
	//u8			_blendIndex [4];	//!< スキニングINDEX  [ u8 x 4]  4 bytes
	f32			_blendIndex [4];	//!< スキニングINDEX  [f32 x 4] 16 bytes
	f32			_blendWeight[4];	//!< スキニングWEIGHT [f32 x 4] 16 bytes
	Float3		_normal;			//!< 法線　　　       [f32 x 3] 12 bytes
	Vector4		_diffuseColor;		//!< デフューズ色 (diffuse)
	Vector4		_specularColor;		//!< スペキュラ色(specular)
	Vector4		_emissiveColor;		//!< 自己照明色(emissive)
	float		_shininess;			//!< パワー値(shininess)
	Float2		_uv;				//!< テクスチャUV     [f32 x 2]  8 bytes
};

//-----------------------------------------------------------------------------
//!	ポリゴンデータ
//-----------------------------------------------------------------------------
struct AssetModelX::Face {
	static const u32	VERTEX_COUNT_MAX = 4;
	s32					_material;
	s32					_vertexCount;
	s32					_vertexIndex[VERTEX_COUNT_MAX];
	s32					_normalIndex[VERTEX_COUNT_MAX];

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	// コンストラクタ
	Face(void)
	: _material(-1)
	, _vertexCount(0)
	{
		for( u32 i=0; i<VERTEX_COUNT_MAX; i++ ) {
			_vertexIndex[i] = 0;
			_normalIndex[i] = 0;
		}
	}

	//@}
};

//-----------------------------------------------------------------------------
//!	スキンウェイト
//-----------------------------------------------------------------------------
class AssetModelX::SkinWeight : public AssetModelX::Name
{
public:
	//---- 頂点ウェイトデータ
	struct Data {
		s32				_vertexIndex;		//!< 頂点番号
		f32				_weight;			//!< ウェイト値
	};

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	SkinWeight(void);
	//! デストラクタ
	virtual ~SkinWeight(void);

	//@}
	//-------------------------------------------------------------
	//! @name 取得/設定
	//-------------------------------------------------------------
	//@{

	//! オフセット行列を設定
	void				setOffsetMatrix(const Matrix& m) { _offsetMatrix = m; }
	//! オフセット行列を取得
	const Matrix&		getOffsetMatrix(void) const { return _offsetMatrix; }

	//@}

public:
	vector<AssetModelX::SkinWeight::Data>	_data;
private:
	Matrix				_offsetMatrix;		//! オフセット移動量
};

//-----------------------------------------------------------------------------
//!	ボーン関節
//-----------------------------------------------------------------------------
struct AssetModelX::Joint {

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	Joint(void)
	: _jointIndex(-1)
	,_parentIndex(-1)
	, _pFrame(NULL)							// 対応するフレーム
	, _offsetMatrix   (Matrix::IDENTITY)	// ボーンの空間にメッシュの頂点を変換する行列（ボーンの変換行列と連結するとボーンの影響を受けるメッシュのワールド空間座標が得られる）
	, _transformMatrix(Matrix::IDENTITY)
	{
	}

	//@}

	//s32					_jointIndex;		//!< 関節番号
	f32					_jointIndex;		//!< 関節番号
	s32					_parentIndex;		//!< 親の関節番号
	AssetModelX::Frame*	_pFrame;			//!< 対応するフレーム

	Matrix				_offsetMatrix;		//!< ボーンの空間にメッシュの頂点を変換する行列（ボーンの変換行列と連結するとボーンの影響を受けるメッシュのワールド空間座標が得られる）
	Matrix				_transformMatrix;	//!< ボーン空間からの相対姿勢
};

//-----------------------------------------------------------------------------
//!	構文パーサー
//-----------------------------------------------------------------------------
class AssetModelX::Reader
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	Reader(void);
	//! デストラクタ
	virtual ~Reader(void);

	//! 初期化
	bool				init(GM_CSTR fileName);
	//! 解放
	void				cleanup(void);

	//@}
	//-------------------------------------------------------------
	//! @name 取得/設定
	//-------------------------------------------------------------
	//@{

	//! 次のトークンを１つ取得
	GM_CSTR				getToken(void);

	//@}

	//! 現在のトークンと文字列比較
	bool				compare(GM_CSTR token);
	//! チャンクをスキップ
	bool				skipChunk(void);
	//! チャンクを開く
	bool				openChunk(void);
	//! チャンクを閉じる
	bool				closeChunk(void);

	//! 次のトークンの出現まで進める
	bool				searchNextToken(GM_CSTR token);


private:
	static const s32	TOKEN_LENGTH_MAX = 512;
	GM_CHAR				_token[TOKEN_LENGTH_MAX];

	GM_STR				_pCurrent;

	GM_STR				_pBuffer;
	s32					_bufferSize;
};

//-----------------------------------------------------------------------------
//!	マテリアルデータ
//-----------------------------------------------------------------------------
class AssetModelX::Material : public AssetModelX::Name, public RefCounter
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	Material(void);

	//@}
	//-------------------------------------------------------------
	//! @name 取得/設定
	//-------------------------------------------------------------
	//@{

	//! ディフューズ色を取得
	Vector4				getDiffuseColor(void) const;
	Vector4				getSpecularColor(void) const;
	Vector4				getEmissiveColor(void) const;
	float				getShininess(void) const;
	//! テクスチャを取得
	//AssetTexture*		getTexture(void) const;
	Texture*			getTexture(void) const;
	Texture*			getSpecularMap(void) const;
	Texture*			getNormalMap(void) const;

	//! ディフューズ色を設定
	void				setDiffuseColor (Vector4 color);
	void				setSpecularColor(Vector4 color);
	void				setEmissiveColor(Vector4 color);
	void				setShininess(f32 shininess);
	//! テクスチャを設定
	//void				setTexture(AssetTexture* pTexture);
	void				setTexture(Texture* pTexture);
	void				setSpecularMap(Texture* pSpecularMap);
	void				setNormalMap(Texture* pNormalMap);

	//@}

	//! シェ―ダのテクスチャを設定
	void				BindTextures();
	//! シェ―ダのテクスチャの設定解除
	void				UnBindTextures();

private:
	//! デストラクタ (直接呼び出しを禁止する)
	~Material(void);
private:
	//AssetTexture*		_pTexture;		//!< テクスチャ
	Vector4				_diffuseColor;	//!< デフューズ色 (diffuse)
	Vector4				_specularColor;	//!< スペキュラ色(specular)
	Vector4				_emissiveColor; //!< 自己照明色(emissive)
	float				_shininess;		//!< パワー値(shininess)
	Texture*			_pTexture;		//!< テクスチャ
	Texture*			_pSpecularMap;	//!< スペキュラマップ
	Texture*			_pNormalMap;	//!< ノーマルマップ
};

//-----------------------------------------------------------------------------
//!	描画グループクラスター
//-----------------------------------------------------------------------------
struct AssetModelX::Cluster {

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	// コンストラクタ
	Cluster(void)
	: _pMaterial(NULL)
	, _vertexStart(NULL)
	, _vertexCount(NULL)
	, _indexStart(NULL)
	, _primCount(NULL)
	{}

	//@}

	AssetModelX::Material*	_pMaterial;			//!< 関連付けられているマテリアル
	s32						_vertexStart;
	s32						_vertexCount;
	s32						_indexStart;
	s32						_primCount;			//!< プリミティブ数（ポリゴン数）
};


//-----------------------------------------------------------------------------
//!	アニメーション
//-----------------------------------------------------------------------------
class AssetModelX::Animation : public AssetModelX::Name
{
public:
	template <typename T> class Key;	// アニメーションキー
	class Bone;							// ボーン

	//-------------------------------------------------------------
	//!	@name	初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	Animation(void)
	: _totalTime(0)
	, _isRepeat(false)
	{}

	//!	デストラクタ
	virtual ~Animation(void);

	//@}
	//-------------------------------------------------------------
	//!	@name	取得・設定
	//-------------------------------------------------------------
	//@{

	//! 総再生時間を取得
	s32						getTotalTime(void) const { return _totalTime; }
	//! リピート再生かどうかを取得
	bool					isRepeat(void) const { return _isRepeat; }

	//! リピート再生フラグを設定
	void					setRepeat(bool repeat) { _isRepeat = repeat; }

	//@}
public:
	//! ボーン/キーフレーム情報を読み込み
	bool					_loadKeyData(AssetModelX::Reader* pReader);
public:
	s32										_totalTime;		//!< 総再生時間
	bool									_isRepeat;		//!< true:リピート再生 false:単独再生
	//Array<AssetModelX::Animation::Bone*>	_bone;
	vector<AssetModelX::Animation::Bone*>	_bone;
};

//-----------------------------------------------------------------------------
//! キーフレーム情報（テンプレート）
//-----------------------------------------------------------------------------
template <typename T>
class AssetModelX::Animation::Key
{
public:
	//-------------------------------------------------------------
	//!	@name	初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	Key(void) : _time(0) {}
	//! コンストラクタ
	Key(s32 t, T data) : _time(t), _data(data) {}

	//@}
public:
	s32						_time;		//!< キー時間
	T						_data;		//!< キーデータ(型に依存)
};

//-----------------------------------------------------------------------------
//!	ボーン情報(各ボーン内にキー配列)
//-----------------------------------------------------------------------------
class AssetModelX::Animation::Bone : public AssetModelX::Name
{
public:
	//-------------------------------------------------------------
	//!	@name	初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	Bone(void)
	: _jointIndex(-1)
	{}

	//@}
	//-------------------------------------------------------------
	//!	@name	取得・設定
	//-------------------------------------------------------------
	//@{

	//! 回転を取得		（補間あり）
	Quaternion				getRotation(f32 frameTime) const;
	//! スケールを取得	（補間あり）
	Vector3					getScale   (f32 frameTime) const;
	//! 位置を取得		（補間あり）
	Vector3					getPosition(f32 frameTime) const;

	//@}
public:
	s32													_jointIndex;	//!< 対象の関節番号
	//Array< AssetModelX::Animation::Key<Quaternion> >	_keyRotations;	//!< 回転
	//Array< AssetModelX::Animation::Key<Vector3>    >	_keyScales;		//!< スケール
	//Array< AssetModelX::Animation::Key<Vector3>    >	_keyPositions;	//!< 位置
	vector< AssetModelX::Animation::Key<Quaternion> >	_keyRotations;	//!< 回転
	vector< AssetModelX::Animation::Key<Vector3>    >	_keyScales;		//!< スケール
	vector< AssetModelX::Animation::Key<Vector3>    >	_keyPositions;	//!< 位置
};

//-----------------------------------------------------------------------------
//!	メッシュデータ
//-----------------------------------------------------------------------------
class AssetModelX::Mesh
{
public:
	//-------------------------------------------------------------
	// ポリゴン情報
	//-------------------------------------------------------------
	//@{

	//Array<AssetModelX::Vertex>		_vertices;		//!< 頂点
	//Array<Vector3>					_normals;		//!< 法線
	//Array<AssetModelX::Face>		_faces;			//!< ポリゴン
	//Array<AssetModelX::Cluster>		_clusters;		//!< 描画グループクラスター
	//Array<AssetModelX::Joint>		_joints;		//!< 関節
	//Array<AssetModelX::Material*>	_materials;		//!< 材質
	//Array<AssetModelX::SkinWeight*>	_skinWeights;	//!< スキニングウェイト
	vector<AssetModelX::Vertex>			_vertices;		//!< 頂点
	vector<Vector3>						_normals;		//!< 法線
	vector<AssetModelX::Face>			_faces;			//!< ポリゴン
	vector<AssetModelX::Cluster>		_clusters;		//!< 描画グループクラスター
	vector<AssetModelX::Joint>			_joints;		//!< 関節
	vector<AssetModelX::Material*>		_materials;		//!< 材質
	vector<AssetModelX::SkinWeight*>	_skinWeights;	//!< スキニングウェイト

	//@}
	//-------------------------------------------------------------
	//!	@name	初期化
	//-------------------------------------------------------------
	//@{
public:
	//! コンストラクタ
	Mesh(AssetModelX* pParent=NULL);
	//! デストラクタ
	virtual ~Mesh(void);

	//@}
	//-------------------------------------------------------------
	//!	@name	取得・設定
	//-------------------------------------------------------------
	//@{

	//! スキニングモデルかどうか
	bool					isSkined       (void) const { return _skinWeights.size() > 0; }
	//! ジョイント番号設定
	void					setJointIndex(s32 index)	{ _myFrameJointIndex = index; }
	//!	動くフラグ設定
	//! ※メモリマッピングするので呼ぶ回数は少なくする
	void					setDynamic(bool isDynamic);
	//! 頂点バッファを取得
	GLuint					getVertexBuffer(void) const { return _pVertexBuffer; }
	//! インデックスバッファを取得
	GLuint					getIndexBuffer (void) const { return _pIndexBuffer; }
	//! 頂点入力レイアウト取得
	VertexArrayObject		getVertexArrayObject(bool isShadow) const { return isShadow ? _shadowVAO : _VAO; }

	//! 自分のジョイント番号取得
	s32						getJointIndex  (void) const { return _myFrameJointIndex; }

	//@}
	//-------------------------------------------------------------
	//!	@name	メッシュ管理
	//-------------------------------------------------------------
	//@{

	//! モデルデータを最適化
	bool					optimize(void);

	//@}

	//! スキニングヘッダの読み込み
	bool					loadSkinMeshHeader(AssetModelX::Reader* pReader);
	// スキニングウェイトを読み込み
	bool					loadSkinWeight(AssetModelX::Reader* pReader);

private:
	//-------------------------------------------------------------
	// アニメーション情報
	//-------------------------------------------------------------
	// ※変数の説明はMSDNより抜粋
	
	s32						_skinWeightsPerVertexCountMax;	// メッシュ内の頂点に影響するトランスフォームの最大数。
	s32						_SkinWeightsPerFaceCountMax;	// 任意の面の 3 つの頂点に影響する一意のトランスフォームの最大数。

	s32						_myFrameJointIndex;				//!< 自分のフレームのジョイント番号
	bool					_isDynamic;						//!< 動くかどうか
	
	VertexArrayObject		_VAO;							//!< 頂点入力レイアウト
	VertexArrayObject		_shadowVAO;						//!< シャドウの時のレイアウト
	GLuint					_pVertexBuffer;					//!< 頂点バッファ
	GLuint					_pIndexBuffer;					//!< インデックスバッファ
	//---- 親のモデル
	AssetModelX*			_pParent;
};

//-----------------------------------------------------------------------------
//!	モデルフレーム
//-----------------------------------------------------------------------------
class AssetModelX::Frame : public AssetModelX::Name
{
public:
	//-------------------------------------------------------------
	//!	@name	初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	Frame(void);
	//!	デストラクタ
	virtual ~Frame(void);

	//@}
	//-------------------------------------------------------------
	//!	@name	階層構造
	//-------------------------------------------------------------
	//@{

	//! 子に追加
	void						add(AssetModelX::Frame* p);
	//! 親を取得
	AssetModelX::Frame*			getParent(void) const { return _pParent; }
	//! 次の兄弟を取得
	AssetModelX::Frame*			getNext  (void) const { return _pNext;   }
	//! 子を取得
	AssetModelX::Frame*			getChild (void) const { return _pChild;  }
	//! 前を取得
	AssetModelX::Frame*			getPrev  (void) const { return _pPrev;   }
	
	//@}
	//-------------------------------------------------------------
	//!	@name	設定
	//-------------------------------------------------------------
	//@{

	//! テクスチャのリピート数設定
	void						setTexRepeat(s32 repeat) { _texRepeat = repeat; }
	//! オフセットを設定
	void						setOffset(const Vector3& vec);
	//! オイラー角を設定
	void						setRotate(const Vector3& rot);

	//! オフセット行列を設定
	void						setOffsetMatrix(const Matrix& m);
	//! 姿勢行列を設定
	void						setTransformMatrix(const Matrix& m);
	//! 回転行列を設定
	void						setRotateMatrix(const Matrix& m);
	//! 関節番号を設定
	//void						setJointIndex(s32 index);
	void						setJointIndex(f32 index);
	//!	動くフラグ設定
	void						setDynamic(bool isDynamic)  {
		_isDynamic = isDynamic; 
		for( u32 i=0; i < _meshes.size(); ++i )	{
			_meshes.at(i)->setDynamic(_isDynamic);
		}
	}
	// 描画フラグ設定
	void						setRender(bool	isRender) {
		_isRender = isRender;
	}

	//! 最大最小値設定
	void						setMinMax(Float2 x, Float2 y, Float2 z){
		_minX = x._x;
		_maxX = x._y;
		_minY = y._x;
		_maxY = y._y;
		_minZ = z._x;
		_maxZ = z._y;
	}


	//@}
	//-------------------------------------------------------------
	//!	@name	取得
	//-------------------------------------------------------------
	//@{

	//! 動くかどうか
	bool						isDynamic	   (void) const { return _isDynamic; }
	//! 描画するかどうか
	bool						isRender	   (void) const { return _isRender; }
	//! オフセット行列を取得
	const Matrix&				getOffsetMatrix(void) const;
	//! 姿勢行列を取得
	const Matrix&				getTransformMatrix(void) const;
	//!	回転行列を取得
	const Matrix&				getRotateMatrix(void) const;
	//! 関節番号を取得
	//s32							getJointIndex(void) const;
	f32							getJointIndex(void) const;
	//! テクスチャのリピート数取得
	s32							getTexRepeat(void) const { return _texRepeat; }
	//! Xの最小値取得
	f32							getMinX() const { return _minX; }
	//! Xの最大値取得
	f32							getMaxX() const { return _maxX; }
	//! Yの最小値取得
	f32							getMinY() const { return _minY; }
	//! Yの最大値取得
	f32							getMaxY() const { return _maxY; }
	//! Zの最小値取得
	f32							getMinZ() const { return _minZ; }
	//! Zの最大値取得
	f32							getMaxZ() const { return _maxZ; }


	//@}
public:
	//---- ジオメトリ関連
	//Array<AssetModelX::Mesh*>	_meshes;			//!< メッシュデータ(モデルデータ)
	vector<AssetModelX::Mesh*>	_meshes;			//!< メッシュデータ(モデルデータ)
private:
	//---- アニメーション関連
	//s32							_jointIndex;		//!< 関節番号
	f32							_jointIndex;		//!< 関節番号
	s32							_texRepeat;			//!< テクスチャのリピート数
	Matrix						_offsetMatrix;		//!< オフセット移動行列
	Matrix						_transformMatrix;	//!< 関節姿勢行列
	Matrix						_rotationMatrix;	//!< 回転行列

	bool						_isDynamic;			//!< 動くかどうか
	bool						_isRender;			//!< 描画するかどうか

	//---- フレームノードの情報
	AssetModelX::Frame*			_pParent;			//!< 親	(AssetModelX::Frame)	
	AssetModelX::Frame*			_pNext;				//!< 次	(AssetModelX::Frame)
	AssetModelX::Frame*			_pPrev;				//!< 前	(AssetModelX::Frame)
	AssetModelX::Frame*			_pChild;			//!< 子	(AssetModelX::Frame)

	// 最大最小
	f32							_minX;
	f32							_maxX;
	f32							_minY;
	f32							_maxY;
	f32							_minZ;
	f32							_maxZ;
};

//=============================================================================
//	END OF FILE
//=============================================================================
