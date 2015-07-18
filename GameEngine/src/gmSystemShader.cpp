//-----------------------------------------------------------------------------
//!
//!	@file	gmSystemShader.h
//!	@brief	�V�F�[�_�Ǘ��V�X�e��
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
// �ÓI�ϐ��̐錾
//-----------------------------------------------------------------------------
ProgramShader SystemShader::_program[SHADER_MAX];		//!< �K�p�p�̃v���O����

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
SystemShader::SystemShader()
: _matrixBindingPoint(1)
, _currentProgram	 (SHADER_TEXTURE)
, _nullTexture		 (nullptr)
, _nullSpecular		 (nullptr)
, _nullNormal		 (nullptr)
{
	SAFE_NEW(_nullTexture);
	_nullTexture->loadAndTexImage2D("../Image/nullTexture.jpg");
	SAFE_NEW(_nullSpecular);
	_nullSpecular->loadAndTexImage2D("../Image/nullSpecular.jpg");
	SAFE_NEW(_nullNormal);
	_nullNormal->loadAndTexImage2D("../Image/nullNormal.jpg");

}
	
//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
SystemShader::~SystemShader()
{
	SAFE_DELETE(_nullTexture);
	SAFE_DELETE(_nullSpecular);
	SAFE_DELETE(_nullNormal);
}


