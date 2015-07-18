//-----------------------------------------------------------------------------
//!
//!	@file	gmController.cpp
//!	@brief	コントローラー
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"



//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
Controller::Controller(s32 playerNum)
{
	_controllerNum = playerNum -1;

	int	Index = 0;
	
	for( s32 i=0; i<16; i++ ){
		// 配列番号用の変数

		if( i == 10 || i == 11 ){
			continue;
		}

		XInputPadState[Index] = 0x001 << i;

		// 次の番号へ
		Index++;
	}

	// 今押しているキーと前のフレームでのキー初期化
	for( s32 i=0; i<PAD_MAX; i++ ){
		prevPadState[i] = false;
		nowPadState[i]  = false;
	}
}

//-----------------------------------------------------------------------------
//!	更新
//-----------------------------------------------------------------------------
void Controller::Update()
{
}

//-----------------------------------------------------------------------------
//!	接続確認
//-----------------------------------------------------------------------------
bool Controller::IsConnected()
{
	XINPUT_STATE state;	// コントローラ情報取得用
	// 初期化
	ZeroMemory( &state, sizeof(XINPUT_STATE) );

	// 状態取得
	DWORD Result = XInputGetState(_controllerNum, &state);

    if(Result == ERROR_SUCCESS){
        return true;	// 接続されています。
    }else{
        return false;	// 接続されていません。
    }
}

