//-----------------------------------------------------------------------------
//!
//!	@file	CascadedShadow.cpp
//!	@brief	�J�X�P�[�h�V���h�E
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"



//=============================================================================
// �J�X�P�[�h�V���h�E�̎���
//=============================================================================

//---- �ÓI�ϐ��̏�����
const Matrix CascadedShadow::_biasMatrix = Matrix(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
);

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
CascadedShadow::CascadedShadow()
: _currentShadowNum	(0)
, _split_weight		(0.75f)
, _modelMatrix		(Matrix::IDENTITY)
, _depthTexture		(nullptr)
{
	for( u32 i=0; i<6; ++i ) {
		_MinMax[i] = 0.0f;
	}

	// �V���h�E�}�b�v�������ƃv���W�F�N�V�����s�񏉊���
	for( u32 i=0; i<SPLIT_COUNT; ++i ) {
		_pShadow[i] = nullptr;
		// �V���h�E�}�b�v������
		SAFE_NEW(_pShadow[i]);
		// �����䏉����
		/*f[i]._fov   = TO_RADIAN(120.0f);
		f[i]._ratio = (f32)WIDTH/(f32)HEIGHT;*/
		// �v���W�F�N�V�����s�񏉊���
		_depthProj[i]  = Matrix::IDENTITY;
	}

	// �e�N�X�`���̉����͕����������
	_depthTexSize = Size<s32>(DEPTH_SIZE * SPLIT_COUNT, DEPTH_SIZE);

	// �V���h�E�e�N�X�`�����쐬
	if( !createShadowTexture() ) {
		MessageBoxA(NULL, "�V���h�E�e�N�X�`���쐬�Ɏ��s���܂���", "�G���[", MB_OK);
	}

	// ���������V�F�[�_�ɓn���Ă���
	GmShader()->changeShader(SystemShader::SHADER_TEXTURE);
	GmShader()->setUniform1f("gSplitCount", (GLfloat)SPLIT_COUNT);
	GmShader()->beginPrevShader();

	TaskModelX::setBiasMatrix(_biasMatrix);

	_isSetRender = false;
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
CascadedShadow::~CascadedShadow()
{
	for( u32 i=0; i<SPLIT_COUNT; ++i ) {
		_pShadow[i] = nullptr;
		// �V���h�E�}�b�v������
		SAFE_NEW(_pShadow[i]);
	}
}

//-----------------------------------------------------------------------------
//! ���ˉe�s��̏�����
//-----------------------------------------------------------------------------
void CascadedShadow::initOrthoMatrix()
{
	CameraBase*	cam = GmCameraMan()->getCurrentCamera();
	Vector3		camPos = cam->getPosition();
	Matrix		camMat = cam->getMatrix();

	// ���������̍X�V
	updateSplitDist(0.01f, 3000.0f);

	for (s32 i = 0; i<SPLIT_COUNT; ++i)
	{
		// �������8�_���X�V
		updateFrustumPoints(f[i], camPos, camMat);

		// �������������߂�
		Vector3 viewDir = -Vector3(camMat._m[2]).normalize();

		// ���S�܂ł̋��������߂�
		f32 centerOfClip = f[i]._fard - f[i]._neard / 2.0f;
		_centerLength[i]  = centerOfClip + f[i]._neard;
		
		// ���������𒆓_�܂ł̋����ɂ��A�J�������W�ɑ���
		Vector3 center = (viewDir * _centerLength[i]) + camPos;



		// �߃N���b�v�ʂ��牓�N���b�v�ʂ܂ł̋��������߂�
		Vector3 p1 = f[i]._point[0];
		Vector3 p2 = f[i]._point[4];
		Vector3 p3 = f[i]._point[6];

		Sphere  clipShpere = calcCircumscribeCircle(p1,p2,p3);

		// ���a�����߂�(���a���܂��������Ȃ��͗l���i�K�͏����傫�߂ɂ���)
		f32	 rad = clipShpere.getRadius() * 1.2f;

		_depthProj[i] = Matrix::ortho(-rad, rad, -rad, rad, -rad, rad);
	}

	TaskModelX::setShadowProj(_depthProj);
}

//-----------------------------------------------------------------------------
//! �r���[�s����v�Z���č��
//! @param	[in]	lightDir	���C�g����
//! @param	[in]	viewDir		�r���[����
//! @param	[in]	camPos		�J�������W
//-----------------------------------------------------------------------------
void CascadedShadow::calcAndSetViewMatrix(Vector3& lightDir, Vector3& viewDir, Vector3& camPos)
{
	for(s32 i=0; i<SPLIT_COUNT; ++i) {
		// ���������𒆓_�܂ł̋����ɂ��A�J�������W�ɑ���
		Vector3 center = (viewDir * _centerLength[i]) + camPos;
		Vector3 lightLength = lightDir * Vector3(450.0f);
		_depthView[i] = Matrix::view(lightLength + center, center, Vector3(0, 1, 0));
	}
	// �^�X�N���f���Ƀr���[�s���n���Ă���
	TaskModelX::setShadowViewMatrix(_depthView);
}

//-----------------------------------------------------------------------------
//! �]���藝���v�Z����
//! ��ABC������Əꍇ
//! @param	[in]	a	��BC�̒���
//! @param	[in]	b	��CA�̒���
//! @param	[in]	c	��AB�̒���
//! @retval	��CAB�̒l
//-----------------------------------------------------------------------------
Radian calcLawOfCosines(f32& a, f32& b, f32& c)
{
	return Radian( acos( (b*b+c*c-a*a)/(2.0f*b*c) ) );
}


//-----------------------------------------------------------------------------
//! �N�������̌����ŊO�S�����߂�
//! @param	[in]	p1	���W1
//! @param	[in]	p2	���W2
//! @param	[in]	p3	���W3
//!	@retval	�O�S�̍��W
//-----------------------------------------------------------------------------
Vector3& calcCircleCenter(Vector3& p1, Vector3&p2, Vector3& p3)
{
	// �O�S��o�Ƃ���� o - p1 = r = o - p2
	// �O�S(x,y)�͈ȉ��̂悤�ɂȂ�
	// �N�������̌����̂��߂̏���
	f32 x1 = p1._x;
	f32 y1 = p1._y;
	f32 z1 = p1._z;

	f32 x2 = p2._x;
	f32 y2 = p2._y;
	f32 z2 = p2._z;

	f32 x3 = p3._x;
	f32 y3 = p3._y;
	f32 z3 = p3._z;

	f32 A1 = 2.0f * (x2 - x1);
	f32 B1 = 2.0f * (y2 - y1);
	f32 C1 = 2.0f * (z2 - z1);
	
	f32 a = (x2*x2 - x1*x1);
	f32 b = (y2*y2 - y1*y1);
	f32 c = (z2*z2 - z1*z1);

	f32 D1 = (x2*x2 - x1*x1) + (y2*y2 - y1*y1) + (z2*z2 - z1*z1);

	f32 A2 = 2.0f * (x3 - x2);
	f32 B2 = 2.0f * (y3 - y2);
	f32 C2 = 2.0f * (z3 - z2);
	f32 D2 = (x3*x3 - x2*x2) + (y3*y3 - y2*y2) + (z3*z3 - z3*z2);

	f32 A3 = 2.0f * (x1 - x3);
	f32 B3 = 2.0f * (y1 - y3);
	f32 C3 = 2.0f * (z1 - z3);
	f32 D3 = (x1*x1 - x3*x3) + (y1*y1 - y3*y3) + (z1*z1 - z3*z3);

	// ��(����)
	f32 Delta = (A1*B2*C3) + (A3*B1*C2) + (A2*B3*C1) - (A3*B2*C1) - (A2*B1*C3) - (A1*B3*C2);

	// �N�������̌������g��x,y,z�����߂�
	http://www.ee.fit.ac.jp/~kudou/1mathA/01/01-2.html
	f32 x	  = (D1*B2*C3) + (D3*B1*C2) + (D2*B3*C1) - (D3*B2*C1) - (D2*B1*C3) - (D1*B3*C2) / Delta;
	f32 y	  = (A1*D2*C3) + (A3*D1*C2) + (A2*D3*C1) - (A3*D2*C1) - (A2*D1*C3) - (A1*D3*C2) / Delta;
	f32 z	  = (A1*B2*D3) + (A3*B1*D2) + (A2*B3*D1) - (A3*B2*D1) - (A2*B1*D3) - (A1*B3*D2) / Delta;

	return Vector3(x,y,z);

}


//-----------------------------------------------------------------------------
//! �O�ډ~���v�Z����
//! @param	[in]	p1	���W1
//! @param	[in]	p2	���W2
//! @param	[in]	p3	���W3
//!	@retval	���\����
//-----------------------------------------------------------------------------
Sphere&	CascadedShadow::calcCircumscribeCircle(Vector3& p1, Vector3& p2, Vector3& p3)
{

	//---- �O�S�����߂�
	Vector3 center = calcCircleCenter(p1, p2, p3);

#if 1 
	//---- ���a�����߂�
	// 3�ӂ̒������v�Z
	Vector3 A = (p1 - p2);
	Vector3 B = (p2 - p3);
	Vector3 C = (p3 - p1);

	f32 a = A.length();
	f32 b = B.length();
	f32 c = C.length();

	// �]���藝�Ŋp�x�����߂�
	Radian radA = calcLawOfCosines(a, b, c);
	// �����藝�Ŕ��a�����߂�
	// 2R = a / sinA
	f32 R = (a / sinf(radA)) / 2.0f;
#else
	// ���߂��O�S���甼�a���v�Z����
	f32 R = (center - p1).length();
#endif
	
	// ���܂������̏���Ԃ�
	return Sphere(center, R);
}
//-----------------------------------------------------------------------------
//! �V���h�E�e�N�X�`�����쐬
//-----------------------------------------------------------------------------
bool CascadedShadow::createShadowTexture()
{
	SAFE_NEW(_depthTexture);

	// �`��^�[�Q�b�g���쐬(�����̃V���h�E�e�N�X�`�����ꖇ�ɂ���̂�WIDTH�̓f�v�X�̉����~������)
	if (!_depthTexture->createRenderTarget(DEPTH_SIZE * SPLIT_COUNT, DEPTH_SIZE, GL_RGB, GL_RGB, GL_FLOAT, false, true))
	{
		MessageBoxA(nullptr, "�V���h�E�e�N�X�`���쐬�Ɏ��s���܂���", "�G���[", MB_OK);
	}

	// �^�X�N���f���Ńf�v�X�e�N�X�`���ɏ������ނ̂ŏ���̂݃f�v�X�n���h�����Z�b�g����
	TaskModelX::setDepthTexHandle(_depthTexture->getDepthBuffer());

	//// �e�N�X�`���쐬
	//glGenTextures(1, &_depthTexture);
	//// �Ђ��t��
	//glBindTexture(GL_TEXTURE_2D, _depthTexture);
	////---- �]��
	//glTexImage2D(GL_TEXTURE_2D,
	//			 0,								// �~�b�v�}�b�v�i��
	//			 GL_DEPTH_COMPONENT,			// �R���|�[�l���g�� 3:RGB 4:RGBA
	//			 (GLsizei)_depthTexSize._w,		// ��
	//			 (GLsizei)_depthTexSize._h,		// ����
	//			 0,								// �{�[�_�[�J���[ 1:���� 0:�Ȃ�
	//			 GL_DEPTH_COMPONENT,			// [�]����] �R���|�[�l���g
	//			 GL_FLOAT,						// [�]����] ��1�v�f������̃T�C�Y
	//			 NULL);							// [�]����] �C���[�W�̃������̐擪
	//// �e�N�X�`���̕i���ݒ�
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	//glBindTexture(GL_TEXTURE_2D, 0);

	////---- �t���[���o�b�t�@���e�N�X�`���ƃf�v�X�o�b�t�@�ƍĕҐ�����
	//glGenFramebuffers(1, &_depthFrameBuffer);
	//// �t���[���o�b�t�@�����蓖��(�Ђ��t��)
	//glBindFramebuffer(GL_FRAMEBUFFER, _depthFrameBuffer);

	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);

	//// �f�v�X�e�N�X�`����FBO�ɐڑ�
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture, 0);


	//// �t���[���o�b�t�@���쐬�ł��Ă��Ȃ���Ύ��s
	//if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
	//return false;
	//
	//// �^�X�N���f���Ƀe�N�X�`���n���h����n���Ă���
	//TaskModelX::setDepthTexHandle(_depthTexture);
	//
	//// �o�b�t�@�o�C���h�J��
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

