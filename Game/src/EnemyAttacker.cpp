//-----------------------------------------------------------------------------
//!
//!	@file	EnemyAttacker.cpp
//!	@brief	敵攻撃タイプ
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

DEF_COL_NAME(EnemyAttacker);
DEF_COL_ATTACK_NAME(EnemyAttakerAttack);
//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
EnemyAttacker::EnemyAttacker()
: EnemyBase(Man::KEY_ATTACKER)
{
	// 操作方法設定(今は仮で自分をリーダとしてる)
	//_key = new KeyAttacker(this, this);
	// 当たり判定初期化
	_myCollision.setCollisionState(&_hitSphere, myName);
	_attackCollision.setCollisionState(&_attackSphere, myAttackName);


	// モデル読み込み
	loadModel("../Model/Enemy/Attacker/Enemy.x", CHARA_SCALE);

	// 初期化
//	Initialize();

	// アニメーション初期化
	initAnimation();
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
EnemyAttacker::~EnemyAttacker()
{
}
//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool EnemyAttacker::Initialize()
{

	// キーの作成
	createKey(this, _pSquad->getLeader());

	// 初期化
	EnemyBase::Initialize();
	
	// ステータス初期化
	_myStatus->setParam(StatusAssets::STATUS_ENEMY);

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
void EnemyAttacker::Update()
{
	EnemyBase::Update();
}

//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void EnemyAttacker::UpdateByCommand()
{
	if( (_goalPos - _position).length() <= 500.0f && !_goGoal){
		// ターゲットの検索、設定
		EnemyBase::searchAndSetTarget();

		
		// キーの更新
		_key->Update();

		//---- キャラの移動
		Vector3	StickState = _key->getVector();
		
		// スティック操作がされていたら
		if( abs(atan2f(StickState._x, StickState._z) ) > 0.0f )
		{
			_rotation._y = atan2f( StickState._x, StickState._z );
			_speed = _myStatus->getParam()->_speedMAX;
		}else{
			_speed = 0.0f;
		}
	}else{
		// 通常通りの更新
		UpdateNormal(_goalPos);
	}

	// 当たり判定処理とダメージ計算
	s32 damage = EnemyBase::calcDamageAndCollision();
	
	s32 currentAnimation = _pTaskModel->getAnimation();

	if( damage > 0 ) {
		Damage(damage);
		// ダメージアニメーションへ
		_pTaskModel->setAnimation(_aniDamage);
	}else if( currentAnimation == _aniDamage ){
		_isDamage = false;
	}
	
		
	_pTaskModel->update();
		
}

//-----------------------------------------------------------------------------
//! リーダーの周りに常にいる
	//!	@param	[in]	leader	リーダー座標
//-----------------------------------------------------------------------------
void EnemyAttacker::UpdateNormal(Vector3& leaderPos)
{
	Vector3 dir = leaderPos - _position;
	// 長さ取得
	f32		length = dir.length();
	Circle	goalCircle(leaderPos, 100.0f);
	Circle	myCircle(_position, _radius);

	if( goalCircle.isHit(myCircle) ){
		_goGoal = false;
		_speed = 0.0f;
	}else{
		Status::Param* myParam = _myStatus->getParam();
		_speed = myParam->_speedMAX;
		dir = dir.normalize();
		_rotation._y = atan2f(dir._x, dir._z);
		_goGoal = true;
	}
}

//-----------------------------------------------------------------------------
//! 描画
//-----------------------------------------------------------------------------
void EnemyAttacker::Render(bool isShadow)
{
	EnemyBase::Render(isShadow);
}


//=============================================================================
//	END OF FILE
//=============================================================================