//-----------------------------------------------------------------------------
//!	入力チェック
//!	@param	[in]	p_state	パッドの種類
//-----------------------------------------------------------------------------
bool Controller::GetPadState(PAD_STATE p_state)
{
	XINPUT_STATE state;	// コントローラ情報取得用
    ZeroMemory( &state, sizeof(XINPUT_STATE) );	// 初期化
    DWORD dwResult;	// 関数結果判定用
	// 情報取得
    dwResult = XInputGetState(_controllerNum,&state);
    if(dwResult == ERROR_SUCCESS){	//	情報がとれたら
		CheckMode(p_state);	//	アナログモードかデジタルかチェック
		//	モードによって入力チェック切替
		switch(mode){
		case ANALOG:	//	アナログ
			if(GetAnalogState(state,p_state)){return true;}
			break;
		case DIGITAL:	//	デジタル
			if(GetDigitalState(state,p_state)){return true;}
			break;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
//!	今押されたかチェック
//!	@param	[in]	p_state	パッドの種類
//-----------------------------------------------------------------------------
bool Controller::GetPushState(PAD_STATE p_state)
{
	// つながっていなかったら処理しない
	if( !IsConnected() ) return false;

	// 前のフレームのキーを入れる
	prevPadState[p_state] = nowPadState[p_state];
	// 現在の状態のを取得
	nowPadState[p_state] = GetPadState(p_state);
	//	前回のそのボタンの状態が押されてなければ
	if((prevPadState[p_state] ^ nowPadState[p_state]) & nowPadState[p_state] ) 
	{
		return true;
	}
	
	return false;
}




//-----------------------------------------------------------------------------
//! デジタルの入力チェック
//!	@param	[in]	state	XINPUTのパッドの種類
//!	@param	[in]	p_state	パッドの種類
//-----------------------------------------------------------------------------
bool Controller::GetDigitalState(XINPUT_STATE state, PAD_STATE p_state)
{
	To_XInput(p_state);	//	XINPUTに変換
	//	入力チェック
	if(state.Gamepad.wButtons & p_state){return true;}	
	return false;
}

//-----------------------------------------------------------------------------
//!	アナログの入力チェック
//!	@param	[in]	state	XINPUTのパッドの種類
//!	@param	[in]	p_state	パッドの種類
//-----------------------------------------------------------------------------
bool Controller::GetAnalogState(XINPUT_STATE state, PAD_STATE p_state)
{
	// トリガーの入力無視範囲設定
	int TRIGGER_DEADZONE = XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
	//	スティックの入力無視判定設定
	int L_STICK_THUMB_DEAD = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	int R_STICK_THUMB_DEAD = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	//	入力の状態によって入力チェック切替
	switch(p_state){
	//	トリガー
	case PAD_LT:
		if(state.Gamepad.bLeftTrigger  > TRIGGER_DEADZONE){return true;}
		break;
	case PAD_RT:
		if(state.Gamepad.bRightTrigger > TRIGGER_DEADZONE){return true;}	
		break;
	//	左スティック
	case PAD_LEFT_U_STICK:
		if(state.Gamepad.sThumbLY >  L_STICK_THUMB_DEAD ){return true;}
		break;
	case PAD_LEFT_D_STICK:
		if(state.Gamepad.sThumbLY < -L_STICK_THUMB_DEAD ){return true;}
		break;
	case PAD_LEFT_R_STICK:
		if(state.Gamepad.sThumbLX >  L_STICK_THUMB_DEAD ){return true;}
		break;
	case PAD_LEFT_L_STICK:
		if(state.Gamepad.sThumbLX < -L_STICK_THUMB_DEAD ){return true;}
		break;
	//	右スティック
	case PAD_RIGHT_U_STICK:
		if(state.Gamepad.sThumbRY >  R_STICK_THUMB_DEAD ){return true;}
		break;
	case PAD_RIGHT_D_STICK:
		if(state.Gamepad.sThumbRY < -R_STICK_THUMB_DEAD ){return true;}
		break;
	case PAD_RIGHT_R_STICK:
		if(state.Gamepad.sThumbRX >  R_STICK_THUMB_DEAD ){return true;}
		break;
	case PAD_RIGHT_L_STICK:
		if(state.Gamepad.sThumbRX < -R_STICK_THUMB_DEAD ){return true;}
		break;
	}
	return false;
}

//-----------------------------------------------------------------------------
//!	アナログ値を取得
//! @param	[in]	Thumb	true:左スティック false:右スティック
//!	@param	[in]	KnockSize	押し加減 0.0f～1.0f
//-----------------------------------------------------------------------------
Vector3	Controller::getStickState(bool Thumb, f32& KnockSize)
{
	// つながっていなかったら処理しない
	if( !IsConnected() ) return Vector3( 0.0f, 0.0f, 0.0f );

	//	スティックの入力無視判定設定
	int L_STICK_THUMB_DEAD = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	int R_STICK_THUMB_DEAD = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	/*int L_STICK_THUMB_DEAD = 32768;
	int R_STICK_THUMB_DEAD = 32768;*/

	Vector3 dir(0.0f, 0.0f, 0.0f);
	XINPUT_STATE	state;
	ZeroMemory( &state, sizeof(XINPUT_STATE) );	// 初期化
	DWORD dwResult;	// 関数結果判定用
	dwResult = XInputGetState(_controllerNum,&state);

    if(dwResult == ERROR_SUCCESS){	//	情報がとれたら
		// trueなら左
		if( Thumb ){

			Vector3 StickL(0.0f,0.0f,0.0f);
			StickL._x = state.Gamepad.sThumbLX;
			StickL._z = state.Gamepad.sThumbLY;
			s32 LX = (s32)( abs(StickL._x) );
			s32 LY = (s32)( abs(StickL._z) );

			if( LX >= L_STICK_THUMB_DEAD || LY >= L_STICK_THUMB_DEAD){
				dir = StickL;
			}
		}
		else
		// falseなら右
		{
			Vector3 StickR(0.0f,0.0f,0.0f);
			StickR._x = state.Gamepad.sThumbRX;
			StickR._z = state.Gamepad.sThumbRY;
			s32 RX = (s32)( abs(StickR._x) );
			s32 RY = (s32)( abs(StickR._z) );

			if( RX >= R_STICK_THUMB_DEAD || RY >= R_STICK_THUMB_DEAD){
				dir = StickR;
			}
			
		}
	}
	// 押し加減の計算
	if( dir != 0.0f ) {
		KnockSize = LinearInterpolation(0.0f, 1.0f, dir.length(), 32767.0f);
		KnockSize = pow(KnockSize, 2.0f);
	}else{
		KnockSize = 0.0f;
	}

	return dir;
}



//-----------------------------------------------------------------------------
//!	XINPUTに変換
//!	@param	[in]	p_state	パッドの種類
//-----------------------------------------------------------------------------
void Controller::To_XInput(PAD_STATE &p_state)
{
	if( p_state >= 10 ){ 
		p_state = (PAD_STATE)(p_state + 2);
	}
	p_state = (PAD_STATE)(0x001 << p_state);
}

//-----------------------------------------------------------------------------
//!	入力モードチェック
//!	@param	[in]	p_state	パッドの種類
//-----------------------------------------------------------------------------
void Controller::CheckMode(PAD_STATE p_state)
{

	int stateNum	   = p_state;
	int LastDigitalNum = PAD_Y;

	if( stateNum > LastDigitalNum ){
		mode = ANALOG;
	}else{
		mode = DIGITAL;
	}
}

//-----------------------------------------------------------------------------
//!	振動開始
//!	@param [in] L_vib	左振動値(0.0f ~ 1.0f)
//!	@param [in] R_vib	右振動値(0.0f ~ 1.0f)
//-----------------------------------------------------------------------------
void Controller::EnableVibration( f32 L_vib, f32 R_vib )
{
	//	振動値を0.0~1.0fから 0 ~ 65535に変換
	L_vib = 65535 * L_vib;
	R_vib = 65535 * R_vib;
	//	振動用の構造体
	XINPUT_VIBRATION	vibration;
	//	初期化
	ZeroMemory( &vibration, sizeof(XINPUT_VIBRATION) );
	//	振動値代入
	vibration.wLeftMotorSpeed  = (WORD)( L_vib );
	vibration.wRightMotorSpeed = (WORD)( R_vib );
	//	結果転送
	XInputSetState( _controllerNum, &vibration);
}
//-----------------------------------------------------------------------------
//!	振動停止
//-----------------------------------------------------------------------------
void Controller::DisableVibration()
{
	//	振動用の構造体
	XINPUT_VIBRATION	vibration;
	//	初期化
	ZeroMemory( &vibration, sizeof(XINPUT_VIBRATION) );
	//	振動値代入
	vibration.wLeftMotorSpeed  = 0;
	vibration.wRightMotorSpeed = 0;
	//	結果転送
	XInputSetState( _controllerNum, &vibration);
}




//=============================================================================
//	END OF FILE
//=============================================================================