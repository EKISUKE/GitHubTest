//-----------------------------------------------------------------------------
//!
//!	@file	KeyHealer.cpp
//!	@brief	�񕜃^�C�v�̑���
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include"Library.h"

//=============================================================================
// AI(�񕜃^�C�v)
//=============================================================================
//-----------------------------------------------------------------------------
//! �f�t�H���g�R���X�g���N�^
//-----------------------------------------------------------------------------
KeyHealer::KeyHealer(Man* me, Man* leader)
: AI			(me, leader)
, _mode			(MODE_APPROACH)
, _searchReach	(200.0f)
, _searchRot	(0.0f)
, _debufReach	(100.0f)
, _healReach	(100.0f)
, _healPower	(2)
, _healOldTime	(0)
, _healNowTime	(0)
{
	_type = AI::TYPE_HEALLER;
	_vecKeyCommand.push_back( KeyComPtr(new KeyCommand("Heal")) );
	_vecKeyCommand.push_back( KeyComPtr(new KeyCommand("Debuf")) );
}
//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
KeyHealer::~KeyHealer()
{
}


////-----------------------------------------------------------------------------
////! �ڕW��{��
////-----------------------------------------------------------------------------
//void KeyHealer::Search()
//{
//	// �������ɐi�ݑ����鎞��
//	static s32 moveCount = 100;
//
//	if(moveCount > 0){
//		// �p�x�̕����ɓ���
//		_movVector._x = sinf(_searchRot);
//		_movVector._z = cosf(_searchRot);
//		moveCount--;
//	}else{
//		// �J�E���g�̃��Z�b�g
//		moveCount = 100;
//		// ���̊p�x�����߂�
//		_searchRot = rand() % 360;
//	}
//
//}
//-----------------------------------------------------------------------------
//! �ڕW�ɐڋ�
//-----------------------------------------------------------------------------
void KeyHealer::Approach()
{
	// �P�ʃx�N�g���ɂ���
	//_dirToTarget = _dirToTarget.normalize();
	// �ړ������x�N�g���ݒ�
	_movVector = _dirToTarget.normalize();
}

//-----------------------------------------------------------------------------
//! �f�o�t���@
//-----------------------------------------------------------------------------
void KeyHealer::DebufMagic()
{
	// �ړ����Ȃ�
	_movVector = Vector3(0.0f, 0.0f, 0.0f);
	// �^�[�Q�b�g�̃X�e�[�^�X���Ƃ��Ă���
	// �ꕔ�̃X�e�[�^�X��������
}

