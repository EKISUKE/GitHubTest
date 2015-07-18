//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskModelX.h
//!	@brief	3Dモデル(Xファイル形式)
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//---- プロトタイプ宣言
class AssetModelX;
class Frame;

//--------------------------------------------------------------
//! アニメーション再生の属性
//--------------------------------------------------------------
enum ANIMATION_PLAY_TYPE {
	ANIMATION_PLAY_DEFAULT,		//!< アニメーションデータに準拠する
	ANIMATION_PLAY_REPEAT,		//!< リピート再生
};

//-----------------------------------------------------------------------------
//!	モデルクラス
//-----------------------------------------------------------------------------
class TaskModelX : public TaskBase
{
	friend class SystemCollision;
public:
	//-------------------------------------------------------------
	//!	@name	初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	TaskModelX(void);
	//! デストラクタ
	virtual ~TaskModelX(void);

	//@}
	//-------------------------------------------------------------
	//!	@name	タスク関数
	//-------------------------------------------------------------
	//@{

	//! 初期化
	virtual	bool		init(void);
	//! 並列非同期実行
	virtual	void		asyncUpdate(void);
	//! 直列同期実行
	virtual	void		update(void);
	//! 描画
	virtual	void		render(bool	isShadow);

	//! MinMax計算
	void				calcMinMax();
	//! デバッグ描画
	void				debugRender();
	//! デバッグ描画(2D)
	void				debugRender2D(void);

	//@}
	//-------------------------------------------------------------
	//!	@name	取得・設定
	//-------------------------------------------------------------
	//@{

	//! モデルデータを設定
	bool				setModel(AssetModelX* pModel);
	//! 拡大率を設定
	void				setScale(f32 scale) { _scale = scale; }

	//! フレームを移動
	bool				setOffset(GM_CSTR frameName, Vector3& offset) { return _pModel->setOffsetByName(frameName, offset); }
	//! フレームを回転
	bool				setRotate(GM_CSTR frameName, Vector3& offset) { return _pModel->setRotateByName(frameName, offset); }
	//! 動くかどうかフラグ設定
	bool				setDynamic(GM_CSTR frameName, bool& isDynamic) { return _pModel->setDynamicByName(frameName, isDynamic); }
	//! 回転行列を設定
	void				setRotateMatrix(const Matrix& m) { _rotateMatrix = m; }
	//! ワールド行列を設定
	void				setWorldMatrix(const Matrix& m) { _worldMatrix = m; }

	//! シャドウマッピングの有効化フラグ設定
	void				setShadowEnable(const bool& enable) { _shadowEnable = enable; }

	//! モデルデータを取得
	AssetModelX*		getModel(void) const { return _pModel; }
	//! 拡大率を取得
	f32					getScale(void) const { return _scale; }



	//! 回転行列を取得
	const Matrix&		getRotateMatrix(void) const { return _rotateMatrix; }

	//! ワールド行列を取得
	const Matrix&		getWorldMatrix(void) const { return _worldMatrix; }


	//@}
	//-------------------------------------------------------------
	//!	@name	アニメーション制御関連
	//-------------------------------------------------------------
	//@{

	//! 名前からアニメーションを検索
	//!	@param	[in]	name	アニメーション名
	//!	@return アニメーション番号 (-1:一致する名前がなく検出できなかった)
	s32					searchAnimation(GM_CSTR name);

	//! アニメーションを再生
	//!	@param	[in]	animatioNumber	アニメーション番号(負の値ならアニメーションなし)
	//!	@param	[in]	playType		アニメーション再生属性(ANIMATION_PLAY_TYPE)
	//!	@param	[in]	interpolate		true:直前のアニメーションとの補間あり false:補間なし
	//!	@retval	true	成功
	//!	@retval	false	失敗（その番号にアニメーション情報が無い）
	bool				setAnimation(s32 animationNumber, ANIMATION_PLAY_TYPE playType=ANIMATION_PLAY_DEFAULT, bool interpolate=true);

	//! アニメーション速度を変更
	//!	@param	[in]	ratio	再生速度倍率(1.0f=等速)
	void				setAnimationSpeed(f32 ratio) { _animationSpeed = ratio; }

	//! シャドウのビュー行列設定
	//!	@param	[in]	viewMatrix	シャドウのビュー行列のポインタ
	static void			setShadowViewMatrix(Matrix* viewMatrix) {
		for(u32 i=0; i<_splitCount; ++i) {
			_shadowView[i] = viewMatrix[i]; 
		}
	}

	//! バイアス行列の設定
	//!	@param	[in]	biasMatrix	バイアス行列
	static void			setBiasMatrix(const Matrix& biasMatrix) { _biasMatrix = biasMatrix; }

	//! 投影行列を設定
	//! @param	[in]	projMatrix	投影行列のポインタ
	static void			setShadowProj(Matrix* projMatrix)		{ 
		for(u32 i=0; i<_splitCount; ++i) {
			_shadowProj[i] = projMatrix[i];
		}
	}

