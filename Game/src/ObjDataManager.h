//-----------------------------------------------------------------------------
//!
//!	@file	ObjDataManager.cpp
//!	@brief	�I�u�W�F�N�g�f�[�^�Ǘ�
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//! �v���g�^�C�v�錾
class Player;
class AllyBase;
class EnemyBase;

//-----------------------------------------------------------------------------
//!	����Ǘ�
//-----------------------------------------------------------------------------
class ObjDataManager
{
private:
	//-------------------------------------------------------------
	//!	@name	������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	ObjDataManager();

	//! �R�s�[�R���X�g���N�^
	ObjDataManager(ObjDataManager& src){}

	//! �f�X�g���N�^
	virtual ~ObjDataManager();

public:

	//@}
	//-------------------------------------------------------------
	//!	@name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �C���X�^���X�̎擾
	static ObjDataManager*	getInstance()
	{
		static ObjDataManager _myInstance;
		return &_myInstance;
	}

	//! �v���C���[�̃Z�b�g
	void	setPlayer(Player* player)		{ _pPlayer = player; }
	//! �����̒ǉ�
	void	addAllyData(AllyBase* add)		{ _pAllyData.push_back(add); }
	//! �G�̒ǉ�
	void	addEnemyData(EnemyBase*	add)	{ _pEnemyData.push_back(add); }
	//! �h�q���W�̒ǉ�
	void	addDeffencePos(Vector3&	pos)	{ _deffencePos.push_back(pos); }

	//@}
	
	Player*				_pPlayer;
	vector<AllyBase*>	_pAllyData;
	vector<EnemyBase*>	_pEnemyData;
	vector<Vector3>		_deffencePos;

	Status*				_enemyStatus;
	Status*				_attackerStatus;
	Status*				_healerStatus;
	Status*				_deffenderStatus;
};



//-----------------------------------------------------------------------------
// �A�N�Z�X�C���^�[�t�F�C�X
//-----------------------------------------------------------------------------
__forceinline
ObjDataManager*		IObjDataManager()
{
	return ObjDataManager::getInstance();
}

//=============================================================================
//	END OF FILE
//=============================================================================