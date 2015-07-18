//-----------------------------------------------------------------------------
//!
//!	@file	gmSystemRender.h
//!	@brief	�`��Ǘ��V�X�e��
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//! �e�N�X�`���X�^�b�N�p
struct StackTexture{
public:
	Texture*	_pTexture;		//!< �X�g�b�N���ꂽ�e�N�X�`��
	GM_CHAR		_name[256];		//!< ���O
};

//-----------------------------------------------------------------------------
//!	�`��Ǘ�
//-----------------------------------------------------------------------------
class SystemRender
{
private:
	//-------------------------------------------------------------
	//!	@name	������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	SystemRender();

	//! �R�s�[�R���X�g���N�^
	SystemRender(SystemRender& src) {}

	//! �f�X�g���N�^
	virtual					~SystemRender();

public:
	//! ������
	bool					init();
	
	//! ���
	void					cleanup();

	//@}
	//-------------------------------------------------------------
	//!	@name	����
	//-------------------------------------------------------------
	//@{

	//! �`��̊J�n
	void					begin();

	//! �`��̏I��
	void					end();

	//! ��ʍX�V
	void					flip();

	//! �t���[���`��J�n
	void					bindFrame();
	
	//! ��O�̂��̂ɖ߂�
	void					bindPrevRender();

private:
	//! �t���[���̕`��I��
	void					unBindFrame();

public:
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
	//! @param	[in]	isFrameBuffer	�t���[���o�b�t�@���ǂ���(�t���[���o�b�t�@�Ȃ�㉺�t���܂ɂ��Ȃ��ŕ`�悷��BOpenGL��uv�͍������_�Ȃ̂Ńe�N�X�`���`��̂Ƃ��͏㉺�t���ŕ`�悵�Ȃ��Ƃ����Ȃ�����)
	void					putTexture(Texture* pTexture, f32 x, f32 y, Color& color = Color::WHITE, Color& emissiveColor = Color::NONE, f32 scale = 1.0f, s32 shaderPass = -1, s32 mipLevel = 0, bool isCopy = false, bool isFrameBuffer = false);

	void					putTexture(GLuint	texHandle, f32 x, f32 y, Size<s32> texSize, Color& color = Color::WHITE, Color& emissiveColor = Color::NONE, f32 scale = 1.0f, s32 shaderPass = -1, s32 mipLevel = 0, bool isFrameBuffer = false);

	//! �o�b�N�o�b�t�@�e�N�X�`���ɃR�s�[
	//! @param	[in]	pDest			�R�s�[��e�N�X�`��
	//! @param	[in]	copyMipMax		�R�s�[����~�b�v�i��(0�Ȃ�~�b�v�̓R�s�[����Ȃ�)
	void					copyBackBuffer(Texture* pDest, s32 copyMipMax = 0);

	//! �o�b�N�o�b�t�@���X�^�b�N�ɃZ�[�u
	//! @param	[in]	name	���O
	void					saveBackBuffer(GM_CSTR name);

	//! �X�^�b�N�`��(�~�b�v�}�b�v�����ɕ\��)
	//! @param	[in]	pTexture	�`��e�N�X�`��
	//! @param	[in]	offset		�I�t�Z�b�g
	void					renderTexture(Texture* pTexture, Vector3& offset);
	//! �X�^�b�N�������̂�`��
	void					renderStackTexture();

	//! �~�b�v�}�b�v���쐬
	void					generateMipMap();
	
	//! HDR���[�h��LDR���[�h�ɖ߂�
	void					switchToLDR();

	//@}
	//-------------------------------------------------------------
	//!	@name	�擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �o�b�N�o�b�t�@�̎擾
	Texture*				getBackBuffer()			 const { return (_isHDR) ? _pBackBufferHDR : _pBackBuffer; }
	
	//! �P�t���[���O�̃o�b�t�@���擾
	Texture*				getPrevFrameBackBuffer() const { return _pPrevFrameBackBuffer; }


