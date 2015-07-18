//-----------------------------------------------------------------------------
//!
//!	@file	gmSystem.cpp
//!	@brief	�ėp�I�֐��Q
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//! ���W�A���ɕϊ�
//-----------------------------------------------------------------------------
f32		TO_RADIAN(f32	deg);

//-----------------------------------------------------------------------------
//!	�f�O���[�ɕϊ�
//-----------------------------------------------------------------------------
f32		TO_DEGREE(f32	rad);

////! 2D�`��p
//void view2D();
//
////! 3D�`��p
//void view3D();

//-----------------------------------------------------------------------------
//! ���`���
//! P' = (E - S) * f / maxf + S
//! @param	[in] start		�ŏ��l(�n�_) ��L��S�ɓ�����
//! @param	[in] end		�ő�l(�I�_) ��L��E�ɓ�����
//! @param	[in] frame		���݂̒l	 ��L��f�ɓ�����
//!	@param	[in] maxFrame	�l�̍ő�l	 ��L��maxf�ɓ�����
//-----------------------------------------------------------------------------
f32 LinearInterpolation( f32 start , f32 end , f32 frame, f32 maxFrame);

//! FPS�擾
float getGLFps();

//! ������\��
void render_string(float x, float y, std::string const& str);


//-----------------------------------------------------------------------------
//!	�u�����h���[�h (Blend Mode)
//-----------------------------------------------------------------------------
enum BLEND_MODE {
	BM_NONE,			//!< �����Ȃ�(OFF)
	BM_BLEND_NOALPHA,	//!< �ʏ�u�����h(�A���t�@�Ȃ�)
	BM_BLEND_ALPHA,		//!< �ʏ�u�����h(�A���t�@����)
	BM_INVERSE,			//!< ���]
	BM_ADD_NOALPHA,		//!< ���Z�u�����h(�A���t�@�Ȃ�)
	BM_ADD_ALPHA,		//!< ���Z�u�����h(�A���t�@����)
	BM_SUB_NOALPHA,		//!< ���Z(�A���t�@�Ȃ�)
	BM_SUB_ALPHA,		//!< ���Z(�A���t�@����)
	BM_MULTI_NOALPHA,	//!< ��Z(�A���t�@�Ȃ�)
	BM_MULTI_ALPHA,		//!< ��Z(�A���t�@����)
	BM_SCREEN_NOALPHA,	//!< �X�N���[��(�A���t�@�Ȃ�)
	BM_DIV_NOALPHA,		//!< ���Z(�A���t�@�Ȃ�)
	BM_INVISIVLE,		//!< ����
	BM_DARKNESS,		//!< �_�[�N�l�X
	BM_TEST,			//!< �e�X�g�p


};

//-----------------------------------------------------------------------------
//!	�u�����h���[�h��ݒ�
//!	@param	[in]	mode	�u�����h���[�h
//-----------------------------------------------------------------------------
void setBlendMode(BLEND_MODE mode);

//-----------------------------------------------------------------------------
//! �_�̕`��
//!	@param	[in]	pos		�_�̍��W
//-----------------------------------------------------------------------------
void DebugPointRender(Vector3& pos);


class Texture;

//! �o�b�N�o�b�t�@�`��
//!	@param	[in]	backBuffer	�o�b�N�o�b�t�@�e�N�X�`��
void drawBackBuffer(Texture* backBuffer);

//! �V���h�E�e�N�X�`���f�o�b�O�`��
//!	@param	[in]	textureID	�e�N�X�`���̃n���h��
//!	@param	[in]	x			�`���X���W
//!	@param	[in]	y			�`���Y���W
void debugShadowTexture(GLuint textureID, s32 x, s32 y);

//! �l�p�`�̕`��
void drawRect(f32 x, f32 y, f32 w, f32 h, const Color& color);

//-----------------------------------------------------------------------------
//! ���O�ƈ�v���邩�ǂ���
//!	�}�N���Ӗ� #x : x�𕶎���ɂ���	is##x : is��x���Ȃ���
//-----------------------------------------------------------------------------
#define DEF_COL_NAME(x)								\
	static char* myName = #x;						\
													\
bool	is##x(Collision* obj)						\
{													\
	if( strcmp(obj->getName(), myName ) == 0 ){		\
		return true;								\
	}												\
	return false;									\
}				

#define DEF_COL_NAME_HEADER(x)						\
bool	is##x(Collision* obj);

//-----------------------------------------------------------------------------
//! ���O�ƈ�v���邩�ǂ���(�U���p)
//!	�}�N���Ӗ� #x : x�𕶎���ɂ���	is##x : is��x���Ȃ���
//-----------------------------------------------------------------------------
#define DEF_COL_ATTACK_NAME(x)							\
	static char* myAttackName = #x;						\
														\
bool	is##x(Collision* obj)							\
{														\
	if( strcmp(obj->getName(), myAttackName ) == 0 ){	\
		return true;									\
	}													\
	return false;										\
}				

#define DEF_COL_ATTACK_NAME_HEADER(x)					\
bool	is##x(Collision* obj);



//-----------------------------------------------------------------------------
//!	�A�T�[�g���b�Z�[�W�t��
//-----------------------------------------------------------------------------
#define ASSERTMSG(IF_, MSG_)	assert( IF_ && MSG_ )


//-----------------------------------------------------------------------------
// GL�̃G���[�����o
//-----------------------------------------------------------------------------
bool getGLError();


namespace math{

	//! �~������
	static const f32	PI = 3.14159265f;

	template <typename T>
	T	getSameValofFour(T search, T a, T b, T c, T d)
	{
		if( search == abs(a) ){
			return a;
		}else if( search == abs(b) ){
			return b;
		}else if( search == abs(c) ){
			return c;
		}else{
			return d;
		}
	}


}	// namespace math



namespace common {

	struct FloatComp {
		union {
			f32 f;	// �t���[�g�̒l
			u32	u;	// �r�b�g�ł̒l 
		};
	};

	//-----------------------------------------------------------------------------
	//! int -> float�ϊ�
	//!	@param	[in]	val		�ϊ���
	//-----------------------------------------------------------------------------
	f32	IntToFloat(u32 val);

	//! Byte -> float�ϊ�
	//!	@param	[in]	val		�ϊ���
	f32			ByteToFloat(u8 val);

	//! 32bitfloat -> 16bitfloat�ϊ�
	//!	@param	[in]	val				�ϊ���
	// �Q��URL : http://www.platinumgames.co.jp/programmer_blog/?p=484
	u16			Float32to16(f32 val);

}	// namespace common
