//-----------------------------------------------------------------------------
//!
//!	@file	gmGeometry.cpp
//!	@brief	形状
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

namespace collision {

	//----------------------------------------------------------------------------
	//! 距離
	//! 平面 vs 点
	//! @param	[in]	plane		平面
	//! @param	[in]	position	点の位置
	//!	@return	最短距離(表側は正の値, 裏側は負の値)
	//----------------------------------------------------------------------------
	f32	getDistance(const Plane& plane, const Vector3& position)
	{
		// 距離は「 Ax + By + Cz + D 」で計算
		return plane.getDistance(position);
	}

	//----------------------------------------------------------------------------
	//! @param [in]		triangle		三角形
	//! @param [in]		line			直線
	//! @param [in]		pT				衝突した点と交差する直線のパラメーターtを受け取る場所
	//! @param [in]		pHitPosition	衝突点
	//! @param [in]		pHitNormal		衝突点の法線
	//----------------------------------------------------------------------------
	void		checkHitPos(const Triangle& triangle,
							const Line&	    line,
							Vector3*		pHitPosition,
							f32*			pT,
							Vector3*		pHitNormal	)
	{
		//------------------------------------------------------------
		// 平面との衝突判定と交差位置を計算
		//------------------------------------------------------------
		f32		t;
		bool	hit = collision::isHit( ( const Plane)triangle, line, &t);

		if( !hit ) return;

		//---- パラメーターtから座標を計算
		Vector3 hitPosition = line.getPosition(t);

		//------------------------------------------------------------
		// 三角形の点に含まれてなければ一番近い点を返す
		//------------------------------------------------------------
		hitPosition = triangle.getHitPos( hitPosition );
		
		//------------------------------------------------------------
		// 結果の保存出力
		//------------------------------------------------------------
		if( pT			 )  *pT			  = t;
		if( pHitPosition )  *pHitPosition = hitPosition;
		if( pHitNormal	 )  *pHitNormal	  = triangle.getNormal();
	}

	//----------------------------------------------------------------------------
	//! 衝突計算
	//! 三角形 vs 直線
	//! @param [in]		triangle		三角形
	//! @param [in]		line			直線
	//! @param [in]		pT				衝突した点と交差する直線のパラメーターtを受け取る場所
	//! @param [in]		pHitPosition	衝突点
	//! @param [in]		pHitNormal		衝突点の法線
	//----------------------------------------------------------------------------
	bool		isHit( const Triangle& triangle,
					   const Line&	   line,
					   Vector3*		   pHitPosition,
					   f32*			   pT,
					   Vector3*		   pHitNormal	)
	{
		//------------------------------------------------------------
		// 平面との衝突判定と交差位置を計算
		//------------------------------------------------------------
		f32		t;
		bool	hit = collision::isHit( ( const Plane)triangle, line, &t);

		if( !hit ) return false;

		//---- パラメーターtから座標を計算
		Vector3 hitPosition = line.getPosition(t);

		//------------------------------------------------------------
		// 三角形の中に衝突点が含まれるかどうかをチェック
		//------------------------------------------------------------
		hit = triangle.isHit( hitPosition );

		if( !hit ) {
			return false;
		}

		//------------------------------------------------------------
		// 結果の保存出力
		//------------------------------------------------------------
		if( pT			 )  *pT			  = t;
		if( pHitPosition )  *pHitPosition = hitPosition;
		if( pHitNormal	 )  *pHitNormal	  = triangle.getNormal();

		return true;
	}