//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool SystemShader::init()
{
	char buf[255];

	//! GLSL�̏�����
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		return false;
	}

	sprintf_s(buf, "OpenGLVersion : (%s)", glGetString(GL_VERSION));

	sprintf_s(buf, "OpenGLVersion : (%s)", glGetString(GL_SHADING_LANGUAGE_VERSION));


	VertexShader	allVertex;
	allVertex.LoadAndCompile("Test.vert");
	FragmentShader	allFrag;
	allFrag.LoadAndCompile("Test.frag");

	//---- �f�o�b�O�`��
	VertexShader	normalVertex;
	//normalVertex.LoadAndCompile("noTexture.vert");
	normalVertex.LoadAndCompile("debugDraw.vert");
	FragmentShader	normalFrag;
	//normalFrag.LoadAndCompile("noTexture.frag");
	normalFrag.LoadAndCompile("debugDraw.frag");

	//---- �V���h�E�������ݗp
	VertexShader	shadowVertex;
	shadowVertex.LoadAndCompile("Shadow.vert");
	FragmentShader	shadowFrag;
	shadowFrag.LoadAndCompile("Shadow.frag");

	//---- �e�N�X�`���f�o�b�O�p
	VertexShader	debugVertex;
	debugVertex.LoadAndCompile("debugTexture.vert");
	FragmentShader	debugFrag;
	debugFrag.LoadAndCompile("debugTexture.frag");

	//---- �L���[�u�}�b�v�`��p
	VertexShader	drawCubeMapVertex;
	drawCubeMapVertex.LoadAndCompile("drawCubeMap.vert");
	FragmentShader	drawCubeMapFrag;
	drawCubeMapFrag.LoadAndCompile("drawCubeMap.frag");

	//---- �ʏ�`��p(�e�N�X�`������)
	VertexShader	standardVertex;
	standardVertex.LoadAndCompile("standard.vert");
	FragmentShader	standardFrag;
	standardFrag.LoadAndCompile("standard.frag");

	//---- �ʏ�`��p(�e�N�X�`���Ȃ�)
	FragmentShader  standardColorFrag;
	standardColorFrag.LoadAndCompile("standardColor.frag");

	//---- 2D�`��p
	VertexShader	filterVertex;
	filterVertex.LoadAndCompile("Filter.vert");
	FragmentShader	filterFrag;
	filterFrag.LoadAndCompile("Filter.frag");

	//---- ���[�V�����u���[�p
	VertexShader	motionBlurVertex;
	motionBlurVertex.LoadAndCompile("MotionBlur.vert");
	FragmentShader	motionBlurFrag;
	motionBlurFrag.LoadAndCompile("MotionBlur.frag");

	//---- �u���[���p�ڂ����t�B���^�[
	VertexShader	gaussian3x3_V_Vertex;
	gaussian3x3_V_Vertex.LoadAndCompile("Gaussian3x3_V.vert");
	VertexShader	gaussian3x3_H_Vertex;
	gaussian3x3_H_Vertex.LoadAndCompile("Gaussian3x3_H.vert");
	FragmentShader	gaussian3x3_Frag;
	gaussian3x3_Frag.LoadAndCompile("Gaussian3x3.frag");
	//---- �u���[���t�B���^�[
	FragmentShader	bloomGlareFrag;
	bloomGlareFrag.LoadAndCompile("BloomGlare.frag");

	//---- ���Z�t�B���^�[
	FragmentShader	subtractFrag;
	subtractFrag.LoadAndCompile("Subtract.frag");

	//---- �g�[���}�b�s���O�p
	VertexShader	getLuminanceVertex;
	getLuminanceVertex.LoadAndCompile("GetLuminance.vert");
	FragmentShader	getLuminanceFrag;
	getLuminanceFrag.LoadAndCompile("GetLuminance.frag");

	VertexShader	luminanceReductionVertex;
	luminanceReductionVertex.LoadAndCompile("LuminanceReduction.vert");
	FragmentShader	luminanceReductionFrag;
	luminanceReductionFrag.LoadAndCompile("LuminanceReduction.frag");

	FragmentShader	lightAdaptationFrag;
	lightAdaptationFrag.LoadAndCompile("LightAdaptation.frag");

	FragmentShader	toneMappingFinalFrag;
	toneMappingFinalFrag.LoadAndCompile("ToneMappingFinal.frag");

	//---- �O���A�p
	VertexShader	glareFilterVertex;
	glareFilterVertex.LoadAndCompile("GlareFilter.vert");
	FragmentShader	glareFilterFrag;
	glareFilterFrag.LoadAndCompile("GlareFilter.frag");

	//---- �����Y�t���A�p
	FragmentShader	lensFlareFrag;
	lensFlareFrag.LoadAndCompile("LensFlare.frag");
	FragmentShader	lensDirtStarFrag;
	lensDirtStarFrag.LoadAndCompile("AddLensEffects.frag");

	VertexShader	anamolFlareFilterVertex;
	anamolFlareFilterVertex.LoadAndCompile("AnamolphicFlareFilter.vert");
	FragmentShader	anamolFlareFilterFrag;
	anamolFlareFilterFrag.LoadAndCompile("AnamolphicFlareFilter.frag");

	VertexShader	ExpandBlurVertex;
	ExpandBlurVertex.LoadAndCompile("ExpandBlur.vert");
	FragmentShader	ExpandBlurFrag;
	ExpandBlurFrag.LoadAndCompile("ExpandBlur.frag");


	// �W���`��
	if( !InitShader(SHADER_STANDARD,			standardVertex,				standardFrag) )				return false;
	getGLError();
	// �W���`��(�e�N�X�`���Ȃ�)
	if( !InitShader(SHADER_COLOR,				standardVertex,				standardColorFrag) )		return false;
	getGLError();
	// �ʏ�`��p
	if( !InitShader(SHADER_NORMAL,				normalVertex,				normalFrag) )				return false;
	getGLError();
	// ���ׂẴG�t�F�N�g�p
	if( !InitShader(SHADER_TEXTURE,				allVertex,					allFrag) )					return false;
	getGLError();
	// �V���h�E�������ݗp
	if( !InitShader(SHADER_SHADOW,				shadowVertex,				shadowFrag) )				return false;
	getGLError();
	// �f�o�b�O�`��p
	if( !InitShader(SHADER_DEBUG,				debugVertex,				debugFrag) )				return false;
	getGLError();
	// �L���[�u�}�b�v�`��p
	if( !InitShader(SHADER_DRAWCUBEMAP,			drawCubeMapVertex,			drawCubeMapFrag) )			return false;
	getGLError();
	// �|�X�g�G�t�F�N�g�p(�~�b�v�}�b�v�t��)
	if( !InitShader(SHADER_FILTER,				filterVertex,				filterFrag) )				return false;
	getGLError();
	// �����u���[
	if( !InitShader(SHADER_GAUSSIAN_3X3_H,		gaussian3x3_H_Vertex,		gaussian3x3_Frag) )			return false;
	getGLError();
	// �����u���[
	if( !InitShader(SHADER_GAUSSIAN_3X3_V,		gaussian3x3_V_Vertex,		gaussian3x3_Frag) )			return false;
	getGLError();
	// ���Z
	if( !InitShader(SHADER_SUBTRACT,			filterVertex,				subtractFrag) )				return false;
	getGLError();
	// �u���[���O���A
	if( !InitShader(SHADER_BLOOM_GLARE,			filterVertex,				bloomGlareFrag) )			return false;
	getGLError();
	// �ŏ��̋P�x�𐶐�
	if( !InitShader(SHADER_GET_LUMINANCE,		getLuminanceVertex,			getLuminanceFrag) )			return false;
	getGLError();
	// �k�����Ă���
	if( !InitShader(SHADER_LUMINANCE_REDUCTION,	luminanceReductionVertex,	luminanceReductionFrag) )	return false;
	getGLError();
	// ������ / �Ï���
	if( !InitShader(SHADER_LIGHT_ADAPTATION,	filterVertex,				lightAdaptationFrag) )		return false;
	getGLError();
	// �g�[���}�b�s���O�ŏI
	if( !InitShader(SHADER_TONEMAPPING_FINAL,	filterVertex,				toneMappingFinalFrag) )		return false;
	getGLError();
	// �O���A
	if( !InitShader(SHADER_GLARE,				glareFilterVertex,			glareFilterFrag) )			return false;
	getGLError();
	// ���[�V�����u���[
	if( !InitShader(SHADER_MOTION_BLUR,			motionBlurVertex,			motionBlurFrag) )			return false;
	getGLError();
	// 2D�`��p(�e�N�X�`������)
	if( !InitShader(SHADER_PUT_TEXTURE,			filterVertex,				standardFrag) )				return false;
	getGLError();
	// 2D�`��p(�e�N�X�`���Ȃ�)
	if( !InitShader(SHADER_RENDER_2D,			filterVertex,				standardColorFrag) )		return false;
	getGLError();
	// �����Y�t���A
	if( !InitShader(SHADER_LENS_FLARE,			filterVertex,				lensFlareFrag) )			return false;
	getGLError();
	// �����Y����A�X�^�[�o�[�X�g
	if( !InitShader(SHADER_LENS_DIRT_STAR,		filterVertex,				lensDirtStarFrag) )			return false;
	getGLError();
	// �A�i�����t�B�b�N�̃t���A
	if( !InitShader(SHADER_ANAMOLPHIC_FLARE,	anamolFlareFilterVertex,	anamolFlareFilterFrag) )	return false;
	getGLError();
	// 4�_0.25�t�F�b�`�g��u���[
	if( !InitShader(SHADER_EXPAND_BLUR,			ExpandBlurVertex,			ExpandBlurFrag) )			return false;

	getGLError();
	
	
	


	////! �V�F�[�_�v���O�����̏�����
	//for( s32 i=0; i<SHADER_MAX; i++ )
	//{
	//	if( !InitShader( (SHADER_MODE)(i) ) ) return false;
	//}

	// UniformBlock������
	InitUniformBlock();

	// �R���e�L�X�g������
	InitContext();

	_changeShader = false;

	return true;
}

