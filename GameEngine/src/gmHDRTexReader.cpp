//-----------------------------------------------------------------------------
//!
//!	@file	gmHDRTexReader.cpp
//!	@brief	HDR�e�N�X�`���ǂݍ���
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"
using namespace HDR;


//=============================================================================
//	HDR�e�N�X�`���ǂݍ��݃N���X�̎���
//=============================================================================

//-----------------------------------------------------------------------------
//! �]���ȉ��s����菜��
//-----------------------------------------------------------------------------
void HDRTexReader::removeCRCF(GM_STR p)
{
	for (s32 len = strlen((GM_CSTR)(p)) - 1; 0 <= len; len--)
	{
		// ���s���Ȃ���ΏI��
		if( p[len] != '\r' && p[len]!='\n' ) break;
		// ���s������ΏI�[�R�[�h�����
		p[len] = '\0';
	}
}

//-----------------------------------------------------------------------------
//! �f�[�^�̕]��
//-----------------------------------------------------------------------------
void HDRTexReader::valData(Info* pInfo, GM_CSTR s)
{
	if( strncmp(s, "#?", 2) == 0 )
	{
		// �}�W�b�N�i���o�[
		strcat(pInfo->_magicNumber, s+2);
		removeCRCF(pInfo->_magicNumber);

	}else if( strncmp(s, "FORMAT=", 7) == 0 ) {
		// �t�H�[�}�b�g
		strcat(pInfo->_fileFormat, s+7);
		removeCRCF(pInfo->_fileFormat);
	}
}

//-----------------------------------------------------------------------------
//! �w�b�_�̓ǂݍ���
//-----------------------------------------------------------------------------
void HDRTexReader::getHeader(Info* pInfo)
{
	while(true) {
		GM_CSTR token =  _pReader->getToken();

		// �w�b�_��񂪏I�������I��
		if(_pReader->compare("-Y") || _pReader->compare("+X")) return;

		// �f�[�^���r���ď����擾
		valData(pInfo, token);
	}
}

//-----------------------------------------------------------------------------
//! �T�C�Y�̓ǂݍ���
//-----------------------------------------------------------------------------
bool HDRTexReader::getResolution(Info* pInfo)
{
	//�u-Y M +X N�v�̂Ƃ��͑����f�[�^��N�s�̃f�[�^��M�񂠂�\��
	// �����ɂ����Ƃ���-Y��+X�܂ł��ĂȂ���΃w�b�_�ǂݍ��ݎ��s���Ă���
	if(!_pReader->compare("-Y") && !_pReader->compare("+X")) return false;

	s32 w,h;
	if(_pReader->compare("-Y")) {			// -Y����Ȃ�
		 h = atoi(_pReader->getToken());
		 // +X���Ƃ΂�
		 _pReader->getToken();
		 w = atoi(_pReader->getToken());
		 _pReader->getByte();	// \n��΂�
		 pInfo->_flags |= YMAJOR;
	}else if (_pReader->compare("+X")) {	// +X����Ȃ�
		w = atoi(_pReader->getToken());
		// -Y���Ƃ΂�
		_pReader->getToken();
		h = atoi(_pReader->getToken());
		_pReader->getByte();	// \n��΂�
	}

	// �T�C�Y��0�ȉ��Ȃ玸�s
	if(w <= 0 || h <= 0 ) return false;

	// �T�C�Y���i�[
	pInfo->_imageSize = Size<s32>(w,h);
	
	// �X�L�������C���̕�������ݒ肷��
	setScanWidthAndHeight(pInfo);

	return true;
}

//-----------------------------------------------------------------------------
//! �X�L�������C���̕�������ݒ肷��
//-----------------------------------------------------------------------------
void HDRTexReader::setScanWidthAndHeight(Info* pInfo)
{
	if(pInfo->_flags & YMAJOR){
		pInfo->_scanLength = pInfo->_imageSize._w;
		pInfo->_scanWidth  = pInfo->_imageSize._h;
	}else{
		pInfo->_scanLength = pInfo->_imageSize._h;
		pInfo->_scanWidth  = pInfo->_imageSize._w;
	}
}

