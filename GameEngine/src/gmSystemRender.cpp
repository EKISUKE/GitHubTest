//-----------------------------------------------------------------------------
//!
//!	@file	gmSystemRender.h
//!	@brief	�`��Ǘ��V�X�e��
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"
//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
SystemRender::SystemRender()
//: _backBuffer    (0)
: _pBackBuffer	 (nullptr)
, _pBackBufferHDR(nullptr)
, _isHDR         (false)
, _currentFrame	 (0)
, _prevFrame	 (0)
{
//	glGenFramebuffers(1, &_frameBuffer);
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
SystemRender::~SystemRender()
{
	cleanup();
//	glDeleteFramebuffers(1, &_frameBuffer);
}

//-----------------------------------------------------------------------------
//!	������
//-----------------------------------------------------------------------------
bool SystemRender::init()
{

	//-------------------------------------------------------------
	// �o�b�N�o�b�t�@���쐬
	//-------------------------------------------------------------
	SAFE_NEW(_pBackBuffer);
	if( _pBackBuffer == nullptr ) return false;
	
	// �����݃E�B���h�E�̉𑜓x��ύX�����Ƃ��Ƀo�b�N�o�b�t�@�̉𑜓x�͕ς��Ȃ�����
	// �o�b�N�o�b�t�@�̉𑜓x���ύX����d�g�݂����\��
	Size<s32>	windowSize = GmWindow()->getResolution();
	if( !_pBackBuffer->createRenderTarget(windowSize._w, windowSize._h, GL_RGBA, GL_RGBA, GL_FLOAT, true) ){
		GM_ASSERT(false, "�o�b�N�o�b�t�@�̍쐬�Ɏ��s���܂����B");
		return false;
	}
	//---- HDR�p�o�b�N�o�b�t�@���쐬
	SAFE_NEW(_pBackBufferHDR);
	if( _pBackBufferHDR == nullptr ) return false;
	
	if (!_pBackBufferHDR->createRenderTarget(windowSize._w, windowSize._h, GL_RGBA16F_ARB, GL_RGBA, GL_HALF_FLOAT_ARB, true)) {
		GM_ASSERT(false, "HDR�o�b�t�@�̍쐬�Ɏ��s���܂����B");
		return false;
	}

	//---- 1�t���[���O�o�b�N�o�b�t�@���쐬
	SAFE_NEW(_pPrevFrameBackBuffer);
	if (_pPrevFrameBackBuffer == nullptr) return false;

	if (!_pPrevFrameBackBuffer->createRenderTarget(windowSize._w, windowSize._h, GL_RGBA16F_ARB, GL_RGBA, GL_HALF_FLOAT_ARB, true)) {
		GM_ASSERT(false, "1�t���[���O�ۑ��p�o�b�t�@�̍쐬�Ɏ��s���܂����B");
		return false;
	}



	//---- �t�B���^�̍쐬
	{
		// ���[�V�����u���[
		if(!_motionBlurFilter.setup()) {
			return false;
		}
		// �u���[��
		if(!_bloomFilter.setup()) {
			return false;
		}
		// �g�[���}�b�s���O
		if(!_toneMappingFilter.setup()) {
			return false;
		}
		// �O���A�t�B���^
		if(!_glareFilter.setup()) {
			return false;
		}
		// �����Y�t���A
		if(!_lensFlareFilter.setup()) {
			return false;
		}
	}


	

	//glDisable(GL_CULL_FACE);	// �\���J�����O = OFF(����)
	//glDisable(GL_LIGHTING);		// �Œ�p�C�v���C���V�F�[�_�[������
	glEnable(GL_DEPTH_TEST);	// �y�o�b�t�@ON

	return true;
}

//-----------------------------------------------------------------------------
//	���
//-----------------------------------------------------------------------------
void SystemRender::cleanup()
{
	_lensFlareFilter.cleanup();
	_glareFilter.cleanup();
	_toneMappingFilter.cleanup();
	_bloomFilter.cleanup();
	_motionBlurFilter.cleanup();

	SAFE_DELETE(_pBackBuffer);
	SAFE_DELETE(_pBackBufferHDR);

	// �X�^�b�N���Ă������e�N�X�`�����폜
	vector<StackTexture*>::iterator	itr;
	for(itr=_copyTextureList.begin(); itr!=_copyTextureList.end();){
		StackTexture* pStock	= *itr;
		Texture*	  pTexture	= pStock->_pTexture;
		SAFE_DELETE(pTexture);
		SAFE_DELETE(pStock);
		itr = _copyTextureList.erase(itr);
	}
}

//-----------------------------------------------------------------------------
//! �`��̊J�n
//-----------------------------------------------------------------------------
void SystemRender::begin()
{
	//---- �V�F�[�_�̊J�n
	GmShader()->setPass(SystemShader::SHADER_TEXTURE);
	GmShader()->beginShader();
	//-------------------------------------------------------------
	// HDR�����_�����O�̂��߂�fp16�o�b�t�@���o�b�N�o�b�t�@�ɂ���
	//-------------------------------------------------------------
	setHDR(true);
	Texture* pCurrent = getBackBuffer();
	// �`����ݒ�
	setRenderTarget(pCurrent);

	//-------------------------------------------------------------
	// ��ʂ̃N���A
	//-------------------------------------------------------------
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);			// �y�o�b�t�@�̏������l
	/*glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);*/
	glDepthFunc(GL_LESS);		// �f�v�X���@�ݒ�
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	Size<s32> windowSize = GmWindow()->getResolution();

	glViewport(0,0,windowSize._w, windowSize._h);
}

