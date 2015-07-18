//-----------------------------------------------------------------------------
//!
//!	@file	Man.cpp
//!	@brief	ゲーム内キャラ
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"




//-----------------------------------------------------------------------------
//! デフォルトコンストラクタ
//-----------------------------------------------------------------------------
Man::Man()
: _speed				(0.0f)
, _attackPosition		(0.0f, 0.0f, 0.0f)
, _attackRadius			(0.0f)
, _waitGenTime			(15.0f)
, _genCount				(0.0f)
, _attackOffsetRange	(20.0f)
, _hitGround			(false)
, _hitCastle			(false)
, _hitObj				(false)
, _hitAttack			(false)
, _isAttack				(false)
, _isJump				(false)
, _isDamage				(false)
, _pAssetModel			(nullptr)
, _pTaskModel			(nullptr)
, _hpOutLine			(nullptr)
, _key					(nullptr)
, _myStatus				(nullptr)
, _myLandScapleGridNum	(0)
{
}

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
Man::Man(KEY_TYPE myType, TYPE manType)
: _speed				(0.0f)
, _attackPosition		(0.0f, 0.0f, 0.0f)
, _attackRadius			(0.0f)
, _waitGenTime			(15.0f)
, _genCount				(0.0f)
, _attackOffsetRange	(20.0f)
, _hitGround			(false)
, _hitCastle			(false)
, _hitObj				(false)
, _hitAttack			(false)
, _isAttack				(false)
, _isJump				(false)
, _isDamage				(false)
, _pAssetModel			(nullptr)
, _pTaskModel			(nullptr)
, _hpOutLine			(nullptr)
, _key					(nullptr)
, _myStatus				(nullptr)
, _myLandScapleGridNum	(0)
, _type					(manType)
, _keyType				(myType)
{
	createStatus();
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
Man::~Man() 
{
	SAFE_DELETE(_pAssetModel);
	SAFE_DELETE(_pTaskModel);
	SAFE_DELETE(_hpOutLine);
	SAFE_DELETE(_key);
	SAFE_DELETE(_myStatus);
};

//-----------------------------------------------------------------------------
//! HPアウトラインのテクスチャ読み込み
//-----------------------------------------------------------------------------
bool Man::loadHPOutLineTexture(GM_CSTR fileName)
{
	// すでに読み込まれていたら
	if( _hpOutLine != nullptr ){
		return true;
	}

	SAFE_NEW(_hpOutLine);
	//! 読み込み
	if( !_hpOutLine->loadAndTexImage2D(fileName) )
	{
		MessageBox(NULL, (LPCWSTR)fileName, L"HPゲージの読み込み失敗",MB_OK); 
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
//! モデルの読み込み
//-----------------------------------------------------------------------------
bool Man::loadModel(GM_CSTR fileName, f32 scale)
{
	// モデルがすでに読み込まれているかどうか
	if( isModelLoaded() )
	{
		return true;
	}
	
	// モデル
	SAFE_NEW(_pAssetModel);
	// メモリ確保できているかどうか
	if( _pAssetModel == nullptr )
	{
		return false;
	}

	// 読み込み
	if( !_pAssetModel->load(fileName) )
	{
		MessageBox(NULL, (LPCWSTR)fileName, L"モデルの読み込みに失敗しました",MB_OK); 
		return false;
	}

	SAFE_NEW(_pTaskModel);
	_pTaskModel->setModel(_pAssetModel);
	_pTaskModel->setScale(scale);
	_pTaskModel->init();

	// 当たり判定用AABB作成
	f32 minX = _pAssetModel->getMinX();
	f32 maxX = _pAssetModel->getMaxX();
	f32 minY = _pAssetModel->getMinY();
	f32 maxY = _pAssetModel->getMaxY();
	f32 minZ = _pAssetModel->getMinZ();
	f32 maxZ = _pAssetModel->getMaxZ();
	Vector3 maxVec(maxX, maxY, maxZ);
	Vector3 minVec(minX, minY, minZ);
	_colAABB.setEmpty();
	_colAABB.expand(maxVec);
	_colAABB.expand(minVec);

	_radius = (_colAABB._max - _colAABB._min).length() * 0.5f;

	// あたり判定用半径取得
	_hitRadius = _radius;


	return true;
}
//-----------------------------------------------------------------------------
//! モデル読み込みされたかどうか
//-----------------------------------------------------------------------------
bool Man::isModelLoaded()
{
	if( _pAssetModel != nullptr )
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
//! キーの作成
//-----------------------------------------------------------------------------
bool Man::createKey(Man* me, Man* leader)
{

	// すでに作成されていたら処理しない
	if( _key != nullptr ) return true;

	switch( _keyType )
	{
		case KEY_INPUT:
			_key = new KeyInput(me, leader);
			break;
		case KEY_ATTACKER:
			_key = new KeyAttacker(me, leader);
			break;
		case KEY_DEFFENDER:	
			_key = new KeyDeffender(me, leader);
			break;
		case KEY_HEALLER:
			_key = new KeyHealer(me, leader);
			break;
		case KEY_LEADER:
			_key = new KeyAttacker(me, leader);
			break;
		/*case KEY_FEINTER:	
			break;*/
	}
	// 初期化失敗
	if( _key == nullptr ){
		LPCWSTR str[KEY_MAX] =
		{
			L"KeyInput",
			L"KeyAttacker",
			L"KeyDeffender",
			L"KeyHealer",
			L"KeyFeinter",
			L"KeyLeader"
		};
		MessageBox(NULL, str[_keyType], L"キーの作成失敗", MB_OK); 
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//! ステータスの作成
//-----------------------------------------------------------------------------
bool Man::createStatus()
{
	// すでに作成されていたら処理しない
	if( _myStatus != nullptr ) return true;

	switch( _keyType )
	{
		case KEY_INPUT:
			_myStatus = new Status();
			break;
		case KEY_ATTACKER:
			_myStatus = new Status();
			break;
		case KEY_DEFFENDER:	
			_myStatus = new Status();
			break;
		case KEY_HEALLER:
			_myStatus = new StatusHeal();
			break;
		case KEY_LEADER:
			_myStatus = new Status();
			break;
		/*case KEY_FEINTER:	
			break;*/
	}
	// 初期化失敗
	if( _myStatus == NULL ){
		LPCWSTR str[KEY_MAX] =
		{
			L"KeyInput",
			L"KeyAttacker",
			L"KeyDeffender",
			L"KeyHealer",
			L"KeyFeinter",
			L"KeyLeaader",
		};
		MessageBox(NULL, str[_keyType], L"ステータスの作成失敗", MB_OK); 
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//!	初期化
//-----------------------------------------------------------------------------
bool Man::Initialize()
{

	// ステータスを初期化
	//SAFE_NEW(_myStatus);
	
	// フラグ関連初期化
	_hitGround = false;
	_hitCastle = false;
	_hitObj	   = false;
	_isAttack  = false;
	_isJump	   = false;
	_isDead	   = false;
			   
	// 当たっているリストを初期化
	_myCollision.clearHitObj();
	// あたり判定用の座標更新
	toHitPosition(_position, _hitPosition);

	// あたり判定用の球情報更新
	setSphereParam(_hitPosition, _hitRadius);

	

	return true;
}

//-----------------------------------------------------------------------------
//! ジャンプ
//-----------------------------------------------------------------------------
void Man::Jump()
{
	// 地面と接地してたら
	//if( _hitGround  ) {
	if( !_isJump ) {
		// 初速設定
		_mov._y += 10.0f;
		// 接地フラグOFF
		_hitGround = false;
		// ジャンプフラグON
		_isJump	   = true;
	}
}

//-----------------------------------------------------------------------------
//! 攻撃
//-----------------------------------------------------------------------------
void Man::Attack()
{
	if( !_isAttack ) {
		// 座標からどれくらいずれるか
		Vector3 attackOffset;
		attackOffset._x = _attackOffsetRange * sinf( _rotation._y / 180.0f * math::PI);
		attackOffset._y = _hitRadius;				   
		attackOffset._z = _attackOffsetRange * cosf( _rotation._y / 180.0f * math::PI);
		// 座標にずれ分足す
	    _attackPosition =  _position + attackOffset;
		// 攻撃範囲設定
		_attackRadius	= _attackRadNum;

		// 攻撃音停止
		ISEManager()->stopMusic(SESoundPlayerManager::SE_ATTACK);
		// 攻撃音再生
		ISEManager()->playMusic(SESoundPlayerManager::SE_ATTACK);

		// 攻撃フラグON
		_isAttack		= true;
	}
}

//-----------------------------------------------------------------------------
//! ダメージ
//-----------------------------------------------------------------------------
void Man::Damage(const s32& damage)
{
	// 最小値以下なら
	//if(_myStatus->getHP() <= 0 ){ 
	s32 nowHP = (s32)_myStatus->getParam()->_HP;
	nowHP  -= damage;
	_myStatus->getParam()->_HP = (f32)nowHP;

	if(_myStatus->getParam()->_HP <= 0 ) {
		// 最小値で固定
		_myStatus->getParam()->_HP = 0;
		return;
	}

	// ダメージ音停止
	ISEManager()->stopMusic(SESoundPlayerManager::SE_DAMAGE);
	// ダメージ音再生
	ISEManager()->playMusic(SESoundPlayerManager::SE_DAMAGE);

	// パーティクル生成
	IParticleMan()->generateParticle(_hitPosition,
									 Vector3(0.0f),
									 Vector3(0.5f, 0.5f, 0.0f),
									 Radian(0.0f),
									 10, 0,
									 Radian(0.0f),
									 ParticleManager::PARTICLE_HIT); 

	_isDamage = true;
}

//-----------------------------------------------------------------------------
//! 座標再設定
//-----------------------------------------------------------------------------
void Man::ResetPosition()
{
	// 座標設定しなおし
	_position._x = (f32)( rand() % 200 - 100 );
	_position._z = (f32)( rand() % 200 - 100 );
}

//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void Man::Update()
{
	// もしも地面より下に言ったときの救済措置
	if( _position._y <= -1000.0f ){
		_position._y = 100.0f;
	}

	// あたり判定用の座標更新
	toHitPosition(_position, _hitPosition);

	// あたり判定用の球情報更新
	//setSphereParam(_position, _hitRadius);
	setSphereParam(_hitPosition, _hitRadius);
	// 攻撃用の球情報更新
	_attackSphere.setPosition(_attackPosition);
	_attackSphere.setRadius(_attackRadius);
}

//-----------------------------------------------------------------------------
//!	描画
//-----------------------------------------------------------------------------
void Man::Render(bool isShadow)
{
}


//=============================================================================
//	END OF FILE
//=============================================================================