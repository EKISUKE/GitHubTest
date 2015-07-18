//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilter.cpp
//!	@brief	�|�X�g�G�t�F�N�g�t�B���^���
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
TaskFilter::TaskFilter()
{
	_pWorkTexture[0] = nullptr;
	_pWorkTexture[1] = nullptr;
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
TaskFilter::~TaskFilter()
{
}


//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool TaskFilter::setup(Size<s32>& workTexResolution)
{
	//-------------------------------------------------------------
	// �`��^�[�Q�b�g�e�N�X�`���̍쐬
	//-------------------------------------------------------------
	// �~�b�v�i���w��ŏk���o�b�t�@�쐬
	for( s32 i=0; i<2; i++ ) {
		SAFE_NEW(_pWorkTexture[i]);
		if( _pWorkTexture[i] == nullptr ) return false;
		
		if (!_pWorkTexture[i]->createRenderTarget(workTexResolution._w, workTexResolution._h, GL_RGBA16F_ARB, GL_RGBA, GL_HALF_FLOAT_ARB, true)) {
			MessageBox(NULL, "RenderTarget�̍쐬�Ɏ��s���܂����B"__FILE__, "ERROR", MB_OK);
			return false;
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
//! ���
//-----------------------------------------------------------------------------
void TaskFilter::cleanup()
{
	SAFE_DELETE(_pWorkTexture[0]);
	SAFE_DELETE(_pWorkTexture[1]);
}

//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void TaskFilter::render()
{
	//-------------------------------------------------------------
	// �o�b�N�o�b�t�@�̓��e���e�N�X�`���ɃR�s�[
	//-------------------------------------------------------------
	copyToWorkTexture(3);
	
	//-------------------------------------------------------------
	// �R�s�[�����e�N�X�`�����g�p���ăV�F�[�_�ŕ`���߂�
	//-------------------------------------------------------------
	s32	workTextureIndex = 0;
	s32 mipLevel		 = 3;
	s32	shaderPass		 = 3;

	put(workTextureIndex, mipLevel, shaderPass);
}


//-----------------------------------------------------------------------------
//! �o�b�N�o�b�t�@���烏�[�N�e�N�X�`���ɃR�s�[
//! @param	[in]	shaderPass  �R�s�[����Ƃ��Ɏg���V�F�[�_�ԍ�
//!	@param	[in]	mipLevel	�R�s�[��̃~�b�v���x��
//!	@param	[in]	isPrevFrame	1�t���[���O�̂��̂��R�s�[���邩�ǂ���
//-----------------------------------------------------------------------------
void TaskFilter::copyToWorkTexture(s32 shaderPass, s32 mipLevel, bool isPrevFrame)
{
	//-------------------------------------------------------------
	//! �o�b�N�o�b�t�@���烏�[�N�e�N�X�`���ɃR�s�[
	//-------------------------------------------------------------
	Texture* pSrc;
	if(isPrevFrame){
		pSrc = GmRender()->getPrevFrameBackBuffer();
	}else{
		pSrc = GmRender()->getBackBuffer();
	}
	Texture* pDest = _pWorkTexture[0];

	// �`���ύX
	//setRenderTarget(pDest, 0);
	setRenderTarget(pDest, mipLevel);
	

	// ��ʂɃe�N�X�`���`��
	put(pSrc, mipLevel, shaderPass);

	// ���ɖ߂�
	setBackBuffer();
	
}

//-----------------------------------------------------------------------------
//! ���[�N�e�N�X�`����S��ʂɒ���t��
//! @param	[in]	index		���[�N�e�N�X�`���̔ԍ�(0 or 1)
//! @param	[in]	mipLevel	�~�b�v���x��
//!	@param	[in]	shaderPass	�V�F�[�_�[�p�X�ԍ�(-1�Ȃ炱�̒��ŃV�F�[�_���J�n�A�I�����Ȃ�)
//!	@param	[in]	pSubTexture	�T�u�e�N�X�`��
//-----------------------------------------------------------------------------
void TaskFilter::put(s32 index, s32 mipLevel, s32 shaderPass, Texture* pSubTexture)
{
//	glDisable(GL_CULL_FACE);
	put(getWorkTexture(index), mipLevel, shaderPass, pSubTexture);
//	glEnable(GL_CULL_FACE);
}

void TaskFilter::put(Texture* pTexture, s32 mipLevel, s32 shaderPass, Texture* pSubTexture)
{

	//-------------------------------------------------------------
	// ���ƍ����������Ŏ擾
	//-------------------------------------------------------------
	//---- �e�N�X�`���̕��ƍ���
	s32		tw = pTexture->getWidth(mipLevel);
	s32		th = pTexture->getHeight(mipLevel);

	//-------------------------------------------------------------
	// �V�F�[�_�萔(Shader Constant)�̐ݒ�
	//-------------------------------------------------------------
	//---- �V�F�[�_�̕ύX
	// �V�F�[�_�p�X��-1�ȊO�Ȃ�
	if( shaderPass != -1 ) {
		// �V�F�[�_��ύX
		GmShader()->changeShader((SystemShader::SHADER_MODE)shaderPass);
	}

	//---- �e�N�X�`���T�C�Y
	Vector4	 textureSize((f32)tw, (f32)th, 1.0f/(f32)tw, 1.0f/(f32)th);
	GmShader()->setUniform4fv("gTextureSize", (GLfloat*)&textureSize);
	//---- �~�b�v�ԍ�
	GmShader()->setUniform1f("gMipLod", (GLfloat)mipLevel);
	//---- �e�N�X�`��
	GmShader()->bindTexture("BaseTexture", pTexture->getColorBuffer(), 0);

	if( pSubTexture ) {
		GmShader()->bindTexture("BaseTexture2", pSubTexture->getColorBuffer(), 1);
	}
	//-------------------------------------------------------------
	// �`�� (Draw)
	//-------------------------------------------------------------
	static GLuint VB;
	static bool	Initialized = false;
	if( !Initialized ) {
		glGenBuffers(1, &VB);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		Initialized = true;
	}

	const Vertex	v[4] = {
		{ Float3(-1.0f, -1.0f, 0.0f), Color(255,255,255,255), Float2(0.0f, 0.0f) },
		{ Float3(+1.0f, -1.0f, 0.0f), Color(255,255,255,255), Float2(1.0f, 0.0f) },
		{ Float3(-1.0f, +1.0f, 0.0f), Color(255,255,255,255), Float2(0.0f, 1.0f) },
		{ Float3(+1.0f, +1.0f, 0.0f), Color(255,255,255,255), Float2(1.0f, 1.0f) }
	};


	//---- �`��
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);

	// ���_�o�b�t�@�o�C���h
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	// �f�[�^�]��
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 4, v);

	GmShader()->setAndEnableVertexAttribute("position"		, 3, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, _position));
	GmShader()->setAndEnableVertexAttribute("uv"			, 2, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, _uv));
	GmShader()->setAndEnableVertexAttribute("color"			, 4, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, _color));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	GmShader()->DisableVertexAttribute("position");
	GmShader()->DisableVertexAttribute("uv");
	GmShader()->DisableVertexAttribute("color");

	GmShader()->unbindTexture();

	glBindBuffer(GL_ARRAY_BUFFER, 0);

