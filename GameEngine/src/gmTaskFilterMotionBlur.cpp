//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilterMotionBlur.cpp
//!	@brief	���[�V�����u���[
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
TaskFilterMotionBlur::TaskFilterMotionBlur()
{
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
TaskFilterMotionBlur::~TaskFilterMotionBlur()
{
}

Texture debugTexture;

//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool TaskFilterMotionBlur::setup()
{
	// �e�N���X�̏��������Ă�
	TaskFilter::setup();

debugTexture.loadAndTexImage2D("../Image/GameClear.png");
	return true;
}

//-----------------------------------------------------------------------------
//! ���
//-----------------------------------------------------------------------------
void TaskFilterMotionBlur::cleanup()
{
	TaskFilter::cleanup();
}

//-----------------------------------------------------------------------------
//! �J��������X�N���[���܂ł̋��������߂�
//!	@param	[in]	screenW		��ʂ̉𑜓x(��)
//!	@param	[in]	screenH		��ʂ̉𑜓x(����)
//!	@param	[in]	acpect		�A�X�y�N�g��
//!	@param	[in]	fov			��p
//!	@return	�J��������X�N���[���܂ł̋���
//-----------------------------------------------------------------------------
f32 getScreenDistance(f32 screenW, f32 screenH, f32 aspect, Radian fov)
{
	f32 distance = 0.0f;

	//					  �@�@    +|
	//					  �@�@  �^||
	//					  �@�@�^  ||
	//					  �@�^	  ||
	//					  �^	  ||
	//					�^		  ||
	//				  �^		  || �X�N���[���T�C�Y �� 2
	//				�^))fov		�s||
	// �J�����ʒu �� -------------||
	//			   |�� distance ��|
	//
	//  screenW / 2
	// ------------- = tan(fov)
	//   distance
	//			   screenW / 2
	// distance = --------------
	//				tan(fov)

	distance = (screenW / 2) / tanf(fov);

	return distance;
}


//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void TaskFilterMotionBlur::render()
{

	static const s32	SHADER_STANDARD			= SystemShader::SHADER_FILTER;

	//-------------------------------------------------------------
	// (1) �o�b�N�o�b�t�@���烏�[�N�o�b�t�@�ɃR�s�[
	//-------------------------------------------------------------
	copyToWorkTexture(SHADER_STANDARD, 0);

#if 1

	// ������������
	setBlendMode(BM_NONE);
	//-------------------------------------------------------------
	// (2) �������w��
	//-------------------------------------------------------------
	Size<s32>	windowSize	= GmWindow()->getResolution();
	f32 screenDistance		= getScreenDistance(windowSize._w, windowSize._h, 16.0f / 9.0f, Degree(45.0f));

	Matrix viewMatrix		= GmScene()->getViewMatrix();
	Matrix cameraMatrix		= viewMatrix.inverseFast();
	Matrix cameraMatrixLast = _viewMatrixLast.inverseFast();

	// �J�����̃X�N���[����̒��S���W�̈ړ��ʂ��v�Z
	Vector3 screenPosition	   = Vector3(cameraMatrix._m[3])	 - Vector3(cameraMatrix._m[2])	   * screenDistance;
	Vector3 screenPositionLast = Vector3(cameraMatrixLast._m[3]) - Vector3(cameraMatrixLast._m[2]) * screenDistance;

	// ���[���h���W�̓_���X�N���[����łǂꂭ�炢�����������v�Z
	Vector3 screenUpDir		= cameraMatrix._m[1];		// �J�����̏�����̃x�N�g��
	Vector3 screenRightDir	= cameraMatrix._m[0];		// �J�����̉E�����̃x�N�g��

	Vector3	move	= screenPosition - screenPositionLast;	// ���[���h�ł̈ړ���

	Vector3	blurDir;
	blurDir._x = Vector3::dot(move, screenRightDir);
	blurDir._y = Vector3::dot(move, screenUpDir) * -1.0f;
	blurDir._z = 0.0f;


	// ���̃t���[���̂��߂ɕۑ�
	_viewMatrixLast = viewMatrix;

	f32 scale = 0.005f;
//	f32 scale = 0.01f;

	blurDir *= scale;


	GmShader()->changeShader(SystemShader::SHADER_MOTION_BLUR);

	// �u���[�����ݒ�
	GmShader()->setUniform2fv("gDir2D", (GLfloat*)& blurDir);
	//-------------------------------------------------------------
	// (3) �u���[��݌v���Ă��� (4pixel�P��)
	//-------------------------------------------------------------
	s32		index = 0;
	s32		mip   = 0;


	for( s32 i=0; i<3; i++ ) {
		
		GmShader()->setUniform1f("gStride", (GLfloat)(pow(4, (f32)i)) );

		//---- �u���[
		setRenderTarget(index ^ 1, mip);
		put(index, mip);

		index ^= 1;
	}

	GmShader()->beginPrevShader();

	setBackBuffer();

	//-------------------------------------------------------------
	// (4) �o�b�N�o�b�t�@�ɏ����߂�
	//-------------------------------------------------------------
	s32	workTextureIndex = 0;
	s32 mipLevel		 = mip;
	s32	shaderPass		 = SHADER_STANDARD;

	put(workTextureIndex, mipLevel, shaderPass);
#endif 
}


//=============================================================================
//	END OF FILE
//=============================================================================