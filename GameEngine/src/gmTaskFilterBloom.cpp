//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilterBloom.cpp
//!	@brief	�u���[���O���A
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
TaskFilterBloom::TaskFilterBloom()
:_brightness(5.0f)
{
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
TaskFilterBloom::~TaskFilterBloom()
{
}

//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool TaskFilterBloom::setup()
{
	// �e�N���X�̏��������Ă�
	TaskFilter::setup();

	return true;
}

//-----------------------------------------------------------------------------
//! ���
//-----------------------------------------------------------------------------
void TaskFilterBloom::cleanup()
{
	TaskFilter::cleanup();
}

//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void TaskFilterBloom::render()
{

	static const s32	SHADER_STANDARD			= SystemShader::SHADER_FILTER;
	static const s32	SHADER_GAUSSIAN_3X3_H	= SystemShader::SHADER_GAUSSIAN_3X3_H;
	static const s32	SHADER_GAUSSIAN_3X3_V	= SystemShader::SHADER_GAUSSIAN_3X3_V;
	static const s32	SHADER_SUBTRACT			= SystemShader::SHADER_SUBTRACT;
	static const s32	SHADER_BLOOM_GLARE		= SystemShader::SHADER_BLOOM_GLARE;

	//-------------------------------------------------------------
	// (1) �o�b�N�o�b�t�@���烏�[�N�o�b�t�@�ɃR�s�[
	// �O���A�̏ꍇ�͂ڂ����̂�1/2�o�b�t�@�̏k�����Ă������Ƃ�
	// ��i�p�X���y�ʉ������(�o�C���j�A��4�s�N�Z������)
	//-------------------------------------------------------------
	copyToWorkTexture(SHADER_STANDARD, 1);

	// ������������
	setBlendMode(BM_NONE);
	//-------------------------------------------------------------
	// (2) ���P�x�̐����𒊏o
	// �摜����臒l(Threshold)�̕������Z
	//-------------------------------------------------------------

	static float threshold = 1.3f;
	static float scale	   = 0.1f;

	if (GetKeyState(VK_OEM_PLUS) & 0x80){
		threshold += 0.1f;
	}
	else if (GetKeyState(VK_OEM_MINUS) & 0x80){
		threshold -= 0.1f;
	}

	GmShader()->changeShader(SystemShader::SHADER_SUBTRACT);

	GmShader()->setUniform1fv("gThreshold", (GLfloat*)&threshold);
	GmShader()->setUniform1fv("scale", (GLfloat*)&scale);

	setRenderTarget(1, 1);
	//setBackBuffer();
	put(0, 1);	// texuter[0] mip=0 : SUBTRACT

	GmShader()->beginPrevShader();

	//return;
	//-------------------------------------------------------------
	// (3) �K�E�V�A���u���[���k�����J��Ԃ��A���i�o�b�t�@����
	//-------------------------------------------------------------
//	s32		resultTextureIndex[16];	// �e�~�b�v�i���̂ǂ���Ɍ��ʂ������Ă��邩
	s32		index = 1;
	s32		mip   = 1;

	//---- �����u���[
	setRenderTarget(index ^ 1, mip);
	put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_H);
	index ^= 1;

	//---- �����u���[
	setRenderTarget(index ^ 1, mip);
	put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_V);
	index ^= 1;

#if 1	
	while( mip<REDUCTION_LEVEL-1 ) {
		
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

	//-------------------------------------------------------------
	// (4) ���Z�����őS�Ă��o�b�N�o�b�t�@�ɍ�������
	//-------------------------------------------------------------
	setBackBuffer();
		
	s32	workTextureIndex = index;
	s32 mipLevel		 = 1;


	setBlendMode(BM_ADD_ALPHA);
	GmShader()->changeShader(SystemShader::SHADER_BLOOM_GLARE);
	GmShader()->setUniform1f("brightness", _brightness);
	put(workTextureIndex, mipLevel);
	GmShader()->beginPrevShader();
	setBlendMode(BM_NONE);
	
#endif 
}


//=============================================================================
//	END OF FILE
//=============================================================================