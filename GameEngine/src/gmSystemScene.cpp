//-----------------------------------------------------------------------------
//!
//!	@file	gmSystemScene.cpp
//!	@brief	�V�[���Ǘ��V�X�e��
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
SystemScene::SystemScene()
: _pScene	 (nullptr)		// ���݂̎��s���̃V�[��
, _pNextScene(nullptr)		// ���N�G�X�g���ꂽ�V�[��
, _viewMatrix(Matrix::IDENTITY)
{
	
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
SystemScene::~SystemScene()
{
	vector<StackData*>::iterator itr;
	for( itr=_pSceneStack.begin(); itr!=_pSceneStack.end();)
	{
		StackData* pStack = *itr;
		SceneBase* pScene = pStack->_pScene;
		pScene->cleanup();
		GM_RELEASE(pScene);
		SAFE_DELETE(pStack);
		itr = _pSceneStack.erase(itr);
	}
	cleanup();
}

//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool SystemScene::init()
{
	_viewMatrix = Matrix::view(Vector3(0,0,1), Vector3(0.0f));
	return true;
}

//-----------------------------------------------------------------------------
//! ���
//-----------------------------------------------------------------------------
void SystemScene::cleanup()
{

}

//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void SystemScene::update()
{
	// ���̃V�[���ւ̃W�����v���N�G�X�g������ꍇ�͐؂�ւ���
	if( _pNextScene ) {

		// �����^�C�v�̃V�[�����擾
		_pScene = getSameTypeScene(*_pNextScene);
		// �Ȃ����
		if( _pScene == nullptr ) {
			// �V�K�쐬
			_pScene = (SceneBase*)_pNextScene->createInstance();
			// �X�^�b�N���쐬
			StackData*	stack = new StackData();
			stack->_pScene    = _pScene;
			stack->_typeData  = (Type*)_pNextScene;
			// �X�^�b�N����
			_pSceneStack.push_back(stack);
		}
		// �������m�ۂł��Ă�����
		if( _pScene != nullptr ) {
			// ������
			if( _pScene->init() == false ) {
				// ���s������폜
				_pScene->cleanup();
				GM_RELEASE(_pScene);
			}
		}
		_pNextScene = nullptr;	// ���N�G�X�g������
	}

	//-------------------------------------------------------------
	// �X�V
	//-------------------------------------------------------------
	if( _pScene ) {
		_pScene->update();
	}
}

//-----------------------------------------------------------------------------
//! �t�B���^�e�����Ȃ����̂̕`��
//-----------------------------------------------------------------------------
void SystemScene::drawNoFilter()
{
	if( _pScene ) {
		_pScene->drawNoFilter();
	}
}

//-----------------------------------------------------------------------------
//! �V�[���W�����v�����N�G�X�g
//! @param	[in]	typeInfo	�V�[���̌^���(SceneBase���p����������)
//-----------------------------------------------------------------------------
void SystemScene::jump(const Type& typeInfo)
{
	_pNextScene = &typeInfo;
}

//-----------------------------------------------------------------------------
//! �����V�[�����擾
//!	@param	[in]	�T���V�[���^�C�v
//!	@retval	�����^�C�v�̃V�[���̃|�C���^ nullptr�Ȃ玸�s
//-----------------------------------------------------------------------------
SceneBase* SystemScene::getSameTypeScene(const Type& typeInfo)
{
	// �S����
	for( u32 i=0; i<_pSceneStack.size(); ++i )
	{

		GM_CSTR sceneName	  = _pSceneStack[i]->_typeData->getName();;
		GM_CSTR typeSceneName = typeInfo.getName();

		// �X�^�b�N�ɓ������O�̂��̂������
		if( strcmp(sceneName, typeSceneName ) == 0 ){
			return _pSceneStack[i]->_pScene;
		}
	}
	return nullptr;
}

//=============================================================================
//	END OF FILE
//=============================================================================