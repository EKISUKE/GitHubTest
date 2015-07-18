//-----------------------------------------------------------------------------
//!
//!	@file	LightBase.h
//!	@brief	光源基底
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//!	光源基底
//-----------------------------------------------------------------------------
class LightBase
{
public:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	LightBase();

	//! デストラクタ
	virtual ~LightBase(){};

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 更新
	virtual void	update();

	//! デバッグ表示
	virtual void		DebugRender(){};

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//---- セット
	//! 位置を指定
	void			setPosition(const Vector3& position) { _position = position; }
	//! 注視点を指定
	void			setLookAt  (const Vector3& lookAt  ) { _lookAt	 = lookAt;	 }
	//! 世界の上方向を設定
	void			setWorldUp (const Vector3& worldUp ) { _worldUp	 = worldUp;	 }
	
	//---- 取得
	//! 角度を取得
	Vector3			getRotation () const { return _rot;		 }
	//! 位置を取得
	Vector3			getPosition	() const { return _position; }
	//! 注視点を取得
	Vector3			getLookAt	() const { return _lookAt;   }
	//! 世界の上方向を取得
	Vector3			getWorldUp	() const { return _worldUp;  }

	//! ライト方向取得
	Vector3			getLightDir () const { return _lightDir; }

	//! 姿勢行列を取得
	const Matrix&	getMatrix	 () const { return _matrix; }
	//! ビュー行列を取得
	const Matrix&	getViewMatrix() const { return _viewMatrix; }
	
	//@}

protected:
	//---- 基本データ
	Vector3			_position;		//!< 位置座標
	Vector3			_lookAt;		//!< 注視点
	Vector3			_worldUp;		//!< 世界の上方向の基準軸
	Vector3			_rot;			//!< 角度
	Vector3			_lightDir;		//!< ライト方向

	Matrix			_matrix;		//!< 姿勢行列
	Matrix			_viewMatrix;	//!< ビュー行列
};

//=============================================================================
//	END OF FILE
//=============================================================================