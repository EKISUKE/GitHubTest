//-----------------------------------------------------------------------------
//!
//!	@file	gmSystemScene.h
//!	@brief	シーン管理システム
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
//!	シーンスタック
//-----------------------------------------------------------------------------
struct StackData
{
	//! デフォルトコンストラクタ
	StackData()
	: _pScene	 (nullptr)
	, _typeData  (nullptr)
	{}
	SceneBase* _pScene;
	Type*	   _typeData;
};

//-----------------------------------------------------------------------------
//!	シーン管理
//-----------------------------------------------------------------------------
class SystemScene
{
public:

	//-------------------------------------------------------------
	//!	@name	制御
	//-------------------------------------------------------------
	//@{

	//! 初期化
	bool		init();
	//! 解放
	void		cleanup();

	//! 更新
	void		update();

	//! フィルタ影響しないものの描画
	void		drawNoFilter();

	//! シーンジャンプをリクエスト
	//! @param	[in]	typeInfo	シーンの型情報(SceneBaseを継承したもの)
	void		jump(const Type& typeInfo);

	//@}
	//-------------------------------------------------------------
	//!	@name 取得・参照
	//-------------------------------------------------------------
	//@{

	//! インスタンス取得
	static SystemScene* getInstance() {
		static SystemScene	_myInstance;
		return &_myInstance;
	}

	//! 同じタイプのシーンを取得
	//!	@param	[in]	探すシーンタイプ
	//!	@retval	同じタイプのシーンのポインタ nullptrなら失敗
	SceneBase*			getSameTypeScene(const Type& typeInfo);

	//! 終了フラグ取得
	bool				isExit() const { return _pScene->isExit(); }

	//! ビュー行列を設定
	void				setViewMatrix(const Matrix& viewMatrix) { _viewMatrix = viewMatrix; }

	//! ビュー行列を取得
	const Matrix&		getViewMatrix() const { return _viewMatrix; }

	//@}

protected:
	//-------------------------------------------------------------
	//!	@name	初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	SystemScene();
	//! コピーコンストラクタ
	SystemScene(SystemScene& src){};
	//! デストラクタ
	virtual ~SystemScene();

	//@}
	
private:
	vector<StackData*>		_pSceneStack;	//!< 今まで使ったシーン保存
	SceneBase*				_pScene;		//!< 現在進行中のシーン
	const Type*				_pNextScene;	//!< リクエストされたシーン
	Matrix					_viewMatrix;	//!< ビュー行列
};

//--------------------------------------------------------------
// アクセスインターフェイス
//--------------------------------------------------------------
__forceinline
SystemScene* GmScene()
{
	return SystemScene::getInstance();
}


//=============================================================================
//	END OF FILE
//=============================================================================