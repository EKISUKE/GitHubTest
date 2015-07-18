//-----------------------------------------------------------------------------
//!
//!	@file	gmTexture.h
//!	@brief	�e�N�X�`��
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	�e�N�X�`��
//-----------------------------------------------------------------------------
class Texture
{
public:

	//! �e�N�X�`���n���h���̎��
	enum TEX_HANDLE
	{
		TEX_STANDARD,	// �W���̃e�N�X�`��
		TEX_COLOR,		// �J���[�e�N�X�`��
		TEX_DEPTH,		// �f�v�X�e�N�X�`��

		TEX_MAX
	};

	// ��{�t�H�[�}�b�g
	enum FILE_FORMAT
	{
		FORMAT_JPG,
		FORMAT_BMP,
		FORMAT_GIF,
		FORMAT_TIFF,
		FORMAT_WMF,
		FORMAT_EMF,
		FORMAT_PNG,
		FORMAT_TGA,
		FORMAT_HDR,

		FORMAT_MAX,
		FORMAT_NONE
	};

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	Texture()
	: _textureHandle(0xffffffffUL)
	, _width(0)
	, _height(0)
	, _mipLevelMax(-1)
	, _format(GL_NONE)
	, _internalFormat(-1)
	, _colorBuffer(0)
	, _depthBuffer(0)
	, _frameBuffer(0)
	, _fileFormat(FORMAT_NONE)
	{
	}
	//! �f�X�g���N�^
	virtual ~Texture() { cleanup(); }

	//! �N���X���e���
	void	cleanup();

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! ��̃e�N�X�`�����쐬
	//! @param	[in]	w	�e�N�X�`���̕�
	//! @param	[in]	h	�e�N�X�`���̍���
	//! @retval	true	����
	//! @retval	false	���s
	bool		create(s32 w, s32 h);


	//! �摜�t�@�C����ǂݍ��݂Ɠ]��
	//! @param	[in]	filePath		�t�@�C����(��"a.bmp")
	//!	@param	[in]	buildMipMaps	�~�b�v�}�b�v���쐬���邩�ǂ���
	//!	@param	[in]	isClamp			�N�����v���邩�ǂ���
	//!	@param	[in]	target			�]����
	//!	@param	[in]	mipLevel		�~�b�v�}�b�v�i��
	//!	@parma	[in]	format			�F�̃t�H�[�}�b�g
	//!	@param	[in]	type			�P�v�f������̃T�C�Y
	bool		loadAndTexImage2D(GM_CSTR filePath, 
								  bool	  buildMipMaps   = false,
								  bool	  isClamp	     = false,
								  GLenum  target	     = GL_TEXTURE_2D,
								  s32	  mipLevel	     = 0,
								  GLenum  format	     = GL_RGB,
								  GLenum  type		     = GL_UNSIGNED_BYTE
								  );
								  

	//! �`��^�[�Q�b�g�̍쐬
	//!	@param	[in]	w					��
	//!	@param	[in]	h					����
	//!	@param	[in]	internalFormat		�ێ��t�H�[�}�b�g(GL�����łǂ�����)
	//!	@param	[in]	format				�s�N�Z���t�H�[�}�b�g
	//!	@param	[in]	type				�F�v�f��̃^�C�v
	//!	@param	[in]	buildMipMaps		�~�b�v�}�b�v���쐬���邩�ǂ���
	//!	@param	[in]	depthOnly			�f�v�X�o�b�t�@�̂ݍ쐬���邩�ǂ���
	bool		createRenderTarget(s32 w, s32 h, GLint internalForamt=GL_RGBA, GLenum format=GL_RGBA, GLenum type = GL_FLOAT, bool buildMipMaps=true, bool depthOnly = false);

