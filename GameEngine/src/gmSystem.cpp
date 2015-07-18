//-----------------------------------------------------------------------------
//!
//!	@file	gmSystem.h
//!	@brief	�ėp�I�֐��Q
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include"stdafx.h"

//-----------------------------------------------------------------------------
//! ���W�A���ɕϊ�
//-----------------------------------------------------------------------------
f32		TO_RADIAN(f32	deg)
{
	return deg * math::PI / 180.0f;
}

//-----------------------------------------------------------------------------
//!	�f�O���[�ɕϊ�
//-----------------------------------------------------------------------------
f32		TO_DEGREE(f32	rad)
{
	return rad * 180.0f / math::PI;
}


//-----------------------------------------------------------------------------
//! ���`���(frame�̒l��S����E�̊Ԃ̒l�Ɏ��܂�)
//! P' = (E - S) * f / maxf + S
//! @param	[in] start		�ŏ��l(�n�_) ��L��S�ɓ�����
//! @param	[in] end		�ő�l(�I�_) ��L��E�ɓ�����
//! @param	[in] frame		���݂̒l	 ��L��f�ɓ�����
//!	@param	[in] maxFrame	�l�̍ő�l	 ��L��maxf�ɓ�����
//-----------------------------------------------------------------------------
f32 LinearInterpolation( f32 start , f32 end , f32 frame, f32 maxFrame){
    return ((long)(end - start) * frame / maxFrame + start);
}



//---------------------------------------------------------------------------
//!	�u�����h���[�h��ݒ�
//!	@param	[in]	mode	�u�����h���[�h
//---------------------------------------------------------------------------
void setBlendMode(BLEND_MODE mode)
{
	//-------------------------------------------------------------
	// �������u�����h ON/OFF
	//-------------------------------------------------------------
	if( mode == BM_NONE ) {		//---- OFF
		glDisable(GL_BLEND);
	}
	else {						//---- ON
		glEnable(GL_BLEND);
	}

	//----------------------------------------------------------------
	// �u�����h���[�h�̍����䗦�̐ݒ�
	//----------------------------------------------------------------
	switch( mode ) 
	{
		case BM_NONE:
			break;
		case BM_BLEND_NOALPHA:
		{
			glBlendFunc(GL_ONE, GL_ZERO);
			break;
		}
		case BM_BLEND_ALPHA:
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		}
		case BM_INVERSE:
		{
			glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE);
			break;
		}
		case BM_ADD_NOALPHA:
		{
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		}
		case BM_ADD_ALPHA:
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		}
		case BM_SUB_NOALPHA:
		{
			glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ZERO);
			glBlendFunc(GL_ONE, GL_ONE);
			glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ZERO);
			break;
		}
		case BM_SUB_ALPHA:
		{
			glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ZERO);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ZERO);
			break;
		}
		case BM_MULTI_NOALPHA:
		{
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
			break;
		}
		case BM_MULTI_ALPHA:
		{
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
			glBlendFunc(GL_ZERO, GL_SRC_ALPHA);
			break;
		}
		case BM_SCREEN_NOALPHA:
		{
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
			break;
		}
		case BM_DIV_NOALPHA:
		{
			glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE_MINUS_SRC_COLOR);
			break;
		}
		case BM_INVISIVLE:
		{
			glBlendFunc(GL_DST_COLOR, GL_ONE);
			break;
		}
		case BM_DARKNESS:
		{
			glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);

			break;
		}

			
	}
}

//-----------------------------------------------------------------------------
//! �e�N�X�`����2D�\��
//!	@param	[in]	pTexture	�e�N�X�`���̃|�C���^
//!	@param	[in]	w			�`��̕�
//!	@param	[in]	h			�`��̍���
//!	@param	[in]	x			�`���X���W
//!	@param	[in]	y			�`���Y���W
//-----------------------------------------------------------------------------
void drawTexture2D(Texture* pTexture, f32 w, f32 h, f32 x, f32 y)
{
	Texture::set(pTexture);

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(x,y);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(x+w,y);

		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(x,y+h);

		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(x+w,y+h);

	glEnd();

	Texture::set(NULL);
}