//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void KeyHealer::Update()
{

	// �ڕW�ւ̃x�N�g��
	_dirToTarget = _target->getPosition() - _me->getPosition();

	// �ڕW�����܂ł̒����𑪂�
	f32 length = _dirToTarget.squareLength();

	// �q�[�������Ɣ�r
	if( length <= _healReach* _healReach )
	{
		int a = 0;
	}else{
		int a = 0;
	}


	// �����^�C�v�Ȃ�
	if( _me->getType() == Man::TYPE_ALLY )
	{
		// �^�[�Q�b�g�̃^�C�v���G�Ȃ�
		if( _target->getType() == Man::TYPE_ENEMY )
		{
			// �G
			_TargetisEnemy = true;
		}else{
			// �Ⴆ�Ζ���
			_TargetisEnemy = false;
		}
	}else{
		// �G�^�C�v�Ȃ�
		// �^�[�Q�b�g���G�^�C�v����Ȃ����
		if( _target->getType() != Man::TYPE_ENEMY)
		{
			// �G
			_TargetisEnemy = true;
		}else{
			// �G�^�C�v�Ȃ疡��
			_TargetisEnemy = false;
		}
	}

	// �{���͈͊O�Ȃ炩�G�Ȃ�
	if( length > _searchReach * _searchReach && _TargetisEnemy)
	{
		// �^�[�Q�b�g�����[�_�[�ɕύX
		setTarget(_leader);
		// �G�t���O��OFF
		_TargetisEnemy = false;
		// �^�[�Q�b�g�̋����Đݒ�
		length = _dirToTarget.squareLength();
	}
	
	
	//----------------------------------------------------------
	// AI����
	//----------------------------------------------------------
	switch(_mode)
	{
		//----------------------------------------------------------
		case MODE_APPROACH:	// �ڕW�ɐڋ�
		//----------------------------------------------------------
		{
			// �G��������
			if( !_TargetisEnemy )
			{
				// �q�[�������Ɣ�r
				if( length <= _healReach* _healReach )
				{
					// �͈͓��Ȃ�q�[����
					_mode = MODE_HEAL;
				}else{
					// �ڋ�
					Approach();
				}
			}else{
				// �f�o�t�͈͓��ɓ����Ă���
				if( length <= _debufReach * _debufReach ) {			
					// �f�o�t��ԂɈڍs
					_mode = MODE_DEBUF;
				}else{
					// �ڋ�
					Approach();
				}
			}
			
			
		}
			break;
		//----------------------------------------------------------
		case MODE_HEAL:	// �ڕW�Ƀq�[��
		//----------------------------------------------------------
		{
			// �ړ����Ȃ�
			_movVector = Vector3(0.0f, 0.0f, 0.0f);
			// ������r
			if( length > _healReach * _healReach )
			{
				_mode = MODE_APPROACH;
				setCommandFlag("Heal", false);
			}else{
				_healNowTime = timeGetTime();
				// �X�e�[�^�X���Ƃ��Ă���
				Status* status = _me->getStatus();
				// �f�B���C���Ԃ��擾
				f32 delay = status->getParam()->_sklDelay;
				f32 targetHP	= _target->getStatus()->getParam()->_HP;
				f32	targetHPMAX = _target->getStatus()->getParam()->_HPMAX;
				// �^�[�Q�b�g��HP���������Ă��犎�O�ȏ�Ȃ�
				if( targetHP < targetHPMAX && targetHP >= 0.0f) {
					// �f�B���C���Ԃ����傫��������
					if( _healNowTime - _healOldTime > 100 *  delay) {
						// �q�[���R�}���h�t���OON
						setCommandFlag("Heal", true);
						// ���Ԃ̍X�V
						_healOldTime = _healNowTime;
					}else{
						// �q�[���R�}���h�t���OOFF
						setCommandFlag("Heal", false);
					}
				}else{
					// �q�[���R�}���h�t���OOFF
					setCommandFlag("Heal", false);
				}
			}
			
		}
			break;
		//----------------------------------------------------------
		case MODE_DEBUF:	// �ڕW�Ƀf�o�t
		//----------------------------------------------------------
		{
			// �f�o�t�͈͊O��������
			if( length > _debufReach * _debufReach) {		
				// �ڋߏ�Ԃɖ߂�
				_mode = MODE_APPROACH;
				// �U���{�^���t���O��|��
				//setCommandFlag(KeyBase::KEY_COMMAND_ATTACK, false);
				setCommandFlag("Attack", false);
			}else{
				_nowTime = timeGetTime();
				// 1000�~���b(1�b)�o��������s����
				// �X�e�[�^�X���Ƃ��Ă���
				Status* status = _me->getStatus();
				// �f�B���C���Ԃ��擾
				f32 delay = status->getParam()->_sklDelay;
				// �f�B���C���Ԃ����傫��������
				if( _nowTime - _oldTime > 1000 *  delay) {
					setCommandFlag("Attack", true);
					// ���Ԃ̍X�V
					_oldTime = _nowTime;
				}else{
					// �U���{�^���t���O��|��
					//setCommandFlag(KeyBase::KEY_COMMAND_ATTACK, false);
					setCommandFlag("Attack", false);
				}
			}
			
		}
			break;
		
	}
}
//=============================================================================
//	END OF FILE
//=============================================================================