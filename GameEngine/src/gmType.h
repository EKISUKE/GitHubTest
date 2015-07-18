//-----------------------------------------------------------------------------
//!
//!	@file	gmType.h
//!	@brief	�^���
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#pragma once


class Base;	// �v���g�^�C�v�錾

//-----------------------------------------------------------------------------
//! �^���
//-----------------------------------------------------------------------------
class Type
{
public:

	//-------------------------------------------------------------
	//!	@name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	//! @param	[in]	pParent		�e�N���X�̌^���
	//!	@param	[in]	className	�N���X��������
	//!	@param	[in]	description	����������
	Type(Type*	pParent, GM_CSTR className, GM_CSTR dscription);

	//@{
	//-------------------------------------------------------------
	//!	@name �擾
	//-------------------------------------------------------------
	//@{

	//! �N���X�����擾
	const char*			getName	 ()	const { return _name; }
	//! �e���擾
	const Type*			getParent() const { return _pParent; }
	//! �Z����擾
	const Type*			getNext	 () const { return _pNext; }
	//! �q���擾
	const Type*			getChild () const { return _pChild; }

	//! �C�������I���X���쐬
	virtual void*		createInstance() const = 0;
	//! �N���X�������񂩂�N���X��new����
	//!	@param	[in]	className	�쐬����N���X��
	//!	@param	[in]	pType		�쐬����N���X�̌^���
	//!	@return	�쐬�����N���X�̃C���X�^���X nullptr�Ȃ玸�s
	static  Base*		createInstance(GM_CSTR className, const Type* pType=nullptr);

	//@}
private:
	const char*			_name;			//!< �N���X��
	const char*			_description;	//!< ����������

	//---- �e�q�֌W�c���[
	Type*				_pParent;		//!< �e
	Type*				_pNext;			//!< �Z��
	Type*				_pChild;		//!< �q
};


//-----------------------------------------------------------------------------
//!	�C���X�^���X�����t�^���
//-----------------------------------------------------------------------------
template<typename T>
class ClassType : public Type
{
public:
	//-------------------------------------------------------------
	//!	@name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	ClassType(Type* pParent, const char className[], const char description[])
	: Type(pParent, className, description)
	{}

	//! �C���X�^���X�쐬
	virtual void*	createInstance() const
	{
		return new T();
	}
	
	//@}
};

//-----------------------------------------------------------------------------
//!	�w�b�_�p�̒�`�}�N��
//-----------------------------------------------------------------------------
#define GM_TYPE( CLASS_ )									\
	typedef This	Super;	/* �e�N���X�̌^ */				\
	typedef CLASS_	This;	/* ���N���X�̌^ */				\
															\
	/* �^���{�� */											\
	static ClassType<CLASS_>	TYPE;						\
															\
	/* �C���X�^���X�̍쐬 */									\
	This*			createInstance() { return new This; }	\


//-----------------------------------------------------------------------------
//!	cpp�p�̒�`�}�N��
//-----------------------------------------------------------------------------
#define GM_TYPE_IMPLEMENT( CLASS_, DESC_ )					\
	ClassType<CLASS_>		CLASS_::TYPE( &Super::TYPE, #CLASS_, DESC_);

//=============================================================================
//	END OF FILE
//=============================================================================
