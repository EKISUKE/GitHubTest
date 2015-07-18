//-----------------------------------------------------------------------------
//!
//!	@file	Man.h
//!	@brief	�Q�[�����L����
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
//!	�Q�[�����L����
//-----------------------------------------------------------------------------
class	Man : public ObjBase
{
public:

	enum TYPE 
	{
		TYPE_PLAYER,
		TYPE_ALLY,
		TYPE_ENEMY, 
	};

	enum KEY_TYPE
	{
		KEY_INPUT,
		KEY_ATTACKER,
		KEY_DEFFENDER,	
		KEY_HEALLER,	
		KEY_FEINTER,
		KEY_LEADER,
		KEY_MAX
	};
	//-------------------------------------------------------------
	//!	@name	������
	//-------------------------------------------------------------
	//@{

private:
	//! �f�t�H���g�R���X�g���N�^
	Man();
public:

	//! �R���X�g���N�^
	Man(KEY_TYPE myType, TYPE manType);

	//! �f�X�g���N�^
	virtual ~Man();

	//!	������
	virtual bool	Initialize();

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �X�V
	virtual void	Update();
	//!	�`��
	virtual void	Render(bool	isShadow);
	//! glut�`��
	virtual void	glutRender() = 0;
	//! �f�o�b�O�`��
	virtual void	debugRender() = 0;

	//! �X�e�[�^�X�̕`��(2D)
	virtual void	StatusRender() = 0;

	//! HP�A�E�g���C���̃e�N�X�`���ǂݍ���
	bool			loadHPOutLineTexture(GM_CSTR fileName);

	//! ���f���̓ǂݍ���
	bool			loadModel(GM_CSTR fileName, f32 scale = 1.0f);
	//! ���f���ǂݍ��݂��ꂽ���ǂ���
	bool			isModelLoaded();

	//! �L�[�̍쐬
	bool			createKey(Man* me, Man* leader);
	
	//! �X�e�[�^�X�̍쐬
	bool			createStatus();

	//@}
	//-------------------------------------------------------------
	//!	@name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//---- �Z�b�g
	void			setHitObj( bool flag )	  { _hitObj    = flag; }
	void			setHitAttack( bool flag ) { _hitAttack = flag; }
	void			setPosition(Vector3& pos) { _position  = pos;  }
	//---- �擾
	bool			isDead				 () const { return _isDead; }
	bool			getHitObj			 () const { return _hitObj;	}
	bool			getHitAttack		 () const { return _hitAttack; }
	bool			getisAttack			 () const { return _isAttack;	}
	TYPE			getType				 () const { return _type;		}
	Status*			getStatus			 () const { return _myStatus;	}
	//! �^�C�v�擾					    
	KEY_TYPE		getKeyType			 () const { return _keyType; }
	//Vector3			getHitPosition	 	  () const { return _hitPosition;	 }
	Vector3			getAttackPosition	 () const { return _attackPosition; }
										 
	//f32				getRadius		     () const { return _hitRadius;		}
	f32				getAttackRadius		 () const { return _attackRadius;	}
										 
	Sphere			getAttackSphere		 () const { return _attackSphere; };
										 
	//! �����蔻��pAABB���擾 		   
	AABB			getColAABB			 () const { return _colAABB; }
	//! �����蔻��pAABB���擾 		   
	AABB			getAttackAABB		 () const { return _attackAABB; }

	//! �����擾
	f32		getLengthtoTarget (Man* target) const {
		Vector3 targetPos = target->getPosition();
		return (targetPos - _position).length();
	}

	//@}
	//-------------------------------------------------------------
	//!	@name	���W�ϊ��֐�
	//-------------------------------------------------------------
	//@{

	//! �����蔻��p�̍��W�ɂ���
	//!	@param	[in]�@position		���W
	//!	@param	[in]�@hitPosition	�����蔻��p�̍��W�ۑ��p
	void	toHitPosition(Vector3 position, Vector3& hitPosition)
	{
		position._y += _hitRadius;
		hitPosition  = position;
	}

	//! ���W�ɂ���
	//!	@param	[in]�@hitPosition	�����蔻��p�̍��W
	//!	@param	[in]�@position		���W�ۑ��p
	void	toPosition(Vector3 hitPosition, Vector3& position)
	{
		hitPosition._y -= _hitRadius;
		position		= hitPosition;
	}

