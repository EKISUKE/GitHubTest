//-----------------------------------------------------------------------------
//!
//!	@file	gmTexture.cpp
//!	@brief	�e�N�X�`��
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//=============================================================================
//	�e�N�X�`���̎���
//=============================================================================

//-----------------------------------------------------------------------------
//! �N���X���e���
//-----------------------------------------------------------------------------
void Texture::cleanup()
{
	if( _textureHandle != 0xffffffffUL ) {
		glDeleteTextures(1, &_textureHandle);
		_textureHandle = 0xffffffffUL;
	}
}

//-----------------------------------------------------------------------------
//! ��̃e�N�X�`�����쐬
//! @param	[in]	w	�e�N�X�`���̕�
//! @param	[in]	h	�e�N�X�`���̍���
//! @retval	true	����
//! @retval	false	���s
//-----------------------------------------------------------------------------
bool Texture::create(s32 w, s32 h)
{
	//-------------------------------------------------------------
	// �@ �e�N�X�`���𐶐�
	//-------------------------------------------------------------
	glGenTextures(1, &_textureHandle);

	//-------------------------------------------------------------
	// �A �e�N�X�`����GPU�ɐݒ�
	//-------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, _textureHandle);

	//-------------------------------------------------------------
	// �B �摜�C���[�W��VRAM�֓]��
	//-------------------------------------------------------------

	//---- �摜�ɕK�v�ȃ��������m��
	u8*	pImage = new u8 [w * h * 4];

	for( s32 y=0; y<h; y++ ) {
		for( s32 x=0; x<w; x++ ) {
			u8* p = &pImage[(y * w + x) * 4];

			p[0] = rand();		// R
			p[1] = rand();		// G
			p[2] = rand();		// B
			p[3] = rand();		// A
		}
	}

	_internalFormat = 4;
	//---- �]��
	glTexImage2D(
				 GL_TEXTURE_2D,
				 0,					// �~�b�v�}�b�v�i��
				 _internalFormat,	// �R���|�[�l���g�� 3:RGB 4:RGBA
				 w,					// ��
				 h,					// ����
				 0,					// �{�[�_�[�J���[ 1:���� 0:�Ȃ�
				 _format,			// [�]����] �R���|�[�l���g
				 GL_UNSIGNED_BYTE,	// [�]����] ��1�v�f������̃T�C�Y
				 &pImage[0]);		// [�]����] �C���[�W�̃������̐擪

	//---- ���[�N�����������
	delete [] pImage;

	return true;
}

//-----------------------------------------------------------------------------
//! TGA�t�@�C����ǂݍ���
//! @param	[in]	filePath		�t�@�C����(��"a.bmp")
//!	@param	[in]	bytePerPixel	�P�v�f�̃o�C�g��
//-----------------------------------------------------------------------------
u8* Texture::loadTGA(GM_CSTR filePath, GLuint& bytePerPixel)
{
	// tga�̓ǂݍ���
	FILE *fp;

	GLubyte header[18];

	// �t�@�C�����J��
	if( fopen_s(&fp, filePath, "rb") != 0 )
	{
		return false;
	}

	// �w�b�_���̓ǂݍ���
	fread(header, 1, sizeof(header), fp);

	f32 w,h;
	// ���ƍ��������߂�
	w = header[13] * 256 + header[12];
	h = header[15] * 256 + header[14];
		
	// �r�b�g�̐[��
	GLuint bpp = header[16];
		
	// ���̃r�b�g�̐[���ɂ���ăt�H�[�}�b�g����
	if( bpp == 24 )
	{
		_format = GL_RGB;
	}else if( bpp == 32 ){
		_format = GL_RGBA;
	}

	// 1�s�N�Z���̃o�C�g�������߂�
	bytePerPixel = bpp/8;

	// �摜�T�C�Y���v�Z
	s32 imageSize = w * h * bytePerPixel;
	// �T�C�Y��ۑ�
	_width  = w;
	_height = h;

	// �����[�h����摜�̃������̉𑜓x��ύX/�ݒ�(���̈ʒu�ɔC�ӂɋL�q���ĉ�����)
	u8* pImage = new u8 [imageSize];

	// �e�N�Z���f�[�^����C�ɓǂݎ��
	fread(pImage, 1, imageSize, fp);

	// BGR(A)��RGB(A)�ɃR���o�[�g
	for( s32 i=0; i<imageSize; i+= bytePerPixel)
	{
		u8 temp		= pImage[i];
		pImage[i]	= pImage[i+2];
		pImage[i+2] = temp;
	}

	// ���ԕύX
	for( s32 i=0; i<imageSize; i++ )
	{
		s32 partIndex	  = imageSize - 1 - i;
		u8 temp			  = pImage[i];
		pImage[i]		  = pImage[partIndex];
		pImage[partIndex] = temp;
	}

	// �t�@�C�������
	fclose(fp);

	return &pImage[0];
	//return (u32*)pImage;
}


