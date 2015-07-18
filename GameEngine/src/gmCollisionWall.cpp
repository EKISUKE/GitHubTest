//-----------------------------------------------------------------------------
//!
//!	@file	gmCollision.cpp
//!	@brief	衝突判定
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

// 分割数
#define WALL_DIVDE_NUM 20

//=============================================================================
// CollisionWallの実装
//=============================================================================
//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
CollisionWall::CollisionWall()
{
	_LastHitTriangle = NULL;
	//SAFE_NEW(_LastHitTriangle);
	// 分割数
	_DivCount = WALL_DIVDE_NUM;
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
CollisionWall::~CollisionWall()
{
	// 仮でやっていない(pTriangleのポインタが消えた時点でもう一回デリートはしていけないため)
	//SAFE_DELETE(_LastHitTriangle);
	for( vecCol::iterator itr = _objects.begin(); itr != _objects.end();){
		itr = _objects.erase(itr);
	}
}

//-----------------------------------------------------------------------------
//! Xファイルからポリゴンデーターを入力
//! @param	[in]	pModel		Xファイル
//-----------------------------------------------------------------------------
bool CollisionWall::load(TaskModelX*	pModel)
{
	// 最大値
	_max._x = -FLT_MAX;
	_min._x =  FLT_MAX;
	_max._z = -FLT_MAX;
	_min._z =  FLT_MAX;

	// オブジェクトの取得
	//SystemCollision::Object obj = loadXfile(pModel->_frames.at(0), _objects, _max, _min, true, scale);
	SystemCollision::Object obj = GmSystemCollision()->loadXfile(pModel, _objects, _max, _min, true);

//	_aabbTree.build(obj.getColDataVec());

	
	//---- グリッド化
	// マイナスのセンター座標のオフセット値
	_offsetX = (s32)_min._x * -1;
	_offsetZ = (s32)_min._z * -1;
	// すこし大きめにして範囲外をなくす
	_max._x += 1;
	_max._z += 1;
	_min._x -= 1;
	_min._z -= 1;
	// 最大値最小値からインデックス番号を求める
	_width  = _max._x - _min._x;
	_height = _max._z - _min._z;
	// ceilは小数点以下切り上げ
	_divX   = (s32)( ceil( (_width ) / (_DivCount) ) );
	_divZ   = (s32)( ceil( (_height) / (_DivCount) ) );
	// メモリを確保
	_objects.resize(_DivCount * _DivCount);

	//---- オブジェクトを追加登録
	for( s32 i=0; i<obj.getColDataCount(); i++ )
	{
		CollisionData* col = obj.getColData(i);
		Triangle* pTriangle = &col->_triangle;
		// 重心を取得
		Vector3	center = pTriangle->getCenter();
		// 三角形の三点をしらべる
		Vector3	position[3] = {
			pTriangle->_position[0],
			pTriangle->_position[1],
			pTriangle->_position[2],
		};

		// 最大最少
		s32 maxIndexX = -INT_MAX;
		s32 maxIndexZ = -INT_MAX;
		s32 minIndexX =  INT_MAX;
		s32 minIndexZ =  INT_MAX;

		// インデックスの最小最大をしらべる
		for( s32 t=0; t<3; ++t )
		{
			s32 x = ( (s32)(position[t]._x) + _offsetX ) / _divX;
			s32 z = ( (s32)(position[t]._z) + _offsetZ ) / _divZ;
			// 最大より大きければ
			if( maxIndexX < x )
			{
				// 最大値更新
				maxIndexX = x;
			}
			// 最大より大きければ
			if( maxIndexZ < z )
			{
				// 最大値更新
				maxIndexZ = z;
			}
			// 最小より小さければ
			if( minIndexX > x )
			{
				// 最小値更新
				minIndexX = x;
			}
			// 最小より小さければ
			if( minIndexZ > z )
			{
				// 最小値更新
				minIndexZ = z;
			}
		}
		
		// またいでいるグリッドすべてに登録
		for( s32 x=minIndexX; x<=maxIndexX; x++ )
		{
			for( s32 z=minIndexZ; z<=maxIndexZ; z++ )
			{
				_objects.at( z * _DivCount +x ).addColData(*col);
			}
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
//! フレームからポリゴンデータ入力
//! @param	[in]	pFrame		フレーム
//!	@param	[in]	parentModel	フレームの属すタスクモデル
//-----------------------------------------------------------------------------
bool CollisionWall::loadWithFrame(AssetModelX::Frame* pFrame, TaskModelX* parentModel)
{
	// 最大値
	_max._x = -FLT_MAX;
	_min._x =  FLT_MAX;
	_max._z = -FLT_MAX;
	_min._z =  FLT_MAX;

	// オブジェクトの取得
	//SystemCollision::Object obj = loadXfile(pModel->_frames.at(0), _objects, _max, _min, true, scale);
	// オブジェクトの取得
	SystemCollision::Object obj = GmSystemCollision()->loadWithFrame(pFrame, _objects, _max, _min, true, parentModel);

//	_aabbTree.build(obj.getColDataVec());

	
	//---- グリッド化
	// マイナスのセンター座標のオフセット値
	_offsetX = (s32)_min._x * -1;
	_offsetZ = (s32)_min._z * -1;
	// すこし大きめにして範囲外をなくす
	_max._x += 1;
	_max._z += 1;
	_min._x -= 1;
	_min._z -= 1;
	// 最大値最小値からインデックス番号を求める
	_width  = _max._x - _min._x;
	_height = _max._z - _min._z;
	// ceilは小数点以下切り上げ
	_divX   = (s32)( ceil( (_width ) / (_DivCount) ) );
	_divZ   = (s32)( ceil( (_height) / (_DivCount) ) );
	// メモリを確保
	_objects.resize(_DivCount * _DivCount);

	//---- オブジェクトを追加登録
	for( s32 i=0; i<obj.getColDataCount(); i++ )
	{
		CollisionData* col = obj.getColData(i);
		Triangle* pTriangle = &col->_triangle;
		// 重心を取得
		Vector3	center = pTriangle->getCenter();
		// 三角形の三点をしらべる
		Vector3	position[3] = {
			pTriangle->_position[0],
			pTriangle->_position[1],
			pTriangle->_position[2],
		};

		// 最大最少
		s32 maxIndexX = -INT_MAX;
		s32 maxIndexZ = -INT_MAX;
		s32 minIndexX =  INT_MAX;
		s32 minIndexZ =  INT_MAX;

		// インデックスの最小最大をしらべる
		for( s32 t=0; t<3; ++t )
		{
			s32 x = ( (s32)(position[t]._x) + _offsetX ) / _divX;
			s32 z = ( (s32)(position[t]._z) + _offsetZ ) / _divZ;
			// 最大より大きければ
			if( maxIndexX < x )
			{
				// 最大値更新
				maxIndexX = x;
			}
			// 最大より大きければ
			if( maxIndexZ < z )
			{
				// 最大値更新
				maxIndexZ = z;
			}
			// 最小より小さければ
			if( minIndexX > x )
			{
				// 最小値更新
				minIndexX = x;
			}
			// 最小より小さければ
			if( minIndexZ > z )
			{
				// 最小値更新
				minIndexZ = z;
			}
		}
		
		// またいでいるグリッドすべてに登録
		for( s32 x=minIndexX; x<=maxIndexX; x++ )
		{
			for( s32 z=minIndexZ; z<=maxIndexZ; z++ )
			{
				_objects.at( z * _DivCount +x ).addColData(*col);
			}
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
//! その座標から一番近い三角形との衝突点の取得
//!	@param	[in]	positon		チェックする位置
//!	@param	[in]	radius		半径
//!	@retval			衝突点
//-----------------------------------------------------------------------------
Vector3 CollisionWall::getHitPos(const Vector3& position, const f32 radius, bool isNear)
//Vector3 CollisionWall::getHitPos(const AABB aabb)
{
	// 接触点
	Vector3	 resultPos(0.0f,0.0f,0.0f);

	/*Vector3 v1 = aabb._max;
	v1._y = 0.0f;
	Vector3 v2 = aabb._min;
	v2._y = 0.0f;*/

	//f32 radius = abs((v1 - v2).length() * 0.5f);

	// 衝突点までの距離(二乗で比較)
	f32	nearestHitPosLength = radius * radius;
	//f32	nearestHitPosLength = FLT_MAX;
	// 衝突したかどうか
	static bool isHit	  = false;

	// オフセット値
	static s32 Count = 0;
	static Vector3 offsetList[5] =
	{
		Vector3(  0, 0,  0 ),	// 中央
		Vector3(  0, 0,  1 ),	// 前
		Vector3(  0, 0, -1 ),	// 後
		Vector3( -1, 0,  0 ),	// 左
		Vector3(  1, 0,  0 ),	// 右
	};


	// もらった座標をインデックス番号に変更
	s32 x = ( (s32)(position._x) + _offsetX ) / _divX;
	s32 y = ( (s32)(position._z) + _offsetZ ) / _divZ;


_checkIndex._x = (f32)( x );
_checkIndex._y = (f32)( y );

	//// 前回衝突がなくカウントが更新されてたら
	//if( !isHit )
	//{
	//	// 最大数をこえてたら処理終了
	//	if( Count >= 5 ) {
	//		// リセット
	//		Count = 0;
	//		return resultPos;
	//	}
	//	// オフセット分ずらす
	//	x += offsetList[Count]._x;
	//	y += offsetList[Count]._z;
	//	// つぎへ
	//	Count++;
	//}

	// 範囲外なら処理しない
	if( x >= _DivCount || y >= _DivCount || x < 0 || y < 0 ) return resultPos;
	

	static f32 prevMinLength = -1;

	// その番号のオブジェクトを調べる
	SystemCollision::Object* pObject = getObject(y * _DivCount + x);

	
	// 初期化
	isHit = false;

	// そのオブジェクトの当たり判定情報を調べる
	//for( s32 t=0; t<pObject->getTriangleCount(); t++ )
	for( s32 t=0; t<pObject->getColDataCount(); t++ )
	{
		//Triangle*	pTriangle = pObject->getTriangle(t);
		Triangle*	pTriangle = &pObject->getColData(t)->_triangle;
		// 三角形の法線取得
		Vector3 triNormal = pTriangle->getNormal();
		// この法線の逆ベクトルをポジションから伸ばす
		// 法線を正規化
		triNormal = triNormal.normalize();
		// 半径に伸ばす
		triNormal *= radius;

		// 当たり判定用ラインの作成
		Line	line;
		line._position[0] = position;
		line._position[1] = position + ( -triNormal );

				
		Vector3	hitPos;
		f32		h;

		// ラインが伸びてなかったら処理しない
		if( line._position[0] == line._position[1] ) continue;

		if( isNear ){
			// 三角形と当たってたら高さ記録
			collision::checkHitPos( *pTriangle, line, &hitPos );
			h = hitPos._y;
		}else{
			// 三角形と当たってたら高さ記録
			if( collision::isHit( *pTriangle, line, &hitPos ) )
			{
				h = hitPos._y;
			}else{
				continue;
			}
		}

		//-----------------------------------------------------------
		// 衝突あり
		//-----------------------------------------------------------
		
		// 頭上の高さ上限チェック
		if( h > position._y + radius * 2.0f ) {
			continue;
		}

		f32 checkLength = (position - hitPos).squareLength();
	
		if( prevMinLength == checkLength )
		{
			// 三角形と当たってたら高さ記録
			collision::checkHitPos( *pTriangle, line, &hitPos );
		}


		// 遠い点なら処理終了
		if(  checkLength >= nearestHitPosLength  )
		{
			continue;
		}

		// 接触点記録
		resultPos	  = hitPos;
		// 一番近い点までの距離を記録
		nearestHitPosLength = checkLength;
		// 当たった三角形の保存
		_LastHitTriangle = pTriangle;
		// 衝突済み
		isHit		  = true;
	}
	prevMinLength = nearestHitPosLength;
	// この時点で見つかったら処理終了
	if( isHit )return resultPos;
	//// 最大までいってなかったら
	////if( Count < 4 )
	//{
	//	resultPos = getHitPos(position, radius);
	//}
	// カウント初期化
	Count = 0;
	return resultPos;
}

//-----------------------------------------------------------------------------
//! デバッグ描画
//-----------------------------------------------------------------------------
void CollisionWall::DebugRender()
{
	static s32 IndexX = 0;
	static s32 IndexY = 0;

	//----------------------------------------------------
	// ツール
	//----------------------------------------------------
	enum {
		//b31                                       b0
		// +-------------------------+---+---+---+---+
		// |                         | R | L | D | U |
		// +-------------------------+---+---+---+---+
		KEY_U = (1 << 0),		// 1 : 0001
		KEY_D = (1 << 1),		// 2 : 0010
		KEY_L = (1 << 2),		// 4 : 0100
		KEY_R = (1 << 3),		// 8 : 1000 
	};
	
	u32	key = 0;
	key |= (GetKeyState(VK_UP   ) & 0x80) ? KEY_U : 0;
	key |= (GetKeyState(VK_DOWN ) & 0x80) ? KEY_D : 0;
	key |= (GetKeyState(VK_LEFT ) & 0x80) ? KEY_L : 0;
	key |= (GetKeyState(VK_RIGHT) & 0x80) ? KEY_R : 0;

	static u32	oldKey = 0;
	u32			trigger = (key ^ oldKey) & key;

	oldKey = key;	// 次のために保存


	if( trigger & KEY_L ) {
		IndexX = max(IndexX - 1, 0);
	}
	if( trigger & KEY_R ) {
		IndexX = min(IndexX + 1, _DivCount-1);
	}
	if( trigger & KEY_U ) {
		IndexY = max(IndexY - 1, 0);
	}
	if( trigger & KEY_D ) {
		IndexY = min(IndexY + 1, _DivCount-1);
	}

	IndexX = (s32)( _checkIndex._x );
	IndexY = (s32)( _checkIndex._y );

	// グリッドの描画
	{
		f32 height = 1.0f;
		Line debugLine;
		for( s32 y=0; y<_DivCount; y++ )
		{
			debugLine._position[0] = Vector3( _min._x, height, _min._z + _divZ * (f32)(y) );
			debugLine._position[1] = Vector3( _max._x, height, _min._z + _divZ * (f32)(y) );
			debugLine.DebugRender();
		}
		for( s32 x=0; x<_DivCount; x++ )
		{
			debugLine._position[0] = Vector3( _min._x + _divX * (f32)(x), height, _min._z );
			debugLine._position[1] = Vector3( _min._x + _divX * (f32)(x), height, _max._z );
			debugLine.DebugRender();
		}
	}

	SystemCollision::Object drawObject;

	//// 範囲外なら処理しない
	//if( IndexX >= _DivCount || IndexY >= _DivCount || IndexX < 0 || IndexY < 0 ) return;

	//drawObject = _objects.at(IndexY * _DivCount + IndexX);

	//// 描画
	////for( s32 i=0; i<drawObject.getTriangleCount(); i++ )
	//for( s32 i=0; i<drawObject.getColDataCount(); i++ )
	//{
	//	//Triangle* drawTri = drawObject.getTriangle(i);
	//	Triangle* drawTri = &drawObject.getColData(i)->_triangle;
	//	drawTri->debugRender(Color(255,255,255));
	//}

	for( s32 y=0; y<_DivCount; ++y )
	{
		for( s32 x=0; x<_DivCount; ++x )
		{
			drawObject = _objects.at(y * _DivCount + x);
			//for( s32 i=0; i<drawObject.getTriangleCount(); ++i )
			for( s32 i=0; i<drawObject.getColDataCount(); ++i )
			{
				//Triangle* drawTri = drawObject.getTriangle(i);
				Triangle* drawTri = &drawObject.getColData(i)->_triangle;
				drawTri->debugRender(Color(255,255,255));
			}
		}
	}
}

//=============================================================================
//	END OF FILE
//=============================================================================
