//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilter.h
//!	@brief	ポストエフェクトフィルタ基底
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	ポストエフェクトフィルター基底
//-----------------------------------------------------------------------------
class TaskFilter : public TaskBase
{
public:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	TaskFilter();

	//! デストラクタ
	virtual ~TaskFilter();

	//@}
	//-------------------------------------------------------------
	//!	@name	タスク関数
	//! 関数を継承してカスタムします。
	//-------------------------------------------------------------
	//@{

	//! 初期化
	virtual bool		setup(Size<s32>& workTexResolution = GmWindow()->getResolution());

	//! 解放
	virtual void		cleanup();

	//! 並列更新
	virtual void		asyncUpdate(){}

	//! 直列更新
	virtual void		update(){}

	//! 描画
	virtual void		render();

	//@}

	//! ワークテクスチャを取得
	Texture*			getWorkTexture(s32 index) { return _pWorkTexture[index]; }

protected:
	//-------------------------------------------------------------
	//!	@name	内部関数
	//-------------------------------------------------------------
	//@{

	//! バックバッファからワークテクスチャにコピー
	//! @param	[in]	shaderPass  コピーするときに使うシェーダ番号
	//!	@param	[in]	mipLevel	コピー先のミップレベル
	//!	@param	[in]	isPrevFrame	1フレーム前のものをコピーするかどうか
	void				copyToWorkTexture(s32 shaderPass, s32 mipLevel = 0, bool isPrevFrame = false);

	//! ワークテクスチャを全画面に張り付け
	//! @param	[in]	index				ワークテクスチャの番号(0 or 1)
	//! @param	[in]	mipLevel			ミップレベル
	//!	@param	[in]	shaderPass			シェーダーパス番号(-1ならこの中でシェーダを開始、終了しない)
	//!	@param	[in]	pSubTexture			サブテクスチャ
	void				put(s32			index, s32 mipLevel, s32 shaderPass = -1, Texture* pSubTexture=nullptr);
	void				put(Texture* pTexture, s32 mipLevel, s32 shaderPass = -1, Texture* pSubTexture=nullptr);

	//! 描画ターゲットの設定
	//! @param	[in]	index		ワークテクスチャの番号(0 or 1)
	//!	@param	[in]	mipLevel	ミップレベル
	void				setRenderTarget(s32 index, s32 mipLevel);
	void				setRenderTarget(Texture* pRenderTarget, s32 mipLevel=0);

	//! 描画ターゲットをバックバッファに戻す
	void				setBackBuffer();

	//@}
protected:
	static const s32	REDUCTION_LEVEL = 7;

private:
	Texture*			_pWorkTexture[2];	//!< 一時ワークテクスチャ(RenderTarget)
};


//=============================================================================
//	END OF FILE
//=============================================================================