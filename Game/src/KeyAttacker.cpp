//-----------------------------------------------------------------------------
//!
//!	@file	KeyAttacker.cpp
//!	@brief	�U���^�C�v�̑���
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
KeyAttacker::KeyAttacker(Man* me, Man* leader)
: AI			(me, leader)
, _mode			(MODE_SEARCH)
, _searchReach	(200.0f)
, _searchRot	(0.0f)
, _attackReach	(45.0f)
{
	_type = AI::TYPE_ATTACKER;
}
//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
KeyAttacker::~KeyAttacker()
{
}

//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void KeyAttacker::Update()
{
	// �g�����W���擾
	Vector3 targetPos = _target->getPosition();
	Vector3 myPos	  = _me->getPosition();
	// �U���Ώۂ֌������x�N�g��
	_dirToTarget    = targetPos - myPos;
	_dirToTarget._y = 0.0f;
	// �U���Ώۂ܂ł̋������擾
	f32 length = _dirToTarget.squareLength();
	

	//----------------------------------------------------------
	// AI����
	//----------------------------------------------------------
	switch(_mode)
	{
		//----------------------------------------------------------
		case MODE_SEARCH:	// �ڕW�{��
		//----------------------------------------------------------
		{
			// �������ɐi�ݑ����鎞��
			static s32 moveCount = 100;

			// �ړ����Ȃ�
			_movVector = Vector3(0.0f, 0.0f, 0.0f);
			// �{���͈͓��ɓ����Ă�����
			if( length <= _searchReach * _searchReach ) {
				// �ڋߏ�ԂɈڍs
				_mode = MODE_APPROACH;
			}else{
				if(moveCount > 0){
					// �p�x�̕����ɓ���
					_movVector._x = sinf(_searchRot);
					_movVector._z = cosf(_searchRot);
					moveCount--;
				}else{
					// �J�E���g�̃��Z�b�g
					moveCount = 100;
					// ���̊p�x�����߂�
					_searchRot = (f32)( rand() % 360 );
				}
			}

		}
			break;
		//----------------------------------------------------------
		case MODE_APPROACH:	// �ڕW�ɐڋ�
		//----------------------------------------------------------
		{
			// �P�ʃx�N�g���ɂ���
			_dirToTarget = _dirToTarget.normalize();
			// �ړ������x�N�g���ݒ�
			_movVector = _dirToTarget;
			
			// ��������
			if( length <= _attackReach * _attackReach ) {			// �U���͈͓��ɓ����Ă���
				// �U����ԂɈڍs
				_mode = MODE_ATTACK;
			}else if( length > _searchReach * _searchReach ) {		// �{���͈͊O��������
				// �{����Ԃɖ߂�
				_mode = MODE_SEARCH;
			}

		}
			break;
		//----------------------------------------------------------
		case MODE_ATTACK:	// �ڕW���U��
		//----------------------------------------------------------
		{
			_movVector = Vector3(0,0,0);

			// �U���t���O�������Ă���
			_nowTime = timeGetTime();
			Status* myStatus = _me->getStatus();
			f32 delay = myStatus->getParam()->_atkDelay;
			// �҂����Ԃ��Ȃ����
			if( _nowTime - _oldTime > 100 *  delay) {
				// �U���{�^���t���O�𗧂Ă�
				setCommandFlag("Attack", true);
				// ���Ԃ̍X�V
				_oldTime = _nowTime;
			}else{
				// �U���{�^���t���O��|��
				setCommandFlag("Attack", false);
			}

			// �U���͈͊O��������
			if( length > _attackReach * _attackReach ) {		
				// �ڋߏ�Ԃɖ߂�
				_mode = MODE_APPROACH;
				// �U���{�^���t���O��|��
				setCommandFlag("Attack", false);
			}
		}
			break;
		
	}
}
//=============================================================================
//	END OF FILE
//=============================================================================