//-----------------------------------------------------------------------------
//!
//!	@file	gmGeometry.h
//!	@brief	形状
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
//!	平面	Plane / Surface
//-----------------------------------------------------------------------------
struct Plane
{
public:

	//-------------------------------------------------------------
	//!	@name 初期化
	//-------------------------------------------------------------
	//@{

	//!	デフォルトコンストラクタ
	Plane() {}

	//! コンストラクタ(3点指定)
	//!	@param [in]		p1	平面上の点1
	//!	@param [in]		p2	平面上の点2
	//!	@param [in]		p3	平面上の点3
	Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3)
	{
		//        ／＼(p1)
		//      ／b   ＼
		//(p2)／        ＼(p3)
		//  +--------------+
		//         a

		// 法線_normal(A,B,C)を求める
		Vector3	a = p3 - p2;
		Vector3	b = p1 - p2;
		// 外積を使って平面に垂直な線(法線)を求める
		_normal = Vector3::cross(a, b).normalize();

		// 現在位置から原点までの距離Dを求める
		// 平面の方程式
		// 法線と平面上の点の内積+距離は0
		// Ax + By + Cz + D = 0
		//				  D = -(Ax + By + Cz)
		_distance = - Vector3::dot(_normal, p1);

	}

	//!	コンストラクタ(平面パラメータ指定)
	//! @param	[in]	normal		面の法線
	//!	@param	[in]	distance	原点までの距離
	Plane(const Vector3& normal, f32 distance)
	: _normal	(normal)
	, _distance	(distance)
	{
	}

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//!	法線の取得
	Vector3		getNormal() const { return _normal; }

	//! 垂直な線と交差する場所のY座標を取得
	f32			getHeight(f32 x, f32 z)
	{
		// Ax + By + Cz + D = 0
		//		By			= -(Ax + Cz + D)
		//		y			= -(Ax + Cz + D)/B

		f32	A = _normal._x;
		f32	B = _normal._y;
		f32	C = _normal._z;
		f32	D = _distance;

		return -(A*x + C*z + D) / B;
	}

	//! 接触点を取得
	Vector3			getHitPos(Vector3 pos)
	{

		Vector3 result;

		// Ax + By + Cz + D = 0
		//		By			= -(Ax + Cz + D)
		//		y			= -(Ax + Cz + D)/B
		f32 x = pos._x;
		f32 y = pos._y;
		f32 z = pos._z;
		f32	A = _normal._x;
		f32	B = _normal._y;
		f32	C = _normal._z;
		f32	D = _distance;

		result._x = -(B*y + C*z + D) / A;
		result._y = -(A*x + C*z + D) / B;
		result._z = -(A*x + B*y + D) / C;

		return result;
	}

	//----------------------------------------------------------------------------
	//! 距離
	//! 平面 vs 点
	//! @param	[in]	position	点の位置
	//!	@return	最短距離(表側は正の値, 裏側は負の値)
	//----------------------------------------------------------------------------
	f32	getDistance(const Vector3& position) const
	{
		// 距離は「 Ax + By + Cz + D 」で計算
		return Vector3::dot(_normal, position) + _distance;
	}
	
	//@}
public:
	// 【平面の方程式 equation】Ax + By + Cz + D = 0
	Vector3		_normal;		// 面の法線		  (ABC)
	f32			_distance;		// 原点までの距離 (D)
	//bool		_hit;			// 当たってるかどうか
};


//-----------------------------------------------------------------------------
//!	三角形	Triangle
//!	@note	三角形は無限平面の三方向を制限したもの
//-----------------------------------------------------------------------------
struct	Triangle : public Plane
{
public:

	//-------------------------------------------------------------
	//!	@name 初期化
	//-------------------------------------------------------------
	//@{

	//!	デフォルトコンストラクタ
	Triangle() {}

	//! コンストラクタ(3点指定)
	//!	@param [in]		p1	平面上の点1
	//!	@param [in]		p2	平面上の点2
	//!	@param [in]		p3	平面上の点3
	Triangle(const Vector3& p1, const Vector3& p2, const Vector3& p3)
	: Plane(p1, p2, p3)		//	親クラス
	{
		_position[0] = p1;
		_position[1] = p2;
		_position[2] = p3;
	}

	//@}
	//-------------------------------------------------------------
	//!	@name タスク関数
	//-------------------------------------------------------------
	//@{

