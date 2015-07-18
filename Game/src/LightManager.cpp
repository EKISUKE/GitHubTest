//-----------------------------------------------------------------------------
//!
//!	@file	LightManager.cpp
//!	@brief	�����Ǘ��N���X
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

//-----------------------------------------------------------------------------
//! �f�t�H���g�R���X�g���N�^
//-----------------------------------------------------------------------------
LightManager::LightManager()
{
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
LightManager::~LightManager()
{
	vector<LightBase*>::iterator itr;
	for( itr = _pVecLight.begin(); itr != _pVecLight.end(); )
	{
		LightBase* light = *itr;
		SAFE_DELETE(light);
		itr = _pVecLight.erase(itr);
	}
}

//-----------------------------------------------------------------------------
//! ���C�g��T��
//!	@param	[in]	search	�T�����C�g�̃|�C���^
//-----------------------------------------------------------------------------
bool LightManager::sameLight(LightBase* search)
{
	vector<LightBase*>::iterator itr;
	// �����V���h�E�����邩�ǂ���
	for( itr=_pVecLight.begin(); itr!=_pVecLight.end(); )	{
		LightBase* light = *itr;
		// �������ǂ���
		if( memcpy(search, light, sizeof(LightBase)) == 0 ) {
			// �����Ȃ�Ԃ�
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
//! �o�^
//-----------------------------------------------------------------------------
void LightManager::regist(LightBase* light)
{
	if( sameLight(light)) {
		GM_ASSERT(false, "�������C�g���o�^����悤�Ƃ��܂���");
		return;
	}
	// �o�^
	_pVecLight.push_back(light);
}

//-----------------------------------------------------------------------------
//! ����
//-----------------------------------------------------------------------------
void LightManager::unregist(LightBase* light)
{
	vector<LightBase*>::iterator itr;
	LightBase* comp;
	for( itr=_pVecLight.begin(); itr!=_pVecLight.end(); )	{
		comp = *itr;
		// �������ǂ���
		if( memcpy(light, comp, sizeof(LightBase)) == 0 ) {
			// �����Ȃ����
			SAFE_DELETE(comp);
			itr = _pVecLight.erase(itr);
			break;
		}
	}
}


//=============================================================================
//	END OF FILE
//=============================================================================