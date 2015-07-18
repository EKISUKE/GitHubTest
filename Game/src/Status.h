//-----------------------------------------------------------------------------
//!
//!	@file	Status.h
//!	@brief	��{�X�e�[�^�X
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

class IStatus
{
public:
	struct ParamBase {};

	virtual ~IStatus() {}

	//virtual ParamBase*	getParamBase() = 0;

	//virtual void	setHp() = 0;

	virtual void	loadCSV(const char* fileName) = 0;
protected:
};


//-----------------------------------------------------------------------------
//! �I�u�W�F�N�g�p�X�e�[�^�X
//-----------------------------------------------------------------------------
class ObjectStatus
{
public:

	//-------------------------------------------------------------
	//!	@name	������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	ObjectStatus()
	{
	}
	//! �R���X�g���N�^
	ObjectStatus(s32 hp, s32 hpMax)
	: _isActive(true)
	{
		setStatus(hp, hpMax);
	}

	//@}
	//-------------------------------------------------------------
	//!	@name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �_���[�W�֐�
	void	damage(u32 damage)
	{
		if( _isActive ){
			_hp -= damage;
		}
		if( _hp <= 0 ){
			_isActive = false;
		}
	}

	//@}
	//-------------------------------------------------------------
	//!	@name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �X�e�[�^�X�ݒ�
	void	setStatus(s32 hp, s32 hpMax)
	{
		_hp    = hp;
		_hpMax = hpMax;
		_isActive = true;
	}

	//@}

	
	s32		_hp;
	s32		_hpMax;
	bool	_isActive;
};

//-----------------------------------------------------------------------------
//! �L�����N�^�[�p�X�e�[�^�X
//-----------------------------------------------------------------------------
class Status : public IStatus
{
public:

	struct Param : public  ParamBase
	{
	/*	f32&	operator [] (STATUS_TYPE index)
		{
			return (f32)(*this)[index];
		}*/

		//f32			_InfOfParam[STATUS_MAX];	//!< �X�e�[�^�X�̏��
		f32			_HP;						//!< �q�b�g�|�C���g
		f32			_HPMAX;						//!< �q�b�g�|�C���g�̍ő�l
		f32			_MP;						//!< �}�i�|�C���g
		f32			_MPMAX;						//!< �}�i�|�C���g�̍ő�l
		f32			_speedMAX;					//!< �ړ����x
		f32			_atkPow;					//!< �U����
		f32			_atkDelay;					//!< �U���̃f�B���C����
		f32			_sklDelay;					//!< �X�L���̃f�B���C����
		f32			_jumpPow;					//!< �W�����v��
		f32			_GaugeScaleH;				//!< �Q�[�W�T�C�YH
		f32			_GaugeScaleW;				//!< �Q�[�W�T�C�YW
		BillBoard	_GaugeBoard;				//!< �q�b�g�|�C���g�Q�[�W�`��p
		//! �`��
		void		Render(bool draw2D, Color& gaugeColor, Vector3 drawPosition, Texture* pTexture);
	};

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	Status() {}

	//! �f�X�g���N�^
	virtual ~Status() {}

	//! �p�����[�^�l������
	virtual void	resetParam();

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �`��
	virtual void Render(bool draw2D, Color& gaugeColor, Vector3& drawPosition = Vector3(0,0,0), Texture* pTexture = nullptr)
	{
		_param.Render(draw2D, gaugeColor, drawPosition, pTexture);
	}


	//! CSV�t�@�C���̓ǂݍ���
	virtual void	loadCSV(const char* fileName);


	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//!	�X�e�[�^�X��ݒ�
	void				setParam(const Param& param)
	{
		_param = param;
	}

	//! �X�e�[�^�X�ݒ�(�X�e�[�^�X�^�C�v�Őݒ�)
	virtual void		setParam(StatusAssets::STATUS_TYPE type)
	{
		_param = *IStatusAssets()->getStatus(type)->getParam();
	}

	//! �p�����[�^�擾
	virtual Param*		getParam()
	{
		return &_param;
	}

	//@}
protected:
	Param	_param;
};

//Status::Param* p = (Status::Param*)gpStatus->getParam();
//f32 hp = p._HP;

//-----------------------------------------------------------------------------
//! �L�����N�^�[�p�X�e�[�^�X(�q�[���[�p)
//-----------------------------------------------------------------------------
class StatusHeal : public Status
{
public:
	
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{


	struct ParamHeal : public Param
	{
		f32 _healPow;
		//f32			_InfOfParam[STATUS_MAX];	//!< �X�e�[�^�X�̏��
	};

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �`��
	virtual void Render(bool draw2D, Color& gaugeColor, Vector3& drawPosition = Vector3(0,0,0), Texture* pTexture = nullptr)
	{
		_paramHeal.Render(draw2D, gaugeColor, drawPosition, pTexture);
	}

	//! CSV�t�@�C���̓ǂݍ���
	virtual void loadCSV(const char* fileName);

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �X�e�[�^�X�ݒ�(�X�e�[�^�X�^�C�v�Őݒ�)
	virtual void		setParam(StatusAssets::STATUS_TYPE type)
	{
		_paramHeal = *(ParamHeal*)IStatusAssets()->getStatus(type)->getParam();
	}

	virtual Param*		getParam()
	{
		return &_paramHeal;
	}

	//@}
protected:
	ParamHeal	_paramHeal;
};


//=============================================================================
//	END OF FILE
//=============================================================================