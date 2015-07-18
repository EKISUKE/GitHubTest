//-----------------------------------------------------------------------------
//!
//!	@file	KeyDeffender.cpp
//!	@brief	防御(護衛)タイプの操作
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include"Library.h"

//=============================================================================
// AI(攻撃タイプ)
//=============================================================================
//-----------------------------------------------------------------------------
//! デフォルトコンストラクタ
//-----------------------------------------------------------------------------
KeyDeffender::KeyDeffender(Man* me, Man* leader)
: AI			(me, leader)
, _mode			(MODE_APPROACH)
, _searchRot	(0.0f)
, _attackReach	(45.0f)
, _deffenceReach(60.0f)
{
	_type = AI::TYPE_DEFFENDER;
}
//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
KeyDeffender::~KeyDeffender()
{
}

//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void KeyDeffender::Update()
{
	// 使う座標を取得
	Vector3 playerPos = _leader->getPosition();
	Vector3 targetPos = _target->getPosition();
	Vector3 myPos	  = _me->getPosition();
	// 防衛対象へ向かうベクトル
	_dirToTarget    = playerPos - myPos;
	_dirToTarget._y = 0.0f;
	// 防衛対象までの距離を取得
	f32 length = _dirToTarget.squareLength();
	

	//----------------------------------------------------------
	// AI制御
	//----------------------------------------------------------
	switch(_mode)
	{
		//----------------------------------------------------------
		case MODE_APPROACH:	// 防衛対象に接近
		//----------------------------------------------------------
		{

			// 距離判定
			// 防御範囲なら
			if( length <= _deffenceReach * _deffenceReach){
				// 防御状態に移行
				_mode = MODE_DEFFENCE;
			}else{
				// 単位ベクトルにする
				_dirToTarget = _dirToTarget.normalize();
				// 移動方向ベクトル設定
				_movVector = _dirToTarget;
			}

		}
			break;
		//----------------------------------------------------------
		case MODE_DEFFENCE:	// 防衛対象を防御
		//----------------------------------------------------------
		{
			// 防衛目標から敵へのベクトル
			Vector3	dirPlayerToTarget = targetPos - playerPos;
			// そのベクトルを防御範囲の大きさに
			dirPlayerToTarget  = dirPlayerToTarget.normalize();
			dirPlayerToTarget *= _deffenceReach;
			dirPlayerToTarget += playerPos;	// プレイヤーの座標を足す
			// その座標までの方向ベクトルを取得
			_movVector = dirPlayerToTarget - myPos;
			// 自分の座標からゴールへの座標
			Vector3	dirMyPosToGoal = _movVector;
			// Y方向への距離は考慮しない
			dirMyPosToGoal._y = 0.0f;
			// その長さが10.0f以下なら
			if( dirMyPosToGoal.length() <= 30.0f ){
				// 移動終了
				_movVector = Vector3(0.0f, 0.0f, 0.0f);
			}else{	// 以上なら
				// 移動する
				_movVector = _movVector.normalize();
			}

			// 敵へのベクトル
			Vector3 dirMyPosToTarget = targetPos - myPos;

			// 敵が自分の攻撃範囲に入っていたら
			if( dirMyPosToTarget.squareLength() <= _attackReach * _attackReach )
			{
				// 攻撃状態に移行
				_mode = MODE_ATTACK;
			}else if( length > _deffenceReach){
				// 防御範囲外ならアプローチに戻る
				_mode = MODE_APPROACH;
			}
		}
			break;
		//----------------------------------------------------------
		case MODE_ATTACK:	// 防衛対象に近づく敵を攻撃
		//----------------------------------------------------------
		{
			// 移動しない
			_movVector = Vector3(0.0f, 0.0f, 0.0f);

			// 攻撃フラグが立ってたら
			_nowTime = timeGetTime();
			Status* myStatus = _me->getStatus();
			f32 delay = myStatus->getParam()->_atkDelay;
			// 待ち時間がなければ
			if( _nowTime - _oldTime > 100 *  delay) {
				// 攻撃ボタンフラグを立てる
				//setCommandFlag(KeyBase::KEY_COMMAND_ATTACK, true);
				setCommandFlag("Attack", true);
				// 時間の更新
				_oldTime = _nowTime;
			}else{
				// 攻撃ボタンフラグを倒す
				//setCommandFlag(KeyBase::KEY_COMMAND_ATTACK, false);
				setCommandFlag("Attack", false);
			}

			// 敵へのベクトル
			Vector3 dirMyPosToTarget = targetPos - myPos;
			// 攻撃範囲外だったら
			if( dirMyPosToTarget.squareLength() > _attackReach * _attackReach ) {	
				// 防衛対象の近くに戻る
				_mode = MODE_APPROACH;
				// 攻撃フラグを落とす
				setCommandFlag("Attack", false);
			}
		}
			break;
		
	}
}
//=============================================================================
//	END OF FILE
//=============================================================================