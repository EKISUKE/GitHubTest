//-----------------------------------------------------------------------------
//!
//!	@file	KeyAttacker.cpp
//!	@brief	攻撃タイプの操作
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
KeyAttacker::KeyAttacker(Man* me, Man* leader)
: AI			(me, leader)
, _mode			(MODE_SEARCH)
, _searchReach	(200.0f)
, _searchRot	(0.0f)
, _attackReach	(45.0f)
{
	_type = AI::TYPE_ATTACKER;
}
//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
KeyAttacker::~KeyAttacker()
{
}

//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void KeyAttacker::Update()
{
	// 使う座標を取得
	Vector3 targetPos = _target->getPosition();
	Vector3 myPos	  = _me->getPosition();
	// 攻撃対象へ向かうベクトル
	_dirToTarget    = targetPos - myPos;
	_dirToTarget._y = 0.0f;
	// 攻撃対象までの距離を取得
	f32 length = _dirToTarget.squareLength();
	

	//----------------------------------------------------------
	// AI制御
	//----------------------------------------------------------
	switch(_mode)
	{
		//----------------------------------------------------------
		case MODE_SEARCH:	// 目標捜索
		//----------------------------------------------------------
		{
			// 一定方向に進み続ける時間
			static s32 moveCount = 100;

			// 移動しない
			_movVector = Vector3(0.0f, 0.0f, 0.0f);
			// 捜索範囲内に入っていたら
			if( length <= _searchReach * _searchReach ) {
				// 接近状態に移行
				_mode = MODE_APPROACH;
			}else{
				if(moveCount > 0){
					// 角度の方向に動く
					_movVector._x = sinf(_searchRot);
					_movVector._z = cosf(_searchRot);
					moveCount--;
				}else{
					// カウントのリセット
					moveCount = 100;
					// 次の角度を決める
					_searchRot = (f32)( rand() % 360 );
				}
			}

		}
			break;
		//----------------------------------------------------------
		case MODE_APPROACH:	// 目標に接近
		//----------------------------------------------------------
		{
			// 単位ベクトルにする
			_dirToTarget = _dirToTarget.normalize();
			// 移動方向ベクトル設定
			_movVector = _dirToTarget;
			
			// 距離判定
			if( length <= _attackReach * _attackReach ) {			// 攻撃範囲内に入ってたら
				// 攻撃状態に移行
				_mode = MODE_ATTACK;
			}else if( length > _searchReach * _searchReach ) {		// 捜索範囲外だったら
				// 捜索状態に戻る
				_mode = MODE_SEARCH;
			}

		}
			break;
		//----------------------------------------------------------
		case MODE_ATTACK:	// 目標を攻撃
		//----------------------------------------------------------
		{
			_movVector = Vector3(0,0,0);

			// 攻撃フラグが立ってたら
			_nowTime = timeGetTime();
			Status* myStatus = _me->getStatus();
			f32 delay = myStatus->getParam()->_atkDelay;
			// 待ち時間がなければ
			if( _nowTime - _oldTime > 100 *  delay) {
				// 攻撃ボタンフラグを立てる
				setCommandFlag("Attack", true);
				// 時間の更新
				_oldTime = _nowTime;
			}else{
				// 攻撃ボタンフラグを倒す
				setCommandFlag("Attack", false);
			}

			// 攻撃範囲外だったら
			if( length > _attackReach * _attackReach ) {		
				// 接近状態に戻る
				_mode = MODE_APPROACH;
				// 攻撃ボタンフラグを倒す
				setCommandFlag("Attack", false);
			}
		}
			break;
		
	}
}
//=============================================================================
//	END OF FILE
//=============================================================================