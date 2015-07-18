//-----------------------------------------------------------------------------
//!
//!	@file	gmAABBTree.h
//!	@brief	AABB-Tree
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//!	ラインの描画 (Flat)
//!	@param	[in]	p1		座標1
//!	@param	[in]	p1		座標2
//!	@param	[in]	color	カラー
void	drawLineF(const Vector3& p1, const Vector3& p2, const Vector4& color);


//-----------------------------------------------------------------------------
//! バウンディングボックス
//-----------------------------------------------------------------------------
struct AABB
{
	//! 空のバウンディングボックスを作成
	void		setEmpty()
	{
		_min = Vector3(+FLT_MAX, +FLT_MAX, +FLT_MAX);
		_max = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	}

	//! 指定座標を含むバウンディングに拡張
	void		expand(const Vector3& position)
	{
		_min._x = min(_min._x, position._x);
		_min._y = min(_min._y, position._y);
		_min._z = min(_min._z, position._z);
	
		_max._x = max(_max._x, position._x);
		_max._y = max(_max._y, position._y);
		_max._z = max(_max._z, position._z);
	}

	Vector3		_min;
	Vector3		_max;
};

namespace AABBCollision {

	inline

	//!	AABB vs AABB
	//!	@param	[in]	a	AABBオブジェクト
	//!	@param	[in]	b	AABBオブジェクト
	bool isHit(const AABB& a, const AABB& b)
	{
		if( a._max._x < b._min._x ) return false;
		if( a._max._y < b._min._y ) return false;
		if( a._max._z < b._min._z ) return false;

		if( b._max._x < a._min._x ) return false;
		if( b._max._y < a._min._y ) return false;
		if( b._max._z < a._min._z ) return false;

		return true;
	}

	inline

	//! めり込み量を取得する
	//!	@param	[in]	a	AABBオブジェクト
	//!	@param	[in]	b	AABBオブジェクト
	Vector3 getSinkVal(const AABB& a, const AABB& b)
	{
		Vector3 result = Vector3(0,0,0);

		if( isHit(a, b) )
		{
			//---- すべての差分を求める
			f32 sinkX1 = a._max._x - b._min._x;
			f32 sinkX2 = b._min._x - a._max._x;
			f32 sinkX3 = a._min._x - b._max._x;
			f32 sinkX4 = b._max._x - a._min._x;

			/*f32 sinkY1 = a._max._y - b._min._y;
			f32 sinkY2 = b._min._y - a._max._y;
			f32 sinkY3 = a._min._y - b._max._y;
			f32 sinkY4 = b._max._y - a._min._y;*/

			f32 sinkZ1 = a._max._z - b._min._z;
			f32 sinkZ2 = b._min._z - a._max._z;
			f32 sinkZ3 = a._min._z - b._max._z;
			f32 sinkZ4 = b._max._z - a._min._z;

			//--- それぞれで一番小さいめり込み量を求める
			f32 sinkMinX = min( min(abs(sinkX1), abs(sinkX2))
							  , min(abs(sinkX3), abs(sinkX4)) );
			////--- それぞれで一番小さいめり込み量を求める
			//f32 sinkMinY = min( min(abs(sinkY1), abs(sinkY2))
			//				  , min(abs(sinkY3), abs(sinkY4)) );

			f32 sinkMinZ = min( min(abs(sinkZ1), abs(sinkZ2))
							  , min(abs(sinkZ3), abs(sinkZ4)) );

			//f32 minVal = min( min(sinkMinX, sinkMinY), sinkMinZ );
			f32 minVal = min( sinkMinX, sinkMinZ );

			//---- 絶対値で比較しているので値を戻す
			sinkMinX = math::getSameValofFour(sinkMinX, sinkX1, sinkX2, sinkX3, sinkX4);
			//sinkMinY = math::getSameValofFour(sinkMinY, sinkY1, sinkY2, sinkY3, sinkY4);
			sinkMinZ = math::getSameValofFour(sinkMinZ, sinkZ1, sinkZ2, sinkZ3, sinkZ4);

			//---- 一番小さいめり込み量を見つけてそれにする
			if( minVal == abs(sinkMinX) ) {
				result = Vector3(sinkMinX,		  0,		0);
			}else{
				result = Vector3(       0,		  0, sinkMinZ);
			}

		}

		return result;

	}

}	// namespace AABBCollision

extern void	drawLineF(const Vector3& p1, const Vector3& p2, const Vector4& color);

extern void drawAABB(const AABB& aabb, const Vector4& color, const Vector3& offset = Vector3(0,0,0));


struct CollisionData;

//-----------------------------------------------------------------------------
//!	AABB-Tree
//-----------------------------------------------------------------------------
class AABBTree
{
public:
	struct Node {
		//! コンストラクタ
		Node()
		{
			_aabb.setEmpty();
			_pChild[0] = nullptr;
			_pChild[1] = nullptr;
		}

		//vector<Triangle> _triangles;				//!< 三角形の配列
		//vector<CollisionData> _colData;				//!< 当たり判定情報
		CollisionData*		  _col;					//!< 当たり判定情報
		AABB				  _aabb;				//!< すべてを囲むバウンディングボックス
		s32					  _dataCount;			//!< 最終ノード以外はデータを持たない

		std::unique_ptr<AABBTree::Node>	_pChild[2];	//!< 子ノード

		//! 三角形リストからノード情報を構築
		//! @param	[in]	colData	当たり判定情報の配列データ
		bool			build(vector<CollisionData> colData);


		// aabbと衝突チェック
		//!	@param	[in]	aabb	当たり判定を行うAABB
		//!	@param	[in]	hitFunc	三角形との衝突判定用の関数ポインタ
		bool			hitCheck(const AABB& aabb, bool(*hitFunc)(Triangle* tri));


		//! デバッグ描画
		//!	@param	[in]	nest	どの深さを描画するか
		void			debugDraw(u32 nest);
	};

	//! コンストラクタ
	AABBTree();

	//! デストラクタ
	virtual ~AABBTree();

	//! ツリー構築
	bool				build(vector<CollisionData>& colData);

	//! デバッグ用の準備
	bool				debugSetup();

	//! デバッグ描画
	void				debugRender();

private:

	AABBTree::Node		_rootNode;		//!< ルートノード
};


//===========================================================================
// END OF FILE
//===========================================================================