//-----------------------------------------------------------------------------
//! �`��̏I��
//-----------------------------------------------------------------------------
void SystemRender::end()
{
	// �o�b�N�o�b�t�@�̃~�b�v�}�b�v�쐬
	generateMipMap();
	// LDR�o�b�t�@�ɃR�s�[
	switchToLDR();

	// �f�v�X����LDR�o�b�t�@�ɃR�s�[���Ă���(�t�B���^�̉e�����󂯂Ȃ����̗p)
//	copyTexture(_pBackBufferHDR, _pBackBuffer, Texture::TEX_DEPTH);

	getGLError();
	//-------------------------------------------------------------
	// �`��I���i�V�[���I���j
	//-------------------------------------------------------------
	//---- �V�F�[�_�̏I��
	GmShader()->endShader();
}

//-----------------------------------------------------------------------------
//! ��ʍX�V
//-----------------------------------------------------------------------------
void SystemRender::flip()
{

	// �f�t�H���g�̃t���[���o�b�t�@�ɂ���
	unBindFrame();

	drawBackBuffer(getBackBuffer());


	//glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	//glDisable(GL_ALPHA_TEST);
	
	// �o�b�N�o�b�t�@�X���b�v
	glutSwapBuffers();
}

//-----------------------------------------------------------------------------
//! �t���[���`��J�n
//-----------------------------------------------------------------------------
void SystemRender::bindFrame()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _frameInfoList[_currentFrame]->getFrameBuffer());	
}

//-----------------------------------------------------------------------------
//! ��O�̂��̂ɖ߂�
//-----------------------------------------------------------------------------
void SystemRender::bindPrevRender()
{
	// ���̃t���[���ԍ�����O�ɂ��ǂ�
	_currentFrame = _prevFrame;
	bindFrame();
}