//-----------------------------------------------------------------------------
//! �������8�_�̍X�V
//-----------------------------------------------------------------------------
void CascadedShadow::updateFrustumPoints(Frustum& f, Vector3& center, Matrix& cameraMatrix)
{
	f.updateFrustumPoints(center, cameraMatrix);
}

//-----------------------------------------------------------------------------
//! ���������̍X�V
//-----------------------------------------------------------------------------
void CascadedShadow::updateSplitDist(f32 nd, f32 fd)
{
	f32	lambda  = _split_weight;
	f32	ratio   = fd/nd;
	f[0]._neard = nd;

	// �������������߃N���b�v�����A���N���b�v���������߂�
	for( s32 i=1; i<SPLIT_COUNT; i++ )
	{
		f32 si = i / (f32)SPLIT_COUNT;

		f[i  ]._neard = lambda*(nd*powf(ratio,si)) + (1-lambda)*(nd + (fd - nd)*si);
		f[i-1]._fard  = f[i]._neard * 1.005f;
	}
	// �Ō�͈�ԉ��������Ȃ̂ōŉ�����������
	f[SPLIT_COUNT-1]._fard = fd;
}


//-----------------------------------------------------------------------------
//! �N���b�v�s��̓K�p
//-----------------------------------------------------------------------------
f32 CascadedShadow::applyCropMatrix(Frustum &f, LightBase* pLight)
{
	f32 maxX = -1000.0f;
	f32 maxY = -1000.0f;
	f32	maxZ = -FLT_MAX;
	f32 minX =  1000.0f;
	f32 minY =  1000.0f;
	f32	minZ =  FLT_MAX;
	

	Vector4 transf;	// �ϊ��ꎞ�ۑ��p

	//-------------------------------------------------------------
	// �V���h�E�̍s��쐬
	//-------------------------------------------------------------

	// ���f���r���[�s��쐬
	Matrix shadowModelView = _modelMatrix * _depthView[0];

	
	/*transf = Vector4(f._point[0], 1.0f).transform( shadowModelView );
	minZ = transf._z;
	maxZ = transf._z;*/
	// �ő�, �ŏ���������̎c��7�_���狁�߂�
	for(s32 i=0; i<8; ++i) 
	{
		transf = Vector4(f._point[i], 1.0f).transform( shadowModelView );
		maxZ = max(maxZ, transf._z);
		minZ = min(minZ, transf._z);

		/*maxX = max(maxX, transf._x);
		minX = min(minX, transf._x);
		maxY = max(maxY, transf._y);
		minY = min(minY, transf._y);*/
	}
	
	//-------------------------------------------------------------
	// Z�̍ő�ŏ����Z�o
	//-------------------------------------------------------------
	// �}�b�v�̍ő�ŏ��Ɣ�ׂ�
	// �}�b�v�̎l��
	//	�� x
	//z��
	//	0 +------+ 1 
	//	  |		 |
	//	  |		 |
	//	2 *------+ 3
	static const Vector3 objectsPos[4] = {
		Vector3(_MinMax[0], 0.0f, _MinMax[4]),	// minX, minY, minZ
		Vector3(_MinMax[1], 0.0f, _MinMax[4]),	// maxX, minY, minZ
		Vector3(_MinMax[0], 0.0f, _MinMax[5]),	// minX, minY, maxZ
		Vector3(_MinMax[1], 0.0f, _MinMax[5]),	// maxX, minY, maxZ

	};

	for( s32 i=0; i<4; ++i ) 
	{
		Vector3 objPos = objectsPos[i];
		transf = Vector4(objPos, 1.0f).transform( shadowModelView );
		maxZ = max(maxZ, transf._z);
		minZ = min(minZ, transf._z);
	}
	

	Matrix shadowProj = Matrix::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -maxZ, -minZ);
	Matrix shadowMVP  = shadowModelView * shadowProj;
	
	for(s32 i=0; i<8; ++i) {
		transf = Vector4(f._point[i], 1.0f).transform(shadowMVP);

		transf._x /= transf._w;
		transf._y /= transf._w;

		maxX = max(maxX, transf._x);
		minX = min(minX, transf._x);
		maxY = max(maxY, transf._y);
		minY = min(minY, transf._y);
	}

