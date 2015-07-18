//-----------------------------------------------------------------------------
//!
//!	@file	gmHDRTexReader.h
//!	@brief	HDR�e�N�X�`���ǂݍ���
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


namespace HDR
{
	typedef		 BYTE		COLR[4];	// RGBE
	static const u32		R = 0;		// RED
	static const u32		G = 1;		// GREEN
	static const u32		B = 2;		// BLUE
	static const u32		E = 3;		// EXPOSUER


	// �X�L�������C���̏��Ԃ̂��߂̃t���O
	#define XDECR			1		// ���̕����ɐL�т邩�ǂ���
	#define YDECR			2		// ���̕����ɐL�т邩�ǂ��� 
	#define YMAJOR			4		// ��ƍs�̂ǂ��炪�D�悩
	#define MIN_E_LEN		8		// �ŏ��摜�𑜓x
	#define MAX_E_LEN		0x7fff	// �ő�摜�𑜓x
}


//-----------------------------------------------------------------------------
//!	HDR�e�N�X�`���ǂݍ���
//-----------------------------------------------------------------------------
class HDRTexReader
{
public:

	class	Reader;
	struct	Info;

private:
	

	//! �]���ȉ��s����菜��
	void	removeCRCF(GM_STR p);
	//! �f�[�^�̕]��
	void	valData(Info* pInfo, GM_CSTR s);
	//! �w�b�_�̓ǂݍ���
	void	getHeader(Info* pInfo);
	//! �T�C�Y�̓ǂݍ���
	bool	getResolution(Info* pInfo);
	//! �X�L�������C���̕�������ݒ肷��
	void	setScanWidthAndHeight(Info* pInfo);
	//! HDR �t�@�C���̓ǂݎ��(�t�H�[�}�b�g�`�F�b�N)
	//!	@return true : �t�H�[�}�b�g��������
	bool	checkHeader(Info* pInfo);
	//! �F����ǂݍ���
	bool	readColor(HDR::COLR* color, s32 len);

	//! Radiance�X�L�������C���t�H�[�}�b�g��RGBE �ϊ�
	//!	@param	[in]	scanLength		�X�L�������C���̒���
	//!	@param	[in]	scanWidth		�X�L�������C���̕�
	//!	@param	[in]	bytePerPixel	�P�v�f�̃o�C�g��
	u8*		RadianceToRGBE(s32 scanLength, s32 scanWidth, GLuint bytePerPixel);

	//! RGBE -> RGB�ϊ�
	//!	@param	[in]	pInfo  �e�N�X�`�����
	//!	@param	[in]	pImage �ϊ���
	u32*	RGBEToRGB(Info* pInfo, u8*& pImage);

	//! int -> half float�ɂ��ĕԂ�
	//!	@param	[in]	pInfo			�e�N�X�`�����
	//!	@param	[in]	pImage			�ϊ���
	//!	@param	[in]	bytePerPixel	�P�v�f�̃o�C�g��
	u16*	IntToHalfFloat(Info* pInfo, u32*& pImage, GLuint bytePerPixel);

public:

	//! HDR�ǂݍ���
	//!	@param	[in]	filePath	�t�@�C���p�X
	//! @param	[in]	w			���ۑ���
	//!	@param	[in]	h			�����ۑ���
	void*	loadHDR(GM_CSTR filePath, s32& w, s32& h);

private:
	HDRTexReader::Reader*		_pReader;
};

//-----------------------------------------------------------------------------
//!	HDR�e�N�X�`���̏��
//-----------------------------------------------------------------------------
struct HDRTexReader::Info
{
	GM_CHAR		_magicNumber[64];	//!< #?�̌� "RADIANCE"�̂�
	GM_CHAR		_fileFormat[64];	//!< �t�H�[�}�b�g
	f64			_exposureTime;		//!< �I������
	Size<s32>	_imageSize;			//!< �摜�T�C�Y
	u8			_flags;				//!< �t���O
	s32			_scanLength;		//!< �X�L�������C���̒���
	s32			_scanWidth;			//!< �X�L�������C���̕�
};

//-----------------------------------------------------------------------------
//!	�\���p�[�T�[
//-----------------------------------------------------------------------------
class HDRTexReader::Reader
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	Reader(void);
	//! �f�X�g���N�^
	virtual ~Reader(void);

	//! ������
	bool				init(GM_CSTR fileName);
	//! ���
	void				cleanup(void);

	//@}
	//-------------------------------------------------------------
	//! @name �擾/�ݒ�
	//-------------------------------------------------------------
	//@{

	//! ���̃g�[�N�����P�擾
	//!	@return �g�[�N��������̐擪�i����getToken()�����s����Ə��ł���ꎞ�I�Ȃ��́j
	//!	@return	NULL�Ȃ�t�@�C���I�[
	GM_CSTR				getToken(void);

	//! ���̈ꕶ�����Ƃ�(getToken�Ɠ������ꎞ�I�Ȃ���)
	//!	@return	NULL�Ȃ�t�@�C���I�[
	BYTE				getByte(bool* eof = NULL);
	//@}

	//! ���݂̃g�[�N���ƕ������r
	bool				compare(GM_CSTR token);

	//! ���̃g�[�N���̏o���܂Ői�߂�
	bool				searchNextToken(GM_CSTR token);


private:
	static const s32	TOKEN_LENGTH_MAX = 512;
	GM_CHAR				_token[TOKEN_LENGTH_MAX];

	GM_STR				_pCurrent;

	GM_STR				_pBuffer;
	s32					_bufferSize;
};


//=============================================================================
//	END OF FILE
//=============================================================================