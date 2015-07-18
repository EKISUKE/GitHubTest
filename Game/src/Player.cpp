//-----------------------------------------------------------------------------
//!
//!	@file	Player.cpp
//!	@brief	プレイヤー
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"



// ここでmyNameをマクロで宣言
DEF_COL_NAME( Player )
DEF_COL_ATTACK_NAME(PlayerAttack);

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
Player::Player()
: Man(Man::KEY_INPUT, Man::TYPE_PLAYER)
, _commandChangeMode(false)
, _gameOver			(false)
, _isNearCastle		(false)
{
	// 当たり判定初期化
	_myCollision.setCollisionState(&_hitSphere, myName);
	_attackCollision.setCollisionState(&_attackSphere, myAttackName);

	loadModel("../Model/Player/Player.x", 5.0f);
	
	loadHPOutLineTexture("../Image/UI/HPBar.png");

	// キーの作成
	if( !createKey(this, this) )
	{
		MessageBoxA(NULL, "プレイヤーのキーの初期化に失敗しました。", "エラー", MB_OK);
	}

	// 初期化
	Initialize();
}
//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
Player::~Player()
{
}

//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool Player::Initialize()
{
		
	_command = COMMAND_NONE;
	// 座標の初期化
	_position._x = -3200.0f;
	_position._y = 0.0f;
	_position._z = -2600.0f;
	
	_rotation	 = Vector3(0.0f, 50.0f, 0.0f);

	// 初期化
	Man::Initialize();

	// ステータス初期化
	_myStatus->setParam(StatusAssets::STATUS_PLAYER);
	// 攻撃時の半径設定
	_attackRadNum		= _radius * 0.5f;
	_attackAABB._min	= -Vector3(_radius * 0.5f);
	_attackAABB._max	=  Vector3(_radius * 0.5f);
	_attackOffsetRange	= _attackRadNum * 1.5f;

	// ゲームオーバーフラグOFF
	_gameOver			= false;
	return true;
}


Vector3 drawPointPos;

