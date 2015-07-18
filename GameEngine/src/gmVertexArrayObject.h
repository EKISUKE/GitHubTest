//-----------------------------------------------------------------------------
//!
//!	@file	gmVertexArrayObject.h
//!	@brief	���_�t�H�[�}�b�g
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//! �ʏ�`��(�A�j���[�V�����t)VAO�t�H�[�}�b�g
//-----------------------------------------------------------------------------
struct NormalContext
{
	GLuint	_positionLocation;
	GLuint	_blendIndicesLocation;
	GLuint	_blendWeightLocation;
	GLuint	_normalLocation;
	GLuint	_uvLocation;
	GLuint	_specularPowerLocation;		
	//! �v���O�����n���h�����珉����
	void	init(GLuint program);
};

//-----------------------------------------------------------------------------
//! �V���h�E�pVAO�t�H�[�}�b�g
//-----------------------------------------------------------------------------
struct ShadowContext
{
	GLuint	_positionLocation;
	GLuint	_blendIndicesLocation;
	GLuint	_blendWeightLocation;
	//! �v���O�����n���h�����珉����
	void	init(GLuint program);
};

//-----------------------------------------------------------------------------
//! �t�B���^�[�pVAO�t�H�[�}�b�g
//-----------------------------------------------------------------------------
struct FilterContext
{
	GLuint	_positionLocation;
	GLuint	_uvLocation;
	GLuint	_diffuseColorLocation;
	GLuint	_emissiveColorLocation;
	//! �v���O�����n���h�����珉����
	void	init(GLuint program);
};

//-----------------------------------------------------------------------------
//! VAO�N���X
//-----------------------------------------------------------------------------
class VertexArrayObject
{
public:
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	VertexArrayObject();
	//! �f�X�g���N�^
	virtual ~VertexArrayObject();

	//! VAO�쐬
	void	genVAO();
	//! VAO�폜
	void	delVAO();

	//@}
	//-------------------------------------------------------------
	//! @name �J�n�E�I��
	//-------------------------------------------------------------
	//@{

	

	//! �R���e�L�X�g�̓o�^�J�n
	//void	beginRegistContext(GLuint& _vbo, GLuint& ibo);
	void	beginRegistContext();
	//! �R���e�L�X�g�̓o�^�I��
	void	endRegistContext();
	//! �`��J�n
	void	beginRender();
	//! �`��I��
	void	endRender();	

	//@}
private:
	GLuint	_vao;	
};




//=============================================================================
// END OF FILE
//=============================================================================