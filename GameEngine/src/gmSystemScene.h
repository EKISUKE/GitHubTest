//-----------------------------------------------------------------------------
//!
//!	@file	gmSystemScene.h
//!	@brief	�V�[���Ǘ��V�X�e��
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
//!	�V�[���X�^�b�N
//-----------------------------------------------------------------------------
struct StackData
{
	//! �f�t�H���g�R���X�g���N�^
	StackData()
	: _pScene	 (nullptr)
	, _typeData  (nullptr)
	{}
	SceneBase* _pScene;
	Type*	   _typeData;
};

//-----------------------------------------------------------------------------
//!	�V�[���Ǘ�
//-----------------------------------------------------------------------------
class SystemScene
{
public:

	//-------------------------------------------------------------
	//!	@name	����
	//-------------------------------------------------------------
	//@{

	//! ������
	bool		init();
	//! ���
	void		cleanup();

	//! �X�V
	void		update();

	//! �t�B���^�e�����Ȃ����̂̕`��
	void		drawNoFilter();

	//! �V�[���W�����v�����N�G�X�g
	//! @param	[in]	typeInfo	�V�[���̌^���(SceneBase���p����������)
	void		jump(const Type& typeInfo);

	//@}
	//-------------------------------------------------------------
	//!	@name �擾�E�Q��
	//-------------------------------------------------------------
	//@{

	//! �C���X�^���X�擾
	static SystemScene* getInstance() {
		static SystemScene	_myInstance;
		return &_myInstance;
	}

	//! �����^�C�v�̃V�[�����擾
	//!	@param	[in]	�T���V�[���^�C�v
	//!	@retval	�����^�C�v�̃V�[���̃|�C���^ nullptr�Ȃ玸�s
	SceneBase*			getSameTypeScene(const Type& typeInfo);

	//! �I���t���O�擾
	bool				isExit() const { return _pScene->isExit(); }

	//! �r���[�s���ݒ�
	void				setViewMatrix(const Matrix& viewMatrix) { _viewMatrix = viewMatrix; }

	//! �r���[�s����擾
	const Matrix&		getViewMatrix() const { return _viewMatrix; }

	//@}

protected:
	//-------------------------------------------------------------
	//!	@name	������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	SystemScene();
	//! �R�s�[�R���X�g���N�^
	SystemScene(SystemScene& src){};
	//! �f�X�g���N�^
	virtual ~SystemScene();

	//@}
	
private:
	vector<StackData*>		_pSceneStack;	//!< ���܂Ŏg�����V�[���ۑ�
	SceneBase*				_pScene;		//!< ���ݐi�s���̃V�[��
	const Type*				_pNextScene;	//!< ���N�G�X�g���ꂽ�V�[��
	Matrix					_viewMatrix;	//!< �r���[�s��
};

//--------------------------------------------------------------
// �A�N�Z�X�C���^�[�t�F�C�X
//--------------------------------------------------------------
__forceinline
SystemScene* GmScene()
{
	return SystemScene::getInstance();
}


//=============================================================================
//	END OF FILE
//=============================================================================