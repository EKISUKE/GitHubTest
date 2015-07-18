//-----------------------------------------------------------------------------
//!
//!	@file	gmCollision.cpp
//!	@brief	衝突判定
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//=============================================================================
// Collisionの実装
//=============================================================================
//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
Collision::Collision(Sphere* hitSphere, char* name)
//: _hitSphere(hitSphere)
{
	/*for( s32 i=0; i<4; ++i )
	{
		_hitObj[i] = NULL;
	}*/
	//strcpy( _name, name );
	// 情報設定
	setCollisionState(hitSphere, name);
	
}
//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
Collision::~Collision()
{
	GmCollisionMan()->UnRegist(this);
}

//-----------------------------------------------------------------------------
//! 情報設定
//!	@param	[in]	col	設定する当たり判定情報
//-----------------------------------------------------------------------------
void Collision::setHitObj(Collision* col)
{
	_hitObj.push_back(col);
}

//-----------------------------------------------------------------------------
//! 基本情報設定
//!	@param	[in]	sphere	当たり判定用の球
//!	@param	[in]	name	当たり判定の名前
//-----------------------------------------------------------------------------
void Collision::setCollisionState(Sphere* sphere, GM_CSTR name)
{
	// あたり判定用球の設定
	setHitSphere(sphere);
	// 名前設定
	setColName(name);

	GmCollisionMan()->Regist(this);
}
//-----------------------------------------------------------------------------
//! 当たり判定用球の設定
//!	@param	[in]	sphere	当たり判定用の球
//-----------------------------------------------------------------------------
void Collision::setHitSphere(Sphere* sphere)
{
	_hitSphere = sphere;
}
//-----------------------------------------------------------------------------
//! 名前の設定
//!	@param	[in]	name	当たり判定の名前
//-----------------------------------------------------------------------------
void Collision::setColName(GM_CSTR name)
{
	strcpy_s( _name, name );
}



//-----------------------------------------------------------------------------
//! 当たっている個数取得
//-----------------------------------------------------------------------------
s32 Collision::getHitCount()
{
	s32	result = 0;
	for( u32 i=0; i<_hitObj.size(); i++ )
	{
		if( _hitObj.at(i) != NULL ){
			result++;
		}
	}
	return result;
}


//-----------------------------------------------------------------------------
//!	当たった後の補正位置取得
//!	@param	[in]	obj		当たり判定の相手
//-----------------------------------------------------------------------------
Vector3	Collision::getOffsetPos(Collision*	obj)
{
	// 相手のあたり判定の球を取得
	Sphere*	eneSphere = obj->getHitSphere();

	// 相手から自分へのベクトルを取得する
	Vector3	dir	  = _hitSphere->getPosition() - eneSphere->getPosition();
	// それぞれの半径の合計を求める
	f32	addLength = _hitSphere->getRadius() + eneSphere->getRadius();
	// 一旦正規化して、それぞれの半径足した分に直す
	dir = dir.normalize();
	dir *= addLength;
	
	Vector3 enePos = eneSphere->getPosition();
	//enePos._y -= eneSphere->getRadius();

	Vector3 result = enePos + dir;
	return result;
}

//-----------------------------------------------------------------------------
//! デバック描画
//-----------------------------------------------------------------------------
void Collision::DebugRender()
{
	_hitSphere->DebugRender();
}

//=============================================================================
// Collisionの実装
//=============================================================================
// 静的メンバーの宣言
LandScape*				SystemCollision::_pLandScape;		
CollisionWall*			SystemCollision::_pColWall;


