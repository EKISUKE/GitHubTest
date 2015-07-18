//-----------------------------------------------------------------------------
//!
//!	@file	gmLandScape.cpp
//!	@brief	衝突判定(ランドスケープ)
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

// 分割数
#define LANDSCAPE_DIVDE_NUM 100

//=============================================================================
// LandScapeの実装
//=============================================================================

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
LandScape::LandScape()
{
	_LastHitTriangle = NULL;
	//SAFE_NEW(_LastHitTriangle);
	// 分割数
	_DivCount = LANDSCAPE_DIVDE_NUM;
}
//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
LandScape::~LandScape()
{
	// 仮でやっていない(pTriangleのポインタが消えた時点でもう一回デリートはしていけないため)
	//SAFE_DELETE(_LastHitTriangle);
	/*for( vecCol::iterator itr = _objects.begin(); itr != _objects.end();){
		itr = _objects.erase(itr);
	}*/

	_objects.clear();
}

//-----------------------------------------------------------------------------
//! Xファイルからポリゴンデーターを入力
//! @param	[in]	pModel		Xファイル
//-----------------------------------------------------------------------------
bool LandScape::load(TaskModelX*	pModel)
{
	
	// 最大値
	_max._x = -FLT_MAX;
	_min._x =  FLT_MAX;
	_max._z = -FLT_MAX;
	_min._z =  FLT_MAX;

	// オブジェクトの取得
	SystemCollision::Object obj = GmSystemCollision()->loadXfile(pModel, _objects, _max, _min, false);

	//_aabbTree.build(obj.getColDataVec());

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
	//for( s32 i=0; i<obj.getTriangleCount(); i++ )
	for( s32 i=0; i<obj.getColDataCount(); i++ )
	{
		CollisionData& col = *obj.getColData(i);
		//Triangle* pTriangle = obj.getTriangle(i);
		Triangle* pTriangle = &col._triangle;
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
				//_objects.at( z * _DivCount +x ).addTriangle(*pTriangle);
				_objects.at( z * _DivCount +x ).addColData(col);
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
bool LandScape::loadWithFrame(AssetModelX::Frame* pFrame, TaskModelX* parentModel)
{
	
	// 最大値
	_max._x = -FLT_MAX;
	_min._x =  FLT_MAX;
	_max._z = -FLT_MAX;
	_min._z =  FLT_MAX;

	// オブジェクトの取得
	SystemCollision::Object obj = GmSystemCollision()->loadWithFrame(pFrame, _objects, _max, _min, false, parentModel);

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
	//for( s32 i=0; i<obj.getTriangleCount(); i++ )
	for( s32 i=0; i<obj.getColDataCount(); i++ )
	{
		CollisionData& col = *obj.getColData(i);
		//Triangle* pTriangle = obj.getTriangle(i);
		Triangle* pTriangle = &col._triangle;
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
				//_objects.at( z * _DivCount +x ).addTriangle(*pTriangle);
				_objects.at( z * _DivCount +x ).addColData(col);
			}
		}
	}

	return true;
}