//-----------------------------------------------------------------------------
//! �t���[���̕`��I��
//-----------------------------------------------------------------------------
void SystemRender::unBindFrame()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//-----------------------------------------------------------------------------
//! �e�N�X�`������ʂɕ`��
//! @param	[in]	pTexture		�`�悷��e�N�X�`��
//! @param	[in]	color			�f�B�t���[�Y�J���[
//! @param	[in]	emissiveColor	�G�~�b�V�u�J���[
//! @param	[in]	x				X���W
//! @param	[in]	y				Y���W
//! @param	[in]	scale			�X�P�[��(�f�t�H���g��1.0f)
//! @param	[in]	shaderPass		�V�F�[�_�̔ԍ�(�f�t�H���g��-1)
//! @param	[in]	mipLevel		�~�b�v�}�b�v�ԍ�(�f�t�H���g��0)
//! @param	[in]	isCopy			�R�s�[���ǂ���(�R�s�[�Ȃ�T�C�Y���~�b�v�}�b�v�łƂ��Ă��Ȃ�)
//! @param	[in]	isFrameBuffer	�t���[���o�b�t�@���ǂ���
//!	(�t���[���o�b�t�@�Ȃ�㉺�t���܂ɂ��Ȃ��ŕ`�悷��B
//! OpenGL��uv�͍������_�Ȃ̂Ńe�N�X�`���`��̂Ƃ��͏㉺�t���ŕ`�悵�Ȃ��Ƃ����Ȃ�����)
//-----------------------------------------------------------------------------
void SystemRender::putTexture(Texture* pTexture, f32 x, f32 y, Color& color, Color& emissiveColor, f32 scale, s32 shaderPass, s32 mipLevel, bool isCopy, bool isFrameBuffer)
{
	GLuint	texHandle = pTexture->getColorBuffer();
	if( texHandle == 0 ) {
		texHandle = pTexture->getTextureHandle();
	}
	s32 mip = 0;
	if(!isCopy) {
		mip = mipLevel;
	}
	Size<s32> textureSize = pTexture->getSize(mip); 
	putTexture(texHandle, x, y, textureSize, color, emissiveColor, scale, shaderPass, mipLevel, isFrameBuffer);
}

void SystemRender::putTexture(GLuint texHandle, f32 x, f32 y, Size<s32> texSize, Color& color, Color& emissiveColor, f32 scale, s32 shaderPass, s32 mipLevel, bool isFrameBuffer)
{
	Size<s32> screenSize  = GmWindow()->getResolution();

	f32 x1, y1, x2, y2;

	if(isFrameBuffer){
		x1 = (x						) * ( 2.0f / screenSize._w) - 1.0f;
		y1 = (y + texSize._h * scale) * (-2.0f / screenSize._h) + 1.0f;

		x2 = (x + texSize._w * scale) * ( 2.0f / screenSize._w) - 1.0f;
		y2 = (y						) * (-2.0f / screenSize._h) + 1.0f;
	}else{
		x1 = (x						) * ( 2.0f / screenSize._w) - 1.0f;
		y1 = (y						) * (-2.0f / screenSize._h) + 1.0f;

		x2 = (x + texSize._w * scale) * ( 2.0f / screenSize._w) - 1.0f;
		y2 = (y + texSize._h * scale) * (-2.0f / screenSize._h) + 1.0f;
	}

	static GLuint VB;
	static bool Initialized = false;
	if( !Initialized ) {
		glGenBuffers(1, &VB);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(FilterVertex) * 4, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		Initialized = true;
	}

	//-------------------------------------------------------------
	// ���_�̍쐬
	//-------------------------------------------------------------
	/*const Vertex	v[4] = {
		{ Float3( x1,  y1, 0.0f), color, Float2(0.0f, 0.0f) },
		{ Float3( x2,  y1, 0.0f), color, Float2(1.0f, 0.0f) },
		{ Float3( x1,  y2, 0.0f), color, Float2(0.0f, 1.0f) },
		{ Float3( x2,  y2, 0.0f), color, Float2(1.0f, 1.0f) },
	};*/

	const FilterVertex v[4] = {
		{ Float3( x1,  y1, 0.0f), color, emissiveColor, Float2(0.0f, 0.0f) },
		{ Float3( x2,  y1, 0.0f), color, emissiveColor, Float2(1.0f, 0.0f) },
		{ Float3( x1,  y2, 0.0f), color, emissiveColor, Float2(0.0f, 1.0f) },
		{ Float3( x2,  y2, 0.0f), color, emissiveColor, Float2(1.0f, 1.0f) },
	};

	if( shaderPass == -1 ) {
		//! PUT_TEXTURE�ɕύX
		GmShader()->changeShader(SystemShader::SHADER_PUT_TEXTURE);
		// �V�F�[�_�[�p�X���X�V
		shaderPass = SystemShader::SHADER_PUT_TEXTURE;
	}else{
		GmShader()->changeShader( (SystemShader::SHADER_MODE)shaderPass );
	}

	//---- �`��
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);

	

	//---- �e�N�X�`��
	GmShader()->bindTexture("BaseTexture", texHandle, 0);
	//---- �~�b�v�ԍ�
	GmShader()->setUniform1f("gMipLod", (GLfloat)mipLevel);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

	getGLError();

	// ���_�o�b�t�@�o�C���h
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	// �f�[�^�]��
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(FilterVertex) * 4, v);


	GmShader()->setAndEnableVertexAttribute("position"		, 3, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _position));
	GmShader()->setAndEnableVertexAttribute("uv"			, 2, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _uv));
	GmShader()->setAndEnableVertexAttribute("color"			, 4, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _diffuseColor));
	if( shaderPass == SystemShader::SHADER_PUT_TEXTURE ){
		GmShader()->setAndEnableVertexAttribute("emissiveColor"	, 4, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _emissiveColor));
	}

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	GmShader()->DisableVertexAttribute("position");
	GmShader()->DisableVertexAttribute("uv");
	GmShader()->DisableVertexAttribute("color");
	GmShader()->DisableVertexAttribute("emissiveColor");

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	GmShader()->unbindTexture();


	// ���ɖ߂�
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	GmShader()->beginPrevShader();
}

