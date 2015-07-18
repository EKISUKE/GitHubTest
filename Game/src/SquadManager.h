//-----------------------------------------------------------------------------
//!
//!	@file	SquadManager.h
//!	@brief	分隊管理
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//! 分隊管理
//-----------------------------------------------------------------------------
class SquadManager
{
private:
	//-------------------------------------------------------------
	//!	@name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	SquadManager()
	{
		Init();
	};

	//! コピーコンストラクタ
	SquadManager(SquadManager& src){}

	//! デストラクタ
	virtual ~SquadManager()
	{
		list<Squad*>::iterator itr;
		for( itr = _pSquadList.begin(); itr != _pSquadList.end(); )
		{
			Squad* squad = *itr;
			SAFE_DELETE(squad);
			itr = _pSquadList.erase(itr);
		}
		
	};

public:
	
	//! 初期化
	void	Init()
	{
		_pSquadList.clear();
	}

	//@}
	//-------------------------------------------------------------
	//!	@name 取得
	//-------------------------------------------------------------
	//@{

	//! インスタンス取得
	static SquadManager* getInstance()
	{
		static SquadManager _myInstance;
		return &_myInstance;
	}

	//! 指定した番号の分隊を返す
	Squad*	getSquad(s32 Index)
	{
		list<Squad*>::iterator itr = _pSquadList.begin();
		for( s32 i=0; i<Index; i++ )
		{
			itr++;
		}
		return *itr;
	}

	//! 同じ分隊を探す
	Squad*	getSameSquad(Squad* search);

	//@}
	//-------------------------------------------------------------
	//!	@name 登録・解除
	//-------------------------------------------------------------
	//@{

	//! 分隊の登録
	void	registSquad(Squad*	_registSquad);

	//! 分隊の解除
	void	unregistSquad(Squad*	_unregistSquad);
	
	//@}

	

protected:
	 list<Squad*>	_pSquadList;		//!< 分隊リスト
};

//-----------------------------------------------------------------------------
// アクセスインターフェイス
//-----------------------------------------------------------------------------
__forceinline
SquadManager*	ISquadManager()
{
	return SquadManager::getInstance();
}


//=============================================================================
//	END OF FILE
//=============================================================================