//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
SystemCollision::SystemCollision()
{
	_pLandScape = NULL;
	_pColWall   = NULL;
	SAFE_NEW(_pLandScape);
	SAFE_NEW(_pColWall);
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
SystemCollision::~SystemCollision()
{
	SAFE_DELETE(_pColWall);
	SAFE_DELETE(_pLandScape);
	//deleteObject();
};

//-----------------------------------------------------------------------------
//! オブジェクトの消去
//-----------------------------------------------------------------------------
void SystemCollision::deleteObject()
{

}

//-----------------------------------------------------------------------------
//! Xファイルからポリゴンデーターを入力
//! @param	[in]	colModel	Xファイル
//!	@param	[in]	scale		拡大率
//-----------------------------------------------------------------------------
bool SystemCollision::loadXfile(TaskModelX* colModel)
{
	//-------------------------------------------------------------
	// それぞれのデータを読み込む
	//-------------------------------------------------------------
	_pLandScape->load(colModel);
	_pColWall->load(colModel);

	return true;
}

//-----------------------------------------------------------------------------
//! フレームからランドスケープ当たり判定を入力
//! @param	[in]	colModel	フレーム
//!	@param	[in]	parentModel	フレームの属すタスクモデル
//-----------------------------------------------------------------------------
bool SystemCollision::loadLandScaleWithFrame(AssetModelX::Frame* colFrame, TaskModelX* parentModel)
{
	// フレームから読み込み
	if( !_pLandScape->loadWithFrame(colFrame, parentModel) ) {
		return false;
	}
	if( !_pColWall->loadWithFrame(colFrame, parentModel) ) {
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
//! 球同士のあたり判定
//!	@param	[in]	cubePos1	一つ目の球の座標
//!	@param	[in]	cubePos2	二つ目の球の座標
//!	@param	[in]	cubeRadius1	一つ目の球の半径
//!	@param	[in]	cubeRadius2	二つ目の球の半径
//!	@retval			true : 当たってる	false : 当たっていない
//-----------------------------------------------------------------------------
bool SystemCollision::HitSphere(Vector3& spherePos1, Vector3& spherePos2, f32 sphereRadius1, f32 sphereRadius2)
{
	Vector3 dir;
	// 球の間の長さを取る
	dir = spherePos1 - spherePos2;
	f32 length = dir.squareLength();

	// 半径同士を足したものを求める
	f32	radius = sphereRadius1 + sphereRadius2;

	// 間の長さが半径を足したものより短ければ
	if( length <= radius * radius ) {
		// 当たっている
		return true;
	}
	// 当たっていない
	return false;
}

//-----------------------------------------------------------------------------
//! 円同士のあたり判定
//!	@param	[in]	cubePos1	一つ目の円の座標
//!	@param	[in]	cubePos2	二つ目の円の座標
//!	@param	[in]	cubeRadius1	一つ目の円の半径
//!	@param	[in]	cubeRadius2	二つ目の円の半径
//!	@retval			true : 当たってる	false : 当たっていない
//-----------------------------------------------------------------------------
bool SystemCollision::HitCircle(Vector3& circlePos1, Vector3& circlePos2, f32 circleRadius1, f32 circleRadius2)
{
	Vector3 dir;
	// Ｙ座標を一旦０にする
	circlePos1._y = 0;
	circlePos2._y = 0;
	// 円の間の長さを取る
	dir = circlePos1 - circlePos2;
	f32	length = dir.squareLength();

	// 半径同士を足した物を求める
	f32	radius = circleRadius1 + circleRadius2;

	// 間の長さが半径を足したものより短ければ
	if( length <= radius * radius ) {
		// 当たっている
		return true;
	}
	// 当たっていない
	return false;
}


//-----------------------------------------------------------------------------
//! デバッグ描画
//-----------------------------------------------------------------------------
void SystemCollision::DebugRender()
{

	_pLandScape->DebugRender();
	//_pColWall->DebugRender();
}

//-----------------------------------------------------------------------------
//! デバッグ描画(2D)
//-----------------------------------------------------------------------------
void SystemCollision::DebugRender2D()
{
	_pLandScape->DebugRender2D();
}

//-----------------------------------------------------------------------------
//! 三角形の角度によって記録するかどうか
//! @retval true : 記録		false : 記録しない
//-----------------------------------------------------------------------------
bool partColForTriRot(Triangle& baseTri, bool isWall)
{

	// ポリゴンの角度を求める
	Vector3 normal = baseTri.getNormal();
	Vector3 worldUp = Vector3(0.0f, 1.0f, 0.0f);
	
	normal  = normal.normalize();
	worldUp = worldUp.normalize();
	f32 dot = Vector3::dot(worldUp, normal);
	f32 degRot = TO_DEGREE(acos(dot));
	//// ポリゴンの角度を求める
	//Vector3 normal = baseTri.getNormal();
	//f32 degRot = TO_DEGREE(atan2f( normal._y, normal._z));
	//// 角度を180～-180に収める
	//if( degRot >=   180.0f ) degRot -= 360.0f;
	//if( degRot <=  -180.0f ) degRot += 360.0f;

	////// 上方向は90度なので-90度する
	//degRot -= 180.0f;

	//// 絶対値で比較
	//degRot = abs(degRot);
	
	// 壁の記録なら
	if( isWall ){
		// 60度以上なら
		if( degRot >= 60.0f) {
			// 記録
			return true;
		}
	}else{
		// 角度が60度以下なら
		if( degRot <= 60.0f){
			// 記録
			return true;
		}
	}

	
	// 記録しない
	return false;
}

//-----------------------------------------------------------------------------
//! 再帰呼び出しでメッシュデータを取得
//-----------------------------------------------------------------------------
AssetModelX::Mesh*		getMesh(AssetModelX::Frame* pModelFrame)
{
	AssetModelX::Mesh*		pModelMesh = nullptr;
	// このフレームにはメッシュが入ってなかったら
	if(pModelFrame->_meshes.size() > 0)
	{
		// 入っていればその先頭アドレスを入れる
		pModelMesh = pModelFrame->_meshes.at(0);
		//// 兄弟ノードがNULLじゃなかったら
		//if( pModelFrame->getNext() != nullptr ){
		//	// 兄弟ノードのメッシュを調べる
		//	AssetModelX::Frame* next = pModelFrame->getNext();
		//	pModelMesh  = getMesh(pModelFrame->getNext());
		//}else{
		//	// 子ノードがNULLじゃなかったら
		//	if( pModelFrame->getChild() != nullptr ) {
		//		// 子ノードをのメッシュを調べる
		//		AssetModelX::Frame* child = pModelFrame->getChild();
		//		pModelMesh = getMesh(pModelFrame->getChild());
		//	}else{
		//		// 取得できるデータがないので終了
		//		return nullptr;
		//	}
		//}
	}
	// メッシュを返す
	return pModelMesh;
}

//-----------------------------------------------------------------------------
//! フレームからデータ読み込み
//! @param	[in]	pModel		Xファイル
//!	@param	[in]	max			最大座標(XZ)保存用
//!	@param	[in]	min			最小座標(XZ)保存用
//! @param  [in]	isWall		壁用かどうか
//! @param  [in]	modelMat	モデル行列
//! @param  [in]	joints		そのフレームの属すジョイント
//-----------------------------------------------------------------------------
SystemCollision::Object SystemCollision::loadData(AssetModelX::Frame* pModelFrame, Vector3& max, Vector3& min, bool isWall, Matrix& modelMat, vector<TaskModelX::Joint> joints)
{
	SystemCollision::Object	CollisionObject;
	// 有効なメッシュを取得する
	AssetModelX::Mesh*		pModelMesh = getMesh(pModelFrame);
	
	TaskModelX::Joint* pJoint = &joints[(s32)( pModelFrame->getJointIndex() )];

	Matrix matWorld = pJoint->_transformMatrix * modelMat;

	// もうメッシュがなければ処理しない
	if(pModelMesh != nullptr) {
		
		//return CollisionObject;
	
		CollisionData colData;
	

		//-------------------------------------------------------------
		// pModelObject(モデルデータ)→ pCollisionObject(衝突用モデル)へ移し替える
		//-------------------------------------------------------------
		for( u32 face=0; face<pModelMesh->_faces.size(); face++ ) {
		
			colData._myFrameNumber = pModelMesh->getJointIndex();

			Triangle	triangle;

			// ポリゴンデータを抽出
			// (四角形ポリゴンは三角形に分割する)
			AssetModelX::Face*	pFace = &pModelMesh->_faces.at(face);

			if( pFace->_vertexCount == 3 ) {	//---- 三角形
				
				//---- 頂点番号
				s32	i0 = pFace->_vertexIndex[0];
				s32	i1 = pFace->_vertexIndex[1];
				s32	i2 = pFace->_vertexIndex[2];

				Vector3 iPos[3];
				iPos[0] = pModelMesh->_vertices.at( i0 )._position;
				iPos[1] = pModelMesh->_vertices.at( i1 )._position;
				iPos[2] = pModelMesh->_vertices.at( i2 )._position;
			
				//---- 座標変換
				iPos[0] = iPos[0].transform(matWorld);
				iPos[1] = iPos[1].transform(matWorld);
				iPos[2] = iPos[2].transform(matWorld);
			

				/*triangle = Triangle(iPos[0],
									iPos[1],
									iPos[2]);*/

				triangle = Triangle(iPos[2],
									iPos[1],
									iPos[0]);

				// 記録するかどうか
				if( !partColForTriRot(triangle, isWall) ) continue;

				for( s32 i=0; i<3; i++ )
				{
					// Xの最大より大きければ
					if( iPos[i]._x > max._x )
					{
						// 更新
						max._x = iPos[i]._x;
					}
					if( iPos[i]._x < min._x ){
						// Xの最小より小さければ更新
						min._x = iPos[i]._x;
					}

					// Zの最大より大きければ
					if( iPos[i]._z > max._z )
					{
						// 更新
						max._z = iPos[i]._z;
					}
					if( iPos[i]._z < min._z ){
						// Zの最小より小さければ更新
						min._z  = iPos[i]._z;
					}
				}

				colData._triangle = triangle;
				//---- 衝突判定用データに登録
				CollisionObject.addColData(colData);

				
			}
			else if( pFace->_vertexCount == 4 ){					//---- 四角形
				
				//-------------------------------------------------------------
				// 1分割目
				//-------------------------------------------------------------
			
				//---- 頂点番号
				s32	i0 = pFace->_vertexIndex[0];
				s32	i1 = pFace->_vertexIndex[1];
				s32	i2 = pFace->_vertexIndex[2];


				Vector3 iPos[3];
				iPos[0] = pModelMesh->_vertices.at( i0 )._position;
				iPos[1] = pModelMesh->_vertices.at( i1 )._position;
				iPos[2] = pModelMesh->_vertices.at( i2 )._position;
			
				//---- 座標変換
				iPos[0] = iPos[0].transform(matWorld);
				iPos[1] = iPos[1].transform(matWorld);
				iPos[2] = iPos[2].transform(matWorld);
			

				triangle = Triangle(iPos[2],
									iPos[1],
									iPos[0]);
				
			
				// 記録するかどうか
				if( partColForTriRot(triangle, isWall) ){
				
					for( s32 i=0; i<3; i++ )
					{
						// Xの最大より大きければ
						if( iPos[i]._x > max._x )
						{
							// 更新
							max._x = iPos[i]._x;
						}
						if( iPos[i]._x < min._x ){
							// Xの最小より小さければ更新
							min._x = iPos[i]._x;
						}

						// Zの最大より大きければ
						if( iPos[i]._z > max._z )
						{
							// 更新
							max._z = iPos[i]._z;
						}
						if( iPos[i]._z < min._z ){
							// Zの最小より小さければ更新
							min._z  = iPos[i]._z;
						}
					}

					colData._triangle = triangle;
					//---- 衝突判定用データに登録
					CollisionObject.addColData(colData);

				}

				//-------------------------------------------------------------
				// 2分割目
				//-------------------------------------------------------------

				//---- 頂点番号
				i0 = pFace->_vertexIndex[1];
				i1 = pFace->_vertexIndex[2];
				i2 = pFace->_vertexIndex[3];
			
				iPos[0] = pModelMesh->_vertices.at( i0 )._position;
				iPos[1] = pModelMesh->_vertices.at( i1 )._position;
				iPos[2] = pModelMesh->_vertices.at( i2 )._position;

				//---- 座標変換
				iPos[0] = iPos[0].transform(matWorld);
				iPos[1] = iPos[1].transform(matWorld);
				iPos[2] = iPos[2].transform(matWorld);


				triangle = Triangle(iPos[2],
									iPos[1],
									iPos[0]);

				// 記録するかどうか
				if( partColForTriRot(triangle, isWall) ){

					for( s32 i=0; i<3; i++ )
					{
						// Xの最大より大きければ
						if( iPos[i]._x > max._x )
						{
							// 更新
							max._x = iPos[i]._x;
						}
						if( iPos[i]._x < min._x ){
							// Xの最小より小さければ更新
							min._x = iPos[i]._x;
						}

						// Zの最大より大きければ
						if( iPos[i]._z > max._z )
						{
							// 更新
							max._z = iPos[i]._z;
						}
						if( iPos[i]._z < min._z ){
							// Zの最小より小さければ更新
							min._z  = iPos[i]._z;
						}
					}

					colData._triangle = triangle;
					//---- 衝突判定用データに登録
					CollisionObject.addColData(colData);
				}
			}
		}

	}

	//---- まだ兄弟、子ノードにフレームがあれば再帰呼び出しする
	if( pModelFrame->getNext() != nullptr )
	{
		SystemCollision::Object obj = loadData(pModelFrame->getNext(), max, min, isWall, modelMat, joints);
		for( s32 i=0; i<obj.getColDataCount(); i++ )
		{
			CollisionData col = *obj.getColData(i);
			CollisionObject.addColData(col);
		}
	}
	if( pModelFrame->getChild() != nullptr ){
		SystemCollision::Object obj = loadData(pModelFrame->getChild(), max, min, isWall, modelMat, joints);
		for( s32 i=0; i<obj.getColDataCount(); i++ )
		{
			CollisionData col = *obj.getColData(i);
			CollisionObject.addColData(col);
		}
	}

	return CollisionObject;

}

//-----------------------------------------------------------------------------
//! フレームからデータ読み込み(再帰なし)
//! @param	[in]	pModel		Xファイル
//!	@param	[in]	max			最大座標(XZ)保存用
//!	@param	[in]	min			最小座標(XZ)保存用
//! @param  [in]	isWall		壁用かどうか
//! @param  [in]	modelMat	モデル行列
//! @param  [in]	joints		そのフレームの属すジョイント
//-----------------------------------------------------------------------------
SystemCollision::Object SystemCollision::loadDataOneFrame(AssetModelX::Frame* pModelFrame, Vector3& max, Vector3& min, bool isWall, Matrix& modelMat, vector<TaskModelX::Joint> joints)
{
	SystemCollision::Object	CollisionObject;
	// 有効なメッシュを取得する
	AssetModelX::Mesh*		pModelMesh = getMesh(pModelFrame);
	
	TaskModelX::Joint* pJoint = &joints[(s32)( pModelFrame->getJointIndex() )];

	Matrix matWorld = pJoint->_transformMatrix * modelMat;

	// もうメッシュがなければ処理しない
	if(pModelMesh != nullptr) {
	
		CollisionData colData;
	

		//-------------------------------------------------------------
		// pModelObject(モデルデータ)→ pCollisionObject(衝突用モデル)へ移し替える
		//-------------------------------------------------------------
		for( u32 face=0; face<pModelMesh->_faces.size(); face++ ) {
		
			colData._myFrameNumber = pModelMesh->getJointIndex();

			Triangle	triangle;

			// ポリゴンデータを抽出
			// (四角形ポリゴンは三角形に分割する)
			AssetModelX::Face*	pFace = &pModelMesh->_faces.at(face);

			if( pFace->_vertexCount == 3 ) {	//---- 三角形
				
				//---- 頂点番号
				s32	i0 = pFace->_vertexIndex[0];
				s32	i1 = pFace->_vertexIndex[1];
				s32	i2 = pFace->_vertexIndex[2];

				Vector3 iPos[3];
				iPos[0] = pModelMesh->_vertices.at( i0 )._position;
				iPos[1] = pModelMesh->_vertices.at( i1 )._position;
				iPos[2] = pModelMesh->_vertices.at( i2 )._position;
			
				//---- 座標変換
				iPos[0] = iPos[0].transform(matWorld);
				iPos[1] = iPos[1].transform(matWorld);
				iPos[2] = iPos[2].transform(matWorld);
			

				/*triangle = Triangle(iPos[0],
									iPos[1],
									iPos[2]);*/

				triangle = Triangle(iPos[2],
									iPos[1],
									iPos[0]);

				// 記録するかどうか
				if( !partColForTriRot(triangle, isWall) ) continue;

				for( s32 i=0; i<3; i++ )
				{
					// Xの最大より大きければ
					if( iPos[i]._x > max._x )
					{
						// 更新
						max._x = iPos[i]._x;
					}
					if( iPos[i]._x < min._x ){
						// Xの最小より小さければ更新
						min._x = iPos[i]._x;
					}

					// Zの最大より大きければ
					if( iPos[i]._z > max._z )
					{
						// 更新
						max._z = iPos[i]._z;
					}
					if( iPos[i]._z < min._z ){
						// Zの最小より小さければ更新
						min._z  = iPos[i]._z;
					}
				}

				colData._triangle = triangle;
				//---- 衝突判定用データに登録
				CollisionObject.addColData(colData);

				
			}
			else if( pFace->_vertexCount == 4 ){					//---- 四角形
				
				//-------------------------------------------------------------
				// 1分割目
				//-------------------------------------------------------------
			
				//---- 頂点番号
				s32	i0 = pFace->_vertexIndex[0];
				s32	i1 = pFace->_vertexIndex[1];
				s32	i2 = pFace->_vertexIndex[2];


				Vector3 iPos[3];
				iPos[0] = pModelMesh->_vertices.at( i0 )._position;
				iPos[1] = pModelMesh->_vertices.at( i1 )._position;
				iPos[2] = pModelMesh->_vertices.at( i2 )._position;
			
				//---- 座標変換
				iPos[0] = iPos[0].transform(matWorld);
				iPos[1] = iPos[1].transform(matWorld);
				iPos[2] = iPos[2].transform(matWorld);
			

				triangle = Triangle(iPos[2],
									iPos[1],
									iPos[0]);
				
			
				// 記録するかどうか
				if( partColForTriRot(triangle, isWall) ){
				
					for( s32 i=0; i<3; i++ )
					{
						// Xの最大より大きければ
						if( iPos[i]._x > max._x )
						{
							// 更新
							max._x = iPos[i]._x;
						}
						if( iPos[i]._x < min._x ){
							// Xの最小より小さければ更新
							min._x = iPos[i]._x;
						}

						// Zの最大より大きければ
						if( iPos[i]._z > max._z )
						{
							// 更新
							max._z = iPos[i]._z;
						}
						if( iPos[i]._z < min._z ){
							// Zの最小より小さければ更新
							min._z  = iPos[i]._z;
						}
					}

					colData._triangle = triangle;
					//---- 衝突判定用データに登録
					CollisionObject.addColData(colData);

				}

				//-------------------------------------------------------------
				// 2分割目
				//-------------------------------------------------------------

				//---- 頂点番号
				i0 = pFace->_vertexIndex[1];
				i1 = pFace->_vertexIndex[2];
				i2 = pFace->_vertexIndex[3];
			
				iPos[0] = pModelMesh->_vertices.at( i0 )._position;
				iPos[1] = pModelMesh->_vertices.at( i1 )._position;
				iPos[2] = pModelMesh->_vertices.at( i2 )._position;

				//---- 座標変換
				iPos[0] = iPos[0].transform(matWorld);
				iPos[1] = iPos[1].transform(matWorld);
				iPos[2] = iPos[2].transform(matWorld);


				triangle = Triangle(iPos[2],
									iPos[1],
									iPos[0]);

				// 記録するかどうか
				if( partColForTriRot(triangle, isWall) ){

					for( s32 i=0; i<3; i++ )
					{
						// Xの最大より大きければ
						if( iPos[i]._x > max._x )
						{
							// 更新
							max._x = iPos[i]._x;
						}
						if( iPos[i]._x < min._x ){
							// Xの最小より小さければ更新
							min._x = iPos[i]._x;
						}

						// Zの最大より大きければ
						if( iPos[i]._z > max._z )
						{
							// 更新
							max._z = iPos[i]._z;
						}
						if( iPos[i]._z < min._z ){
							// Zの最小より小さければ更新
							min._z  = iPos[i]._z;
						}
					}

					colData._triangle = triangle;
					//---- 衝突判定用データに登録
					CollisionObject.addColData(colData);
				}
			}
		}
	}

	return CollisionObject;

}



//-----------------------------------------------------------------------------
//! Xファイルからポリゴンデーターを返す
//! @param	[in]	pModel		Xファイル
//!	@param	[in]	dst			読み込み結果保存先
//!	@param	[in]	max			最大座標(XZ)保存用
//!	@param	[in]	min			最小座標(XZ)保存用
//! @param  [in]	isWall		壁用かどうか
//-----------------------------------------------------------------------------
SystemCollision::Object	SystemCollision::loadXfile(TaskModelX* pModel, vecCol& _dst, Vector3& max, Vector3& min, bool isWall)
{

	SystemCollision::Object	CollisionObject;
	
	// モデルのオリジナルデータ取得
	AssetModelX*			pAssetModel = pModel->getModel();
	// メッシュデータの先頭を取得
	AssetModelX::Frame*		pModelFrame	= pAssetModel->_frames.at(0);
	
	// modelMatrixを作成
	f32	   scale	= pModel->getScale();
	Matrix worldMat = pModel->getWorldMatrix();
	Matrix rotMat	= pModel->getRotateMatrix();
	Matrix modelMat = Matrix::scale( Vector3(scale, scale, -scale) ) * rotMat * worldMat;

	// データ読み込み
	CollisionObject = loadData(pModelFrame, max, min, isWall, modelMat, pModel->_joints);


	/*if( CollisionObject.getTriangleCount() > 10000 )
	{
		int a = 0;
	}*/

	return CollisionObject;
}

//-----------------------------------------------------------------------------
//! Xファイルからポリゴンデーターを返す
//! @param	[in]	pModel		Xファイル
//!	@param	[in]	dst			読み込み結果保存先
//!	@param	[in]	max			最大座標(XZ)保存用
//!	@param	[in]	min			最小座標(XZ)保存用
//! @param  [in]	isWall		壁用かどうか
//-----------------------------------------------------------------------------
SystemCollision::Object	SystemCollision::loadWithFrame(AssetModelX::Frame* colFrame, vecCol& _dst, Vector3& max, Vector3& min, bool isWall, TaskModelX* parentModel)
{

	SystemCollision::Object	CollisionObject;
	
	AssetModelX::Frame*		pModelFrame	= colFrame;
	
	// modelMatrixを作成
	f32	   scale	= parentModel->getScale();
	Matrix worldMat = parentModel->getWorldMatrix();
	Matrix rotMat	= parentModel->getRotateMatrix();
	Matrix modelMat = Matrix::scale( Vector3(scale, scale, -scale) ) * rotMat * worldMat;

	// データ読み込み
	CollisionObject = loadDataOneFrame(pModelFrame, max, min, isWall, modelMat, parentModel->_joints);

	

	return CollisionObject;
}


//-----------------------------------------------------------------------------
//!	指定した座標とモデルの当たり判定と補正
//!	@param	[in]	positon		チェックする位置
//!	@param	[in]	heightLimit	判定上限高さ
//! @param	[in]	myGridNum	グリッド番号
//! @param	[in]	isHitGround 地面に当たったかどうか		
//-----------------------------------------------------------------------------
void SystemCollision::checkCollisionModel(Vector3& position, f32 radius, bool& isHitGround)
{
	s32	checkCount = 5;
	
	// 地面と非衝突
	isHitGround = false;

	AABB hitAABB;

	hitAABB._min     = position;
	hitAABB._min._x -= radius;
	hitAABB._min._z -= radius;
	hitAABB._min._y -= radius;
	hitAABB._max     = position;
	hitAABB._max._x += radius;
	hitAABB._max._z += radius;
	hitAABB._max._y += radius;


	// 衝突判定
	for( s32 i=0; i<checkCount; ++i )
	{
		bool isHit = false;
		// 壁との当たり判定
		// 衝突点の取得
		Vector3 hitPos = _pColWall->getHitPos(position, radius);
		//Vector3 hitPos = Vector3( 0.0f, 0.0f, 0.0f );
		// 当たり判定後のオフセット後のポジション
		// 衝突点からポジションまでのベクトル
		Vector3 dir = position - hitPos;
		// 長さが半径より小さかったら補正する
		//if( dir.length() < radius )
		if( dir.squareLength() < radius * radius )
		{
			// 衝突あり
			isHit = true;
			// 衝突点と移動ベクトルの内積がマイナスなら反対方向なので無視
			//if( Vector3::dot(-dir, mov) < 0.0f ) return;
			// 正規化
			dir  = dir.normalize();
			// 半径分に直す
			dir *= radius;
			// 衝突点に足してオフセット後を求める
			position = hitPos + dir;
		}

		f32 height = _pLandScape->getHeight(position, radius * 2.0f);
		if( height != 0.0f )
		{
			if( position._y - radius < height )
			{
				// 衝突あり
				isHit = true;
				// 地面と衝突
				isHitGround = true;
				position._y = height + radius;
			}
		}else{
			
			f32 a = _pLandScape->getHeight(position, radius * 2.0f);
		}

		// この時点でどちらの補正もなければあたってない
		if( !isHit ) {
			break;
		}
	}

	
}

//=============================================================================
//	END OF FILE
//=============================================================================
