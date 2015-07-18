//-----------------------------------------------------------------------------
//!
//!	@file	KeyAttacker.h
//!	@brief	�U���^�C�v�̑���
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	AI(�U���^�C�v)
//-----------------------------------------------------------------------------
class KeyAttacker : public AI
{
public:
	//! �s�����
	//@{

	enum MODE
	{
		MODE_SEARCH,	//!< �ڕW�{��
		MODE_APPROACH,	//!< �ڕW�ɐڋ�
		MODE_ATTACK,	//!< �ڕW���U��
		MODE_MAX		//!< ��ԑJ�ڂ̍ő�
	};

	//@}
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	KeyAttacker(Man* me, Man* leader);
	//! �f�X�g���N�^
	virtual ~KeyAttacker();

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �X�V
	virtual void Update();

	//@}
protected:
	f32				_searchReach;		//!< �ڕW�{���͈�
	f32				_searchRot;			//!< �T������
	f32				_attackReach;		//!< �U���\�͈�

	

	MODE			_mode;				//!< �s����ԗp

};


//=============================================================================
//	END OF FILE
//=============================================================================