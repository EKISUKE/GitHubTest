//-----------------------------------------------------------------------------
//!
//!	@file	EnemyDeffender.cpp
//!	@brief	敵防御タイプ
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

DEF_COL_NAME(EnemyDeffender);
DEF_COL_ATTACK_NAME(EnemyDeffenderAttack);
//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
EnemyDeffender::EnemyDeffender()
: EnemyBase(Man::KEY_DEFFENDER)
{
	// 当たり判定初期化
	_myCollision.setCollisionState(&_hitSphere, myName);
	_attackCollision.setCollisionState(&_attackSphere, myAttackName);

	// モデルサイズ変更
	_modelScale = 1.2f;
	// モデル読み込み
	loadModel("../Model/Enemy/Attacker/Enemy.x", CHARA_SCALE * _modelScale);

	// アニメーション初期化
	initAnimation();
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
EnemyDeffender::~EnemyDeffender()
{
}
//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool EnemyDeffender::Initialize()
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
void EnemyDeffender::Update()
{
	EnemyBase::Update();
}

//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void EnemyDeffender::UpdateByCommand()
{
	if( !_goGoal ){
		// ターゲットの検索、設定
		EnemyBase::searchAndSetTarget();
		
		// キーの更新
		_key->Update();

		// 角度を求める
		// 3D上ではxzの順番で角度が求められる
		Vector3	StickState = _key->getVector();
	
	
		if( abs(atan2f(StickState._x, StickState._z) ) > 0.0f )
		{
			_rotation._y = atan2f( StickState._x, StickState._z );
			_speed = _myStatus->getParam()->_speedMAX;
		}else{
			_speed = 0.0f;
		}
	}else{
		UpdateNormal(_goalPos);
	}

	// ダメージ保存用
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
void EnemyDeffender::UpdateNormal(Vector3& leaderPos)
{
	Vector3 dir = leaderPos - _position;
	// 長さ取得
	f32		length = dir.length();
	Circle	goalCircle(leaderPos, 100.0f);
	Circle	myCircle(_position, _radius);

	if( goalCircle.isHit(myCircle) ){
		_speed = 0.0f;
		_goGoal = false;
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
void EnemyDeffender::Render(bool isShadow)
{
	EnemyBase::Render(isShadow);
}


//=============================================================================
//	END OF FILE
//=============================================================================