private:

	//! TGA�t�@�C����ǂݍ���
	//! @param	[in]	filePath		�t�@�C����(��"a.bmp")
	//!	@param	[in]	bytePerPixel	�P�v�f�̃o�C�g��
	u8*			loadTGA(GM_CSTR filePath, /*u8*& image,*/ GLuint& bytePerPixel);

	//! GDI+�ŉ摜�t�@�C���ǂݍ���
	//! �y�Ή��摜�`���zBMP, JPEG, PNG, GIF, TIFF, WMF, EMF
	//! @param	[in]	filePath		�t�@�C����(��"a.bmp")
	//!	@param	[in]	bytePerPixel	�P�v�f�̃o�C�g��
	//!	@return �摜�f�[�^�z��
	u8*			loadByGdiplus(GM_CSTR filePath, GLuint bytePerPixel);

	//! HDR�t�@�C����ǂݍ���
	//! @param	[in]	filePath		�t�@�C����(��"a.bmp")
	//!	@param	[in]	bytePerPixel	�P�v�f�̃o�C�g��
	u16*		loadHDR(GM_CSTR filePath, GLuint& bytePerPixel);

	//! �摜�t�@�C����ǂݍ���
	//! �y�Ή��摜�`���zBMP, JPEG, PNG, GIF, TIFF, WMF, EMF
	//! @param	[in]	filePath		�t�@�C����(��"a.bmp")
	//!	@param	[in]	bytePerPixel	�P�v�f�̃o�C�g��
	//!	@param	[in]	target			�]����
	void*		load(GM_CSTR filePath, GLuint& bytePerPixel, GLenum target);
	
	//! �L���[�u�}�b�v���ǂ���
	//!	@param	[in]	target			�]����
	bool		isCubeMap(GLenum target);

public:

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! GL�e�N�X�`���n���h�����擾
	GLuint		getTextureHandle() const { return _textureHandle; }

	//! �e�N�X�`���^�C�v����n���h���擾
	GLuint		getTextureHandle(TEX_HANDLE handleType) const 
	{ 
		if( handleType == TEX_STANDARD ) {
			return getTextureHandle();
		}else if( handleType == TEX_COLOR ) {
			return getColorBuffer();
		}else {
			return getDepthBuffer();
		}
	}

	//! �J���[�o�b�t�@�擾
	GLuint		getColorBuffer() const { return _colorBuffer; }
	//! �f�v�X�o�b�t�@�擾
	GLuint		getDepthBuffer() const { return _depthBuffer; } 
	//! �t���[���o�b�t�@�n���h���擾
	GLuint		getFrameBuffer() const { return _frameBuffer; }
	//! �T�C�Y�擾
	Size<s32>	getSize(s32 mipLevel = 0)	const { return Size<s32>(getWidth(mipLevel), getHeight(mipLevel)); }
	//! ���擾
	s32			getWidth(s32 mipLevel = 0)	const { 

		f32 mipPow = powf(2.0f, (f32)mipLevel);
		f32 width  = max(_width/mipPow, 1.0f);
		return (s32)width; 
	}
	//! �����擾
	s32			getHeight(s32 mipLevel = 0)	const {

		f32 mipPow = powf(2.0f, (f32)mipLevel);
		f32 height  = max(_height/mipPow, 1.0f);
		return (s32)height; 
	}

	//! �~�b�v�}�b�v�ő吔�擾
	s32		getMipLevelMax() const { return _mipLevelMax; }

	//! �e�N�X�`����GL�ɐݒ�
	//!	@param	[in]	pTexture	�ݒ肷��e�N�X�`��
	//!	@param	[in]	texNum		�ݒ肷��e�N�X�`���ԍ�(GL_TEXTURE0 + texNum)
	static void set(Texture* pTexture, GLuint texNum = 0);
	
private:
	//! �t�@�C���p�X����t�H�[�}�b�g����
	//! @param	[in]	filePath		�t�@�C����(��"a.bmp")
	bool		checkFileFormat(GM_CSTR filePath);


	//! �t���[�g�ɃR���o�[�g
	//!	@param	[in]	pImage			�ϊ���
	//!	@param	[in]	bytePerPixel	�P�v�f�̃o�C�g��
	f32*		IntToFloat(u8* pImage, GLuint bytePerPixel);

	//! float ���� half float�ɕϊ�
	//!	@param	[in]	pImage			�ϊ���
	//!	@param	[in]	bytePerPixel	�P�v�f�̃o�C�g��
	u16*		FloatToHalf(f32* pImage, GLuint bytePerPixel);

	//@}
private:
	s32			_width;				//!< ��
	s32			_height;			//!< ����
	s32			_mipLevelMax;		//!< �~�b�v���x����
	GLenum		_format;			//!< �e�N�X�`���t�H�[�}�b�g
	GLint		_internalFormat;	//!< �ێ��t�H�[�}�b�g(GL�����łǂ�����)
	GLuint		_textureHandle;		//!< OpenGL�e�N�X�`���n���h��
	GLuint		_colorBuffer;		//!< �J���[�o�b�t�@
	GLuint		_depthBuffer;		//!< �f�v�X�o�b�t�@
	GLuint		_frameBuffer;		//!< �t���[���o�b�t�@
	FILE_FORMAT	_fileFormat;		//!< �t�@�C���t�H�[�}�b�g
};
//=============================================================================
//	END OF FILE
//=============================================================================