//-----------------------------------------------------------------------------
//! 更新処理
//-----------------------------------------------------------------------------
void Player::Update()
{
	// 更新
	_key->Update();


	if( GetKeyState('R') & 0x80 ) {
		_position._x = 1300.0f;
		_position._y = 1000.0f;
		_position._z = 760.0f;
	}

	//---- キャラ操作
	//---- 移動処理
	_speed = 0.0f;
	// コントローラーのスティックの角度を求める
	Vector3	StickState	= _key->getVector();
		

	// 命令変更ボタンが押されていれば変更モードへ(後で使用する)
	//if( _key->getPushState(KeyBase::KEY_COMMAND_COMMANDCHANGE) ) 
	/*if( _key->getPushState("CommandChange") )
	{
		_commandChangeMode = true;
	}else{
		_commandChangeMode = false;
	}*/
	

	////----　更新モードによって更新呼び分け(後で使用する)
	//if( _commandChangeMode ) {
	//	UpdateCommandChange(_key->getStickVector());
	//}else{
	//	UpdateNormal(StickState);
	//}

	
	if( _isDead ) {
		UpdateGameOver();
	}else{
		UpdateNormal(StickState);

		// 更新
		_oldPosition = _position;

		// 攻撃してなかったら
		if( !checkFreeze(_aniState) ) {
			// 移動してなければ
			if( _mov._x == 0.0f && _mov._z == 0.0f )
			{
				// 立ちモーション
				_aniState = STATE_WAIT;
				if( _pTaskModel->getAnimation() != STATE_WAIT ) {
					_pTaskModel->setAnimationSpeed(1.0f);
					_pTaskModel->setAnimation(_aniState, ANIMATION_PLAY_REPEAT);
				}
			}else{
				// 歩きモーション
				_aniState = STATE_MOVE;
				f32 knock = StickState.length();
				_pTaskModel->setAnimationSpeed(knock);
				if( _pTaskModel->getAnimation() != STATE_MOVE ) {
					_pTaskModel->setAnimation(_aniState, ANIMATION_PLAY_REPEAT);
					//_pTaskModel->setAnimation(_aniState, ANIMATION_PLAY_DEFAULT);
				}
			}
		}

	

		// 攻撃してたら
		if( _aniState == STATE_ATTACK ) {
			// 移動速度を少なくする
			_mov._x *= 0.5f;
			_mov._z *= 0.5f;
		}else if( _aniState == STATE_DAMAGE ){
			// ダメージを受けていたら
			// 移動速度を少なくする
			_mov._x *= 0.1f;
			_mov._z *= 0.1f;
		}


		// 移動
		_position += _mov * Global::deltaTime;

		// 瞬間移動バグ検出用
		if( (_oldPosition - _position).length() >= 1000.0f ) {
			s32 a = 0;
		}

		//---- パーティクル
		// パーティクルの生成待ち時間カウント
		_genCount += 1 * Global::deltaTime;

		Vector3 sub = _oldPosition - _position;

		// 移動している且つ、生成待ち時間が切れた時
		if( (abs(sub._x) > 0.0f || abs(sub._z) > 0.0f) 
			&& _genCount >= _waitGenTime 
			&& _hitGround) {

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

		// 当たり判定とダメージ計算
		s32 damage = calcDamageAndCollision();

		// ダメージがあったら
		if( damage > 0 ) {
			Damage(damage);
			// ダメージアニメーションへ
			_aniState = STATE_DAMAGE;
			_pTaskModel->setAnimationSpeed(1.0f);
			_pTaskModel->setAnimation(_aniState);
		}else if( _aniState == STATE_DAMAGE ){
			_isDamage = false;
			// 現在再生中のアニメーションにする(再生が終わっていればSTATE_NONE)
			_aniState = (ANIMATION_STATE)_pTaskModel->getAnimation();
		}
	
	

		//---- 地面との当たり判定
	#if 1 
		GmSystemCollision()->checkCollisionModel(_hitPosition, _radius, _hitGround);
	#else
		if( _position._y <= 0.0f )
		{
			_hitGround = true;
		}else{
			_hitGround = false;
		}
	#endif

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
		// 描画座標変更
		drawPointPos = GmSystemCollision()->getLastHitPosForWall();

	

		// 重力処理
		if( !_hitGround )
		{
			_mov._y -= 0.6f * Global::deltaTime;
		}
		else{
			_mov._y = 0.0f;
			_isJump = false;
			// 現在再生中のアニメーションにする(再生が終わっていればSTATE_NONE)
			_aniState = (ANIMATION_STATE)_pTaskModel->getAnimation();
		}

		// 最大重力設定
		if( _mov._y <= -9.8f )
		{
			_mov._y = -9.8f; 
		}
	
		_pTaskModel->update();

		if(_myStatus->getParam()->_HP <= 0 ){
			_isDead = true;
		}
	}		// isDead == false

	CameraBase* pCamera = GmCameraMan()->getCurrentCamera();

	if(_isNearCastle) {
		// 現在の注視点との距離を取得
		f32 lookAtLength = pCamera->getLookAtLength();
		static const f32 maxLookAtLength = 600.0f;
		if(lookAtLength < maxLookAtLength) {
			pCamera->changeLookAtLength(4.0f);
		}
	}else{
		pCamera->setLookAtLengthMax(200.0f);
	}
}


//-----------------------------------------------------------------------------
//! 通常時更新
//-----------------------------------------------------------------------------
void Player::UpdateNormal(Vector3& stickState)
{

	// スティック操作があった時(0.0以外のとき)はプレイヤーの角度に割り当てる
	if( abs( TO_DEGREE(atan2f( stickState._x, stickState._z )) )  > 0.0f ){
			_rotation._y = TO_DEGREE(atan2f( stickState._x, stickState._z ) );
			// 速度設定
			//_speed = _myStatus->getInfOfStatus(Status::STATUS_SPEEDMAX);
			Status::Param* myParam = (Status::Param*) _myStatus->getParam();
			_speed = myParam->_speedMAX;
	}
	

	// 移動量設定
	_mov._x = _speed * stickState._x;
	_mov._z = _speed * stickState._z;


	//---- ジャンプ操作
	//if( _key->getPushState(KeyBase::KEY_COMMAND_JUMP) ) 
	if( _key->getPushState("Jump") )
	{
		if( !_isJump ) {
			_aniState = STATE_JUMP;
			_pTaskModel->setAnimationSpeed(1.0f);
			_pTaskModel->setAnimation(_aniState);
		}
		Jump();
		
	}


	//---- 攻撃操作
	if( _key->getPushState("Attack") )
	{
		// アニメーションを攻撃に移行
		_aniState = STATE_ATTACK;
		// 攻撃アニメーション再生
		_pTaskModel->setAnimationSpeed(1.0f);
		_pTaskModel->setAnimation(_aniState);
	}else if ( _pTaskModel->getAnimation() == STATE_ATTACK ) {
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
		// 現在再生中のアニメーションにする(再生が終わっていればSTATE_NONE)
		_aniState = (ANIMATION_STATE)_pTaskModel->getAnimation();
	}

	//---- デバッグ用あたり判定無効化
	//if( control.GetPadState(control.PAD_RT) ){
	//_hitObj = false;
		
		
	/*if( _hitObj ){
		_mov._x = 0.0f;
		_mov._y = 0.0f;
		_mov._z = 0.0f;
	}*/
	
}

//-----------------------------------------------------------------------------
//! 命令変更時更新
//-----------------------------------------------------------------------------
void Player::UpdateCommandChange(Vector3& stickState)
{
	// 左スティックで命令変更
	Vector3 dir = stickState;

	dir.normalize();

	if( dir._z >  0.7f )
	{
		_command = COMMAND_ATTACK;
	}

	if( dir._z < -0.7f ) 
	{
		_command = COMMAND_BACK;
	}

	if( dir._z == 0.0f ){
		_command = COMMAND_NONE;
	}

}

//-----------------------------------------------------------------------------
//! ゲームオーバー時の更新
//-----------------------------------------------------------------------------
void Player::UpdateGameOver()
{
	static bool isGameOverAnimation = false;
	if( _aniState != STATE_GAMEOVER ){
		_aniState = STATE_GAMEOVER;
		_pTaskModel->setAnimationSpeed(1.0f);
		_pTaskModel->setAnimation(_aniState);
		isGameOverAnimation = true;
	}
	// ゲームオーバーのアニメーションが始まってて
	if( isGameOverAnimation ){
		if( _pTaskModel->getAnimation() == STATE_NONE ){
			_gameOver = true;
		}
	}

	_pTaskModel->update();
}


//-----------------------------------------------------------------------------
//! 描画処理
//-----------------------------------------------------------------------------
void Player::Render(bool isShadow)
{
	static float debugRotation = 0.0f;

	debugRotation += 0.01f;

	// 移動用マトリックス
	Matrix	offsetMatrix(_pTaskModel->getWorldMatrix());
	offsetMatrix = offsetMatrix.translate(_position);
	Matrix rotateMatrix(_pTaskModel->getRotateMatrix());
	rotateMatrix = rotateMatrix.rotateY(Degree(_rotation._y) );
	//rotateMatrix = rotateMatrix.rotateY(debugRotation);
	_pTaskModel->setWorldMatrix(offsetMatrix);
	_pTaskModel->setRotateMatrix(rotateMatrix);

	_pTaskModel->render(isShadow);

}

//-----------------------------------------------------------------------------
//! glut描画関連
//-----------------------------------------------------------------------------
void Player::glutRender()
{

	// 赤
	//GLfloat red[]	 = { 1.0f, 0.0f, 0.0f, 0.3f };
	GLfloat red[]	 = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat blue[]	 = { 0.0f, 0.0f, 1.0f, 1.0f };
	GLfloat white[]	 = { 1.0f, 1.0f, 1.0f, 1.0f };

	//// デバック表示
	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
	//_myCollision.DebugRender();
	// 攻撃のビジュアル化
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
	_attackCollision.DebugRender();

	CommandRender();
}

//-----------------------------------------------------------------------------
//! デバッグ描画
//-----------------------------------------------------------------------------
void Player::debugRender()
{
	drawAABB( _colAABB, Vector4(1.0f, 1.0f, 1.0f, 1.0f), _position );
	if( _isAttack ) {
		drawAABB( _attackAABB, Vector4(1.0f, 0.0f, 0.0f, 1.0f), _attackPosition );
	}
}

//-----------------------------------------------------------------------------
//! 命令用描画
//-----------------------------------------------------------------------------
void Player::CommandRender()
{
	if( !_commandChangeMode ) return;
	
	GLfloat red[]	 = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat blue[]	 = { 0.0f, 0.0f, 1.0f, 1.0f };




	Vector3 CamPos  = GmCameraMan()->getCurrentCamera()->getPosition();
	Vector3 CamLook = GmCameraMan()->getCurrentCamera()->getLookAt();
	Vector3	WorldUp	= GmCameraMan()->getCurrentCamera()->getWorldUp();

	Vector3 axisZ	= (CamLook - CamPos).normalize();
	Vector3	axisX	= Vector3::cross(WorldUp, axisZ).normalize();
	Vector3	axisY	= Vector3::cross(axisZ, axisX).normalize();

	f32		CamRot	= GmCameraMan()->getCurrentCamera()->getRotation()._y;

	
	// 基準の座標
	Vector3	BasePos;
	// 攻撃の球の座標
	Vector3	position1;
	// 撤退の球の座標
	Vector3	position2;

	axisZ = axisZ.normalize();

	// カメラの見てる先の方向に100だけ伸びた座標
	// この座標から一定量ずらして描画
	BasePos._x	= CamPos._x + axisZ._x * 100.0f;
	BasePos._y	= CamPos._y + axisZ._y * 100.0f;
	BasePos._z	= CamPos._z + axisZ._z * 100.0f;
	
	position1	= BasePos;
	position2	= BasePos;

	position1._y += axisY._y * 30.0f;

	position2._y -= axisY._y * 30.0f;

	f32	radius1 = 10.0f;
	f32	radius2 = 10.0f;

	if( _command == COMMAND_ATTACK )
	{
		radius1 = 16.0f;
	}else if( _command == COMMAND_BACK )
	{
		radius2 = 16.0f;
	}

	// 攻撃用の球
	glPushMatrix();
		glTranslatef(position1._x, position1._y, position1._z);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
		setBlendMode(BM_ADD_ALPHA);		//================================ 【半透明処理開始】
		glutSolidSphere(radius1,16,16);
		setBlendMode(BM_NONE);			//================================ 【半透明処理終了】
	glPopMatrix();

	// 撤退用の球
	glPushMatrix();
		glTranslatef(position2._x, position2._y, position2._z);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
		setBlendMode(BM_ADD_ALPHA);		//================================ 【半透明処理開始】
		glutSolidSphere(radius2,16,16);
		setBlendMode(BM_NONE);			//================================ 【半透明処理終了】
	glPopMatrix();


	
}


//-----------------------------------------------------------------------------
//! ステータスの描画(2D)
//-----------------------------------------------------------------------------
void Player::StatusRender()
{
	// 体力を緑で表示
	glColor3f(0,255,0);
	// 体力の描画
	Vector3 drawPos(100.0, 100.0f, 0.0f);
	Color gaugeColor(0,255,0);
	// 体力ゲージの描画
	_myStatus->Render(true, gaugeColor, drawPos, _hpOutLine);
}

//-----------------------------------------------------------------------------
//! ダメージ計算と当たり判定処理
//-----------------------------------------------------------------------------
s32 Player::calcDamageAndCollision()
{
	s32 damage = 0;
	//---- 当たり判定
	for( s32 i=0; i<_myCollision.getHitCount(); ++i )
	{
		Collision* obj = _myCollision.getHitObj(i);
		//// 名前が敵なら
		//if( isEnemy(obj) ){
		//	// 補正位置を取得する
		//	Vector3	dir = _myCollision.getOffsetPos(obj);
		//	// 補正位置に設定
		//	_hitPosition = dir;
		//	// あたり判定用の球情報更新
		//	setSphereParam(_hitPosition, _hitRadius);
		//}else if( isEnemyAttack(obj) ) {
		if( isEnemyLeaderAttack(obj) || isEnemyAttakerAttack(obj) || isEnemyDeffenderAttack(obj) ){
			// ダメージ判定(防御ステータスを考慮してダメージ計算を行う予定)
			damage += (s32)( IObjDataManager()->_pEnemyData[0]->getStatus()->getParam()->_atkPow * 0.4f );
		}
		// 座標の設定
		toPosition(_hitPosition, _position);
	}
	return damage;
}

//-----------------------------------------------------------------------------
//! アニメーションによる移動不可チェック
//-----------------------------------------------------------------------------
bool Player::checkFreeze(const s32&	currentAnimation)
{
	// アニメーションがなければ
	if( currentAnimation != -1 ){
		// 攻撃中ならフリーズ
		if( currentAnimation == STATE_ATTACK ) return true;
		// ダメージ中ならフリーズ
		if( currentAnimation == STATE_DAMAGE ) return true;
		// ジャンプ中ならフリーズ
		if( currentAnimation == STATE_JUMP   ) return true;
	}
	// ここまできたらフリーズしない
	return false;
}
//=============================================================================
//	END OF FILE
//=============================================================================