//-----------------------------------------------------------------------------
//! �o�b�N�o�b�t�@�e�N�X�`���ɃR�s�[
//! @param	[in]	pDest			�R�s�[��e�N�X�`��
//! @param	[in]	copyMipMax		�R�s�[����~�b�v�i��
//-----------------------------------------------------------------------------
void SystemRender::copyBackBuffer(Texture* pDest, s32 copyMipMax)
{
	// �`���ݒ�
	Texture* pSrc = _frameInfoList[_currentFrame];
	s32		 mip  = 0;
	do {
		setRenderTarget(pDest, Float2(0, 0), mip);
		getGLError();
		// ��ʂɃe�N�X�`���`��
		putTexture(pSrc, 0, 0, Color::WHITE, Color::WHITE, 1.0f, SystemShader::SHADER_FILTER, mip, true, true);
		// ����
		mip = min(mip + 1, copyMipMax);	// �ő�l�𒴂��Ȃ�
	} while (mip < copyMipMax);
	// �o�b�N�o�b�t�@�ɖ߂�
	setRenderTarget(pSrc, Float2(0, 0), 0);

}


//-----------------------------------------------------------------------------
//! �o�b�N�o�b�t�@���X�^�b�N�ɃZ�[�u
//! @param	[in]	name	���O
//-----------------------------------------------------------------------------
void SystemRender::saveBackBuffer(GM_CSTR name)
{

	//-------------------------------------------------------------
	//! �R�s�[��e�N�X�`���ƃR�s�[�ۑ��p�\���̂̍쐬
	//-------------------------------------------------------------
	Texture* pDest = nullptr;	// �e�N�X�`���`���
	bool	 isNew = false;		// �V�K�쐬���邩�ǂ����t���O
	// ���X�g����T��
	StackTexture*  pStock = getSame(name);
	// ���������Ȃ�
	if(pStock){
		// �e�N�X�`�����X�g�b�N�̂��̂ɂ���
		pDest = pStock->_pTexture;
	}else{
		// �V�����쐬����B
		SAFE_NEW(pDest);
		SAFE_NEW(pStock);
		// �V�K�쐬�t���OON
		isNew	= true;
		// �f�[�^��ݒ�
		pStock->_pTexture = pDest;
		// �R�s�[�T�C�Y�v�Z
		s32 copySize = min(strlen(name) + 1, sizeof(pStock->_name) - 1);
		memcpy(pStock->_name, name, copySize);
	}

	// �V�K�쐬����Ȃ�
	if( isNew ) {
		Size<s32>	windowSize = GmWindow()->getResolution();
		// HDR �Ȃ�
		if(isHDR()) {
			if (!pDest->createRenderTarget(windowSize._w, windowSize._h, GL_RGBA16F_ARB, GL_RGBA, GL_HALF_FLOAT_ARB, true)){
				return;
			}
		}else{
			if (!pDest->createRenderTarget(windowSize._w, windowSize._h, GL_RGBA, GL_RGBA, GL_FLOAT, true)){
				return;
			}
		}
	}

	//-------------------------------------------------------------
	//! �o�b�N�o�b�t�@����R�s�[��e�N�X�`���ɃR�s�[
	//-------------------------------------------------------------
	const s32	mipLevelMax = pDest->getMipLevelMax();
	// �o�b�N�o�b�t�@���R�s�[
	copyBackBuffer(pDest, mipLevelMax);


	//-------------------------------------------------------------
	//! �쐬�����R�s�[��ۑ�
	//-------------------------------------------------------------
	// �V�K�쐬�������̂Ȃ�
	if(isNew){
		// �X�g�b�N���Ă���
		_copyTextureList.push_back(pStock);
	}

}

