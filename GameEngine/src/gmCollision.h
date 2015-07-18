//-----------------------------------------------------------------------------
//!
//!	@file	gmCollision.h
//!	@brief	衝突判定
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
//! 衝突判定
//-----------------------------------------------------------------------------
class Collision
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{
	
	//! デフォルトコンストラクタ
	Collision(){}
	//! コンストラクタ
	Collision(Sphere* hitSphere, char* name);
	//! デストラクタ
	virtual		~Collision();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! デバック描画
	void				DebugRender();

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! 情報設定
	//!	@param	[in]	col	設定する当たり判定情報
	void				setHitObj(Collision* col);
	//! 当たり判定情報初期化
	void				clearHitObj() { _hitObj.clear(); }

	//! 基本情報設定
	//!	@param	[in]	sphere	当たり判定用の球
	//!	@param	[in]	name	当たり判定の名前
	void				setCollisionState(Sphere* sphere, GM_CSTR name);
	//! 当たり判定用球の設定
	//!	@param	[in]	sphere	当たり判定用の球
	void				setHitSphere(Sphere* sphere);
	//! 名前の設定
	//!	@param	[in]	name	当たり判定の名前
	void				setColName(GM_CSTR name);


	//! 情報取得
	Collision*			getHitObj(s32 index)	{ return _hitObj[index]; }
	//! 名前取得
	char*				getName()	{ return _name; }
	//! 当たり判定用の球取得
	Sphere*				getHitSphere() { return _hitSphere; }
	//! 当たっている個数取得
	s32					getHitCount();

	//!	当たった後の補正位置取得
	//!	@param	[in]	obj		当たり判定の相手
	Vector3				getOffsetPos(Collision*	obj);
	
	//@}
private:
	//---- 基本メンバー
	char				_name[32];		//!< 名前
	Sphere*				_hitSphere;		//!< 当たり判定用の球
	vector<Collision*>	_hitObj;		//!< 当たった相手の情報
};



// プロトタイプ宣言
class LandScape;
class CollisionWall;



