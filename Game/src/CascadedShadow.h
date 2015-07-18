//-----------------------------------------------------------------------------
//!
//!	@file	CascadedShadow.h
//!	@brief	�J�X�P�[�h�V���h�E
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


#define DEPTH_SIZE 2048		//!< �V���h�E�e�N�X�`���T�C�Y


//-----------------------------------------------------------------------------
//! �J�X�P�[�h�V���h�E
//-----------------------------------------------------------------------------
class CascadedShadow
{
private:
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//---- �O������̍쐬�폜�֎~
	//! �R���X�g���N�^
	CascadedShadow();
	//! �R�s�[�R���X�g���N�^
	CascadedShadow(CascadedShadow& src){}
	//! �f�X�g���N�^
	virtual ~CascadedShadow();
	
public:
	//! ���ˉe�s��̏�����
	void	initOrthoMatrix();

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �r���[�s����v�Z���č��
	//! @param	[in]	lightDir	���C�g����
	//! @param	[in]	viewDir		�r���[����
	//! @param	[in]	camPos		�J�������W
	void					calcAndSetViewMatrix(Vector3& lightDir, Vector3& viewDir, Vector3& camPos);
	
	//! �O�ډ~���v�Z����
	//! @param	[in]	p1	���W1
	//! @param	[in]	p2	���W2
	//! @param	[in]	p3	���W3
	//!	@retval	���\����
	Sphere&					calcCircumscribeCircle(Vector3& p1, Vector3& p2, Vector3& p3);
	

	//! �V���h�E�e�N�X�`�����쐬
	bool					createShadowTexture();

	//! �������8�_�̍X�V
	void					updateFrustumPoints(Frustum& f, Vector3& center, Matrix& cameraMatrix);

	//! ���������̍X�V
	void					updateSplitDist(f32 nd, f32 fd);

	//! �N���b�v�s��̓K�p
	f32						applyCropMatrix(Frustum &f, LightBase* pLight);

	//! �}�g���b�N�X���̍X�V
	void					update(Matrix& modelMatrix);

	//! �J��������̉��N���b�v�ʋ����v�Z
	void					calcFarDist();

	//! �f�o�b�O�`��
	void					debugShadow();

	//@}
	//-------------------------------------------------------------
	//! @name �J�n�E�I��
	//-------------------------------------------------------------
	//@{
	//! �V���h�E�������݊J�n
	void					begin(LightBase* pLight);
	//! �V���h�E�������ݏI��
	void					end();
	//@}
	//-------------------------------------------------------------
	//! @name �擾/�ݒ�
	//-------------------------------------------------------------
	//@{

	//! ���f���̍ő�ŏ��ݒ�
	void					setMinMax(f32 minX, f32 maxX, f32 minY, f32 maxY, f32 minZ, f32 maxZ) {
		_MinMax[0] = minX;
		_MinMax[1] = maxX;
		_MinMax[2] = minY;
		_MinMax[3] = maxY;
		_MinMax[4] = minZ;
		_MinMax[5] = maxZ;
	}
	

	//! �C���X�^���X�擾
	static CascadedShadow*	getInstance()
	{
		static CascadedShadow _myInstance;
		return &_myInstance;
	}
	
	//! �f�v�X�e�N�X�`���擾
	GLuint					getDepthTexture()		 const { return _depthTexture->getDepthBuffer(); }

	//! �������擾
	s32						getSplitCount()			 const { return SPLIT_COUNT; }
	
	//! �v���W�F�N�V�����s��擾
	Matrix					getProjMatrix(u32 Index) const { return _depthProj[Index]; }

	//! �r���[�s��擾
	Matrix					getViewMatrix(u32 Index) const { return _depthView[Index]; }

	//! �o�C�A�X�s��擾
	Matrix					getBiasMatrix()			 const { return _biasMatrix; }

	//! �V���h�E�}�b�v��T��
	//!	@param	[in]	search	�T���V���h�E�̃|�C���^
	bool					sameShadowMap(ShadowMap* search);

	

	//@}
private:
	static const u32		SPLIT_COUNT = 3;			    //!< ������

	Frustum					f[SPLIT_COUNT];					//!< ������
	f32						_farDist[SPLIT_COUNT];			//!< �J��������̉��N���b�v�ʃ��X�g
	f32						_MinMax[6];					    //!< �ő�l�ŏ��l(0:minX 1:maxX 2:minY 3:maxY 4:minZ 5:maxZ)
	f32						_centerLength[SPLIT_COUNT];		//!< ���_�܂ł̋���
	u32						_currentShadowNum;			    //!< ���ݎg�p���̃V���h�E�}�b�v�ԍ�
	f32						_split_weight;				    
	Matrix					_modelMatrix;				    //!< ���[���h�s��
	LightBase*				_pLight;					    //!< ���C�g
	ShadowMap*				_pShadow[SPLIT_COUNT];		    //!< �V���h�E�}�b�v
	ShadowMap*				_pCurrentShadow;			    //!< ���݂̃V���h�E�}�b�v
	static const Matrix		_biasMatrix;				    //!< �o�C�A�X�s��(�e�N�X�`�����W����X�N���[�����W�ɂ��邽�߂̂���)
	Texture*				_depthTexture;					//!< �f�v�X�e�N�X�`��
	//GLuint					_depthFrameBuffer;				//!< �f�v�X�t���[���o�b�t�@
	//GLuint					_depthTexture;					//!< �f�v�X�e�N�X�`��
	Matrix					_depthProj[SPLIT_COUNT];		//!< �f�v�X�v���W�F�N�V�����s��(��SPLIT_COUNT��ύX������V�F�[�_���ύX�K�{)
	Matrix					_depthView[SPLIT_COUNT];		//!< �f�v�X�r���[�s��(�r���[�s��͕ύX���Ȃ��̂ŁA�ЂƂ���
	Matrix					_depthMVP;						//!< �f�v�X�r���[�s��
	Size<s32>				_depthTexSize;					//!< �f�v�X�e�N�X�`���T�C�Y
	bool					_isSetRender;					//!< �����_�[�^�[�Q�b�g��ݒ肵�����ǂ���
};

//-----------------------------------------------------------------------------
// �A�N�Z�X�C���^�[�t�F�C�X
//-----------------------------------------------------------------------------
__forceinline
CascadedShadow* ICasCaded()
{
	return CascadedShadow::getInstance();
}


//=============================================================================
//	END OF FILE
//=============================================================================