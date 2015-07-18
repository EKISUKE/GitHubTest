//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskBase.h
//!	@brief	�^�X�N���
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//! �֐��Ăяo����p�N���X
//-----------------------------------------------------------------------------
class Invoker
{
public:
	virtual void invoke() = 0;
};

//-----------------------------------------------------------------------------
//! �֐��Ăяo����p�N���X(�e���v���[�g)
//-----------------------------------------------------------------------------
template <typename T>
class TaskInvoker : public Invoker
{
public:
	//-------------------------------------------------------------
	//!	@name	������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	TaskInvoker() : _pThis(nullptr) {}

	//! �R���X�g���N�^
	TaskInvoker(T* pThis)
	: _pThis(pThis)
	{
	}

	//@}
	//-------------------------------------------------------------
	//!	@name	�^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �Ăяo��
	virtual void invoke()
	{
		_pThis->asyncUpdate();
	}

	//@}

	T* _pThis;	//! �����̃|�C���^
};

//-----------------------------------------------------------------------------
//! �^�X�N���
//!	@breif ������p�����ă^�X�N���쐬
//-----------------------------------------------------------------------------
class TaskBase
{
public:
	//-------------------------------------------------------------
	//!	@name	������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	TaskBase() {}

	//! �f�X�g���N�^
	virtual ~TaskBase() {}

	//@}
	//-------------------------------------------------------------
	//!	@name	�^�X�N�֐�
	//! �֐����p�����ăJ�X�^�����܂��B
	//-------------------------------------------------------------
	//@{

	//! ������
	virtual bool	setup() { return true; }

	//! ���
	virtual void	cleanup() {}

	//! ����X�V
	virtual void	asyncUpdate() {}

	//! ����X�V
	virtual void	update() {}

	//! �`��
	virtual void	render() {}


	//@}
private:
	list<TaskBase>	_taskList;	//!< �^�X�N���X�g
};


//=============================================================================
//	END OF FILE
//=============================================================================