//-----------------------------------------------------------------------------
//!
//!	@file	gmLandScape.cpp
//!	@brief	�Փ˔���(�����h�X�P�[�v)
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

// ������
#define LANDSCAPE_DIVDE_NUM 100

//=============================================================================
// LandScape�̎���
//=============================================================================

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
LandScape::LandScape()
{
	_LastHitTriangle = NULL;
	//SAFE_NEW(_LastHitTriangle);
	// ������
	_DivCount = LANDSCAPE_DIVDE_NUM;
}
//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
LandScape::~LandScape()
{
	// ���ł���Ă��Ȃ�(pTriangle�̃|�C���^�����������_�ł������f���[�g�͂��Ă����Ȃ�����)
	//SAFE_DELETE(_LastHitTriangle);
	/*for( vecCol::iterator itr = _objects.begin(); itr != _objects.end();){
		itr = _objects.erase(itr);
	}*/

	_objects.clear();
}

//-----------------------------------------------------------------------------
//! X�t�@�C������|���S���f�[�^�[�����
//! @param	[in]	pModel		X�t�@�C��
//-----------------------------------------------------------------------------
bool LandScape::load(TaskModelX*	pModel)
{
	
	// �ő�l
	_max._x = -FLT_MAX;
	_min._x =  FLT_MAX;
	_max._z = -FLT_MAX;
	_min._z =  FLT_MAX;

	// �I�u�W�F�N�g�̎擾
	SystemCollision::Object obj = GmSystemCollision()->loadXfile(pModel, _objects, _max, _min, false);

	//_aabbTree.build(obj.getColDataVec());

	//---- �O���b�h��
	// �}�C�i�X�̃Z���^�[���W�̃I�t�Z�b�g�l
	_offsetX = (s32)_min._x * -1;
	_offsetZ = (s32)_min._z * -1;
	// �������傫�߂ɂ��Ĕ͈͊O���Ȃ���
	_max._x += 1;
	_max._z += 1;
	_min._x -= 1;
	_min._z -= 1;
	// �ő�l�ŏ��l����C���f�b�N�X�ԍ������߂�
	_width  = _max._x - _min._x;
	_height = _max._z - _min._z;
	// ceil�͏����_�ȉ��؂�グ
	_divX   = (s32)( ceil( (_width ) / (_DivCount) ) );
	_divZ   = (s32)( ceil( (_height) / (_DivCount) ) );

	// ���������m��
	_objects.resize(_DivCount * _DivCount);

	//---- �I�u�W�F�N�g��ǉ��o�^
	//for( s32 i=0; i<obj.getTriangleCount(); i++ )
	for( s32 i=0; i<obj.getColDataCount(); i++ )
	{
		CollisionData& col = *obj.getColData(i);
		//Triangle* pTriangle = obj.getTriangle(i);
		Triangle* pTriangle = &col._triangle;
		// �d�S���擾
		Vector3	center = pTriangle->getCenter();
		// �O�p�`�̎O�_������ׂ�
		Vector3	position[3] = {
			pTriangle->_position[0],
			pTriangle->_position[1],
			pTriangle->_position[2],
		};
				
		// �ő�ŏ�
		s32 maxIndexX = -INT_MAX;
		s32 maxIndexZ = -INT_MAX;
		s32 minIndexX =  INT_MAX;
		s32 minIndexZ =  INT_MAX;

		// �C���f�b�N�X�̍ŏ��ő������ׂ�
		for( s32 t=0; t<3; ++t )
		{
			s32 x = ( (s32)(position[t]._x) + _offsetX ) / _divX;
			s32 z = ( (s32)(position[t]._z) + _offsetZ ) / _divZ;
			// �ő���傫�����
			if( maxIndexX < x )
			{
				// �ő�l�X�V
				maxIndexX = x;
			}
			// �ő���傫�����
			if( maxIndexZ < z )
			{
				// �ő�l�X�V
				maxIndexZ = z;
			}
			// �ŏ���菬�������
			if( minIndexX > x )
			{
				// �ŏ��l�X�V
				minIndexX = x;
			}
			// �ŏ���菬�������
			if( minIndexZ > z )
			{
				// �ŏ��l�X�V
				minIndexZ = z;
			}
		}
		
		// �܂����ł���O���b�h���ׂĂɓo�^
		for( s32 x=minIndexX; x<=maxIndexX; x++ )
		{
			for( s32 z=minIndexZ; z<=maxIndexZ; z++ )
			{
				//_objects.at( z * _DivCount +x ).addTriangle(*pTriangle);
				_objects.at( z * _DivCount +x ).addColData(col);
			}
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
//! �t���[������|���S���f�[�^����
//! @param	[in]	pFrame		�t���[��
//!	@param	[in]	parentModel	�t���[���̑����^�X�N���f��
//-----------------------------------------------------------------------------
bool LandScape::loadWithFrame(AssetModelX::Frame* pFrame, TaskModelX* parentModel)
{
	
	// �ő�l
	_max._x = -FLT_MAX;
	_min._x =  FLT_MAX;
	_max._z = -FLT_MAX;
	_min._z =  FLT_MAX;

	// �I�u�W�F�N�g�̎擾
	SystemCollision::Object obj = GmSystemCollision()->loadWithFrame(pFrame, _objects, _max, _min, false, parentModel);

	//---- �O���b�h��
	// �}�C�i�X�̃Z���^�[���W�̃I�t�Z�b�g�l
	_offsetX = (s32)_min._x * -1;
	_offsetZ = (s32)_min._z * -1;
	// �������傫�߂ɂ��Ĕ͈͊O���Ȃ���
	_max._x += 1;
	_max._z += 1;
	_min._x -= 1;
	_min._z -= 1;
	// �ő�l�ŏ��l����C���f�b�N�X�ԍ������߂�
	_width  = _max._x - _min._x;
	_height = _max._z - _min._z;
	// ceil�͏����_�ȉ��؂�グ
	_divX   = (s32)( ceil( (_width ) / (_DivCount) ) );
	_divZ   = (s32)( ceil( (_height) / (_DivCount) ) );

	// ���������m��
	_objects.resize(_DivCount * _DivCount);

	//---- �I�u�W�F�N�g��ǉ��o�^
	//for( s32 i=0; i<obj.getTriangleCount(); i++ )
	for( s32 i=0; i<obj.getColDataCount(); i++ )
	{
		CollisionData& col = *obj.getColData(i);
		//Triangle* pTriangle = obj.getTriangle(i);
		Triangle* pTriangle = &col._triangle;
		// �d�S���擾
		Vector3	center = pTriangle->getCenter();
		// �O�p�`�̎O�_������ׂ�
		Vector3	position[3] = {
			pTriangle->_position[0],
			pTriangle->_position[1],
			pTriangle->_position[2],
		};
				
		// �ő�ŏ�
		s32 maxIndexX = -INT_MAX;
		s32 maxIndexZ = -INT_MAX;
		s32 minIndexX =  INT_MAX;
		s32 minIndexZ =  INT_MAX;

		// �C���f�b�N�X�̍ŏ��ő������ׂ�
		for( s32 t=0; t<3; ++t )
		{
			s32 x = ( (s32)(position[t]._x) + _offsetX ) / _divX;
			s32 z = ( (s32)(position[t]._z) + _offsetZ ) / _divZ;
			// �ő���傫�����
			if( maxIndexX < x )
			{
				// �ő�l�X�V
				maxIndexX = x;
			}
			// �ő���傫�����
			if( maxIndexZ < z )
			{
				// �ő�l�X�V
				maxIndexZ = z;
			}
			// �ŏ���菬�������
			if( minIndexX > x )
			{
				// �ŏ��l�X�V
				minIndexX = x;
			}
			// �ŏ���菬�������
			if( minIndexZ > z )
			{
				// �ŏ��l�X�V
				minIndexZ = z;
			}
		}
		
		// �܂����ł���O���b�h���ׂĂɓo�^
		for( s32 x=minIndexX; x<=maxIndexX; x++ )
		{
			for( s32 z=minIndexZ; z<=maxIndexZ; z++ )
			{
				//_objects.at( z * _DivCount +x ).addTriangle(*pTriangle);
				_objects.at( z * _DivCount +x ).addColData(col);
			}
		}
	}

	return true;
}





//-----------------------------------------------------------------------------
//!	�w�肵�����W�ɂ���n�ʂ̍������擾
//!	@param	[in]	positon		�`�F�b�N����ʒu
//!	@param	[in]	heightLimit	����������
//!	@return	�n�ʂ̍���(Y���W)
//-----------------------------------------------------------------------------
f32 LandScape::getHeight(const Vector3& position, f32 heightLimit)
{
	// �ڐG�_
	f32	 height = 0.0f;

	// �Փ˓_�܂ł̋���(���Ŕ�r)
	f32	nearestHitPosLength = FLT_MAX;
	// float��MAX�l
	f32 nearestHeight = FLT_MAX;
	// �v���C���[�����ԋ߂��O�p�`�̏e�g�܂ł̋���(���Ŕ�r)
	//f32	nearestLength = FLT_MAX * FLT_MAX;
	// �Փ˂������ǂ���
	static bool isHit	  = false;

	// �d�͂̈ړ�����
	Vector3 gravityMov = Vector3(0,-1,0);


	// ����������W���C���f�b�N�X�ԍ��ɕύX
	s32 x = ( (s32)(position._x) + _offsetX ) / _divX;
	s32 y = ( (s32)(position._z) + _offsetZ ) / _divZ;

_checkIndex._x = (f32)x;
_checkIndex._y = (f32)y;

	
	// �͈͊O�Ȃ珈�����Ȃ�
	if( x >= _DivCount || y >= _DivCount || x < 0 || y < 0 ) return height;
	

	if( x == 10 && y == 11 )
	{
		s32 a = 0;
	}

	// �O���ԒZ����������
	static f32 prevMinLength = -1;

	// ���̔ԍ��̃I�u�W�F�N�g�𒲂ׂ�
	SystemCollision::Object* pObject = getObject(y * _DivCount + x);
		
	// ������
	isHit = false;

	//for( s32 i=0; i<pObject->getTriangleCount(); i++ ) {
	for( s32 i=0; i<pObject->getColDataCount(); i++ ) {

		CollisionData& col = *pObject->getColData(i);
		

		//Triangle*	pTriangle = pObject->getTriangle(i);
		Triangle*	pTriangle = &col._triangle;
		//// ���̎O�p�`�̏d�S�����߂�
		//Vector3	triCenter = pTriangle->getCenter();
		//// �d�S������W�܂ł̋���
		//f32		lengthFromCenter = abs((position - triCenter).length());
		//// ���̋��������l���傫����Όv�Z���Ȃ�
		//if( lengthFromCenter >= 400.0f ) continue;

		// �����蔻��p���C���̍쐬
		Line	line;
		line._position[0] = position;
		line._position[1] = position + gravityMov;
		Vector3	hitPos;
		f32		h;

		if( collision::isHit( *pTriangle, line, &hitPos ) )
		{
				h = hitPos._y;
		}else{
			continue;
		}
		//-----------------------------------------------------------
		// �Փ˂���
		//-----------------------------------------------------------
		// ����̍�������`�F�b�N
		if( h > position._y + heightLimit ) {
			continue;
		}

		if( h != 0.0f )
		{
			int a = 0;
		}

		// �����_�Ȃ珈���I��
		if( (position - hitPos).squareLength() >= nearestHitPosLength * nearestHitPosLength )
		{
			continue;
		}

		//// ���������Ȃ珈���I��
		//if( h >= nearestHeight ) {
		//	//continue;
		//}
		

		// ��ԋ߂��_�܂ł̋������L�^
		nearestHitPosLength = (position - hitPos).length();
		// ��ԋ߂��Ƃ�����L�^
		//nearestLength = lengthFromCenter;
		// �����΂�߂��������L�^
		nearestHeight = h;
		// �����L�^
		height		  = h;
		// ���������O�p�`�̋L�^
		_LastHitTriangle = pTriangle;
		// �Փˍς�
		isHit = true;
	}
	prevMinLength = nearestHitPosLength;
	// ���̎��_�Ō��������珈���I��
	if( isHit )return height;
	return height;
}


//-----------------------------------------------------------------------------
//! �f�o�b�O�`��
//-----------------------------------------------------------------------------
void LandScape::DebugRender()
{
	static s32 IndexX = 10;
	static s32 IndexY = 11;

	//----------------------------------------------------
	// �c�[��
	//----------------------------------------------------
	enum {
		//b31                                       b0
		// +-------------------------+---+---+---+---+
		// |                         | R | L | D | U |
		// +-------------------------+---+---+---+---+
		KEY_U = (1 << 0),		// 1 : 0001
		KEY_D = (1 << 1),		// 2 : 0010
		KEY_L = (1 << 2),		// 4 : 0100
		KEY_R = (1 << 3),		// 8 : 1000 
	};
	
	u32	key = 0;
	key |= (GetKeyState(VK_UP   ) & 0x80) ? KEY_U : 0;
	key |= (GetKeyState(VK_DOWN ) & 0x80) ? KEY_D : 0;
	key |= (GetKeyState(VK_LEFT ) & 0x80) ? KEY_L : 0;
	key |= (GetKeyState(VK_RIGHT) & 0x80) ? KEY_R : 0;

	static u32	oldKey = 0;
	u32			trigger = (key ^ oldKey) & key;

	oldKey = key;	// ���̂��߂ɕۑ�


	if( trigger & KEY_L ) {
		IndexX = max(IndexX - 1, 0);
	}
	if( trigger & KEY_R ) {
		IndexX = min(IndexX + 1, _DivCount-1);
	}
	if( trigger & KEY_U ) {
		IndexY = max(IndexY - 1, 0);
	}
	if( trigger & KEY_D ) {
		IndexY = min(IndexY + 1, _DivCount-1);
	}


// ���ׂĂ�O���b�h
IndexX = (s32)_checkIndex._x;
IndexY = (s32)_checkIndex._y;

	// �O���b�h�̕`��
	{
		f32 height = 1.0f;
		Line debugLine;
		for( s32 y=0; y<_DivCount; y++ )
		{
			debugLine._position[0] = Vector3( _min._x, height, _min._z + _divZ * (f32)(y) );
			debugLine._position[1] = Vector3( _max._x, height, _min._z + _divZ * (f32)(y) );
			debugLine.DebugRender();
		}
		for( s32 x=0; x<_DivCount; x++ )
		{
			debugLine._position[0] = Vector3( _min._x + _divX * (f32)(x), height, _min._z );
			debugLine._position[1] = Vector3( _min._x + _divX * (f32)(x), height, _max._z );
			debugLine.DebugRender();
		}
	}

	SystemCollision::Object drawObject;
	

	
	

	//drawObject = _objects.at(IndexY * _DivCount + IndexX);
	//// �`��
	//for( s32 i=0; i<drawObject.getColDataCount(); ++i )
	//{
	//	Triangle* drawTri = &drawObject.getColData(i)->_triangle;
	//	drawTri->debugRender(Color(255,255,255));
	//}
	
	
	
	//for( s32 y=0; y<_DivCount; ++y )
	//{
	//	for( s32 x=0; x<_DivCount; ++x )
	//	{
	//		drawObject = _objects.at(y * _DivCount + x);
	//		//for( s32 i=0; i<drawObject.getTriangleCount(); ++i )
	//		for( s32 i=0; i<drawObject.getColDataCount(); ++i )
	//		{
	//			//Triangle* drawTri = drawObject.getTriangle(i);
	//			Triangle* drawTri = &drawObject.getColData(i)->_triangle;
	//			drawTri->debugRender(Color(255,255,255));
	//		}
	//	}
	//}


	_LastHitTriangle->debugRender(Color(255,0,0));
}

//-----------------------------------------------------------------------------
//! �f�o�b�O�`��(2D)
//-----------------------------------------------------------------------------
void LandScape::DebugRender2D()
{
	Vector3 normal = _LastHitTriangle->getNormal();
	Vector3 worldUp = Vector3(0.0f, 1.0f, 0.0f);
	
	normal  = normal.normalize();
	worldUp = worldUp.normalize();
	f32 dot = Vector3::dot(worldUp, normal);
	//f32 degRot = TO_DEGREE(atan2f( normal._y, normal._z));
	//f32 degRot = TO_DEGREE(atan2f(-normal._z, normal._y));
	f32 degRot = TO_DEGREE(acos(dot));

	//degRot -= 90.0f;
	degRot -= 180.0f;

	// �p�x��180�`-180�Ɏ��߂�
	//if( degRot >=   180.0f ) degRot -= 360.0f;
	//if( degRot <=  -180.0f ) degRot += 360.0f;

	
	// ��Βl�Ŕ�r
	degRot = abs(degRot);

	Vector3 pos	   = _LastHitTriangle->_position[0];

	stringstream str;
	str<< "LastHitTriangleRotX: ";
	str<< degRot;
	//font.Draw3DStringW(Vector3(1.0f, 0.0f, 0.0f), 100.0f, 100.0f, 0.0f, 1, false, (wchar_t*)str.str().c_str());
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	render_string(100.0f, 150.0f, str.str());
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}


//=============================================================================
//	END OF FILE
//=============================================================================
