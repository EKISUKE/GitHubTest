//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilterLensFlare.cpp
//!	@brief	�����Y�t���A
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
TaskFilterLensFlare::TaskFilterLensFlare()
: _lensColorTex		(nullptr)
, _lensDirtTex		(nullptr)
, _lensStarBurstTex	(nullptr)
{
	SAFE_NEW(_lensColorTex);
	SAFE_NEW(_lensDirtTex);
	SAFE_NEW(_lensStarBurstTex);

	_lensColorTex->loadAndTexImage2D("../Image/LensFlare/lenscolor.png",	false, false, GL_TEXTURE_1D);
	_lensDirtTex->loadAndTexImage2D("../Image/LensFlare/lensdirt.png",		false);
	//_lensDirtTex->loadAndTexImage2D("../Image/LensFlare/dust2.jpg",			false);
	_lensStarBurstTex->loadAndTexImage2D("../Image/LensFlare/lensstar.png", false, true);

}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
TaskFilterLensFlare::~TaskFilterLensFlare()
{
	SAFE_DELETE(_lensColorTex);
	SAFE_DELETE(_lensDirtTex);
	SAFE_DELETE(_lensStarBurstTex);
}

//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool TaskFilterLensFlare::setup()
{
	Size<s32>	windowSize = GmWindow()->getResolution();
	Size<s32>	workTexResolution;
	workTexResolution._w = windowSize._w * (1.0f/8.0f);
	workTexResolution._h = windowSize._h;
	// �e�N���X�̏��������Ă�
	TaskFilter::setup(workTexResolution);

	//---- �e�N�X�`���̃������m��
	SAFE_NEW(_accumTexture);
	if(_accumTexture == nullptr) return false;
	SAFE_NEW(_stockTexture);
	if(_stockTexture == nullptr) return false;


	//---- ��e�N�X�`���̍쐬
	if (!_accumTexture->createRenderTarget(windowSize._w, windowSize._h, GL_RGBA16F_ARB, GL_RGBA, GL_HALF_FLOAT_ARB, true)) {
		MessageBox(NULL, "RenderTarget�̍쐬�Ɏ��s���܂����B"__FILE__, "ERROR", MB_OK);
		return false;
	}

	if (!_stockTexture->createRenderTarget(windowSize._w, windowSize._h, GL_RGBA16F_ARB, GL_RGBA, GL_HALF_FLOAT_ARB, true)) {
		MessageBox(NULL, "RenderTarget�̍쐬�Ɏ��s���܂����B"__FILE__, "ERROR", MB_OK);
		return false;
	}

	for(u32 i=0; i<2; ++i){
		SAFE_NEW(_bilenarExpandTex[i]);
		if (_bilenarExpandTex[i] == nullptr) return false;
		// ���[�N�e�N�X�`����2�{�̕��̃e�N�X�`��
		if (!_bilenarExpandTex[i]->createRenderTarget(workTexResolution._w * 2.0f, workTexResolution._h, GL_RGBA16F_ARB, GL_RGBA, GL_HALF_FLOAT_ARB, true)) {
			MessageBox(NULL, "RenderTarget�̍쐬�Ɏ��s���܂����B"__FILE__, "ERROR", MB_OK);
			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
//! ���
//-----------------------------------------------------------------------------
void TaskFilterLensFlare::cleanup()
{
	TaskFilter::cleanup();
	for (u32 i = 0; i<2; ++i){
		SAFE_DELETE(_bilenarExpandTex[i]);
	}

}

//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void TaskFilterLensFlare::render()
{

	static const s32	SHADER_STANDARD	   = SystemShader::SHADER_FILTER;
	static const s32	SHADER_SUBTRACT	   = SystemShader::SHADER_SUBTRACT;
	static const s32	SHADER_EXPAND_BLUR = SystemShader::SHADER_EXPAND_BLUR;

	//-------------------------------------------------------------
	// (1) �o�b�N�o�b�t�@���烏�[�N�o�b�t�@�ɃR�s�[
	// �O���A�̏ꍇ�͂ڂ����̂�1/2�o�b�t�@�̏k�����Ă������Ƃ�
	// ��i�p�X���y�ʉ������(�o�C���j�A��4�s�N�Z������)
	//-------------------------------------------------------------
	copyToWorkTexture(SHADER_STANDARD, 0);

	// ������������
	setBlendMode(BM_NONE);
	//-------------------------------------------------------------
	// (2)���P�x�̐����𒊏o
	// �摜����臒l(Threshold)�̕������Z
	//-------------------------------------------------------------
	s32 index = 1;
	s32 mip	  = 0;
	

	//static float threshold = 0.9f;	// �^�C�g���Ŋm�F���鎞�p
	static float threshold = 2.5f;
	//static float threshold = 9.1f;
	static float scale = 1000.0f;

	Controller* control = GmControlMan()->getController(1);

	if(control->GetPadState(Controller::PAD_RB)){

		threshold += 0.01f;

	}else if (control->GetPadState(Controller::PAD_LB)){
		threshold -= 0.01f;
	}

	threshold = max(0.0f, threshold);

	GmShader()->changeShader(SystemShader::SHADER_SUBTRACT);

	GmShader()->setUniform1fv("gThreshold", (GLfloat*)&threshold);
	GmShader()->setUniform1fv("scale", (GLfloat*)&scale);

	setRenderTarget(index, mip);
	//setBackBuffer();
	put(0, 0);	// texuter[0] mip=0 : SUBTRACT


	GmShader()->beginPrevShader();

	GmRender()->saveBackBuffer("�����Y�t���A����");


	//-------------------------------------------------------------
	// (3) �A�i�����t�B�b�N�����Y�̃S�[�X�g�쐬(���Ώ�)
	//-------------------------------------------------------------
	/*GmShader()->changeShader(SystemShader::SHADER_LENS_FLARE);

	GmShader()->bindTexture("lensColorTex", _lensColorTex->getTextureHandle(), 1, GL_TEXTURE_1D);

	GmShader()->setUniform1i("isAnamolphic", (GLint)true);

	setRenderTarget(index ^ 1, mip);
	put(index, mip);
	index ^= 1;

	GmShader()->unbindTexture(GL_TEXTURE_1D);

	GmRender()->saveBackBuffer("�����Y�t���A�쐬��");
*/

	
	

	//-------------------------------------------------------------
	// (3) �������u���[&���F
	//-------------------------------------------------------------
	//// ���E�Ƀu���[���|���邽�ߓ�񂵂Ă���
	//for (s32 d = 0; d<2; ++d){

	//	// ���̕������w��
	//	f32 x = 1;
	//	f32 y = 0.0f;

	//	if(d == 1) {
	//		x = -1;
	//		//---- �ꎞ�ۑ����Ă������e�N�X�`������R�s�[
	//		setRenderTarget(index, mip);
	//		put(_stockTexture, mip, SHADER_STANDARD);
	//	}

	//	GmShader()->changeShader(SystemShader::SHADER_GLARE);

	//	// �u���[�����ݒ�
	//	GmShader()->setUniform1f("gAttenuation", 0.99f);

	//	// �u���[�����ݒ�
	//	GmShader()->setUniform2fv("gDir2D", (GLfloat*)&Vector3(x, y, 0.0f));


	//	for (s32 i = 0; i<3; i++)
	//	{

	//		// �s�N�Z���̊Ԋu(1, 8, 64)
	//		GmShader()->setUniform1f("gStride", (GLfloat)(pow(8, (f32)i)));

	//		//---- �����u���[
	//		setRenderTarget(index ^ 1, mip);
	//		put(index, mip);
	//		index ^= 1;
	//	}

	//	// ���ɖ߂�
	//	GmShader()->beginPrevShader();

	//	// �~�σe�N�X�`���ɕۑ�
	//	GmRender()->setRenderTarget(_accumTexture, Float2(0, 0), 0);

	//	if(d==0){
	//		glClear(GL_COLOR_BUFFER_BIT);
	//	}

	//	setBlendMode(BM_ADD_NOALPHA);
	//	put(index, mip, SHADER_STANDARD);
	//}


	// ���̕������w��
	f32 x = 1;
	f32 y = 0.0f;

	GmShader()->changeShader(SystemShader::SHADER_ANAMOLPHIC_FLARE);

	// �u���[�����ݒ�
	GmShader()->setUniform1f("gAttenuation", 0.98f);

	// �u���[�����ݒ�
	GmShader()->setUniform2fv("gDir2D", (GLfloat*)&Vector3(x, y, 0.0f));


	for (s32 i = 0; i<3; i++)
	{

		// �s�N�Z���̊Ԋu(1, 8, 64)
		GmShader()->setUniform1f("gStride", (GLfloat)(pow(8, (f32)i)));

		//---- �����u���[
		setRenderTarget(index ^ 1, mip);
		put(index, mip);
		index ^= 1;
	}

	// ���ɖ߂�
	GmShader()->beginPrevShader();

	GmRender()->saveBackBuffer("�����Y�t���A�u���[�� 1/4�T�C�Y");

	//---- 2�{�Ƀo�C���j�A�Ŋg��
	{
		GmRender()->setRenderTarget(_bilenarExpandTex[0], Float2(0, 0), 0);
		put(index, mip, SHADER_EXPAND_BLUR);
		GmRender()->saveBackBuffer("�o�C���j�A��2�{�Ɋg��");
		//---- ���s�m�F�̂��ߕۑ�
		GmRender()->setRenderTarget(_stockTexture, Float2(0, 0), 0);
		put(_bilenarExpandTex[0], mip, SHADER_STANDARD);
		GmRender()->saveBackBuffer("�o�C���j�A��4�{�Ɋg��");
	}

	//---- 4�_0.25�t�F�b�`�ł킸���ɂڂ����ăo�C���j�A�Ŋg��
	{
		static f32 offset = 0.5f;

		if (GetKeyState(VK_OEM_PLUS) & 0x80){
			offset += 0.01f;
		}
		else if (GetKeyState(VK_OEM_MINUS) & 0x80){
			offset -= 0.01f;
		}

		GmShader()->changeShader(SystemShader::SHADER_EXPAND_BLUR);
		
		GmShader()->setUniform1f("gUVOffset", offset);
		GmRender()->setRenderTarget(_stockTexture, Float2(0, 0), 0);
		put(_bilenarExpandTex[0], mip);
		GmRender()->saveBackBuffer("�X�g�b�N�e�N�X�`��");

		GmShader()->beginPrevShader();
	}

	//-------------------------------------------------------------
	// (3) �ʏ탌���Y�ł̃t���A&HALO�쐬
	//-------------------------------------------------------------


	
#if 0 
	GmShader()->changeShader(SystemShader::SHADER_LENS_FLARE);
	
	GmShader()->bindTexture("lensColorTex", _lensColorTex->getTextureHandle(), 1, GL_TEXTURE_1D);

	GmShader()->setUniform1i("isAnamolphic", (GLint)true);

	setRenderTarget(index^1, mip);
	put(index, mip);
	index ^= 1;

	GmShader()->unbindTexture(GL_TEXTURE_1D);

	GmRender()->saveBackBuffer("�����Y�t���A�쐬��");

#endif
	
	//-------------------------------------------------------------
	// (4) �K�E�V�A���u���[���k�����J��Ԃ��A���i�o�b�t�@����
	//-------------------------------------------------------------
# if 0
	//---- �����u���[
	setRenderTarget(index ^ 1, mip);
	put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_H);
	index ^= 1;

	//---- �����u���[
	setRenderTarget(index ^ 1, mip);
	put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_V);
	index ^= 1;

	while (mip<4) {

		//---- �~�b�v�i������i���ɃR�s�[
		setRenderTarget(index, mip + 1);
		put(index, mip, SHADER_STANDARD);
		mip++;

		//---- �����u���[
		setRenderTarget(index ^ 1, mip);
		put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_H);
		index ^= 1;

		////---- �����u���[
		//setRenderTarget(index ^ 1, mip);
		//put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_V);
		//index ^= 1;

	}
#endif

#define IS_DIRT_AND_BURST 0
	//-------------------------------------------------------------
	// (5) �����Y����A�X�^�[�o�[�X�g������
	//-------------------------------------------------------------
#if IS_DIRT_AND_BURST
	GmShader()->changeShader(SystemShader::SHADER_LENS_DIRT_STAR);

	Matrix	viewMatrix		= GmScene()->getViewMatrix();
	Vector3 right			=  Vector3(viewMatrix._m[0]);
	Vector3 viewDir			= -Vector3(viewMatrix._m[2]);

	float	camRot			= Vector3::dot(right, Vector3(0,0,1)) + Vector3::dot(viewDir, Vector3(0,1,0));

	Matrix	cameraRotZMat	= Matrix::rotateZ(Radian(camRot));

	static Matrix scaleBias1 = Matrix(
		2.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 2.0f, -1.0f, 0.0f,
		0.0f, 0.0f,  1.0f, 0.0f,
		0.0f, 0.0f,  0.0f, 1.0f
	);
	static Matrix scaleBias2 = Matrix(
		0.5f, 0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	Matrix lensStarMatrix = scaleBias2 * cameraRotZMat * scaleBias1;
	//Matrix lensStarMatrix = cameraRotZMat;

	GmShader()->setMatrix4x4("lensStarMatrix", 1, GL_TRUE, (GLfloat*)&lensStarMatrix);
	
	
	GmShader()->bindTexture("lensStarTex", _lensStarBurstTex->getTextureHandle(), 3);
	GmShader()->bindTexture("lensDirtTex", _lensDirtTex->getTextureHandle(), 2);
	

	// ����e�N�X�`����mip 0 �ł��Ȃ��Ƃڂ₯�Ă��܂����� mip 0�ɑ΂��čs���Ă���
	setRenderTarget(index^1, 0);
	put(index, mip);
	index ^= 1;

	GmShader()->unbindTexture();
	GmShader()->beginPrevShader();


#endif

	//-------------------------------------------------------------
	// (6) ���Z�����őS�Ă��o�b�N�o�b�t�@�ɍ�������
	//-------------------------------------------------------------
	setBackBuffer();

	s32	workTextureIndex = index;
	s32 mipLevel = mip;

	// ����t�B���^��ʂ�ꍇ��mipLevel 0�ł���Ă��邽��mipLevel��ύX
	if (IS_DIRT_AND_BURST == 1) {
		mipLevel		 = 0;
	}

	static bool isResult = false;

	// BACK�{�^���Ń����Y�t���A�̂ݕ`�悩���Z��������؂�ւ�
	if (control->GetPushState(Controller::PAD_BACK)){
		isResult ^= true;
	}

	if (isResult){
		setBlendMode(BM_NONE);
		//put(workTextureIndex, mipLevel, SHADER_STANDARD);
		put(_stockTexture, mipLevel, SHADER_STANDARD);
	}else{
		setBlendMode(BM_ADD_NOALPHA);
		put(_stockTexture, mipLevel, SHADER_STANDARD);
		setBlendMode(BM_NONE);
	}
	

}


//=============================================================================
//	END OF FILE
//=============================================================================