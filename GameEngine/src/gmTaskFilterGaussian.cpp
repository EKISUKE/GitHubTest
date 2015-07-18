//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilterGaussian.cpp
//!	@brief	�ڂ����t�B���^�[
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
TaskFilterGaussian::TaskFilterGaussian()
: _blurPathCount(4)
{
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
TaskFilterGaussian::~TaskFilterGaussian()
{
}

//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool TaskFilterGaussian::setup()
{
	// �e�N���X�̏��������Ă�
	TaskFilter::setup();

	return true;
}

//-----------------------------------------------------------------------------
//! ���
//-----------------------------------------------------------------------------
void TaskFilterGaussian::cleanup()
{
	TaskFilter::cleanup();
}

//-----------------------------------------------------------------------------
//! �`��(�n���ꂽ�摜���ڂ����ĕۑ���ɕۑ�����)
//!	@param	[in]	pSrc	�ڂ������摜
//!	@param	[out]	pDest	�ڂ������摜�ۑ���
//-----------------------------------------------------------------------------
void TaskFilterGaussian::render(Texture* pSrc, Texture* pDest)
{

	static const s32	SHADER_STANDARD			= SystemShader::SHADER_FILTER;
	static const s32	SHADER_GAUSSIAN_3X3_H	= SystemShader::SHADER_GAUSSIAN_3X3_H;
	static const s32	SHADER_GAUSSIAN_3X3_V	= SystemShader::SHADER_GAUSSIAN_3X3_V;
	//-------------------------------------------------------------
	// (1) ���摜���R�s�[����
	//-------------------------------------------------------------
	GmRender()->setRenderTarget(getWorkTexture(0), Float2(0, 0), 1);

	put(pSrc, 1, SHADER_STANDARD);

	setBackBuffer();

	//-------------------------------------------------------------
	// (2) �R�s�[�������̂��u���[���Ăڂ���
	//-------------------------------------------------------------
	s32		index = 0;
	s32		mip = 1;


#if 1
	//---- �����u���[
	setRenderTarget(index ^ 1, mip);
	put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_H);
	index ^= 1;

	//---- �����u���[
	setRenderTarget(index ^ 1, mip);
	put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_V);
	index ^= 1;


	while (mip<_blurPathCount) {

		//---- �~�b�v�i������i���ɃR�s�[
		setRenderTarget(index, mip + 1);
		put(index, mip, SHADER_STANDARD);
		mip++;

		//---- �����u���[
		setRenderTarget(index ^ 1, mip);
		put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_H);
		index ^= 1;

		//---- �����u���[
		setRenderTarget(index ^ 1, mip);
		put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_V);
		index ^= 1;

	}
#else

	setRenderTarget(index ^ 1, mip);
	put(index, mip, SystemShader::SHADER_EXPAND_BLUR);
	index ^= 1;

#endif

	//-------------------------------------------------------------
	// (4) �ڂ��������ʂ�ۑ���ɕ`��
	//-------------------------------------------------------------
	GmRender()->setRenderTarget(pDest, Float2(0, 0), 0);

	put(0, mip, SHADER_STANDARD);

	// �o�b�N�o�b�t�@�ɖ߂�
	setBackBuffer();
	
}


//=============================================================================
//	END OF FILE
//=============================================================================