//-----------------------------------------------------------------------------
//! ���
//-----------------------------------------------------------------------------
void SystemShader::cleanup()
{
}

//-----------------------------------------------------------------------------
//! �V�F�[�_�v���O�����̏�����
//! �V�F�[�_�v���O�����̏�����
//!	@param	[in]	mode	�V�F�[�_�̃��[�h
//!	@param	[in]	vertex	�o�[�e�b�N�X�V�F�[�_
//!	@param	[in]	vertex	�t���O�����g�V�F�[�_
//-----------------------------------------------------------------------------
bool SystemShader::InitShader(SHADER_MODE mode, VertexShader& vertex, FragmentShader& frag)
{
	if( !_program[mode].createProgram(vertex.getShader(), frag.getShader()) ) return false;
	return true;
}

//-----------------------------------------------------------------------------
//! UniformBlock�̏�����
//-----------------------------------------------------------------------------
bool SystemShader::InitUniformBlock()
{
	//---- �g���V�F�[�_��UniformBlock���o�C���h����
	_program[SHADER_TEXTURE].BindUniformBlockPoint("Matrices", _matrixBindingPoint);
	_program[SHADER_SHADOW].BindUniformBlockPoint("Matrices", _matrixBindingPoint);
	_program[SHADER_DRAWCUBEMAP].BindUniformBlockPoint("Matrices", _matrixBindingPoint);
	_program[SHADER_STANDARD].BindUniformBlockPoint("Matrices", _matrixBindingPoint);
	_program[SHADER_COLOR].BindUniformBlockPoint("Matrices", _matrixBindingPoint);

	GLsizei dataSize = ( sizeof(Matrix) * 3 );

	// �o�b�t�@�m��
	glGenBuffers(1, &_uniformBuffer);
	// �R�t��
	glBindBuffer(GL_UNIFORM_BUFFER, _uniformBuffer);
	// �f�[�^�]���̈�m��
	glBufferData(GL_UNIFORM_BUFFER, dataSize, nullptr, GL_DYNAMIC_DRAW);
	// �o�b�t�@�ƃo�C���h�|�C���g���Ђ��t��
	glBindBufferBase(GL_UNIFORM_BUFFER, _matrixBindingPoint, _uniformBuffer);


	// �R�t���J��
	glBindBuffer(GL_UNIFORM_BUFFER, 0 );

	
	return true;
}

