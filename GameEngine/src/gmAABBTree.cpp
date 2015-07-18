//-----------------------------------------------------------------------------
//!
//!	@file	gmAABBTree.cpp
//!	@brief	AABB-Tree
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
//!	ラインの描画 (Flat)
//!	@param	[in]	p1		座標1
//!	@param	[in]	p1		座標2
//!	@param	[in]	color	カラー
//! TODO:今はなぜか線が描画されないので原因を考える
//---------------------------------------------------------------------------
void	drawLineF(const Vector3& p1, const Vector3& p2, const Vector4& color)
{
	GmShader()->changeShader(SystemShader::SHADER_COLOR);

	// 頂点の作成
	const Vertex v[2] = {
		{ Float3( p1 ),  color },
		{ Float3( p2 ),  color },
	};

	static GLuint VB;
	static bool Initialized = false;
	if( !Initialized ) {
		glGenBuffers(1, &VB);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 2, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		Initialized = true;
	}


	// 頂点バッファバインド
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	// データ転送
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 2, v);

	GmShader()->setAndEnableVertexAttribute("position"		, 3, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, _position));
	GmShader()->setAndEnableVertexAttribute("diffuseColor"	, 4, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, _color));

	glDrawArrays(GL_LINES, 0, 2);

	GmShader()->DisableVertexAttribute("position");
	GmShader()->DisableVertexAttribute("diffuseColor");

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GmShader()->beginPrevShader();

}

