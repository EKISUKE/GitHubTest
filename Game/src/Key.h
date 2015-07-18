//-----------------------------------------------------------------------------
//!
//!	@file	Key.h
//!	@brief	オブジェクト操作
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	オブジェクト操作インターフェース
//-----------------------------------------------------------------------------
class	IKey
{
public:
	virtual		~IKey() {}
};

//-----------------------------------------------------------------------------
//! キーコマンド構造体
//-----------------------------------------------------------------------------
struct KeyCommand
{
public:
	//! コンストラクタ
	KeyCommand(char* name)
	: _name(name)
	, _isActive(false)
	{
	}

	//! 同じ構造体かどうか返す
	//!	@param [in] name
	//! @retval	true : 一致	false : 不一致
	bool	isSame(char* name)
	{
		if( strcmp(_name, name) == 0 )
		{
			return true;
		}
		return false;
	}

	char*	_name;		//!< 名前
	bool	_isActive;	//!< アクティブフラグ
};

// プロトタイプ宣言
class Man;

//-----------------------------------------------------------------------------
//!	オブジェクト操作基底
//-----------------------------------------------------------------------------
class KeyBase : public IKey
{
public:

	//---- タイプ
	enum	KEY_TYPE {
		KEY_TYPE_AI,		//!< NPC
		KEY_TYPE_INPUT,		//!< コントローラ操作

		KEY_TYPE_NUM
	};


	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	KeyBase(Man* me, Man* leader)
	: _me(me)
	, _leader(leader)
	{
		// コマンドの初期化
		/*for( s32 i=0; i<KEY_COMMAND_NUM; ++i )
		{
			_commandState[i] = false;
		}*/
		
		_vecKeyCommand.push_back( KeyComPtr(new KeyCommand("Attack")) );
		_vecKeyCommand.push_back( KeyComPtr(new KeyCommand("Jump")) );
	};
	//! デストラクタ
	virtual			~KeyBase() 
	{
		VecKeyComPtr::iterator itr;
		for( itr = _vecKeyCommand.begin(); itr != _vecKeyCommand.end();)
		{
			(*itr).reset();
			itr = _vecKeyCommand.erase(itr);
		}
	}
	
	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{
	
	virtual void	Update() = 0;

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//---- 取得関連
	//! タイプ取得
	KEY_TYPE		getKeyType()		const {return _keyType;} 
	//! 移動方向取得
	Vector3			getVector()			const {return _movVector;}
	//! スティックのベクトル
	Vector3			getStickVector()	const {return _stickVector.normalize();}
	//! コマンドが押されたかチェック
	//!	@param [in]		key		コマンド
	//bool			getPushState(KEY_COMMAND	key) const { return _commandState[key]; }
	bool			getPushState(char*	key)  
	{ 
		VecKeyComPtr::iterator itr;
		// 同じ名前の構造体を探す
		for( itr = _vecKeyCommand.begin(); itr != _vecKeyCommand.end(); ++itr )
		{
			// 同じ名前なら
			KeyComPtr keycommand = *itr;
			// 同じ名前なら
			if( keycommand->isSame(key) )
			{
				// アクティブフラグを返す
				return keycommand->_isActive;
			}
		}
		return false;
	}

	//---- セット関連

	//! 目標の情報セット
	virtual void	setTarget(Man*	target){ _target = target; }
	
	bool			setCommandFlag(char* command, bool flag)
	{
		VecKeyComPtr::iterator itr;
		// 同じ名前の構造体を探す
		for( itr = _vecKeyCommand.begin(); itr != _vecKeyCommand.end(); ++itr )
		{
			// 同じ名前なら
			KeyComPtr keycommand = *itr;
			// 同じ名前なら
			if( keycommand->isSame(command) )
			{
				// アクティブフラグを返す
				keycommand->_isActive = flag;
				return true;
			}
		}
		// 設定失敗
		return false;
	}
	
	//@}
protected:
	Vector3		_movVector;							//!< 移動ベクトル
	Vector3		_stickVector;						//!< コントローラーのベクトル
	Man*		_leader;							//!< リーダー
	Man*		_target;							//!< ターゲット
	Man*		_me;								//!< 自分

	KEY_TYPE	_keyType;							//!< タイプ

	typedef shared_ptr<KeyCommand> KeyComPtr;		//!< キーコマンドのスマートポインタ
	typedef vector<KeyComPtr>	   VecKeyComPtr;	//!< スマートポインタ配列
	VecKeyComPtr				   _vecKeyCommand;	//!< コマンド状態
};

//-----------------------------------------------------------------------------
//!	AI操作基底
//-----------------------------------------------------------------------------
class	AI	: public KeyBase
{
public:
	//! NPCの種類
	enum TYPE
	{
		TYPE_ATTACKER,	//!< 攻撃タイプ
		TYPE_DEFFENDER,	//!< 防御タイプ
		TYPE_HEALLER,	//!< 回復タイプ
		TYPE_FEINTER,	//!< 陽動タイプ
		TYPE_MAX		//!< タイプの最大値
	};
public:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	AI(Man* me, Man* leader);

	//! デストラクタ
	virtual ~AI(){}

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 更新
	virtual void	Update();

	//@}

	////! 目標をセット
	//virtual void	setToTargetPos(Vector3	myPos, Vector3 targetPos)
	//{
	//	setMyPos(myPos);
	//	setTargetPos(targetPos);
	//	_dirToTarget = targetPos - myPos;
	//}
protected:
	Vector3		_dirToTarget;
	TYPE		_type;

	//---- 遅延用
	u32				_oldTime;			//!< 前回の時間
	u32				_nowTime;			//!< 現在の時間
};

//=============================================================================
//	END OF FILE
//=============================================================================