//-----------------------------------------------------------------------------
//! �X�^�b�N�`��(�~�b�v�}�b�v�����ɕ\��)
//! @param	[in]	pTexture	�`��e�N�X�`��
//! @param	[in]	offset		�I�t�Z�b�g
//-----------------------------------------------------------------------------
void SystemRender::renderTexture(Texture* pTexture, Vector3& offset)
{
	const s32	mipLevelMax = pTexture->getMipLevelMax();
	s32			mip			= 0;
	f32			scale		= 0.2f;
	Vector3		position	= offset;

	// �ő�𑜓x�Ƃ̔䗦
	Size<f32> resScale = GmWindow()->getRateResMax();

	if (mipLevelMax <= 0) {
		putTexture(pTexture, position._x, position._y, Color::WHITE, Color::WHITE, scale, SystemShader::SHADER_FILTER, mip, false, true);
	}
	else{
		Size<s32>	texSize = pTexture->getSize(mip);
		f32			originalW = texSize._w * scale;

		// �~�b�v�}�b�v�t�ŕ`��
		do{
			f32 w = pTexture->getWidth(mip) * scale;
			putTexture(pTexture, position._x, position._y, Color::WHITE, Color::WHITE, scale, SystemShader::SHADER_FILTER, mip, false, true);
			// �ŏ��̈��݈̂���
			if (mip == 0) {
				position._x -= originalW;
			}
			mip = min(mip + 1, mipLevelMax);	// �ő�l�ȏ�ɂ��Ȃ�
			position._x += w;
			position._y  = texSize._h * scale;
		} while (mip < mipLevelMax);
	}
	

}
//-----------------------------------------------------------------------------
//! �X�^�b�N�������̂�`��
//-----------------------------------------------------------------------------
void SystemRender::renderStackTexture()
{
	//---- ���X�g�̂��̂�S�`��
	Vector3	offset = Vector3(0.0f);
	f32	scale  = 0.2f;
	vector<StackTexture*>::iterator	itr;
	for (itr = _copyTextureList.begin(); itr != _copyTextureList.end(); ++itr)
	{
		Texture* pTexture = (*itr)->_pTexture;
		//putTexture(pTexture, offset, 800.0f, Color::WHITE, scale, SystemShader::SHADER_FILTER);
		renderTexture(pTexture, offset);
		f32 w = pTexture->getWidth();
		offset._x += (w * scale);
	}

}

