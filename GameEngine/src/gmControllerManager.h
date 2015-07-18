//-----------------------------------------------------------------------------
//!
//!	@file	gmControllerManager.h
//!	@brief	�R���g���[���[�Ǘ�
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	�R���g���[���[�Ǘ�
//-----------------------------------------------------------------------------
class ControllerManager
{
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{
private:
	//!	�R���X�g���N�^
    ControllerManager();
	//! �R�s�[�R���X�g���N�^
	ControllerManager(ControllerManager& src){}
	//! �f�X�g���N�^
	virtual ~ControllerManager();

	//@}
	//-------------------------------------------------------------
	//! @name �쐬
	//-------------------------------------------------------------
	//@{
public:	
	//! �R���g���[���[�̍쐬
	//!	@retval �쐬�����R���g���[���[�̃|�C���^ null�Ȃ玸�s
	Controller*	createController();

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{
	
	//! �C���X�^���X�擾
	static ControllerManager* getInstance() {
		static ControllerManager _myInstance;
		return &_myInstance;
	}

	//! �w�肵���ԍ��̃R���g���[���[�擾
	Controller*	getController(s32 number) { return _pVecController.at(number-1); }

	//@}
private:
	s32							_currentCreateCount;	//!< ���ݍ쐬���ꂽ�R���g���[����
	vector<Controller*>			_pVecController;		//!< �R���g���[���[�̃��X�g
};


//-----------------------------------------------------------------------------
//	�A�N�Z�X�C���^�[�t�F�C�X
//-----------------------------------------------------------------------------
__forceinline
ControllerManager* GmControlMan()
{
	return ControllerManager::getInstance();
}


//=============================================================================
//	END OF FILE
//=============================================================================