	//@}
protected:
	virtual void		Jump();						//!< �W�����v
	virtual void		Attack();					//!< �U��
	virtual void		Damage(const s32& damage);	//!< �_���[�W
		

	//! ���W�ݒ肵�Ȃ���
	virtual void		ResetPosition();

	//! �_�������T���Đݒ�
	//!	@return true:�G��ݒ芮�� false:�_���G�͂��Ȃ�
	virtual bool		searchAndSetTarget(){return true;}

	//! �_���[�W�v�Z
	virtual s32			calcDamageAndCollision(){return 0;}

	//! �A�j���[�V�����ɂ��ړ��s�`�F�b�N
	virtual bool		checkFreeze(const s32& currentAnimation){return false;}

	

	bool				_hitGround;					//!< �ڒn�t���O
	bool				_hitCastle;					//!< ��A��ǂƂ̏Փ˃t���O
	bool				_hitObj;					//!< �I�u�W�F�N�g�Ƃ̃q�b�g�t���O
	bool				_hitAttack;					//!< �I�u�W�F�N�g�ƂɍU�����ꂽ���ǂ���
	bool				_isAttack;					//!< �U���t���O
	bool				_isJump;					//!< �W�����v�t���O
	bool				_isDamage;					//!< �_���[�W�t���O
	bool				_isDead;					//!< ���S�t���O

	//---- ��{���
	Vector3				_attackPosition;			//!< �U���p
	f32					_speed;						//!< �ړ����x
	//---- �U�����
	f32					_attackRadius;				//!< �U���̔��a
	f32					_attackRadNum;				//!< �U���̔��a�̏����l
	f32					_attackOffsetRange;			//!< �U���̋���
	f32					_waitGenTime;				//!< �p�[�e�B�N���̐����҂�����
	f32					_genCount;					//!< �����҂��J�E���g
	Status*				_myStatus;					//!< �X�e�[�^�X
	AssetModelX*		_pAssetModel;				//!< ���f���f�[�^
	TaskModelX*			_pTaskModel;				//!< ���f���ύX�p

	TYPE				_type;						//!< �����̎��
	KEY_TYPE			_keyType;					//!< �L�[�̎��
	KeyBase*			_key;						//!< �����̑���L�[

	Sphere				_attackSphere;				//!< �U���p�̋�

	AABB				_colAABB;					//!< �����蔻��pAABB
	AABB				_attackAABB;				//!< �U���Փ˔���pAABB

	f32					_height;					//!< ����

	Collision			_myCollision;				//!< �����蔻��p

	s32					_myLandScapleGridNum;		//!< �����h�X�P�[�v�̃O���b�h�ԍ�
	
	Texture*			_hpOutLine;			//!< HP�Q�[�W�̃A�E�g���C��

};


//-----------------------------------------------------------------------------
//!	�L�����Ǘ��N���X
//-----------------------------------------------------------------------------
class ManManager : public Singleton<ManManager>
{
	//-------------------------------------------------------------
	//!	@name ������
	//-------------------------------------------------------------
	//@{

	//!	�f�t�H���g�R���X�g���N�^
	ManManager()
	: _nowIndex(0)
	{
		// �S������
		for( s32 i=0; i<LIST_MAX; ++i )
		{
			_AttackListForPlayer[i] = NULL;
			_AttackListForEnemy[i] = NULL;
		}
	}
	//!	�f�X�g���N�^
	virtual ~ManManager();

	//@}
	//-------------------------------------------------------------
	//!	@name �o�^�E����
	//-------------------------------------------------------------
	//@{

	//! �o�^
	void	Regist(Man*	regist);
	//! ����
	void	UnRegist(Man* regist);

	//@}

	//! �X�V
	void	Update();
private:
	static const s32 LIST_MAX = 50;
	Man*		_AttackListForPlayer[LIST_MAX];	//!< �v���C���[�̂��߂̍U�����X�g
	Man*		_AttackListForEnemy[LIST_MAX];	//!< �G�̂��߂̍U�����X�g
	s32			_nowIndex;
};

//=============================================================================
//	END OF FILE
//=============================================================================