#if 1 
	_depthProj[_currentShadowNum] = Matrix::ortho(minX, maxX, minY, maxY, -maxZ, -minZ);
#else
	f32 scaleX = 2.0f/(maxX - minX);
	f32 scaleY = 2.0f/(maxX - minX);
	f32 offsetX = -0.5f*(maxX + minX)*scaleX;
	f32 offsetY = -0.5f*(maxY + minY)*scaleY;

	shadowMVP = Matrix::IDENTITY;
	shadowMVP._m[0][0] = scaleX;
	shadowMVP._m[1][1] = scaleY;
	shadowMVP._m[3][0] = offsetX;
	shadowMVP._m[3][1] = offsetY;
	//Matrix shadowCrop = shadowMVP.transpose();
	Matrix shadowCrop = shadowMVP;
	_depthProj[_currentShadowNum] = shadowCrop * shadowProj;
#endif 	

	// ���f���r���[�v���W�F�N�V�����s��쐬
	_depthMVP					  = shadowModelView * _depthProj[_currentShadowNum];

	return minZ;	
}

//-----------------------------------------------------------------------------
//! �}�g���b�N�X���̍X�V
//-----------------------------------------------------------------------------
void CascadedShadow::update(Matrix& modelMatrix)
{
	_modelMatrix = modelMatrix;
	_pLight		 = ILight()->getLight(0);
}

