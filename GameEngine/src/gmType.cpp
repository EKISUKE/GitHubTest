//-----------------------------------------------------------------------------
//!
//!	@file	gmType.h
//!	@brief	型情報
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
//	コンストラクタ
//! @param	[in]	pParent		親のクラスの型情報
//! @param	[in]	className	クラス名文字列
//! @param	[in]	description	説明文字列
//-----------------------------------------------------------------------------
Type::Type(Type* pParent, GM_CSTR className, GM_CSTR description)
: _name(className)
, _description(description)
{
	//-------------------------------------------------------------
	// 親子継承関係をツリー構築
	//-------------------------------------------------------------
	if( pParent ) {					// 親クラスの指定がある時
		if( pParent->_pChild ) {
			//---- ２つめ以降の子になる場合

			// 子の兄弟の末尾に登録
			for( Type* p=pParent->_pChild; p; p=p->_pNext ) {
				if( p->_pNext == NULL ) {
					p->_pNext = this;
					break;
				}
			}
		}
		else {
			//---- 子の初回登録
			pParent->_pChild = this;
		}

		_pParent = pParent;
	}

}



//-----------------------------------------------------------------------------
//	クラス名文字列からクラスをnewする
//!	@param	[in]	className	作成するクラス名
//!	@param	[in]	pType		作成するクラスの型情報
//!	@return	作成したクラスのインスタンス nullptrなら失敗
//-----------------------------------------------------------------------------
Base* Type::createInstance(GM_CSTR className, const Type* pType)
{
	if( pType == nullptr ) {
		// 外部から呼ばれてきた場合は基底のクラスからツリー探索して返す
		return createInstance(className, &Base::TYPE);
	}

	//-------------------------------------------------------------
	// 自分自身が該当するかチェック
	//-------------------------------------------------------------
	if( strcmp(className, pType->getName()) == 0 ) {
		return (Base*)pType->createInstance();
	}

	//-------------------------------------------------------------
	// 再帰探索
	//-------------------------------------------------------------

	//---- 兄弟
	if( pType->getNext() ) {
		Base*	p = createInstance(className, pType->getNext());
		if( p ) return p;
	}

	//---- 子
	if( pType->getChild() ) {
		Base*	p = createInstance(className, pType->getChild());
		if( p ) return p;
	}

	return nullptr;
}



//=============================================================================
//	END OF FILE
//=============================================================================

