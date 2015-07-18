//-----------------------------------------------------------------------------
//!
//!	@file	SkyBox.cpp
//!	@brief	�X�J�C�{�b�N�X
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"


//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool SkyBox::init()
{
	_dim = 1.0f;
	//_dim = 512.0f;
	glGenBuffers(1, &_vbo_cube_vertices);
	glGenBuffers(1, &_ibo_cube_indices);
	return true;
}

//-----------------------------------------------------------------------------
//! �X�J�C�{�b�N�X�̍쐬
//! @param	[in]	directory �t�@�C���ւ̃f�B���N�g��
//! @param	[in]	front	  ���ʂ̉摜�t�@�C����
//! @param	[in]	back	  ��ʂ̉摜�t�@�C����
//! @param	[in]	left	  ���ʂ̉摜�t�@�C����
//! @param	[in]	right	  �E�ʂ̉摜�t�@�C����
//! @param	[in]	top		  ��ʂ̉摜�t�@�C����
//! @param	[in]	bottom	  ���ʂ̉摜�t�@�C����
//-----------------------------------------------------------------------------
void SkyBox::loadSkyBox(string directory,
						string front,
						string back,
						string left,
						string right,
						string top,
						string bottom)
{
	// �X�J�C�{�b�N�X�p�̃e�N�X�`���n���h���擾
	glGenTextures(1, &_skyBoxTextureHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _skyBoxTextureHandle);
	// �e�N�X�`���̓ǂݍ���
	texture[0].loadAndTexImage2D( (directory + front ).c_str(), false, false, GL_TEXTURE_CUBE_MAP_POSITIVE_Z );
	texture[1].loadAndTexImage2D( (directory + back	 ).c_str(), false, false, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z );
	texture[2].loadAndTexImage2D( (directory + left	 ).c_str(), false, false, GL_TEXTURE_CUBE_MAP_NEGATIVE_X );
	texture[3].loadAndTexImage2D( (directory + right ).c_str(), false, false, GL_TEXTURE_CUBE_MAP_POSITIVE_X );
	texture[4].loadAndTexImage2D( (directory + top	 ).c_str(), false, false, GL_TEXTURE_CUBE_MAP_POSITIVE_Y );
	texture[5].loadAndTexImage2D( (directory + bottom).c_str(), false, false, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y );

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void SkyBox::render()
{

	glDepthMask(GL_FALSE);

	CameraBase* cam   = GmCameraMan()->getCurrentCamera();
	Matrix matWorld   = Matrix::translate(cam->getPosition());
	Matrix matView	  = cam->getViewMatrix();
	Matrix matProj	  = cam->getProjMatrix();

	Matrix sendMatrix[] =
	{
		matWorld,
		matView,
		matProj,
	};

	// FBO�p�̒��_�f�[�^�z��
	GLfloat cube_vertices[] = {
		-_dim,  _dim,  _dim,
		-_dim, -_dim,  _dim,
		 _dim, -_dim,  _dim,
		 _dim,  _dim,  _dim,
		-_dim,  _dim, -_dim,
		-_dim, -_dim, -_dim,
		 _dim, -_dim, -_dim,
		 _dim,  _dim, -_dim,
	};
	
	glBindBuffer(GL_ARRAY_BUFFER, _vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	// FBO�p�̃C���f�b�N�X�f�[�^�z��
	GLushort cube_indices[] = {
		  0, 1, 2, 3,
		  3, 2, 6, 7,
		  7, 6, 5, 4,
		  4, 5, 1, 0,
		  0, 3, 7, 4,
		  6, 2, 1, 5,
	};
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo_cube_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	static float intensity = 2.5f;

	GmShader()->setUniform1fv("intensity", (GLfloat*)&intensity);
	GmShader()->setUniformMatrixBuffer(sendMatrix, sizeof(sendMatrix));
	GmShader()->setAndEnableVertexAttribute("position", 3, GL_FLOAT, 0,0);
	GmShader()->bindTexture("cubeMap", _skyBoxTextureHandle, 0, GL_TEXTURE_CUBE_MAP);

	// �`��
	glDrawElements(GL_QUADS, sizeof(cube_indices)/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	GmShader()->DisableVertexAttribute("position");
	GmShader()->unbindTexture(GL_TEXTURE_CUBE_MAP);

	// �R�t������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDepthMask(GL_TRUE);
	
}

void SkyBox::debugRender()
{
	glDepthMask(GL_FALSE);

	GmShader()->endShader();
	glPushMatrix();
	Vector3 camPos = GmCameraMan()->getCurrentCamera()->getPosition();
	glTranslatef(camPos._x, camPos._y, camPos._z);

	// �E
	Texture::set(&texture[3]);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-_dim, +_dim, -_dim);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(+_dim, +_dim, -_dim);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-_dim, -_dim, -_dim);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(+_dim, -_dim, -_dim);
	glEnd();
	Texture::set(NULL);

	// ��
	Texture::set(&texture[1]);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(+_dim, +_dim, -_dim);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(+_dim, +_dim, +_dim);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(+_dim, -_dim, -_dim);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(+_dim, -_dim, +_dim);
	glEnd();
	Texture::set(NULL);

	// ��
	Texture::set(&texture[2]);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(+_dim, +_dim, +_dim);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-_dim, +_dim, +_dim);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(+_dim, -_dim, +_dim);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-_dim, -_dim, +_dim);
	glEnd();
	Texture::set(NULL);

	// �O
	Texture::set(&texture[0]);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-_dim, +_dim, +_dim);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-_dim, +_dim, -_dim);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-_dim, -_dim, +_dim);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-_dim, -_dim, -_dim);
	glEnd();
	Texture::set(NULL);

	// ��
	Texture::set(&texture[4]);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(+_dim, +_dim, +_dim);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(+_dim, +_dim, -_dim);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-_dim, +_dim, +_dim);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-_dim, +_dim, -_dim);
	glEnd();
	Texture::set(NULL);

	// ��
	Texture::set(&texture[5]);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(+_dim, -_dim, +_dim);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-_dim, -_dim, +_dim);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(+_dim, -_dim, -_dim);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-_dim, -_dim, -_dim);
		
	glEnd();
	Texture::set(NULL);

	GmShader()->beginPrevShader();

	glPopMatrix();
	glDepthMask(GL_TRUE);
	GmShader()->beginPrevShader();
}
//=============================================================================
//	END OF FILE
//=============================================================================