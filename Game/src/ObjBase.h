//-----------------------------------------------------------------------------
//!
//!	@file	ObjBase.h
//!	@brief	オブジェクト基底
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

class CollisionObject;
//-----------------------------------------------------------------------------
//!	オブジェクト基底
//-----------------------------------------------------------------------------
class	ObjBase : virtual public IObj , public CollisionObject, public Base
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	ObjBase()
	: _position		(0.0f, 0.0f, 0.0f)
	, _oldPosition	(0.0f, 0.0f, 0.0f)
	, _mov			(0.0f, 0.0f, 0.0f)
	, _rotation		(0.0f, 0.0f, 0.0f)
	, _radius		(0.0f)
	{
	}

	//! デストラクタ
	virtual ~ObjBase() 
	{
	};

	//!	初期化
	virtual bool	Initialize() { return true; }

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 更新
	virtual void	Update()	 {};
	//!	描画
	virtual void	Render()	 {};

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//---- 取得
	Vector3			getPosition	() const { return _position; }
	Vector3			getMov		() const { return _mov; }
	Vector3			getRotation () const { return _rotation; }
	f32				getRadius	() const { return _radius; }

	Vector3			getHitPosition	  () const { return _hitPosition;	 }
	f32				getHitRadius      () const { return _hitRadius;		 }

	//@}
protected:

	//---- メンバー変数
	Vector3			_position;
	Vector3			_oldPosition;
	Vector3			_mov;
	Vector3			_rotation;
	f32				_radius;

	Vector3			_hitPosition;		//!< あたり判定用座標
	f32				_hitRadius;			//!< あたり判定用
	
};

//=============================================================================
//	END OF FILE
//=============================================================================