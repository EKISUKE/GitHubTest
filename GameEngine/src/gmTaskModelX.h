//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskModelX.h
//!	@brief	3D���f��(X�t�@�C���`��)
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//---- �v���g�^�C�v�錾
class AssetModelX;
class Frame;

//--------------------------------------------------------------
//! �A�j���[�V�����Đ��̑���
//--------------------------------------------------------------
enum ANIMATION_PLAY_TYPE {
	ANIMATION_PLAY_DEFAULT,		//!< �A�j���[�V�����f�[�^�ɏ�������
	ANIMATION_PLAY_REPEAT,		//!< ���s�[�g�Đ�
};

//-----------------------------------------------------------------------------
//!	���f���N���X
//-----------------------------------------------------------------------------
class TaskModelX : public TaskBase
{
	friend class SystemCollision;
public:
	//-------------------------------------------------------------
	//!	@name	������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	TaskModelX(void);
	//! �f�X�g���N�^
	virtual ~TaskModelX(void);

	//@}
	//-------------------------------------------------------------
	//!	@name	�^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! ������
	virtual	bool		init(void);
	//! ����񓯊����s
	virtual	void		asyncUpdate(void);
	//! ���񓯊����s
	virtual	void		update(void);
	//! �`��
	virtual	void		render(bool	isShadow);

	//! MinMax�v�Z
	void				calcMinMax();
	//! �f�o�b�O�`��
	void				debugRender();
	//! �f�o�b�O�`��(2D)
	void				debugRender2D(void);

	//@}
	//-------------------------------------------------------------
	//!	@name	�擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! ���f���f�[�^��ݒ�
	bool				setModel(AssetModelX* pModel);
	//! �g�嗦��ݒ�
	void				setScale(f32 scale) { _scale = scale; }

	//! �t���[�����ړ�
	bool				setOffset(GM_CSTR frameName, Vector3& offset) { return _pModel->setOffsetByName(frameName, offset); }
	//! �t���[������]
	bool				setRotate(GM_CSTR frameName, Vector3& offset) { return _pModel->setRotateByName(frameName, offset); }
	//! �������ǂ����t���O�ݒ�
	bool				setDynamic(GM_CSTR frameName, bool& isDynamic) { return _pModel->setDynamicByName(frameName, isDynamic); }
	//! ��]�s���ݒ�
	void				setRotateMatrix(const Matrix& m) { _rotateMatrix = m; }
	//! ���[���h�s���ݒ�
	void				setWorldMatrix(const Matrix& m) { _worldMatrix = m; }

	//! �V���h�E�}�b�s���O�̗L�����t���O�ݒ�
	void				setShadowEnable(const bool& enable) { _shadowEnable = enable; }

	//! ���f���f�[�^���擾
	AssetModelX*		getModel(void) const { return _pModel; }
	//! �g�嗦���擾
	f32					getScale(void) const { return _scale; }



	//! ��]�s����擾
	const Matrix&		getRotateMatrix(void) const { return _rotateMatrix; }

	//! ���[���h�s����擾
	const Matrix&		getWorldMatrix(void) const { return _worldMatrix; }


	//@}
	//-------------------------------------------------------------
	//!	@name	�A�j���[�V��������֘A
	//-------------------------------------------------------------
	//@{

	//! ���O����A�j���[�V����������
	//!	@param	[in]	name	�A�j���[�V������
	//!	@return �A�j���[�V�����ԍ� (-1:��v���閼�O���Ȃ����o�ł��Ȃ�����)
	s32					searchAnimation(GM_CSTR name);

	//! �A�j���[�V�������Đ�
	//!	@param	[in]	animatioNumber	�A�j���[�V�����ԍ�(���̒l�Ȃ�A�j���[�V�����Ȃ�)
	//!	@param	[in]	playType		�A�j���[�V�����Đ�����(ANIMATION_PLAY_TYPE)
	//!	@param	[in]	interpolate		true:���O�̃A�j���[�V�����Ƃ̕�Ԃ��� false:��ԂȂ�
	//!	@retval	true	����
	//!	@retval	false	���s�i���̔ԍ��ɃA�j���[�V������񂪖����j
	bool				setAnimation(s32 animationNumber, ANIMATION_PLAY_TYPE playType=ANIMATION_PLAY_DEFAULT, bool interpolate=true);

	//! �A�j���[�V�������x��ύX
	//!	@param	[in]	ratio	�Đ����x�{��(1.0f=����)
	void				setAnimationSpeed(f32 ratio) { _animationSpeed = ratio; }

	//! �V���h�E�̃r���[�s��ݒ�
	//!	@param	[in]	viewMatrix	�V���h�E�̃r���[�s��̃|�C���^
	static void			setShadowViewMatrix(Matrix* viewMatrix) {
		for(u32 i=0; i<_splitCount; ++i) {
			_shadowView[i] = viewMatrix[i]; 
		}
	}

	//! �o�C�A�X�s��̐ݒ�
	//!	@param	[in]	biasMatrix	�o�C�A�X�s��
	static void			setBiasMatrix(const Matrix& biasMatrix) { _biasMatrix = biasMatrix; }

	//! ���e�s���ݒ�
	//! @param	[in]	projMatrix	���e�s��̃|�C���^
	static void			setShadowProj(Matrix* projMatrix)		{ 
		for(u32 i=0; i<_splitCount; ++i) {
			_shadowProj[i] = projMatrix[i];
		}
	}

