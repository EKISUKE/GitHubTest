//-----------------------------------------------------------------------------
//!
//!	@file	CascadedShadow.h
//!	@brief	カスケードシャドウ
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


#define DEPTH_SIZE 2048		//!< シャドウテクスチャサイズ


//-----------------------------------------------------------------------------
//! カスケードシャドウ
//-----------------------------------------------------------------------------
class CascadedShadow
{
private:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//---- 外部からの作成削除禁止
	//! コンストラクタ
	CascadedShadow();
	//! コピーコンストラクタ
	CascadedShadow(CascadedShadow& src){}
	//! デストラクタ
	virtual ~CascadedShadow();
	
public:
	//! 正射影行列の初期化
	void	initOrthoMatrix();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! ビュー行列を計算して作る
	//! @param	[in]	lightDir	ライト方向
	//! @param	[in]	viewDir		ビュー方向
	//! @param	[in]	camPos		カメラ座標
	void					calcAndSetViewMatrix(Vector3& lightDir, Vector3& viewDir, Vector3& camPos);
	
	//! 外接円を計算する
	//! @param	[in]	p1	座標1
	//! @param	[in]	p2	座標2
	//! @param	[in]	p3	座標3
	//!	@retval	球構造体
	Sphere&					calcCircumscribeCircle(Vector3& p1, Vector3& p2, Vector3& p3);
	

	//! シャドウテクスチャを作成
	bool					createShadowTexture();

	//! 視錐台の8点の更新
	void					updateFrustumPoints(Frustum& f, Vector3& center, Matrix& cameraMatrix);

	//! 分割距離の更新
	void					updateSplitDist(f32 nd, f32 fd);

	//! クリップ行列の適用
	f32						applyCropMatrix(Frustum &f, LightBase* pLight);

	//! マトリックス等の更新
	void					update(Matrix& modelMatrix);

	//! カメラからの遠クリップ面距離計算
	void					calcFarDist();

	//! デバッグ描画
	void					debugShadow();

	//@}
	//-------------------------------------------------------------
	//! @name 開始・終了
	//-------------------------------------------------------------
	//@{
	//! シャドウ書き込み開始
	void					begin(LightBase* pLight);
	//! シャドウ書き込み終了
	void					end();
	//@}
	//-------------------------------------------------------------
	//! @name 取得/設定
	//-------------------------------------------------------------
	//@{

	//! モデルの最大最少設定
	void					setMinMax(f32 minX, f32 maxX, f32 minY, f32 maxY, f32 minZ, f32 maxZ) {
		_MinMax[0] = minX;
		_MinMax[1] = maxX;
		_MinMax[2] = minY;
		_MinMax[3] = maxY;
		_MinMax[4] = minZ;
		_MinMax[5] = maxZ;
	}
	

	//! インスタンス取得
	static CascadedShadow*	getInstance()
	{
		static CascadedShadow _myInstance;
		return &_myInstance;
	}
	
	//! デプステクスチャ取得
	GLuint					getDepthTexture()		 const { return _depthTexture->getDepthBuffer(); }

	//! 分割数取得
	s32						getSplitCount()			 const { return SPLIT_COUNT; }
	
	//! プロジェクション行列取得
	Matrix					getProjMatrix(u32 Index) const { return _depthProj[Index]; }

	//! ビュー行列取得
	Matrix					getViewMatrix(u32 Index) const { return _depthView[Index]; }

	//! バイアス行列取得
	Matrix					getBiasMatrix()			 const { return _biasMatrix; }

	//! シャドウマップを探す
	//!	@param	[in]	search	探すシャドウのポインタ
	bool					sameShadowMap(ShadowMap* search);

	

	//@}
private:
	static const u32		SPLIT_COUNT = 3;			    //!< 分割数

	Frustum					f[SPLIT_COUNT];					//!< 視錐台
	f32						_farDist[SPLIT_COUNT];			//!< カメラからの遠クリップ面リスト
	f32						_MinMax[6];					    //!< 最大値最小値(0:minX 1:maxX 2:minY 3:maxY 4:minZ 5:maxZ)
	f32						_centerLength[SPLIT_COUNT];		//!< 中点までの距離
	u32						_currentShadowNum;			    //!< 現在使用中のシャドウマップ番号
	f32						_split_weight;				    
	Matrix					_modelMatrix;				    //!< ワールド行列
	LightBase*				_pLight;					    //!< ライト
	ShadowMap*				_pShadow[SPLIT_COUNT];		    //!< シャドウマップ
	ShadowMap*				_pCurrentShadow;			    //!< 現在のシャドウマップ
	static const Matrix		_biasMatrix;				    //!< バイアス行列(テクスチャ座標からスクリーン座標にするためのもの)
	Texture*				_depthTexture;					//!< デプステクスチャ
	//GLuint					_depthFrameBuffer;				//!< デプスフレームバッファ
	//GLuint					_depthTexture;					//!< デプステクスチャ
	Matrix					_depthProj[SPLIT_COUNT];		//!< デプスプロジェクション行列(※SPLIT_COUNTを変更したらシェーダも変更必須)
	Matrix					_depthView[SPLIT_COUNT];		//!< デプスビュー行列(ビュー行列は変更がないので、ひとつだけ
	Matrix					_depthMVP;						//!< デプスビュー行列
	Size<s32>				_depthTexSize;					//!< デプステクスチャサイズ
	bool					_isSetRender;					//!< レンダーターゲットを設定したかどうか
};

//-----------------------------------------------------------------------------
// アクセスインターフェイス
//-----------------------------------------------------------------------------
__forceinline
CascadedShadow* ICasCaded()
{
	return CascadedShadow::getInstance();
}


//=============================================================================
//	END OF FILE
//=============================================================================