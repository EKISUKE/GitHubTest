//-----------------------------------------------------------------------------
//!
//!	@file	gmCollision.h
//!	@brief	�Փ˔���
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
//! �Փ˔���
//-----------------------------------------------------------------------------
class Collision
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{
	
	//! �f�t�H���g�R���X�g���N�^
	Collision(){}
	//! �R���X�g���N�^
	Collision(Sphere* hitSphere, char* name);
	//! �f�X�g���N�^
	virtual		~Collision();

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �f�o�b�N�`��
	void				DebugRender();

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! ���ݒ�
	//!	@param	[in]	col	�ݒ肷�铖���蔻����
	void				setHitObj(Collision* col);
	//! �����蔻���񏉊���
	void				clearHitObj() { _hitObj.clear(); }

	//! ��{���ݒ�
	//!	@param	[in]	sphere	�����蔻��p�̋�
	//!	@param	[in]	name	�����蔻��̖��O
	void				setCollisionState(Sphere* sphere, GM_CSTR name);
	//! �����蔻��p���̐ݒ�
	//!	@param	[in]	sphere	�����蔻��p�̋�
	void				setHitSphere(Sphere* sphere);
	//! ���O�̐ݒ�
	//!	@param	[in]	name	�����蔻��̖��O
	void				setColName(GM_CSTR name);


	//! ���擾
	Collision*			getHitObj(s32 index)	{ return _hitObj[index]; }
	//! ���O�擾
	char*				getName()	{ return _name; }
	//! �����蔻��p�̋��擾
	Sphere*				getHitSphere() { return _hitSphere; }
	//! �������Ă�����擾
	s32					getHitCount();

	//!	����������̕␳�ʒu�擾
	//!	@param	[in]	obj		�����蔻��̑���
	Vector3				getOffsetPos(Collision*	obj);
	
	//@}
private:
	//---- ��{�����o�[
	char				_name[32];		//!< ���O
	Sphere*				_hitSphere;		//!< �����蔻��p�̋�
	vector<Collision*>	_hitObj;		//!< ������������̏��
};



// �v���g�^�C�v�錾
class LandScape;
class CollisionWall;



//-----------------------------------------------------------------------------
//! �Փ˔���V�X�e��
//-----------------------------------------------------------------------------
class SystemCollision
{
public:
	class	Object;		//!< ���̃f�[�^�@�I�u�W�F�N�g

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{
private:
	//! �R���X�g���N�^
	SystemCollision();
	//! �R�s�[�R���X�g���N�^
	SystemCollision(SystemCollision& src){};
	//! �f�X�g���N�^
	virtual ~SystemCollision();

public:

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �f�o�b�O�`��
	void				DebugRender();
	//! �f�o�b�O�`��(2D)
	void				DebugRender2D();

	//@}
	//-------------------------------------------------------------
	//! @name �ǂݍ��݁E�폜
	//-------------------------------------------------------------
	//@{

	//! X�t�@�C������|���S���f�[�^�[�����
	//! @param	[in]	colModel	X�t�@�C��
	bool				loadXfile(TaskModelX* colModel);

	//! �t���[������|���S���f�[�^�[�����
	//! @param	[in]	colModel	�t���[��
	//!	@param	[in]	parentModel	�t���[���̑����^�X�N���f��
	bool				loadLandScaleWithFrame(AssetModelX::Frame* colFrame, TaskModelX* parentModel);

	//! �t���[������f�[�^�ǂݍ���
	//! @param	[in]	pModel		X�t�@�C��
	//!	@param	[in]	max			�ő���W(XZ)�ۑ��p
	//!	@param	[in]	min			�ŏ����W(XZ)�ۑ��p
	//! @param  [in]	isWall		�Ǘp���ǂ���
	//! @param  [in]	modelMat	���f���s��
	//! @param  [in]	joints		���̃t���[���̑����W���C���g
	SystemCollision::Object loadData(AssetModelX::Frame* pModelFrame, Vector3& max, Vector3& min, bool isWall, Matrix& modelMat, vector<TaskModelX::Joint> joints);

	//! �t���[������f�[�^�ǂݍ���(�ċA�Ȃ�)
	//! @param	[in]	pModel		X�t�@�C��
	//!	@param	[in]	max			�ő���W(XZ)�ۑ��p
	//!	@param	[in]	min			�ŏ����W(XZ)�ۑ��p
	//! @param  [in]	isWall		�Ǘp���ǂ���
	//! @param  [in]	modelMat	���f���s��
	//! @param  [in]	joints		���̃t���[���̑����W���C���g
	SystemCollision::Object loadDataOneFrame(AssetModelX::Frame* pModelFrame, Vector3& max, Vector3& min, bool isWall, Matrix& modelMat, vector<TaskModelX::Joint> joints);


	//! X�t�@�C������|���S���f�[�^�[��Ԃ�
	//! @param	[in]	pModel		X�t�@�C��
	//!	@param	[in]	dst			�ǂݍ��݌��ʕۑ���
	//!	@param	[in]	max			�ő���W(XZ)�ۑ��p
	//!	@param	[in]	min			�ŏ����W(XZ)�ۑ��p
	//! @param  [in]	isWall		�Ǘp���ǂ���
	SystemCollision::Object	loadXfile(TaskModelX* pModel, vector<SystemCollision::Object>& _dst, Vector3& max, Vector3& min, bool isWall);

	//! �t���[������|���S���f�[�^��Ԃ�
	//! @param	[in]	colFrame	Frame
	//!	@param	[in]	dst			�ǂݍ��݌��ʕۑ���
	//!	@param	[in]	max			�ő���W(XZ)�ۑ��p
	//!	@param	[in]	min			�ŏ����W(XZ)�ۑ��p
	//! @param  [in]	isWall		�Ǘp���ǂ���
	//! @param  [in]	parentModel	���̃t���[�����������f��
	SystemCollision::Object loadWithFrame(AssetModelX::Frame* colFrame, vector<SystemCollision::Object>& _dst, Vector3& max, Vector3& min, bool isWall, TaskModelX* parentModel);

	//! �I�u�W�F�N�g�̏���
	void				deleteObject();
	////!	�I�u�W�F�N�g�̎擾
	//Collision::Object*	getObject(s32 index) const {return &_objects[index]; }

	////!	�I�u�W�F�N�g�̌����擾
	//s32					getObjectCount() const { return _objects.size(); }

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{
	
	//!	�w�肵�����W�ƃ��f���̓����蔻��ƕ␳
	//!	@param	[in]	positon		�`�F�b�N����ʒu
	//!	@param	[in]	heightLimit	����������
	//! @param	[in]	myGridNum	�O���b�h�ԍ�
	//! @param	[in]	isHitGround �n�ʂɓ����������ǂ���		
	void				checkCollisionModel(Vector3& position, f32 radius, bool& isHitGround);

	

	//! �C���X�^���X�擾
	static SystemCollision*	getInstance()
	{
		static SystemCollision _myInstance;
		return &_myInstance;
	}


	//! �Ō�Ƀq�b�g�������W�̎擾
	Vector3				getLastHitPosForWall()	{ return _LastHitPosForWall; }


	//! �����m�̂����蔻��
	//!	@param	[in]	cubePos1	��ڂ̋��̍��W
	//!	@param	[in]	cubePos2	��ڂ̋��̍��W
	//!	@param	[in]	cubeRadius1	��ڂ̋��̔��a
	//!	@param	[in]	cubeRadius2	��ڂ̋��̔��a
	//!	@retval			true : �������Ă�	false : �������Ă��Ȃ�
	bool				HitSphere(Vector3& spherePos1, Vector3& spherePos2, f32 sphereRadius1, f32 sphereRadius2);

	//! �~���m�̂����蔻��
	//!	@param	[in]	cubePos1	��ڂ̉~�̍��W
	//!	@param	[in]	cubePos2	��ڂ̉~�̍��W
	//!	@param	[in]	cubeRadius1	��ڂ̉~�̔��a
	//!	@param	[in]	cubeRadius2	��ڂ̉~�̔��a
	//!	@retval			true : �������Ă�	false : �������Ă��Ȃ�
	bool				HitCircle(Vector3& circlePos1, Vector3& circlePos2, f32 circleRadius1, f32 circleRadius2);

	//@}
	
private:
	
	static	LandScape*					_pLandScape;		//!< �����h�X�P�[�v
	static	CollisionWall*				_pColWall;			//!< ��
	Vector3								_LastHitPosForWall;	//!< �Ō�ɓ����������W
	

};

typedef vector<SystemCollision::Object>	vecCol;			//!< �����蔻��I�u�W�F�N�g��vector

//-----------------------------------------------------------------------------
// �A�N�Z�X�C���^�[�t�F�C�X
//-----------------------------------------------------------------------------
__forceinline
SystemCollision*	GmSystemCollision()
{
	return SystemCollision::getInstance();
}


//-----------------------------------------------------------------------------
//! �����蔻����
//-----------------------------------------------------------------------------
struct CollisionData
{
	Triangle			_triangle;			//!< �|���S���f�[�^
	u32					_myFrameNumber;		//!< �����̑����t���[���̔ԍ�
	bool				_isActive;			//!< �A�N�e�B�u���ǂ���
};

//-----------------------------------------------------------------------------
//!	���̃f�[�^�I�u�W�F�N�g
//-----------------------------------------------------------------------------
class SystemCollision::Object
{
public:
	//!	�R���X�g���N�^
	Object() {}

	//! �����蔻�����ǉ�
	void		addColData(const CollisionData& col)
	{
		_colData.push_back(col);
	}

	//! ���X�g�̏�����
	void		resetColData()
	{
		_colData.clear();
	}

	//! �����蔻����擾
	CollisionData*		getColData(u32 index)
	{
		return &_colData[index];
	}

	//! �����蔻����̐����擾
	s32					getColDataCount() const 
	{
		return _colData.size();
	}

	//! �����蔻����z��擾
	vector<CollisionData>	getColDataVec() const
	{
		return _colData;
	}

private:
	vector<CollisionData>	_colData;
};


//=============================================================================
//	END OF FILE
//=============================================================================