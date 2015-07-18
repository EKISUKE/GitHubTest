//-----------------------------------------------------------------------------
//!
//!	@file	KeyHealer.cpp
//!	@brief	回復タイプの操作
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include"Library.h"

//=============================================================================
// AI(回復タイプ)
//=============================================================================
//-----------------------------------------------------------------------------
//! デフォルトコンストラクタ
//-----------------------------------------------------------------------------
KeyHealer::KeyHealer(Man* me, Man* leader)
: AI			(me, leader)
, _mode			(MODE_APPROACH)
, _searchReach	(200.0f)
, _searchRot	(0.0f)
, _debufReach	(100.0f)
, _healReach	(100.0f)
, _healPower	(2)
, _healOldTime	(0)
, _healNowTime	(0)
{
	_type = AI::TYPE_HEALLER;
	_vecKeyCommand.push_back( KeyComPtr(new KeyCommand("Heal")) );
	_vecKeyCommand.push_back( KeyComPtr(new KeyCommand("Debuf")) );
}
//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
KeyHealer::~KeyHealer()
{
}


////-----------------------------------------------------------------------------
////! 目標を捜索
////-----------------------------------------------------------------------------
//void KeyHealer::Search()
//{
//	// 一定方向に進み続ける時間
//	static s32 moveCount = 100;
//
//	if(moveCount > 0){
//		// 角度の方向に動く
//		_movVector._x = sinf(_searchRot);
//		_movVector._z = cosf(_searchRot);
//		moveCount--;
//	}else{
//		// カウントのリセット
//		moveCount = 100;
//		// 次の角度を決める
//		_searchRot = rand() % 360;
//	}
//
//}
//-----------------------------------------------------------------------------
//! 目標に接近
//-----------------------------------------------------------------------------
void KeyHealer::Approach()
{
	// 単位ベクトルにする
	//_dirToTarget = _dirToTarget.normalize();
	// 移動方向ベクトル設定
	_movVector = _dirToTarget.normalize();
}

//-----------------------------------------------------------------------------
//! デバフ魔法
//-----------------------------------------------------------------------------
void KeyHealer::DebufMagic()
{
	// 移動しない
	_movVector = Vector3(0.0f, 0.0f, 0.0f);
	// ターゲットのステータスをとってくる
	// 一部のステータスを下げる
}

//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void KeyHealer::Update()
{

	// 目標へのベクトル
	_dirToTarget = _target->getPosition() - _me->getPosition();

	// 目標距離までの長さを測る
	f32 length = _dirToTarget.squareLength();

	// ヒール距離と比較
	if( length <= _healReach* _healReach )
	{
		int a = 0;
	}else{
		int a = 0;
	}


	// 味方タイプなら
	if( _me->getType() == Man::TYPE_ALLY )
	{
		// ターゲットのタイプが敵なら
		if( _target->getType() == Man::TYPE_ENEMY )
		{
			// 敵
			_TargetisEnemy = true;
		}else{
			// 違えば味方
			_TargetisEnemy = false;
		}
	}else{
		// 敵タイプなら
		// ターゲットが敵タイプじゃなければ
		if( _target->getType() != Man::TYPE_ENEMY)
		{
			// 敵
			_TargetisEnemy = true;
		}else{
			// 敵タイプなら味方
			_TargetisEnemy = false;
		}
	}

	// 捜索範囲外ならかつ敵なら
	if( length > _searchReach * _searchReach && _TargetisEnemy)
	{
		// ターゲットをリーダーに変更
		setTarget(_leader);
		// 敵フラグをOFF
		_TargetisEnemy = false;
		// ターゲットの距離再設定
		length = _dirToTarget.squareLength();
	}
	
	
	//----------------------------------------------------------
	// AI制御
	//----------------------------------------------------------
	switch(_mode)
	{
		//----------------------------------------------------------
		case MODE_APPROACH:	// 目標に接近
		//----------------------------------------------------------
		{
			// 敵だったら
			if( !_TargetisEnemy )
			{
				// ヒール距離と比較
				if( length <= _healReach* _healReach )
				{
					// 範囲内ならヒールへ
					_mode = MODE_HEAL;
				}else{
					// 接近
					Approach();
				}
			}else{
				// デバフ範囲内に入ってたら
				if( length <= _debufReach * _debufReach ) {			
					// デバフ状態に移行
					_mode = MODE_DEBUF;
				}else{
					// 接近
					Approach();
				}
			}
			
			
		}
			break;
		//----------------------------------------------------------
		case MODE_HEAL:	// 目標にヒール
		//----------------------------------------------------------
		{
			// 移動しない
			_movVector = Vector3(0.0f, 0.0f, 0.0f);
			// 距離比較
			if( length > _healReach * _healReach )
			{
				_mode = MODE_APPROACH;
				setCommandFlag("Heal", false);
			}else{
				_healNowTime = timeGetTime();
				// ステータスをとってくる
				Status* status = _me->getStatus();
				// ディレイ時間を取得
				f32 delay = status->getParam()->_sklDelay;
				f32 targetHP	= _target->getStatus()->getParam()->_HP;
				f32	targetHPMAX = _target->getStatus()->getParam()->_HPMAX;
				// ターゲットのHPが下がってたら且つ０以上なら
				if( targetHP < targetHPMAX && targetHP >= 0.0f) {
					// ディレイ時間がより大きかったら
					if( _healNowTime - _healOldTime > 100 *  delay) {
						// ヒールコマンドフラグON
						setCommandFlag("Heal", true);
						// 時間の更新
						_healOldTime = _healNowTime;
					}else{
						// ヒールコマンドフラグOFF
						setCommandFlag("Heal", false);
					}
				}else{
					// ヒールコマンドフラグOFF
					setCommandFlag("Heal", false);
				}
			}
			
		}
			break;
		//----------------------------------------------------------
		case MODE_DEBUF:	// 目標にデバフ
		//----------------------------------------------------------
		{
			// デバフ範囲外だったら
			if( length > _debufReach * _debufReach) {		
				// 接近状態に戻る
				_mode = MODE_APPROACH;
				// 攻撃ボタンフラグを倒す
				//setCommandFlag(KeyBase::KEY_COMMAND_ATTACK, false);
				setCommandFlag("Attack", false);
			}else{
				_nowTime = timeGetTime();
				// 1000ミリ秒(1秒)経ったら実行する
				// ステータスをとってくる
				Status* status = _me->getStatus();
				// ディレイ時間を取得
				f32 delay = status->getParam()->_sklDelay;
				// ディレイ時間がより大きかったら
				if( _nowTime - _oldTime > 1000 *  delay) {
					setCommandFlag("Attack", true);
					// 時間の更新
					_oldTime = _nowTime;
				}else{
					// 攻撃ボタンフラグを倒す
					//setCommandFlag(KeyBase::KEY_COMMAND_ATTACK, false);
					setCommandFlag("Attack", false);
				}
			}
			
		}
			break;
		
	}
}
//=============================================================================
//	END OF FILE
//=============================================================================