	//! デプステクスチャハンドル設定
	//! @param	[in]	texHandle	テクスチャハンドル
	static void			setDepthTexHandle(const GLuint& texHandle)	{ _depthTexHandle = texHandle; }
	
	//! 再生中のアニメーション番号を取得
	//!	@return アニメーション番号 (-1:アニメーションなし)
	s32					getAnimation(void)		const { return _animationNumber; }

	//! アニメーション速度を取得
	f32					getAnimationSpeed(void) const { return _animationSpeed; }

	//! アニメーションの進み具合取得
	//! @return アニメーションの進み具合 0.0f ～ 1.0f
	f32					getAnimationRate(void)	const { return _animationRate; }

	//! 再生中のアニメーション名取得
	GM_CSTR				getAnimationName(void)	const;
	//@}
private:
	//! フレームに属するメッシュを描画
	void						_renderFrame(AssetModelX::Frame* pFrame);
	//!	メッシュを描画
	void						_renderMesh(AssetModelX::Mesh* pMesh);
private:

	//---- シャドウ用
	bool						_shadowEnable;				//!< シャドウ使うかどうか

	//---- モデル情報
	AssetModelX*				_pModel;					//!< モデルデータ
	f32							_scale;						//!< 拡大率

	//---- 行列
	Matrix						_rotateMatrix;				//!< 回転行列
	Matrix						_worldMatrix;				//!< 姿勢行列

	//---- シャドウ用行列
	static const u32			_splitCount = 3;			//!< カスケードの分割数
	static GLuint				_depthTexHandle;			//!< デプステクスチャのハンドル
	static Matrix				_biasMatrix;				//!< シャドウのビュー行列
	static Matrix				_shadowView[_splitCount];	//!< シャドウのビュー行列
	static Matrix				_shadowProj[_splitCount];	//!< シャドウのプロジェクション行列
	

	//---- アニメーション
	bool						_isAnimationRepeat;			//!< アニメーションをリピート再生するかどうか
	ANIMATION_PLAY_TYPE			_animationPlayType;			//!< アニメーションの属性
	s32							_animationNumber;			//!< アニメーション番号
	f32							_animationTime;				//!< アニメーションの再生中の時間
	f32							_animationSpeed;			//!< アニメーション再生速度
	f32							_animationRate;				//!< 再生中のアニメーションの再生比率
	GM_CSTR						_animationName;				//!< 再生中のアニメーション名

	//---- アニメーション補間
	f32							_animationBlendRatio;		//!< アニメーションブレンド係数

	//---- ボーン関節
	struct Joint;
	vector<Joint>				_joints;					//!< アニメーションボーン関節配列
	vector<Matrix>				_skinLocalMatrices;			//!< ボーン関節行列（ローカル）

	//---- 描画バッファオブジェクト
	VertexArrayObject*			_pVAO;
};

//-----------------------------------------------------------------------------
//!	ジョイント
//!	@brief	ボーン関節
//!	ボーン関節は各関節の行列情報の他に、アニメーション補間のための
//!	ブレンド元の姿勢情報も持つようにする。
//-----------------------------------------------------------------------------
struct TaskModelX::Joint {
	//! コンストラクタ
	Joint(void)
	: _animationMatrix(Matrix::IDENTITY)		// アニメーション行列
	, _transformMatrix(Matrix::IDENTITY)		// フレーム行列
	, _rotation       (Quaternion::IDENTITY)	// アニメーション回転
	, _position       (0.0f, 0.0f, 0.0f)		// アニメーション移動
	, _scale          (1.0f, 1.0f, 1.0f)		// アニメーション拡縮
	, _blendRotation  (Quaternion::IDENTITY)	// アニメーション回転(ブレンド元)
	, _blendPosition  (0.0f, 0.0f, 0.0f)		// アニメーション移動(ブレンド元)
	, _blendScale     (1.0f, 1.0f, 1.0f)		// アニメーション拡縮(ブレンド元)
	{}

	//---- 行列化したデータ
	Matrix			_animationMatrix;			//!< アニメーション行列
	Matrix			_transformMatrix;			//!< フレーム行列

	//---- アニメーション姿勢
	// (A) 現在の再生中のアニメーション姿勢
	Quaternion		_rotation;					//!< アニメーション回転
	Vector3			_position;					//!< アニメーション移動
	Vector3			_scale;						//!< アニメーション拡縮
	//    ↑↑↑ (B)から(A)に向かって補間
	// (B) アニメーション変更前の姿勢
	Quaternion		_blendRotation;				//!< アニメーション回転(ブレンド元)
	Vector3			_blendPosition;				//!< アニメーション移動(ブレンド元)
	Vector3			_blendScale;				//!< アニメーション拡縮(ブレンド元)
};

//===========================================================================
//	END OF FILE
//===========================================================================
