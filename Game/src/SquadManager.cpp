//-----------------------------------------------------------------------------
//!
//!	@file	SquadManager.cpp
//!	@brief	�����Ǘ�
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"


//-----------------------------------------------------------------------------
//! ����������T��
//-----------------------------------------------------------------------------
Squad* SquadManager::getSameSquad(Squad* search)
{
	for( list<Squad*>::iterator itr = _pSquadList.begin(); itr != _pSquadList.end(); itr++ )
	{
		Squad* tmpSqd = *itr;
		// ����������
		if( tmpSqd == search )
		{
			return *itr;
		}
	}
	// ������Ȃ�����
	return NULL;
}


//-----------------------------------------------------------------------------
//! �����̓o�^
//-----------------------------------------------------------------------------
void SquadManager::registSquad(Squad*	_registSquad)
{
	// �������̂��Ȃ����
	if( !getSameSquad(_registSquad) )
	{
		// �o�^
		_pSquadList.push_back(_registSquad);
	}
}

//-----------------------------------------------------------------------------
//! �����̉���
//-----------------------------------------------------------------------------
void SquadManager::unregistSquad(Squad*	_unregistSquad)
{
	// �������̂��폜����
	_pSquadList.remove(_unregistSquad);
}
	



//=============================================================================
//	END OF FILE
//=============================================================================