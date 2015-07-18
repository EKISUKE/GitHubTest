//-----------------------------------------------------------------------------
//!	
//!	@file	BillBoard.h
//!	@brief	�r���{�[�h
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
//!	�r���{�[�h
//-----------------------------------------------------------------------------
class BillBoard
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	BillBoard()
	: _position	( 0.0f, 0.0f, 0.0f )
	, _scale	( 0.0f, 0.0f, 0.0f )
	, _size		( 0.0f, 0.0f, 0.0f )
	, _rotation	( 0.0f )
	, _texture	( nullptr )
	{
		init();
	}
	//! �R���X�g���N�^
	BillBoard(Vector3		position,
			  Vector3		size,
			  Vector3		scale,
			  Radian		rotation,
			  Texture*		texture )
	{
		setParam(position, size, rotation);
		setTexture(texture);
		_scale = scale;
	}

	//! �f�X�g���N�^
	virtual ~BillBoard(){}

	//! ������
	bool	init();

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �r���{�[�h�̂��߂Ɉʒu�v�Z
	void	calcBillBoardPosition(Vector3* billBoardPos);

	//! �`��
	void	draw(const Color&	color, const s32& aniCount = 0 );

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! ���ݒ�
	void	setParam(Vector3		position,
					 Vector3		size,
					 Radian			rotation)
	{
		_position = position;
		_size	  = size;
		_rotation = rotation;
	}

	
	//! ���W�ݒ�
	void	setPosition(Vector3& position)	{ _position = position; }
	//! �T�C�Y�w��
	void	setSize(Vector3&  size)			{ _size = size; }
	//! �`��T�C�Y�w��
	void	setScale(Vector3& scale)		{ _scale = scale; }
	//! ��]�p�x�ݒ�
	void	setRotate(Radian& rotation)		{ _rotation = rotation; }
	//! �e�N�X�`���ݒ�
	void	setTexture(Texture* texture)    { _texture  = texture;  }

	//! ���W�擾
	Vector3	getPosition()					{ return _position; }
	//! ��]�擾
	Radian	getRotation()					{ return _rotation; }

	//! �e�N�X�`���擾
	Texture* getTexture()					{ return _texture; }
	//@}
protected:
	Vector3		_position;
	Vector3		_scale;
	Vector3		_size;
	Radian		_rotation;
	Texture*	_texture;
	GLuint		_vertex;
};
//=============================================================================
// END OF FILE
//=============================================================================