	//----------------------------------------------------------------------------
	//! 衝突計算
	//! 平面 vs 直線
	//! @param [in]		plane			平面
	//! @param [in]		line			直線
	//! @param [in]		pT				衝突した点と交差する直線のパラメーターtを受け取る場所
	//----------------------------------------------------------------------------
	bool		isHit( const Plane& plane, const Line& line, f32* pT)
	{

		Vector3	S = line._position[0];	//!< 始点
		Vector3	E = line._position[1];	//!< 終点

		//-------------------------------------------------------------
		// 平面の方程式から直線の始点Sと終点Eの高さ (HS, HE)を計算
		//-------------------------------------------------------------
		//
		// - - -◎S
		// ↑     ＼
		//          ＼
		// HS         ＼
		// ↓           ＼
		// ---------------●----------
		// ↑               ＼ 
		// ↓HE               ＼
		// - - - - - - - - - - -◎E
		f32	HS = collision::getDistance(plane, S);		//!< 始点から平面までの距離
		f32	HE = collision::getDistance(plane, E);		//!< 終点から平面までの距離

		// 始点終点が同じなら終了
		if( HS == HE ) return false;

		//---- HS,HE 2つの比率からパラメーターtを計算
		// - - -◎S
		// ↑ 　 |＼
		//       |  ＼
		// HS    |    ＼
		// ↓    |      ＼
		// ------+--------●-------------------
		// ↑    |          ＼ 
		// ↓HE  |            ＼
		// - - - + - - - - - - -◎E
		// パラメータtの計算 (0≦t≦1)
		// t = 
		// 線形補間のためHSの割合を求める
		f32	t = HS / (HS - HE);

		//---- 情報を返す
		if( pT ) *pT = t;

		return true;
	}

}	// namespace collision

//=============================================================================
// トライアングル(三角形)の実装
//=============================================================================
//-----------------------------------------------------------------------------
//! デバッグ用描画
//-----------------------------------------------------------------------------
void Triangle::debugRender(Color& col)
{
	//----
	// ポリゴンオフセット機能によってポリゴンを少し奥へずらす
	// (ラインとのZファイト防止)
	glPolygonOffset(-5.0f, 1.0f);
	glEnable(GL_POLYGON_OFFSET_FILL);

	// カラー
	glColor4ubv((GLubyte*)&col);

	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, (GLfloat*)&col);

	glBegin(GL_TRIANGLES);
		glVertex3fv((GLfloat*)&_position[0]);
		glVertex3fv((GLfloat*)&_position[1]);
		glVertex3fv((GLfloat*)&_position[2]);
	glEnd();


	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	Vector3 center = getCenter();
	glBegin(GL_LINES);
		glVertex3fv((GLfloat*)&center);
		glVertex3fv((GLfloat*)&(center + _normal * 10) );
	glEnd();

	glDisable(GL_POLYGON_OFFSET_FILL);
}

//=============================================================================
// ライン(直線)の実装
//=============================================================================
//-----------------------------------------------------------------------------
//! デバッグ用描画
//-----------------------------------------------------------------------------
void Line::DebugRender()
{
	glPushMatrix();
		glBegin(GL_LINES);
		glVertex3fv((GLfloat*)&_position[0]);
		glVertex3fv((GLfloat*)&_position[1]);
		glEnd();
	glPopMatrix();
}



//=============================================================================
// 球の実装
//=============================================================================

//-----------------------------------------------------------------------------
//! 衝突計算
//! 円同士
//! @param	[in]	sphere 当たり判定のしたい球
//-----------------------------------------------------------------------------
bool Sphere::isHit(Sphere& sphere)
{
	return GmSystemCollision()->HitSphere(_position, sphere.getPosition(), _radius, sphere.getRadius());
}

//-----------------------------------------------------------------------------
//! デバッグ表示
//-----------------------------------------------------------------------------
void Sphere::DebugRender()
{
	glPushMatrix();
		glTranslatef( _position._x, _position._y, _position._z );
		setBlendMode(BM_ADD_ALPHA);		//================================ 【半透明処理開始】
		glutSolidSphere(_radius,16,16);
		setBlendMode(BM_NONE);			//================================ 【半透明処理終了】
	glPopMatrix();
}

//=============================================================================
// 円の実装
//=============================================================================
//-----------------------------------------------------------------------------
//! 衝突計算
//! 円同士
//! @param	[in]	circle 当たり判定のしたい円
//-----------------------------------------------------------------------------
bool Circle::isHit(Circle& circle)
{
	return GmSystemCollision()->HitCircle(_position, circle.getPosition(), _radius, circle.getRadius());
}
	
//-----------------------------------------------------------------------------
//! デバッグ表示
//-----------------------------------------------------------------------------
void Circle::DebugRender()
{
	Degree rot1, rot2;

	f32 x,y;
	x = _position._x;
	y = _position._z;

	for( rot1._degree = 0.0f; rot1._degree <= 360.0f; rot1._degree += 1.0f )
	{
		rot2._degree = rot1._degree + 10.0f;
		Radian rot1Rad(rot1);
		Radian rot2Rad(rot2);

		f32 x1 = _radius * cosf(rot1Rad);
		f32 y1 = _radius * sinf(rot1Rad);
		f32 x2 = _radius * cosf(rot2Rad);
		f32 y2 = _radius * sinf(rot2Rad);

		glBegin(GL_TRIANGLES);
			glVertex2f(x,y);
			glVertex2f(x1+x, y1+y);
			glVertex2f(x2+x, y2+y);
		glEnd();

	}
}