	//! �����t���[���o�b�t�@�������Ă��邩�ǂ���
	//!	@param	[in]	frame	���ׂ�t���[��
	//! @param	[out]	number	���������t���[���̔ԍ�(������Ȃ����-1)
	//!	@return true : �����Ă��� false : �����Ă��Ȃ�
	bool					haveSameFrame(Texture* frame, s32& number);

	//! HDR���[�h�̐؂�ւ�
	void					setHDR(bool enable) { _isHDR = enable; }

	////! �`���ݒ�
	////!	@param	[in]	color	�`���e�N�X�`���n���h��(�J���[)
	////!	@param	[in]	depth	�`���e�N�X�`���n���h��(�f�v�X)
	////! @return true : ���� false : ���s
	//bool					setRenderTarget(const GLuint& color, const GLuint& depth);

	//! �`���ݒ�
	//!	@param	[in]	pFrameBufferTexture	�t���[���o�b�t�@�e�N�X�`��
	//!	@param	[in]	mipLevel			�������ރ~�b�v�}�b�v�ԍ�
	//!	@param	[in]	pRenderSize			�������ރT�C�Y
	//! @return true : ���� false : ���s
	bool					setRenderTarget(Texture* pFrameBufferTexture, Float2& offset = Float2(0, 0), s32 mipLevel = -1, Size<s32>* pRenderSize = nullptr);

	
	//! HDR���[�h���擾
	bool					isHDR() const { return _isHDR; }

	//!	�C���X�^���X���擾
	static SystemRender*	getInstance()
	{
		static SystemRender	_myInstance;
		return &_myInstance;
	}

	//! �`�悳��邩�ǂ���(�J�����O)
	//! @param	[in]	mySphere �`��I�u�W�F�N�g�̋�
	bool					isRender(Sphere& mySphere);

private:
	//! �X�g�b�N�e�N�X�`�����̓������O�̂��̂��擾
	//! @param	[in]	name	���O
	//!	@return	�����e�N�X�`��	nullptr �Ȃ�Ȃ�
	StackTexture*			getSame(GM_CSTR	name);
	

	//@}
private:
	vector<Texture*>		_frameInfoList;			//!< �t���[���o�b�t�@���̃��X�g
	s32						_currentFrame;			//!< ���̃t���[���̔z��ԍ�
	s32						_prevFrame;				//!< ��O�̃t���[���̔z��ԍ�
	//GLuint					_currentBuffer[2];	//!< ���̃o�b�t�@    (1:Color 2:Depth)
	//GLuint					_prevBuffer[2];		//!< ��O�̃o�b�t�@(1:Color 2:Depth)
	//GLuint					_frameBuffer;		//!< �t���[���o�b�t�@
	Texture*				_pBackBuffer;			//!< �o�b�N�o�b�t�@
	Texture*				_pBackBufferHDR;		//!< HDR�o�b�N�o�b�t�@(fp16�`��)
	Texture*				_pPrevFrameBackBuffer;	//!< 1�t���[���O�̃o�b�t�@
	bool					_isHDR;					//!< ���݂�HDR���[�h true:fp16 false:RGBA8

	TaskFilterMotionBlur	_motionBlurFilter;		//!< ���[�V�����u���[
	TaskFilterBloom			_bloomFilter;			//!< �u���[��
	TaskFilterToneMapping	_toneMappingFilter;		//!< �g�[���}�b�s���O
	TaskFilterGlare			_glareFilter;			//!< �O���A
	TaskFilterLensFlare		_lensFlareFilter;		//!< �����Y�t���A

	vector<StackTexture*>	_copyTextureList;		//!< �R�s�[�p�e�N�X�`��(�r���ŕۑ����Ă��̂Ƃ��̉�ʒm��p)
};

//-----------------------------------------------------------------------------
//	�A�N�Z�X�C���^�[�t�F�C�X
//-----------------------------------------------------------------------------
__forceinline
SystemRender*	GmRender()
{
	return SystemRender::getInstance();
}

//=============================================================================
//	END OF FILE
//=============================================================================
