//-----------------------------------------------------------------------------
//!
//!	@file	CollisionObject.h
//!	@brief	オブジェクト当たり判定用
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------

class ObjBase;

//-----------------------------------------------------------------------------
//!	オブジェクト当たり判定用
//-----------------------------------------------------------------------------
class CollisionObject
{
public:
	//! デフォルトコンストラクタ
	CollisionObject(){}
	//! コンストラクタ
	CollisionObject(Vector3& pos, f32 radius)
	:_hitSphere(pos, radius)
	{
	}

	//! 球情報設定
	void				setSphereParam(Vector3& pos, f32 radius)
	{
		/*Vector3	tmp = pos;
		tmp._y += _hitSphere.getRadius();*/

		//_hitSphere.setPosition(tmp);
		_hitSphere.setPosition(pos);
		_hitSphere.setRadius(radius);
	}

	//! オブジェクトとの当たり判定
	//! @param	[in]	obj			    当たり判定を行いたいオブジェクト
	bool				isHitObj(ObjBase*	obj);
	//! オブジェクトと攻撃とのあたり判定
	//! @param	[in]	attackSphere	当たり判定を行いたい攻撃の球
	bool				isHitSphere(Sphere&	attackSphere);
	//! 当たり判定用の球の取得
	Sphere&				getHitSphere() { return _hitSphere; }
protected:
	Sphere				_hitSphere;		//!< 当たり判定用の球
};

//=============================================================================
//	END OF FILE
//=============================================================================