//-----------------------------------------------------------------------------
//!
//!	@file	Ally.cpp
//!	@brief	味方基底
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

DEF_COL_NAME(AllyDeffender);
DEF_COL_ATTACK_NAME(AllyDeffenderAttack);
//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
AllyDeffender::AllyDeffender()
: AllyBase(Man::KEY_DEFFENDER)
{
	// 当たり判定初期化
	_myCollision.setCollisionState(&_hitSphere, myName);
	_attackCollision.setCollisionState(&_attackSphere, myAttackName);

	// モデル読み込み
	loadModel("../Model/Ally/Deffender/Ally_high.x", CHARA_SCALE);
	
	// アニメーション初期化
	initAnimation();
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
AllyDeffender::~AllyDeffender()
{
}

//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool AllyDeffender::Initialize()
{

	// キーの作成
	createKey(this, _pSquad->getLeader());

	// 初期化
	AllyBase::Initialize();

	// ステータス初期化
	_myStatus->setParam(StatusAssets::STATUS_DEF);
	
	// 攻撃時の半径設定
	_attackRadNum = _radius * 0.5f;
	_attackAABB._min = -Vector3(_radius * 0.5f);
	_attackAABB._max =  Vector3(_radius * 0.5f);
	_attackOffsetRange = _attackRadNum * 1.5f;

	return true;
}

//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void AllyDeffender::Update()
{
	AllyBase::Update();
}

//-----------------------------------------------------------------------------
//! 更新(コマンドによって変わる)(今は変わらない)
//-----------------------------------------------------------------------------
void AllyDeffender::UpdateByCommand()
{
	// あとで使う(Updateの引数をなくすときに使う)
	Vector3 playerPos = IObjDataManager()->_pPlayer->getPosition();


	// ゴールへ向かうフラグがOFFなら
	if( !_goGoal ){

		// ターゲットの検索、設定
		if( AllyBase::searchAndSetTarget() ) 
		{
			// キーの更新
			_key->Update();

			// 角度を求める
			// 3D上ではxzの順番で角度が求められる
			Vector3	StickState = _key->getVector();
	
			// 移動があれば更新
			if( abs(atan2f(StickState._x, StickState._z) ) > 0.0f )
			{
				_rotation._y = atan2f( StickState._x, StickState._z );
				Status::Param* myParam = (Status::Param*) _myStatus->getParam();
				_speed = myParam->_speedMAX;
			}else{
				_speed = 0.0f;
			}
		}else{
			// ターゲットがいなければプレイヤーに向かう
			_goGoal = true;
		}

	} else {
		// それ以外は通常通り
		UpdateNormal(playerPos);
	}

	// 当たり判定処理とダメージ計算
	s32 damage = calcDamageAndCollision();


	s32	currentAnimation = _pTaskModel->getAnimation();

	if( damage > 0 ) {
		// ダメージ処理
		Damage(damage);
		// ダメージアニメーションへ
		_pTaskModel->setAnimation(_aniDamage);
	}else if( currentAnimation == _aniDamage ){
		_isDamage = false;
	}

		
	_pTaskModel->update();

}

//-----------------------------------------------------------------------------
//! 更新(プレイヤーの周りに常にいる)
//!	@param	[in]	player	プレイヤー座標
//-----------------------------------------------------------------------------
void AllyDeffender::UpdateNormal(Vector3& playerPos)
{
	// プレイヤーへのベクトル
	Vector3	dir	   = playerPos - _position;
	// 長さ取得
	f32		length = dir.length();


	Circle	goalCircle(playerPos, 100.0f);
	Circle	myCircle(_position, _radius);

	if( goalCircle.isHit(myCircle) ){
		_speed = 0.0f;
		_goGoal = false;
	}else{
		Status::Param*	myParam = _myStatus->getParam();
		_speed = myParam->_speedMAX;
		dir = dir.normalize();
		_rotation._y = atan2f(dir._x, dir._z);
		_goGoal = true;
	}
}

//-----------------------------------------------------------------------------
//! 描画
//-----------------------------------------------------------------------------
void AllyDeffender::Render(bool isShadow)
{
	AllyBase::Render(isShadow);
}

//-----------------------------------------------------------------------------
//! ダメージ計算と当たり判定処理
//-----------------------------------------------------------------------------
s32	AllyDeffender::calcDamageAndCollision()
{
	// ダメージ保存用
	s32 damage = 0;
	//---- 当たり判定
	for( s32 i=0; i<_myCollision.getHitCount(); ++i )
	{
		Collision* obj = _myCollision.getHitObj(i);
		if( isAllyDeffender(obj) ){

			// 補正位置を取得する
			Vector3	dir = _myCollision.getOffsetPos(obj);
			// 補正位置に設定
			_hitPosition = dir;
			// あたり判定用の球情報更新
			setSphereParam(_hitPosition, _hitRadius);

		}else if( isEnemyLeaderAttack(obj) || isEnemyAttakerAttack(obj) || isEnemyDeffenderAttack(obj) ){
			// ダメージ判定
			_hitAttack = true;
			damage += (u32)IObjDataManager()->_enemyStatus->getParam()->_atkPow;
		}
		// 座標の設定
		toPosition(_hitPosition, _position);
	}
	return damage;
}

//=============================================================================
//	END OF FILE
//=============================================================================