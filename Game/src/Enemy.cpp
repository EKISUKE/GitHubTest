//-----------------------------------------------------------------------------
//!
//!	@file	Enemy.cpp
//!	@brief	味方基底
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"


//-----------------------------------------------------------------------------
//! デフォルトコンストラクタ
//-----------------------------------------------------------------------------
EnemyBase::EnemyBase()
: Man			(Man::KEY_ATTACKER, Man::TYPE_ENEMY)
, _pSquad		(nullptr)
, _pPrevTarget	(nullptr)
, _goGoal		(true)
, _modelScale	(1.0f)
{
}

//-----------------------------------------------------------------------------
//! コンストラクタ
//!	@param	[in]	myType	キータイプ
//-----------------------------------------------------------------------------
EnemyBase::EnemyBase(Man::KEY_TYPE myType)
: Man			(myType, Man::TYPE_ENEMY)
, _pSquad		(nullptr)
, _pPrevTarget	(nullptr)
, _goGoal		(true)
, _modelScale	(1.0f)
{
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
EnemyBase::~EnemyBase()
{
}

//-----------------------------------------------------------------------------
//!	初期化
//-----------------------------------------------------------------------------
bool EnemyBase::Initialize()
{
	if( _keyType != KEY_LEADER) {
		_goalPos = Vector3(0.0f);
	}else{
		_goalPos = IObjDataManager()->_deffencePos[0];
	}
	// 座標設定
	ResetPosition();
	// 初期化
	Man::Initialize();

	return true;
}

//-----------------------------------------------------------------------------
//! アニメーション初期化
//-----------------------------------------------------------------------------
void EnemyBase::initAnimation()
{
	_aniMove	= _pTaskModel->searchAnimation("Run");
	_aniAttack	= _pTaskModel->searchAnimation("Attack");
	_aniWait	= _pTaskModel->searchAnimation("Wait");
	_aniDamage	= _pTaskModel->searchAnimation("Damage");
	_aniHeal	= _pTaskModel->searchAnimation("Heal");
}

//-----------------------------------------------------------------------------
//! 更新(この中で更新すべきUpdateを呼び出す)
//-----------------------------------------------------------------------------
void EnemyBase::Update()
{
	// 死亡フラグが立っていたら処理しない
	if( _isDead ) return;
	// 所属している分隊からリーダーの位置を取得
	if( _keyType != KEY_LEADER ) { 
		_goalPos = _pSquad->getLeader()->getPosition();
	}

	s32	currentAnimation = _pTaskModel->getAnimation();

	// リーダーの命令によって処理変更
	UpdateByCommand();
	
	//---- 移動
	// キャラの移動(左スティックの情報取得)
	_mov._x = sinf( _rotation._y )  * _speed;
	_mov._z = cosf( _rotation._y )  * _speed;

	if( _speed > 0.0f ) 
	{
		// ﾃﾞｸﾞﾘｰに直す
		_rotation._y = TO_DEGREE(_rotation._y );
	}

	// 更新
	_oldPosition = _position;

	// フリーズしなかったら
	if( !checkFreeze(currentAnimation) ){
		// 移動してなければ
		if( _mov._x == 0.0f && _mov._z == 0.0f )
		{
			// 立ちモーション
			//_aniState = STATE_WAIT;
			if( currentAnimation != _aniWait ) {
				_pTaskModel->setAnimation(_aniWait, ANIMATION_PLAY_REPEAT);
			}
		}else{
			// 歩きモーション
			//_aniState = STATE_MOVE;
			if( currentAnimation != _aniMove ) {
				_pTaskModel->setAnimation(_aniMove, ANIMATION_PLAY_REPEAT);
			}
		}
	}

	// 攻撃してたら
	if( currentAnimation == _aniAttack ) {
		// 移動速度を少なくする
		_mov._x *= 0.5f;
		_mov._z *= 0.5f;
	}else if( currentAnimation == _aniDamage ){
		// 移動速度をなくす
		_mov._x *= 0.0f;
		_mov._z *= 0.0f;
	}

	_position += _mov * Global::deltaTime;

	//---- パーティクル
	// パーティクルの生成待ち時間カウント
	_genCount += 1 * Global::deltaTime;

	Vector3 sub = _oldPosition - _position;

	// 移動している且つ、生成待ち時間が切れた時
	if( (abs(sub._x) > 0.0f || abs(sub._z) > 0.0f) && _genCount >= _waitGenTime ) {

		// カウントリセット
		_genCount = 0;

		Vector3 pos = _position + Vector3( (f32)(rand() % 40 - 20) );

		Vector3 mov = -_mov;

		mov._x *= (rand() %  50) / 100.0f;
		mov._y  = (rand() % 100) / 100.0f;
		mov._z *= (rand() %  50) / 100.0f;

		f32		rot = ( rand() % 20 ) / 20.0f;
		Radian	angVel = Radian( ( rand() % 10 ) / 100.0f );

		// パーティクル生成
		IParticleMan()->generateParticle(pos,
										 mov,
										 Vector3(0.5f, 0.5f, 0.0f),
										 Radian(rot),
										 100, 0,
										 angVel,
										 ParticleManager::PARTICLE_SMOKE); 
	}


	// あたり判定の球の更新
	Man::Update();
	
	//---- マップとの当たり判定
	GmSystemCollision()->checkCollisionModel(_hitPosition, _radius, _hitGround);

	// 外にいけなくする
	ICastleCol()->hitOutRange(_hitPosition);

	// あたり判定用の球情報更新
	setSphereParam(_hitPosition, _hitRadius);
	// 座標の設定
	toPosition(_hitPosition, _position);

	// 重力処理
	if( !_hitGround ){
		_mov._y -= 0.6f * Global::deltaTime;
	}
	else{
		_mov._y = 0.0f;
	}

	// 最大重力設定
	if( _mov._y <= -9.8f )
	{
		_mov._y = -9.8f; 
	}

	if( _aniAttack != -1 ) {

		// 攻撃ボタンが押されているかどうか
		if( _key->getPushState("Attack") )
		{
			// 攻撃アニメーション再生
			_pTaskModel->setAnimation(_aniAttack);
			// 攻撃
			Attack();

		}else if ( currentAnimation == _aniAttack ) {

			// 攻撃再生中なら
			// 再生比率取得
			f32	aniRate = _pTaskModel->getAnimationRate();
			// 再生が半分いったら
			if( aniRate >= 0.5f && !_isAttack ) {
				// 攻撃
				Attack();
			}else if( _isAttack ){
				// 攻撃範囲、座標を戻す
				_attackPosition = Vector3( 0.0f, 0.0f, 0.0f );
				_attackRadius	= 0.0f;
			}

		}else{
			// この時点で半径が初期化されてなければ
			if( _attackRadius != 0.0f ){
				// 攻撃範囲、座標を戻す
				_attackPosition = Vector3( 0.0f, 0.0f, 0.0f );
				_attackRadius	= 0.0f;
			}
			// 攻撃フラグを元に戻す
			_isAttack = false;
		}

	}

	// 体力が０以下なら
	if( _myStatus->getParam()->_HP <= 0 )
	{
		// 死亡フラグを立てる
		_isDead = true;
	}
}

//-----------------------------------------------------------------------------
//!	描画
//-----------------------------------------------------------------------------
void EnemyBase::Render(bool isShadow)
{
	// 死亡フラグが立っていたら処理しない
	if( _isDead ) return;
	// カリングされるかどうか
	Sphere hitSphere = *_myCollision.getHitSphere();
	if (GmRender()->isRender(hitSphere)) return;

	// 移動用マトリックス
	Matrix	offsetMatrix(_pTaskModel->getWorldMatrix());
	offsetMatrix = offsetMatrix.translate(_position);
	Matrix rotateMatrix(_pTaskModel->getRotateMatrix());
	rotateMatrix = rotateMatrix.rotateY(Degree(_rotation._y) );
	_pTaskModel->setWorldMatrix(offsetMatrix);
	_pTaskModel->setRotateMatrix(rotateMatrix);
	
	_pTaskModel->render(isShadow);
}

//-----------------------------------------------------------------------------
//! glut描画関連
//-----------------------------------------------------------------------------
void EnemyBase::glutRender()
{
	// 死亡フラグが立っていたら処理しない
	if( _isDead ) return;
	// 青
	GLfloat blue[]	 = { 0.0f, 0.0f, 1.0f, 1.0f };
	// 黒紫
	GLfloat indigo[]	 = { 0.29f, 0.0f, 0.52f, 1.0f };
	// 緑
	GLfloat green[]	 = { 0.0f, 1.0f, 0.0f, 1.0f };

	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
	// デバック表示
	//_myCollision.DebugRender();

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
	_attackCollision.DebugRender();
}

//-----------------------------------------------------------------------------
//! デバッグ描画
//-----------------------------------------------------------------------------
void EnemyBase::debugRender()
{
	// 死亡フラグが立っていたら処理しない
	if( _isDead ) return;
	drawAABB( _colAABB, Vector4(1.0f, 1.0f, 1.0f, 1.0f), _position );
	if( _isAttack ) {
		drawAABB( _attackAABB, Vector4(1.0f, 0.0f, 0.0f, 1.0f), _attackPosition );
	}
}

//-----------------------------------------------------------------------------
//! ステータスの描画(2D)
//-----------------------------------------------------------------------------
void EnemyBase::StatusRender()
{
	// 死亡フラグが立っていたら処理しない
	if( _isDead ) return;
	// 描画座標設定
	Vector3 drawPos;
	drawPos._x = _position._x - _myStatus->getParam()->_GaugeScaleW * 0.5f;
	drawPos._y = _position._y + _radius + (20.0f * _modelScale);
	drawPos._z = _position._z;

	// 体力を赤で表示
	Color gaugeColor(255, 0, 0);
	// 描画
	_myStatus->Render(false, gaugeColor, drawPos);
}

//-----------------------------------------------------------------------------
//! 座標設定しなおし
//-----------------------------------------------------------------------------
void EnemyBase::ResetPosition()
{
# if 0 
	// 座標設定しなおし
	_position._x = (f32)( rand() % 1000 - 500 );
	_position._z = (f32)( rand() % 1000 - 500 );
#else
	// 座標設定しなおし(オフセット計算)
	_position._x = (f32)( rand() % 200 - 100 );
	_position._z = (f32)( rand() % 200 - 100 );
	// 目標座標に足す
	_position = _goalPos + _position;
	_position._y = _goalPos._y;
#endif
	// 攻撃当たってるフラグOFF
	_hitAttack = false;
}

//-----------------------------------------------------------------------------
//! ダメージ計算
//-----------------------------------------------------------------------------
s32 EnemyBase::calcDamageAndCollision()
{
	// ダメージ保存用
	s32 damage = 0;

	//---- 当たり判定
	for( s32 i=0; i<_myCollision.getHitCount(); ++i )
	{
		Collision* obj = _myCollision.getHitObj(i);
		// 名前がプレイヤーの攻撃なら
		if( isPlayerAttack(obj) ){
			_hitAttack = true;
			damage += (s32)IObjDataManager()->_pPlayer->getStatus()->getParam()->_atkPow;

		// プレイヤー味方(敵)の攻撃なら
		}else if( isAllyAttackerAttack(obj)  ){
			_hitAttack = true;
			damage += (s32)IObjDataManager()->_attackerStatus->getParam()->_atkPow;
		}else if( isAllyDeffenderAttack(obj) ){
			_hitAttack = true;
			damage += (s32)IObjDataManager()->_deffenderStatus->getParam()->_atkPow;
		}
	}

	return damage;
}

//-----------------------------------------------------------------------------
//! フリーズするかどうか
//-----------------------------------------------------------------------------
bool EnemyBase::checkFreeze(const s32&	currentAnimation)
{
	// アニメーションがなければ
	if( currentAnimation != -1 ){
		// 攻撃中ならフリーズ
		if( currentAnimation == _aniAttack ) return true;
		// ダメージ中ならフリーズ
		if( currentAnimation == _aniDamage ) return true;
		// ヒール中ならフリーズ
		if( currentAnimation == _aniHeal   ) return true;
	}
	// ここまできたらフリーズしない
	return false;
}

//-----------------------------------------------------------------------------
//! 狙う相手を探して設定
//-----------------------------------------------------------------------------
bool EnemyBase::searchAndSetTarget()
{
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

	// 敵(プレイヤーの味方)のデータを取得
	vector<AllyBase*> pAllyData = IObjDataManager()->_pAllyData;
	vector<AllyBase*>::iterator itr;
	f32 minLength = FLT_MAX;
	Man* target = nullptr;
	for( itr = pAllyData.begin(); itr != pAllyData.end(); itr++ )
	{
		AllyBase* ally = *itr;
		// 死亡していたら処理しない
		if( ally->isDead() ) continue;
		// 自分から相手へのベクトル
		f32 length = (ally->getPosition() - _position).squareLength();
		if( minLength > length )
		{
			// 更新
			minLength = length;
			target	  = ally;
		}
	}
	Player* player = IObjDataManager()->_pPlayer;
	// プレイヤーへの距離
	f32 toPlayerLength = (player->getPosition() - _position).squareLength();
	if( minLength > toPlayerLength )
	{
		// 更新
		target = player;
	}

	if( target != nullptr ) {
		// 次回用ターゲットに設定しておく
		_pPrevTarget = target;
		// 一番近いやつをターゲットにする
		_key->setTarget(target);
	}else{
		return false;
	}

	return true;
}
//=============================================================================
//	END OF FILE
//=============================================================================