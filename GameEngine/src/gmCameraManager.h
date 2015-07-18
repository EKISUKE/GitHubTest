//-----------------------------------------------------------------------------
//!
//!	@file	gmCameraMaganger.h
//!	@brief	�J�����Ǘ��N���X
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	�J�����Ǘ�
//-----------------------------------------------------------------------------
class CameraManager
{
private:
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	CameraManager();
	//! �R�s�[�R���X�g���N�^
	CameraManager(CameraManager& src){};
	//! �f�X�g���N�^
	virtual ~CameraManager();

	
	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{
public:
	//! �J�������쐬
	//!	@param	[in]	camera �ǉ�����J����
	void		createCamera(CameraBase* camera = new CameraBase());
	//! �w�肵���J�������폜
	//!	@param	[in]	Index �폜����J�����̃C���f�b�N�X�ԍ�
	void		deleteCamera(s32 Index);

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �C���X�^���X�̎擾
	static CameraManager* getInstance()
	{
		static CameraManager _myInstance;
		return &_myInstance;
	}

	//! ���ݎg�p���̃J�������擾
	CameraBase*		getCurrentCamera()
	{
		return _pVecCamera.at(currentIndex);
	}

	//! ���ݎg�p���̃J������ύX
	//! @param	[in]	Index	�ύX��̔ԍ�
	//! @retval	true: �X�V����	false: �X�V���s
	bool		setCurrentCamera(u32 Index)
	{
		if( Index < _pVecCamera.size() ) {
			currentIndex = Index;
			return true;
		}
		return false;
	}

	//@}
protected:
	vector<CameraBase*>		_pVecCamera;
	s32	currentIndex;
};



//-----------------------------------------------------------------------------
// �A�N�Z�X�C���^�[�t�F�C�X
//-----------------------------------------------------------------------------
__forceinline
CameraManager*	GmCameraMan()
{
	return CameraManager::getInstance();
}

//=============================================================================
//	END OF FILE
//=============================================================================