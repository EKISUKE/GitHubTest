//-----------------------------------------------------------------------------
//!
//!	@file	AllyHealer.cpp
//!	@brief	味方回復タイプ
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

DEF_COL_NAME(AllyHealer);
DEF_COL_ATTACK_NAME(AllyHealerAttack);
//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
AllyHealer::AllyHealer()
: AllyBase		(Man::KEY_HEALLER)
, _SeverityMax	( 10 )
, _isHeal		(false)
{
	// 当たり判定初期化
	_myCollision.setCollisionState(&_hitSphere, myName);
	_attackCollision.setCollisionState(&_attackSphere, myAttackName);

	// モデル読み込み
	loadModel("../Model/Ally/Healer/Ally_high.x", CHARA_SCALE);


	// 初期化
//	Initialize();

	// アニメーション初期化
	initAnimation();
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
AllyHealer::~AllyHealer()
{
}

//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool AllyHealer::Initialize()
{
	// キーの作成
	createKey(this, _pSquad->getLeader());

	// 初期化
	AllyBase::Initialize();

	// ステータス初期化
	_myStatus->setParam(StatusAssets::STATUS_HEAL);

	// 攻撃時の半径設定
	_attackRadNum = _radius * 0.5f;
	_attackAABB._min = -Vector3(_radius * 0.5f);
	_attackAABB._max =  Vector3(_radius * 0.5f);
	_attackOffsetRange = _attackRadNum * 1.5f;
	return true;
}

//-----------------------------------------------------------------------------
//! ヒール
//!	@param	[in]	target	目標
//-----------------------------------------------------------------------------
void AllyHealer::Heal(Man*	target)
{
	
	

	// ターゲットのステータスをとってくる
	Status::Param* targetParam = target->getStatus()->getParam();
	// ターゲットの体力を回復する
	f32 HP = targetParam->_HP;
	StatusHeal::ParamHeal* myParam = (StatusHeal::ParamHeal*)_myStatus->getParam();
	//HP += _myStatus->getParam()->_healPow;
	HP += myParam->_healPow;
	// HPを設定
	targetParam->_HP = HP;
	// 範囲外指定
	if( targetParam->_HP > targetParam->_HPMAX )
	{
		targetParam->_HP = targetParam->_HPMAX;
	}

	// ターゲットの座標取得
	Vector3 targetPos = target->getHitPosition();

	// パーティクル生成
	IParticleMan()->generateParticle(targetPos,
									 Vector3(0.0f),
									 Vector3(0.5f, 0.5f, 0.0f),
									 Radian(0.0f),
									 60, 0,
									 Radian(0.0f),
									 ParticleManager::PARTICLE_HEAL); 
	// ヒール音停止
	ISEManager()->stopMusic(SESoundPlayerManager::SE_HEAL);
	// ヒール音再生
	ISEManager()->playMusic(SESoundPlayerManager::SE_HEAL);

	_isHeal = true;
}

//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void AllyHealer::Update()
{
	AllyBase::Update();
}

//-----------------------------------------------------------------------------
//! 更新(コマンドによって変わる)(今はかわらない)
//-----------------------------------------------------------------------------
void AllyHealer::UpdateByCommand()
{

	//---- プレイヤー情報取得
	Player*	pPlayer   = IObjDataManager()->_pPlayer;
	Vector3 playerPos = pPlayer->getPosition();

	// 回復中なら更新しない
	if( !_isHeal ) {
		// ターゲットの検索、設定
		if( searchAndSetTarget() )
		{
			// ターゲットが見つからなければ
			_keyUpdate = false;
		}
	}


	s32	currentAnimation = _pTaskModel->getAnimation();
		
	// プレイヤーの周りにいるならば
	if( _keyUpdate ){
		
		// キーの更新
		_key->Update();

		// 角度を求める
		// 3D上ではxzの順番で角度が求められる
		Vector3	StickState = _key->getVector();
	
		// ヒールコマンドがONならヒールする
		if( _key->getPushState("Heal") )
		{
			// 攻撃アニメーション再生(1はヒール)
			_pTaskModel->setAnimation(_aniHeal);
		}else if ( currentAnimation == _aniHeal ) {
			// ヒール再生中なら
			// 再生比率取得
			f32	aniRate = _pTaskModel->getAnimationRate();
			// 再生が半分いったら
			if( aniRate >= 0.1f && !_isHeal) {
				// ヒール
				Heal(_target);
			}
		}else{
			_isHeal = false;
		}


		// 移動があれば更新
		if( abs(atan2f(StickState._x, StickState._z) ) > 0.0f )
		{
			_rotation._y = atan2f( StickState._x, StickState._z );
			Status::Param* myParam = _myStatus->getParam();
			_speed = myParam->_speedMAX;
		}else{
			_speed = 0.0f;
		}

	} else {
		// それ以外は通常通り
		UpdateNormal(playerPos);
	}

	// 当たり判定処理とダメージ計算
	s32 damage = calcDamageAndCollision();


	if( damage > 0 ) {
		// ダメージ処理
		Damage(damage);
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
void AllyHealer::UpdateNormal(Vector3& playerPos)
{
	// プレイヤーへのベクトル
	Vector3	dir	   = playerPos - _position;
	// 長さ取得
	f32		length = dir.length();

	Circle	goalCircle(playerPos, 100.0f);
	Circle	myCircle(_position, _radius);

	// プレイヤーの近くにいるかどうか
	if( goalCircle.isHit(myCircle) ){
		// 近くにいる
		_speed = 0.0f;
		_keyUpdate = true;
	}else{
		// 近くにいない
		_speed = 3.0f;
		dir = dir.normalize();
		_rotation._y = atan2f(dir._x, dir._z);
		_keyUpdate = false;
	}



}

//-----------------------------------------------------------------------------
//! 描画
//-----------------------------------------------------------------------------
void AllyHealer::Render(bool isShadow)
{
	AllyBase::Render(isShadow);
}

//-----------------------------------------------------------------------------
//! 狙う相手を探して設定
//!	@return true:敵を設定完了 false:狙う敵はいない
//-----------------------------------------------------------------------------
bool AllyHealer::searchAndSetTarget()
{

	Player*	pPlayer   = IObjDataManager()->_pPlayer;

	// 前の情報があったらかつ、死亡していなければ
	if( _pPrevTarget != nullptr && !_pPrevTarget->isDead() ) {
		f32 lengthToTarget = getLengthtoTarget(_pPrevTarget);
		// 捜索範囲内なら
		static const f32 searchLength = 200.0f;
		if( lengthToTarget < searchLength ) {
			// 検索しない
			return true;
		}
	}

	// 最初はプレイヤー
	_target = pPlayer;
	// 味方データから一番体力の少ない味方を割り出す
	vector<AllyBase*>::iterator itr;
	// プレイヤーの体力
	Status::Param*	playerParam = IObjDataManager()->_pPlayer->getStatus()->getParam();

	f32 PlayerHP	   = playerParam->_HP;
	f32	PlayerHPMAX	   = playerParam->_HPMAX;
	s32 playerSeverity = (s32)( LinearInterpolation( (f32)(_SeverityMax) , 0, PlayerHP, PlayerHPMAX) ); 
	// 緊急度
	s32 Severity = playerSeverity;

	// すべてのHPを検査、緊急度をチェック
	for( itr = IObjDataManager()->_pAllyData.begin(); itr != IObjDataManager()->_pAllyData.end(); itr++)
	{
		AllyBase* ally = *itr;
		// 死亡していたら処理しない
		if( ally->isDead() ) continue;
		Status::Param* allyParam = ally->getStatus()->getParam();
		f32 HP		= allyParam->_HP;
		f32 HPMAX	= allyParam->_HPMAX;
		// 今回の緊急度
		s32 nowSeverity	  = (s32)LinearInterpolation((f32)_SeverityMax, 0, HP, HPMAX);
		// 今回の緊急度が高かったら
		if( Severity < nowSeverity )
		{
			// 今回で更新
			Severity = nowSeverity;
			_target	 = ally;
		}
	}

	// 一番近い敵をターゲットに設定
	if( _target != nullptr ) {
		// 次回用ターゲットに設定しておく
		_pPrevTarget = _target;
		// 一番近いやつをターゲットにする
		_key->setTarget(_target);
	}else{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//! ダメージ計算と当たり判定処理
//-----------------------------------------------------------------------------
s32 AllyHealer::calcDamageAndCollision()
{
	// ダメージ保存用
	s32 damage = 0;
	//---- 当たり判定
	for( s32 i=0; i<_myCollision.getHitCount(); ++i )
	{
		Collision* obj = _myCollision.getHitObj(i);
		if( isAllyHealer(obj) ){

			// 補正位置を取得する
			Vector3	dir = _myCollision.getOffsetPos(obj);
			// 補正位置に設定
			_hitPosition = dir;
			// あたり判定用の球情報更新
			setSphereParam(_hitPosition, _hitRadius);

		}else if( isEnemyLeaderAttack(obj) || isEnemyAttakerAttack(obj) || isEnemyDeffenderAttack(obj) ){
			// ダメージ判定
			_hitAttack = true;
			damage += (s32)IObjDataManager()->_enemyStatus->getParam()->_atkPow;
		}
		// 座標の設定
		toPosition(_hitPosition, _position);
	}

	return damage;
}


//=============================================================================
//	END OF FILE
//=============================================================================