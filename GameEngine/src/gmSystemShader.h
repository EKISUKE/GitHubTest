//-----------------------------------------------------------------------------
//!
//!	@file	gmSystemShader.h
//!	@brief	�V�F�[�_�Ǘ��V�X�e��
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

// �V�F�[�_�t�@�C���܂ł̃p�X
#define SHADER_PATH	"shaderFile/"

//-----------------------------------------------------------------------------
//!	�V�F�[�_�Ǘ�
//-----------------------------------------------------------------------------
class SystemShader
{
public:
	//! �V�F�[�_�̎��
	enum SHADER_MODE
	{
		SHADER_STANDARD,
		SHADER_COLOR,
		SHADER_NORMAL,
		SHADER_TEXTURE,
		SHADER_SHADOW,
		SHADER_DEBUG,
		SHADER_DRAWCUBEMAP,
		SHADER_FILTER,
		SHADER_GAUSSIAN_3X3_H,
		SHADER_GAUSSIAN_3X3_V,
		SHADER_SUBTRACT,
		SHADER_BLOOM_GLARE,
		SHADER_GET_LUMINANCE,
		SHADER_LUMINANCE_REDUCTION,
		SHADER_LIGHT_ADAPTATION,
		SHADER_TONEMAPPING_FINAL,
		SHADER_GLARE,
		SHADER_MOTION_BLUR,
		SHADER_PUT_TEXTURE,
		SHADER_RENDER_2D,
		SHADER_LENS_FLARE,
		SHADER_LENS_DIRT_STAR,
		SHADER_ANAMOLPHIC_FLARE,
		SHADER_EXPAND_BLUR,

