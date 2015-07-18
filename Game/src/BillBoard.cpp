//-----------------------------------------------------------------------------
//!	
//!	@file	BillBoard.cpp
//!	@brief	�r���{�[�h
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"



//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool BillBoard::init()
{

	//GLsizei vertexBufferSize = ( sizeof( Vector3 ) * 4 );
	GLsizei vertexBufferSize = ( sizeof( FilterVertex ) * 4 );
	
	glGenBuffers(1, &_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, _vertex);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

//-----------------------------------------------------------------------------
//! �r���{�[�h�̂��߂Ɉʒu�v�Z
//-----------------------------------------------------------------------------
void BillBoard::calcBillBoardPosition(Vector3* billBoardPos)
{
	// �r���{�[�h�̃T�C�Y�ݒ�
	f32 w = _size._x;
	f32 h = _size._y;
	Vector3 position = _position;

	//-------------------------------------------------------------
	// �J�������猩���^��Ɛ^���������擾
	//-------------------------------------------------------------
	Matrix worldMatrix = GmCameraMan()->getCurrentCamera()->getMatrix();

	Vector3 axisX = worldMatrix._m[0];	// �^������
	Vector3 axisY = worldMatrix._m[1];	// �^�����

//	glDepthMask(GL_FALSE);		// Z�X�V������

	GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};
	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
	// �J���[
	//glColor4ubv((GLubyte*)&color);


	// 4�_�̍��W�v�Z
	f32	halfW = w * 0.5f;
	f32	halfH = h * 0.5f;

	/*if( halfW != 5.0f )
	{
		s32 a = 0;
	}*/

	//---- XY�����̈ړ���
	Vector3 offset[] = {
		Vector3(-halfW, +halfH, 0.0f),
		Vector3(+halfW, +halfH, 0.0f),
		Vector3(-halfW, -halfH, 0.0f),
		Vector3(+halfW, -halfH, 0.0f)
	};

	//---- Z�����S��]
	for( s32 i=0; i<4; i++ ) {
		f32 x = (offset[i]._x *  cosf(_rotation)) + (offset[i]._y * sinf(_rotation));
		f32 y = (offset[i]._x * -sinf(_rotation)) + (offset[i]._y * cosf(_rotation));

		offset[i]._x = x;
		offset[i]._y = y;

		//billBoardPos[i] = 
	}
}