//-----------------------------------------------------------------------------
//! HDR �t�@�C���̓ǂݎ��(�t�H�[�}�b�g�`�F�b�N)
//!	@return true : �t�H�[�}�b�g��������
//-----------------------------------------------------------------------------
bool HDRTexReader::checkHeader(Info* pInfo)
{
	//---- �f�[�^�̏�����
	pInfo->_magicNumber[0]	= '\0';
	pInfo->_fileFormat[0]	= '\0';
	pInfo->_exposureTime	= 1.0000000000000;
	pInfo->_flags			= 0;
	pInfo->_scanLength		= 0;
	pInfo->_scanWidth		= 0;


	// �w�b�_���̓ǂݎ��
	getHeader(pInfo);

	// �w�b�_���̃`�F�b�N
	if( pInfo->_magicNumber[0] == '\0' || strcmp("RADIANCE", pInfo->_magicNumber) )			return false;

	// �t�H�[�}�b�g�`�F�b�N
	if( pInfo->_fileFormat[0] == '\0' || strcmp("32-bit_rle_rgbe", pInfo->_fileFormat) )	return false;

	// �T�C�Y�ǂݍ���
	if( !getResolution(pInfo) ) return false;

	return true;
}

//-----------------------------------------------------------------------------
//! �F����ǂݍ���
//! @param	[in]	len		�X�L�������郉�C���̒���
//-----------------------------------------------------------------------------
bool HDRTexReader::readColor(HDR::COLR* color, s32 len)
{
	s32	i, j;
	s32 code, val;

	// �ǂݍ��݃^�C�v�����߂�
	// �ŐV�Ȃ�ŏ�4�̐�����
	// [ 2 | 2 | �摜�T�C�Y�̏��7bit | �摜�T�C�Y�̉���8bit ]
	if( len < MIN_E_LEN || MAX_E_LEN < len ) {
		// �Â��^�C�v
	}

	i = _pReader->getByte();

	if( i != 2 ) {
		// �ŏ���2�łȂ��̂͌Â��^�C�v

	}

	bool checkEOF = true;
	// �Œ�S�̃f�[�^���K�v
	color[0][G] = _pReader->getByte();
	color[0][B] = _pReader->getByte();
	i = _pReader->getByte(&checkEOF);
	if (checkEOF) return false;

	// �ŏ��̃f�[�^��"22"�Ƃ��ɂȂ��Ă��Ȃ�������A
	// �R�o�C�g�ڂ̃f�[�^�ɂ��čő吔�𒴂��Ă��邩�̃`�F�b�N
	if (color[0][G] != 2 || color[0][B] & 128) {
		color[0][R] = 2;
		color[0][E] = i;
//		return(s_OldReadColors(scanline + 1, len - 1, fp));
	}

	// �������̊m�F(������)
	if( (color[0][B] << 8 | i) != len ) return false;

	// �e������ǂݍ���
	for(i=0; i<4; ++i)
	{
		for(j=0; j<len; )
		{
			// �f�[�^���r�؂�Ă���
			code = _pReader->getByte(&checkEOF);
			if(checkEOF) return false;

			// �R�[�h��128�ȏ�Ȃ爳�k����Ă���
			if( 128 < code ) {
				// ����+�l�̃��������O�X
				code &= 127;
				val = _pReader->getByte();
				while(code--)	color[j++][i] = val;
			}else{
				// ���������O�X���k����Ă��Ȃ����̂�code����
				while (code--)	color[j++][i] = _pReader->getByte();
			}
		}
	}
}

//-----------------------------------------------------------------------------
//! Radiance�X�L�������C���t�H�[�}�b�g��RGBE �ϊ�
//!	@param	[in]	scanLength	�X�L�������C���̒���
//!	@param	[in]	scanWidth	�X�L�������C���̕�
//!	@param	[in]	bytePerPixel	�P�v�f�̃o�C�g��
//-----------------------------------------------------------------------------
u8* HDRTexReader::RadianceToRGBE(s32 scanLength, s32 scanWidth, GLuint bytePerPixel)
{
	s32			ret = 0;
	HDR::COLR*	scanColor;
	s32			x, y;
	
	u8*			pImage = nullptr;

	// �������̊m��
	if ((scanColor = (HDR::COLR *)malloc(scanLength*sizeof(HDR::COLR))) == nullptr)
	{
		MessageBoxA(nullptr, "�������[�̊m�ۂɎ��s���܂����B", "�G���[", MB_OK);
		// ���������
		free((u8*)scanColor);
		return nullptr;
	}


	s32 w = scanWidth;
	s32 h = scanLength;
	// �ۑ���̃��������m�ۂ���
	pImage = new u8[w * h * bytePerPixel];
	//srcImage = new u32[w * h * bytePerPixel];


	// �摜�̕ϊ�
	for(y=0; y<scanWidth; ++y) 
	{
		if(!readColor(scanColor, scanLength)) 
		{
			MessageBoxA(nullptr, "�J���[�̓ǂݍ��݂Ɏ��s���܂����B", "�G���[", MB_OK);
			break;
		}

		
		for(x=0; x<scanLength; ++x) 
		{
			// x, y�Ԗڂ̃s�N�Z���̃A�h���X���擾
			u8* pDst = pImage + (y * scanWidth + x) * bytePerPixel;

			pDst[0] = scanColor[x][R];	// red
			pDst[1] = scanColor[x][G];	// green
			pDst[2] = scanColor[x][B];	// blue
			pDst[3] = scanColor[x][E];	// exposure

			/*pDst[0] = pDst[0] *  pow(2, (pDst[3] - 128));
			pDst[1] = pDst[1] *  pow(2, (pDst[3] - 128));
			pDst[2] = pDst[2] *  pow(2, (pDst[3] - 128));*/
		}

	}


	// ���������
	free((u8*)scanColor);

	return pImage;
}