//-----------------------------------------------------------------------------
//! GDI+�ŉ摜�t�@�C���ǂݍ���
//! �y�Ή��摜�`���zBMP, JPEG, PNG, GIF, TIFF, WMF, EMF
//! @param	[in]	filePath		�t�@�C����(��"a.bmp")
//!	@param	[in]	bytePerPixel	�P�v�f�̃o�C�g��
//!	@return �摜�f�[�^�z��	nullptr�Ȃ玸�s
//-----------------------------------------------------------------------------
u8* Texture::loadByGdiplus(GM_CSTR filePath, GLuint bytePerPixel)
{
	// �����R�[�h�����C�h������ɕϊ�
	// �y���Ӂz�{���͂��̉ӏ��͕�����o�b�t�@���̍l���̂ق���
	// ������I�[�R�[�h������������Z�L���A�ȑΉ����D�܂���
	wchar_t		path[MAX_PATH];
	size_t		pathLength = 0;
	// ���P�[���w��
	setlocale(LC_ALL, "japanese");

	if (mbstowcs_s(&pathLength,     // [out]    �ϊ����ꂽ������
		&path[0],					// [out]    �ϊ����ꂽ���C�h��������i�[����o�b�t�@�̃A�h���X(�ϊ���)
		MAX_PATH,					// [in]     �o�͑��̃o�b�t�@�̃T�C�Y(�P��:������)
		filePath,					// [in]     �}���`�o�C�g������̃A�h���X(�ϊ���)
		_TRUNCATE) != 0) {			// [in]     �o�͐�Ɋi�[���郏�C�h�����̍ő吔
		return nullptr;
	}

	bool	result = false;
	u8*		pImage = nullptr;

	// GDI+�I�u�W�F�N�g(�摜�W�J�ɕK�v)
	Gdiplus::GdiplusStartupInput	gdiplusStartupInput;
	ULONG_PTR						gdiplusToken;

	//---- GDI+�̏����ݒ�
	if (Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr) != Gdiplus::Ok) {
		return false;
	}

	//---- �摜�t�@�C�����J��
	//	  �y�΍R�摜�`���z BMP, JPEG, PNG, GIF, TIFF, WMF, EMF
	Gdiplus::Bitmap*		pBitmap = Gdiplus::Bitmap::FromFile(path);
	if (pBitmap && pBitmap->GetLastStatus() == Gdiplus::Ok) {
		//---- �摜�T�C�Y���̗̈�m��
		_width  = pBitmap->GetWidth();
		_height = pBitmap->GetHeight();


		// �����[�h����摜�̃������̉𑜓x��ύX/�ݒ�(���̈ʒu�ɔC�ӂɋL�q���ĉ�����)
		pImage = new u8[_width * _height * bytePerPixel];

		// �������m�ۂł��Ă����
		if (pImage != nullptr) {
			//---- �摜�C���[�W�̓ǂݍ���
			for (s32 y = 0; y<_height; y++) {
				for (s32 x = 0; x<_width; x++) {
					Gdiplus::Color	srcColor;
					pBitmap->GetPixel(x, y, &srcColor);

					u8 r = srcColor.GetR();
					u8 g = srcColor.GetG();
					u8 b = srcColor.GetB();
					u8 a = srcColor.GetA();

					// ���s�N�Z���ւ̕`��(���̈ʒu�ɔC�ӂɋL�q���Ă�������)
					// setPixel(x, y, r, g, b, a);

					// x, y�Ԗڂ̃s�N�Z���̃A�h���X���擾
					u8	*pDst = pImage + (y * _width + x) * bytePerPixel;
					// ���̃s�N�Z���ɐF��ݒ�
					pDst[0] = r;		// Red
					pDst[1] = g;		// Green
					pDst[2] = b;		// Blue
					if (bytePerPixel == 4) {	// RGBA�̂Ƃ��̂ݏ�������
						pDst[3] = a;		// Alpha
					}
				}
			}
		}

		delete pBitmap;

		//---- GDI+�̊J��
		Gdiplus::GdiplusShutdown(gdiplusToken);

	}else{
		//---- GDI+�̊J��
		Gdiplus::GdiplusShutdown(gdiplusToken);
		return nullptr;
	}

	return &pImage[0];
	
}