//-----------------------------------------------------------------------------
//! �J��������̉��N���b�v�ʋ����v�Z
//-----------------------------------------------------------------------------
void CascadedShadow::calcFarDist()
{
	// ���݂̃J�������擾
	CameraBase*	camera  = GmCameraMan()->getCurrentCamera();
	Matrix		camProj = camera->getProjMatrix();

	for( u32 i=0; i<SPLIT_COUNT; ++i ) {
		//_farDist[i] = 0.5f*(-f[i]._fard*camProj._m[2][3]+camProj._m[3][3])/f[2]._fard+0.5f;
		_farDist[i] = f[i]._fard;
	}
}

//-----------------------------------------------------------------------------
//! �f�o�b�O�`��
//-----------------------------------------------------------------------------
void CascadedShadow::debugShadow()
{
	static s32 num = 0;

	if( GetKeyState('1') & 0x80 ) {
		num = 0;
	}else if( GetKeyState('2') & 0x80 ) {
		num = 1;
	}else if( GetKeyState('3') & 0x80 ) {
		num = 2;
	}else if( GetKeyState('4') & 0x80 ) {
		num = 4;
	}

	// �������`�悷��
	if( num == 0 ) {
		f[0].debugRender(Color(Color::RED));
	}else if( num == 1 ) {
		f[1].debugRender(Color(Color::CYAN));
	}else if( num == 2 ) {
		f[2].debugRender(Color(Color::YELLOW));
	}

	// �V���h�E�}�b�v��`�悷��B
	GmRender()->putTexture(_depthTexture->getDepthBuffer(), 0, 0, _depthTexSize, Color::WHITE, Color::NONE, 0.05f, -1, 0, true);
}

