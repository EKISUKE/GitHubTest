//-----------------------------------------------------------------------------
//!
//!	@file	LightBase.h
//!	@brief	�������
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//!	�������
//-----------------------------------------------------------------------------
class LightBase
{
public:
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	LightBase();

	//! �f�X�g���N�^
	virtual ~LightBase(){};

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �X�V
	virtual void	update();

	//! �f�o�b�O�\��
	virtual void		DebugRender(){};

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//---- �Z�b�g
	//! �ʒu���w��
	void			setPosition(const Vector3& position) { _position = position; }
	//! �����_���w��
	void			setLookAt  (const Vector3& lookAt  ) { _lookAt	 = lookAt;	 }
	//! ���E�̏������ݒ�
	void			setWorldUp (const Vector3& worldUp ) { _worldUp	 = worldUp;	 }
	
	//---- �擾
	//! �p�x���擾
	Vector3			getRotation () const { return _rot;		 }
	//! �ʒu���擾
	Vector3			getPosition	() const { return _position; }
	//! �����_���擾
	Vector3			getLookAt	() const { return _lookAt;   }
	//! ���E�̏�������擾
	Vector3			getWorldUp	() const { return _worldUp;  }

	//! ���C�g�����擾
	Vector3			getLightDir () const { return _lightDir; }

	//! �p���s����擾
	const Matrix&	getMatrix	 () const { return _matrix; }
	//! �r���[�s����擾
	const Matrix&	getViewMatrix() const { return _viewMatrix; }
	
	//@}

protected:
	//---- ��{�f�[�^
	Vector3			_position;		//!< �ʒu���W
	Vector3			_lookAt;		//!< �����_
	Vector3			_worldUp;		//!< ���E�̏�����̊��
	Vector3			_rot;			//!< �p�x
	Vector3			_lightDir;		//!< ���C�g����

	Matrix			_matrix;		//!< �p���s��
	Matrix			_viewMatrix;	//!< �r���[�s��
};

//=============================================================================
//	END OF FILE
//=============================================================================