	//! デバッグ描画
	void			debugRender(Color& col);

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//!	指定した座標の点が三角形に含まれるかどうかをテスト
	bool		isHit(const Vector3& p) const
	{
		// 点A,B,Cからそれぞれ点Pに向かうベクトルを計算
		//          A／|＼				
		//         ／  |  ＼				A→P
		//       ／    |    ＼				B→P
		//     ／   _ -●-__  ＼			C→P
		// B ／__---   P    ---_＼ C
		// +-----------------------+
		//
		static const s32		nextIndex[3] = {1, 2, 0};
		for( s32 i=0; i<3; i++ ) {
			Vector3 dirEdge = _position[ nextIndex[i] ] - _position[i]; // 外周方向を反時計回り
			Vector3	dirToP	= p - _position[i];							// Pに向かうベクトル


			Vector3 n = Vector3::cross(dirEdge, dirToP);				// 回転方向を表す法線

			//---- 法線の裏表判定 (面の法線と同じ方向を向いているかどうかを判定)
			if( Vector3::dot(n, getNormal()) < 0.0f ) return false;
		}
		return true;
	}

	//! 指定した座標から一番近い三角形の点を返す
	Vector3		getHitPos(const Vector3& p) const
	{
		Vector3 result = p;
		// 点A,B,Cからそれぞれ点Pに向かうベクトルを計算
		//          A／|＼				
		//         ／  |  ＼				A→P
		//       ／    |    ＼				B→P
		//     ／   _ -●-__  ＼			C→P
		// B ／__---   P    ---_＼ C
		// +-----------------------+
		//
		static const s32		nextIndex[3] = {1, 2, 0};
		for( s32 i=0; i<3; i++ ) {
			Vector3 dirEdge = _position[ nextIndex[i] ] - _position[i]; // 外周方向を反時計回り
			Vector3	dirToP	= p - _position[i];							// Pに向かうベクトル


			Vector3 n = Vector3::cross(dirEdge, dirToP);				// 回転方向を表す法線

			//---- 法線の裏表判定 (面の法線と同じ方向を向いているかどうかを判定)
			if( Vector3::dot(n, getNormal()) < 0.0f ){
				Vector3 tmp = dirEdge;
				// 正規化
				dirEdge = dirEdge.normalize();
				//dirToP	= dirToP.normalize();
				// 入ってなかったら三角形に近い点までの距離を取得
				f32 length = Vector3::dot(dirEdge, dirToP);

				f32	persent = length / tmp.length();

				// 1.0f以上なら端の点を返す
				if( persent > 1.0f ){
					result = _position[ nextIndex[i] ];
					return result;
				}else if( persent < 0.0f )
				{
					result = _position[i];
					return result;
				}
				// その長さに直して渡す
				result = dirEdge * length;
				result = _position[i] + result;
				return result;
			}
		}
		return result;
	}

	//! 重心を取得
	Vector3		getCenter() const
	{
		Vector3	center;

		center._x = (_position[0]._x + _position[1]._x + _position[2]._x) / 3.0f;
		center._y = (_position[0]._y + _position[1]._y + _position[2]._y) / 3.0f;
		center._z = (_position[0]._z + _position[1]._z + _position[2]._z) / 3.0f;

		return center;
	}

	//@}
	
public:
	Vector3		_position[3];		//!< 3点の位置座標
};


//-----------------------------------------------------------------------------
//!	直線	Line
//-----------------------------------------------------------------------------
struct Line
{
public:
	//---- 直線の種類
	enum TYPE {
		TYPE_LINE,		//!< ライン	(Line)
		TYPE_RAY,		//!< 光線	(Ray)
		TYPE_SEGMENT	//!< 線分	(Line Segment)
	};

	//-------------------------------------------------------------
	//!	@name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	Line()
	{
		_type = Line::TYPE_LINE;
	}

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! パラメータtで指定する位置の座標を計算
	Vector3 getPosition(f32 t) const
	{
		// P' = (E - S) * t + S;
		return (_position[1] - _position[0]) * t + _position[0];
	}

	//@}
	//-------------------------------------------------------------
	//!	@name タスク関数
	//-------------------------------------------------------------
	//@{

	//! デバッグ用描画
	void	DebugRender();

	//@}
public:
	Vector3		_position[2];			//!< 始点・終点の２点
	Line::TYPE	_type;
};

//-----------------------------------------------------------------------------
//!	光線	Ray
//-----------------------------------------------------------------------------
struct Ray : public Line
{
public:

	//-------------------------------------------------------------
	//!	@name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	Ray()
	{
		_type = Line::TYPE_RAY;
	}

	//@}
public:
};

//-----------------------------------------------------------------------------
//!	線分	Line Segment
//-----------------------------------------------------------------------------
struct LineSegment : public Line
{
public:

	//-------------------------------------------------------------
	//!	@name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	LineSegment()
	{
		_type = Line::TYPE_SEGMENT;
	}

	//@}
public:
};


namespace collision {


	//! @param [in]		triangle		三角形
	//! @param [in]		line			直線
	//! @param [in]		pT				衝突した点と交差する直線のパラメーターtを受け取る場所
	//! @param [in]		pHitPosition	衝突点
	//! @param [in]		pHitNormal		衝突点の法線
	void		checkHitPos(const Triangle& triangle,
							const Line&	    line,
							Vector3*		pHitPosition = NULL,
							f32*			pT			 = NULL,
							Vector3*		pHitNormal	 = NULL	);

