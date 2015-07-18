//-----------------------------------------------------------------------------
//!
//!	@file	gmBase.h
//!	@brief	基底クラス
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//! 基底クラス
//-----------------------------------------------------------------------------
class Base
{
public:
	typedef Base			This;		//!< 自分のクラスの型

	static ClassType<Base>	TYPE;		//!< 自分のタイプ

	//-------------------------------------------------------------
	//!	@name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	Base();


protected:
	
	//! デストラクタ
	// ※ deleteで直接削除することを禁止する。
	//    解放は GM_RELEASE(p) 経由で行うことを強制指定。
	virtual ~Base() {}

	//@}
	
public:
	s32						_refCount;	//!< 参照カウンター

	//! 参照カウンターを加算
	//!	@return	加算後の参照カウンター値
	s32		addRef()
	{
		return ++_refCount;
	}

	// 解放
	//!	@return	減算後の参照カウンター値
	s32		release()
	{
		s32	refCount = --_refCount;

		if( refCount == 0 ) {
			delete this;		// 自殺
		}
		return refCount;	// ※deleteで消えていることを考慮してローカル変数を使用
	}

};


//=============================================================================
//	END OF FILE
//=============================================================================