		SHADER_MAX,
		SHADER_NONE,
	};


	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{
private:

	//! �R���X�g���N�^
	SystemShader();
	//! �R�s�[�R���X�g���N�^
	SystemShader(SystemShader& src){}

	//! �f�X�g���N�^
	virtual ~SystemShader();
public:
	//! ������
	bool		init();

	//! ���
	void		cleanup();

	//! �V�F�[�_�v���O�����̏�����
	//!	@param	[in]	mode	�V�F�[�_�̃��[�h
	//!	@param	[in]	vertex	�o�[�e�b�N�X�V�F�[�_
	//!	@param	[in]	vertex	�t���O�����g�V�F�[�_
	bool		InitShader(SHADER_MODE mode, VertexShader& vertex, FragmentShader& frag);
	//! UniformBlock�̏�����
	bool		InitUniformBlock();
	//! �R���e�L�X�g�̏�����
	bool		InitContext();

	//@}
	//-------------------------------------------------------------
	//! @name �J�n�E�I���E�ύX
	//-------------------------------------------------------------
	//@{

	//! �V�F�[�_�̊J�n
	void		beginShader();
	//! �ЂƂO�̃V�F�[�_�̊J�n
	void		beginPrevShader();
	//! �V�F�[�_�̏I��
	void		endShader();

	//! �V�F�[�_�v���O�����ύX
	//!	@param	[in]	mode	�V�F�[�_���[�h
	void		changeShader(SHADER_MODE mode);

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �C���X�^���X�̎擾
	static SystemShader* getInstance() {
		static SystemShader	_myInstance;
		return &_myInstance;
	}

	//! �V�F�[�_�v���O�����̎擾
	//!	@param	[in]	mode	�V�F�[�_���[�h
	GLuint				getShaderProgram( SHADER_MODE mode ) { return _program[mode].getShaderProgram(); }
	//! ���݂̃V�F�[�_�v���O�����擾
	GLuint				getCurrentShaderPorgram() const { return _program[_currentProgram].getShaderProgram(); }
	//! �ЂƂO�̃V�F�[�_�v���O�����擾
	GLuint				getPrevShaderProgram()    const { return _program[_prevProgram].getShaderProgram(); }
	//! �}�g���b�N�X�̃o�C���h�|�C���g���擾
	GLuint				getMatrixBindingPoint()   const { return _matrixBindingPoint; }
	//! �ʏ�`��̃R���e�L�X�g�擾
	NormalContext		getNormalContext()	      const { return _normalContext; }
	//! �V���h�E�̃R���e�L�X�g�擾
	ShadowContext		getShadowContext()		  const { return _shadowContext; }
	//! �t�B���^�[�̃R���e�L�X�g�擾
	FilterContext		getFilterContext()		  const { return _filterContext; }
	//! �k���e�N�X�`���̃n���h���擾(�f�B�t���[�Y)
	GLuint				getNullTexHandle()		  const { return _nullTexture->getTextureHandle(); }
	//! �k���e�N�X�`���̃n���h���擾(�X�y�L����)
	GLuint				getNullSpecHandle()		  const { return _nullSpecular->getTextureHandle(); }
	//! �k���e�N�X�`���̃n���h���擾(�m�[�}��)
	GLuint				getNullNormHandle()		  const { return _nullNormal->getTextureHandle(); }

	//! �V�F�[�_�̎�ސݒ�
	//!	@param	[in]	mode	�V�F�[�_���[�h
	void				setPass(SHADER_MODE mode);
	

	//---- Uniform�n�ݒ�

	//! �}�g���b�N�X�f�[�^��]��
	//!	@param	[in]	data		�s��f�[�^
	//!	@param	[in]	dataSize	�f�[�^�T�C�Y
	void				setUniformMatrixBuffer(Matrix* data, GLsizei dataSize);
	
	//! Float�̃��j�t�H�[���ϐ���ݒ�
	//!	@param	[in]	name	�ϐ���	
	//!	@param	[in]	data	�f�[�^
	void				setUniform1f(const GLchar* name, GLfloat data);

	//! Float�̃��j�t�H�[���ϐ���ݒ�
	//!	@param	[in]	name	�ϐ���	
	//!	@param	[in]	data	�f�[�^
	void				setUniform1fv(const GLchar* name, GLfloat* data, GLsizei count = 1);

	//! Float2�̃��j�t�H�[���ϐ���ݒ�
	//!	@param	[in]	name	�ϐ���	
	//!	@param	[in]	data	�f�[�^
	//!	@param	[in]	count	�f�[�^�̌�
	void				setUniform2fv(const GLchar* name, GLfloat* data, GLsizei count = 1);

	//! Float3�̃��j�t�H�[���ϐ���ݒ�
	//!	@param	[in]	name	�ϐ���	
	//!	@param	[in]	data	�f�[�^
	//!	@param	[in]	count	�f�[�^�̌�
	void				setUniform3fv(const GLchar* name, GLfloat* data, GLsizei count = 1);

	//! Float4�̃��j�t�H�[���ϐ���ݒ�
	//!	@param	[in]	name	�ϐ���	
	//!	@param	[in]	data	�f�[�^
	//!	@param	[in]	count	�f�[�^�̌�
	void				setUniform4fv(const GLchar* name, GLfloat* data, GLsizei count = 1);

	//! int�̃��j�t�H�[���ϐ���ݒ�
	//!	@param	[in]	name	�ϐ���	
	//!	@param	[in]	data	�f�[�^
	void				setUniform1i(const GLchar* name, GLint data);


	//! �s���ݒ�
	//!	@param	[in]	name		�ϐ���	
	//!	@param	[in]	dataCount	�s��̐�
	//!	@param	[in]	transpose	�]�u���邩�ǂ���
	//!	@param	[in]	data		�f�[�^
	void				setMatrix4x4(const GLchar* name, GLuint dataCount, GLboolean transpose, GLfloat* data);
	
	//! �e�N�X�`���ݒ�
	//!	@param	[in]	name		�ϐ���	
	//!	@param	[in]	texHandle	�e�N�X�`���̃n���h��
	//!	@param	[in]	texNum		�L��������e�N�X�`���̔ԍ�(GL_TEXTURE0���Ȃ�0)
	void				bindTexture(const GLchar* name, GLuint texHandle, GLuint texNum, GLenum target = GL_TEXTURE_2D);


	//! �e�N�X�`���ݒ����
	//!	@param	[in]	target		�A���o�C���h����^�[�Q�b�g
	void				unbindTexture(GLenum target = GL_TEXTURE_2D);

	//---- in(attrib)�n�ݒ�


	//! �o�[�e�b�N�X�ϐ���ݒ肷��(1fv)
	//!	@param	[in]	name	�ϐ���	
	//!	@param	[in]	data	�f�[�^
	void				setVertex1fv(const GLchar* name, GLfloat* data);

	//! �o�[�e�b�N�X�ϐ���ݒ肷��(2fv)
	//!	@param	[in]	name	�ϐ���	
	//!	@param	[in]	data	�f�[�^
	void				setVertex2fv(const GLchar* name, GLfloat* data);

	//! �o�[�e�b�N�X�ϐ���ݒ肷��(4fv)
	//!	@param	[in]	name	�ϐ���	
	//!	@param	[in]	data	�f�[�^
	void				setVertex4fv(const GLchar* name, GLfloat* data);

	//! �o�[�e�b�N�X�ϐ���ݒ肷��(3fv)
	//!	@param	[in]	name	�ϐ���	
	//!	@param	[in]	data	�f�[�^
	void				setVertex3fv(const GLchar* name, GLfloat* data);
	
	//! �o�[�e�b�N�X�ϐ���ݒ肵�L���ɂ���
	//!	@param	[in]	name	�ϐ���
	//!	@param	[in]	size	�f�[�^�ЂƂ��̌�
	//!	@param	[in]	size	�f�[�^�^�C�v
	//!	@param	[in]	size	�f�[�^�ЂƂ��Ƃ̃X���C�h��
	//!	@param	[in]	offset	�f�[�^�̊J�n�ʒu
	void				setAndEnableVertexAttribute(const GLchar* name, GLint size, GLenum type, GLsizei stride, GLvoid* offset);

	//! �o�[�e�b�N�X�ϐ���ݒ肵�L���ɂ���(�n���h����)
	//!	@param	[in]	handle	�ϐ��̃n���h��
	//!	@param	[in]	size	�f�[�^�ЂƂ��̌�
	//!	@param	[in]	size	�f�[�^�^�C�v
	//!	@param	[in]	size	�f�[�^�ЂƂ��Ƃ̃X���C�h��
	//!	@param	[in]	offset	�f�[�^�̊J�n�ʒu
	void				setAndEnableVertexAttributeWithHandle(GLint handle, GLint size, GLenum type, GLsizei stride, GLvoid* offset);
	
	//! �ݒ�𖳌��ɂ���
	//!	@param	[in]	name	�ϐ���
	void				DisableVertexAttribute(const GLchar* name);
	
	//! �ݒ�𖳌��ɂ���(�n���h����)
	//!	@param	[in]	name	�ϐ���
	void				DisableVertexAttributeWithHandle(GLint handle);
	
	

	//@}
private:

	static ProgramShader	_program[SHADER_MAX];
	SHADER_MODE				_currentProgram;					//!< ���ݎg�p���̃v���O����
	SHADER_MODE				_prevProgram;						//!< �ЂƂO�̃V�F�[�_�v���O����
	GLuint					_matrixBindingPoint;				//!< �s��̃o�C�g�|�C���g
	GLuint					_uniformBuffer;						//!< �o�b�t�@�f�[�^
	bool					_changeShader;						//!< �V�F�[�_�`�F���W���Ă邩�ǂ���

	NormalContext			_normalContext;						//!< �ʏ�`��p�R���e�L�X�g
	ShadowContext			_shadowContext;						//!< �V���h�E�p�R���e�L�X�g
	FilterContext			_filterContext;						//!< �t�B���^�[�p�R���e�L�X�g
	Texture*				_nullTexture;						//!< �k���e�N�X�`��(�f�B�t���[�Y)
	Texture*				_nullSpecular;						//!< �k���e�N�X�`��(�X�y�L����)
	Texture*				_nullNormal;						//!< �k���e�N�X�`��(�m�[�}��)
};

//-----------------------------------------------------------------------------
// �A�N�Z�X�C���^�[�t�F�C�X
//-----------------------------------------------------------------------------
__forceinline
SystemShader*	GmShader()
{
	return SystemShader::getInstance();
}


//=============================================================================
// END OF FILE
//=============================================================================