	//! 衝突計算
	//! 三角形 vs 直線
	//! @param [in]		triangle		三角形
	//! @param [in]		line			直線
	//! @param [in]		pT				衝突した点と交差する直線のパラメーターtを受け取る場所
	//! @param [in]		pHitPosition	衝突点
	//! @param [in]		pHitNormal		衝突点の法線
	bool		isHit( const Triangle& triangle,
					   const Line&	   line,
					   Vector3*		   pHitPosition = NULL,
					   f32*			   pT			= NULL,
					   Vector3*		   pHitNormal	= NULL );

	//! 衝突計算
	//! 平面 vs 直線
	//! @param [in]		plane			平面
	//! @param [in]		line			直線
	//! @param [in]		pT				衝突した点と交差する直線のパラメーターtを受け取る場所
	bool		isHit( const Plane& plane, const Line& line, f32* pT);

};



//-----------------------------------------------------------------------------
//!	球	Sphere
//-----------------------------------------------------------------------------
struct Sphere 
{
public:

	//-------------------------------------------------------------
	//!	@name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	Sphere(){}
	//! コンストラクタ
	Sphere(Vector3	position, f32	radius)
	: _position(position)
	, _radius(radius)
	{
	}

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! デバッグ表示
	void		DebugRender();

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//---- 設定
	//! 座標設定
	void		setPosition(Vector3& pos)
	{
		_position = pos;
	}
	//! 半径設定
	void		setRadius(f32 radius)
	{
		_radius = radius;
	}

	//---- 取得
	//! 座標取得
	Vector3		getPosition() { return _position; }
	//! 半径取得
	f32			getRadius()	  { return _radius;	  }
	
	//! 衝突計算
	//! 球同士
	//! @param	[in]	sphere 当たり判定のしたい球
	bool		isHit(Sphere& sphere);

	//@}
private:
	Vector3		_position;	//!< 座標
	f32			_radius;	//!< 半径
};

//-----------------------------------------------------------------------------
//!	円	Circle
//-----------------------------------------------------------------------------
struct Circle
{
public:

	//-------------------------------------------------------------
	//!	@name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	Circle(){}
	//! コンストラクタ
	Circle(Vector3 position, f32 radius)
	: _position(position)
	, _radius(radius)
	{
	}

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{
	
	//! デバッグ表示
	void		DebugRender();

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//---- 設定
	//! 座標設定
	void		setPosition(Vector3& pos)
	{
		_position = pos;
	}
	//! 半径設定
	void		setRadius(f32 radius)
	{
		_radius = radius;
	}

	//---- 取得
	//! 座標取得
	Vector3		getPosition() { return _position; }
	//! 半径取得
	f32			getRadius()	  { return _radius;	  }
	
	//! 衝突計算
	//! 円同士
	//! @param	[in]	circle 当たり判定のしたい円
	bool		isHit(Circle& circle);
	
	//@}
private:
	Vector3		_position;		//!< 座標
	f32			_radius;		//!< 半径
};

//-----------------------------------------------------------------------------
//! 視錐台
//-----------------------------------------------------------------------------
struct Frustum
{
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	Frustum()
	: _neard(0.0f)
	, _fard (0.0f)
	, _fov  ( Radian( Degree(85.0f) ) )
	, _ratio( GmWindow()->getWindowRatio() )
	{
		// 座標の初期化
		for(s32 i=0; i<8; ++i){
			_point[i] = Vector3(0.0f);
		}
	}
	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 視錐台の8点の更新
	//! @param	[in]	center		中心座標
	//! @param	[in]	matrix		元となる姿勢行列
	void		updateFrustumPoints(Vector3& center, Matrix& matrix);

	//! 視錐台の6面の更新
	void		updateFrustumPlane();

	//! デバッグ描画
	void		debugRender(Color& col) const;

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! 衝突計算
	//! 球同士
	//! @param	[in]	sphere 当たり判定のしたい球
	bool		isHit(Sphere& sphere);

	//! 情報設定
	//! @param	[in]	neard	z近距離
	//! @param	[in]	fard	z遠距離
	//!	@param	[in]	fov		画角(Degree)
	//!	@param	[in]	aspect	アスペクト比
	void		setStatus(f32 neard, f32 fard, Degree fov, f32 aspect)
	{
		_neard	= neard;
		_fard	= fard;
		_fov	= Radian(fov);
		_ratio	= aspect;
	}

	//@}

	//---- メンバー変数
	f32		_neard;
	f32		_fard;
	f32		_fov;
	f32		_ratio;
	Vector3 _point[8];

	Vector3 _position;
	Plane	_plane[6];
};


//=============================================================================
//	END OF FILE
//=============================================================================