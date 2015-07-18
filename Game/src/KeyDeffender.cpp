//-----------------------------------------------------------------------------
//!
//!	@file	KeyDeffender.cpp
//!	@brief	�h��(��q)�^�C�v�̑���
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include"Library.h"

//=============================================================================
// AI(�U���^�C�v)
//=============================================================================
//-----------------------------------------------------------------------------
//! �f�t�H���g�R���X�g���N�^
//-----------------------------------------------------------------------------
KeyDeffender::KeyDeffender(Man* me, Man* leader)
: AI			(me, leader)
, _mode			(MODE_APPROACH)
, _searchRot	(0.0f)
, _attackReach	(45.0f)
, _deffenceReach(60.0f)
{
	_type = AI::TYPE_DEFFENDER;
}
//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
KeyDeffender::~KeyDeffender()
{
}

//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void KeyDeffender::Update()
{
	// �g�����W���擾
	Vector3 playerPos = _leader->getPosition();
	Vector3 targetPos = _target->getPosition();
	Vector3 myPos	  = _me->getPosition();
	// �h�q�Ώۂ֌������x�N�g��
	_dirToTarget    = playerPos - myPos;
	_dirToTarget._y = 0.0f;
	// �h�q�Ώۂ܂ł̋������擾
	f32 length = _dirToTarget.squareLength();
	

	//----------------------------------------------------------
	// AI����
	//----------------------------------------------------------
	switch(_mode)
	{
		//----------------------------------------------------------
		case MODE_APPROACH:	// �h�q�Ώۂɐڋ�
		//----------------------------------------------------------
		{

			// ��������
			// �h��͈͂Ȃ�
			if( length <= _deffenceReach * _deffenceReach){
				// �h���ԂɈڍs
				_mode = MODE_DEFFENCE;
			}else{
				// �P�ʃx�N�g���ɂ���
				_dirToTarget = _dirToTarget.normalize();
				// �ړ������x�N�g���ݒ�
				_movVector = _dirToTarget;
			}

		}
			break;
		//----------------------------------------------------------
		case MODE_DEFFENCE:	// �h�q�Ώۂ�h��
		//----------------------------------------------------------
		{
			// �h�q�ڕW����G�ւ̃x�N�g��
			Vector3	dirPlayerToTarget = targetPos - playerPos;
			// ���̃x�N�g����h��͈͂̑傫����
			dirPlayerToTarget  = dirPlayerToTarget.normalize();
			dirPlayerToTarget *= _deffenceReach;
			dirPlayerToTarget += playerPos;	// �v���C���[�̍��W�𑫂�
			// ���̍��W�܂ł̕����x�N�g�����擾
			_movVector = dirPlayerToTarget - myPos;
			// �����̍��W����S�[���ւ̍��W
			Vector3	dirMyPosToGoal = _movVector;
			// Y�����ւ̋����͍l�����Ȃ�
			dirMyPosToGoal._y = 0.0f;
			// ���̒�����10.0f�ȉ��Ȃ�
			if( dirMyPosToGoal.length() <= 30.0f ){
				// �ړ��I��
				_movVector = Vector3(0.0f, 0.0f, 0.0f);
			}else{	// �ȏ�Ȃ�
				// �ړ�����
				_movVector = _movVector.normalize();
			}

			// �G�ւ̃x�N�g��
			Vector3 dirMyPosToTarget = targetPos - myPos;

			// �G�������̍U���͈͂ɓ����Ă�����
			if( dirMyPosToTarget.squareLength() <= _attackReach * _attackReach )
			{
				// �U����ԂɈڍs
				_mode = MODE_ATTACK;
			}else if( length > _deffenceReach){
				// �h��͈͊O�Ȃ�A�v���[�`�ɖ߂�
				_mode = MODE_APPROACH;
			}
		}
			break;
		//----------------------------------------------------------
		case MODE_ATTACK:	// �h�q�Ώۂɋ߂Â��G���U��
		//----------------------------------------------------------
		{
			// �ړ����Ȃ�
			_movVector = Vector3(0.0f, 0.0f, 0.0f);

			// �U���t���O�������Ă���
			_nowTime = timeGetTime();
			Status* myStatus = _me->getStatus();
			f32 delay = myStatus->getParam()->_atkDelay;
			// �҂����Ԃ��Ȃ����
			if( _nowTime - _oldTime > 100 *  delay) {
				// �U���{�^���t���O�𗧂Ă�
				//setCommandFlag(KeyBase::KEY_COMMAND_ATTACK, true);
				setCommandFlag("Attack", true);
				// ���Ԃ̍X�V
				_oldTime = _nowTime;
			}else{
				// �U���{�^���t���O��|��
				//setCommandFlag(KeyBase::KEY_COMMAND_ATTACK, false);
				setCommandFlag("Attack", false);
			}

			// �G�ւ̃x�N�g��
			Vector3 dirMyPosToTarget = targetPos - myPos;
			// �U���͈͊O��������
			if( dirMyPosToTarget.squareLength() > _attackReach * _attackReach ) {	
				// �h�q�Ώۂ̋߂��ɖ߂�
				_mode = MODE_APPROACH;
				// �U���t���O�𗎂Ƃ�
				setCommandFlag("Attack", false);
			}
		}
			break;
		
	}
}
//=============================================================================
//	END OF FILE
//=============================================================================