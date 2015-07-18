//-----------------------------------------------------------------------------
//!
//!	@file	gmCameraMaganger.h
//!	@brief	カメラ管理クラス
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	カメラ管理
//-----------------------------------------------------------------------------
class CameraManager
{
private:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	CameraManager();
	//! コピーコンストラクタ
	CameraManager(CameraManager& src){};
	//! デストラクタ
	virtual ~CameraManager();

	
	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{
public:
	//! カメラを作成
	//!	@param	[in]	camera 追加するカメラ
	void		createCamera(CameraBase* camera = new CameraBase());
	//! 指定したカメラを削除
	//!	@param	[in]	Index 削除するカメラのインデックス番号
	void		deleteCamera(s32 Index);

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! インスタンスの取得
	static CameraManager* getInstance()
	{
		static CameraManager _myInstance;
		return &_myInstance;
	}

	//! 現在使用中のカメラを取得
	CameraBase*		getCurrentCamera()
	{
		return _pVecCamera.at(currentIndex);
	}

	//! 現在使用中のカメラを変更
	//! @param	[in]	Index	変更後の番号
	//! @retval	true: 更新成功	false: 更新失敗
	bool		setCurrentCamera(u32 Index)
	{
		if( Index < _pVecCamera.size() ) {
			currentIndex = Index;
			return true;
		}
		return false;
	}

	//@}
protected:
	vector<CameraBase*>		_pVecCamera;
	s32	currentIndex;
};



//-----------------------------------------------------------------------------
// アクセスインターフェイス
//-----------------------------------------------------------------------------
__forceinline
CameraManager*	GmCameraMan()
{
	return CameraManager::getInstance();
}

//=============================================================================
//	END OF FILE
//=============================================================================