//-----------------------------------------------------------------------------
//! �o�b�N�o�b�t�@�`��
//!	@param	[in]	backBuffer	�o�b�N�o�b�t�@�e�N�X�`��
//-----------------------------------------------------------------------------
void drawBackBuffer(Texture* backBuffer)
{

	//-------------------------------------------------------------
	// ���ƍ����������Ŏ擾
	//-------------------------------------------------------------
	//---- �e�N�X�`���̕��ƍ���
	Size<s32> texSize = backBuffer->getSize();

	glPushAttrib(GL_VIEWPORT_BIT);
	// �������݃T�C�Y���e�N�X�`���͈̔͂ɐݒ�
	glViewport(0, 0, texSize._w, texSize._h);
	//-------------------------------------------------------------
	// �V�F�[�_�萔(Shader Constant)�̐ݒ�
	//-------------------------------------------------------------
	//---- �V�F�[�_�̕ύX
	// �V�F�[�_��ύX
	GmShader()->changeShader(SystemShader::SHADER_PUT_TEXTURE);
	
	//---- �e�N�X�`��
	GmShader()->bindTexture("BaseTexture", backBuffer->getColorBuffer(), 0);

	//-------------------------------------------------------------
	// �`�� (Draw)
	//-------------------------------------------------------------
	static GLuint VB;
	static bool	Initialized = false;
	if( !Initialized ) {
		glGenBuffers(1, &VB);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, nullptr, GL_DYNAMIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, sizeof(FilterVertex) * 4, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		Initialized = true;
	}

	const FilterVertex v[4] = {
		{ Float3(-1.0f, -1.0f, 0.0f), Color(255,255,255,255), Color::NONE, Float2(0.0f, 0.0f) },
		{ Float3(+1.0f, -1.0f, 0.0f), Color(255,255,255,255), Color::NONE, Float2(1.0f, 0.0f) },
		{ Float3(-1.0f, +1.0f, 0.0f), Color(255,255,255,255), Color::NONE, Float2(0.0f, 1.0f) },
		{ Float3(+1.0f, +1.0f, 0.0f), Color(255,255,255,255), Color::NONE, Float2(1.0f, 1.0f) },
	};

	/*const Vertex	v[4] = {
		{ Float3(-1.0f, -1.0f, 0.0f), Color(255,255,255,255), Float2(0.0f, 0.0f) },
		{ Float3(+1.0f, -1.0f, 0.0f), Color(255,255,255,255), Float2(1.0f, 0.0f) },
		{ Float3(-1.0f, +1.0f, 0.0f), Color(255,255,255,255), Float2(0.0f, 1.0f) },
		{ Float3(+1.0f, +1.0f, 0.0f), Color(255,255,255,255), Float2(1.0f, 1.0f) }
	};*/

	//---- �`��
	//glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);

	// ���_�o�b�t�@�o�C���h
	glBindBuffer(GL_ARRAY_BUFFER, VB);
// �f�[�^�]��
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(FilterVertex) * 4, v);


	GmShader()->setAndEnableVertexAttribute("position"		, 3, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _position));
	GmShader()->setAndEnableVertexAttribute("uv"			, 2, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _uv));
	GmShader()->setAndEnableVertexAttribute("color"			, 4, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _diffuseColor));
	GmShader()->setAndEnableVertexAttribute("emissiveColor"	, 4, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _emissiveColor));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	GmShader()->DisableVertexAttribute("position");
	GmShader()->DisableVertexAttribute("uv");
	GmShader()->DisableVertexAttribute("color");
	GmShader()->DisableVertexAttribute("emissiveColor");

	GmShader()->unbindTexture();

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// ���ɖ߂�
	//glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GmShader()->beginPrevShader();
	glPopAttrib();
}


