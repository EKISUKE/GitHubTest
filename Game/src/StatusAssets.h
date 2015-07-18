//-----------------------------------------------------------------------------
//!
//!	@file	StatusAssets.h
//!	@brief	�X�e�[�^�X�f�[�^
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

class Status;

//-----------------------------------------------------------------------------
//!	�X�e�[�^�X�f�[�^
//-----------------------------------------------------------------------------
class StatusAssets
{
public:

	// �X�e�[�^�X�̃^�C�v
	enum STATUS_TYPE
	{
		STATUS_ATK,		// �U���^�C�v
		STATUS_DEF,		// �h��^�C�v
		STATUS_PLAYER,	// �v���C���[
		STATUS_ENEMY,	// �G��{
		STATUS_HEAL,	// �񕜃^�C�v

		STATUS_MAX
	};

	//-------------------------------------------------------------
	//!	@name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{
	
	//! �X�e�[�^�X���擾
	Status*		getStatus(STATUS_TYPE status) { 
		Status*	result = _pStatusList[(s32)status]; 
		return result;
	}

	//! �C���X�^���X�̎擾
	static StatusAssets*	getInstance()
	{
		static StatusAssets _myInstance;
		return &_myInstance;
	}
	//@}

private:
	//-------------------------------------------------------------
	//!	@name ������
	//-------------------------------------------------------------
	//@{
	//!	�f�t�H���g�R���X�g���N�^
	StatusAssets();
		
	//!	�f�X�g���N�^
	virtual ~StatusAssets();

	//! �X�e�[�^�X�̏�����
	void	loadAssetStatus();
	//@}
	

private:
	Status*			_pStatusList[STATUS_MAX];
};


//-----------------------------------------------------------------------------
// �A�N�Z�X�C���^�[�t�F�[�X
//-----------------------------------------------------------------------------
inline
StatusAssets*	IStatusAssets()
{
	return StatusAssets::getInstance();
}
//=============================================================================
//	END OF FILE
//=============================================================================