//-----------------------------------------------------------------------------
//!
//!	@file	Squad.h
//!	@brief	分隊
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------



struct Member
{
public:
	Man*	_me;				// 自分のキャラデータ
	Vector3	_offsetPosition;	// リーダーからのオフセット値
};

//-----------------------------------------------------------------------------
//! 分隊
//-----------------------------------------------------------------------------
class Squad
{
public:

	//-------------------------------------------------------------
	//!	@name	初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	Squad()
	{
		Init();
	}


	//! デストラクタ
	virtual ~Squad()
	{
		list<Member*>::iterator itr;
		for( itr=_pMemberList.begin(); itr!= _pMemberList.end(); )
		{
			Member* mem = *itr;
			SAFE_DELETE(mem);
			itr = _pMemberList.erase(itr);
		}
	}

	//! 初期化
	void Init()
	{
		_pMemberList.clear();
	}

	//@}
	//-------------------------------------------------------------
	//! @name 追加・削除
	//-------------------------------------------------------------
	//@{


	//! メンバーの追加
	void	addMenber(Man* member);
	//! メンバーの離脱
	void	leftMenber(Man* member);

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! リーダーの設定
	void	setLeader(Man* leader)
	{
		_leader = leader;
	}

	//! リーダー取得
	Man*	getLeader()	const { return _leader; }

	//! 自分のオフセット位置の取得
	Vector3	getMyPostion(Man* member)
	{
		Member*	tmpMem = getSameMember(member);
		// 見つかったら
		if( tmpMem )
		{
			// 自分のオフセット位置
			Vector3 myOffSetPosition;
			// オフセットをとってくる
			myOffSetPosition = tmpMem->_offsetPosition;
			// プレイヤーからの回転行列の作成
			Matrix	matLeaderRot;
			matLeaderRot = matLeaderRot.rotateY(Radian(_leader->getRotation()._y));
			myOffSetPosition = myOffSetPosition.transform(matLeaderRot);

			//return _leader->getPosition() + tmpMem->_offsetPosition;
			// オフセット結果を返す
			return _leader->getPosition() + myOffSetPosition;
		}
	}

	//! メンバーの取得
	Member*	getSameMember(Man* menber)
	{
		// メンバーのリストから全検索
		for( list<Member*>::iterator itr = _pMemberList.begin(); itr!= _pMemberList.end(); itr++ )
		{
			Member*	tmpMem = *itr;
			// 見つかったら
			if(  tmpMem->_me == menber )
			{
				// そのイテレーターを返す
				return *itr;
			}
		}
		// 見つからなかったらNULLを返す
		return NULL;
	}

	//@}

protected:
	Man*					_leader;						// 分隊長
	list<Member*>			_pMemberList;					// 分隊メンバー
	static const s32		MEMBER_MAX = 10;				// メンバーの最大数
	static const Vector3	offsetPosition[MEMBER_MAX];		// リーダーからのオフセット値
	
};

//=============================================================================
//	END OF FILE
//=============================================================================