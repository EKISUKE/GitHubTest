//-----------------------------------------------------------------------------
//!
//!	@file	gmType.h
//!	@brief	�^���
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
//	�R���X�g���N�^
//! @param	[in]	pParent		�e�̃N���X�̌^���
//! @param	[in]	className	�N���X��������
//! @param	[in]	description	����������
//-----------------------------------------------------------------------------
Type::Type(Type* pParent, GM_CSTR className, GM_CSTR description)
: _name(className)
, _description(description)
{
	//-------------------------------------------------------------
	// �e�q�p���֌W���c���[�\�z
	//-------------------------------------------------------------
	if( pParent ) {					// �e�N���X�̎w�肪���鎞
		if( pParent->_pChild ) {
			//---- �Q�߈ȍ~�̎q�ɂȂ�ꍇ

			// �q�̌Z��̖����ɓo�^
			for( Type* p=pParent->_pChild; p; p=p->_pNext ) {
				if( p->_pNext == NULL ) {
					p->_pNext = this;
					break;
				}
			}
		}
		else {
			//---- �q�̏���o�^
			pParent->_pChild = this;
		}

		_pParent = pParent;
	}

}



//-----------------------------------------------------------------------------
//	�N���X�������񂩂�N���X��new����
//!	@param	[in]	className	�쐬����N���X��
//!	@param	[in]	pType		�쐬����N���X�̌^���
//!	@return	�쐬�����N���X�̃C���X�^���X nullptr�Ȃ玸�s
//-----------------------------------------------------------------------------
Base* Type::createInstance(GM_CSTR className, const Type* pType)
{
	if( pType == nullptr ) {
		// �O������Ă΂�Ă����ꍇ�͊��̃N���X����c���[�T�����ĕԂ�
		return createInstance(className, &Base::TYPE);
	}

	//-------------------------------------------------------------
	// �������g���Y�����邩�`�F�b�N
	//-------------------------------------------------------------
	if( strcmp(className, pType->getName()) == 0 ) {
		return (Base*)pType->createInstance();
	}

	//-------------------------------------------------------------
	// �ċA�T��
	//-------------------------------------------------------------

	//---- �Z��
	if( pType->getNext() ) {
		Base*	p = createInstance(className, pType->getNext());
		if( p ) return p;
	}

	//---- �q
	if( pType->getChild() ) {
		Base*	p = createInstance(className, pType->getChild());
		if( p ) return p;
	}

	return nullptr;
}



//=============================================================================
//	END OF FILE
//=============================================================================

