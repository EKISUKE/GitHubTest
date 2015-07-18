//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilterGlare.h
//!	@brief	�O���A�t�B���^�[
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	�O���A�t�B���^�[
//-----------------------------------------------------------------------------
class TaskFilterGlare : public TaskFilter
{
public:
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	TaskFilterGlare();

	//! �f�X�g���N�^
	virtual ~TaskFilterGlare();

	//@}
	//-------------------------------------------------------------
	//!	@name	�^�X�N�֐�
	//! �֐����p�����ăJ�X�^�����܂��B
	//-------------------------------------------------------------
	//@{

	//! ������
	virtual bool		setup();

	//! ���
	virtual void		cleanup();

	//! ����X�V
	virtual void		asyncUpdate(){}

	//! ����X�V
	virtual void		update(){}

	//! �`��
	virtual void		render();

	//@}
private:
};


//=============================================================================
//	END OF FILE
//=============================================================================