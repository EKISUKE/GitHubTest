//-----------------------------------------------------------------------------
//!
//!	@file	gmSystemScene.cpp
//!	@brief	シーン管理システム
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
SystemScene::SystemScene()
: _pScene	 (nullptr)		// 現在の実行中のシーン
, _pNextScene(nullptr)		// リクエストされたシーン
, _viewMatrix(Matrix::IDENTITY)
{
	
}

//-----------------------------------------------------------------------------
//! デストラクタ
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
//! 初期化
//-----------------------------------------------------------------------------
bool SystemScene::init()
{
	_viewMatrix = Matrix::view(Vector3(0,0,1), Vector3(0.0f));
	return true;
}

//-----------------------------------------------------------------------------
//! 解放
//-----------------------------------------------------------------------------
void SystemScene::cleanup()
{

}

//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void SystemScene::update()
{
	// 次のシーンへのジャンプリクエストがある場合は切り替える
	if( _pNextScene ) {

		// 同じタイプのシーンを取得
		_pScene = getSameTypeScene(*_pNextScene);
		// なければ
		if( _pScene == nullptr ) {
			// 新規作成
			_pScene = (SceneBase*)_pNextScene->createInstance();
			// スタック情報作成
			StackData*	stack = new StackData();
			stack->_pScene    = _pScene;
			stack->_typeData  = (Type*)_pNextScene;
			// スタックする
			_pSceneStack.push_back(stack);
		}
		// メモリ確保できていたら
		if( _pScene != nullptr ) {
			// 初期化
			if( _pScene->init() == false ) {
				// 失敗したら削除
				_pScene->cleanup();
				GM_RELEASE(_pScene);
			}
		}
		_pNextScene = nullptr;	// リクエストを消す
	}

	//-------------------------------------------------------------
	// 更新
	//-------------------------------------------------------------
	if( _pScene ) {
		_pScene->update();
	}
}

//-----------------------------------------------------------------------------
//! フィルタ影響しないものの描画
//-----------------------------------------------------------------------------
void SystemScene::drawNoFilter()
{
	if( _pScene ) {
		_pScene->drawNoFilter();
	}
}

//-----------------------------------------------------------------------------
//! シーンジャンプをリクエスト
//! @param	[in]	typeInfo	シーンの型情報(SceneBaseを継承したもの)
//-----------------------------------------------------------------------------
void SystemScene::jump(const Type& typeInfo)
{
	_pNextScene = &typeInfo;
}

//-----------------------------------------------------------------------------
//! 同じシーンを取得
//!	@param	[in]	探すシーンタイプ
//!	@retval	同じタイプのシーンのポインタ nullptrなら失敗
//-----------------------------------------------------------------------------
SceneBase* SystemScene::getSameTypeScene(const Type& typeInfo)
{
	// 全検索
	for( u32 i=0; i<_pSceneStack.size(); ++i )
	{

		GM_CSTR sceneName	  = _pSceneStack[i]->_typeData->getName();;
		GM_CSTR typeSceneName = typeInfo.getName();

		// スタックに同じ名前のものがあれば
		if( strcmp(sceneName, typeSceneName ) == 0 ){
			return _pSceneStack[i]->_pScene;
		}
	}
	return nullptr;
}

//=============================================================================
//	END OF FILE
//=============================================================================