//-----------------------------------------------------------------------------
//! �V���h�E�e�N�X�`���f�o�b�O�`��
//!	@param	[in]	textureID	�e�N�X�`���̃n���h��
//!	@param	[in]	x			�`���X���W
//!	@param	[in]	y			�`���Y���W
//-----------------------------------------------------------------------------
void debugShadowTexture(GLuint textureID, s32 x, s32 y)
{
	glPushAttrib(GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);


	int shadH  = GmShader()->getCurrentShaderPorgram();
	int handle = glGetUniformLocation(shadH, "quadTexture");
	glUniform1i(handle, 0);
	glViewport(x, y, 300, 300);

	GLfloat vertex_data[] =
	{
		-1,-1, 0,
		 1,-1, 0,
		 1, 1, 0,
		-1, 1, 0,
	};

	/*glBegin(GL_QUADS);
	glVertex3f(-1,-1, 0);
	glVertex3f( 1,-1, 0);
	glVertex3f( 1, 1, 0);
	glVertex3f(-1, 1, 0);
	glEnd();*/

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertex_data);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);

	glEnable(GL_DEPTH_TEST);
//	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glPopAttrib();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//-----------------------------------------------------------------------------
//! �l�p�`�̕`��
//-----------------------------------------------------------------------------
void drawRect(f32 x, f32 y, f32 w, f32 h, const Color& color)
{
	Size<s32> screenSize  = GmWindow()->getResolution();

	f32 x1 = (x + w) * ( 2.0f / screenSize._w) - 1.0f;
	f32 y1 = (y	+ h) * (-2.0f / screenSize._h) + 1.0f;

	f32 x2 = (x    ) * ( 2.0f / screenSize._w) - 1.0f;
	f32 y2 = (y    ) * (-2.0f / screenSize._h) + 1.0f;

	static GLuint VB;
	static bool Initialized = false;
	if( !Initialized ) {
		glGenBuffers(1, &VB);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		Initialized = true;
	}

	// ���_�̍쐬
	const Vertex v[4] = {
		{ Float3( x1,  y1, 0.0f),  color },
		{ Float3( x2,  y1, 0.0f),  color },
		{ Float3( x1,  y2, 0.0f),  color },
		{ Float3( x2,  y2, 0.0f),  color },
	};

	//! �J���[�V�F�[�_�[�ɕύX
	GmShader()->changeShader(SystemShader::SHADER_RENDER_2D);

	//---- �`��
	setBlendMode(BM_BLEND_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);

	// ���_�o�b�t�@�o�C���h
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	// �f�[�^�]��
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 4, v);

	GmShader()->setAndEnableVertexAttribute("position"		, 3, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, _position));
	GmShader()->setAndEnableVertexAttribute("color"			, 4, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, _color));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	GmShader()->DisableVertexAttribute("position");
	GmShader()->DisableVertexAttribute("color");

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// ���ɖ߂�
	setBlendMode(BM_NONE);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	GmShader()->beginPrevShader();
	
}


//-----------------------------------------------------------------------------
//! FPS�擾
//-----------------------------------------------------------------------------
float	getGLFps()
{
	f32 resultFps = 0;
	static s32 GLframe;
    static u32 GLtimebase;
    u32		   GLtimenow;
 
    GLframe++;
    GLtimenow = glutGet(GLUT_ELAPSED_TIME);
 
    if (GLtimenow - GLtimebase > 1000) {
        resultFps = GLframe * 1000.0f / (GLtimenow - GLtimebase);
        GLtimebase = GLtimenow;
        GLframe = 0;
    }
	return resultFps;
}

//-----------------------------------------------------------------------------
//! ������\��
//-----------------------------------------------------------------------------
void render_string(float x, float y, std::string const& str)
{
    float z = 0.0f;
    glRasterPos3f(x, y, z);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)str.c_str());
}


//-----------------------------------------------------------------------------
//! �_�̕`��
//-----------------------------------------------------------------------------
void DebugPointRender(Vector3& pos)
{
	/* �_�ɃA���`�G���A�V���O�������s�� */
	glEnable(GL_POINT_SMOOTH);

	/* �A���`�G���A�V���O�����̕i�����ō��ɂ��� */
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	
	/* �A���t�@�u�����f�B���O��L���ɂ��� */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);
	setBlendMode(BM_ADD_ALPHA);

	glPointSize(10);
	glPushMatrix();
		glBegin(GL_POINTS);
		glVertex3fv((GLfloat*)&pos);
		glEnd();
	glPopMatrix();

	//glDisable(GL_BLEND);
	setBlendMode(BM_NONE);
	glDisable(GL_POINT_SMOOTH);
}


