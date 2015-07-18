//-----------------------------------------------------------------------------
//!
//!	@file	gmSystemMain.h
//!	@brief	�A�v���P�[�V�����G���g���|�C���g
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	���C���V�X�e��
//-----------------------------------------------------------------------------
class SystemMain
{

public:

	//-------------------------------------------------------------
	//!	@name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{
	
	//!	���C�����[�v
	//!	@param	[in]	windowClassName	�E�B���h�E�̖��O
	//!	@param	[in]	windowTitleName	�E�B���h�E�̃^�C�g����
	int		mainLoop(GM_CSTR windowClassName, GM_CSTR windotTileName);

	//! �n���ꂽ�t���O�ɂ���ăt���X�N���[��<->�E�B���h�E�ɕύX
	//! @param	[in]	isFullScreen �t���X�N���[���ɂ��邩�ǂ���
	void	fullScreen(bool isFullScreen);

	//@}
	//-------------------------------------------------------------
	//!	@name �擾
	//-------------------------------------------------------------
	//@{

	//! �C���X�^���X�擾
	static SystemMain* getInstance() {
		static SystemMain	_myInstance;
		return &_myInstance;
	}
	
	//@}
protected:
	//-------------------------------------------------------------
	//!	@name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	SystemMain(){}
	//! �R�s�[�R���X�g���N�^
	SystemMain(SystemMain& dst){}
	//! �f�X�g���N�^
	virtual ~SystemMain(){}

	//@}
	//-------------------------------------------------------------
	//!	@name �p���J�X�^�}�C�Y����֐�
	//-------------------------------------------------------------
	//@{

	//! �������R�[���o�b�N
	virtual bool	onInitialize() { return true; }

	//! ����R�[���o�b�N
	virtual void	onFinalize() {}

	//@}
	
	////! �E�B���h�E�v���V�[�W��
	//virtual LRESULT		wndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	//
	////!	�E�B���h�E�v���V�[�W��(WindowsOS����̃R�[���o�b�N)
	//static LRESULT CALLBACK	wndProc__(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
};

//-----------------------------------------------------------------------------
// �A�N�Z�X�C���^�[�t�F�C�X
//-----------------------------------------------------------------------------
__forceinline
SystemMain* GmMain()
{
	return SystemMain::getInstance();
}



//=============================================================================
//	END OF FILE
//=============================================================================
