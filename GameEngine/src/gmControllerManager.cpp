//-----------------------------------------------------------------------------
//!
//!	@file	gmControllerManager.cpp
//!	@brief	�R���g���[���[�Ǘ�
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
ControllerManager::ControllerManager()
: _currentCreateCount(0)
{
	
}

//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
ControllerManager::~ControllerManager()
{
	vector<Controller*>::iterator itr;
	for( itr = _pVecController.begin(); itr != _pVecController.end(); )
	{
		//Controller* control = *itr;
		SAFE_DELETE(*itr);
		itr = _pVecController.erase(itr);
	}
}


//-----------------------------------------------------------------------------
//! �R���g���[���[�̍쐬
//!	@retval �쐬�����R���g���[���[�̃|�C���^ null�Ȃ玸�s
//-----------------------------------------------------------------------------
Controller*	ControllerManager::createController()
{
	// �R���g���[���[�쐬
	Controller*	controller = new Controller(++_currentCreateCount);
	// �o�^
	_pVecController.push_back(controller);
	return controller;
}



//=============================================================================
//	END OF FILE
//=============================================================================