//-----------------------------------------------------------------------------
//!
//!	@file	SceneOption.h
//!	@brief	�I�v�V����
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
//! �I�v�V����
//-----------------------------------------------------------------------------
class SceneOption : public SceneBase, public TaskFilter // Filter�͂ڂ����̂��߂ɓ���Ă��邪�ʂ̕��@���l����
{
public:
	GM_TYPE(SceneOption)

	//! �I�����
	enum SELECT_STATE{
		SELECT_FULLSCREEN,
		SELECT_MASTER_VOLUME,	
		SELECT_BGM_VOLUME,		
		SELECT_SE_VOLUME,		
		SELECT_MAX
	};


	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	SceneOption();
	//! �f�X�g���N�^
	virtual				~SceneOption();
	//! ������
	virtual bool		init();

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �X�V
	virtual void		update();
	//! ���
	//virtual void		cleanup();
	//! �X�V
	virtual void		update(Controller& control);
	//! �`��
	virtual void		render();
	//! �t�B���^�[�̉e�����Ȃ��`��
	virtual void		drawNoFilter();

private:
	//! �����݂̃o�b�N�o�b�t�@���R�s�[���Ăڂ����Ĕw�i�Ɏg��
	void				createBackGroundImage();
	//! �L�[����
	void				checkKey(Controller& control);
	//! ���E�L�[�ɂ�鉹�ʒ���
	bool				changeVolumeByKey(f32& volume);
	//! �I�����Ă�����̂ɂ��X�V
	void				updateSelect();
public:

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//---- �擾
	SELECT_STATE		getSelectState() { return _select; }

	//@}
private:
	Texture*			_pBackGround;					//!< �I�v�V�����w�i
	Texture*			_pTitle;						//!< �^�C�g���摜
	Texture*			_font[SELECT_MAX];				//!< ��ʓ������p
	Texture*			_pSettingBar;					//!< �Z�b�e�B���O�̃Q�[�W�o�[
	Texture*			_pSettingCheckBox;				//!< �Z�b�e�B���O�̃`�F�b�N�{�b�N�X
	SELECT_STATE		_select;						//!< �I�𒆂̍���
	bool				_nowJump;						//!< �ʂ̃V�[�����痈�����ǂ���
	bool				_isFullScreen;					//!< �t���X�N���[�����ǂ���
	//---- �L�[����p
	bool				_keyUp;							//!< ��L�[
	bool				_keyDown;						//!< ���L�[
	bool				_keyRight;						//!< �E�L�[
	bool				_keyLeft;						//!< ���L�[
	bool				_keySelect;						//!< ����L�[
	bool				_keyBack;						//!< �߂�L�[

	TaskFilterGaussian	_gaussianFilter;				//!< �ڂ����t�B���^�[
};

//=============================================================================
//	END OF FILE
//=============================================================================