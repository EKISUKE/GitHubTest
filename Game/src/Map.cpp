//-----------------------------------------------------------------------------
//!
//!	@file	Map.cpp
//!	@brief	マップ既定
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"


//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
Map::Map()
: _pAssetMapModel(nullptr)
, _pTaskMapModel (nullptr)
{
	// モデル初期化
	SAFE_NEW(_pAssetMapModel);

	// 地面モデルの読み込み
	//if(!_pAssetMapModel->load("../Model/Map/TestMap/testMapTransReset.x")){
	if (!_pAssetMapModel->load("../Model/Map/TestMap/testMapAdjustment.x")){
	//if (!_pAssetMapModel->load("../Model/Map/TestMap/testMap.x")){
	//if(_model->load( "../Model/Map/Ground/ground_high.mqo", 1.0f ) == false){
	//if(_model->load( "../Model/Map/Ground/ground_big.mqo", 1.0f ) == false){
	//if(_model->load( "../Model/Map/Ground/Map.mqo", 1.0f ) == false){
		//MessageBox(NULL, L"マップモデルの読み込みに失敗しました", L"エラー", MB_OK); 
		GM_ASSERT(false, "マップモデルの読み込みに失敗しました");
	}

	SAFE_NEW(_pTaskMapModel);
	_pTaskMapModel->setModel(_pAssetMapModel);
	_pTaskMapModel->setScale(5.0f);
	_pTaskMapModel->init();

	// リピート数設定
	AssetModelX::Frame* pGroundFrame = _pAssetMapModel->searchFrame("Ground");

	s32 texRepeat = 10;

	if( pGroundFrame ) {
		pGroundFrame->setTexRepeat(texRepeat);
	}

	// スケール値設定
	f32 minX = _pAssetMapModel->getMinX();
	f32 maxX = _pAssetMapModel->getMaxX();
	f32 minY = _pAssetMapModel->getMinY();
	f32 maxY = _pAssetMapModel->getMaxY();
	f32 minZ = _pAssetMapModel->getMinZ();
	f32 maxZ = _pAssetMapModel->getMaxZ();


	ICasCaded()->setMinMax(minX, maxX, minY, maxY, minZ, maxZ);

	//---- 当たり判定モデル読み込み
	//if( !ISystemCollision()->loadXfile(_pTestMapTask) )
	if( !GmSystemCollision()->loadLandScaleWithFrame(pGroundFrame, _pTaskMapModel) )
	{
		GM_ASSERT( false, "ランドスケープ読み込み失敗" );
	}

	// 城のフレーム取得
	AssetModelX::Frame* castle = _pAssetMapModel->searchFrame("castle");

	// 城クラス初期化
	_castle	= new Castle(castle);




	string frameName = "Wall";
	AssetModelX::Frame* wall = _pAssetMapModel->searchFrame(frameName.c_str());
	_myWall					 = new CastleWall(wall);


	//// 城壁クラス初期化
	//for( u32 i=0; i<WALL_MAX; ++i )
	//{
	//	string frameName = "wall";
	//	AssetModelX::Frame* wall = _assetMapModel->searchFrame(frameName.c_str());

	//	_myWall[i] = new CastleWall(wall);
	//}

	// 初期化
	Initialize();
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
Map::~Map()
{
	SAFE_DELETE(_pAssetMapModel);
	SAFE_DELETE(_pTaskMapModel);
	SAFE_DELETE(_castle);
	/*for( u32 i=0; i<WALL_MAX; ++i )
	{
		SAFE_DELETE(_myWall[i]);
	}*/

	SAFE_DELETE(_myWall);
}

//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool Map::Initialize()
{
	// 城の初期化
	_castle->Initialize();

	//// 城壁の初期化
	//for( s32 i=0; i<WALL_MAX; ++i )
	//{
	//	_myWall[i]->Initialize();
	//}
	_myWall->Initialize();

	// 当たり判定設定
	ICastleCol()->setCastleCol(_castle, _myWall);

	return true;
}
//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void Map::Update()
{

	_pTaskMapModel->update();

	// 城の更新
	_castle->Update();

	//// 城壁の更新
	//for( s32 i=0; i<WALL_MAX; ++i )
	//{
	//	_myWall[i]->Update();
	//}
	_myWall->Update();
}
//-----------------------------------------------------------------------------
//! 描画
//-----------------------------------------------------------------------------
void Map::Render(bool isShadow)
{
	Matrix offsetMatrix = Matrix::translate(_position);
	_pTaskMapModel->setWorldMatrix(offsetMatrix);
	_pTaskMapModel->render(isShadow);
}

//-----------------------------------------------------------------------------
//! デバッグ描画
//-----------------------------------------------------------------------------
void Map::debugRender()
{
	//---- デバッグ描画
	_castle->debugRender();
	_myWall->debugRender();
}

//=============================================================================
//	城壁クラスの実装
//=============================================================================
//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
CastleWall::CastleWall(AssetModelX::Frame* wall)
: _myStatus(nullptr)
, _isCrash (false)
{
	_wallModel = wall;

	// 当たり判定用AABB
	f32 minX = _wallModel->getMinX();
	f32 maxX = _wallModel->getMaxX();
	f32 minY = _wallModel->getMinY();
	f32 maxY = _wallModel->getMaxY();
	f32 minZ = _wallModel->getMinZ();
	f32 maxZ = _wallModel->getMaxZ();

	Vector3 maxVec(maxX, maxY, maxZ);
	Vector3 minVec(minX, minY, minZ);
	_colAABB.setEmpty();
	_colAABB.expand(maxVec);
	_colAABB.expand(minVec);

	

	SAFE_NEW(_myStatus);
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
CastleWall::~CastleWall()
{
	SAFE_DELETE(_myStatus);
}

//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool CastleWall::Initialize()
{
	
	//// HPの初期化
	//Status::Param param;
	//_myStatus->setParam;
	//// ゲージのサイズを設定
	//_myStatus->setGaugeScale(10.0f, 20.0f);
	//// つぶされた城壁を戻す処理

	_drawPosition = Vector3(-3.646f, 47.134f, 9.408f);
	_wallModel->setOffset(_drawPosition);
	// 描画フラグもどす
	_wallModel->setRender(true);
	_myStatus->setStatus(1500, 1500);

	_isCrash  = false;
	_isDamage = false;
	return true;
}

//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void CastleWall::Update()
{
	// 体力がなくなったら
	if( !_myStatus->_isActive ){
		// 崩れる
		Crash();
	}

	if(_isDamage){
		
		Controller*	currentController = GmControlMan()->getController(1);
		CameraBase*	currentCamera	  = GmCameraMan()->getCurrentCamera();
		static s32 count;
		count++;
		if( count >= 10 ){
			count = 0;
			_isDamage = false;
			// 元の位置で描画
			_wallModel->setOffset(_drawPosition);
			currentController->DisableVibration();
//			currentCamera->Disablevibration();
		}else{
			f32 vib = LinearInterpolation(1.0f, 0.0f, (f32)_myStatus->_hp, (f32)_myStatus->_hpMax);
			if( vib > 1.0f ){
				vib = 1.0f;
			}
			currentController->EnableVibration(vib, vib);
			// カメラぶれするとスカイボックスがくずれる(スカイボックスはカメラの位置をもとにしてるため)
//			currentCamera->Enablevibration(vib);
			Vibration();
		}
	}
}

//-----------------------------------------------------------------------------
//! デバッグ描画
//-----------------------------------------------------------------------------
void CastleWall::debugRender()
{
	drawAABB( _colAABB, Vector4(1.0f, 1.0f, 1.0f, 1.0f) );
}

//-----------------------------------------------------------------------------
//! 壁が崩れる
//-----------------------------------------------------------------------------
void CastleWall::Crash()
{
	if( _drawPosition._y >= -100.0f ) {

		// 崩れる音再生
		ISEManager()->playMusic(SESoundPlayerManager::SE_CRASH);
		_isDamage = true;
		_drawPosition._y -= 0.4f * Global::deltaTime;
		_wallModel->setOffset(_drawPosition);

		static f32 maxX = _colAABB._max._x;
		static f32 minX = _colAABB._min._x;
		static f32 maxZ = _colAABB._max._z;
		static f32 minZ = _colAABB._min._z;

		for( s32 i=0; i<10; ++i) {
			Vector3 pos = Vector3(0,0,0);

			s32 random = rand() % 4;
			if( random == 0 ){
				pos._x =  minX;
				pos._y = _drawPosition._y - 200.0f;
				pos._z = _drawPosition._x + (f32)(rand() % (s32)(maxZ * 2) - maxZ);
			}else if( random == 1 ){
				pos._x = maxX;
				pos._y = _drawPosition._y - 200.0f;
				pos._z = _drawPosition._x + (f32)(rand() % (s32)(maxZ * 2) - maxZ);
			}else if( random == 2 ) {
				pos._x = _drawPosition._x + (f32)(rand() % (s32)(maxX * 2) - maxX);
				pos._y = _drawPosition._y - 200.0f;
				pos._z = minZ;
			}else if( random == 3 ) {
				pos._x = _drawPosition._x + (f32)(rand() % (s32)(maxX * 2) - maxX);
				pos._y = _drawPosition._y - 200.0f;
				pos._z = maxZ;
			}
			
			Vector3 mov = Vector3(0.0f, 0.01f, 0.0f);

			mov._x *= (rand() %  50) / 100.0f;
			mov._y  = (rand() % 100) / 100.0f;
			mov._z *= (rand() %  50) / 100.0f;

			f32		rot = ( rand() % 20 ) / 20.0f;
			Radian	angVel = Radian( ( rand() % 10 ) / 100.0f );

			// パーティクル生成
			IParticleMan()->generateParticle(pos,
											 mov,
											 Vector3(1.0f, 1.0f, 0.0f),
											 Radian(rot),
											 100, 0,
											 angVel,
											 ParticleManager::PARTICLE_SMOKE); 
		}

	}else if( !_isCrash ) {
		ISEManager()->stopMusic(SESoundPlayerManager::SE_CRASH);
		// 描画フラグもどす
		_wallModel->setRender(false);
		_isCrash = true;
	}
}

//-----------------------------------------------------------------------------
//! ぶれ
//-----------------------------------------------------------------------------
void CastleWall::Vibration()
{
	static Vector3 offset;
	offset._x = ( rand() % 32768 ) * (1.0f / 32768.0f);
	offset._y = ( rand() % 32768 ) * (1.0f / 32768.0f);
	offset._z = ( rand() % 32768 ) * (1.0f / 32768.0f);
	static f32		vibrationPower = 1.0f;
	Vector3 drawPos = _drawPosition + offset * vibrationPower;
	_wallModel->setOffset(drawPos);
}

//-----------------------------------------------------------------------------
//! ダメージ
//-----------------------------------------------------------------------------
void CastleWall::Damage(u32	damage)
{
	_myStatus->damage(damage);
	_isDamage = true;
}



//=============================================================================
//	城クラスの実装
//=============================================================================

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
Castle::Castle(AssetModelX::Frame* castle)
: _isCrash (false)
, _myStatus(nullptr)
{
	// 城モデル初期化
	_castleModel    = castle;
	
	// 当たり判定用AABB
	f32 minX = _castleModel->getMinX();
	f32 maxX = _castleModel->getMaxX();
	f32 minY = _castleModel->getMinY();
	f32 maxY = _castleModel->getMaxY();
	f32 minZ = _castleModel->getMinZ();
	f32 maxZ = _castleModel->getMaxZ();

	Vector3 maxVec(maxX, maxY, maxZ);
	Vector3 minVec(minX, minY, minZ);
	_colAABB.setEmpty();
	_colAABB.expand(maxVec);
	_colAABB.expand(minVec);

	SAFE_NEW(_myStatus);
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
Castle::~Castle()
{
	SAFE_DELETE(_myStatus);
}

//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool Castle::Initialize()
{
	_drawPosition = Vector3(14.373f, 0.0f, 12.589f);
	_castleModel->setOffset(_drawPosition);
	// 描画フラグ戻す
	_castleModel->setRender(true);
	_myStatus->setStatus(3000, 3000);

	_isCrash  = false;
	_isDamage = false;
	
	return true;
}
//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void Castle::Update()
{
	// 体力がなくなったら
	if( !_myStatus->_isActive )
	{
		// 崩れる
		Crash();
	}

	if(_isDamage){
		
		Controller*	currentController = GmControlMan()->getController(1);
		CameraBase*	currentCamera	  = GmCameraMan()->getCurrentCamera();
		static s32 count;
		count++;
		if( count >= 10 ){
			count = 0;
			_isDamage = false;
			// 元の位置で描画
			_castleModel->setOffset(_drawPosition);
			currentController->DisableVibration();
//			currentCamera->Disablevibration();
		}else{
			f32 vib = LinearInterpolation(1.0f, 0.0f, (f32)_myStatus->_hp, (f32)_myStatus->_hpMax);
			if( vib > 1.0f ){
				vib = 1.0f;
			}
			currentController->EnableVibration(vib, vib);
//			currentCamera->Enablevibration(vib);
			Vibration();
		}
	}
}

//-----------------------------------------------------------------------------
//! デバッグ描画
//-----------------------------------------------------------------------------
void Castle::debugRender()
{
	drawAABB( _colAABB, Vector4(1.0f, 1.0f, 1.0f, 1.0f) );
}


//-----------------------------------------------------------------------------
//! 壁が崩れる
//-----------------------------------------------------------------------------
void Castle::Crash()
{
	if( _drawPosition._y >= -150.0f ) {

		// 崩れる音再生
		ISEManager()->playMusic(SESoundPlayerManager::SE_CRASH);
		_isDamage = true;
		_drawPosition._y -= 0.5f * Global::deltaTime;
		_castleModel->setOffset(_drawPosition);
		

		static f32 maxX = _colAABB._max._x;
		static f32 minX = _colAABB._min._x;
		static f32 maxZ = _colAABB._max._z;
		static f32 minZ = _colAABB._min._z;

		for( s32 i=0; i<10; ++i) {
			Vector3 pos = Vector3(0,0,0);

			// ランダムで四辺のどこに出すか決定
			s32 random = rand() % 4;
			if( random == 0 ){
				pos._x =  minX;
				pos._y = _drawPosition._y - 200.0f;
				pos._z = _drawPosition._x + (f32)(rand() % (s32)(maxZ * 2) - maxZ);
			}else if( random == 1 ){
				pos._x = maxX;
				pos._y = _drawPosition._y - 200.0f;
				pos._z = _drawPosition._x + (f32)(rand() % (s32)(maxZ * 2) - maxZ);
			}else if( random == 2 ) {
				pos._x = _drawPosition._x + (f32)(rand() % (s32)(maxX * 2) - maxX);
				pos._y = _drawPosition._y - 200.0f;
				pos._z = minZ;
			}else if( random == 3 ) {
				pos._x = _drawPosition._x + (f32)(rand() % (s32)(maxX * 2) - maxX);
				pos._y = _drawPosition._y - 200.0f;
				pos._z = maxZ;
			}
			
			Vector3 mov = Vector3(0.0f, 0.01f, 0.0f);

			mov._x *= (rand() %  50) / 100.0f;
			mov._y  = (rand() % 100) / 100.0f;
			mov._z *= (rand() %  50) / 100.0f;

			f32		rot = ( rand() % 20 ) / 20.0f;
			Radian	angVel = Radian( ( rand() % 10 ) / 100.0f );

			// パーティクル生成
			IParticleMan()->generateParticle(pos,
											 mov,
											 Vector3(1.0f, 1.0f, 0.0f),
											 Radian(rot),
											 100, 0,
											 angVel,
											 ParticleManager::PARTICLE_SMOKE); 
		}

	}else if( !_isCrash ){
		ISEManager()->stopMusic(SESoundPlayerManager::SE_CRASH);
		_castleModel->setRender(false);
		_isCrash = true;
	}
}

//-----------------------------------------------------------------------------
//! ぶれ
//-----------------------------------------------------------------------------
void Castle::Vibration()
{
	static Vector3 offset;
	offset._x = ( rand() % 32768 ) * (1.0f / 32768.0f);
	offset._y = ( rand() % 32768 ) * (1.0f / 32768.0f);
	offset._z = ( rand() % 32768 ) * (1.0f / 32768.0f);
	static f32		vibrationPower = 1.0f;
	Vector3 drawPos = _drawPosition + offset * vibrationPower;
	_castleModel->setOffset(drawPos);

}



//-----------------------------------------------------------------------------
//! ダメージ
//-----------------------------------------------------------------------------
void Castle::Damage(u32	damage)
{
	_myStatus->damage(damage);
	_isDamage = true;
}

//=============================================================================
//	城、城壁あたり判定管理クラスの実装
//=============================================================================

//-----------------------------------------------------------------------------
//! 攻撃衝突判定とダメージ
//! @param	[in]	attackAABB	判定する攻撃AABB
//!	@param	[in]	damage		当たったものに与えるダメージ量
//-----------------------------------------------------------------------------
void CastleCollisionManager::hitAttackAndDamage(AABB& attackAABB, Vector3&offset, u32 damage)
{
	// 実際使う当たり判定
	AABB hitAABB;
	hitAABB._max = attackAABB._max + offset;
	hitAABB._min = attackAABB._min + offset;
	// 城壁が壊れていなければ
	if( !_castleWall->isCrash() )
	{
		// AABB vs AABB 当たり判定
		if( AABBCollision::isHit(_castleWall->getAABB(), hitAABB ) ){
			_castleWall->Damage(damage);
			return;
		}
	}

	// 城がアクティブかどうか
	if( !_castle->isCrash() )
	{
		// AABB vs AABB 当たり判定
		if( AABBCollision::isHit(_castle->getAABB(), hitAABB ) ){
			_castle->Damage(damage);
			return;
		}
	}
}

//-----------------------------------------------------------------------------
//! 外に出れないようにするための関数
//-----------------------------------------------------------------------------
void CastleCollisionManager::hitOutRange(Vector3& position)
{
	if( position._x >= 3600.0f ){
		position._x = 3600.0f;
	}
	if( position._x <= -3600.0f ){
		position._x = -3600.0f;
	}
	if( position._z >= 3600.0f ) {
		position._z = 3600.0f;
	}
	if( position._z <= -3600.0f ) {
		position._z = -3600.0f;
	}
}

//-----------------------------------------------------------------------------
//! 城オブジェクト(城壁など)の近くかどうか(XButton表示用)
//!	@param	[in]	colAABB 当たり判定用AABB
//!	@param	[in]	offset	AABBの座標
//!	@param	[in]	addBouns	追加当たり判定
//!	@return true:近い false:遠い
//-----------------------------------------------------------------------------
bool CastleCollisionManager::isNearCastleObject(AABB& aabb, Vector3& offset, f32 addBounds)
{
	// 実際使う当たり判定
	AABB hitAABB;
	hitAABB._max = aabb._max + offset;
	hitAABB._min = aabb._min + offset;

	// 城壁が壊れていなければ
	if( !_castleWall->isCrash() )
	{
		AABB wallAABB = _castleWall->getAABB();
		// 大きめの当たり判定にする
		wallAABB._max += addBounds;
		wallAABB._min -= addBounds;
		// AABB vs AABB 当たり判定
		if( AABBCollision::isHit(wallAABB, hitAABB) ){
			return true;
		}
	}

	// 城がアクティブかどうか
	if( !_castle->isCrash() )
	{
		AABB castleAABB = _castle->getAABB();
		// 大きめの当たり判定にする
		castleAABB._max += addBounds;
		castleAABB._min -= addBounds;
		// AABB vs AABB 当たり判定
		if( AABBCollision::isHit(castleAABB, hitAABB) ){
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
//! 現在アクティブな当たり判定と当たっているどうか
//-----------------------------------------------------------------------------
Vector3 CastleCollisionManager::getSinkVal(AABB& aabb, bool& hitGround, Vector3& offset)
{

	Vector3 sink;

	// 実際使う当たり判定
	AABB hitAABB;
	hitAABB._max = aabb._max + offset;
	hitAABB._min = aabb._min + offset;
	

	// 城壁が壊れていなければ
	if( !_castleWall->isCrash() )
	{
		// AABB vs AABB 当たり判定
		sink = AABBCollision::getSinkVal(_castleWall->getAABB(), hitAABB);
		if( sink != Vector3(0,0,0) ){
			hitGround = true;
			return sink;
		}
	}

	// 城がアクティブかどうか
	if( !_castle->isCrash() )
	{
		// AABB vs AABB 当たり判定
		sink = AABBCollision::getSinkVal(_castle->getAABB(), hitAABB);
		if( sink != Vector3(0,0,0) ){
			hitGround = true;
			return sink;
		}
	}

	hitGround = false;
	return sink;
}

//-----------------------------------------------------------------------------
//! 城と城壁の当たり判定設定
//-----------------------------------------------------------------------------
void CastleCollisionManager::setCastleCol(Castle* colCastle, CastleWall* colCastleWall)
{
	_castle		= colCastle;
	_castleWall = colCastleWall;
}


//=============================================================================
//	END OF FILE
//=============================================================================