//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void BillBoard::draw(const Color&	color, const s32& aniCount)
{

	// �r���{�[�h�̃T�C�Y�ݒ�
	f32	rateX; 
	f32 rateY; 
	s32	count; 
	if( _texture != nullptr ) {
		rateX = _size._x / _texture->getWidth();
		rateY = _size._y / _texture->getHeight();
		count = (s32)(_texture->getWidth() / _size._x);
	}else{
		rateX = 1.0f;
		rateY = 1.0f;
		count = 1;
	}
	
	f32 dispX = _size._x * _scale._x;
	f32 dispY = _size._y * _scale._y;
	
	Vector3 position = _position;

	//-------------------------------------------------------------
	// �J�������猩���^��Ɛ^���������擾
	//-------------------------------------------------------------
	Matrix worldMatrix = GmCameraMan()->getCurrentCamera()->getMatrix();

	Vector3 axisX = worldMatrix._m[0];	// �^������
	Vector3 axisY = worldMatrix._m[1];	// �^�����


	glDepthMask(GL_FALSE);		// Z�X�V������

	GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};
	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
	// �J���[
	//glColor4ubv((GLubyte*)&color);


	// 4�_�̍��W�v�Z
	/*f32	halfW = w * 0.5f;
	f32	halfH = h * 0.5f;*/
	f32	halfW = dispX * 0.5f;
	f32	halfH = dispY * 0.5f;

	/*if( halfW != 5.0f )
	{
		s32 a = 0;
	}*/

	//---- XY�����̈ړ���
	Vector3 offset[] = {
		Vector3(-halfW, +halfH, 0.0f),
		Vector3(+halfW, +halfH, 0.0f),
		Vector3(-halfW, -halfH, 0.0f),
		Vector3(+halfW, -halfH, 0.0f)
	};

	//---- Z�����S��]
	for( s32 i=0; i<4; i++ ) {
		f32 x = (offset[i]._x *  cosf(_rotation)) + (offset[i]._y * sinf(_rotation));
		f32 y = (offset[i]._x * -sinf(_rotation)) + (offset[i]._y * cosf(_rotation));

		offset[i]._x = x;
		offset[i]._y = y;
	}

	//Float2 uv[] = {
	//	Float2(0.0f /* + uvOffset._x */, 0.0f /* + uvOffset._y*/ ),
	//	Float2(1.0f /* + uvOffset._x */, 0.0f /* + uvOffset._y*/ ),
	//	Float2(0.0f /* + uvOffset._x */, 1.0f /* + uvOffset._y*/ ),
	//	Float2(1.0f /* + uvOffset._x */, 1.0f /* + uvOffset._y*/ ),
	//};

	f32 left	= rateX * (aniCount % count);
	f32 top		= rateY * (aniCount / count);
	f32 right   = left	+ rateX;
	f32 bottom  = top	+ rateY;

	Float2 uv[] = {
		Float2(left  , top),
		Float2(right , top),
		Float2(left  , bottom),
		Float2(right , bottom),
		
	};

	/*f32 r = LinearInterpolation(0.0f, 1.0f, color._r, 255);
	f32 g = LinearInterpolation(0.0f, 1.0f, color._g, 255);
	f32 b = LinearInterpolation(0.0f, 1.0f, color._b, 255);
	f32 a = LinearInterpolation(0.0f, 1.0f, color._a, 255);*/

	//---- ���S���W�����Z
	FilterVertex p[4];
	for( s32 i=0; i<4; i++ ){
		p[i]._position		= position + (axisX * offset[i]._x) + (axisY * offset[i]._y);
		p[i]._uv			= uv[i];
		p[i]._diffuseColor	= Vector4(color);
		p[i]._emissiveColor = Vector4(color);
	}
		

	////---- �}�g���b�N�X�]��
	//CameraBase* cam   = GmCameraMan()->getCurrentCamera();
	////Matrix matWorld   = Matrix::translate(position);
	//Matrix matWorld   = Matrix::IDENTITY;
	//Matrix matView	  = cam->getViewMatrix();
	//Matrix matProj	  = cam->getProjMatrix();

	//Matrix sendMatrix[] =
	//{
	//	matWorld,
	//	matView,
	//	matProj,
	//};

	//GmShader()->setUniformMatrixBuffer(sendMatrix, sizeof(sendMatrix));

	glBindBuffer(GL_ARRAY_BUFFER, _vertex);
	
	s32 size = sizeof(FilterVertex) * 4;

	glBufferSubData(GL_ARRAY_BUFFER, 0, size, p);

	GLint handle;
	if( _texture ) {
		handle = _texture->getTextureHandle();
	}else{
		handle = GmShader()->getNullNormHandle();
	}

	GmShader()->bindTexture("BaseTexture", handle, 0);
	// �e�N�X�`���̕i���ݒ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	

	//GmShader()->setAndEnableVertexAttribute("position", 3, GL_FLOAT, sizeof(Vector3), (GLvoid*)(&p[i]));
	GmShader()->setAndEnableVertexAttribute("position"		, 3, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _position));
	GmShader()->setAndEnableVertexAttribute("uv"			, 2, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _uv));
	GmShader()->setAndEnableVertexAttribute("diffuseColor"	, 4, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _diffuseColor));
	GmShader()->setAndEnableVertexAttribute("emissiveColor"	, 4, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _emissiveColor));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	GmShader()->DisableVertexAttribute("position");
	GmShader()->DisableVertexAttribute("uv");
	GmShader()->DisableVertexAttribute("diffuseColor");
	GmShader()->DisableVertexAttribute("emissiveColor");

	GmShader()->unbindTexture();
	

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	glDepthMask(GL_TRUE);				// �u�y�X�V�L���v�ɖ߂�

}
//=============================================================================
// END OF FILE
//=============================================================================