//-----------------------------------------------------------------------------
//! RGBE -> RGB�ϊ�
//!	@param	[in]	pInfo  �e�N�X�`�����
//!	@param	[in]	pImage �ϊ���
//-----------------------------------------------------------------------------
u32* HDRTexReader::RGBEToRGB(Info* pInfo, u8*& pImage)
{
	s32  w					= pInfo->_scanWidth;
	s32  h					= pInfo->_scanLength;
	s32  bytePerPixel		= 3;					// RGB�ɕϊ����邽�߂�4�ł͂Ȃ�3
	s32  bytePerPixelSrc	= 4;					// RGBE�p�̃o�C�g/�s�N�Z��
	u32* convertResult		= new u32[w * h * bytePerPixel];

	// �ϊ����Ă���
	for(s32 y=0; y<h; ++y)
	{
		for (s32 x = 0; x<w; ++x)
		{
			// x, y�Ԗڂ̃s�N�Z���̃A�h���X���擾
			u8* pSrc  = pImage		 + (y * w + x) * bytePerPixelSrc;
			u32* pDst = convertResult + (y * w + x) * bytePerPixel;
			
			pDst[0] = (u32)( pSrc[0] *  pow(2, (pSrc[3] - 128)) );
			pDst[1] = (u32)( pSrc[1] *  pow(2, (pSrc[3] - 128)) );
			pDst[2] = (u32)( pSrc[2] *  pow(2, (pSrc[3] - 128)) );
		}
	}

	// �ϊ����I������̂�pImage�͍폜����
	SAFE_DELETE(pImage);
	// �|�C���^
	return convertResult;
}


//-----------------------------------------------------------------------------
//! int -> half float�ɂ��ĕԂ�
//!	@param	[in]	pInfo			�e�N�X�`�����
//!	@param	[in]	pImage			�ϊ���
//!	@param	[in]	bytePerPixel	�P�v�f�̃o�C�g��
//-----------------------------------------------------------------------------
u16* HDRTexReader::IntToHalfFloat(Info* pInfo, u32*& pImage, GLuint bytePerPixel)
{
	s32  w			   = pInfo->_scanWidth;
	s32  h			   = pInfo->_scanLength;
	s32  imageSize	   = w * h * bytePerPixel;
	u16* convertResult = new u16[imageSize];

	for(s32 i=0; i<imageSize; ++i){
		f32 floatVal	 = common::IntToFloat(pImage[i]);
		convertResult[i] = common::Float32to16(floatVal);
	}

	// �ϊ����I������̂�pImage�͍폜����
	SAFE_DELETE(pImage);

	return convertResult;

}

