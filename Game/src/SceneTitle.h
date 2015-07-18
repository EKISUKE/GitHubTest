//-----------------------------------------------------------------------------
//!
//!	@file	SceneTitle.h
//!	@brief	タイトル画面
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
//! タイトル
//-----------------------------------------------------------------------------
class SceneTitle : public SceneBase
{
public:
	GM_TYPE(SceneTitle)

	enum SELECT_STATE{
		SELECT_NEWGAME,	//	初回から開始ボタン選択中
//		SELECT_CONTINUE,//	続きあから開始ボタン選択中(作成予定)
		SELECT_OPTION,	//	オプションボタン選択中
		SELECT_EXIT,	//	終了ボタン選択中
		SELECT_MAX
	};


	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	SceneTitle();
	//! デストラクタ
	virtual				~SceneTitle();
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
	//! キー判定
	void				checkKey(Controller& control);
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
	Texture*			_backGroundImage;				//!< タイトル画像
	Texture*			_title;							//!< タイトル画像
	Texture*			_font[SELECT_MAX];				//!< 画面内文字用
	Texture*			_Abutton;						//!< Aボタン説明用
	GLFont*				_titleFont;						//!< フォントの描画用
	SELECT_STATE		_select;						//!< 選択中の項目
	bool				_cantSelect[SELECT_MAX];		//!< 選択できないようにする(特殊な場合)


	//---- キー判定用
	bool				_keyUp;							//!< 上キー
	bool				_keyDown;						//!< 下キー
	bool				_keySelect;						//!< 決定キー
	bool				_keyBack;						//!< 戻るキー
};

//=============================================================================
//	END OF FILE
//=============================================================================