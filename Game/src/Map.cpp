//-----------------------------------------------------------------------------
//!
//!	@file	Map.cpp
//!	@brief	�}�b�v����
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"


//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
Map::Map()
: _pAssetMapModel(nullptr)
, _pTaskMapModel (nullptr)
{
	// ���f��������
	SAFE_NEW(_pAssetMapModel);

	// �n�ʃ��f���̓ǂݍ���
	//if(!_pAssetMapModel->load("../Model/Map/TestMap/testMapTransReset.x")){
	if (!_pAssetMapModel->load("../Model/Map/TestMap/testMapAdjustment.x")){
	//if (!_pAssetMapModel->load("../Model/Map/TestMap/testMap.x")){
	//if(_model->load( "../Model/Map/Ground/ground_high.mqo", 1.0f ) == false){
	//if(_model->load( "../Model/Map/Ground/ground_big.mqo", 1.0f ) == false){
	//if(_model->load( "../Model/Map/Ground/Map.mqo", 1.0f ) == false){
		//MessageBox(NULL, L"�}�b�v���f���̓ǂݍ��݂Ɏ��s���܂���", L"�G���[", MB_OK); 
		GM_ASSERT(false, "�}�b�v���f���̓ǂݍ��݂Ɏ��s���܂���");
	}

	SAFE_NEW(_pTaskMapModel);
	_pTaskMapModel->setModel(_pAssetMapModel);
	_pTaskMapModel->setScale(5.0f);
	_pTaskMapModel->init();

	// ���s�[�g���ݒ�
	AssetModelX::Frame* pGroundFrame = _pAssetMapModel->searchFrame("Ground");

	s32 texRepeat = 10;

	if( pGroundFrame ) {
		pGroundFrame->setTexRepeat(texRepeat);
	}

	// �X�P�[���l�ݒ�
	f32 minX = _pAssetMapModel->getMinX();
	f32 maxX = _pAssetMapModel->getMaxX();
	f32 minY = _pAssetMapModel->getMinY();
	f32 maxY = _pAssetMapModel->getMaxY();
	f32 minZ = _pAssetMapModel->getMinZ();
	f32 maxZ = _pAssetMapModel->getMaxZ();


	ICasCaded()->setMinMax(minX, maxX, minY, maxY, minZ, maxZ);

	//---- �����蔻�胂�f���ǂݍ���
	//if( !ISystemCollision()->loadXfile(_pTestMapTask) )
	if( !GmSystemCollision()->loadLandScaleWithFrame(pGroundFrame, _pTaskMapModel) )
	{
		GM_ASSERT( false, "�����h�X�P�[�v�ǂݍ��ݎ��s" );
	}

	// ��̃t���[���擾
	AssetModelX::Frame* castle = _pAssetMapModel->searchFrame("castle");

	// ��N���X������
	_castle	= new Castle(castle);




	string frameName = "Wall";
	AssetModelX::Frame* wall = _pAssetMapModel->searchFrame(frameName.c_str());
	_myWall					 = new CastleWall(wall);


	//// ��ǃN���X������
	//for( u32 i=0; i<WALL_MAX; ++i )
	//{
	//	string frameName = "wall";
	//	AssetModelX::Frame* wall = _assetMapModel->searchFrame(frameName.c_str());

	//	_myWall[i] = new CastleWall(wall);
	//}

	// ������
	Initialize();
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
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
//! ������
//-----------------------------------------------------------------------------
bool Map::Initialize()
{
	// ��̏�����
	_castle->Initialize();

	//// ��ǂ̏�����
	//for( s32 i=0; i<WALL_MAX; ++i )
	//{
	//	_myWall[i]->Initialize();
	//}
	_myWall->Initialize();

	// �����蔻��ݒ�
	ICastleCol()->setCastleCol(_castle, _myWall);

	return true;
}
//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void Map::Update()
{

	_pTaskMapModel->update();

	// ��̍X�V
	_castle->Update();

	//// ��ǂ̍X�V
	//for( s32 i=0; i<WALL_MAX; ++i )
	//{
	//	_myWall[i]->Update();
	//}
	_myWall->Update();
}
//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void Map::Render(bool isShadow)
{
	Matrix offsetMatrix = Matrix::translate(_position);
	_pTaskMapModel->setWorldMatrix(offsetMatrix);
	_pTaskMapModel->render(isShadow);
}

//-----------------------------------------------------------------------------
//! �f�o�b�O�`��
//-----------------------------------------------------------------------------
void Map::debugRender()
{
	//---- �f�o�b�O�`��
	_castle->debugRender();
	_myWall->debugRender();
}

//=============================================================================
//	��ǃN���X�̎���
//=============================================================================
//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
CastleWall::CastleWall(AssetModelX::Frame* wall)
: _myStatus(nullptr)
, _isCrash (false)
{
	_wallModel = wall;

	// �����蔻��pAABB
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
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
CastleWall::~CastleWall()
{
	SAFE_DELETE(_myStatus);
}

//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool CastleWall::Initialize()
{
	
	//// HP�̏�����
	//Status::Param param;
	//_myStatus->setParam;
	//// �Q�[�W�̃T�C�Y��ݒ�
	//_myStatus->setGaugeScale(10.0f, 20.0f);
	//// �Ԃ��ꂽ��ǂ�߂�����

	_drawPosition = Vector3(-3.646f, 47.134f, 9.408f);
	_wallModel->setOffset(_drawPosition);
	// �`��t���O���ǂ�
	_wallModel->setRender(true);
	_myStatus->setStatus(1500, 1500);

	_isCrash  = false;
	_isDamage = false;
	return true;
}

//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void CastleWall::Update()
{
	// �̗͂��Ȃ��Ȃ�����
	if( !_myStatus->_isActive ){
		// �����
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
			// ���̈ʒu�ŕ`��
			_wallModel->setOffset(_drawPosition);
			currentController->DisableVibration();
//			currentCamera->Disablevibration();
		}else{
			f32 vib = LinearInterpolation(1.0f, 0.0f, (f32)_myStatus->_hp, (f32)_myStatus->_hpMax);
			if( vib > 1.0f ){
				vib = 1.0f;
			}
			currentController->EnableVibration(vib, vib);
			// �J�����Ԃꂷ��ƃX�J�C�{�b�N�X���������(�X�J�C�{�b�N�X�̓J�����̈ʒu�����Ƃɂ��Ă邽��)
//			currentCamera->Enablevibration(vib);
			Vibration();
		}
	}
}

//-----------------------------------------------------------------------------
//! �f�o�b�O�`��
//-----------------------------------------------------------------------------
void CastleWall::debugRender()
{
	drawAABB( _colAABB, Vector4(1.0f, 1.0f, 1.0f, 1.0f) );
}

//-----------------------------------------------------------------------------
//! �ǂ������
//-----------------------------------------------------------------------------
void CastleWall::Crash()
{
	if( _drawPosition._y >= -100.0f ) {

		// ����鉹�Đ�
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

			// �p�[�e�B�N������
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
		// �`��t���O���ǂ�
		_wallModel->setRender(false);
		_isCrash = true;
	}
}

//-----------------------------------------------------------------------------
//! �Ԃ�
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
//! �_���[�W
//-----------------------------------------------------------------------------
void CastleWall::Damage(u32	damage)
{
	_myStatus->damage(damage);
	_isDamage = true;
}



//=============================================================================
//	��N���X�̎���
//=============================================================================

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
Castle::Castle(AssetModelX::Frame* castle)
: _isCrash (false)
, _myStatus(nullptr)
{
	// �郂�f��������
	_castleModel    = castle;
	
	// �����蔻��pAABB
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
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
Castle::~Castle()
{
	SAFE_DELETE(_myStatus);
}

//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool Castle::Initialize()
{
	_drawPosition = Vector3(14.373f, 0.0f, 12.589f);
	_castleModel->setOffset(_drawPosition);
	// �`��t���O�߂�
	_castleModel->setRender(true);
	_myStatus->setStatus(3000, 3000);

	_isCrash  = false;
	_isDamage = false;
	
	return true;
}
//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void Castle::Update()
{
	// �̗͂��Ȃ��Ȃ�����
	if( !_myStatus->_isActive )
	{
		// �����
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
			// ���̈ʒu�ŕ`��
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
//! �f�o�b�O�`��
//-----------------------------------------------------------------------------
void Castle::debugRender()
{
	drawAABB( _colAABB, Vector4(1.0f, 1.0f, 1.0f, 1.0f) );
}


//-----------------------------------------------------------------------------
//! �ǂ������
//-----------------------------------------------------------------------------
void Castle::Crash()
{
	if( _drawPosition._y >= -150.0f ) {

		// ����鉹�Đ�
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

			// �����_���Ŏl�ӂ̂ǂ��ɏo��������
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

			// �p�[�e�B�N������
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
//! �Ԃ�
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
//! �_���[�W
//-----------------------------------------------------------------------------
void Castle::Damage(u32	damage)
{
	_myStatus->damage(damage);
	_isDamage = true;
}

//=============================================================================
//	��A��ǂ����蔻��Ǘ��N���X�̎���
//=============================================================================

//-----------------------------------------------------------------------------
//! �U���Փ˔���ƃ_���[�W
//! @param	[in]	attackAABB	���肷��U��AABB
//!	@param	[in]	damage		�����������̂ɗ^����_���[�W��
//-----------------------------------------------------------------------------
void CastleCollisionManager::hitAttackAndDamage(AABB& attackAABB, Vector3&offset, u32 damage)
{
	// ���ێg�������蔻��
	AABB hitAABB;
	hitAABB._max = attackAABB._max + offset;
	hitAABB._min = attackAABB._min + offset;
	// ��ǂ����Ă��Ȃ����
	if( !_castleWall->isCrash() )
	{
		// AABB vs AABB �����蔻��
		if( AABBCollision::isHit(_castleWall->getAABB(), hitAABB ) ){
			_castleWall->Damage(damage);
			return;
		}
	}

	// �邪�A�N�e�B�u���ǂ���
	if( !_castle->isCrash() )
	{
		// AABB vs AABB �����蔻��
		if( AABBCollision::isHit(_castle->getAABB(), hitAABB ) ){
			_castle->Damage(damage);
			return;
		}
	}
}

//-----------------------------------------------------------------------------
//! �O�ɏo��Ȃ��悤�ɂ��邽�߂̊֐�
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
//! ��I�u�W�F�N�g(��ǂȂ�)�̋߂����ǂ���(XButton�\���p)
//!	@param	[in]	colAABB �����蔻��pAABB
//!	@param	[in]	offset	AABB�̍��W
//!	@param	[in]	addBouns	�ǉ������蔻��
//!	@return true:�߂� false:����
//-----------------------------------------------------------------------------
bool CastleCollisionManager::isNearCastleObject(AABB& aabb, Vector3& offset, f32 addBounds)
{
	// ���ێg�������蔻��
	AABB hitAABB;
	hitAABB._max = aabb._max + offset;
	hitAABB._min = aabb._min + offset;

	// ��ǂ����Ă��Ȃ����
	if( !_castleWall->isCrash() )
	{
		AABB wallAABB = _castleWall->getAABB();
		// �傫�߂̓����蔻��ɂ���
		wallAABB._max += addBounds;
		wallAABB._min -= addBounds;
		// AABB vs AABB �����蔻��
		if( AABBCollision::isHit(wallAABB, hitAABB) ){
			return true;
		}
	}

	// �邪�A�N�e�B�u���ǂ���
	if( !_castle->isCrash() )
	{
		AABB castleAABB = _castle->getAABB();
		// �傫�߂̓����蔻��ɂ���
		castleAABB._max += addBounds;
		castleAABB._min -= addBounds;
		// AABB vs AABB �����蔻��
		if( AABBCollision::isHit(castleAABB, hitAABB) ){
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
//! ���݃A�N�e�B�u�ȓ����蔻��Ɠ������Ă���ǂ���
//-----------------------------------------------------------------------------
Vector3 CastleCollisionManager::getSinkVal(AABB& aabb, bool& hitGround, Vector3& offset)
{

	Vector3 sink;

	// ���ێg�������蔻��
	AABB hitAABB;
	hitAABB._max = aabb._max + offset;
	hitAABB._min = aabb._min + offset;
	

	// ��ǂ����Ă��Ȃ����
	if( !_castleWall->isCrash() )
	{
		// AABB vs AABB �����蔻��
		sink = AABBCollision::getSinkVal(_castleWall->getAABB(), hitAABB);
		if( sink != Vector3(0,0,0) ){
			hitGround = true;
			return sink;
		}
	}

	// �邪�A�N�e�B�u���ǂ���
	if( !_castle->isCrash() )
	{
		// AABB vs AABB �����蔻��
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
//! ��Ə�ǂ̓����蔻��ݒ�
//-----------------------------------------------------------------------------
void CastleCollisionManager::setCastleCol(Castle* colCastle, CastleWall* colCastleWall)
{
	_castle		= colCastle;
	_castleWall = colCastleWall;
}


//=============================================================================
//	END OF FILE
//=============================================================================