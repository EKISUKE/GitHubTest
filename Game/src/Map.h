//-----------------------------------------------------------------------------
//!
//!	@file	Map.h
//!	@brief	�}�b�v
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

class Castle;
class CastleWall;

//-----------------------------------------------------------------------------
//! �}�b�v
//-----------------------------------------------------------------------------
class Map : public ObjBase
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	Map();

	//! �f�X�g���N�^
	virtual			~Map();
	//! ������
	virtual bool	Initialize();

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �X�V
	virtual void	Update();
	//! �`��
	virtual void	Render(bool isShadow);
	//! �f�o�b�O�`��
	virtual void	debugRender();

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! ��C���X�^���X���擾
	Castle*			getCastle() {return _castle;}

	//! �Ǐ��擾
	CastleWall*		getWall() { return _myWall; }

	////! �Ǐ��擾
	//CastleWall*	getWall(s32 Index) { return _myWall[Index]; }

	////! �ǂ̐��擾
	//s32					getWallCount() { return WALL_MAX; }
	//@}
protected:
	AssetModelX*		_pAssetMapModel;	//!< �}�b�v���f��(�I���W�i���f�[�^)
	TaskModelX*			_pTaskMapModel;		//!< �}�b�v���f��(�`��ړ�)
	Castle*				_castle;			//!< ��
	//static const int	WALL_MAX = 36;		//!< �ǂ̍ő吔
	//CastleWall*			_myWall[WALL_MAX];	//!< ���
	CastleWall*			_myWall;			//!< ���
};

//-----------------------------------------------------------------------------
//! ��ǈ��
//-----------------------------------------------------------------------------
class CastleWall  : public CollisionObject
{
public:
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	//! @param	[in]	wall ��ǂ̃t���[���̃|�C���^�[
	CastleWall(AssetModelX::Frame*	wall);

	//! �f�X�g���N�^
	virtual ~CastleWall();

	//! ������
	bool	Initialize();

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �X�V
	void	Update();

	//! �f�o�b�O�`��
	void	debugRender();

	//! �ǂ������
	void	Crash();

	//! �Ԃ�
	void	Vibration();
	
	//! �_���[�W
	void	Damage(u32	damage);

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�Q��
	//-------------------------------------------------------------
	//@{

	//! AABB�f�[�^�擾
	AABB&		getAABB()        { return _colAABB; }

	//! ���Ă��邩�ǂ���
	bool		isCrash() const { return _isCrash; }



	//@}
protected:
	Vector3					_drawPosition;		//!< �`����W
	f32						_rotation;			//!< �p�x
	AssetModelX::Frame*		_wallModel;			//!< �ǃ��f��(�t���[���̃|�C���^)
	AABB					_colAABB;			//!< �����蔻��pAABB
	ObjectStatus*			_myStatus;			//!< �X�e�[�^�X�̍\����
	bool					_isCrash;			//!< �j�󂳂ꂽ���ǂ���
	bool					_isDamage;			//!< �_���[�W
};


//-----------------------------------------------------------------------------
//! ��
//-----------------------------------------------------------------------------
class Castle
{
public:
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	//! @param	[in]	castle ��̃t���[���|�C���^�[
	Castle(AssetModelX::Frame*	castle);
	//! �f�X�g���N�^
	virtual		~Castle();
	//! ������
	bool		Initialize();

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �X�V
	void		Update();
	
	//! �f�o�b�O�`��
	void		debugRender();

	//! �邪�����
	void		Crash();
	
	//! �Ԃ�
	void		Vibration();

	//! �_���[�W
	void		Damage(u32 damage);
	
	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! AABB�f�[�^�擾
	AABB&		getAABB()        { return  _colAABB; }

	//! ���Ă��邩�ǂ���
	bool		isCrash() const  { return _isCrash; }

	//! �A�N�e�B�u���ǂ���
	bool		isActive() const { return _myStatus->_isActive; }

	//@}
protected:
	AssetModelX::Frame*	_castleModel;	//!< ��
	
	Vector3				_drawPosition;	//!< �`����W
	AABB				_colAABB;		//!< �����蔻��pAABB
	ObjectStatus*		_myStatus;		//!< �X�e�[�^�X�̍\����
	bool				_isCrash;		//!< �j�󂳂ꂽ���ǂ���
	bool				_isDamage;		//!< �_���[�W
};


//-----------------------------------------------------------------------------
//! ��A��ǂ݂̂̓����蔻��Ǘ�
//-----------------------------------------------------------------------------
class CastleCollisionManager
{
private:
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	CastleCollisionManager(){}
	//! �R�s�[�R���X�g���N�^
	CastleCollisionManager(CastleCollisionManager& src){}
	//! �f�X�g���N�^
	virtual ~CastleCollisionManager(){}

	//@}
public:

	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �U���Փ˔���ƃ_���[�W
	//! @param	[in]	attackAABB	���肷��U��AABB
	//!	@param	[in]	offset		�U���̍��W
	//!	@param	[in]	damage		�����������̂ɗ^����_���[�W��
	void	hitAttackAndDamage(AABB& attackAABB, Vector3& offset, u32 damage);

	//! �O�ɏo��Ȃ��悤�ɂ��邽�߂̊֐�
	void	hitOutRange(Vector3& position);

	//@}
	//-------------------------------------------------------------
	//! @name �擾��ݒ�
	//-------------------------------------------------------------
	//@{

	//! �C���X�^���X�擾
	static CastleCollisionManager* getInstannce()
	{
		static CastleCollisionManager _myInstance;
		return &_myInstance;
	}

	//! ��I�u�W�F�N�g(��ǂȂ�)�̋߂����ǂ���(XButton�\���p)
	//!	@param	[in]	aabb		�����蔻��pAABB
	//!	@param	[in]	offset		AABB�̍��W
	//!	@param	[in]	addBouns	�ǉ������蔻��
	//!	@return true:�߂� false:����
	bool		isNearCastleObject(AABB& aabb, Vector3& offset, f32 addBounds = 100.0f);

	//! ���݃A�N�e�B�u�ȓ����蔻��Ɠ������Ă���ǂ���
	Vector3		getSinkVal(AABB& aabb, bool& hitGround, Vector3& offset = Vector3(0,0,0));

	//! ��Ə�ǂ̓����蔻��ݒ�
	void		setCastleCol(Castle* colCastle, CastleWall* colCastleWall);

	//@}
private:
	Castle*		_castle;		//!< ��̃|�C���^	
	CastleWall*	_castleWall;	//!< ��ǂ̃|�C���^
};

__forceinline
//-----------------------------------------------------------------------------
// �A�N�Z�X�C���^�[�t�F�C�X
//-----------------------------------------------------------------------------
CastleCollisionManager* ICastleCol()
{
	return CastleCollisionManager::getInstannce();
}


//=============================================================================
//	END OF FILE
//=============================================================================