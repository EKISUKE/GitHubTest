//-----------------------------------------------------------------------------
//!
//!	@file	gmCameraBase.h
//!	@brief	カメラ基底
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
//!	カメラ基底
//-----------------------------------------------------------------------------
class CameraBase
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	CameraBase();

	//! 仮想デストラクタ
	virtual ~CameraBase(){};

	//! 初期化
	virtual bool	init(Vector3& lookAt, Vector3& _rot);
	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 更新
	virtual void		update(Controller& control, Vector3& PlayerPos, const f32& PlayerRot,  POINT mouseOffset);
	//! デバッグ表示
	virtual void		DebugRender();

	//! バイブレーション有効化
	//!	@param	[in] vibrationPower バイブレーション量
	virtual void		Enablevibration(f32 vibrationPower);

	//! バイブレーション無効化
	//!	@param	[in] vibrationPower バイブレーション量
	virtual void		Disablevibration();

protected:
	//! ゆっくりと目標座標まで移動する
	//! @param	[in]	目標座標
	//! @return	true: 移動完了 false: 移動中
	bool		moveGoal(Vector3& goalPos);
public:


	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//---- セット
	//! 位置を指定
	void			setPosition(const Vector3& position)			{ _position = position; }
	//! 注視点を指定
	void			setLookAt  (const Vector3& lookAt  )			{ _lookAt	 = lookAt;	}
	//! 世界の上方向を設定
	void			setWorldUp (const Vector3& worldUp )			{ _worldUp	 = worldUp;	}
	//!	注視点との距離変更
	void			changeLookAtLength(const f32& change)			
	{
		_lookAtLength += change;
		setLookAtLengthMax(_lookAtLengthMax + change);
	}
	//! 注視点との距離最大値設定
	void			setLookAtLengthMax(const f32& lookAtLengthMax)	{ _lookAtLengthMax = lookAtLengthMax; }

	//---- 取得
	//! 注視点との距離取得
	f32				getLookAtLength() const { return _lookAtLength; }

	//! 角度を取得
	Vector3			getRotation () const	{ return _rot;		}
	//! 位置を取得
	Vector3			getPosition	() const	{ return _position; }
	//! 注視点を取得
	Vector3			getLookAt	() const	{ return _lookAt;   }
	//! 世界の上方向を取得
	Vector3			getWorldUp	() const	{ return _worldUp;  }

	//! 姿勢行列を取得
	const Matrix&	getMatrix	 () const	{ return _matrix; }
	//! ビュー行列を取得
	const Matrix&	getViewMatrix() const	{ return _viewMatrix; }
	//! 投影行列の取得
	const Matrix&	getProjMatrix() const	{ return _projMatrix; }
	//! ビュー×投影行列の取得
	Matrix			getViewProjMatrix()		{ return _viewMatrix * _projMatrix; }
	//! 視錐台の取得
	const Frustum&	getViewFrustum() const	{ return _viewFrustum; }


	//@}

protected:

	//---- 基本データ
	Vector3			_position;			//!< 位置座標
	Vector3			_cameraOffset;		//!< オフセット値
	Vector3			_lookAt;			//!< 注視点
	Vector3			_worldUp;			//!< 世界の上方向の基準軸
	Vector3			_rot;				//!< 角度

	f32				_radius;			//!< 半径
	f32				_lookAtLength;		//!< 注視点との距離
	f32				_lookAtLengthMax;	//!< 注視点との距離の最大値


	Matrix			_matrix;			//!< 姿勢行列
	Matrix			_viewMatrix;		//!< ビュー行列
	Matrix			_projMatrix;		//!< 投影行列
	bool			_isVib;				//!< バイブレーション中かどうか

	Frustum			_viewFrustum;		//!< 視錐台
};

//=============================================================================
//	END OF FILE
//=============================================================================