//-----------------------------------------------------------------------------
//! 衝突判定システム
//-----------------------------------------------------------------------------
class SystemCollision
{
public:
	class	Object;		//!< 物体データ　オブジェクト

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{
private:
	//! コンストラクタ
	SystemCollision();
	//! コピーコンストラクタ
	SystemCollision(SystemCollision& src){};
	//! デストラクタ
	virtual ~SystemCollision();

public:

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! デバッグ描画
	void				DebugRender();
	//! デバッグ描画(2D)
	void				DebugRender2D();

	//@}
	//-------------------------------------------------------------
	//! @name 読み込み・削除
	//-------------------------------------------------------------
	//@{

	//! Xファイルからポリゴンデーターを入力
	//! @param	[in]	colModel	Xファイル
	bool				loadXfile(TaskModelX* colModel);

	//! フレームからポリゴンデーターを入力
	//! @param	[in]	colModel	フレーム
	//!	@param	[in]	parentModel	フレームの属すタスクモデル
	bool				loadLandScaleWithFrame(AssetModelX::Frame* colFrame, TaskModelX* parentModel);

	//! フレームからデータ読み込み
	//! @param	[in]	pModel		Xファイル
	//!	@param	[in]	max			最大座標(XZ)保存用
	//!	@param	[in]	min			最小座標(XZ)保存用
	//! @param  [in]	isWall		壁用かどうか
	//! @param  [in]	modelMat	モデル行列
	//! @param  [in]	joints		そのフレームの属すジョイント
	SystemCollision::Object loadData(AssetModelX::Frame* pModelFrame, Vector3& max, Vector3& min, bool isWall, Matrix& modelMat, vector<TaskModelX::Joint> joints);

	//! フレームからデータ読み込み(再帰なし)
	//! @param	[in]	pModel		Xファイル
	//!	@param	[in]	max			最大座標(XZ)保存用
	//!	@param	[in]	min			最小座標(XZ)保存用
	//! @param  [in]	isWall		壁用かどうか
	//! @param  [in]	modelMat	モデル行列
	//! @param  [in]	joints		そのフレームの属すジョイント
	SystemCollision::Object loadDataOneFrame(AssetModelX::Frame* pModelFrame, Vector3& max, Vector3& min, bool isWall, Matrix& modelMat, vector<TaskModelX::Joint> joints);


	//! Xファイルからポリゴンデーターを返す
	//! @param	[in]	pModel		Xファイル
	//!	@param	[in]	dst			読み込み結果保存先
	//!	@param	[in]	max			最大座標(XZ)保存用
	//!	@param	[in]	min			最小座標(XZ)保存用
	//! @param  [in]	isWall		壁用かどうか
	SystemCollision::Object	loadXfile(TaskModelX* pModel, vector<SystemCollision::Object>& _dst, Vector3& max, Vector3& min, bool isWall);

	//! フレームからポリゴンデータを返す
	//! @param	[in]	colFrame	Frame
	//!	@param	[in]	dst			読み込み結果保存先
	//!	@param	[in]	max			最大座標(XZ)保存用
	//!	@param	[in]	min			最小座標(XZ)保存用
	//! @param  [in]	isWall		壁用かどうか
	//! @param  [in]	parentModel	そのフレームが属すモデル
	SystemCollision::Object loadWithFrame(AssetModelX::Frame* colFrame, vector<SystemCollision::Object>& _dst, Vector3& max, Vector3& min, bool isWall, TaskModelX* parentModel);

	//! オブジェクトの消去
	void				deleteObject();
	////!	オブジェクトの取得
	//Collision::Object*	getObject(s32 index) const {return &_objects[index]; }

	////!	オブジェクトの個数を取得
	//s32					getObjectCount() const { return _objects.size(); }

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{
	
	//!	指定した座標とモデルの当たり判定と補正
	//!	@param	[in]	positon		チェックする位置
	//!	@param	[in]	heightLimit	判定上限高さ
	//! @param	[in]	myGridNum	グリッド番号
	//! @param	[in]	isHitGround 地面に当たったかどうか		
	void				checkCollisionModel(Vector3& position, f32 radius, bool& isHitGround);

	

	//! インスタンス取得
	static SystemCollision*	getInstance()
	{
		static SystemCollision _myInstance;
		return &_myInstance;
	}


	//! 最後にヒットした座標の取得
	Vector3				getLastHitPosForWall()	{ return _LastHitPosForWall; }


	//! 球同士のあたり判定
	//!	@param	[in]	cubePos1	一つ目の球の座標
	//!	@param	[in]	cubePos2	二つ目の球の座標
	//!	@param	[in]	cubeRadius1	一つ目の球の半径
	//!	@param	[in]	cubeRadius2	二つ目の球の半径
	//!	@retval			true : 当たってる	false : 当たっていない
	bool				HitSphere(Vector3& spherePos1, Vector3& spherePos2, f32 sphereRadius1, f32 sphereRadius2);

	//! 円同士のあたり判定
	//!	@param	[in]	cubePos1	一つ目の円の座標
	//!	@param	[in]	cubePos2	二つ目の円の座標
	//!	@param	[in]	cubeRadius1	一つ目の円の半径
	//!	@param	[in]	cubeRadius2	二つ目の円の半径
	//!	@retval			true : 当たってる	false : 当たっていない
	bool				HitCircle(Vector3& circlePos1, Vector3& circlePos2, f32 circleRadius1, f32 circleRadius2);

	//@}
	
private:
	
	static	LandScape*					_pLandScape;		//!< ランドスケープ
	static	CollisionWall*				_pColWall;			//!< 壁
	Vector3								_LastHitPosForWall;	//!< 最後に当たった座標
	

};

typedef vector<SystemCollision::Object>	vecCol;			//!< 当たり判定オブジェクトのvector

//-----------------------------------------------------------------------------
// アクセスインターフェイス
//-----------------------------------------------------------------------------
__forceinline
SystemCollision*	GmSystemCollision()
{
	return SystemCollision::getInstance();
}


//-----------------------------------------------------------------------------
//! 当たり判定情報
//-----------------------------------------------------------------------------
struct CollisionData
{
	Triangle			_triangle;			//!< ポリゴンデータ
	u32					_myFrameNumber;		//!< 自分の属すフレームの番号
	bool				_isActive;			//!< アクティブかどうか
};

//-----------------------------------------------------------------------------
//!	物体データオブジェクト
//-----------------------------------------------------------------------------
class SystemCollision::Object
{
public:
	//!	コンストラクタ
	Object() {}

	//! 当たり判定情報を追加
	void		addColData(const CollisionData& col)
	{
		_colData.push_back(col);
	}

	//! リストの初期化
	void		resetColData()
	{
		_colData.clear();
	}

	//! 当たり判定情報取得
	CollisionData*		getColData(u32 index)
	{
		return &_colData[index];
	}

	//! 当たり判定情報の数を取得
	s32					getColDataCount() const 
	{
		return _colData.size();
	}

	//! 当たり判定情報配列取得
	vector<CollisionData>	getColDataVec() const
	{
		return _colData;
	}

private:
	vector<CollisionData>	_colData;
};


//=============================================================================
//	END OF FILE
//=============================================================================