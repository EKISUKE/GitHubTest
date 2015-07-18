//-----------------------------------------------------------------------------
//!
//!	@file	EnemyLeader.cpp
//!	@brief	敵攻撃タイプ
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

DEF_COL_NAME(EnemyLeader);
DEF_COL_ATTACK_NAME(EnemyLeaderAttack);
//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
EnemyLeader::EnemyLeader()
: EnemyBase(Man::KEY_LEADER)
{
	// 操作方法設定(今は仮で自分をリーダとしてる)
	//_key = new KeyAttacker(this, this);
	// 当たり判定初期化
	_myCollision.setCollisionState(&_hitSphere, myName);
	_attackCollision.setCollisionState(&_attackSphere, myAttackName);


	// モデルサイズ変更
	_modelScale = 1.5f;

	// モデル読み込み(リーダーは少し大きめにする)
	loadModel("../Model/Enemy/Attacker/Enemy.x", CHARA_SCALE * _modelScale);

	// キーの作成
	createKey(this, this);

	// 防衛座標設定
//	_deffencePos = IObjDataManager()->_deffencePos[0];

	// 初期化
//	Initialize();

	// アニメーション初期化
	initAnimation();
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
EnemyLeader::~EnemyLeader()
{
}
//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool EnemyLeader::Initialize()
{
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
void EnemyLeader::Update()
{
	EnemyBase::Update();
}

//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void EnemyLeader::UpdateByCommand()
{
	
	// 一定距離はなれていなければ
	if( (_goalPos - _position).length() <= 3000.0f && !_goGoal){
		
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
		UpdateNormal(_goalPos);
	}

	// 当たり判定とダメージ計算
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
void EnemyLeader::UpdateNormal(Vector3& leaderPos)
{
	Vector3 dir = leaderPos - _position;
	// 長さ取得
	f32		length = dir.length();
	Circle	goalCircle(leaderPos, 500.0f);
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
void EnemyLeader::Render(bool isShadow)
{
	EnemyBase::Render(isShadow);
}


//=============================================================================
//	END OF FILE
//=============================================================================