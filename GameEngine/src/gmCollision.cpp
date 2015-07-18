//-----------------------------------------------------------------------------
//!
//!	@file	gmCollision.cpp
//!	@brief	�Փ˔���
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//=============================================================================
// Collision�̎���
//=============================================================================
//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
Collision::Collision(Sphere* hitSphere, char* name)
//: _hitSphere(hitSphere)
{
	/*for( s32 i=0; i<4; ++i )
	{
		_hitObj[i] = NULL;
	}*/
	//strcpy( _name, name );
	// ���ݒ�
	setCollisionState(hitSphere, name);
	
}
//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
Collision::~Collision()
{
	GmCollisionMan()->UnRegist(this);
}

//-----------------------------------------------------------------------------
//! ���ݒ�
//!	@param	[in]	col	�ݒ肷�铖���蔻����
//-----------------------------------------------------------------------------
void Collision::setHitObj(Collision* col)
{
	_hitObj.push_back(col);
}

//-----------------------------------------------------------------------------
//! ��{���ݒ�
//!	@param	[in]	sphere	�����蔻��p�̋�
//!	@param	[in]	name	�����蔻��̖��O
//-----------------------------------------------------------------------------
void Collision::setCollisionState(Sphere* sphere, GM_CSTR name)
{
	// �����蔻��p���̐ݒ�
	setHitSphere(sphere);
	// ���O�ݒ�
	setColName(name);

	GmCollisionMan()->Regist(this);
}
//-----------------------------------------------------------------------------
//! �����蔻��p���̐ݒ�
//!	@param	[in]	sphere	�����蔻��p�̋�
//-----------------------------------------------------------------------------
void Collision::setHitSphere(Sphere* sphere)
{
	_hitSphere = sphere;
}
//-----------------------------------------------------------------------------
//! ���O�̐ݒ�
//!	@param	[in]	name	�����蔻��̖��O
//-----------------------------------------------------------------------------
void Collision::setColName(GM_CSTR name)
{
	strcpy_s( _name, name );
}



//-----------------------------------------------------------------------------
//! �������Ă�����擾
//-----------------------------------------------------------------------------
s32 Collision::getHitCount()
{
	s32	result = 0;
	for( u32 i=0; i<_hitObj.size(); i++ )
	{
		if( _hitObj.at(i) != NULL ){
			result++;
		}
	}
	return result;
}


//-----------------------------------------------------------------------------
//!	����������̕␳�ʒu�擾
//!	@param	[in]	obj		�����蔻��̑���
//-----------------------------------------------------------------------------
Vector3	Collision::getOffsetPos(Collision*	obj)
{
	// ����̂����蔻��̋����擾
	Sphere*	eneSphere = obj->getHitSphere();

	// ���肩�玩���ւ̃x�N�g�����擾����
	Vector3	dir	  = _hitSphere->getPosition() - eneSphere->getPosition();
	// ���ꂼ��̔��a�̍��v�����߂�
	f32	addLength = _hitSphere->getRadius() + eneSphere->getRadius();
	// ��U���K�����āA���ꂼ��̔��a���������ɒ���
	dir = dir.normalize();
	dir *= addLength;
	
	Vector3 enePos = eneSphere->getPosition();
	//enePos._y -= eneSphere->getRadius();

	Vector3 result = enePos + dir;
	return result;
}

//-----------------------------------------------------------------------------
//! �f�o�b�N�`��
//-----------------------------------------------------------------------------
void Collision::DebugRender()
{
	_hitSphere->DebugRender();
}

//=============================================================================
// Collision�̎���
//=============================================================================
// �ÓI�����o�[�̐錾
LandScape*				SystemCollision::_pLandScape;		
CollisionWall*			SystemCollision::_pColWall;