//-----------------------------------------------------------------------------
//! �R���e�L�X�g�̏�����
//-----------------------------------------------------------------------------
bool SystemShader::InitContext()
{
	_normalContext.init(_program[SHADER_TEXTURE].getShaderProgram());
	_shadowContext.init(_program[SHADER_SHADOW].getShaderProgram());
	_filterContext.init(_program[SHADER_PUT_TEXTURE].getShaderProgram());

	return true;
}

//-----------------------------------------------------------------------------
//! �V�F�[�_�̊J�n
//-----------------------------------------------------------------------------
void SystemShader::beginShader()
{
	if( _currentProgram == SHADER_NONE )
	{
		endShader();
	}else {
		//! �V�F�[�_�v���O�����̓K�p
		glUseProgram(_program[_currentProgram].getShaderProgram());
	}
}

//-----------------------------------------------------------------------------
//! �ЂƂO�̃V�F�[�_�̊J�n
//-----------------------------------------------------------------------------
void SystemShader::beginPrevShader()
{
	// �ύX�t���O��OFF�Ȃ珈�����Ȃ�
	if( !_changeShader ) return;
	// ���̃v���O�����Ƃ��ď㏑��
	_currentProgram = _prevProgram;
	beginShader();
	// �ύX�t���O��OFF�ɂ���
	_changeShader = false;
}

//-----------------------------------------------------------------------------
//! �V�F�[�_�̏I��
//-----------------------------------------------------------------------------
void SystemShader::endShader()
{
	setPass(SHADER_NONE);
	//! �V�F�[�_�v���O�����̓K�p
	glUseProgram(GL_NONE);

}

//-----------------------------------------------------------------------------
//! �V�F�[�_�v���O�����ύX
//!	@param	[in]	mode	�V�F�[�_���[�h
//-----------------------------------------------------------------------------
void SystemShader::changeShader(SHADER_MODE mode)
{
	// �������̂Ȃ̂ŏ������Ȃ�
	if( _currentProgram == mode ) return;
	// �`�F���W�t���OON
	_changeShader = true;
	// �V�����V�F�[�_��ݒ�
	setPass(mode);
	// �V�F�[�_���J�n
	beginShader();
}