	//! �f�v�X�e�N�X�`���n���h���ݒ�
	//! @param	[in]	texHandle	�e�N�X�`���n���h��
	static void			setDepthTexHandle(const GLuint& texHandle)	{ _depthTexHandle = texHandle; }
	
	//! �Đ����̃A�j���[�V�����ԍ����擾
	//!	@return �A�j���[�V�����ԍ� (-1:�A�j���[�V�����Ȃ�)
	s32					getAnimation(void)		const { return _animationNumber; }

	//! �A�j���[�V�������x���擾
	f32					getAnimationSpeed(void) const { return _animationSpeed; }

	//! �A�j���[�V�����̐i�݋�擾
	//! @return �A�j���[�V�����̐i�݋ 0.0f �` 1.0f
	f32					getAnimationRate(void)	const { return _animationRate; }

	//! �Đ����̃A�j���[�V�������擾
	GM_CSTR				getAnimationName(void)	const;
	//@}
private:
	//! �t���[���ɑ����郁�b�V����`��
	void						_renderFrame(AssetModelX::Frame* pFrame);
	//!	���b�V����`��
	void						_renderMesh(AssetModelX::Mesh* pMesh);
private:

	//---- �V���h�E�p
	bool						_shadowEnable;				//!< �V���h�E�g�����ǂ���

	//---- ���f�����
	AssetModelX*				_pModel;					//!< ���f���f�[�^
	f32							_scale;						//!< �g�嗦

	//---- �s��
	Matrix						_rotateMatrix;				//!< ��]�s��
	Matrix						_worldMatrix;				//!< �p���s��

	//---- �V���h�E�p�s��
	static const u32			_splitCount = 3;			//!< �J�X�P�[�h�̕�����
	static GLuint				_depthTexHandle;			//!< �f�v�X�e�N�X�`���̃n���h��
	static Matrix				_biasMatrix;				//!< �V���h�E�̃r���[�s��
	static Matrix				_shadowView[_splitCount];	//!< �V���h�E�̃r���[�s��
	static Matrix				_shadowProj[_splitCount];	//!< �V���h�E�̃v���W�F�N�V�����s��
	

	//---- �A�j���[�V����
	bool						_isAnimationRepeat;			//!< �A�j���[�V���������s�[�g�Đ����邩�ǂ���
	ANIMATION_PLAY_TYPE			_animationPlayType;			//!< �A�j���[�V�����̑���
	s32							_animationNumber;			//!< �A�j���[�V�����ԍ�
	f32							_animationTime;				//!< �A�j���[�V�����̍Đ����̎���
	f32							_animationSpeed;			//!< �A�j���[�V�����Đ����x
	f32							_animationRate;				//!< �Đ����̃A�j���[�V�����̍Đ��䗦
	GM_CSTR						_animationName;				//!< �Đ����̃A�j���[�V������

	//---- �A�j���[�V�������
	f32							_animationBlendRatio;		//!< �A�j���[�V�����u�����h�W��

	//---- �{�[���֐�
	struct Joint;
	vector<Joint>				_joints;					//!< �A�j���[�V�����{�[���֐ߔz��
	vector<Matrix>				_skinLocalMatrices;			//!< �{�[���֐ߍs��i���[�J���j

	//---- �`��o�b�t�@�I�u�W�F�N�g
	VertexArrayObject*			_pVAO;
};

//-----------------------------------------------------------------------------
//!	�W���C���g
//!	@brief	�{�[���֐�
//!	�{�[���֐߂͊e�֐߂̍s����̑��ɁA�A�j���[�V������Ԃ̂��߂�
//!	�u�����h���̎p���������悤�ɂ���B
//-----------------------------------------------------------------------------
struct TaskModelX::Joint {
	//! �R���X�g���N�^
	Joint(void)
	: _animationMatrix(Matrix::IDENTITY)		// �A�j���[�V�����s��
	, _transformMatrix(Matrix::IDENTITY)		// �t���[���s��
	, _rotation       (Quaternion::IDENTITY)	// �A�j���[�V������]
	, _position       (0.0f, 0.0f, 0.0f)		// �A�j���[�V�����ړ�
	, _scale          (1.0f, 1.0f, 1.0f)		// �A�j���[�V�����g�k
	, _blendRotation  (Quaternion::IDENTITY)	// �A�j���[�V������](�u�����h��)
	, _blendPosition  (0.0f, 0.0f, 0.0f)		// �A�j���[�V�����ړ�(�u�����h��)
	, _blendScale     (1.0f, 1.0f, 1.0f)		// �A�j���[�V�����g�k(�u�����h��)
	{}

	//---- �s�񉻂����f�[�^
	Matrix			_animationMatrix;			//!< �A�j���[�V�����s��
	Matrix			_transformMatrix;			//!< �t���[���s��

	//---- �A�j���[�V�����p��
	// (A) ���݂̍Đ����̃A�j���[�V�����p��
	Quaternion		_rotation;					//!< �A�j���[�V������]
	Vector3			_position;					//!< �A�j���[�V�����ړ�
	Vector3			_scale;						//!< �A�j���[�V�����g�k
	//    ������ (B)����(A)�Ɍ������ĕ��
	// (B) �A�j���[�V�����ύX�O�̎p��
	Quaternion		_blendRotation;				//!< �A�j���[�V������](�u�����h��)
	Vector3			_blendPosition;				//!< �A�j���[�V�����ړ�(�u�����h��)
	Vector3			_blendScale;				//!< �A�j���[�V�����g�k(�u�����h��)
};

//===========================================================================
//	END OF FILE
//===========================================================================