//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
SystemCollision::SystemCollision()
{
	_pLandScape = NULL;
	_pColWall   = NULL;
	SAFE_NEW(_pLandScape);
	SAFE_NEW(_pColWall);
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
SystemCollision::~SystemCollision()
{
	SAFE_DELETE(_pColWall);
	SAFE_DELETE(_pLandScape);
	//deleteObject();
};

//-----------------------------------------------------------------------------
//! �I�u�W�F�N�g�̏���
//-----------------------------------------------------------------------------
void SystemCollision::deleteObject()
{

}

//-----------------------------------------------------------------------------
//! X�t�@�C������|���S���f�[�^�[�����
//! @param	[in]	colModel	X�t�@�C��
//!	@param	[in]	scale		�g�嗦
//-----------------------------------------------------------------------------
bool SystemCollision::loadXfile(TaskModelX* colModel)
{
	//-------------------------------------------------------------
	// ���ꂼ��̃f�[�^��ǂݍ���
	//-------------------------------------------------------------
	_pLandScape->load(colModel);
	_pColWall->load(colModel);

	return true;
}

//-----------------------------------------------------------------------------
//! �t���[�����烉���h�X�P�[�v�����蔻������
//! @param	[in]	colModel	�t���[��
//!	@param	[in]	parentModel	�t���[���̑����^�X�N���f��
//-----------------------------------------------------------------------------
bool SystemCollision::loadLandScaleWithFrame(AssetModelX::Frame* colFrame, TaskModelX* parentModel)
{
	// �t���[������ǂݍ���
	if( !_pLandScape->loadWithFrame(colFrame, parentModel) ) {
		return false;
	}
	if( !_pColWall->loadWithFrame(colFrame, parentModel) ) {
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
//! �����m�̂����蔻��
//!	@param	[in]	cubePos1	��ڂ̋��̍��W
//!	@param	[in]	cubePos2	��ڂ̋��̍��W
//!	@param	[in]	cubeRadius1	��ڂ̋��̔��a
//!	@param	[in]	cubeRadius2	��ڂ̋��̔��a
//!	@retval			true : �������Ă�	false : �������Ă��Ȃ�
//-----------------------------------------------------------------------------
bool SystemCollision::HitSphere(Vector3& spherePos1, Vector3& spherePos2, f32 sphereRadius1, f32 sphereRadius2)
{
	Vector3 dir;
	// ���̊Ԃ̒��������
	dir = spherePos1 - spherePos2;
	f32 length = dir.squareLength();

	// ���a���m�𑫂������̂����߂�
	f32	radius = sphereRadius1 + sphereRadius2;

	// �Ԃ̒��������a�𑫂������̂��Z�����
	if( length <= radius * radius ) {
		// �������Ă���
		return true;
	}
	// �������Ă��Ȃ�
	return false;
}

//-----------------------------------------------------------------------------
//! �~���m�̂����蔻��
//!	@param	[in]	cubePos1	��ڂ̉~�̍��W
//!	@param	[in]	cubePos2	��ڂ̉~�̍��W
//!	@param	[in]	cubeRadius1	��ڂ̉~�̔��a
//!	@param	[in]	cubeRadius2	��ڂ̉~�̔��a
//!	@retval			true : �������Ă�	false : �������Ă��Ȃ�
//-----------------------------------------------------------------------------
bool SystemCollision::HitCircle(Vector3& circlePos1, Vector3& circlePos2, f32 circleRadius1, f32 circleRadius2)
{
	Vector3 dir;
	// �x���W����U�O�ɂ���
	circlePos1._y = 0;
	circlePos2._y = 0;
	// �~�̊Ԃ̒��������
	dir = circlePos1 - circlePos2;
	f32	length = dir.squareLength();

	// ���a���m�𑫂����������߂�
	f32	radius = circleRadius1 + circleRadius2;

	// �Ԃ̒��������a�𑫂������̂��Z�����
	if( length <= radius * radius ) {
		// �������Ă���
		return true;
	}
	// �������Ă��Ȃ�
	return false;
}


//-----------------------------------------------------------------------------
//! �f�o�b�O�`��
//-----------------------------------------------------------------------------
void SystemCollision::DebugRender()
{

	_pLandScape->DebugRender();
	//_pColWall->DebugRender();
}

//-----------------------------------------------------------------------------
//! �f�o�b�O�`��(2D)
//-----------------------------------------------------------------------------
void SystemCollision::DebugRender2D()
{
	_pLandScape->DebugRender2D();
}

//-----------------------------------------------------------------------------
//! �O�p�`�̊p�x�ɂ���ċL�^���邩�ǂ���
//! @retval true : �L�^		false : �L�^���Ȃ�
//-----------------------------------------------------------------------------
bool partColForTriRot(Triangle& baseTri, bool isWall)
{

	// �|���S���̊p�x�����߂�
	Vector3 normal = baseTri.getNormal();
	Vector3 worldUp = Vector3(0.0f, 1.0f, 0.0f);
	
	normal  = normal.normalize();
	worldUp = worldUp.normalize();
	f32 dot = Vector3::dot(worldUp, normal);
	f32 degRot = TO_DEGREE(acos(dot));
	//// �|���S���̊p�x�����߂�
	//Vector3 normal = baseTri.getNormal();
	//f32 degRot = TO_DEGREE(atan2f( normal._y, normal._z));
	//// �p�x��180�`-180�Ɏ��߂�
	//if( degRot >=   180.0f ) degRot -= 360.0f;
	//if( degRot <=  -180.0f ) degRot += 360.0f;

	////// �������90�x�Ȃ̂�-90�x����
	//degRot -= 180.0f;

	//// ��Βl�Ŕ�r
	//degRot = abs(degRot);
	
	// �ǂ̋L�^�Ȃ�
	if( isWall ){
		// 60�x�ȏ�Ȃ�
		if( degRot >= 60.0f) {
			// �L�^
			return true;
		}
	}else{
		// �p�x��60�x�ȉ��Ȃ�
		if( degRot <= 60.0f){
			// �L�^
			return true;
		}
	}

	
	// �L�^���Ȃ�
	return false;
}

//-----------------------------------------------------------------------------
//! �ċA�Ăяo���Ń��b�V���f�[�^���擾
//-----------------------------------------------------------------------------
AssetModelX::Mesh*		getMesh(AssetModelX::Frame* pModelFrame)
{
	AssetModelX::Mesh*		pModelMesh = nullptr;
	// ���̃t���[���ɂ̓��b�V���������ĂȂ�������
	if(pModelFrame->_meshes.size() > 0)
	{
		// �����Ă���΂��̐擪�A�h���X������
		pModelMesh = pModelFrame->_meshes.at(0);
		//// �Z��m�[�h��NULL����Ȃ�������
		//if( pModelFrame->getNext() != nullptr ){
		//	// �Z��m�[�h�̃��b�V���𒲂ׂ�
		//	AssetModelX::Frame* next = pModelFrame->getNext();
		//	pModelMesh  = getMesh(pModelFrame->getNext());
		//}else{
		//	// �q�m�[�h��NULL����Ȃ�������
		//	if( pModelFrame->getChild() != nullptr ) {
		//		// �q�m�[�h���̃��b�V���𒲂ׂ�
		//		AssetModelX::Frame* child = pModelFrame->getChild();
		//		pModelMesh = getMesh(pModelFrame->getChild());
		//	}else{
		//		// �擾�ł���f�[�^���Ȃ��̂ŏI��
		//		return nullptr;
		//	}
		//}
	}
	// ���b�V����Ԃ�
	return pModelMesh;
}

//-----------------------------------------------------------------------------
//! �t���[������f�[�^�ǂݍ���
//! @param	[in]	pModel		X�t�@�C��
//!	@param	[in]	max			�ő���W(XZ)�ۑ��p
//!	@param	[in]	min			�ŏ����W(XZ)�ۑ��p
//! @param  [in]	isWall		�Ǘp���ǂ���
//! @param  [in]	modelMat	���f���s��
//! @param  [in]	joints		���̃t���[���̑����W���C���g
//-----------------------------------------------------------------------------
SystemCollision::Object SystemCollision::loadData(AssetModelX::Frame* pModelFrame, Vector3& max, Vector3& min, bool isWall, Matrix& modelMat, vector<TaskModelX::Joint> joints)
{
	SystemCollision::Object	CollisionObject;
	// �L���ȃ��b�V�����擾����
	AssetModelX::Mesh*		pModelMesh = getMesh(pModelFrame);
	
	TaskModelX::Joint* pJoint = &joints[(s32)( pModelFrame->getJointIndex() )];

	Matrix matWorld = pJoint->_transformMatrix * modelMat;

	// �������b�V�����Ȃ���Ώ������Ȃ�
	if(pModelMesh != nullptr) {
		
		//return CollisionObject;
	
		CollisionData colData;
	

		//-------------------------------------------------------------
		// pModelObject(���f���f�[�^)�� pCollisionObject(�Փ˗p���f��)�ֈڂ��ւ���
		//-------------------------------------------------------------
		for( u32 face=0; face<pModelMesh->_faces.size(); face++ ) {
		
			colData._myFrameNumber = pModelMesh->getJointIndex();

			Triangle	triangle;

			// �|���S���f�[�^�𒊏o
			// (�l�p�`�|���S���͎O�p�`�ɕ�������)
			AssetModelX::Face*	pFace = &pModelMesh->_faces.at(face);

			if( pFace->_vertexCount == 3 ) {	//---- �O�p�`
				
				//---- ���_�ԍ�
				s32	i0 = pFace->_vertexIndex[0];
				s32	i1 = pFace->_vertexIndex[1];
				s32	i2 = pFace->_vertexIndex[2];

				Vector3 iPos[3];
				iPos[0] = pModelMesh->_vertices.at( i0 )._position;
				iPos[1] = pModelMesh->_vertices.at( i1 )._position;
				iPos[2] = pModelMesh->_vertices.at( i2 )._position;
			
				//---- ���W�ϊ�
				iPos[0] = iPos[0].transform(matWorld);
				iPos[1] = iPos[1].transform(matWorld);
				iPos[2] = iPos[2].transform(matWorld);
			

				/*triangle = Triangle(iPos[0],
									iPos[1],
									iPos[2]);*/

				triangle = Triangle(iPos[2],
									iPos[1],
									iPos[0]);

				// �L�^���邩�ǂ���
				if( !partColForTriRot(triangle, isWall) ) continue;

				for( s32 i=0; i<3; i++ )
				{
					// X�̍ő���傫�����
					if( iPos[i]._x > max._x )
					{
						// �X�V
						max._x = iPos[i]._x;
					}
					if( iPos[i]._x < min._x ){
						// X�̍ŏ���菬������΍X�V
						min._x = iPos[i]._x;
					}

					// Z�̍ő���傫�����
					if( iPos[i]._z > max._z )
					{
						// �X�V
						max._z = iPos[i]._z;
					}
					if( iPos[i]._z < min._z ){
						// Z�̍ŏ���菬������΍X�V
						min._z  = iPos[i]._z;
					}
				}

				colData._triangle = triangle;
				//---- �Փ˔���p�f�[�^�ɓo�^
				CollisionObject.addColData(colData);

				
			}
			else if( pFace->_vertexCount == 4 ){					//---- �l�p�`
				
				//-------------------------------------------------------------
				// 1������
				//-------------------------------------------------------------
			
				//---- ���_�ԍ�
				s32	i0 = pFace->_vertexIndex[0];
				s32	i1 = pFace->_vertexIndex[1];
				s32	i2 = pFace->_vertexIndex[2];


				Vector3 iPos[3];
				iPos[0] = pModelMesh->_vertices.at( i0 )._position;
				iPos[1] = pModelMesh->_vertices.at( i1 )._position;
				iPos[2] = pModelMesh->_vertices.at( i2 )._position;
			
				//---- ���W�ϊ�
				iPos[0] = iPos[0].transform(matWorld);
				iPos[1] = iPos[1].transform(matWorld);
				iPos[2] = iPos[2].transform(matWorld);
			

				triangle = Triangle(iPos[2],
									iPos[1],
									iPos[0]);
				
			
				// �L�^���邩�ǂ���
				if( partColForTriRot(triangle, isWall) ){
				
					for( s32 i=0; i<3; i++ )
					{
						// X�̍ő���傫�����
						if( iPos[i]._x > max._x )
						{
							// �X�V
							max._x = iPos[i]._x;
						}
						if( iPos[i]._x < min._x ){
							// X�̍ŏ���菬������΍X�V
							min._x = iPos[i]._x;
						}

						// Z�̍ő���傫�����
						if( iPos[i]._z > max._z )
						{
							// �X�V
							max._z = iPos[i]._z;
						}
						if( iPos[i]._z < min._z ){
							// Z�̍ŏ���菬������΍X�V
							min._z  = iPos[i]._z;
						}
					}

					colData._triangle = triangle;
					//---- �Փ˔���p�f�[�^�ɓo�^
					CollisionObject.addColData(colData);

				}

				//-------------------------------------------------------------
				// 2������
				//-------------------------------------------------------------

				//---- ���_�ԍ�
				i0 = pFace->_vertexIndex[1];
				i1 = pFace->_vertexIndex[2];
				i2 = pFace->_vertexIndex[3];
			
				iPos[0] = pModelMesh->_vertices.at( i0 )._position;
				iPos[1] = pModelMesh->_vertices.at( i1 )._position;
				iPos[2] = pModelMesh->_vertices.at( i2 )._position;

				//---- ���W�ϊ�
				iPos[0] = iPos[0].transform(matWorld);
				iPos[1] = iPos[1].transform(matWorld);
				iPos[2] = iPos[2].transform(matWorld);


				triangle = Triangle(iPos[2],
									iPos[1],
									iPos[0]);

				// �L�^���邩�ǂ���
				if( partColForTriRot(triangle, isWall) ){

					for( s32 i=0; i<3; i++ )
					{
						// X�̍ő���傫�����
						if( iPos[i]._x > max._x )
						{
							// �X�V
							max._x = iPos[i]._x;
						}
						if( iPos[i]._x < min._x ){
							// X�̍ŏ���菬������΍X�V
							min._x = iPos[i]._x;
						}

						// Z�̍ő���傫�����
						if( iPos[i]._z > max._z )
						{
							// �X�V
							max._z = iPos[i]._z;
						}
						if( iPos[i]._z < min._z ){
							// Z�̍ŏ���菬������΍X�V
							min._z  = iPos[i]._z;
						}
					}

					colData._triangle = triangle;
					//---- �Փ˔���p�f�[�^�ɓo�^
					CollisionObject.addColData(colData);
				}
			}
		}

	}

	//---- �܂��Z��A�q�m�[�h�Ƀt���[��������΍ċA�Ăяo������
	if( pModelFrame->getNext() != nullptr )
	{
		SystemCollision::Object obj = loadData(pModelFrame->getNext(), max, min, isWall, modelMat, joints);
		for( s32 i=0; i<obj.getColDataCount(); i++ )
		{
			CollisionData col = *obj.getColData(i);
			CollisionObject.addColData(col);
		}
	}
	if( pModelFrame->getChild() != nullptr ){
		SystemCollision::Object obj = loadData(pModelFrame->getChild(), max, min, isWall, modelMat, joints);
		for( s32 i=0; i<obj.getColDataCount(); i++ )
		{
			CollisionData col = *obj.getColData(i);
			CollisionObject.addColData(col);
		}
	}

	return CollisionObject;

}

//-----------------------------------------------------------------------------
//! �t���[������f�[�^�ǂݍ���(�ċA�Ȃ�)
//! @param	[in]	pModel		X�t�@�C��
//!	@param	[in]	max			�ő���W(XZ)�ۑ��p
//!	@param	[in]	min			�ŏ����W(XZ)�ۑ��p
//! @param  [in]	isWall		�Ǘp���ǂ���
//! @param  [in]	modelMat	���f���s��
//! @param  [in]	joints		���̃t���[���̑����W���C���g
//-----------------------------------------------------------------------------
SystemCollision::Object SystemCollision::loadDataOneFrame(AssetModelX::Frame* pModelFrame, Vector3& max, Vector3& min, bool isWall, Matrix& modelMat, vector<TaskModelX::Joint> joints)
{
	SystemCollision::Object	CollisionObject;
	// �L���ȃ��b�V�����擾����
	AssetModelX::Mesh*		pModelMesh = getMesh(pModelFrame);
	
	TaskModelX::Joint* pJoint = &joints[(s32)( pModelFrame->getJointIndex() )];

	Matrix matWorld = pJoint->_transformMatrix * modelMat;

	// �������b�V�����Ȃ���Ώ������Ȃ�
	if(pModelMesh != nullptr) {
	
		CollisionData colData;
	

		//-------------------------------------------------------------
		// pModelObject(���f���f�[�^)�� pCollisionObject(�Փ˗p���f��)�ֈڂ��ւ���
		//-------------------------------------------------------------
		for( u32 face=0; face<pModelMesh->_faces.size(); face++ ) {
		
			colData._myFrameNumber = pModelMesh->getJointIndex();

			Triangle	triangle;

			// �|���S���f�[�^�𒊏o
			// (�l�p�`�|���S���͎O�p�`�ɕ�������)
			AssetModelX::Face*	pFace = &pModelMesh->_faces.at(face);

			if( pFace->_vertexCount == 3 ) {	//---- �O�p�`
				
				//---- ���_�ԍ�
				s32	i0 = pFace->_vertexIndex[0];
				s32	i1 = pFace->_vertexIndex[1];
				s32	i2 = pFace->_vertexIndex[2];

				Vector3 iPos[3];
				iPos[0] = pModelMesh->_vertices.at( i0 )._position;
				iPos[1] = pModelMesh->_vertices.at( i1 )._position;
				iPos[2] = pModelMesh->_vertices.at( i2 )._position;
			
				//---- ���W�ϊ�
				iPos[0] = iPos[0].transform(matWorld);
				iPos[1] = iPos[1].transform(matWorld);
				iPos[2] = iPos[2].transform(matWorld);
			

				/*triangle = Triangle(iPos[0],
									iPos[1],
									iPos[2]);*/

				triangle = Triangle(iPos[2],
									iPos[1],
									iPos[0]);

				// �L�^���邩�ǂ���
				if( !partColForTriRot(triangle, isWall) ) continue;

				for( s32 i=0; i<3; i++ )
				{
					// X�̍ő���傫�����
					if( iPos[i]._x > max._x )
					{
						// �X�V
						max._x = iPos[i]._x;
					}
					if( iPos[i]._x < min._x ){
						// X�̍ŏ���菬������΍X�V
						min._x = iPos[i]._x;
					}

					// Z�̍ő���傫�����
					if( iPos[i]._z > max._z )
					{
						// �X�V
						max._z = iPos[i]._z;
					}
					if( iPos[i]._z < min._z ){
						// Z�̍ŏ���菬������΍X�V
						min._z  = iPos[i]._z;
					}
				}

				colData._triangle = triangle;
				//---- �Փ˔���p�f�[�^�ɓo�^
				CollisionObject.addColData(colData);

				
			}
			else if( pFace->_vertexCount == 4 ){					//---- �l�p�`
				
				//-------------------------------------------------------------
				// 1������
				//-------------------------------------------------------------
			
				//---- ���_�ԍ�
				s32	i0 = pFace->_vertexIndex[0];
				s32	i1 = pFace->_vertexIndex[1];
				s32	i2 = pFace->_vertexIndex[2];


				Vector3 iPos[3];
				iPos[0] = pModelMesh->_vertices.at( i0 )._position;
				iPos[1] = pModelMesh->_vertices.at( i1 )._position;
				iPos[2] = pModelMesh->_vertices.at( i2 )._position;
			
				//---- ���W�ϊ�
				iPos[0] = iPos[0].transform(matWorld);
				iPos[1] = iPos[1].transform(matWorld);
				iPos[2] = iPos[2].transform(matWorld);
			

				triangle = Triangle(iPos[2],
									iPos[1],
									iPos[0]);
				
			
				// �L�^���邩�ǂ���
				if( partColForTriRot(triangle, isWall) ){
				
					for( s32 i=0; i<3; i++ )
					{
						// X�̍ő���傫�����
						if( iPos[i]._x > max._x )
						{
							// �X�V
							max._x = iPos[i]._x;
						}
						if( iPos[i]._x < min._x ){
							// X�̍ŏ���菬������΍X�V
							min._x = iPos[i]._x;
						}

						// Z�̍ő���傫�����
						if( iPos[i]._z > max._z )
						{
							// �X�V
							max._z = iPos[i]._z;
						}
						if( iPos[i]._z < min._z ){
							// Z�̍ŏ���菬������΍X�V
							min._z  = iPos[i]._z;
						}
					}

					colData._triangle = triangle;
					//---- �Փ˔���p�f�[�^�ɓo�^
					CollisionObject.addColData(colData);

				}

				//-------------------------------------------------------------
				// 2������
				//-------------------------------------------------------------

				//---- ���_�ԍ�
				i0 = pFace->_vertexIndex[1];
				i1 = pFace->_vertexIndex[2];
				i2 = pFace->_vertexIndex[3];
			
				iPos[0] = pModelMesh->_vertices.at( i0 )._position;
				iPos[1] = pModelMesh->_vertices.at( i1 )._position;
				iPos[2] = pModelMesh->_vertices.at( i2 )._position;

				//---- ���W�ϊ�
				iPos[0] = iPos[0].transform(matWorld);
				iPos[1] = iPos[1].transform(matWorld);
				iPos[2] = iPos[2].transform(matWorld);


				triangle = Triangle(iPos[2],
									iPos[1],
									iPos[0]);

				// �L�^���邩�ǂ���
				if( partColForTriRot(triangle, isWall) ){

					for( s32 i=0; i<3; i++ )
					{
						// X�̍ő���傫�����
						if( iPos[i]._x > max._x )
						{
							// �X�V
							max._x = iPos[i]._x;
						}
						if( iPos[i]._x < min._x ){
							// X�̍ŏ���菬������΍X�V
							min._x = iPos[i]._x;
						}

						// Z�̍ő���傫�����
						if( iPos[i]._z > max._z )
						{
							// �X�V
							max._z = iPos[i]._z;
						}
						if( iPos[i]._z < min._z ){
							// Z�̍ŏ���菬������΍X�V
							min._z  = iPos[i]._z;
						}
					}

					colData._triangle = triangle;
					//---- �Փ˔���p�f�[�^�ɓo�^
					CollisionObject.addColData(colData);
				}
			}
		}
	}

	return CollisionObject;

}



//-----------------------------------------------------------------------------
//! X�t�@�C������|���S���f�[�^�[��Ԃ�
//! @param	[in]	pModel		X�t�@�C��
//!	@param	[in]	dst			�ǂݍ��݌��ʕۑ���
//!	@param	[in]	max			�ő���W(XZ)�ۑ��p
//!	@param	[in]	min			�ŏ����W(XZ)�ۑ��p
//! @param  [in]	isWall		�Ǘp���ǂ���
//-----------------------------------------------------------------------------
SystemCollision::Object	SystemCollision::loadXfile(TaskModelX* pModel, vecCol& _dst, Vector3& max, Vector3& min, bool isWall)
{

	SystemCollision::Object	CollisionObject;
	
	// ���f���̃I���W�i���f�[�^�擾
	AssetModelX*			pAssetModel = pModel->getModel();
	// ���b�V���f�[�^�̐擪���擾
	AssetModelX::Frame*		pModelFrame	= pAssetModel->_frames.at(0);
	
	// modelMatrix���쐬
	f32	   scale	= pModel->getScale();
	Matrix worldMat = pModel->getWorldMatrix();
	Matrix rotMat	= pModel->getRotateMatrix();
	Matrix modelMat = Matrix::scale( Vector3(scale, scale, -scale) ) * rotMat * worldMat;

	// �f�[�^�ǂݍ���
	CollisionObject = loadData(pModelFrame, max, min, isWall, modelMat, pModel->_joints);


	/*if( CollisionObject.getTriangleCount() > 10000 )
	{
		int a = 0;
	}*/

	return CollisionObject;
}

//-----------------------------------------------------------------------------
//! X�t�@�C������|���S���f�[�^�[��Ԃ�
//! @param	[in]	pModel		X�t�@�C��
//!	@param	[in]	dst			�ǂݍ��݌��ʕۑ���
//!	@param	[in]	max			�ő���W(XZ)�ۑ��p
//!	@param	[in]	min			�ŏ����W(XZ)�ۑ��p
//! @param  [in]	isWall		�Ǘp���ǂ���
//-----------------------------------------------------------------------------
SystemCollision::Object	SystemCollision::loadWithFrame(AssetModelX::Frame* colFrame, vecCol& _dst, Vector3& max, Vector3& min, bool isWall, TaskModelX* parentModel)
{

	SystemCollision::Object	CollisionObject;
	
	AssetModelX::Frame*		pModelFrame	= colFrame;
	
	// modelMatrix���쐬
	f32	   scale	= parentModel->getScale();
	Matrix worldMat = parentModel->getWorldMatrix();
	Matrix rotMat	= parentModel->getRotateMatrix();
	Matrix modelMat = Matrix::scale( Vector3(scale, scale, -scale) ) * rotMat * worldMat;

	// �f�[�^�ǂݍ���
	CollisionObject = loadDataOneFrame(pModelFrame, max, min, isWall, modelMat, parentModel->_joints);

	

	return CollisionObject;
}


//-----------------------------------------------------------------------------
//!	�w�肵�����W�ƃ��f���̓����蔻��ƕ␳
//!	@param	[in]	positon		�`�F�b�N����ʒu
//!	@param	[in]	heightLimit	����������
//! @param	[in]	myGridNum	�O���b�h�ԍ�
//! @param	[in]	isHitGround �n�ʂɓ����������ǂ���		
//-----------------------------------------------------------------------------
void SystemCollision::checkCollisionModel(Vector3& position, f32 radius, bool& isHitGround)
{
	s32	checkCount = 5;
	
	// �n�ʂƔ�Փ�
	isHitGround = false;

	AABB hitAABB;

	hitAABB._min     = position;
	hitAABB._min._x -= radius;
	hitAABB._min._z -= radius;
	hitAABB._min._y -= radius;
	hitAABB._max     = position;
	hitAABB._max._x += radius;
	hitAABB._max._z += radius;
	hitAABB._max._y += radius;


	// �Փ˔���
	for( s32 i=0; i<checkCount; ++i )
	{
		bool isHit = false;
		// �ǂƂ̓����蔻��
		// �Փ˓_�̎擾
		Vector3 hitPos = _pColWall->getHitPos(position, radius);
		//Vector3 hitPos = Vector3( 0.0f, 0.0f, 0.0f );
		// �����蔻���̃I�t�Z�b�g��̃|�W�V����
		// �Փ˓_����|�W�V�����܂ł̃x�N�g��
		Vector3 dir = position - hitPos;
		// ���������a��菬����������␳����
		//if( dir.length() < radius )
		if( dir.squareLength() < radius * radius )
		{
			// �Փ˂���
			isHit = true;
			// �Փ˓_�ƈړ��x�N�g���̓��ς��}�C�i�X�Ȃ甽�Ε����Ȃ̂Ŗ���
			//if( Vector3::dot(-dir, mov) < 0.0f ) return;
			// ���K��
			dir  = dir.normalize();
			// ���a���ɒ���
			dir *= radius;
			// �Փ˓_�ɑ����ăI�t�Z�b�g������߂�
			position = hitPos + dir;
		}

		f32 height = _pLandScape->getHeight(position, radius * 2.0f);
		if( height != 0.0f )
		{
			if( position._y - radius < height )
			{
				// �Փ˂���
				isHit = true;
				// �n�ʂƏՓ�
				isHitGround = true;
				position._y = height + radius;
			}
		}else{
			
			f32 a = _pLandScape->getHeight(position, radius * 2.0f);
		}

		// ���̎��_�łǂ���̕␳���Ȃ���΂������ĂȂ�
		if( !isHit ) {
			break;
		}
	}

	
}

//=============================================================================
//	END OF FILE
//=============================================================================
