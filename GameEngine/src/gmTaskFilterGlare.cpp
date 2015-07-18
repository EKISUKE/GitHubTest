//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilterGlare.cpp
//!	@brief	�O���A�t�B���^�[
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
TaskFilterGlare::TaskFilterGlare()
{
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
TaskFilterGlare::~TaskFilterGlare()
{
}

//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool TaskFilterGlare::setup()
{
	// �e�N���X�̏��������Ă�
	TaskFilter::setup();

	return true;
}

//-----------------------------------------------------------------------------
//! ���
//-----------------------------------------------------------------------------
void TaskFilterGlare::cleanup()
{
	TaskFilter::cleanup();
}

//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void TaskFilterGlare::render()
{

	static const s32	SHADER_STANDARD	= SystemShader::SHADER_FILTER;
	static const s32	SHADER_SUBTRACT	= SystemShader::SHADER_SUBTRACT;

	//-------------------------------------------------------------
	// (1) �o�b�N�o�b�t�@���烏�[�N�o�b�t�@�ɃR�s�[
	// �O���A�̏ꍇ�͂ڂ����̂�1/2�o�b�t�@�̏k�����Ă������Ƃ�
	// ��i�p�X���y�ʉ������(�o�C���j�A��4�s�N�Z������)
	//-------------------------------------------------------------
	copyToWorkTexture(SHADER_STANDARD, 0);

	s32 divCount = 6;

	// 1/4�̊p�x�����炷
	f32 rotationOffset = (math::PI * 2.0f) * (0.25f * (1.0f / divCount));

	for(s32 d=0; d<divCount; d++)
	{
		// ������������
		setBlendMode(BM_NONE);

		// ���̕������w��
		f32 angle = (math::PI * 2.0f) * ((f32)d * (1.0f / divCount)) + rotationOffset;
		f32 x	  = cosf(angle);
		f32 y	  = sinf(angle);


		GmShader()->changeShader(SystemShader::SHADER_GLARE);

		// �u���[�����ݒ�
		GmShader()->setUniform2fv("gDir2D", (GLfloat*)&Vector3(x,y,0.0f));
		GmShader()->setUniform1f("gAttenuation", 0.99f);

		//-------------------------------------------------------------
		// (2) ���P�x�̐����𒊏o
		// �摜����臒l(Threshold)�̕������Z
		//-------------------------------------------------------------

		//static float threshold = 0.7f;
		static float threshold = 7.9;
		static float scale	   = 0.1f;

		GmShader()->changeShader(SystemShader::SHADER_SUBTRACT);

		GmShader()->setUniform1fv("gThreshold", (GLfloat*)&threshold);
		GmShader()->setUniform1fv("scale", (GLfloat*)&scale);

		setRenderTarget(1, 1);
		put(0, 0);	// texuter[0] mip=0 : SUBTRACT

		GmShader()->beginPrevShader();

		
		////---- �o�b�N�o�b�t�@���R�s�[
		//{
		//	stringstream name;
		//	name << "�O���A���Z";
		//	name << d;
		//	name << "���";
		//	GmRender()->saveBackBuffer(name.str().c_str());
		//}

		//-------------------------------------------------------------
		// (3) �u���[��݌v���Ă��� (8pixel�P��)
		//-------------------------------------------------------------
		s32		index = 1;
		s32		mip	  = 1;

		for(s32 i=0; i<3; i++)
		{

			// �s�N�Z���̊Ԋu(1, 8, 64)
			GmShader()->setUniform1f("gStride", (GLfloat)(pow(8, (f32)i)) );

			//---- �����u���[
			setRenderTarget(index ^ 1, mip);
			put(index, mip);
			index ^= 1;
		}

		// ���ɖ߂�
		GmShader()->beginPrevShader();

		//-------------------------------------------------------------
		// (4) ���Z�����őS�Ă��o�b�N�o�b�t�@�ɍ�������
		//-------------------------------------------------------------
		setBackBuffer();

		s32	workTextureIndex = index;
		s32 mipLevel		 = mip;


		setBlendMode(BM_ADD_NOALPHA);
		put(workTextureIndex, mipLevel, SHADER_STANDARD);

	}

	setBlendMode(BM_NONE);
}


//=============================================================================
//	END OF FILE
//=============================================================================