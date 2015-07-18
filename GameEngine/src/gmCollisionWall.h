//-----------------------------------------------------------------------------
//!
//!	@file	gmCollisionWall.h
//!	@brief	�Փ˔���(��)
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//! �Փ˔���N���X(��)
//-----------------------------------------------------------------------------
class CollisionWall
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	CollisionWall();
	//! �f�X�g���N�^
	virtual ~CollisionWall();

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! X�t�@�C������|���S���f�[�^�[�����
	//! @param	[in]	pModel		X�t�@�C��
	bool				load(TaskModelX*	pModel);

	//! �t���[������|���S���f�[�^����
	//! @param	[in]	pFrame		�t���[��
	//!	@param	[in]	parentModel	�t���[���̑����^�X�N���f��
	bool				loadWithFrame(AssetModelX::Frame* pFrame, TaskModelX* parentModel);

	//! �f�o�b�O�`��
	void				DebugRender();
	
	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//!	�I�u�W�F�N�g�̎擾
	SystemCollision::Object*	getObject(s32 index)   {return &_objects[index]; }

	//!	�I�u�W�F�N�g�̌����擾
	s32					getObjectCount() const { return _objects.size(); }

	//! ���̍��W�����ԋ߂��O�p�`�Ƃ̏Փ˓_�̎擾
	//!	@param	[in]	positon		�`�F�b�N����ʒu
	//!	@param	[in]	radius		���a
	//!	@retval			�Փ˓_
	Vector3				getHitPos(const Vector3& position, const f32 radius, bool isNear = true);
	//Vector3				getHitPos(const AABB aabb);

	//@}
private:
	vecCol						_objects;			//!< �ǂݍ��񂾃��f���̃|���S���ۑ��p
	AABBTree					_aabbTree;			//!< �T���p
	f32							_width;				//!< �|���S���̕�
	f32							_height;			//!< �|���S���̍���
	Vector3						_max;				//!< �d�S�̍ő�(XZ)
	Vector3						_min;				//!< �d�S�̍ŏ�(XZ)
	s32							_offsetX;			//!< �}�C�i�X�̃Z���^�[���W�I�t�Z�b�g�l
	s32							_offsetZ;			//!< �}�C�i�X�̃Z���^�[���W�I�t�Z�b�g�l
	s32							_divX;				//!< X�����̕�����
	s32							_divZ;				//!< Z�����̕�����
	s32							_DivCount;			//!< ������
	Triangle*					_LastHitTriangle;	//!< �Ō�ɓ��������O�p�`
	Vector3						_checkIndex;		//!< ���ׂĂ���C���f�b�N�X�ԍ�
};


//=============================================================================
//	END OF FILE
//=============================================================================