//-----------------------------------------------------------------------------
//! �~�b�v�}�b�v���쐬
//-----------------------------------------------------------------------------
void SystemRender::generateMipMap()
{
	Texture* current = getBackBuffer();
	glBindTexture(GL_TEXTURE_2D, current->getColorBuffer());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//-----------------------------------------------------------------------------
//! HDR���[�h��LDR���[�h�ɖ߂�
//-----------------------------------------------------------------------------
void SystemRender::switchToLDR()
{

	// HDR�o�b�t�@���g���Ă��Ȃ��ꍇ�A�������͂��ł�LDR�ɂȂ��Ă���ꍇ�͏������Ȃ�
	if( isHDR() == false ) return;

	//-------------------------------------------------------------
	// �g�[���}�b�s���O
	// �� LDR���[�h�߂�
	//-------------------------------------------------------------
	if (Global::filterEnable) {
		_bloomFilter.render();
		_glareFilter.render();
		_motionBlurFilter.render();
		_lensFlareFilter.render();
	}
	
	

	// �g�[���}�b�s���O�O�̃e�N�X�`����ۑ����Ă���
	saveBackBuffer("�g�[���}�b�s���O�O\0");
	
	// 1�t���[���O�̃o�b�t�@��ۑ�
	copyBackBuffer(_pPrevFrameBackBuffer, _pPrevFrameBackBuffer->getMipLevelMax());


	_toneMappingFilter.render();

	
	// �X�^�b�N���Ă������e�N�X�`���̕`��
	if( Global::debugMode ){
		renderStackTexture();
	}

	// �t�B���^�[�̉e�����󂯂Ȃ����̂�`�悷��
	GmScene()->drawNoFilter();

	
}


//-----------------------------------------------------------------------------
//! �����t���[���o�b�t�@�������Ă��邩�ǂ���
//!	@param	[in]	frame	���ׂ�t���[��
//! @param	[out]	number	���������t���[���̔ԍ�(������Ȃ����-1)
//!	@return true : �����Ă��� false : �����Ă��Ȃ�
//-----------------------------------------------------------------------------
bool SystemRender::haveSameFrame(Texture* frame, s32& number)
{
	// ���X�g����
	for( u32 i=0; i < _frameInfoList.size(); ++i ) {
		Texture* checkFrame = _frameInfoList[i];
		if( checkFrame == frame ) { 
			// ���������ԍ����L�^
			number = i;
			return true;
		}
	}

	// ������Ȃ������̂�-1
	number = -1;
	return false;	
}

//-----------------------------------------------------------------------------
//! �`���ݒ�
//! �`���ݒ�
//!	@param	[in]	pFrameBufferTexture	�t���[���o�b�t�@�e�N�X�`��
//!	@param	[in]	mipLevel			�������ރ~�b�v�}�b�v�ԍ�
//!	@param	[in]	renderSize			�������ރT�C�Y
//! @return true : ���� false : ���s
//-----------------------------------------------------------------------------
bool SystemRender::setRenderTarget(Texture* pFrameBufferTexture, Float2& offset, s32 mipLevel, Size<s32>* pRenderSize)
{
	s32 number = 0;

	// �O��̃t���[���ԍ��L�^
	_prevFrame    = _currentFrame;

	// �����Ă��Ȃ����
	if (!haveSameFrame(pFrameBufferTexture, number)) {
		// �V�����o�^
		_frameInfoList.push_back(pFrameBufferTexture);
		// ���̃t���[���̓��X�g�̃T�C�Y - 1
		_currentFrame = _frameInfoList.size() - 1;
	}else{
		_currentFrame = number;
	}

	bindFrame();

	getGLError();

	// �~�b�v�ԍ����ݒ肳��Ă�����
	if( mipLevel != -1 ) {
		// �~�b�v�ԍ��ݒ肷��
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pFrameBufferTexture->getColorBuffer(), mipLevel);
	}

	if(pRenderSize == nullptr) {
		Size<s32> renderTargetSize = pFrameBufferTexture->getSize(mipLevel);
		pRenderSize = &renderTargetSize;
	}
	
	

	glViewport((GLint)offset._x, (GLint)offset._y, (GLsizei)pRenderSize->_w, (GLsizei)pRenderSize->_h);
	return true;
}

//-----------------------------------------------------------------------------
//! �`�悳��邩�ǂ���(�J�����O)
//! @param	[in]	mySphere �`��I�u�W�F�N�g�̋�
//-----------------------------------------------------------------------------
bool SystemRender::isRender(Sphere& mySphere)
{
	Frustum camFrustum = GmCameraMan()->getCurrentCamera()->getViewFrustum();
	return !camFrustum.isHit(mySphere);
}


//-----------------------------------------------------------------------------
//! �X�^�b�N�e�N�X�`�����̓������O�̂��̂��擾
//! @param	[in]	name	���O
//!	@return	�����e�N�X�`�� nullptr �Ȃ�Ȃ�
//-----------------------------------------------------------------------------
StackTexture* SystemRender::getSame(GM_CSTR	name)
{
	//---- ���X�g�̒����猟������
	vector<StackTexture*>::iterator	itr;
	for(itr=_copyTextureList.begin(); itr!=_copyTextureList.end(); ++itr)
	{
		GM_CSTR pName = (*itr)->_name;
		// �������O�Ȃ�
		if(strcmp(name, pName) == 0){
			// �f�[�^��Ԃ�
			return *itr;
		}
	}
	// ������Ȃ����nullptr��Ԃ�
	return nullptr;
}
//=============================================================================
//	END OF FILE
//=============================================================================
