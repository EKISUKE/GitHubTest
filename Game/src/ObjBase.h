//-----------------------------------------------------------------------------
//!
//!	@file	ObjBase.h
//!	@brief	�I�u�W�F�N�g���
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

class CollisionObject;
//-----------------------------------------------------------------------------
//!	�I�u�W�F�N�g���
//-----------------------------------------------------------------------------
class	ObjBase : virtual public IObj , public CollisionObject, public Base
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	ObjBase()
	: _position		(0.0f, 0.0f, 0.0f)
	, _oldPosition	(0.0f, 0.0f, 0.0f)
	, _mov			(0.0f, 0.0f, 0.0f)
	, _rotation		(0.0f, 0.0f, 0.0f)
	, _radius		(0.0f)
	{
	}

	//! �f�X�g���N�^
	virtual ~ObjBase() 
	{
	};

	//!	������
	virtual bool	Initialize() { return true; }

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �X�V
	virtual void	Update()	 {};
	//!	�`��
	virtual void	Render()	 {};

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//---- �擾
	Vector3			getPosition	() const { return _position; }
	Vector3			getMov		() const { return _mov; }
	Vector3			getRotation () const { return _rotation; }
	f32				getRadius	() const { return _radius; }

	Vector3			getHitPosition	  () const { return _hitPosition;	 }
	f32				getHitRadius      () const { return _hitRadius;		 }

	//@}
protected:

	//---- �����o�[�ϐ�
	Vector3			_position;
	Vector3			_oldPosition;
	Vector3			_mov;
	Vector3			_rotation;
	f32				_radius;

	Vector3			_hitPosition;		//!< �����蔻��p���W
	f32				_hitRadius;			//!< �����蔻��p
	
};

//=============================================================================
//	END OF FILE
//=============================================================================