//	glDisable(GL_TEXTURE_2D);
	// ���ɖ߂�
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	// �V�F�[�_�p�X��-1�ȊO�Ȃ�
	if( shaderPass != -1 ) {
		// �ȑO�̃V�F�[�_�ɖ߂�
		GmShader()->beginPrevShader();
	}
//	glPopAttrib();
}

//-----------------------------------------------------------------------------
//! �`��^�[�Q�b�g�̐ݒ�
//! @param	[in]	index		���[�N�e�N�X�`���̔ԍ�(0 or 1)
//!	@param	[in]	mipLevel	�~�b�v���x��
//-----------------------------------------------------------------------------
void TaskFilter::setRenderTarget(s32 index, s32 mipLevel)
{
	setRenderTarget( getWorkTexture(index), mipLevel );
}
void TaskFilter::setRenderTarget(Texture* pRenderTarget, s32 mipLevel)
{
	GmRender()->setRenderTarget(pRenderTarget, Float2(0,0), mipLevel);
}

//-----------------------------------------------------------------------------
//! �`��^�[�Q�b�g���o�b�N�o�b�t�@�ɖ߂�
//-----------------------------------------------------------------------------
void TaskFilter::setBackBuffer()
{
	s32 mipLevel = 0;
	GmRender()->setRenderTarget(GmRender()->getBackBuffer(), Float2(0,0), mipLevel);
}


//=============================================================================
//	END OF FILE
//=============================================================================