//-----------------------------------------------------------------------------
//!
//!	@file	SceneOption.h
//!	@brief	オプション
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
//! オプション
//-----------------------------------------------------------------------------
class SceneOption : public SceneBase, public TaskFilter // Filterはぼかしのために入れているが別の方法を考え中
{
public:
	GM_TYPE(SceneOption)

	//! 選択状態
	enum SELECT_STATE{
		SELECT_FULLSCREEN,
		SELECT_MASTER_VOLUME,	
		SELECT_BGM_VOLUME,		
		SELECT_SE_VOLUME,		
		SELECT_MAX
	};


	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	SceneOption();
	//! デストラクタ
	virtual				~SceneOption();
	//! 初期化
	virtual bool		init();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 更新
	virtual void		update();
	//! 解放
	//virtual void		cleanup();
	//! 更新
	virtual void		update(Controller& control);
	//! 描画
	virtual void		render();
	//! フィルターの影響しない描画
	virtual void		drawNoFilter();

private:
	//! 今現在のバックバッファをコピーしてぼかして背景に使う
	void				createBackGroundImage();
	//! キー判定
	void				checkKey(Controller& control);
	//! 左右キーによる音量調節
	bool				changeVolumeByKey(f32& volume);
	//! 選択しているものによる更新
	void				updateSelect();
public:

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//---- 取得
	SELECT_STATE		getSelectState() { return _select; }

	//@}
private:
	Texture*			_pBackGround;					//!< オプション背景
	Texture*			_pTitle;						//!< タイトル画像
	Texture*			_font[SELECT_MAX];				//!< 画面内文字用
	Texture*			_pSettingBar;					//!< セッティングのゲージバー
	Texture*			_pSettingCheckBox;				//!< セッティングのチェックボックス
	SELECT_STATE		_select;						//!< 選択中の項目
	bool				_nowJump;						//!< 別のシーンから来たかどうか
	bool				_isFullScreen;					//!< フルスクリーンかどうか
	//---- キー判定用
	bool				_keyUp;							//!< 上キー
	bool				_keyDown;						//!< 下キー
	bool				_keyRight;						//!< 右キー
	bool				_keyLeft;						//!< 左キー
	bool				_keySelect;						//!< 決定キー
	bool				_keyBack;						//!< 戻るキー

	TaskFilterGaussian	_gaussianFilter;				//!< ぼかしフィルター
};

//=============================================================================
//	END OF FILE
//=============================================================================