//=============================================================================
// 視錐台の実装
//=============================================================================



//-----------------------------------------------------------------------------
//! 視錐台の8点の更新
//! @param	[in]	center		中心座標
//! @param	[in]	matrix		元となる姿勢行列
//-----------------------------------------------------------------------------
void Frustum::updateFrustumPoints(Vector3& center, Matrix& matrix)
{

	Vector3 up		=  Vector3(matrix._m[1]);		// 元となるビューの上
	Vector3 right	=  Vector3(matrix._m[0]);		// 元となるビューの右
	Vector3 viewDir = -Vector3(matrix._m[2]);		// 元となるビューの前(視線方向:右手座標なのでマイナス)
	
	// 遠クリップ面
	Vector3 fc = center + viewDir * _fard;
	// 近クリップ面
	Vector3 nc = center + viewDir * _neard;
	
	// 近、遠クリップ面の幅高さ計算
	f32 near_height = tan(_fov/2.0f) * _neard;
	f32 near_width  = near_height	 * _ratio;
	f32 far_height  = tan(_fov/2.0f) * _fard;
	f32 far_width   = far_height	 * _ratio;

	// 座標の更新
	// p[1]/[5]+-------------+p[2]/[6]
	//		   |			 |
	//		   |	nc/fc	 |
	//		   |			 |
	//		   |			 |
	// p[0]/[4]+-------------+p[3]/[7]

	// 近クリップ
	_point[0] = nc - up*near_height - right*near_width;
	_point[1] = nc + up*near_height - right*near_width;
	_point[2] = nc + up*near_height + right*near_width;
	_point[3] = nc - up*near_height + right*near_width;

	// 遠クリップ
	_point[4] = fc - up*far_height - right*far_width;
	_point[5] = fc + up*far_height - right*far_width;
	_point[6] = fc + up*far_height + right*far_width;
	_point[7] = fc - up*far_height + right*far_width;

	_position = center;
}

//-----------------------------------------------------------------------------
//! 視錐台の6面の更新
//-----------------------------------------------------------------------------
void Frustum::updateFrustumPlane()
{
	// 6面の更新
	// 近面
	_plane[0] = Plane(_point[0], _point[1], _point[2]);
	// 遠面
	_plane[1] = Plane(_point[6], _point[5], _point[4]);
	// 上面
	_plane[2] = Plane(_position, _point[5], _point[6]); 
	// 下面
	_plane[3] = Plane(_position, _point[7], _point[4]);
	// 右面
	_plane[4] = Plane(_position, _point[6], _point[7]);
	// 左面
	_plane[5] = Plane(_position, _point[4], _point[5]);
}

//-----------------------------------------------------------------------------
//! デバッグ描画
//-----------------------------------------------------------------------------
void Frustum::debugRender(Color& col) const
{
	for( s32 i=0; i<8-1; ++i ) { 
		drawLineF(_point[i], _point[i+1], col);
	}
}

//-----------------------------------------------------------------------------
//! 衝突計算
//! 視錐台 vs 球
//! @param	[in]	sphere 当たり判定のしたい球
//-----------------------------------------------------------------------------
bool Frustum::isHit(Sphere& sphere)
{
	// 視錐台の面の更新
	updateFrustumPlane();
	// 球の座標
	Vector3 spherePos = sphere.getPosition();
	// 球の半径
	f32		sphereRad = sphere.getRadius();

	// 面との距離がすべての面の内側にあればあたっている
	for (s32 i = 0; i < 6; ++i){
		f32 distance = _plane[i].getDistance(spherePos);

		// 裏面の場合距離はマイナスなので半径を足しても
		// 負の場合視錐台の外側で且つ半径以上離れている
		if (distance + sphereRad < 0.0f){
			return false;
		}
	}

	return true;
}




//=============================================================================
//	END OF FILE
//=============================================================================