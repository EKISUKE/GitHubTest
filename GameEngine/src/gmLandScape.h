//-----------------------------------------------------------------------------
//!
//!	@file	gmLandScape.h
//!	@brief	�Փ˔���(�����h�X�P�[�v)
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
//! �Փ˔���N���X(�����h�X�P�[�v)
//-----------------------------------------------------------------------------
class LandScape
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	LandScape();
	//! �f�X�g���N�^
	virtual ~LandScape();

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
	//! �f�o�b�O�`��(2D)
	void				DebugRender2D();

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//!	�I�u�W�F�N�g�̎擾
	SystemCollision::Object*	getObject(s32 index)   {return &_objects[index]; }

	//!	�I�u�W�F�N�g�̌����擾
	s32							getObjectCount() const { return _objects.size(); }


	//!	�w�肵�����W�ɂ���n�ʂ̍������擾
	//!	@param	[in]	positon		�`�F�b�N����ʒu
	//!	@param	[in]	heightLimit	����������
	//!	@return	�n�ʂ̍���(Y���W)
	f32							getHeight(const Vector3& position, f32 heightLimit);

	//@}
private:
	vecCol						_objects;			//!< �ǂݍ��񂾃��f���̃|���S���ۑ��p
	AABBTree					_aabbTree;
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