//-----------------------------------------------------------------------------
//! �V���h�E�������݊J�n
//-----------------------------------------------------------------------------
void CascadedShadow::begin(LightBase* pLight)
{
	// �V���h�E�}�b�v���X�g���������Ă����珈�����Ȃ�
	if(_currentShadowNum >= SPLIT_COUNT ) return;

	static f32	minMax[6];
	
	// ���݂̃V���h�E��ۑ�
	_pCurrentShadow = _pShadow[_currentShadowNum];

	CameraBase*	cam		= GmCameraMan()->getCurrentCamera();
	Vector3		camPos	= cam->getPosition();
	Matrix		camMat  = cam->getMatrix();

	// �������8�_���X�V
//	updateFrustumPoints(f[_currentShadowNum], camPos, camMat);
	// �N���b�v�s��쐬
//	f32 minZ = applyCropMatrix(f[_currentShadowNum], pLight);

	

	if( !_isSetRender ) {
		calcAndSetViewMatrix(pLight->getLightDir().normalize(), -Vector3(camMat._m[2]).normalize(), camPos);
		// �r���[�|�[�g��ޔ�
		glPushAttrib(GL_VIEWPORT_BIT);
		// �J��������̋������X�g���v�Z(�R�������ɃV�F�[�_�Ńe�N�X�`�����g��������)
		calcFarDist();
		// �]��
		GmShader()->changeShader(SystemShader::SHADER_TEXTURE);
		GmShader()->setUniform1fv("farDist", _farDist, SPLIT_COUNT);
		GmShader()->beginPrevShader();

		if (!GmRender()->setRenderTarget(_depthTexture, Float2(0, 0), -1, &Size<s32>(DEPTH_SIZE, DEPTH_SIZE))) {
			MessageBoxA(NULL, "�V���h�E�̃o�C���h�Ɏ��s���܂���", "�G���[", MB_OK);
		}
		glClear(GL_DEPTH_BUFFER_BIT);
		_isSetRender = true;	// �`��^�[�Q�b�g�ݒ�t���OON
	}

	// ���f���r���[�v���W�F�N�V�����s��쐬
	_depthMVP = _modelMatrix * _depthView[_currentShadowNum] * _depthProj[_currentShadowNum];

	// ���݂̃V���h�E��`��J�n����
	_pCurrentShadow->begin(pLight, _depthMVP, Float2((f32)( DEPTH_SIZE*_currentShadowNum ), 0.0f));
	
	// ���̃V���h�E�}�b�v��
	++_currentShadowNum;

}

//-----------------------------------------------------------------------------
//! �V���h�E�������ݏI��
//-----------------------------------------------------------------------------
void CascadedShadow::end()
{
	// �V���h�E�`��I������
	ShadowMap::end();
	// �^�X�N���f���ɓ��e�s���n���Ă���
	TaskModelX::setShadowProj(_depthProj);
	glPopAttrib();
	GmRender()->bindPrevRender();
	// �`��^�[�Q�b�g�ݒ�t���OOFF
	_isSetRender = false;
	glClear(GL_DEPTH_BUFFER_BIT);
	// �V���h�E�}�b�v�ԍ������Z�b�g
	_currentShadowNum = 0;
	
}


//=============================================================================
//	END OF FILE
//=============================================================================