//-----------------------------------------------------------------------------
//! HDR�t�@�C����ǂݍ���
//! @param	[in]	filePath		�t�@�C����(��"a.bmp")
//!	@param	[in]	bytePerPixel	�P�v�f�̃o�C�g��
//-----------------------------------------------------------------------------
u16* Texture::loadHDR(GM_CSTR filePath, GLuint& bytePerPixel)
{
	u16* pImage;
	//u32* pImage;

	HDRTexReader	hdrReader;

	pImage = (u16*)hdrReader.loadHDR(filePath, _width, _height);
	if( pImage ==nullptr )
	{
		return nullptr;
	}


	return pImage;
}

//-----------------------------------------------------------------------------
//! �L���[�u�}�b�v���ǂ���
//!	@param	[in]	target			�]����
//-----------------------------------------------------------------------------
bool Texture::isCubeMap(GLenum target)
{
	static const GLenum checkList[] = {
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	};

	// ��ł���v����΃L���[�u�}�b�v�ł���
	static const s32 count = sizeof(checkList) / sizeof(GLenum);
	for(s32 i=0; i<count; ++i){
		if(checkList[i] == target){
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
//! �摜�t�@�C����ǂݍ���
//! �y�Ή��摜�`���zBMP, JPEG, PNG, GIF, TIFF, WMF, EMF,TGA
//! @param	[in]	fileName		�t�@�C����(��"a.bmp")
//!	@param	[in]	bytePerPixel	�P�v�f�̃o�C�g��
//!	@param	[in]	target			�]����
//-----------------------------------------------------------------------------
void*  Texture::load(GM_CSTR filePath, GLuint& bytePerPixel, GLenum target)
{
	//-------------------------------------------------------------
	// �@ �e�N�X�`���𐶐�
	//-------------------------------------------------------------
	glGenTextures(1, &_textureHandle);

	//-------------------------------------------------------------
	// �A �e�N�X�`����GPU�ɐݒ�
	//-------------------------------------------------------------
	if(!isCubeMap(target)) {
		glBindTexture(target, _textureHandle);
	}

	//-------------------------------------------------------------
	// �B �摜�̓ǂݍ���
	//-------------------------------------------------------------

	void* pImage = nullptr;
	

	//---- �t�@�C���t�H�[�}�b�g�ɂ���ēǂݍ��݊֐����Ăѕ���
	switch(_fileFormat)
	{
	case FORMAT_TGA:	// TGA�ǂݍ���
		pImage = loadTGA(filePath, bytePerPixel);
		break;
	case FORMAT_HDR:
		pImage		 = loadHDR(filePath, bytePerPixel);
		bytePerPixel = 3;	// �ϊ����I�������RGB
		break;
	default:
		pImage = loadByGdiplus(filePath, bytePerPixel);
		break;
	}

	if(pImage == nullptr){
		return nullptr;
	}


	return pImage;

}


//-----------------------------------------------------------------------------
//! �摜�t�@�C����ǂݍ��݂Ɠ]��
//! @param	[in]	filePath		�t�@�C����(��"a.bmp")
//!	@param	[in]	target			�]����
//!	@param	[in]	buildMipMaps	�~�b�v�}�b�v���쐬���邩�ǂ���
//!	@param	[in]	mipLevel		�~�b�v�}�b�v�i��
//!	@parma	[in]	format			�F�̃t�H�[�}�b�g
//!	@param	[in]	type			�P�v�f������̃T�C�Y
//-----------------------------------------------------------------------------
bool Texture::loadAndTexImage2D(GM_CSTR filePath, 
								bool	buildMipMaps,
								bool	isClamp,
								GLenum  target,
								s32		mipLevel,
								GLenum  format,
								GLenum  type)
{

	// �t�H�[�}�b�g��ݒ�(�f�t�H���g���)
	_format = format;
	// �t�@�C���t�H�[�}�b�g�ݒ�
	if( checkFileFormat(filePath) )
	{
		GLuint bytePerPixel;
		if (_format == GL_RGBA){
			bytePerPixel = 4;
		}else{
			bytePerPixel = 3;
		}


		// �t�H�[�}�b�g��HDR�łȂ���
		if (_fileFormat != FORMAT_HDR) {
			// �����ɂ���
			_internalFormat = _format;
		}else{
			// �t�H�[�}�b�g��RGB�ɂ��ǂ��B
			_format			= GL_RGB;
			_internalFormat = GL_RGB16F;
			type			= GL_HALF_FLOAT;
			//type			= GL_UNSIGNED_BYTE;
		}


		void*	pImage = load(filePath, bytePerPixel, target);
	

		/*if(type == GL_FLOAT){
			pImage = IntToFloat((u8*)pImage, bytePerPixel);
		}else if(type == GL_HALF_FLOAT){
			pImage = IntToFloat((u8*)pImage, bytePerPixel);
			pImage = FloatToHalf((f32*)pImage, bytePerPixel);
		}*/


		

		if( pImage != nullptr ) {
			if (buildMipMaps) {
				gluBuild2DMipmaps(GL_TEXTURE_2D, _internalFormat, _width, _height, _format, type, pImage);
				glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);					// �g�厞�̃t�B���^(GL_LINEAR_MIPMAP_LINEAR�̓T�|�[�g���Ă��Ȃ�)
				glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// �k�����̃t�B���^
				f32	maxSize = (f32)(max(_width, _height));
				_mipLevelMax = (s32)(log((f32)maxSize) / log(2.0f));

			}else if( target == GL_TEXTURE_1D ){
				glTexImage1D(
					target,				// �]����
					0,					// �~�b�v�}�b�v�i��
					_internalFormat,	// GL�����łǂ�����
					_width,				// ��
					0,					// �{�[�_�[�J���[ 1:���� 0:�Ȃ�
					_format,			// [�]����] �R���|�[�l���g
					type,				// [�]����] ��1�v�f������̃T�C�Y	
					pImage);			// [�]����] �C���[�W�̃������̐擪

				glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			}else{

				//---- �]��
				glTexImage2D(
					target,				// �]����
					0,					// �~�b�v�}�b�v�i��
					_internalFormat,	// �R���|�[�l���g�� 3:RGB 4:RGBA
					_width,				// ��
					_height,			// ����
					0,					// �{�[�_�[�J���[ 1:���� 0:�Ȃ�
					_format,			// [�]����] �R���|�[�l���g
					type,				// [�]����] ��1�v�f������̃T�C�Y	
					pImage);			// [�]����] �C���[�W�̃������̐擪

				if(!isCubeMap(target)) {
					glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
			}

			// �N�����v�t���O��ON�Ȃ�N�����v
			if( isClamp ) 
			{
				glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			// �ꎞ�I�u�W�F�N�g���
			delete [] pImage;
			pImage = nullptr;

			if(!isCubeMap(target)) {
				glBindTexture(target, 0);
			}

			return true;
		}else{
			glBindTexture(target, 0);
			glDeleteTextures(1, &_textureHandle);
		}

	}	// checkFileFormat()


	glBindTexture(target, 0);
	// �ǂݍ��ݎ��s�t�@�C�������o�͂���B
	MessageBoxA(nullptr, filePath, "�t�@�C����", MB_OK);
	return false;
}
//-----------------------------------------------------------------------------
//! �`��^�[�Q�b�g�̍쐬
//!	@param	[in]	w					��
//!	@param	[in]	h					����
//!	@param	[in]	internalFormat		�ێ��t�H�[�}�b�g(GL�����łǂ�����)
//!	@param	[in]	format				�s�N�Z���t�H�[�}�b�g
//!	@param	[in]	type				�F�v�f��̃^�C�v
//!	@param	[in]	buildMipMaps		�~�b�v�}�b�v���쐬���邩�ǂ���
//!	@param	[in]	depthOnly			�f�v�X�o�b�t�@�̂ݍ쐬���邩�ǂ���
//-----------------------------------------------------------------------------
bool Texture::createRenderTarget(s32 w, s32 h, GLint internalForamt, GLenum format,  GLenum type, bool buildMipMaps, bool depthOnly)
{

	//----------------------------------------------------------
	// �J���[�o�b�t�@���쐬�A�o�C���h���ē]��
	//----------------------------------------------------------
	// �f�v�X�݂̂Ȃ�쐬���Ȃ�
	if(!depthOnly) {
		glGenTextures(1, &_colorBuffer);
		glBindTexture(GL_TEXTURE_2D, _colorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, internalForamt, w, h, 0, format, type, nullptr);
	
		// �~�b�v�}�b�v���쐬����Ȃ�
		if( buildMipMaps ) {
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);				// �g�厞�̃t�B���^(GL_LINEAR_MIPMAP_LINEAR�̓T�|�[�g���Ă��Ȃ�)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// �k�����̃t�B���^
			getGLError();
			f32	maxSize  = (f32)( max(w, h) );
			_mipLevelMax = (s32)( log((f32)maxSize) / log(2.0f) );
		}else{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	//----------------------------------------------------------
	// �f�v�X�o�b�t�@���쐬�A�o�C���h���ē]��
	//----------------------------------------------------------
	glGenTextures(1, &_depthBuffer);
	glBindTexture(GL_TEXTURE_2D, _depthBuffer);
	//glTexImage2D(GL_TEXTURE_2D, mipLevel, GL_DEPTH_COMPONENT,		   w,		   h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexImage2D(GL_TEXTURE_2D,		   0, GL_DEPTH_COMPONENT, (GLsizei)w, (GLsizei)h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);								// [�]����] �C���[�W�̃������̐擪

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	getGLError();

	// �e�N�X�`�����
	glBindTexture(GL_TEXTURE_2D, 0);

	// �t���[���o�b�t�@���쐬���e�N�X�`�����o�C���h
	glGenFramebuffers(1, &_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
	// �f�v�X�݂̂Ȃ�o�C���h���Ȃ�
	if(!depthOnly){
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorBuffer, 0);
	}
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT , GL_TEXTURE_2D, _depthBuffer, 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
		return false;

	// �f�t�H���g�̃����_�[�^�[�Q�b�g�ɖ߂�
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//---- �L�^���Ă���
	_width  = w;
	_height = h;
	_format = format;

	return true;
}

//-----------------------------------------------------------------------------
//! �e�N�X�`����GL�ɐݒ�
//!	@param	[in]	pTexture			�ݒ肷��e�N�X�`��
//!	@param	[in]	texNum		�ݒ肷��e�N�X�`���ԍ�(GL_TEXTURE0 + texNum)
//-----------------------------------------------------------------------------
void Texture::set(Texture* pTexture, GLuint texNum)
{
	GM_ASSERT(texNum <= 31, "�e�N�X�`���̗L���ԍ����z���Ă��܂�");
	if( pTexture ) {	//---- �e�N�X�`��ON
		glActiveTexture(GL_TEXTURE0 + texNum);
		glEnable(GL_TEXTURE_2D);
		GLuint	textureHandle = pTexture->getTextureHandle();
		glBindTexture(GL_TEXTURE_2D, textureHandle);

		// �e�N�X�`���̕i���ݒ�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {				//---- �e�N�X�`��OFF
		glActiveTexture(GL_TEXTURE0 + texNum);
		glDisable(GL_TEXTURE_2D);
	}
}


//-----------------------------------------------------------------------------
//! �t�@�C���p�X����t�H�[�}�b�g����
//! @param	[in]	filePath		�t�@�C����(��"a.bmp")
//-----------------------------------------------------------------------------
bool Texture::checkFileFormat(GM_CSTR filePath)
{
	static const char *extensions[] =
	{
		".jpg",
		".bmp",
		".gif",
		".tif",
		".wmf",
		".emf",
		".png",
		".tga",
		".hdr",
	};

	//---- �g���q����t�H�[�}�b�g����
	string newPath = filePath;
	// .�̈ʒu�擾
	s32	   extPos = newPath.rfind(".");
	if (extPos > 0){
		// ���̈ʒu�̃A�h���X�擾
		char *tail = &newPath[extPos];
		for (u32 i=0; i<FORMAT_MAX; ++i) {
			if (strncmp(tail, extensions[i], 4) == 0) {
				if (i >= FORMAT_PNG /*&& i < FORMAT_HDR*/) {
					_format = GL_RGBA;
				/*}else if( i== FORMAT_HDR ) {
					_format = GL_RGBA32F;*/
				}
				_fileFormat = (FILE_FORMAT)i;
				break;
			}
		}
	}

	if(_format == FORMAT_NONE) {
		MessageBoxA(nullptr, "�Ή����Ă��Ȃ��t�H�[�}�b�g�ł��B", "�e�N�X�`���ǂݍ��݃G���[", MB_OK);
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
//! �t���[�g�ɃR���o�[�g
//!	@param	[in]	pImage			�ϊ���
//!	@param	[in]	bytePerPixel	�P�v�f�̃o�C�g��
//-----------------------------------------------------------------------------
f32* Texture::IntToFloat(u8* pImage, GLuint bytePerPixel)
{

	// �摜�T�C�Y���v�Z
	s32 imageSize = _width * _height * bytePerPixel;
	f32* result	  = new f32[imageSize];
	
	for (s32 i = 0; i<imageSize; ++i) {
		result[i] = common::ByteToFloat(pImage[i]);
	}

	// �O�̃f�[�^�͍폜����
	SAFE_DELETE(pImage);

	return result;
}

//-----------------------------------------------------------------------------
//! float ���� half float�ɕϊ�
//!	@param	[in]	pImage			�ϊ���
//!	@param	[in]	bytePerPixel	�P�v�f�̃o�C�g��
//-----------------------------------------------------------------------------
u16* Texture::FloatToHalf(f32* pImage, GLuint bytePerPixel)
{
	// �摜�T�C�Y���v�Z
	s32 imageSize = _width * _height * bytePerPixel;
	u16* result = new u16[imageSize];

	for (s32 i = 0; i<imageSize; ++i) {
		result[i] = common::Float32to16(pImage[i]);
	}

	// �O�̃f�[�^�͍폜����
	SAFE_DELETE(pImage);

	return result;
}

//=============================================================================
//	END OF FILE
//=============================================================================