//-----------------------------------------------------------------------------
//!	指定した座標にある地面の高さを取得
//!	@param	[in]	positon		チェックする位置
//!	@param	[in]	heightLimit	判定上限高さ
//!	@return	地面の高さ(Y座標)
//-----------------------------------------------------------------------------
f32 LandScape::getHeight(const Vector3& position, f32 heightLimit)
{
	// 接触点
	f32	 height = 0.0f;

	// 衝突点までの距離(二乗で比較)
	f32	nearestHitPosLength = FLT_MAX;
	// floatのMAX値
	f32 nearestHeight = FLT_MAX;
	// プレイヤーから一番近い三角形の銃身までの距離(二乗で比較)
	//f32	nearestLength = FLT_MAX * FLT_MAX;
	// 衝突したかどうか
	static bool isHit	  = false;

	// 重力の移動方向
	Vector3 gravityMov = Vector3(0,-1,0);


	// もらった座標をインデックス番号に変更
	s32 x = ( (s32)(position._x) + _offsetX ) / _divX;
	s32 y = ( (s32)(position._z) + _offsetZ ) / _divZ;

_checkIndex._x = (f32)x;
_checkIndex._y = (f32)y;

	
	// 範囲外なら処理しない
	if( x >= _DivCount || y >= _DivCount || x < 0 || y < 0 ) return height;
	

	if( x == 10 && y == 11 )
	{
		s32 a = 0;
	}

	// 前回一番短かった長さ
	static f32 prevMinLength = -1;

	// その番号のオブジェクトを調べる
	SystemCollision::Object* pObject = getObject(y * _DivCount + x);
		
	// 初期化
	isHit = false;

	//for( s32 i=0; i<pObject->getTriangleCount(); i++ ) {
	for( s32 i=0; i<pObject->getColDataCount(); i++ ) {

		CollisionData& col = *pObject->getColData(i);
		

		//Triangle*	pTriangle = pObject->getTriangle(i);
		Triangle*	pTriangle = &col._triangle;
		//// この三角形の重心を求める
		//Vector3	triCenter = pTriangle->getCenter();
		//// 重心から座標までの距離
		//f32		lengthFromCenter = abs((position - triCenter).length());
		//// その距離が一定値より大きければ計算しない
		//if( lengthFromCenter >= 400.0f ) continue;

		// 当たり判定用ラインの作成
		Line	line;
		line._position[0] = position;
		line._position[1] = position + gravityMov;
		Vector3	hitPos;
		f32		h;

		if( collision::isHit( *pTriangle, line, &hitPos ) )
		{
				h = hitPos._y;
		}else{
			continue;
		}
		//-----------------------------------------------------------
		// 衝突あり
		//-----------------------------------------------------------
		// 頭上の高さ上限チェック
		if( h > position._y + heightLimit ) {
			continue;
		}

		if( h != 0.0f )
		{
			int a = 0;
		}

		// 遠い点なら処理終了
		if( (position - hitPos).squareLength() >= nearestHitPosLength * nearestHitPosLength )
		{
			continue;
		}

		//// 遠い高さなら処理終了
		//if( h >= nearestHeight ) {
		//	//continue;
		//}
		

		// 一番近い点までの距離を記録
		nearestHitPosLength = (position - hitPos).length();
		// 一番近いところを記録
		//nearestLength = lengthFromCenter;
		// いちばん近い高さを記録
		nearestHeight = h;
		// 高さ記録
		height		  = h;
		// あたった三角形の記録
		_LastHitTriangle = pTriangle;
		// 衝突済み
		isHit = true;
	}
	prevMinLength = nearestHitPosLength;
	// この時点で見つかったら処理終了
	if( isHit )return height;
	return height;
}


//-----------------------------------------------------------------------------
//! デバッグ描画
//-----------------------------------------------------------------------------
void LandScape::DebugRender()
{
	static s32 IndexX = 10;
	static s32 IndexY = 11;

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


// 調べてるグリッド
IndexX = (s32)_checkIndex._x;
IndexY = (s32)_checkIndex._y;

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
	

	
	

	//drawObject = _objects.at(IndexY * _DivCount + IndexX);
	//// 描画
	//for( s32 i=0; i<drawObject.getColDataCount(); ++i )
	//{
	//	Triangle* drawTri = &drawObject.getColData(i)->_triangle;
	//	drawTri->debugRender(Color(255,255,255));
	//}
	
	
	
	//for( s32 y=0; y<_DivCount; ++y )
	//{
	//	for( s32 x=0; x<_DivCount; ++x )
	//	{
	//		drawObject = _objects.at(y * _DivCount + x);
	//		//for( s32 i=0; i<drawObject.getTriangleCount(); ++i )
	//		for( s32 i=0; i<drawObject.getColDataCount(); ++i )
	//		{
	//			//Triangle* drawTri = drawObject.getTriangle(i);
	//			Triangle* drawTri = &drawObject.getColData(i)->_triangle;
	//			drawTri->debugRender(Color(255,255,255));
	//		}
	//	}
	//}


	_LastHitTriangle->debugRender(Color(255,0,0));
}

//-----------------------------------------------------------------------------
//! デバッグ描画(2D)
//-----------------------------------------------------------------------------
void LandScape::DebugRender2D()
{
	Vector3 normal = _LastHitTriangle->getNormal();
	Vector3 worldUp = Vector3(0.0f, 1.0f, 0.0f);
	
	normal  = normal.normalize();
	worldUp = worldUp.normalize();
	f32 dot = Vector3::dot(worldUp, normal);
	//f32 degRot = TO_DEGREE(atan2f( normal._y, normal._z));
	//f32 degRot = TO_DEGREE(atan2f(-normal._z, normal._y));
	f32 degRot = TO_DEGREE(acos(dot));

	//degRot -= 90.0f;
	degRot -= 180.0f;

	// 角度を180～-180に収める
	//if( degRot >=   180.0f ) degRot -= 360.0f;
	//if( degRot <=  -180.0f ) degRot += 360.0f;

	
	// 絶対値で比較
	degRot = abs(degRot);

	Vector3 pos	   = _LastHitTriangle->_position[0];

	stringstream str;
	str<< "LastHitTriangleRotX: ";
	str<< degRot;
	//font.Draw3DStringW(Vector3(1.0f, 0.0f, 0.0f), 100.0f, 100.0f, 0.0f, 1, false, (wchar_t*)str.str().c_str());
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	render_string(100.0f, 150.0f, str.str());
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}


//=============================================================================
//	END OF FILE
//=============================================================================
