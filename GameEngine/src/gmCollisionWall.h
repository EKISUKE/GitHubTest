//-----------------------------------------------------------------------------
//!
//!	@file	gmCollisionWall.h
//!	@brief	衝突判定(壁)
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//! 衝突判定クラス(壁)
//-----------------------------------------------------------------------------
class CollisionWall
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	CollisionWall();
	//! デストラクタ
	virtual ~CollisionWall();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! Xファイルからポリゴンデーターを入力
	//! @param	[in]	pModel		Xファイル
	bool				load(TaskModelX*	pModel);

	//! フレームからポリゴンデータ入力
	//! @param	[in]	pFrame		フレーム
	//!	@param	[in]	parentModel	フレームの属すタスクモデル
	bool				loadWithFrame(AssetModelX::Frame* pFrame, TaskModelX* parentModel);

	//! デバッグ描画
	void				DebugRender();
	
	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//!	オブジェクトの取得
	SystemCollision::Object*	getObject(s32 index)   {return &_objects[index]; }

	//!	オブジェクトの個数を取得
	s32					getObjectCount() const { return _objects.size(); }

	//! その座標から一番近い三角形との衝突点の取得
	//!	@param	[in]	positon		チェックする位置
	//!	@param	[in]	radius		半径
	//!	@retval			衝突点
	Vector3				getHitPos(const Vector3& position, const f32 radius, bool isNear = true);
	//Vector3				getHitPos(const AABB aabb);

	//@}
private:
	vecCol						_objects;			//!< 読み込んだモデルのポリゴン保存用
	AABBTree					_aabbTree;			//!< 探索用
	f32							_width;				//!< ポリゴンの幅
	f32							_height;			//!< ポリゴンの高さ
	Vector3						_max;				//!< 重心の最大(XZ)
	Vector3						_min;				//!< 重心の最小(XZ)
	s32							_offsetX;			//!< マイナスのセンター座標オフセット値
	s32							_offsetZ;			//!< マイナスのセンター座標オフセット値
	s32							_divX;				//!< X方向の分割量
	s32							_divZ;				//!< Z方向の分割量
	s32							_DivCount;			//!< 分割数
	Triangle*					_LastHitTriangle;	//!< 最後に当たった三角形
	Vector3						_checkIndex;		//!< 調べているインデックス番号
};


//=============================================================================
//	END OF FILE
//=============================================================================