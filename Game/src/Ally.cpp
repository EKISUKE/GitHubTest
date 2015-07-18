//-----------------------------------------------------------------------------
//!
//!	@file	Ally.cpp
//!	@brief	味方基底
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

//=============================================================================
//	味方基底
//=============================================================================

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
AllyBase::AllyBase()
: Man(Man::KEY_ATTACKER,  Man::TYPE_ALLY)
, _pSquad		(nullptr)
, _pPrevTarget	(nullptr)
, _goGoal		(true)
{
	
}

//-----------------------------------------------------------------------------
//! コンストラクタ
//!	@param	[in]	myType	キータイプ
//-----------------------------------------------------------------------------
AllyBase::AllyBase(Man::KEY_TYPE myType)
: Man(myType,  Man::TYPE_ALLY)
, _pSquad		(nullptr)
, _pPrevTarget	(nullptr)
, _goGoal		(true)
{
	
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
AllyBase::~AllyBase()
{
}
//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool AllyBase::Initialize()
{
	// プレイヤーの初期位置に設定する
	_leaderPos = Vector3(-3200.0f, 0.0f, -2600.0f);

	// 座標設定
	ResetPosition();
	// 初期化
	Man::Initialize();
	return true;
}


//! アニメーション初期化
void AllyBase::initAnimation()
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
void AllyBase::Update()
{
	// 死亡フラグが立っていたら処理しない
	if( _isDead ) return;
	
	// 所属している分隊からリーダーの位置を取得
	_leaderPos = _pSquad->getLeader()->getPosition();

	// 現在のアニメーションを取得
	s32	currentAnimation = _pTaskModel->getAnimation();


// デバッグ用
_DebugGoalPos = IObjDataManager()->_pPlayer->getPosition();

	// プレイヤーの命令によって処理変更
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

	//---- 城、城壁との当たり判定
	Vector3 sink = ICastleCol()->getSinkVal(_colAABB, _hitCastle, _position);
	
	if( _hitCastle )
	{
		_hitPosition += sink;
	}

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
			// アニメーションを攻撃に移行
			//_aniState = STATE_ATTACK;
			// 攻撃アニメーション再生
			_pTaskModel->setAnimation(_aniAttack);
		}else if ( currentAnimation == _aniAttack ) {

			// 攻撃再生中なら
			// 再生比率取得
			f32	aniRate = _pTaskModel->getAnimationRate();
			// 再生が半分いったら
			if( aniRate >= 0.5f && !_isAttack ) {
				// 攻撃
				Attack();
				ICastleCol()->hitAttackAndDamage(_attackAABB, _attackPosition, (u32)_myStatus->getParam()->_atkPow);	
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
		_isDead = true;
	}
	
	
}

//-----------------------------------------------------------------------------
//! 描画
//-----------------------------------------------------------------------------
void AllyBase::Render(bool isShadow)
{
	// 死亡フラグが立っていたら処理しない
	if( _isDead ) return;
	// カリングされるかどうか
	Sphere hitSphere = *_myCollision.getHitSphere();
	if( GmRender()->isRender(hitSphere) ) return; 

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
void AllyBase::glutRender()
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
void AllyBase::debugRender()
{
	// 死亡フラグが立っていたら処理しない
	if( _isDead ) return;

	drawAABB( _colAABB, Vector4(1.0f, 1.0f, 1.0f, 1.0f), _position );
	if( _isAttack ) {
		drawAABB( _attackAABB, Vector4(1.0f, 0.0f, 0.0f, 1.0f), _attackPosition );
	}
}

//-----------------------------------------------------------------------------
//! ステータスの描画(3D)
//-----------------------------------------------------------------------------
void AllyBase::StatusRender()
{
	// 死亡フラグが立っていたら処理しない
	if( _isDead ) return;

	// 描画座標設定
	Vector3 drawPos;
	drawPos._x = _position._x - _myStatus->getParam()->_GaugeScaleW * 0.5f;
	drawPos._y = _position._y + _radius + 20.0f;
	drawPos._z = _position._z;

	// 体力を青で表示
	Color gaugeColor(0, 0, 255);
	// 描画
	_myStatus->Render(false, gaugeColor, drawPos);
}


//-----------------------------------------------------------------------------
//! 座標再設定
//-----------------------------------------------------------------------------
void AllyBase::ResetPosition()
{
	// 座標設定しなおし(オフセット計算)
	_position._x = (f32)( rand() % 200 - 100 );
	_position._z = (f32)( rand() % 200 - 100 );
	// 目標座標に足す
	_position = _leaderPos + _position;
	_position._y = _leaderPos._y;
	// 攻撃当たってるフラグOFF
	_hitAttack = false;
}

//-----------------------------------------------------------------------------
//! 狙う相手を探して設定
//!	@return true:敵を設定完了 false:狙う敵はいない
//-----------------------------------------------------------------------------
bool AllyBase::searchAndSetTarget()
{
	Vector3	playerPos = IObjDataManager()->_pPlayer->getPosition();
	// 前の情報があったらかつ、死亡していなければ
	if( _pPrevTarget != nullptr && !_pPrevTarget->isDead() ) {
		f32 lengthToTarget = getLengthtoTarget(_pPrevTarget);
		// 捜索範囲内なら
		static const f32 searchLength = 200.0f;
		if( lengthToTarget < searchLength ) {
			// 検索しない(敵は設定されている)
			return true;
		}
	}

	// 敵のステータスをもらってくる
	vector<EnemyBase*> pEnemyData = IObjDataManager()->_pEnemyData;
	vector<EnemyBase*>::iterator	itr;
	f32	 minLength = FLT_MAX;
	EnemyBase* target = nullptr;
	for( itr = pEnemyData.begin(); itr != pEnemyData.end(); itr++ )
	{
		EnemyBase* enemy = *itr;
		// 死亡していたら処理しない
		if( enemy->isDead() ) continue;
		// プレイヤーから敵への距離を取得
		f32 length = (enemy->getPosition() - playerPos).squareLength();
		// 今回の長さが短ければ
		if( minLength > length )
		{
			// 更新
			minLength = length;
			target	  = enemy;
		}
	}
	// 一番近い敵をターゲットに設定
	if( target != nullptr ) {
		// 次回用ターゲットに設定しておく
		_pPrevTarget = target;
		// 一番近いやつをターゲットにする
		_key->setTarget(target);
	}else{
		// 狙う敵はいない
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//! アニメーションによる移動不可チェック
//-----------------------------------------------------------------------------
bool AllyBase::checkFreeze(const s32&	currentAnimation)
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

//=============================================================================
//	END OF FILE
//=============================================================================