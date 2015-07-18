//-----------------------------------------------------------------------------
//!
//!	@file	Key.h
//!	@brief	�I�u�W�F�N�g����
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	�I�u�W�F�N�g����C���^�[�t�F�[�X
//-----------------------------------------------------------------------------
class	IKey
{
public:
	virtual		~IKey() {}
};

//-----------------------------------------------------------------------------
//! �L�[�R�}���h�\����
//-----------------------------------------------------------------------------
struct KeyCommand
{
public:
	//! �R���X�g���N�^
	KeyCommand(char* name)
	: _name(name)
	, _isActive(false)
	{
	}

	//! �����\���̂��ǂ����Ԃ�
	//!	@param [in] name
	//! @retval	true : ��v	false : �s��v
	bool	isSame(char* name)
	{
		if( strcmp(_name, name) == 0 )
		{
			return true;
		}
		return false;
	}

	char*	_name;		//!< ���O
	bool	_isActive;	//!< �A�N�e�B�u�t���O
};

// �v���g�^�C�v�錾
class Man;

//-----------------------------------------------------------------------------
//!	�I�u�W�F�N�g������
//-----------------------------------------------------------------------------
class KeyBase : public IKey
{
public:

	//---- �^�C�v
	enum	KEY_TYPE {
		KEY_TYPE_AI,		//!< NPC
		KEY_TYPE_INPUT,		//!< �R���g���[������

		KEY_TYPE_NUM
	};


	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	KeyBase(Man* me, Man* leader)
	: _me(me)
	, _leader(leader)
	{
		// �R�}���h�̏�����
		/*for( s32 i=0; i<KEY_COMMAND_NUM; ++i )
		{
			_commandState[i] = false;
		}*/
		
		_vecKeyCommand.push_back( KeyComPtr(new KeyCommand("Attack")) );
		_vecKeyCommand.push_back( KeyComPtr(new KeyCommand("Jump")) );
	};
	//! �f�X�g���N�^
	virtual			~KeyBase() 
	{
		VecKeyComPtr::iterator itr;
		for( itr = _vecKeyCommand.begin(); itr != _vecKeyCommand.end();)
		{
			(*itr).reset();
			itr = _vecKeyCommand.erase(itr);
		}
	}
	
	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{
	
	virtual void	Update() = 0;

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//---- �擾�֘A
	//! �^�C�v�擾
	KEY_TYPE		getKeyType()		const {return _keyType;} 
	//! �ړ������擾
	Vector3			getVector()			const {return _movVector;}
	//! �X�e�B�b�N�̃x�N�g��
	Vector3			getStickVector()	const {return _stickVector.normalize();}
	//! �R�}���h�������ꂽ���`�F�b�N
	//!	@param [in]		key		�R�}���h
	//bool			getPushState(KEY_COMMAND	key) const { return _commandState[key]; }
	bool			getPushState(char*	key)  
	{ 
		VecKeyComPtr::iterator itr;
		// �������O�̍\���̂�T��
		for( itr = _vecKeyCommand.begin(); itr != _vecKeyCommand.end(); ++itr )
		{
			// �������O�Ȃ�
			KeyComPtr keycommand = *itr;
			// �������O�Ȃ�
			if( keycommand->isSame(key) )
			{
				// �A�N�e�B�u�t���O��Ԃ�
				return keycommand->_isActive;
			}
		}
		return false;
	}

	//---- �Z�b�g�֘A

	//! �ڕW�̏��Z�b�g
	virtual void	setTarget(Man*	target){ _target = target; }
	
	bool			setCommandFlag(char* command, bool flag)
	{
		VecKeyComPtr::iterator itr;
		// �������O�̍\���̂�T��
		for( itr = _vecKeyCommand.begin(); itr != _vecKeyCommand.end(); ++itr )
		{
			// �������O�Ȃ�
			KeyComPtr keycommand = *itr;
			// �������O�Ȃ�
			if( keycommand->isSame(command) )
			{
				// �A�N�e�B�u�t���O��Ԃ�
				keycommand->_isActive = flag;
				return true;
			}
		}
		// �ݒ莸�s
		return false;
	}
	
	//@}
protected:
	Vector3		_movVector;							//!< �ړ��x�N�g��
	Vector3		_stickVector;						//!< �R���g���[���[�̃x�N�g��
	Man*		_leader;							//!< ���[�_�[
	Man*		_target;							//!< �^�[�Q�b�g
	Man*		_me;								//!< ����

	KEY_TYPE	_keyType;							//!< �^�C�v

	typedef shared_ptr<KeyCommand> KeyComPtr;		//!< �L�[�R�}���h�̃X�}�[�g�|�C���^
	typedef vector<KeyComPtr>	   VecKeyComPtr;	//!< �X�}�[�g�|�C���^�z��
	VecKeyComPtr				   _vecKeyCommand;	//!< �R�}���h���
};

//-----------------------------------------------------------------------------
//!	AI������
//-----------------------------------------------------------------------------
class	AI	: public KeyBase
{
public:
	//! NPC�̎��
	enum TYPE
	{
		TYPE_ATTACKER,	//!< �U���^�C�v
		TYPE_DEFFENDER,	//!< �h��^�C�v
		TYPE_HEALLER,	//!< �񕜃^�C�v
		TYPE_FEINTER,	//!< �z���^�C�v
		TYPE_MAX		//!< �^�C�v�̍ő�l
	};
public:
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	AI(Man* me, Man* leader);

	//! �f�X�g���N�^
	virtual ~AI(){}

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �X�V
	virtual void	Update();

	//@}

	////! �ڕW���Z�b�g
	//virtual void	setToTargetPos(Vector3	myPos, Vector3 targetPos)
	//{
	//	setMyPos(myPos);
	//	setTargetPos(targetPos);
	//	_dirToTarget = targetPos - myPos;
	//}
protected:
	Vector3		_dirToTarget;
	TYPE		_type;

	//---- �x���p
	u32				_oldTime;			//!< �O��̎���
	u32				_nowTime;			//!< ���݂̎���
};

//=============================================================================
//	END OF FILE
//=============================================================================