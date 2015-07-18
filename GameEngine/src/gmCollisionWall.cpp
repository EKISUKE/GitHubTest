//-----------------------------------------------------------------------------
//!
//!	@file	gmCollision.cpp
//!	@brief	�Փ˔���
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

// ������
#define WALL_DIVDE_NUM 20

//=============================================================================
// CollisionWall�̎���
//=============================================================================
//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
CollisionWall::CollisionWall()
{
	_LastHitTriangle = NULL;
	//SAFE_NEW(_LastHitTriangle);
	// ������
	_DivCount = WALL_DIVDE_NUM;
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
CollisionWall::~CollisionWall()
{
	// ���ł���Ă��Ȃ�(pTriangle�̃|�C���^�����������_�ł������f���[�g�͂��Ă����Ȃ�����)
	//SAFE_DELETE(_LastHitTriangle);
	for( vecCol::iterator itr = _objects.begin(); itr != _objects.end();){
		itr = _objects.erase(itr);
	}
}

//-----------------------------------------------------------------------------
//! X�t�@�C������|���S���f�[�^�[�����
//! @param	[in]	pModel		X�t�@�C��
//-----------------------------------------------------------------------------
bool CollisionWall::load(TaskModelX*	pModel)
{
	// �ő�l
	_max._x = -FLT_MAX;
	_min._x =  FLT_MAX;
	_max._z = -FLT_MAX;
	_min._z =  FLT_MAX;

	// �I�u�W�F�N�g�̎擾
	//SystemCollision::Object obj = loadXfile(pModel->_frames.at(0), _objects, _max, _min, true, scale);
	SystemCollision::Object obj = GmSystemCollision()->loadXfile(pModel, _objects, _max, _min, true);

//	_aabbTree.build(obj.getColDataVec());

	
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
	for( s32 i=0; i<obj.getColDataCount(); i++ )
	{
		CollisionData* col = obj.getColData(i);
		Triangle* pTriangle = &col->_triangle;
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
				_objects.at( z * _DivCount +x ).addColData(*col);
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
bool CollisionWall::loadWithFrame(AssetModelX::Frame* pFrame, TaskModelX* parentModel)
{
	// �ő�l
	_max._x = -FLT_MAX;
	_min._x =  FLT_MAX;
	_max._z = -FLT_MAX;
	_min._z =  FLT_MAX;

	// �I�u�W�F�N�g�̎擾
	//SystemCollision::Object obj = loadXfile(pModel->_frames.at(0), _objects, _max, _min, true, scale);
	// �I�u�W�F�N�g�̎擾
	SystemCollision::Object obj = GmSystemCollision()->loadWithFrame(pFrame, _objects, _max, _min, true, parentModel);

//	_aabbTree.build(obj.getColDataVec());

	
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
	for( s32 i=0; i<obj.getColDataCount(); i++ )
	{
		CollisionData* col = obj.getColData(i);
		Triangle* pTriangle = &col->_triangle;
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
				_objects.at( z * _DivCount +x ).addColData(*col);
			}
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
//! ���̍��W�����ԋ߂��O�p�`�Ƃ̏Փ˓_�̎擾
//!	@param	[in]	positon		�`�F�b�N����ʒu
//!	@param	[in]	radius		���a
//!	@retval			�Փ˓_
//-----------------------------------------------------------------------------
Vector3 CollisionWall::getHitPos(const Vector3& position, const f32 radius, bool isNear)
//Vector3 CollisionWall::getHitPos(const AABB aabb)
{
	// �ڐG�_
	Vector3	 resultPos(0.0f,0.0f,0.0f);

	/*Vector3 v1 = aabb._max;
	v1._y = 0.0f;
	Vector3 v2 = aabb._min;
	v2._y = 0.0f;*/

	//f32 radius = abs((v1 - v2).length() * 0.5f);

	// �Փ˓_�܂ł̋���(���Ŕ�r)
	f32	nearestHitPosLength = radius * radius;
	//f32	nearestHitPosLength = FLT_MAX;
	// �Փ˂������ǂ���
	static bool isHit	  = false;

	// �I�t�Z�b�g�l
	static s32 Count = 0;
	static Vector3 offsetList[5] =
	{
		Vector3(  0, 0,  0 ),	// ����
		Vector3(  0, 0,  1 ),	// �O
		Vector3(  0, 0, -1 ),	// ��
		Vector3( -1, 0,  0 ),	// ��
		Vector3(  1, 0,  0 ),	// �E
	};


	// ����������W���C���f�b�N�X�ԍ��ɕύX
	s32 x = ( (s32)(position._x) + _offsetX ) / _divX;
	s32 y = ( (s32)(position._z) + _offsetZ ) / _divZ;


_checkIndex._x = (f32)( x );
_checkIndex._y = (f32)( y );

	//// �O��Փ˂��Ȃ��J�E���g���X�V����Ă���
	//if( !isHit )
	//{
	//	// �ő吔�������Ă��珈���I��
	//	if( Count >= 5 ) {
	//		// ���Z�b�g
	//		Count = 0;
	//		return resultPos;
	//	}
	//	// �I�t�Z�b�g�����炷
	//	x += offsetList[Count]._x;
	//	y += offsetList[Count]._z;
	//	// ����
	//	Count++;
	//}

	// �͈͊O�Ȃ珈�����Ȃ�
	if( x >= _DivCount || y >= _DivCount || x < 0 || y < 0 ) return resultPos;
	

	static f32 prevMinLength = -1;

	// ���̔ԍ��̃I�u�W�F�N�g�𒲂ׂ�
	SystemCollision::Object* pObject = getObject(y * _DivCount + x);

	
	// ������
	isHit = false;

	// ���̃I�u�W�F�N�g�̓����蔻����𒲂ׂ�
	//for( s32 t=0; t<pObject->getTriangleCount(); t++ )
	for( s32 t=0; t<pObject->getColDataCount(); t++ )
	{
		//Triangle*	pTriangle = pObject->getTriangle(t);
		Triangle*	pTriangle = &pObject->getColData(t)->_triangle;
		// �O�p�`�̖@���擾
		Vector3 triNormal = pTriangle->getNormal();
		// ���̖@���̋t�x�N�g�����|�W�V��������L�΂�
		// �@���𐳋K��
		triNormal = triNormal.normalize();
		// ���a�ɐL�΂�
		triNormal *= radius;

		// �����蔻��p���C���̍쐬
		Line	line;
		line._position[0] = position;
		line._position[1] = position + ( -triNormal );

				
		Vector3	hitPos;
		f32		h;

		// ���C�����L�тĂȂ������珈�����Ȃ�
		if( line._position[0] == line._position[1] ) continue;

		if( isNear ){
			// �O�p�`�Ɠ������Ă��獂���L�^
			collision::checkHitPos( *pTriangle, line, &hitPos );
			h = hitPos._y;
		}else{
			// �O�p�`�Ɠ������Ă��獂���L�^
			if( collision::isHit( *pTriangle, line, &hitPos ) )
			{
				h = hitPos._y;
			}else{
				continue;
			}
		}

		//-----------------------------------------------------------
		// �Փ˂���
		//-----------------------------------------------------------
		
		// ����̍�������`�F�b�N
		if( h > position._y + radius * 2.0f ) {
			continue;
		}

		f32 checkLength = (position - hitPos).squareLength();
	
		if( prevMinLength == checkLength )
		{
			// �O�p�`�Ɠ������Ă��獂���L�^
			collision::checkHitPos( *pTriangle, line, &hitPos );
		}


		// �����_�Ȃ珈���I��
		if(  checkLength >= nearestHitPosLength  )
		{
			continue;
		}

		// �ڐG�_�L�^
		resultPos	  = hitPos;
		// ��ԋ߂��_�܂ł̋������L�^
		nearestHitPosLength = checkLength;
		// ���������O�p�`�̕ۑ�
		_LastHitTriangle = pTriangle;
		// �Փˍς�
		isHit		  = true;
	}
	prevMinLength = nearestHitPosLength;
	// ���̎��_�Ō��������珈���I��
	if( isHit )return resultPos;
	//// �ő�܂ł����ĂȂ�������
	////if( Count < 4 )
	//{
	//	resultPos = getHitPos(position, radius);
	//}
	// �J�E���g������
	Count = 0;
	return resultPos;
}

//-----------------------------------------------------------------------------
//! �f�o�b�O�`��
//-----------------------------------------------------------------------------
void CollisionWall::DebugRender()
{
	static s32 IndexX = 0;
	static s32 IndexY = 0;

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

	IndexX = (s32)( _checkIndex._x );
	IndexY = (s32)( _checkIndex._y );

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

	//// �͈͊O�Ȃ珈�����Ȃ�
	//if( IndexX >= _DivCount || IndexY >= _DivCount || IndexX < 0 || IndexY < 0 ) return;

	//drawObject = _objects.at(IndexY * _DivCount + IndexX);

	//// �`��
	////for( s32 i=0; i<drawObject.getTriangleCount(); i++ )
	//for( s32 i=0; i<drawObject.getColDataCount(); i++ )
	//{
	//	//Triangle* drawTri = drawObject.getTriangle(i);
	//	Triangle* drawTri = &drawObject.getColData(i)->_triangle;
	//	drawTri->debugRender(Color(255,255,255));
	//}

	for( s32 y=0; y<_DivCount; ++y )
	{
		for( s32 x=0; x<_DivCount; ++x )
		{
			drawObject = _objects.at(y * _DivCount + x);
			//for( s32 i=0; i<drawObject.getTriangleCount(); ++i )
			for( s32 i=0; i<drawObject.getColDataCount(); ++i )
			{
				//Triangle* drawTri = drawObject.getTriangle(i);
				Triangle* drawTri = &drawObject.getColData(i)->_triangle;
				drawTri->debugRender(Color(255,255,255));
			}
		}
	}
}

//=============================================================================
//	END OF FILE
//=============================================================================
