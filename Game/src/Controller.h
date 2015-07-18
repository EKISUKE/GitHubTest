//-----------------------------------------------------------------------------
//!
//!	@file	Controller.h
//!	@brief	コントローラー
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	@class	コントローラー
//-----------------------------------------------------------------------------
class Controller
{
public:

	//! コマンド列挙体
	//@{
    enum PAD_STATE{
		// デジタル
		PAD_UP,				//	十字キーの上
		PAD_DOWN,			//	十字キーの下
		PAD_LEFT,			//	十字キーの左
		PAD_RIGHT,			//	十字キーの右
		PAD_START,			//	Start
		PAD_BACK,			//	Back
		PAD_LEFT_THUMB,		//	左スティック
		PAD_RIGHT_THUMB,	//	右スティック
		PAD_LB,				//	LB
		PAD_RB,				//	RB
        PAD_A,				//	Aボタン
        PAD_B,				//	Bボタン
        PAD_X,				//	Xボタン
        PAD_Y,				//	Yボタン
		// アナログ
		PAD_LEFT_U_STICK,	//	左スティックの上
		PAD_LEFT_D_STICK,	//	左スティックの下
		PAD_LEFT_L_STICK,	//	左スティックの左
		PAD_LEFT_R_STICK,	//	左スティックの右
		PAD_RIGHT_U_STICK,	//	右スティックの上
        PAD_RIGHT_D_STICK,	//	右スティックの下
		PAD_RIGHT_L_STICK,	//	右スティックの左
        PAD_RIGHT_R_STICK,	//	右スティックの右
        PAD_LT,				//	左トリガー
		PAD_RT,				//	右トリガー
    };
	//@}

	//!	入力モード
	//@{
	enum MODE{
		DIGITAL,
		ANALOG,
	};
	//@}

public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//!	コンストラクタ
    Controller(s32 playerNum);

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 更新
	void	Update();

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//!	接続確認関数
    bool	IsConnected();
	//!	入力チェック関数
	bool	GetPadState(PAD_STATE p_state);
	//! 今押されたかチェック
	virtual bool	GetPushState(PAD_STATE p_state);
	
	//!	振動
	void	EnableVibration( f32 L_vib, f32 R_vib );
	void	DisableVibration();

	//!	アナログ値を取得
	//! @param	[in]	Thumb		true:左スティック false:右スティック
	//!	@param	[in]	KnockSize	押し加減 0.0f～1.0f
	virtual Vector3	getStickState(bool Thumb, f32& KnockSize);
	
	//@}
protected :
	
	s32		_controllerNum;		//!<	接続コントローラー数
	MODE	mode;				//!<	入力モード

	s32		XInputPadState[14]; //!<	XInputのパッドの列挙が入っている
	bool	nowPadState[14];	//!<	今のキー
	bool	prevPadState[14];	//!<	ひとつ前のパッドの状態
	//! デジタルの入力チェック
	bool	GetDigitalState(XINPUT_STATE state, PAD_STATE p_state);
	//!	アナログの入力チェック
	bool	GetAnalogState(XINPUT_STATE state, PAD_STATE p_state);
	//!	XINPUTに変換
	void	To_XInput(PAD_STATE &p_state);
	//!	入力モードチェック関数
	void	CheckMode(PAD_STATE p_state);
};


//=============================================================================
//	END OF FILE
//=============================================================================