//-----------------------------------------------------------------------------
//! �V�F�[�_�̎�ސݒ�
//!	@param	[in]	mode	�V�F�[�_���[�h
//-----------------------------------------------------------------------------
void SystemShader::setPass(SHADER_MODE mode)
{
	_prevProgram	= _currentProgram;
	_currentProgram = mode;
}

//=============================================================================
// Uniform�n�ݒ�
//=============================================================================

//-----------------------------------------------------------------------------
//! �}�g���b�N�X�f�[�^��]��
//!	@param	[in]	data		�s��f�[�^
//!	@param	[in]	dataSize	�f�[�^�T�C�Y
//-----------------------------------------------------------------------------
void SystemShader::setUniformMatrixBuffer(Matrix* data, GLsizei dataSize)
{

	// �R�t��
	glBindBuffer(GL_UNIFORM_BUFFER, _uniformBuffer);
	
	glBufferSubData(GL_UNIFORM_BUFFER, 0, dataSize, data);

	// �R�t���J��
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	
}

//-----------------------------------------------------------------------------
//! Float�̃��j�t�H�[���ϐ���ݒ�
//!	@param	[in]	name	�ϐ���	
//!	@param	[in]	data	�f�[�^
//-----------------------------------------------------------------------------
void SystemShader::setUniform1f(const GLchar* name, GLfloat data)
{
	if(_currentProgram == SHADER_NONE) return;
	GLint handle = glGetUniformLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	glUniform1f(handle, data);
}

//-----------------------------------------------------------------------------
//! Float�̃��j�t�H�[���ϐ���ݒ�
//!	@param	[in]	name	�ϐ���	
//!	@param	[in]	data	�f�[�^
//-----------------------------------------------------------------------------
void SystemShader::setUniform1fv(const GLchar* name, GLfloat* data, GLsizei count)
{
	if(_currentProgram == SHADER_NONE) return;
	GLint handle = glGetUniformLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	glUniform1fv(handle, count, data);
}

//-----------------------------------------------------------------------------
//! Float2�̃��j�t�H�[���ϐ���ݒ�
//!	@param	[in]	name	�ϐ���	
//!	@param	[in]	data	�f�[�^
//!	@param	[in]	count	�f�[�^�̌�
//-----------------------------------------------------------------------------
void SystemShader::setUniform2fv(const GLchar* name, GLfloat* data, GLsizei count)
{
	if(_currentProgram == SHADER_NONE) return;
	GLint handle = glGetUniformLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	glUniform2fv(handle, count, data);
}

//-----------------------------------------------------------------------------
//! Float3�̃��j�t�H�[���ϐ���ݒ�
//!	@param	[in]	name	�ϐ���	
//!	@param	[in]	data	�f�[�^
//!	@param	[in]	count	�f�[�^�̌�
//-----------------------------------------------------------------------------
void SystemShader::setUniform3fv(const GLchar* name, GLfloat* data, GLsizei count)
{
	if(_currentProgram == SHADER_NONE) return;
	GLint handle = glGetUniformLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	glUniform3fv(handle, count, data);
}

//-----------------------------------------------------------------------------
//! Float4�̃��j�t�H�[���ϐ���ݒ�
//!	@param	[in]	name	�ϐ���	
//!	@param	[in]	data	�f�[�^
//!	@param	[in]	count	�f�[�^�̌�
//-----------------------------------------------------------------------------
void SystemShader::setUniform4fv(const GLchar* name, GLfloat* data, GLsizei count)
{
	if(_currentProgram == SHADER_NONE) return;
	GLint handle = glGetUniformLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	glUniform4fv(handle, count, data);
}

//-----------------------------------------------------------------------------
//! int�̃��j�t�H�[���ϐ���ݒ�
//!	@param	[in]	name	�ϐ���	
//!	@param	[in]	data	�f�[�^
//-----------------------------------------------------------------------------
void SystemShader::setUniform1i(const GLchar* name, GLint data)
{
	if (_currentProgram == SHADER_NONE) return;
	GLint handle = glGetUniformLocation(_program[_currentProgram].getShaderProgram(), name);
	if (handle == -1) return;
	glUniform1i(handle, data);
}

