//-----------------------------------------------------------------------------
//!
//!	@file	gmCameraBase.h
//!	@brief	�J�������
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
//!	�J�������
//-----------------------------------------------------------------------------
class CameraBase
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	CameraBase();

	//! ���z�f�X�g���N�^
	virtual ~CameraBase(){};

	//! ������
	virtual bool	init(Vector3& lookAt, Vector3& _rot);
	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �X�V
	virtual void		update(Controller& control, Vector3& PlayerPos, const f32& PlayerRot,  POINT mouseOffset);
	//! �f�o�b�O�\��
	virtual void		DebugRender();

	//! �o�C�u���[�V�����L����
	//!	@param	[in] vibrationPower �o�C�u���[�V������
	virtual void		Enablevibration(f32 vibrationPower);

	//! �o�C�u���[�V����������
	//!	@param	[in] vibrationPower �o�C�u���[�V������
	virtual void		Disablevibration();

protected:
	//! �������ƖڕW���W�܂ňړ�����
	//! @param	[in]	�ڕW���W
	//! @return	true: �ړ����� false: �ړ���
	bool		moveGoal(Vector3& goalPos);
public:


	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//---- �Z�b�g
	//! �ʒu���w��
	void			setPosition(const Vector3& position)			{ _position = position; }
	//! �����_���w��
	void			setLookAt  (const Vector3& lookAt  )			{ _lookAt	 = lookAt;	}
	//! ���E�̏������ݒ�
	void			setWorldUp (const Vector3& worldUp )			{ _worldUp	 = worldUp;	}
	//!	�����_�Ƃ̋����ύX
	void			changeLookAtLength(const f32& change)			
	{
		_lookAtLength += change;
		setLookAtLengthMax(_lookAtLengthMax + change);
	}
	//! �����_�Ƃ̋����ő�l�ݒ�
	void			setLookAtLengthMax(const f32& lookAtLengthMax)	{ _lookAtLengthMax = lookAtLengthMax; }

	//---- �擾
	//! �����_�Ƃ̋����擾
	f32				getLookAtLength() const { return _lookAtLength; }

	//! �p�x���擾
	Vector3			getRotation () const	{ return _rot;		}
	//! �ʒu���擾
	Vector3			getPosition	() const	{ return _position; }
	//! �����_���擾
	Vector3			getLookAt	() const	{ return _lookAt;   }
	//! ���E�̏�������擾
	Vector3			getWorldUp	() const	{ return _worldUp;  }

	//! �p���s����擾
	const Matrix&	getMatrix	 () const	{ return _matrix; }
	//! �r���[�s����擾
	const Matrix&	getViewMatrix() const	{ return _viewMatrix; }
	//! ���e�s��̎擾
	const Matrix&	getProjMatrix() const	{ return _projMatrix; }
	//! �r���[�~���e�s��̎擾
	Matrix			getViewProjMatrix()		{ return _viewMatrix * _projMatrix; }
	//! ������̎擾
	const Frustum&	getViewFrustum() const	{ return _viewFrustum; }


	//@}

protected:

	//---- ��{�f�[�^
	Vector3			_position;			//!< �ʒu���W
	Vector3			_cameraOffset;		//!< �I�t�Z�b�g�l
	Vector3			_lookAt;			//!< �����_
	Vector3			_worldUp;			//!< ���E�̏�����̊��
	Vector3			_rot;				//!< �p�x

	f32				_radius;			//!< ���a
	f32				_lookAtLength;		//!< �����_�Ƃ̋���
	f32				_lookAtLengthMax;	//!< �����_�Ƃ̋����̍ő�l


	Matrix			_matrix;			//!< �p���s��
	Matrix			_viewMatrix;		//!< �r���[�s��
	Matrix			_projMatrix;		//!< ���e�s��
	bool			_isVib;				//!< �o�C�u���[�V���������ǂ���

	Frustum			_viewFrustum;		//!< ������
};

//=============================================================================
//	END OF FILE
//=============================================================================