//-----------------------------------------------------------------------------
//! AABB描画
//-----------------------------------------------------------------------------
void drawAABB(const AABB& aabb, const Vector4& color, const Vector3& offset )
{
	const Vector3	p[] = {
		Vector3( aabb._min._x, aabb._max._y, aabb._min._z ) + offset,
		Vector3( aabb._max._x, aabb._max._y, aabb._min._z ) + offset,
		Vector3( aabb._max._x, aabb._min._y, aabb._min._z ) + offset,
		Vector3( aabb._min._x, aabb._min._y, aabb._min._z ) + offset,

		Vector3( aabb._min._x, aabb._max._y, aabb._max._z ) + offset,
		Vector3( aabb._max._x, aabb._max._y, aabb._max._z ) + offset,
		Vector3( aabb._max._x, aabb._min._y, aabb._max._z ) + offset,
		Vector3( aabb._min._x, aabb._min._y, aabb._max._z ) + offset
	};

	drawLineF(p[0], p[1], color);
	drawLineF(p[1], p[2], color);
	drawLineF(p[2], p[3], color);
	drawLineF(p[3], p[0], color);

	drawLineF(p[4], p[5], color);
	drawLineF(p[5], p[6], color);
	drawLineF(p[6], p[7], color);
	drawLineF(p[7], p[4], color);

	drawLineF(p[0], p[4], color);
	drawLineF(p[1], p[5], color);
	drawLineF(p[2], p[6], color);
	drawLineF(p[3], p[7], color);
}

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
AABBTree::AABBTree()
{
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
AABBTree::~AABBTree()
{
}


//-----------------------------------------------------------------------------
//! ツリー構築
//-----------------------------------------------------------------------------
bool AABBTree::build(vector<CollisionData>& colData)
{
	return _rootNode.build(colData);
}

//-----------------------------------------------------------------------------
//! 三角形リストからノード情報を構築
//! @param	[in]	colData	当たり判定情報の配列データ
//-----------------------------------------------------------------------------
bool AABBTree::Node::build(vector<CollisionData> colData)
{
	//-------------------------------------------------------------
	// (1) すべてのポリゴンを検索してバウンディングボックスを計算
	//-------------------------------------------------------------
	AABB&	aabb = _aabb;

	aabb.setEmpty();

	//for( s32 i=0; i<_triangles.size(); i++ ) 
	for( u32 i=0; i<colData.size(); ++i )
	{
		//Triangle&	triangle = _triangles[i];
		Triangle&	triangle = colData[i]._triangle;

		aabb.expand(triangle._position[0]);
		aabb.expand(triangle._position[1]);
		aabb.expand(triangle._position[2]);
	}

	// データの量取得
	_dataCount = colData.size();

	//---- 最後の1枚の三角形だった場合は分配作業完了！
	//if( _triangles.size() <= 1 ) 
	//if( _colData.size() <= 1 ) 
	if( _dataCount <= 1 )
	{
		// メモリ確保
		_col = new CollisionData();
		// データを格納
		*_col = colData[0];
		return true;
	}

	//-------------------------------------------------------------
	// (2) バウンディングボックスの各軸の長さで最も長い軸を検出
	//-------------------------------------------------------------
	s32		axis      = -1;
	f32		maxExtent = -FLT_MAX;

	for( s32 i=0; i<3; i++ ) {		// X, Y, Z
		f32	extent = aabb._max[i] - aabb._min[i];

		// 今の長さが一番長い場合
		if( maxExtent < extent ) {
			axis      = i;
			maxExtent = extent;
		}
	}


	//-------------------------------------------------------------
	// (3) 選択した軸方向のどこを基準に分割するかを求める
	//-------------------------------------------------------------
	f32		averagePosition = 0.0f;
	//for( s32 i=0; i<_triangles.size(); i++ ) {
	for( u32 i=0; i<colData.size(); i++ ) {
		//Triangle&	triangle = _triangles[i];
		Triangle&	triangle = colData[i]._triangle;

		averagePosition += triangle._position[0][axis];
		averagePosition += triangle._position[1][axis];
		averagePosition += triangle._position[2][axis];
	}
	//f32		splitPoint = averagePosition / (f32)(_triangles.size() * 3);	// 平均
	f32		splitPoint = averagePosition / (f32)(colData.size() * 3);	// 平均




	//-------------------------------------------------------------
	// (4) 子ノードに基準点をもとに振り分ける
	//-------------------------------------------------------------
	_pChild[0] = std::unique_ptr<AABBTree::Node>( new AABBTree::Node() ); 
	_pChild[1] = std::unique_ptr<AABBTree::Node>( new AABBTree::Node() ); 
	if( _pChild[0].get() == nullptr ) return false;
	if( _pChild[1].get() == nullptr ) return false;

	// 再帰呼び出しのための引数
	vector<CollisionData> child[2];

	//for( s32 i=0; i<_triangles.size(); i++ ) {
	for( u32 i=0; i<colData.size(); i++ ) {
		//Triangle&	triangle = _triangles[i];
		CollisionData& col	 = colData[i];
		Triangle&	triangle = col._triangle;
		Vector3*	p        = &triangle._position[0];

		f32		minPosition = min(min(p[0][axis], p[1][axis]), p[2][axis]);
		f32		maxPosition = max(max(p[0][axis], p[1][axis]), p[2][axis]);
		f32		center = (minPosition + maxPosition) * 0.5f;

		

		if( center < splitPoint ) {
			//_pChild[0]->_triangles.push_back(triangle);
			//_pChild[0]->_colData.push_back(col);
			child[0].push_back(col);
		}
		else {
			//_pChild[1]->_triangles.push_back(triangle);
			//_pChild[1]->_colData.push_back(col);
			child[1].push_back(col);
		}
	}

	// ※ 注意 ※
	// どちらか片方に偏る場合があるため、その場合は均等に配分しなおす。
	//if( _pChild[0]->_triangles.size() == 0 || _pChild[1]->_triangles.size() == 0 ) {
	//if( _pChild[0]->_colData.size() == 0 || _pChild[1]->_colData.size() == 0 ) {
	if( child[0].size() == 0 || child[1].size() == 0 ) {
		// 均等に半分しなおす
		/*_pChild[0]->_triangles.resize(0);
		_pChild[1]->_triangles.resize(0);*/
		/*_pChild[0]->_colData.resize(0);
		_pChild[1]->_colData.resize(0);*/
		child[0].resize(0);
		child[1].resize(0);

		//for( s32 i=0; i<_triangles.size(); i++ ) {
		for( u32 i=0; i<colData.size(); i++ ) {

			//_pChild[i & 1]->_triangles.push_back( _triangles[i] );
			//_pChild[i & 1]->_colData.push_back( _colData[i] );
			child[i & 1].push_back( colData[i] );
		}
	}

	//-------------------------------------------------------------
	// 子供のノードを再帰処理（反復してノード構築）
	//-------------------------------------------------------------
	bool	result;
	
	result = _pChild[0]->build(child[0]);
	if( result == false ) return false;
	
	result = _pChild[1]->build(child[1]);
	if( result == false ) return false;

	return true;
}

//-----------------------------------------------------------------------------
//! aabbと衝突チェック
//!	@param	[in]	aabb	当たり判定を行うAABB
//!	@param	[in]	hitFunc	三角形との衝突判定用の関数ポインタ
//-----------------------------------------------------------------------------
bool AABBTree::Node::hitCheck(const AABB& aabb, bool(*hitFunc)(Triangle* tri))
{
	//drawAABB(_aabb, Vector4(1.0f, 0.0f, 0.0f, 1.0f));

	bool result = false;

	// 最後のノードなら終了
	//if( _triangles.size() <= 1 ) {
	//if( _colData.size() <= 1 ) {
	if( _dataCount <= 1 ) {

		drawAABB(_aabb, Vector4(1.0f, 0.0f, 0.0f, 1.0f));

		//CollisionData& col	 = _colData[0];
		CollisionData& col	 = *_col;
		if( col._isActive )
		{
			//Triangle&	triangle = _triangles[0];
			Triangle&	triangle = col._triangle;

			//------------------------------------------------------------
			// ■ ここにtriangleとの衝突判定を作成する
			//	コールバック関数で受け取るようにすることで利便性が増す
			//------------------------------------------------------------
			result = (*hitFunc)(&triangle);


			return result;
		}else{
			return result;
		}
	}

	//bool	result = false;
	for( s32 i=0; i<2; i++ ) {
		if( AABBCollision::isHit(aabb, _pChild[i]->_aabb) == false ) continue;

		if( _pChild[i]->hitCheck(aabb, (*hitFunc)) ) {
			result = true;
		}
	}
	return result;
}

//-----------------------------------------------------------------------------
//! デバッグ描画
//!	@param	[in]	nest	どの深さを描画するか
//-----------------------------------------------------------------------------
void AABBTree::Node::debugDraw(u32 nest)
{
	if( nest == 0 ) {
		// デバッグ描画
		// AABB
		//drawAABB(_aabb, Vector4(1.0f, 0.5f, 1.0f, 1.0f));
		//return;
	}
	// 最後のノードなら終了
	//if( _triangles.size() <= 1 ) return;
	//if( _colData.size() <= 1 ) return;
	if( _dataCount <= 1 ){
		if( _col->_isActive ) {
			Vector4	color = Vector4(1.0f, 1.0f, 0.0f, 1.0f);

			Triangle&	triangle = _col->_triangle;

			drawLineF(triangle._position[0], triangle._position[1], color);
			drawLineF(triangle._position[1], triangle._position[2], color);
			drawLineF(triangle._position[2], triangle._position[0], color);
		}
		return;
	}


	nest--;
	_pChild[0]->debugDraw(nest);
	_pChild[1]->debugDraw(nest);
}





// 0.0f～1.0fの乱数
#define FRAND()	((f32)rand() * (1.0f / 32767.0f))

//-----------------------------------------------------------------------------
//! デバッグ用の準備
//-----------------------------------------------------------------------------
bool AABBTree::debugSetup()
{
	srand(400);

	vector<CollisionData> debugData;

	for( s32 group=0; group<10; group++ ) {
		Vector3		groupCenter;
		groupCenter._x = (FRAND() - 0.5f) * 2.0f * 10.0f;
		groupCenter._y =  FRAND()                *  2.0f;
		groupCenter._z = (FRAND() - 0.5f) * 2.0f * 10.0f;




		for( s32 t=0; t<50; t++ ) {
			Vector3		position[3];


			Vector3		center;
			center._x = FRAND() * 5.0f;
			center._y = FRAND() * 5.0f;
			center._z = FRAND() * 5.0f;


			for( s32 i=0; i<3; i++ ) {
				Vector3&	p = position[i];
	
				p._x = FRAND() * 1.0f;
				p._y = FRAND() * 1.0f;
				p._z = FRAND() * 1.0f;

				p += center + groupCenter;
			}

			Triangle	triangle(position[0], position[1], position[2]);
			CollisionData col;
			col._isActive = true;
			col._myFrameNumber = 0;
			col._triangle = triangle;
			

			//_rootNode._triangles.push_back(triangle);
			//_rootNode._colData.push_back(col);
			debugData.push_back(col);
		}

	}

	return build(debugData);
}



bool	debugColTriangleFunc(Triangle* tri)
{
	return true;
}


//-----------------------------------------------------------------------------
//! デバッグ描画
//-----------------------------------------------------------------------------
void AABBTree::debugRender()
{
	//for( s32 i=0; i<_rootNode._triangles.size(); i++ ) {
	//for( s32 i=0; i<_rootNode._colData.size(); i++ ) {

	//	CollisionData& col = _rootNode._colData[i];
	//	if( col._isActive ) {
	//		Vector4	color = Vector4(1.0f, 1.0f, 0.0f, 1.0f);

	//		//Triangle&	triangle = _rootNode._triangles[i];
	//		Triangle&	triangle = col._triangle;

	//		drawLineF(triangle._position[0], triangle._position[1], color);
	//		drawLineF(triangle._position[1], triangle._position[2], color);
	//		drawLineF(triangle._position[2], triangle._position[0], color);
	//	}
	//}

	drawAABB(_rootNode._aabb, Vector4(0.0f, 0.5f, 1.0f, 1.0f));


	// 子ノードの状態を表示
#if 1
	static int count = 0;
	count++;

	u32	nest =  (count / (60*1) ) & 15;

	_rootNode.debugDraw(nest);
#endif


	static Vector3 position(0,0,0);

	
	AABB	aabb;
	aabb._min = position - Vector3(1, 1, 1);
	aabb._max = position + Vector3(1, 1, 1);
	drawAABB(aabb, Vector4(1.0f,1.0f,1.0f,1.0f));

	f32	speed = 0.2f;

	if( GetKeyState(VK_NUMPAD6) & 0x8000 ) {
		position._x += speed;
	}
	if( GetKeyState(VK_NUMPAD4) & 0x8000 ) {
		position._x -= speed;
	}

	if( GetKeyState(VK_NUMPAD8) & 0x8000 ) {
		position._z -= speed;
	}
	if( GetKeyState(VK_NUMPAD2) & 0x8000 ) {
		position._z += speed;
	}

	if( GetKeyState(VK_NUMPAD9) & 0x8000 ) {
		position._y += speed;
	}
	if( GetKeyState(VK_NUMPAD3) & 0x8000 ) {
		position._y -= speed;
	}


	_rootNode.hitCheck(aabb, (*debugColTriangleFunc));












}

//===========================================================================
// END OF FILE
//===========================================================================