//-----------------------------------------------------------------------------
//! �s���ݒ�
//!	@param	[in]	name		�ϐ���	
//!	@param	[in]	dataCount	�s��̐�
//!	@param	[in]	transpose	�]�u���邩�ǂ���
//!	@param	[in]	data		�f�[�^
//-----------------------------------------------------------------------------
void SystemShader::setMatrix4x4(const GLchar* name, GLuint dataCount, GLboolean transpose, GLfloat* data)
{
	if(_currentProgram == SHADER_NONE) return;

	GLint shaderHandle = _program[_currentProgram].getShaderProgram();
	// �n���h���擾
	int handle = glGetUniformLocation(shaderHandle, name);
	if( handle == -1 ) return;
	// �s��]��
	glUniformMatrix4fv(handle, dataCount, transpose,  data);
}

//-----------------------------------------------------------------------------
//! �e�N�X�`���ݒ�
//!	@param	[in]	name		�ϐ���	
//!	@param	[in]	texHandle	�e�N�X�`���̃n���h��
//!	@param	[in]	texNum		�L��������e�N�X�`���̔ԍ�(GL_TEXTURE0���Ȃ�0)
//-----------------------------------------------------------------------------
void SystemShader::bindTexture(const GLchar* name, GLuint texHandle, GLuint texNum, GLenum target)
{
	if(_currentProgram == SHADER_NONE) return;

	GM_ASSERT(texNum <= 31, "�e�N�X�`���̗L���ԍ����z���Ă��܂�");

	GLint shaderHandle = _program[_currentProgram].getShaderProgram();
	// �n���h���擾
	int handle = glGetUniformLocation(shaderHandle, name);
	if( handle == -1 ) return;
	glActiveTexture(GL_TEXTURE0 + texNum);
	// �e�N�X�`���]��
	glBindTexture(target, texHandle);
	// �n���h���ɔԍ����Ђ��t��
	glUniform1i(handle, texNum);
}

//-----------------------------------------------------------------------------
//! �e�N�X�`���ݒ����
//!	@param	[in]	target		�A���o�C���h����^�[�Q�b�g
//-----------------------------------------------------------------------------
void SystemShader::unbindTexture(GLenum target)
{
	if(_currentProgram == SHADER_NONE) return;

	glActiveTexture(GL_TEXTURE0);
	// �e�N�X�`���]��
	glBindTexture(target, 0);
	
}


//=============================================================================
// in(attrib)�n�ݒ�
//=============================================================================

//-----------------------------------------------------------------------------
//! �o�[�e�b�N�X�ϐ���ݒ肷��(1fv)
//!	@param	[in]	name	�ϐ���	
//!	@param	[in]	data	�f�[�^
//-----------------------------------------------------------------------------
void SystemShader::setVertex1fv(const char* name, GLfloat* data)
{
	if(_currentProgram == SHADER_NONE) return;

	// �n���h���擾
	GLint handle = glGetAttribLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	// �o�[�e�b�N�X�ϐ��]��
	glVertexAttrib1fv(handle, data);
}


//-----------------------------------------------------------------------------
//! �o�[�e�b�N�X�ϐ���ݒ肷��(2fv)
//!	@param	[in]	name	�ϐ���	
//!	@param	[in]	data	�f�[�^
//-----------------------------------------------------------------------------
void SystemShader::setVertex2fv(const char* name, GLfloat* data)
{
	if(_currentProgram == SHADER_NONE) return;

	// �n���h���擾
	GLint handle = glGetAttribLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	// �o�[�e�b�N�X�ϐ��]��
	glVertexAttrib2fv(handle, data);
}

