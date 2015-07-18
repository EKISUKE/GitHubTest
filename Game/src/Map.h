//-----------------------------------------------------------------------------
//!
//!	@file	Map.h
//!	@brief	マップ
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

class Castle;
class CastleWall;

//-----------------------------------------------------------------------------
//! マップ
//-----------------------------------------------------------------------------
class Map : public ObjBase
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	Map();

	//! デストラクタ
	virtual			~Map();
	//! 初期化
	virtual bool	Initialize();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 更新
	virtual void	Update();
	//! 描画
	virtual void	Render(bool isShadow);
	//! デバッグ描画
	virtual void	debugRender();

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! 城インスタンスを取得
	Castle*			getCastle() {return _castle;}

	//! 壁情報取得
	CastleWall*		getWall() { return _myWall; }

	////! 壁情報取得
	//CastleWall*	getWall(s32 Index) { return _myWall[Index]; }

	////! 壁の数取得
	//s32					getWallCount() { return WALL_MAX; }
	//@}
protected:
	AssetModelX*		_pAssetMapModel;	//!< マップモデル(オリジナルデータ)
	TaskModelX*			_pTaskMapModel;		//!< マップモデル(描画移動)
	Castle*				_castle;			//!< 城
	//static const int	WALL_MAX = 36;		//!< 壁の最大数
	//CastleWall*			_myWall[WALL_MAX];	//!< 城壁
	CastleWall*			_myWall;			//!< 城壁
};

//-----------------------------------------------------------------------------
//! 城壁一つ分
//-----------------------------------------------------------------------------
class CastleWall  : public CollisionObject
{
public:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	//! @param	[in]	wall 城壁のフレームのポインター
	CastleWall(AssetModelX::Frame*	wall);

	//! デストラクタ
	virtual ~CastleWall();

	//! 初期化
	bool	Initialize();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 更新
	void	Update();

	//! デバッグ描画
	void	debugRender();

	//! 壁が崩れる
	void	Crash();

	//! ぶれ
	void	Vibration();
	
	//! ダメージ
	void	Damage(u32	damage);

	//@}
	//-------------------------------------------------------------
	//! @name 取得・参照
	//-------------------------------------------------------------
	//@{

	//! AABBデータ取得
	AABB&		getAABB()        { return _colAABB; }

	//! 壊れているかどうか
	bool		isCrash() const { return _isCrash; }



	//@}
protected:
	Vector3					_drawPosition;		//!< 描画座標
	f32						_rotation;			//!< 角度
	AssetModelX::Frame*		_wallModel;			//!< 壁モデル(フレームのポインタ)
	AABB					_colAABB;			//!< 当たり判定用AABB
	ObjectStatus*			_myStatus;			//!< ステータスの構造体
	bool					_isCrash;			//!< 破壊されたかどうか
	bool					_isDamage;			//!< ダメージ
};


//-----------------------------------------------------------------------------
//! 城
//-----------------------------------------------------------------------------
class Castle
{
public:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	//! @param	[in]	castle 城のフレームポインター
	Castle(AssetModelX::Frame*	castle);
	//! デストラクタ
	virtual		~Castle();
	//! 初期化
	bool		Initialize();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 更新
	void		Update();
	
	//! デバッグ描画
	void		debugRender();

	//! 城が崩れる
	void		Crash();
	
	//! ぶれ
	void		Vibration();

	//! ダメージ
	void		Damage(u32 damage);
	
	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! AABBデータ取得
	AABB&		getAABB()        { return  _colAABB; }

	//! 壊れているかどうか
	bool		isCrash() const  { return _isCrash; }

	//! アクティブかどうか
	bool		isActive() const { return _myStatus->_isActive; }

	//@}
protected:
	AssetModelX::Frame*	_castleModel;	//!< 城
	
	Vector3				_drawPosition;	//!< 描画座標
	AABB				_colAABB;		//!< 当たり判定用AABB
	ObjectStatus*		_myStatus;		//!< ステータスの構造体
	bool				_isCrash;		//!< 破壊されたかどうか
	bool				_isDamage;		//!< ダメージ
};


//-----------------------------------------------------------------------------
//! 城、城壁のみの当たり判定管理
//-----------------------------------------------------------------------------
class CastleCollisionManager
{
private:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	CastleCollisionManager(){}
	//! コピーコンストラクタ
	CastleCollisionManager(CastleCollisionManager& src){}
	//! デストラクタ
	virtual ~CastleCollisionManager(){}

	//@}
public:

	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 攻撃衝突判定とダメージ
	//! @param	[in]	attackAABB	判定する攻撃AABB
	//!	@param	[in]	offset		攻撃の座標
	//!	@param	[in]	damage		当たったものに与えるダメージ量
	void	hitAttackAndDamage(AABB& attackAABB, Vector3& offset, u32 damage);

	//! 外に出れないようにするための関数
	void	hitOutRange(Vector3& position);

	//@}
	//-------------------------------------------------------------
	//! @name 取得･設定
	//-------------------------------------------------------------
	//@{

	//! インスタンス取得
	static CastleCollisionManager* getInstannce()
	{
		static CastleCollisionManager _myInstance;
		return &_myInstance;
	}

	//! 城オブジェクト(城壁など)の近くかどうか(XButton表示用)
	//!	@param	[in]	aabb		当たり判定用AABB
	//!	@param	[in]	offset		AABBの座標
	//!	@param	[in]	addBouns	追加当たり判定
	//!	@return true:近い false:遠い
	bool		isNearCastleObject(AABB& aabb, Vector3& offset, f32 addBounds = 100.0f);

	//! 現在アクティブな当たり判定と当たっているどうか
	Vector3		getSinkVal(AABB& aabb, bool& hitGround, Vector3& offset = Vector3(0,0,0));

	//! 城と城壁の当たり判定設定
	void		setCastleCol(Castle* colCastle, CastleWall* colCastleWall);

	//@}
private:
	Castle*		_castle;		//!< 城のポインタ	
	CastleWall*	_castleWall;	//!< 城壁のポインタ
};

__forceinline
//-----------------------------------------------------------------------------
// アクセスインターフェイス
//-----------------------------------------------------------------------------
CastleCollisionManager* ICastleCol()
{
	return CastleCollisionManager::getInstannce();
}


//=============================================================================
//	END OF FILE
//=============================================================================