//-----------------------------------------------------------------------------
// GL�̃G���[�����o
//-----------------------------------------------------------------------------
bool getGLError()
{

#if 1
	GLenum error = glGetError();

	switch( error )
	{
		case GL_NO_ERROR:			// 0
			return true;
			break;
		case GL_INVALID_ENUM:		// 1280 
			MessageBoxA(NULL, "�L����enum�ł͂���܂���(GL_INVALID_ENUM)", "GL�G���[", MB_OK);
			break;
		case GL_INVALID_VALUE:		// 1281
			MessageBoxA(NULL, "�L���Ȓl�ł͂���܂���(GL_INVALID_VALUE)", "GL�G���[", MB_OK);
			break;
		case GL_INVALID_OPERATION:	// 1282
			MessageBoxA(NULL, "�L���ȃI�y���[�V�����ł͂���܂���(GL_INVALID_OPERATION)", "GL�G���[", MB_OK);
			break;
		case GL_STACK_OVERFLOW:		// 1283
			MessageBoxA(NULL, "�X�^�b�N�ő�𒴂��܂���(GL_STACK_OVERFLOW)", "GL�G���[", MB_OK);
			break;
		case GL_STACK_UNDERFLOW:	// 1284
			MessageBoxA(NULL, "�X�^�b�N�ŏ��������܂���(GL_STACK_UNDERFLOW)", "GL�G���[", MB_OK);
			break;
		case GL_OUT_OF_MEMORY:		// 1285
			MessageBoxA(NULL, "�������O�A�N�Z�X�����܂���(GL_OUT_OF_MEMORY)", "GL�G���[", MB_OK);
			break;
		default:
			MessageBoxA(NULL, "UNKNOWN", "GL�G���[", MB_OK);
			break;
	}

	assert(false);
#endif
	return false;
}


namespace common {

//-----------------------------------------------------------------------------
//! int -> float�ϊ�
//!	@param	[in]	val		�ϊ���
//-----------------------------------------------------------------------------
f32	IntToFloat(u32 val)
{
	return  (f32)val * (1.0f / 255.0f);
}

//-----------------------------------------------------------------------------
//! Byte -> float�ϊ�
//!	@param	[in]	val		�ϊ���
//-----------------------------------------------------------------------------
f32	ByteToFloat(u8 val)
{
	return  (f32)val * (1.0f / 255.0f);
}

//-----------------------------------------------------------------------------
//! 32bitfloat -> 16bitfloat�ϊ�
//!	@param	[in]	val		�ϊ���
// �Q��URL : http://www.platinumgames.co.jp/programmer_blog/?p=484
//-----------------------------------------------------------------------------
u16 Float32to16(f32 val)
{
	
	FloatComp comp;
	// float��������
	comp.f = val;

	//-------------------------------------------------------------
	// �e���𕪗� + �����������k
	//-------------------------------------------------------------
	// ������
	s32	sign		= (comp.u >> 16) & 0x8000;	// 16�r�b�g�E�V�t�g���čŏ��bit���擾����
	// �w����
	s32	exponent	= (comp.u >> 23) & 0x00ff;	// 23bit�E�V�t�g���ĉ���8bit���擾����
	// ������(���k���s��)
	s32	significand	= (comp.u >> 13) & 0x03ff;	// 13bit�E�V�t�g���ĉ���13bit�����A����10bit���擾����


	//-------------------------------------------------------------
	// �w�����̌���
	//-------------------------------------------------------------
	// float �̌v�Z
	// 2.5 = (-1)^������ * 2^(�w����-�o�C�A�X(127)) * (1 + ������(0.25));
	exponent = exponent - 127;	// ����32bit�̏�ԂȂ̂�-127����2�̉���Ȃ̂����߂�
	exponent = exponent + 15;	// 15(�o�C�A�X�l)�𑫂���16bit�ł̎w�����ɂ���

	//-------------------------------------------------------------
	// ����
	//-------------------------------------------------------------
	// �w������10bit�V�t�g���ď��5bit�擾����ȊO�͕��ʂɍ���
	return sign | ( (exponent << 10) & 0x7c00 ) | significand;
}



}	//	namespace common