//-----------------------------------------------------------------------------
//! �o�[�e�b�N�X�ϐ���ݒ肷��(3fv)
//!	@param	[in]	name	�ϐ���	
//!	@param	[in]	data	�f�[�^
//-----------------------------------------------------------------------------
void SystemShader::setVertex3fv(const char* name, GLfloat* data)
{
	if(_currentProgram == SHADER_NONE) return;

	// �n���h���擾
	GLint handle = glGetAttribLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	// �o�[�e�b�N�X�ϐ��]��
	glVertexAttrib3fv(handle, data);
}


//-----------------------------------------------------------------------------
//! �o�[�e�b�N�X�ϐ���ݒ肷��(4fv)
//!	@param	[in]	name	�ϐ���	
//!	@param	[in]	data	�f�[�^
//-----------------------------------------------------------------------------
void SystemShader::setVertex4fv(const GLchar* name, GLfloat* data)
{
	if(_currentProgram == SHADER_NONE) return;

	// �n���h���擾
	GLint handle = glGetAttribLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	// �o�[�e�b�N�X�ϐ��]��
	glVertexAttrib4fv(handle, data);
}



//-----------------------------------------------------------------------------
//! �o�[�e�b�N�X�ϐ���ݒ肵�L���ɂ���
//!	@param	[in]	name	�ϐ���
//!	@param	[in]	size	�f�[�^�ЂƂ��̌�
//!	@param	[in]	size	�f�[�^�^�C�v
//!	@param	[in]	size	�f�[�^�ЂƂ��Ƃ̃X���C�h��
//!	@param	[in]	offset	�f�[�^�̊J�n�ʒu
//-----------------------------------------------------------------------------
void SystemShader::setAndEnableVertexAttribute(const GLchar* name, GLint size, GLenum type, GLsizei stride, GLvoid* offset)
{
	if(_currentProgram == SHADER_NONE) return;
	// �n���h���擾
	GLint handle = glGetAttribLocation(_program[_currentProgram].getShaderProgram(), name);
//	getGLError();
	// �f�[�^�]���ƗL����
	setAndEnableVertexAttributeWithHandle(handle, size, type, stride, offset);
//	getGLError();
}
//-----------------------------------------------------------------------------
//! �o�[�e�b�N�X�ϐ���ݒ肵�L���ɂ���(�n���h����)
//!	@param	[in]	handle	�ϐ��̃n���h��
//!	@param	[in]	size	�f�[�^�ЂƂ��̌�
//!	@param	[in]	size	�f�[�^�^�C�v
//!	@param	[in]	size	�f�[�^�ЂƂ��Ƃ̃X���C�h��
//!	@param	[in]	offset	�f�[�^�̊J�n�ʒu
//-----------------------------------------------------------------------------
void SystemShader::setAndEnableVertexAttributeWithHandle(GLint handle, GLint size, GLenum type, GLsizei stride, GLvoid* offset)
{
	if( handle == -1 ) return;
	if(_currentProgram == SHADER_NONE) return;
	// �o�[�e�b�N�X�z���]��
	glVertexAttribPointer(handle, size, type, GL_FALSE, stride, offset);
	// �o�[�e�b�N�X�z���L����
	glEnableVertexAttribArray(handle);

//	getGLError();
}

//-----------------------------------------------------------------------------
//! �ݒ�𖳌��ɂ���
//!	@param	[in]	name	�ϐ���
//-----------------------------------------------------------------------------
void SystemShader::DisableVertexAttribute(const GLchar* name)
{
	if(_currentProgram == SHADER_NONE) return;

	// �n���h���擾
	GLint handle = glGetAttribLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	// �ݒ肵���o�[�e�b�N�X�z��̖�����
	DisableVertexAttributeWithHandle(handle);
}

//-----------------------------------------------------------------------------
//! �ݒ�𖳌��ɂ���(�n���h����)
//!	@param	[in]	name	�ϐ���
//-----------------------------------------------------------------------------
void SystemShader::DisableVertexAttributeWithHandle(GLint handle)
{
	if( handle == -1 ) return;
	if(_currentProgram == SHADER_NONE) return;
	// �ݒ肵���o�[�e�b�N�X�z��̖�����
	glDisableVertexAttribArray(handle);
}




//=============================================================================
// END OF FILE
//=============================================================================