//-----------------------------------------------------------------------------
//! HDR�ǂݍ���
//!	@param	[in]	filePath	�t�@�C���p�X
// !@param	[in]	w			���ۑ���
//!	@param	[in]	h			�����ۑ���
//-----------------------------------------------------------------------------
void* HDRTexReader::loadHDR(GM_CSTR filePath, s32& w, s32& h)
{

	Info	info;

	void* pImage = nullptr;

	// �\���p�[�T�[�ňꊇ�ǂݍ���
	_pReader = new HDRTexReader::Reader();
	if( !_pReader->init(filePath) ) {
		MessageBoxA(nullptr, "�\���p�[�T�[�ł̈ꊇ�ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK );
		return false;
	}

	// �w�b�_�𒲂ׂ�
	if( !checkHeader(&info) )
	{
		MessageBoxA(nullptr, "HDR�e�N�X�`���̃t�H�[�}�b�g�����������ł��B", "�G���[", MB_OK);
		return false;
	}

	// �R���o�[�g����
	if ( ( pImage = RadianceToRGBE(info._scanLength, info._scanWidth, 4) ) == nullptr){
		// ���s�����̂ŉ��
		SAFE_DELETE(pImage);
		return nullptr;
	}

	// RGB�ɕϊ�����
	pImage = RGBEToRGB(&info, (u8*&)pImage);

	// halfFloat�ɕϊ�����
	pImage = IntToHalfFloat(&info, (u32*&)pImage, 3);

	w = info._imageSize._w;
	h = info._imageSize._h;


	return pImage;
}



//=============================================================================
//	HDRTexReader::Reader			�\���p�[�T�[
//=============================================================================

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
HDRTexReader::Reader::Reader(void)
	: _pCurrent(nullptr)
	, _pBuffer(nullptr)
	, _bufferSize(0)
{
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
HDRTexReader::Reader::~Reader(void)
{
	cleanup();
}

//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool HDRTexReader::Reader::init(GM_CSTR fileName)
{
	//-------------------------------------------------------------
	// �t�@�C����ǂݍ���
	//-------------------------------------------------------------
	HANDLE	hFile = CreateFileA(
		fileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_READONLY,
		nullptr);

	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBoxA(nullptr, fileName, "�t�@�C���ǂݍ��݂Ɏ��s���܂����B", MB_OK);
		return false;
	}

	// �t�@�C���T�C�Y���擾
	u32		size = GetFileSize(hFile, nullptr);
	_pBuffer = new GM_CHAR[size + 1];
	_pBuffer[size] = ' ';

	_bufferSize = size + 1;
	// ��������̃o�b�t�@�Ɉꊇ�ǂݍ���
	DWORD	readSize;
	ReadFile(hFile, _pBuffer, size, &readSize, nullptr);

	// �t�@�C�������
	CloseHandle(hFile);


	_pCurrent = _pBuffer;
	return true;
}

//-----------------------------------------------------------------------------
//! ���
//-----------------------------------------------------------------------------
void HDRTexReader::Reader::cleanup(void)
{
	if (_pBuffer) {
		delete[] _pBuffer;
		_pBuffer = nullptr;
	}
}

//-----------------------------------------------------------------------------
//! ���̃g�[�N�����P�擾
//!	@return �g�[�N��������̐擪�i����getToken()�����s����Ə��ł���ꎞ�I�Ȃ��́j
//!	@return	NULL�Ȃ�t�@�C���I�[
//-----------------------------------------------------------------------------
GM_CSTR HDRTexReader::Reader::getToken(void)
{
	GM_STR& p	  = _pCurrent;
	GM_STR	pTail = _pBuffer + _bufferSize;

	// �Ō�܂ł��Ă�����I��
	if( p >= pTail )	return nullptr;

	//---- ��؂�L���A�󔒁A���s���X�L�b�v
	while( *p == ' ' || *p == '\t' || *p == 0x0a || *p == 0x0d )
	{
		p++;
		if (p >= pTail) return nullptr;
	}

	s32	count = 0;

	//---- ���̋�؂�܂Ō���
	do { 
		_token[count] = *p;

		// ���̕�����
		count++;
		p++;

		// �Ō�܂ł��Ă�����I��
		if (p >= pTail)	return nullptr;

		// �󔒁A���s������΃��[�v�I��
	} while ( !(*p == ' ' || *p == '\t' || *p == 0x0a || *p == 0x0d) );


	_token[count] = '\0';

	return _token;

}

//-----------------------------------------------------------------------------
//! ���̈ꕶ�����Ƃ�(getToken�Ɠ������ꎞ�I�Ȃ���)
//!	@return	NULL�Ȃ�t�@�C���I�[
//-----------------------------------------------------------------------------
BYTE HDRTexReader::Reader::getByte(bool* eof)
{
	GM_STR& p	  = _pCurrent;
	GM_STR	pTail = _pBuffer + _bufferSize;

	if (eof) *eof = false;
	// �Ō�܂ł��Ă�����I��
	if (p >= pTail)	{
		if (eof) *eof = true;
	}

	s32	count = 0;

	//---- ���̋�؂�܂Ō���
	BYTE b = *p;

	// ���̕�����
	count++;
	p++;

	return b;
}

//-----------------------------------------------------------------------------
//! ���݂̃g�[�N���ƕ������r
//-----------------------------------------------------------------------------
bool HDRTexReader::Reader::compare(GM_CSTR token)
{
	return strncmp(_token, token, strlen(_token)) == 0;
}

//-----------------------------------------------------------------------------
//! ���̃g�[�N���̏o���܂Ői�߂�
//-----------------------------------------------------------------------------
bool HDRTexReader::Reader::searchNextToken(GM_CSTR token)
{
	// ��v���镶����܂Ői��
	while( compare(token) == false ) {
		if( getToken() == nullptr ) {
			return false;
		}
	}
	// ��v���镶���𔭌�
	return true;
}

//=============================================================================
//	END OF FILE
//=============================================================================
