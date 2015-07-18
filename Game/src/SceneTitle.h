//-----------------------------------------------------------------------------
//!
//!	@file	SceneTitle.h
//!	@brief	�^�C�g�����
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
//! �^�C�g��
//-----------------------------------------------------------------------------
class SceneTitle : public SceneBase
{
public:
	GM_TYPE(SceneTitle)

	enum SELECT_STATE{
		SELECT_NEWGAME,	//	���񂩂�J�n�{�^���I��
//		SELECT_CONTINUE,//	����������J�n�{�^���I��(�쐬�\��)
		SELECT_OPTION,	//	�I�v�V�����{�^���I��
		SELECT_EXIT,	//	�I���{�^���I��
		SELECT_MAX
	};


	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	SceneTitle();
	//! �f�X�g���N�^
	virtual				~SceneTitle();
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
	//! �L�[����
	void				checkKey(Controller& control);
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
	Texture*			_backGroundImage;				//!< �^�C�g���摜
	Texture*			_title;							//!< �^�C�g���摜
	Texture*			_font[SELECT_MAX];				//!< ��ʓ������p
	Texture*			_Abutton;						//!< A�{�^�������p
	GLFont*				_titleFont;						//!< �t�H���g�̕`��p
	SELECT_STATE		_select;						//!< �I�𒆂̍���
	bool				_cantSelect[SELECT_MAX];		//!< �I���ł��Ȃ��悤�ɂ���(����ȏꍇ)


	//---- �L�[����p
	bool				_keyUp;							//!< ��L�[
	bool				_keyDown;						//!< ���L�[
	bool				_keySelect;						//!< ����L�[
	bool				_keyBack;						//!< �߂�L�[
};

//=============================================================================
//	END OF FILE
//=============================================================================