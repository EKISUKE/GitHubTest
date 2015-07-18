//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilterTonemapping.cpp
//!	@brief	�g�[���}�b�s���O�t�B���^�[
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
TaskFilterToneMapping::TaskFilterToneMapping()
: _adaptationIndex(0)
{
	for( s32 i=0; i<2; ++i ) {
		_pAdaptationTexture[i] = nullptr;
	}
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
TaskFilterToneMapping::~TaskFilterToneMapping()
{
}

//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool TaskFilterToneMapping::setup()
{
	// �e�N���X�̏��������Ă�
	TaskFilter::setup();

	//----------------------------------------------------------
	// �����e�N�X�`��(1x1)
	//----------------------------------------------------------
	for( s32 i=0; i<2; ++i ) {
		Texture*&	p = _pAdaptationTexture[i];

		SAFE_NEW(p);
		if( p == nullptr ) return false;

		if (!p->createRenderTarget(1, 1, GL_RGBA16F_ARB, GL_RGBA, GL_HALF_FLOAT_ARB)) {
			MessageBox(NULL, "RenderTarget�̍쐬�Ɏ��s���܂���. " __FILE__, "ERROR", MB_OK);
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
//! ���
//-----------------------------------------------------------------------------
void TaskFilterToneMapping::cleanup()
{
	TaskFilter::cleanup();

	SAFE_DELETE(_pAdaptationTexture[0]);
	SAFE_DELETE(_pAdaptationTexture[1]);
}

//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void TaskFilterToneMapping::render()
{
	static const s32	SHADER_STANDARD				= SystemShader::SHADER_FILTER;
	static const s32	SHADER_GET_LUMINANCE		= SystemShader::SHADER_GET_LUMINANCE;
	static const s32	SHADER_LUMINANCE_REDUCTION	= SystemShader::SHADER_LUMINANCE_REDUCTION;
	static const s32	SHADER_LIGHT_ADAPTATION		= SystemShader::SHADER_LIGHT_ADAPTATION;
	static const s32	SHADER_TONEMAPPING_FINAL	= SystemShader::SHADER_TONEMAPPING_FINAL;
	

	//-------------------------------------------------------------
	// (1) �o�b�N�o�b�t�@���烏�[�N�o�b�t�@�ɃR�s�[
	// �O���A�̏ꍇ�͂ڂ����̂�1/2�o�b�t�@�̏k�����Ă������Ƃ�
	// ��i�p�X���y�ʉ������(�o�C���j�A��4�s�N�Z������)
	//-------------------------------------------------------------
	copyToWorkTexture(SHADER_STANDARD, 0);

	// ������������
	setBlendMode(BM_NONE);
	//-------------------------------------------------------------
	// (2) �P�x�����𒊏o���Alog(�ΐ�)��ԂŋP�x��ۑ�����
	// 2x2��4�s�N�Z�������Q�Ƃ���1/2�k���������ɂ����Ȃ�
	//-------------------------------------------------------------
	s32	mip	  = 1;
	s32	index = 0;

	setRenderTarget(index, mip);
	put(0, 0, SHADER_GET_LUMINANCE);

	//-------------------------------------------------------------
	// (3) 1x1�s�N�Z���ɂȂ�܂ŏk�����J��Ԃ��A�ō��P�x���擾
	//-------------------------------------------------------------
	Texture*	pTexture = getWorkTexture(0);

	s32			mipLevelMax = pTexture->getMipLevelMax();

#if 1

	do{
		//---- �~�b�v�i����2�i���ɃR�s�[(1/4�k��)
		s32	nextMip = min(mip + 2, mipLevelMax);	// �ő�l�ȏ�ɂ��Ȃ�
		setRenderTarget(index, nextMip);
		put(index, mip, SHADER_LUMINANCE_REDUCTION);
		// �~�b�v�i�����X�V
		mip = nextMip;
	}while( mip < mipLevelMax );

	//GmRender()->setHDR(false);		// LDR�o�b�t�@�ɐ؂�ւ���
	//setBackBuffer();
	//put(index, mip, SHADER_STANDARD);
	//
	//return;
	// workTexture(0, mip)�ɏW�v���ʂ�����

	//-------------------------------------------------------------
	// (4) ������ / �Ï���
	//-------------------------------------------------------------
	// �����ϊ��p�e�N�X�`���ɍ��̌��ʂ�ۑ�����
	setRenderTarget(_pAdaptationTexture[_adaptationIndex ^ 1]);

	put(index, mip, SHADER_LIGHT_ADAPTATION, _pAdaptationTexture[_adaptationIndex]);

	_adaptationIndex ^= 1;	// �؂�ւ�

	//-------------------------------------------------------------
	// (4) ���ʂ�\��t����
	//-------------------------------------------------------------
	GmRender()->setHDR(false);		// LDR�o�b�t�@�ɐ؂�ւ���
	setBackBuffer();

	s32	workTextureIndex = index;
	s32 mipLevel		 = mip;
	s32 shaderPass		 = SHADER_TONEMAPPING_FINAL;

	// BaseTexcture = 1x1�ɏk�������f�[�^
	// BaseTexture	= ����HDR�e�N�X�`��

	put(_pAdaptationTexture[_adaptationIndex], 0, shaderPass, getWorkTexture(0) );
#endif 
}


//=============================================================================
//	END OF FILE
//=============================================================================