//-----------------------------------------------------------------------------
//!
//!	@file	gmMatrix.h
//!	@brief	�s�񉉎Z
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	�S���S�s��
//-----------------------------------------------------------------------------
struct Matrix
{
public:
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	Matrix();

	//! �R���X�g���N�^
	Matrix(f32 m11, f32 m12, f32 m13, f32 m14,
		   f32 m21, f32 m22, f32 m23, f32 m24,
		   f32 m31, f32 m32, f32 m33, f32 m34,
		   f32 m41, f32 m42, f32 m43, f32 m44);

	//@}
	//-------------------------------------------------------------
	//! @name ���Z
	//-------------------------------------------------------------
	//@{

	//! ��Z����
	//! @param	[in]	m	���葤�̍s��
	//!	@return	������̍s��
	//
	//	return �� this * m
	Matrix		operator * (const Matrix& m) const;

	//@}
	//-------------------------------------------------------------
	//! @name �s��擾
	//-------------------------------------------------------------
	//@{

	//! ��]�p�x�擾
	Matrix		getRotate() const;

	//@}
	//-------------------------------------------------------------
	//! @name �s�񐶐�
	//-------------------------------------------------------------
	//@{

	//! �t�s��(���s�s��p�̍�����)
	Matrix		inverseFast();

	//! �]�u�s��
	Matrix		transpose();

	//!	���s�ړ�����s��
	//! @param	[in]	v	�ړ�����
	//!	@return ���s�ړ��s��
	static Matrix	translate(const Vector3& v);


	//! �X�P�[�����O
	//!	@param	[in]	s	XYZ�X�P�[���l
	//!	@return	�X�P�[�����O�s��
	static Matrix	scale(const Vector3& s);
	

	//!	�x�����S�̉�]
	//!	@param	[in]	radian	��]�p�x
	//!	@return ��]�s��
	static Matrix	rotateY(const Radian& radian);
	
	//! �y�����S�̉�]
	//!	@param	[in]	radian	��]�p�x
	//!	@return	��]�s��
	static Matrix	rotateZ(const Radian& radian);


	//!	�C�ӎ����S�̉�]
	//!	@param	[in]	v		��]���S�̎�
	//!	@param	[in]	radian	��]�p�x
	//!	@return ��]�s��
	static Matrix	rotateAxis(const Vector3& v, const Radian& radian);



	//! �p���s��(LookAtMatrix)�̍쐬
	//!	@param	[in]	position	�J�����̈ʒu
	//!	@param	[in]	lookAt		�����_
	//!	@param	[in]	worldUp		���E�̏�����̃x�N�g��
	//!	@return	�p���s��
	static Matrix lookAt(const Vector3& position,
						const Vector3& lookAt,
						const Vector3& worldUp = Vector3(0.0f, 1.0f, 0.0f));

	//! �r���[�s��̐���
	//!	@param	[in]	position	�J�����̈ʒu
	//!	@param	[in]	lookAt		�����_
	//!	@param	[in]	worldUp		���E�̏�����̃x�N�g��
	//!	@return �r���[�s��
	static Matrix	view(
						const Vector3& position,
						const Vector3& lookAt,
						const Vector3& worldUp = Vector3(0.0f, 1.0f, 0.0f));

	//! ���e�s��̐���
	//!	@param	[in]	fov			��p(���W�A��)
	//!	@param	[in]	aspect		�A�X�y�N�g�� (��������)
	//!	@param	[in]	nearZ		�߃N���b�v��Z
	//!	@param	[in]	farZ		���N���b�v��Z
	//!	@return ���e�s��
	static Matrix	projection(const Degree& fov, f32 aspect, f32 nearZ, f32 farZ);

	//! ���ˉe�s��̍쐬
	//!	@param	[in]	Left		�N���b�s���O�̍����W
	//!	@param	[in]	Right		�N���b�s���O�̉E���W
	//!	@param	[in]	Bottom		�N���b�s���O�̉����W
	//!	@param	[in]	Top			�N���b�s���O�̏���W
	//!	@param	[in]	Near		�߃N���b�v��
	//!	@param	[in]	Far			���N���b�v��
	//!	@return ���˓��e�s��
	static Matrix	ortho(f32 Left, f32 Right, f32 Bottom, f32 Top, f32 Near, f32 Far);

	//@}

	static const Matrix	ZERO;		//!< �[���s��
	static const Matrix	IDENTITY;	//!< �P�ʍs��
public:
	//---- �S�v�f �~ �S�s
	//			+====+====+====+====+
	//	Vector4	| _x | _y | _z | _w |	 _m[0]  
	//			+====+====+====+====+
	//	Vector4	| _x | _y | _z | _w |	 _m[1]  
	//			+====+====+====+====+
	//	Vector4	| _x | _y | _z | _w |	 _m[2]  
	//			+====+====+====+====+
	//	Vector4	| _x | _y | _z | _w |	 _m[3]  
	//			+====+====+====+====+
	Vector4		_m[4];

};

//=============================================================================
//	END OF FILE
//=============================================================================
