//-----------------------------------------------------------------------------
//!
//!	@file	gmType.h
//!	@brief	型情報
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#pragma once


class Base;	// プロトタイプ宣言

//-----------------------------------------------------------------------------
//! 型情報
//-----------------------------------------------------------------------------
class Type
{
public:

	//-------------------------------------------------------------
	//!	@name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	//! @param	[in]	pParent		親クラスの型情報
	//!	@param	[in]	className	クラス名文字列
	//!	@param	[in]	description	説明文字列
	Type(Type*	pParent, GM_CSTR className, GM_CSTR dscription);

	//@{
	//-------------------------------------------------------------
	//!	@name 取得
	//-------------------------------------------------------------
	//@{

	//! クラス名を取得
	const char*			getName	 ()	const { return _name; }
	//! 親を取得
	const Type*			getParent() const { return _pParent; }
	//! 兄弟を取得
	const Type*			getNext	 () const { return _pNext; }
	//! 子を取得
	const Type*			getChild () const { return _pChild; }

	//! インす多オンスを作成
	virtual void*		createInstance() const = 0;
	//! クラス名文字列からクラスをnewする
	//!	@param	[in]	className	作成するクラス名
	//!	@param	[in]	pType		作成するクラスの型情報
	//!	@return	作成したクラスのインスタンス nullptrなら失敗
	static  Base*		createInstance(GM_CSTR className, const Type* pType=nullptr);

	//@}
private:
	const char*			_name;			//!< クラス名
	const char*			_description;	//!< 説明文字列

	//---- 親子関係ツリー
	Type*				_pParent;		//!< 親
	Type*				_pNext;			//!< 兄弟
	Type*				_pChild;		//!< 子
};


//-----------------------------------------------------------------------------
//!	インスタンス生成付型情報
//-----------------------------------------------------------------------------
template<typename T>
class ClassType : public Type
{
public:
	//-------------------------------------------------------------
	//!	@name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	ClassType(Type* pParent, const char className[], const char description[])
	: Type(pParent, className, description)
	{}

	//! インスタンス作成
	virtual void*	createInstance() const
	{
		return new T();
	}
	
	//@}
};

//-----------------------------------------------------------------------------
//!	ヘッダ用の定義マクロ
//-----------------------------------------------------------------------------
#define GM_TYPE( CLASS_ )									\
	typedef This	Super;	/* 親クラスの型 */				\
	typedef CLASS_	This;	/* 自クラスの型 */				\
															\
	/* 型情報本体 */											\
	static ClassType<CLASS_>	TYPE;						\
															\
	/* インスタンスの作成 */									\
	This*			createInstance() { return new This; }	\


//-----------------------------------------------------------------------------
//!	cpp用の定義マクロ
//-----------------------------------------------------------------------------
#define GM_TYPE_IMPLEMENT( CLASS_, DESC_ )					\
	ClassType<CLASS_>		CLASS_::TYPE( &Super::TYPE, #CLASS_, DESC_);

//=============================================================================
//	END OF FILE
//=============================================================================
