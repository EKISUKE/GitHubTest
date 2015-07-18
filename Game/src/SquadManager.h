//-----------------------------------------------------------------------------
//!
//!	@file	SquadManager.h
//!	@brief	�����Ǘ�
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//! �����Ǘ�
//-----------------------------------------------------------------------------
class SquadManager
{
private:
	//-------------------------------------------------------------
	//!	@name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	SquadManager()
	{
		Init();
	};

	//! �R�s�[�R���X�g���N�^
	SquadManager(SquadManager& src){}

	//! �f�X�g���N�^
	virtual ~SquadManager()
	{
		list<Squad*>::iterator itr;
		for( itr = _pSquadList.begin(); itr != _pSquadList.end(); )
		{
			Squad* squad = *itr;
			SAFE_DELETE(squad);
			itr = _pSquadList.erase(itr);
		}
		
	};

public:
	
	//! ������
	void	Init()
	{
		_pSquadList.clear();
	}

	//@}
	//-------------------------------------------------------------
	//!	@name �擾
	//-------------------------------------------------------------
	//@{

	//! �C���X�^���X�擾
	static SquadManager* getInstance()
	{
		static SquadManager _myInstance;
		return &_myInstance;
	}

	//! �w�肵���ԍ��̕�����Ԃ�
	Squad*	getSquad(s32 Index)
	{
		list<Squad*>::iterator itr = _pSquadList.begin();
		for( s32 i=0; i<Index; i++ )
		{
			itr++;
		}
		return *itr;
	}

	//! ����������T��
	Squad*	getSameSquad(Squad* search);

	//@}
	//-------------------------------------------------------------
	//!	@name �o�^�E����
	//-------------------------------------------------------------
	//@{

	//! �����̓o�^
	void	registSquad(Squad*	_registSquad);

	//! �����̉���
	void	unregistSquad(Squad*	_unregistSquad);
	
	//@}

	

protected:
	 list<Squad*>	_pSquadList;		//!< �������X�g
};

//-----------------------------------------------------------------------------
// �A�N�Z�X�C���^�[�t�F�C�X
//-----------------------------------------------------------------------------
__forceinline
SquadManager*	ISquadManager()
{
	return SquadManager::getInstance();
}


//=============================================================================
//	END OF FILE
//=============================================================================