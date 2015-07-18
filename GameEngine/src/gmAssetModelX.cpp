//-----------------------------------------------------------------------------
//!
//!	@file	gmAssetModelX.cpp
//!	@brief	3Dモデルアセット
//!	@author	Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//GM_TYPE_IMPLEMENT(AssetModelX, "3Dモデルアセット");

//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
AssetModelX::AssetModelX(void)
: _pReader(NULL)
, _minX( FLT_MAX)
, _maxX(-FLT_MAX)
, _minY( FLT_MAX)
, _maxY(-FLT_MAX)
, _minZ( FLT_MAX)
, _maxZ(-FLT_MAX)
{
}

//-----------------------------------------------------------------------------
//	デストラクタ
//-----------------------------------------------------------------------------
AssetModelX::~AssetModelX(void)
{
	//---- フレームの解放
	for( vector<AssetModelX::Frame*>::iterator itr = _frames.begin(); itr != _frames.end(); )
	{
		AssetModelX::Frame* pFrame = *itr;
		SAFE_DELETE(pFrame);
		itr = _frames.erase(itr);
	}
	//---- マテリアルの解放
	for( vector<AssetModelX::Material*>::iterator itr = _materials.begin(); itr != _materials.end(); )
	{
		AssetModelX::Material* pMaterial = *itr;
		GM_RELEASE(pMaterial);
		itr = _materials.erase(itr);
	}
	//---- マテリアルの解放
	for( vector<AssetModelX::Animation*>::iterator itr = _animations.begin(); itr != _animations.end(); )
	{
		AssetModelX::Animation* pAnimation = *itr;
		SAFE_DELETE(pAnimation);
		itr = _animations.erase(itr);
	}

	delete _pReader;
}

//-----------------------------------------------------------------------------
//	ロード
//!	@param	[in]	fileName	ファイルパス名
//!	@retval	true	成功
//!	@retval	false	失敗
//-----------------------------------------------------------------------------
bool AssetModelX::load(GM_CSTR fileName)
{
	// ファイルパスを取得
	GM_CHAR	driveName[MAX_PATH];
	GM_CHAR	pathName [MAX_PATH];
	_splitpath(fileName, driveName, pathName, NULL, NULL);
	_makepath (_path   , driveName, pathName, NULL, NULL);

	//-------------------------------------------------------------
	// 構文パーサーを作成
	//-------------------------------------------------------------
	_pReader = new AssetModelX::Reader();
	if( _pReader->init(fileName) == false ) {
		return false;
	}

	//-------------------------------------------------------------
	// フレームのルート作成
	//-------------------------------------------------------------
	AssetModelX::Frame*	pFrame;
	{
		pFrame = new AssetModelX::Frame();
	
		_frames.push_back( pFrame );
		s32 index = _frames.size() - 1;
		pFrame->setJointIndex((f32)index);
	}
	//--------------------------------------------------------------
	// ファイル解析
	//--------------------------------------------------------------
	enum PHASE {
		PHASE_ROOT,		// ルート解析中
		PHASE_FRAME,	// Frame		チャンク解析中
	};

	static const s32	NEST_COUNT_MAX = 256;
	PHASE				phase[NEST_COUNT_MAX];
	s32					nest   = 0;

	phase[nest] = PHASE_ROOT;

	//---- ファイルヘッダ解析
	Mesh*				pMesh      = NULL;

	_pReader->getToken();		// "xof"
	_pReader->getToken();		// "0303txt"
	_pReader->getToken();		// "0032"

	//---- ファイルデータ解析
	while( nest >= 0 ) {

		GM_CSTR	token = _pReader->getToken();
		if( token == NULL ) {
			break;
		}

//for( s32 rr=0;rr<nest; rr++ ) GM_TRACE("-");
//GM_TRACE("%s\n", token);

		//---- 【Frame】モデルフレーム
		if( _pReader->compare("Frame") ) {
			AssetModelX::Frame*		p = _loadFrame();
			if( p == NULL ) {
				nest = -1;
				break;
			}
			pFrame->add( p );
			pFrame = p;

			phase[++nest] = PHASE_FRAME;

			continue;
		}

		//---- 【FrameTransformMatrix】フレーム内の関節姿勢行列
		if( _pReader->compare("FrameTransformMatrix") ) {
			_pReader->openChunk();
			
			Matrix	transformMatrix;
			f32		x,y,z,w;
			token = _pReader->getToken(); x = (f32)atof(token);
			token = _pReader->getToken(); y = (f32)atof(token);
			token = _pReader->getToken(); z = (f32)atof(token);
			token = _pReader->getToken(); w = (f32)atof(token);
			transformMatrix._m[0] = Vector4(x, y, z, w);

			token = _pReader->getToken(); x = (f32)atof(token);
			token = _pReader->getToken(); y = (f32)atof(token);
			token = _pReader->getToken(); z = (f32)atof(token);
			token = _pReader->getToken(); w = (f32)atof(token);
			transformMatrix._m[1] = Vector4(x, y, z, w);

			token = _pReader->getToken(); x = (f32)atof(token);
			token = _pReader->getToken(); y = (f32)atof(token);
			token = _pReader->getToken(); z = (f32)atof(token);
			token = _pReader->getToken(); w = (f32)atof(token);
			transformMatrix._m[2] = Vector4(x, y, z, w);

			token = _pReader->getToken(); x = (f32)atof(token);
			token = _pReader->getToken(); y = (f32)atof(token);
			token = _pReader->getToken(); z = (f32)atof(token);
			token = _pReader->getToken(); w = (f32)atof(token);
			transformMatrix._m[3] = Vector4(x, y, z, w);

			pFrame->setTransformMatrix(transformMatrix);

			_pReader->closeChunk();
			continue;
		}

		//---- 【Mesh】モデルメッシュ
		if( _pReader->compare("Mesh") ) {
			pMesh = _loadMesh();
			pMesh->setJointIndex( (s32)(pFrame->getJointIndex()) );
			pFrame->_meshes.push_back(pMesh);
			continue;
		}

		//---- 【Material】マテリアル
		if( _pReader->compare("Material") ) {
			_loadMaterial();
			continue;
		}

		//---- 【AnimationSet】アニメーション
		if( _pReader->compare("AnimationSet") ) {
			_loadAnimation();
			continue;
		}

		if( _pReader->compare("{") ) {
			_pReader->openChunk();
			nest++;
			continue;
		}
		if( _pReader->compare("}") ) {
			if( phase[nest] == PHASE_FRAME ) {
				// ひとつ上の階層に戻る
				pFrame = pFrame->getParent();
			}
			_pReader->closeChunk();
			nest--;
			continue;
		}

		//---- 未使用/未知のチャンクは読み捨てる
		_pReader->skipChunk();
	}

	//-------------------------------------------------------------
	// メッシュデータを最適化
	//-------------------------------------------------------------
	for( u32 f=0; f<_frames.size(); f++ ) {
		AssetModelX::Frame*	pFrame = _frames[f];

		for( u32 m=0; m<pFrame->_meshes.size(); m++ ) {
			AssetModelX::Mesh*	pMesh = pFrame->_meshes[m];
			pMesh->optimize();
		}
	}

	//-------------------------------------------------------------
	// アニメーション関節の関連付け
	//-------------------------------------------------------------
	for( u32 a=0; a<_animations.size(); a++ ) {
		AssetModelX::Animation*		pAnimation = _animations[a];
		
		for( u32 bone=0; bone<pAnimation->_bone.size(); bone++ ) {
			AssetModelX::Animation::Bone*	pBone  = pAnimation->_bone[bone];
		
			AssetModelX::Frame*				pFrame = searchFrame( pBone->getName() );

			if( pFrame ) {
				pBone->_jointIndex = (s32)( pFrame->getJointIndex() );
			}
			else {
				// 関連付けがされない場合は関節番号は -1
				pBone->_jointIndex = -1;

				// MessageBox(NULL, "アニメーション関節の関連付けがされていません.", "警告", MB_OK);
			}
		}
	}
	//=============================================================
	// 頂点入力レイアウトの作成 (頂点宣言)
	//=============================================================
	//	Float3		_position;			// 位置座標　       [f32 x 3] 12 bytes
	//	u8			_blendIndex [4];	// スキニングINDEX  [ u8 x 4]  4 bytes
	//	f32			_blendWeight[4];	// スキニングWEIGHT [f32 x 4] 16 bytes
	//	Float3		_normal;			// 法線　　　       [f32 x 3] 12 bytes
	//	D3DCOLOR	_color;				// カラー　　       [ u8 x 4]  4 bytes
	//	f32			_uv;				// テクスチャUV     [f32 x 2]  8 bytes
	//D3DVERTEXELEMENT9	element[] = {
	//	{0, GM_OFFSET_OF(AssetModelX::GpuVertex, _position   ), D3DDECLTYPE_FLOAT3  , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION    , 0},
	//	{0, GM_OFFSET_OF(AssetModelX::GpuVertex, _blendIndex ), D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
	//	{0, GM_OFFSET_OF(AssetModelX::GpuVertex, _blendWeight), D3DDECLTYPE_FLOAT4  , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT , 0},
	//	{0, GM_OFFSET_OF(AssetModelX::GpuVertex, _normal     ), D3DDECLTYPE_FLOAT3  , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL      , 0},
	//	{0, GM_OFFSET_OF(AssetModelX::GpuVertex, _color      ), D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR       , 0},
	//	{0, GM_OFFSET_OF(AssetModelX::GpuVertex, _uv         ), D3DDECLTYPE_FLOAT2  , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD    , 0},
	//	//---- 定義終端
	//	D3DDECL_END()
	//};

	//IDirect3DDevice9*	pD3DDevice = GmRender()->getD3DDevice();
	//HRESULT	hr = pD3DDevice->CreateVertexDeclaration(&element[0], &_pD3DVertexDecl);
	//if( hr != D3D_OK ) {
	//	MessageBox(NULL, L"頂点入力レイアウトの作成に失敗しました.", L"エラー", MB_OK);

	//	// エラー終了
	//	return false;
	//}

	return true;
}

//-----------------------------------------------------------------------------
//! フレームを移動させる
//!	@param	[in]	frameName	適用するフレーム名
//!	@param	[in]	offset		移動量
//-----------------------------------------------------------------------------
bool AssetModelX::setOffsetByName(GM_CSTR frameName, Vector3& offset)
{
	// フレーム取得
	AssetModelX::Frame* pFrame = searchFrame(frameName);
	if( !pFrame ) return false;
	// 動くかどうかフラグ取得
	if( !pFrame->isDynamic() ){
		// 動くフラグ設定
		pFrame->setDynamic(true);
	}

	// 移動行列作成
	Matrix	offsetMatrix = Matrix::translate(offset);
	// 移動行列設定
	pFrame->setOffsetMatrix(offsetMatrix);

	return true;
}

//-----------------------------------------------------------------------------
//! フレームを回転させる
//!	@param	[in]	frameName	適用するフレーム名
//!	@param	[in]	rotate		回転量(Radian)
//-----------------------------------------------------------------------------
bool AssetModelX::setRotateByName(GM_CSTR frameName, Vector3& rotate)
{
	// フレーム取得
	AssetModelX::Frame* pFrame = searchFrame(frameName);
	if( !pFrame ) return false;
	// 動くかどうかフラグ取得
	if( !pFrame->isDynamic() ){
		// 動くフラグ設定
		pFrame->setDynamic(true);
	}
	// クォータニオン作成
	Quaternion	rotateQuaternion(rotate);
	// 回転行列に変換
	Matrix	rotateMatrix = rotateQuaternion.toRotateMatrix();
	// 回転行列設定
	pFrame->setRotateMatrix(rotateMatrix);
	
	return true;
}

//-----------------------------------------------------------------------------
//! フレームが動くかどうか設定
//!	@param	[in]	frameName	適用するフレーム名
//!	@param	[in]	isDynamic	動くかどうかフラグ
//-----------------------------------------------------------------------------
bool AssetModelX::setDynamicByName(GM_CSTR frameName, bool& isDynamic)
{
	// フレーム取得
	AssetModelX::Frame* pFrame = searchFrame(frameName);
	if( !pFrame ) return false;
	// フラグ設定
	pFrame->setDynamic(isDynamic);
	return true;
}

//-----------------------------------------------------------------------------
//! 動的フレームを持っているかどうか
//-----------------------------------------------------------------------------
bool AssetModelX::haveDynamicFrame() {
	// 自分のフレームにアクティブがあるかどうか
	for( u32 i=0; i<_frames.size(); ++i ) {
		if( _frames.at(i)->isDynamic() ){
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
//! フレームを名前で検索
//-----------------------------------------------------------------------------
AssetModelX::Frame* AssetModelX::searchFrame(GM_CSTR name)
{
	AssetModelX::Frame*	pFrame = NULL;

	for( u32 i=0; i<_frames.size(); i++ ) {
		AssetModelX::Frame*	p = _frames[i];
		
		if( strcmp(p->getName(), name) ) continue;
		
		pFrame = p;
		break;
	}
	return pFrame;
}


//-----------------------------------------------------------------------------
//	フレームの読み込み
//!	@return	確保されたフレームのインデックス番号。-1の場合はメモリ不足で失敗
//-----------------------------------------------------------------------------
AssetModelX::Frame* AssetModelX::_loadFrame(void)
{
	//---- 新規追加
	AssetModelX::Frame*	p = new AssetModelX::Frame();

	_frames.push_back( p );
	s32 jointIndex = _frames.size() - 1;
	
	if( jointIndex == -1 ) {
		delete p;
		return NULL;
	}
	p->setJointIndex( (f32)(jointIndex) );

	//---- 名前を取得
	GM_CSTR	name = _pReader->getToken();
	if( _pReader->compare("{") == false ) {
		// 名前が定義されている場合は登録
		p->setName(name);
	}
	else {
		// 名前がない場合は空文字列 "" を割り当てる
		p->setName(name + 1);
	}
	_pReader->openChunk();
			
	return p;
}

//-----------------------------------------------------------------------------
//	メッシュの読み込み
//!	@return	作成されたオブジェクト。NULLの場合はメモリ不足で失敗
//-----------------------------------------------------------------------------
AssetModelX::Mesh* AssetModelX::_loadMesh(void)
{
	AssetModelX::Mesh*	pMesh = new AssetModelX::Mesh(this);
	if( pMesh == NULL ) {
		return NULL;
	}

	GM_CSTR	token;

	_pReader->openChunk();

	//---- 頂点個数 【n;】
	token = _pReader->getToken();
	s32	vertexCount = atoi(token);

	// 頂点個数に合わせて頂点配列を確保
	pMesh->_vertices.resize(vertexCount);

	//---- 頂点データ(xyz)が個数分存在する【x;y;z;,】
	// 【※注意※】このときの座標系は「左手座標系」
	for( s32 i=0; i<vertexCount; i++ ) {
		token = _pReader->getToken();
		f32	x = static_cast<f32>( atof(token) );
		token = _pReader->getToken();
		f32	y = static_cast<f32>( atof(token) );
		token = _pReader->getToken();
		f32	z = static_cast<f32>( atof(token) );

		// 頂点に書き込み
		pMesh->_vertices[i]._position = Vector3(x, y, z);		
	}

	//---- ポリゴン数【n;】
	token = _pReader->getToken();
	s32	faceCount = atoi(token);
	pMesh->_faces.resize(faceCount);

	//---- 【n角形; index1, index2, index3;,】
	for( s32 i=0; i<faceCount; i++ ) {
		Face&	face = pMesh->_faces[i];

		// n角形
		token = _pReader->getToken();
		s32	indexCount = atoi(token);

		// 配列数をオーバーしているポリゴンはカットする
		// 将来的には三角形分割で対応可能
		if( indexCount > Face::VERTEX_COUNT_MAX ) {
//			GM_TRACE("多角形が発見されました。現時点では未対応です。\n");
			indexCount = Face::VERTEX_COUNT_MAX - 1;
		}
		face._vertexCount = indexCount;
		// 頂点インデックスを取得
		for( s32 n=0; n<indexCount; n++ ) {
			token = _pReader->getToken();
			s32	index = atoi(token);
			face._vertexIndex[n] = index;
		}
	}

	while( token = _pReader->getToken() ) {
		//---- チャンク群の終了
		if( _pReader->compare("}") ) {
			break;
		}
//GM_TRACE("=");
//GM_TRACE("%s\n", token);
		//------------------------------------------------------------
		// 法線
		//------------------------------------------------------------
		if( _pReader->compare("MeshNormals") ) {
			_pReader->openChunk();

			//---- 法線配列数
			token = _pReader->getToken();
			s32	count = atoi(token);
			pMesh->_normals.resize(count);

			// 法線データXYZ
			for( s32 i=0; i<count; i++ ) {
				token = _pReader->getToken();
				f32	x = (f32)atof(token);
				token = _pReader->getToken();
				f32	y = (f32)atof(token);
				token = _pReader->getToken();
				f32	z = (f32)atof(token);

				pMesh->_normals[i] = Vector3(x,y,z);
			}

			//---- 法線インデックス配列数
			token = _pReader->getToken();
			s32	indexCount = atoi(token);

			// 法線インデックスデータ
			for( s32 i=0; i<indexCount; i++ ) {

				token = _pReader->getToken();
				s32	vertexCount = atoi(token);

				for( s32 vi=0; vi<vertexCount; vi++ ) {
					token = _pReader->getToken();
					s32		normalIndex = atoi(token);

					pMesh->_faces[i]._normalIndex[vi] = normalIndex;
				}
			}

			_pReader->closeChunk();
			continue;
		}

		//------------------------------------------------------------
		// テクスチャUV座標
		//------------------------------------------------------------
		if( _pReader->compare("MeshTextureCoords") ) {
			_pReader->openChunk();

			//---- UV個数
			token = _pReader->getToken();
			s32	count = atoi(token);
			// UV座標リスト(頂点列に対応)
			for( s32 i=0; i<count; i++ ) {
				token = _pReader->getToken();
				f32	u = (f32)atof(token);
				token = _pReader->getToken();
				f32	v = (f32)atof(token);

				pMesh->_vertices[i]._uv = Float2(u, v);
			}
			_pReader->closeChunk();
			continue;
		}

		//------------------------------------------------------------
		// テクスチャ座標と法線のデータ（MeshTextureCoordsとMeshNormalsの）
		//------------------------------------------------------------
		if(_pReader->compare("DeclData") ) {
			_pReader->openChunk();

			token = _pReader->getToken();
			signed int elements = atoi(token);
			signed int type[3];

			if(elements > 3) {
				MessageBoxA(NULL, "DeclData contains more than 3 elements, please redo code or change the model.", "WARNING!", MB_OK | MB_ICONERROR);
			}
			
			// 情報わかっているので取得しなくていい
			token = _pReader->getToken();
			token = _pReader->getToken();
			token = _pReader->getToken();
			type[0] = atoi(token);
			token = _pReader->getToken();

			token = _pReader->getToken();
			token = _pReader->getToken();
			token = _pReader->getToken();
			type[1] = atoi(token);
			token = _pReader->getToken();

			if(elements > 2) {
				token = _pReader->getToken();
				token = _pReader->getToken();
				token = _pReader->getToken();
				type[2] = atoi(token);
				token = _pReader->getToken();
			}

			token = _pReader->getToken();
			signed int count = atoi(token);
			//法線 : 3xDWORD タンジェント: 3xDWORD テクスチャ座標 : 2xDWORD = 8x DWORD
			signed int divVal;
			signed int size;
			if(elements == 2) {
				if(type[1] == 6) {
					divVal = 6;
					size = (count / divVal);

					//pMesh->_tangents.resize(size);
				}
				if(type[1] == 5) {
					divVal = 5;
					size = (count / divVal);
				}
			}else {
				divVal = 8;
				size = (count / divVal);

				//pMesh->_tangents.resize(size);
			}			

			pMesh->_normals.resize(size);
			
			signed int elementNumber = 0;

			float x, y, z;
			x = y = z = 0.0f;
			float tx, ty, tz;
			tx = ty = tz = 0.0f;
			float u, v;
			u = v = 0.0f;
			signed int arrNum = 0;
			//	順番は法線の x->y->z タンジェントの x->y->z テクスチャの u->v
			for(signed int i = 0; i < count; ++i) {

				if(i == count-1) {
					int a = 0;
				}

				token = _pReader->getToken();
				DWORD val = strtoul(token, 0, 0);
				switch(elementNumber) {
				case 0:	//---- 0 = x
					x = *(float*)&val;
					break;
				case 1:	//---- 1 = y
					y = *(float*)&val;
					break;
				case 2:	//---- 2 = z
					z = *(float*)&val;
					pMesh->_normals[arrNum] = Vector3(x,y,z);
					x = y = z = 0.0f;
					break;
				case 3:	//---- 3 = tx
					tx = *(float*)&val;
					break;
				case 4:	//---- 4 = ty
					ty = *(float*)&val;
					break;
				case 5:	//---- 5 = tz
					tz = *(float*)&val;
					//pMesh->_tangents[arrNum] = Vector3(tx,ty,tz);
					tx = ty = tz = 0.0f;
					break;
				case 6:	//---- 6 = u
					u = *(float*)&val;
					break;
				case 7:	//---- 4 = v
					v = *(float*)&val;

					pMesh->_vertices[arrNum]._uv = Float2(u, v);
					u = v = 0.0f;
					break;
				};
				elementNumber++;
				//---- タンジェントの要素ない場合テクスチャ座標にスキップする
				if(elements == 2 && divVal == 5 && elementNumber == 3) {
					elementNumber = 6;
				}
				//---- テクスチャ座標の要素ない場合は法線にスキップする
				if(elements == 2 && divVal == 6 && elementNumber == 6) {
					elementNumber = 8;
				}

				if(elementNumber == 8) {
					arrNum++;
					elementNumber = 0;
				}
			}

			signed int faceCount = pMesh->_faces.size();
			for(int i = 0; i < faceCount; ++i) {
				for(int j = 0; j < pMesh->_faces[i]._vertexCount; ++j) {
					signed int normIdx = pMesh->_faces[i]._vertexIndex[j];
					pMesh->_faces[i]._normalIndex[j] = normIdx;
				}
			}

			_pReader->closeChunk();
			continue;
		}

		//------------------------------------------------------------
		// スキンアニメーション
		//------------------------------------------------------------
		if( _pReader->compare("XSkinMeshHeader") ) {	//---- ヘッダ
			pMesh->loadSkinMeshHeader(_pReader);
			continue;
		}

		if( _pReader->compare("SkinWeights") ) {		//---- スキンウェイト
			pMesh->loadSkinWeight(_pReader);
			continue;
		}

		//------------------------------------------------------------
		// マテリアル情報（テクスチャ名）
		//------------------------------------------------------------
		if( _pReader->compare("MeshMaterialList") ) {
			_pReader->openChunk();

			//---- マテリアル個数
			token = _pReader->getToken();
			s32	materialCount = atoi(token);

			//---- マテリアル番号個数
			token = _pReader->getToken();
			s32	count = atoi(token);

			//s32	faceCount = pMesh->_faces.getCount();
			s32	faceCount = pMesh->_faces.size();
		
			// マテリアル番号リスト
			if( count == 1 ) {
				token = _pReader->getToken();
				s32	index = atoi(token);

				for( s32 i=0; i<faceCount; i++ ) {
				
					// ポリゴンにマテリアル番号を設定
					pMesh->_faces[i]._material = index;
				}
			}
			else {
				for( s32 i=0; i<count; i++ ) {
					token = _pReader->getToken();
					s32	index = atoi(token);
				
					// ポリゴンにマテリアル番号を設定
					pMesh->_faces[i]._material = index;
				}
			}
			//---- マテリアル
			for( s32 i=0; i<materialCount; i++ ) {
				// マテリアルの最初のトークンを読み込んでおく
				_pReader->getToken();
				
				AssetModelX::Material*	pMaterial = _loadMaterial();
				//pMesh->_materials.push(pMaterial);
				pMesh->_materials.push_back(pMaterial);
			}
			_pReader->getToken();		// 直前のマテリアルの "}"をスキップしておく

			_pReader->closeChunk();
			continue;
		}

		//---- 未知のチャンクは読み捨てる
		_pReader->skipChunk();
	}
	_pReader->closeChunk();

	return pMesh;
}

//-----------------------------------------------------------------------------
//	マテリアル読み込み
//!	@return	作成されたオブジェクト。NULLの場合はメモリ不足で失敗
//!	@note	実体宣言とラベル指定の場合で挙動が異なり、ラベルの場合は
//!			既存のマテリアルから名前検索して該当するマテリアルを返す。
//-----------------------------------------------------------------------------
AssetModelX::Material* AssetModelX::_loadMaterial(void)
{
	Material*	pMaterial = NULL;

	//---- ラベル
	//	【書式】	{ マテリアル名 }
	if( _pReader->compare("{") ) {
		_pReader->openChunk();

		GM_CSTR	materialName = _pReader->getToken();
		//------------------------------------------------------------
		// マテリアルリストから名前で検索
		//------------------------------------------------------------
		for( u32 i=0; i<_materials.size(); i++ ) {
			AssetModelX::Material* p = _materials[i];
			if( strcmp(materialName, p->getName() ) != 0 ) continue;
		
			pMaterial = p;
			break;
		}
		if( pMaterial == NULL ) {
			MessageBox(NULL, "マテリアル名に対応するデータが検出できませんでした。", NULL, MB_OK);
		}
		_pReader->closeChunk();
	}
	else {
		//------------------------------------------------------------
		// マテリアルを新規取得
		//------------------------------------------------------------
		GM_CSTR	token = _pReader->getToken();
		
		pMaterial = new AssetModelX::Material();

		//---- マテリアル名を取得
		if( _pReader->compare("{") ) {	//---- マテリアル名なし
			pMaterial->setName("");
		}
		else {							//---- マテリアル名あり
			// マテリアル名の定義がある場合は名前登録する
			pMaterial->setName(token);
		}

		//------------------------------------------------------------
		// Materialチャンクの読取り
		//------------------------------------------------------------
		_pReader->openChunk();
		{
			//---- マテリアル情報
			f32	r,g,b,a;

			// faceColor
			token = _pReader->getToken(); r = (f32)atof(token);
			token = _pReader->getToken(); g = (f32)atof(token);
			token = _pReader->getToken(); b = (f32)atof(token);
			token = _pReader->getToken(); a = (f32)atof(token);

			//D3DCOLOR	faceColor = D3DCOLOR_COLORVALUE(r, g, b, a);
			Vector4	faceColor = Vector4(r, g, b, a);
			pMaterial->setDiffuseColor(faceColor);

			f32 specPow;
			// specularPower
			token = _pReader->getToken(); specPow = (f32)atof(token);
			pMaterial->setShininess(specPow);
			// specularColor
			token = _pReader->getToken(); r = (f32)atof(token);
			token = _pReader->getToken(); g = (f32)atof(token);
			token = _pReader->getToken(); b = (f32)atof(token);
			Vector4	specularColor = Vector4(r, g, b, a);
			pMaterial->setSpecularColor(specularColor);
			// emissiveColor
			token = _pReader->getToken(); r = (f32)atof(token);
			token = _pReader->getToken(); g = (f32)atof(token);
			token = _pReader->getToken(); b = (f32)atof(token);
			Vector4	emissiveColor = Vector4(r, g, b, a);
			pMaterial->setEmissiveColor(emissiveColor);

			//---- TextureFileNameチャンク
			token = _pReader->getToken();
			if( _pReader->compare("TextureFilename") || _pReader->compare("TextureFileName") ) {
				_pReader->openChunk();
				{
					// テクスチャファイル名文字列
					// 前後に ""記号がつく
					token = _pReader->getToken();
					
					s32		length   = strlen(token);
					GM_STR	filePath = new char[ length ];	// ” + 文字列 + ”
					
					// 両端の""記号を除去する
					strcpy(filePath, token + 1);
					filePath[length - 2] = '\0';

					// Xファイルの位置からの相対位置を作成
					GM_CHAR		fullPath[MAX_PATH];
					_makepath(fullPath, NULL, _path, filePath, NULL);

					// テクスチャファイルの読み込み
					Texture*	pTexture = new Texture();
					if( !pTexture->loadAndTexImage2D(fullPath, true) )
					{
						ASSERTMSG(false, "ディフューズテクスチャの読み込み失敗");
					}
					pMaterial->setTexture( pTexture );
					
					// スペキュラマップの読み込み
					string newPath = fullPath;
					s32 under = newPath.rfind("_d");
					if( under > 0 ){
						under++; // _の部分なので次へ
						newPath[under] = 's';
						Texture* pSpecularMap = new Texture();
						if( pSpecularMap->loadAndTexImage2D(newPath.c_str(), true) ){
							pMaterial->setSpecularMap(pSpecularMap);
						}else{
							SAFE_DELETE(pSpecularMap);
						}
					}

					under = newPath.rfind("_s");
					if( under > 0 ){
						under++;
						newPath[under] = 'n';
						Texture* pNormalMap = new Texture();
						if( pNormalMap->loadAndTexImage2D(newPath.c_str(), true) ) {
							pMaterial->setNormalMap(pNormalMap);
						}else{
							SAFE_DELETE(pNormalMap);
						}
					}

					delete [] filePath;
				}
				_pReader->closeChunk();
				_pReader->getToken();		// 連続で括弧を閉じるため、トークンを進めておく
			}			
			//-----------------------------------------------------------
			// マテリアルを全体のリストに登録
			//-----------------------------------------------------------
			//_materials.push(pMaterial);
			_materials.push_back(pMaterial);
		}
		_pReader->closeChunk();
	}
	return pMaterial;
}

//-----------------------------------------------------------------------------
//	アニメーション読み込み
//-----------------------------------------------------------------------------
AssetModelX::Animation* AssetModelX::_loadAnimation(void)
{
	//---- 新規登録
	AssetModelX::Animation*	pAnimation = new AssetModelX::Animation();
	if( pAnimation == NULL ) {
		return NULL;
	}
	_animations.push_back(pAnimation);

	//-------------------------------------------------------------
	// アニメーション名を取得
	//-------------------------------------------------------------
	{
		GM_CSTR	animationSetName = _pReader->getToken();	// アニメーション名
		pAnimation->setName(animationSetName);
	}

	//-------------------------------------------------------------
	// アニメーションキーフレームデータを取得
	//-------------------------------------------------------------
	_pReader->openChunk();
	for( ;; ) {
		GM_CSTR	token = _pReader->getToken();
		if( _pReader->compare("}") ) {				//---- 終端
			_pReader->closeChunk();
			break;
		}

		//---- アニメーションキーフレームデータ
		if( _pReader->compare("Animation") ) {
			GM_CSTR	animationName = _pReader->getToken();

			pAnimation->_loadKeyData(_pReader);
			continue;
		}

		//---- 未使用/未知のチャンクは読み捨てる
		_pReader->skipChunk();
	}

	return pAnimation;
}

//=============================================================================
//! AssetModelX::Name			名前文字列データ
//=============================================================================

//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
AssetModelX::Name::Name(GM_CSTR name)
{
	// 文字列バッファを溢れないように末尾にも \0 を入れておく。
	_name[0] = '\0';						// 先頭
	_name[ sizeof(_name) - 1 ] = '\0';		// 末尾

	setName(name);
}

//-----------------------------------------------------------------------------
//	文字列を設定
//-----------------------------------------------------------------------------
void AssetModelX::Name::setName(GM_CSTR name)
{
	// コピーサイズを計算。 \0を含むサイズとバッファサイズで小さい方を使用。
	// ※ この場合、バッファの末尾は壊さないようにする(1文字少なくコピー)。
	//    バッファオーバーフローの対策にコンストラクタで \0をセットしている。
	s32	copySize = min(strlen(name) + 1,  sizeof(_name) - 1);
	memcpy(_name, name, copySize);
}

//-----------------------------------------------------------------------------
//	文字列を取得
//-----------------------------------------------------------------------------
GM_CSTR AssetModelX::Name::getName(void) const
{
	return _name;
}


//=============================================================================
//	AssetModelX::SkinWeight		スキンウェイト
//=============================================================================

//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
AssetModelX::SkinWeight::SkinWeight(void)
: _offsetMatrix(Matrix::IDENTITY)
{
}

//-----------------------------------------------------------------------------
//	デストラクタ
//-----------------------------------------------------------------------------
AssetModelX::SkinWeight::~SkinWeight(void)
{
}

//=============================================================================
//	AssetModelX::Reader			構文パーサー
//=============================================================================

//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
AssetModelX::Reader::Reader(void)
: _pCurrent(NULL)
, _pBuffer(NULL)
, _bufferSize(0)
{
}

//-----------------------------------------------------------------------------
//	デストラクタ
//-----------------------------------------------------------------------------
AssetModelX::Reader::~Reader(void)
{
	cleanup();
}

//-----------------------------------------------------------------------------
//	初期化
//-----------------------------------------------------------------------------
bool AssetModelX::Reader::init(GM_CSTR fileName)
{
	//-------------------------------------------------------------
	// ファイルを読み込む
	//-------------------------------------------------------------
	HANDLE	hFile = CreateFileA(
						fileName,
						GENERIC_READ,
						FILE_SHARE_READ,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_READONLY,
						NULL);

	if( hFile == INVALID_HANDLE_VALUE ) {
		MessageBoxA(NULL, fileName , "ファイル読み込みに失敗しました。", MB_OK);
		return false;
	}

	// ファイルサイズを取得
	u32		size    = GetFileSize(hFile, NULL);
	_pBuffer = new GM_CHAR [ size + 1 ];
	_pBuffer[size] = ' ';

	_bufferSize = size + 1;
	// メモリ上のバッファに一括読み込み
	DWORD	readSize;
	ReadFile(hFile, _pBuffer, size, &readSize, NULL);

	// ファイルを閉じる
	CloseHandle(hFile);


	_pCurrent = _pBuffer;
	return true;
}

//-----------------------------------------------------------------------------
//	解放
//-----------------------------------------------------------------------------
void AssetModelX::Reader::cleanup(void)
{
	if( _pBuffer ) {
		delete [] _pBuffer;
		_pBuffer = NULL;
	}
}

//-----------------------------------------------------------------------------
//! 次のトークンを１つ取得
//!	@return トークン文字列の先頭（次回getToken()を実行すると消滅する一時的なもの）
//!	@return	NULLならファイル終端
//-----------------------------------------------------------------------------
GM_CSTR AssetModelX::Reader::getToken(void)
{
	GM_STR&	p     = _pCurrent;
	GM_STR	pTail = _pBuffer + _bufferSize;

	if( p >= pTail ) return NULL;

	//---- 区切り記号、空白、改行をスキップ
	while(
		*p == ' ' || *p == '\t' || *p == 0x0a || *p == 0x0d || *p == ';' || *p == ','
	) {
		p++;
		if( p >= pTail ) return NULL;
	}

	s32	count = 0;

	//---- コメント文の検知
	// コメントは改行まで読み飛ばす
	if( p[0] == '/' && p[1] == '/' ) {
		while(
			!(*p == 0x0a || *p == 0x0d)
		) {
			p++;
			if( p >= pTail ) return NULL;
		}

		//---- 区切り記号、空白、改行をスキップ
		while(
			*p == ' ' || *p == '\t' || *p == 0x0a || *p == 0x0d || *p == ';' || *p == ','
		) {
			p++;
			if( p >= pTail ) return NULL;
		}
	}

	//---- 次の区切りまで検索
	do {
		_token[count] = *p;
		
		// 中括弧はだけ_tokenに入れるためbreak?(下の条件文に入れても同じじゃない？)
		// 下にいれたらif(count == 0)いらなくなるんじゃない？
		if( *p == '{' || *p == '}' ) {
			break;	
		}
		count++;
		p++;
		if( p >= pTail ) return NULL;

	} while ( !(
		 *p == ' ' || *p == '\t' || *p == 0x0a || *p == 0x0d || *p == ';' || *p == ','
	) );

	if( count == 0 ) {
		count++;
		p++;
	}
	_token[count]=  '\0';

	return _token;
}

//-----------------------------------------------------------------------------
//	現在のトークンと文字列比較
//-----------------------------------------------------------------------------
bool AssetModelX::Reader::compare(GM_CSTR token)
{
	return strncmp(_token, token, strlen(_token)) == 0;
}

//-----------------------------------------------------------------------------
//	チャンクをスキップ
//!	@retval	true	成功（正常終了）
//!	@retval	false	失敗（エラー終了）
//!
//!	{}で囲まれたチャンクデータを内容にかかわらずスキップして読み捨てます。
//-----------------------------------------------------------------------------
bool AssetModelX::Reader::skipChunk(void)
{
	//----- 最初のチャンクオープンを検索
	if( searchNextToken("{") == false ) {
		return false;
	}

	//---- チャンクを閉じるまで検索
	s32	nest = 1;
	do {
		GM_CSTR token = getToken();

		// トークンが途切れたらエラー終了
		if( token == NULL ) {
			return false;
		}

		// 入れ子構造（ネスト）もカウントして同一階層が閉じるまで検索
		if     ( compare("{") ) nest++;
		else if( compare("}") ) nest--;

	} while( 0 < nest );

	// 成功
	return true;
}

//-----------------------------------------------------------------------------
//	チャンクを開く
//-----------------------------------------------------------------------------
bool AssetModelX::Reader::openChunk(void)
{
	return searchNextToken("{");
}

//-----------------------------------------------------------------------------
//	チャンクを閉じる
//-----------------------------------------------------------------------------
bool AssetModelX::Reader::closeChunk(void)
{
	return searchNextToken("}");
}

//-----------------------------------------------------------------------------
//	次のトークンの出現まで進める
//-----------------------------------------------------------------------------
bool AssetModelX::Reader::searchNextToken(GM_CSTR token)
{
	while( compare(token) == false ) {
		if( getToken() == NULL ) {
			return false;
		}
	}
	return true;
}

//=============================================================================
//	AssetModelX::Material		マテリアルデータ
//=============================================================================

//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
AssetModelX::Material::Material(void)
: _diffuseColor ( 255,255,255,255 )
, _pTexture		(nullptr)
, _pSpecularMap (nullptr)
, _pNormalMap	(nullptr)
{
}

//-----------------------------------------------------------------------------
//	デストラクタ (直接呼び出しを禁止する)
//-----------------------------------------------------------------------------
AssetModelX::Material::~Material(void)
{
	SAFE_DELETE(_pTexture);
	SAFE_DELETE(_pSpecularMap);
	SAFE_DELETE(_pNormalMap);
}

//-----------------------------------------------------------------------------
//	ディフューズ色を取得
//-----------------------------------------------------------------------------
Vector4 AssetModelX::Material::getDiffuseColor(void) const
{
	return _diffuseColor;
}

//-----------------------------------------------------------------------------
//	スペキュラ色を取得
//-----------------------------------------------------------------------------
Vector4 AssetModelX::Material::getSpecularColor(void) const
{
	return _specularColor;
}

//-----------------------------------------------------------------------------
//	自己照明色を取得
//-----------------------------------------------------------------------------
Vector4 AssetModelX::Material::getEmissiveColor(void) const
{
	return _emissiveColor;
}

//-----------------------------------------------------------------------------
//	パワー値を取得
//-----------------------------------------------------------------------------
float AssetModelX::Material::getShininess(void) const
{
	return _shininess;
}
	


//-----------------------------------------------------------------------------
//	テクスチャを取得
//-----------------------------------------------------------------------------
Texture* AssetModelX::Material::getTexture(void) const
{
	return _pTexture;
}

//-----------------------------------------------------------------------------
//	スペキュラマップを取得
//-----------------------------------------------------------------------------
Texture* AssetModelX::Material::getSpecularMap(void) const
{
	return _pSpecularMap;
}

//-----------------------------------------------------------------------------
//	ノーマルマップを取得
//-----------------------------------------------------------------------------
Texture* AssetModelX::Material::getNormalMap(void) const
{
	return _pNormalMap;
}

//-----------------------------------------------------------------------------
//	ディフューズ色を設定
//-----------------------------------------------------------------------------
void AssetModelX::Material::setDiffuseColor(Vector4 color)
{
	_diffuseColor = color;
}


//-----------------------------------------------------------------------------
//	スペキュラ色を設定
//-----------------------------------------------------------------------------
void AssetModelX::Material::setSpecularColor(Vector4 color)
{
	_specularColor = color;
}

//-----------------------------------------------------------------------------
//	自己照明色を設定
//-----------------------------------------------------------------------------
void AssetModelX::Material::setEmissiveColor(Vector4 color)
{
	_emissiveColor = color;
}

//-----------------------------------------------------------------------------
//	パワー値を設定
//-----------------------------------------------------------------------------
void AssetModelX::Material::setShininess(f32 shininess)
{
	_shininess = shininess;
}


//-----------------------------------------------------------------------------
//	テクスチャを設定
//-----------------------------------------------------------------------------
void AssetModelX::Material::setTexture(Texture* pTexture)
{
	_pTexture = pTexture;
}

//-----------------------------------------------------------------------------
//	スペキュラマップを設定
//-----------------------------------------------------------------------------
void AssetModelX::Material::setSpecularMap(Texture* pSpecularMap)
{
	_pSpecularMap = pSpecularMap;
}

//-----------------------------------------------------------------------------
//	ノーマルマップを設定
//-----------------------------------------------------------------------------
void AssetModelX::Material::setNormalMap(Texture* pNormalMap)
{
	_pNormalMap = pNormalMap;
}

//-----------------------------------------------------------------------------
//! シェ―ダのテクスチャを設定
//-----------------------------------------------------------------------------
void AssetModelX::Material::BindTextures()
{
	// 現在使用中のシェ―ダ取得
	GLuint shader   = GmShader()->getShaderProgram(SystemShader::SHADER_TEXTURE);
	// シェ―ダテクスチャハンドル取得
	GLuint hTexture = glGetUniformLocation(shader, "BaseTexture");
	// テクスチャ０を有効化
	glActiveTexture(GL_TEXTURE0);
	// テクスチャハンドル取得
	GLuint	textureHandle;
	// テクスチャがなければ
	if( _pTexture != nullptr ){
			textureHandle = _pTexture->getTextureHandle();
	}else{
			textureHandle = GmShader()->getNullTexHandle();
	}
	// テクスチャ割り当て
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	// ハンドルに番号をひも付け
	glUniform1i(hTexture, 0);
	// テクスチャの品質設定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
		
	GLuint hSpecular = glGetUniformLocation(shader, "SpecularMap");
	glActiveTexture(GL_TEXTURE1);
	GLuint	specularHandle;
	// テクスチャがなければ
	if( _pSpecularMap != nullptr ) {
		specularHandle = _pSpecularMap->getTextureHandle();
	}else{
		specularHandle = GmShader()->getNullSpecHandle();
	}
	glBindTexture(GL_TEXTURE_2D, specularHandle);
	// ハンドルに番号をひも付け
	glUniform1i(hSpecular, 1);
	// テクスチャの品質設定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
	GLuint hNormal = glGetUniformLocation(shader, "NormalMap");
	glActiveTexture(GL_TEXTURE2);
	GLuint	normalHandle;
	if( _pNormalMap != nullptr ) {
		normalHandle = _pNormalMap->getTextureHandle();
	}else{
		normalHandle = GmShader()->getNullNormHandle();
	}
	glBindTexture(GL_TEXTURE_2D, normalHandle);
	// ハンドルに番号をひも付け
	glUniform1i(hNormal, 2);
	// テクスチャの品質設定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//-----------------------------------------------------------------------------
//! シェ―ダのテクスチャの設定を解除
//-----------------------------------------------------------------------------
void AssetModelX::Material::UnBindTextures()
{
	Texture::set(nullptr);
}


//=============================================================================
//	AssetModelX::Animation
//=============================================================================

//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
AssetModelX::Animation::~Animation(void)
{
	//---- ボーンの解放
	vector<AssetModelX::Animation::Bone*>::iterator itr;
	for( itr = _bone.begin(); itr != _bone.end(); )
	{
		AssetModelX::Animation::Bone* pBone = *itr;
		SAFE_DELETE(pBone);
		itr = _bone.erase(itr);
	}
}

//-----------------------------------------------------------------------------
//	ボーン/キーフレーム情報を読み込み
//-----------------------------------------------------------------------------
bool AssetModelX::Animation::_loadKeyData(AssetModelX::Reader* pReader)
{
	pReader->openChunk();

	// ボーン生成（解放はアニメーションクラスが行なう）
	AssetModelX::Animation::Bone*	pBone = new AssetModelX::Animation::Bone();
	_bone.push_back(pBone);

	//--------------------------------------------------------------
	// アニメーショチャンク内の解析
	//--------------------------------------------------------------
	for( ;; ) {
		GM_CSTR		token = pReader->getToken();

		//---- 終端チェック
		if( pReader->compare("}") ) {
			pReader->closeChunk();
			break;
		}

		//---- Frame関節名	{名前}
		if( pReader->compare("{") ) {
			pReader->openChunk();
			GM_CSTR	jointName = pReader->getToken();

			pBone->setName(jointName);

			pReader->closeChunk();
			continue;
		}

		//---- キーデータ AnimationKey
		if( pReader->compare("AnimationKey") ) {
			pReader->openChunk();

			// キーの種類
			s32		keyType  = atoi( pReader->getToken() );

			// キーの個数
			s32		keyCount = atoi( pReader->getToken() );

			// キーデータ配列
			for( s32 i=0; i<keyCount; i++ ) {
				// キーフレーム番号
				s32		t = atoi( pReader->getToken() );
				_totalTime = max(_totalTime, t);

				//----------------------------------------------------------
				// キーデータを読み込む（Typeによってサイズと内容は異なる）
				//----------------------------------------------------------
				s32		dataCount   = atoi( pReader->getToken() );		// データ個数

				f32		data[16];	// 取りうる最大個数は行列の16要素
				for( s32 d=0; d<dataCount; d++ ) {
					data[d] = (f32)atof( pReader->getToken() );
				}

				//---- キーに反映
				switch( keyType ) {
				case 0:		// 回転クォータニオン	[Quaternion]
					// Xファイルのクォータニオンは(w, x, y, z)の順で並んでいるため、並び替えてロードする
					{
						Quaternion	rotation = Quaternion(data[1], data[2], data[3], data[0]).normalize();
						pBone->_keyRotations.push_back( AssetModelX::Animation::Key<Quaternion>(t, rotation) );
					}
					break;
				case 1:		// スケール			[Vector3]
					{
						Vector3	scale = Vector3(data[0], data[1], data[2]);
						pBone->_keyScales.push_back( AssetModelX::Animation::Key<Vector3>(t, scale) );
					}
					break;
				case 2:		// 位置				[Vector3]
					{
						Vector3	position = Vector3(data[0], data[1], data[2]);
						pBone->_keyPositions.push_back( AssetModelX::Animation::Key<Vector3>(t, position) );
					}
					break;
				case 4:		// 行列				[Matrix]
					{
						Matrix	m( *(Matrix*)data );
						
						// 行列は「位置・スケール・回転」の３要素に分解
						f32	sx = Vector3( m._m[0] ).length();
						f32	sy = Vector3( m._m[1] ).length();
						f32	sz = Vector3( m._m[2] ).length();
						Vector3	scale    = Vector3(sx, sy, sz);

						Vector3	position = m._m[3];

						m._m[0] = Vector4( Vector3(m._m[0]).normalize(), 0.0f );
						m._m[1] = Vector4( Vector3(m._m[1]).normalize(), 0.0f );
						m._m[2] = Vector4( Vector3(m._m[2]).normalize(), 0.0f );
						m._m[3] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

						Quaternion	rotation;
						bool	success = rotation.fromRotateMatrix(m);
						// ここで失敗した場合には何も処理しない。
						// キーデータには最初からデフォルト値 (位置(0,0,0) スケール(0,0,0) 回転なし)が入っている
						ASSERTMSG(success, "行列変換結果が不正です。");
						// クォータニオンは正規化しておく
						rotation = rotation.normalize();

						//---- キーフレームとして保存
						pBone->_keyRotations.push_back( AssetModelX::Animation::Key<Quaternion>(t, rotation) );
						pBone->_keyScales   .push_back( AssetModelX::Animation::Key<Vector3   >(t, scale   ) );
						pBone->_keyPositions.push_back( AssetModelX::Animation::Key<Vector3   >(t, position) );
					}
					break;
				default:
					MessageBox(NULL, "未知のアニメーションキーTYPEです。", "ファイル解析エラー", MB_OK);
					break;
				}
			}

			pReader->closeChunk();
			continue;
		}

		//---- キー補間設定 AnimationOptions
		if( pReader->compare("AnimationOptions") ) {
			pReader->openChunk();
				// 第１パラメータ = リピート(0)/非リピート(1);
				token = pReader->getToken();
				bool	repeat = (token[0] == '0');

				// 第２パラメータ = スプライン補間(0)/線形補間(1);
				token = pReader->getToken();
				bool	linear = (token[0] == '0');
			pReader->closeChunk();
			continue;
		}

		//---- 未使用/未知のチャンクは読み捨てる
		pReader->skipChunk();
	}
	// アニメーションチャンク終了
	pReader->closeChunk();

	return true;
}


//=============================================================================
//	AssetModelX::Animation::Bone
//=============================================================================

//-----------------------------------------------------------------------------
//	回転を取得(補間あり)
//-----------------------------------------------------------------------------
Quaternion AssetModelX::Animation::Bone::getRotation(f32 frameTime) const
{
	//-------------------------------------------------------------
	// 補間対象の区間を検索
	//-------------------------------------------------------------
	const AssetModelX::Animation::Key<Quaternion>* 	pKeyOld = &_keyRotations[0];
	const AssetModelX::Animation::Key<Quaternion>* 	pKey    = &_keyRotations[0];

	for( u32 i=1; i<_keyRotations.size(); i++ ) {
		const AssetModelX::Animation::Key<Quaternion>*	p = &_keyRotations[i];

		pKeyOld = pKey;
		pKey    = p;

		if( p->_time > frameTime ) {
			break;
		}
	}

	//-------------------------------------------------------------
	//	回転姿勢をクォータニオン補間
	//-------------------------------------------------------------
	Quaternion	rotation;
	if( pKeyOld->_time <= frameTime && frameTime <= pKey->_time ) {
		// 区間内の位置を0.0～1.0でパラメータtを求める
		f32	range = (f32)(pKey->_time - pKeyOld->_time);
		f32	t = 0.0f;

		if( range > 0.0f ) {	// 分母が0除算になるのを防止
			t = (frameTime - (f32)pKeyOld->_time) / range;
		}
		rotation = Quaternion::slerp(pKeyOld->_data, pKey->_data, t);
	}
	else {
		// 補間区間にない場合は補間なし
		rotation = pKeyOld->_data;
	}

	return rotation;
}

//-----------------------------------------------------------------------------
//	スケールを取得(補間あり)
//-----------------------------------------------------------------------------
Vector3 AssetModelX::Animation::Bone::getScale(f32 frameTime) const
{
	//-------------------------------------------------------------
	// 補間対象の区間を検索
	//-------------------------------------------------------------
	const AssetModelX::Animation::Key<Vector3>* 	pKeyOld = &_keyScales[0];
	const AssetModelX::Animation::Key<Vector3>* 	pKey    = &_keyScales[0];

	for( u32 i=1; i<_keyScales.size(); i++ ) {
		const AssetModelX::Animation::Key<Vector3>*	p = &_keyScales[i];

		pKeyOld = pKey;
		pKey    = p;

		if( p->_time > frameTime ) {
			break;
		}
	}

	//-------------------------------------------------------------
	//	位置を線形補間 (さらに高品質にするためにはスプライン補間にする)
	//-------------------------------------------------------------
	Vector3	scale;
	if( pKeyOld->_time <= frameTime && frameTime <= pKey->_time ) {
		// 区間内の位置を0.0～1.0でパラメータtを求める
		f32	range = (f32)(pKey->_time - pKeyOld->_time);
		f32	t = 0.0f;

		if( range > 0.0f ) {	// 分母が0除算になるのを防止
			t = (frameTime - (f32)pKeyOld->_time) / range;
		}
		scale = pKeyOld->_data * (1.0f - t) + pKey->_data * t;
	}
	else {
		// 補間区間にない場合は補間なし
		scale = pKeyOld->_data;
	}

	return scale;
}

//-----------------------------------------------------------------------------
//	位置を取得(補間あり)
//-----------------------------------------------------------------------------
Vector3 AssetModelX::Animation::Bone::getPosition(f32 frameTime) const
{
	//-------------------------------------------------------------
	// 補間対象の区間を検索
	//-------------------------------------------------------------
	const AssetModelX::Animation::Key<Vector3>* pKeyOld = &_keyPositions[0];
	const AssetModelX::Animation::Key<Vector3>* pKey    = &_keyPositions[0];

	for( u32 i=1; i<_keyPositions.size(); i++ ) {
		const AssetModelX::Animation::Key<Vector3>*	p = &_keyPositions[i];

		pKeyOld = pKey;
		pKey    = p;

		if( p->_time > frameTime ) {
			break;
		}
	}

	//-------------------------------------------------------------
	//	位置を線形補間 (さらに高品質にするためにはスプライン補間にする)
	//-------------------------------------------------------------
	Vector3	position;
	if( pKeyOld->_time <= frameTime && frameTime <= pKey->_time ) {
		// 区間内の位置を0.0～1.0でパラメータtを求める
		f32	range = (f32)(pKey->_time - pKeyOld->_time);
		f32	t = 0.0f;

		if( range > 0.0f ) {	// 分母が0除算になるのを防止
			t = (frameTime - (f32)pKeyOld->_time) / range;
		}
		position = pKeyOld->_data * (1.0f - t) + pKey->_data * t;
	}
	else {
		// 補間区間にない場合は補間なし
		position = pKeyOld->_data;
	}

	return position;
}

//=============================================================================
//	AssetModelX::Mesh	メッシュデータ
//=============================================================================

//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
AssetModelX::Mesh::Mesh(AssetModelX* pParent)
: _pParent(pParent)					// 親のモデル
, _pVertexBuffer(0)					// 頂点バッファ
, _pIndexBuffer (0)					// インデックスバッファ
, _skinWeightsPerVertexCountMax(0)	// メッシュ内の頂点に影響するトランスフォームの最大数。
, _SkinWeightsPerFaceCountMax(0)	// 任意の面の 3 つの頂点に影響する一意のトランスフォームの最大数。
, _myFrameJointIndex(0)				// 自分の属すフレーム番号
, _isDynamic(false)					// 動くかどうか
{
}

//-----------------------------------------------------------------------------
//	デストラクタ
//-----------------------------------------------------------------------------
AssetModelX::Mesh::~Mesh(void)
{
	glDeleteBuffers(1, &_pVertexBuffer);
	glDeleteBuffers(1, &_pIndexBuffer);

	//---- スキンウェイトの解放
	for(vector<AssetModelX::SkinWeight*>::iterator itr = _skinWeights.begin(); itr != _skinWeights.end(); )
	{
		AssetModelX::SkinWeight* pSkinWeight = *itr;
		SAFE_DELETE(pSkinWeight);
		itr = _skinWeights.erase(itr);
	}
}

//---- ポリゴンのクイックソート用コールバック関数
int	sortFunc(const void* p1, const void* p2)
{
	AssetModelX::Face*	pFace1 = (AssetModelX::Face*)p1;
	AssetModelX::Face*	pFace2 = (AssetModelX::Face*)p2;

	if(pFace1->_material == pFace2->_material) return 0;
	return (pFace1->_material < pFace2->_material) ? -1 : +1;
}

//-----------------------------------------------------------------------------
//!	動くフラグ設定
//! ※メモリマッピングするので呼ぶ回数は少なくする
//-----------------------------------------------------------------------------
void AssetModelX::Mesh::setDynamic(bool isDynamic)
{
	// 渡された値が同じなら変更しない
	if(_isDynamic == isDynamic ) return;
	_isDynamic = isDynamic;

	// GL_ARRAY_BUFFERに_pVertexbufferを割り当て
	glBindBuffer(GL_ARRAY_BUFFER, _pVertexBuffer);
	AssetModelX::GpuVertex*	pVertex = nullptr;
	u32*					pIndex  = nullptr;
	//---- 頂点バッファ/インデックスバッファをマッピングして書き換え
	pVertex = (AssetModelX::GpuVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	// 動くフラグによってウェイト決定
	f32 weight = _isDynamic ? 1.0f : 0.0f;
	
	s32		totalVertexCount = 0;				// 全頂点数

	for( u32 f=0; f<_faces.size(); f++ ) {
		AssetModelX::Face&	face =	_faces[f];	
	
		for( s32 triangle=0; triangle<face._vertexCount - 2; triangle++ ) {
			AssetModelX::GpuVertex*	v = &pVertex[totalVertexCount];
			for( s32 vi=0; vi<3; vi++ ) {
				for( s32 skin=0; skin<4; skin++ ) {
					if( v[vi]._blendIndex[skin] == _myFrameJointIndex ){
						// スキニングウェイト
						v[vi]._blendWeight[skin] = weight;
					}
				}
			}
		}
		totalVertexCount += 3;
	}

	//---- マッピング解除
	GLboolean resultArray = glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//-----------------------------------------------------------------------------
//	モデルデータを最適化
//-----------------------------------------------------------------------------
bool AssetModelX::Mesh::optimize(void)
{
	//-------------------------------------------------------------
	// スキニングウェイト情報を作成構築
	//-------------------------------------------------------------
	for( u32 i=0; i<_skinWeights.size(); i++ ) {
		AssetModelX::SkinWeight*	pSkinWeight = _skinWeights[i];
		AssetModelX::Frame*			pFrame      = _pParent->searchFrame( pSkinWeight->getName() );

		if( pFrame == NULL ) {
			MessageBoxA(NULL, pSkinWeight->getName(), "スキンウエイトのバインドに失敗しました.", MB_OK);
			return false;
		}

		pFrame->setOffsetMatrix( pSkinWeight->getOffsetMatrix() );

		for( u32 s=0; s<pSkinWeight->_data.size(); s++ ) {
			AssetModelX::SkinWeight::Data*	p = &pSkinWeight->_data[s];

			s32	index  = p->_vertexIndex;
			f32	weight = p->_weight;

			AssetModelX::Vertex	*	pVertex = &_vertices[index];
			pVertex->_skinFrame [3] = pVertex->_skinFrame [2];
			pVertex->_skinFrame [2] = pVertex->_skinFrame [1];
			pVertex->_skinFrame [1] = pVertex->_skinFrame [0];
			pVertex->_skinFrame [0] = pFrame;

			pVertex->_skinWeight[3] = pVertex->_skinWeight[2];
			pVertex->_skinWeight[2] = pVertex->_skinWeight[1];
			pVertex->_skinWeight[1] = pVertex->_skinWeight[0];
			pVertex->_skinWeight[0] = weight;
			
		}
	}

	//-------------------------------------------------------------
	// 有効な関節を抽出
	//-------------------------------------------------------------
	for( u32 i=0; i<_skinWeights.size(); i++ ) {
		AssetModelX::SkinWeight*	pSkinWeight = _skinWeights[i];
		AssetModelX::Joint*			pJoint      = &_joints[i];
		AssetModelX::Frame*			pFrame      = _pParent->searchFrame( pSkinWeight->getName() );

		pJoint->_jointIndex      = pFrame->getJointIndex();
		pJoint->_parentIndex     = -1;
		pJoint->_pFrame          = pFrame;
		pJoint->_offsetMatrix    = pSkinWeight->getOffsetMatrix();
		pJoint->_transformMatrix = pFrame     ->getTransformMatrix();

		//---- 親の関節番号を取得
		AssetModelX::Frame*	pParentFrame = pFrame->getParent();
		if( pParentFrame ) {
			pJoint->_parentIndex = (s32)( pParentFrame->getJointIndex() );
		}
	}

	//-------------------------------------------------------------
	// マテリアル単位で昇順に並び替え
	//-------------------------------------------------------------
	qsort(&_faces[0], _faces.size(), sizeof(AssetModelX::Face), &sortFunc);
#if 0
	// マテリアル単位で並び替え（動作検証用バブルソート。非常に低速）
	for( s32 face=getFaceCount()-1; face>=1; face-- ) {
		for( s32 i=0; i<face; i++ ) {
			AssetModelX::Face*	pFace1 = getFace(i);
			AssetModelX::Face*	pFace2 = getFace(i + 1);

			if( pFace1->_material > pFace2->_material ) {
				AssetModelX::Face	temp = *pFace1;
				*pFace1 = *pFace2;
				*pFace2 = temp;
			}
		}
	}
#endif
	//-------------------------------------------------------------
	// メッシュ全体のポリゴン数を数える。
	// (３角形は１ポリゴン、４角形は２ポリゴンとして数える)
	//-------------------------------------------------------------
	s32	triangleCount = 0;
	for( u32 f=0; f<_faces.size(); f++ ) {
		// ３角形=1、４角形=2
		triangleCount += _faces[f]._vertexCount - 2;
	}

	//-------------------------------------------------------------
	// GPU上のVRAMに頂点バッファを確保
	//-------------------------------------------------------------
	u32 vertexBufferSize = sizeof(AssetModelX::GpuVertex) * triangleCount * 3;
	u32	indexBufferSize = sizeof(u32) * triangleCount * 3;

	//---- 頂点バッファ
	{
		glGenBuffers(1, &_pVertexBuffer);				// メモリを確保
		glBindBuffer(GL_ARRAY_BUFFER, _pVertexBuffer);	// これからARRAY_BUFFERに行う処理はこのIDに反映する。
		glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, NULL, GL_STATIC_DRAW);
		
	}
	{
		//---- インデックスバッファ
		glGenBuffers(1, &_pIndexBuffer);						// メモリを確保
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _pIndexBuffer);	// これからELEMENT_ARRAY_BUFFERに行う処理はこのIDに反映する。
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, NULL, GL_STATIC_DRAW);
		
	}



	//-------------------------------------------------------------
	// 頂点のスキニングインデックス番号をウェイト昇順に並び替えておく
	// (4ウェイトの描画と1ウェイトの描画両方に対応できるようにするため)
	//-------------------------------------------------------------
	for( u32 i=0; i<_vertices.size(); i++ ) {
		AssetModelX::Vertex&	v = _vertices[i];

		for( s32 n1=4-2; n1>=0; n1-- )
		for( s32 n2=4-2; n2>=0; n2-- ) {
			if( v._skinWeight[n2] >= v._skinWeight[n2 + 1] ) continue;	

			//---- スキンインデックスをウェイト値をセットで交換
			f32					weight = v._skinWeight[n2];
			AssetModelX::Frame*	pFrame = v._skinFrame [n2];

			v._skinWeight[n2] = v._skinWeight[n2 + 1];
			v._skinFrame [n2] = v._skinFrame [n2 + 1];

			v._skinWeight[n2 + 1] = weight;
			v._skinFrame [n2 + 1] = pFrame;
		}	
	}

	//-------------------------------------------------------------
	// 頂点バッファ/インデックスバッファにデータを書き込む
	//-------------------------------------------------------------
	AssetModelX::GpuVertex*	pVertex = NULL;
	u32*					pIndex  = NULL;
	//---- 頂点バッファ/インデックスバッファをメインメモリにマッピング
	pVertex = (AssetModelX::GpuVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	pIndex  = (u32*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

	s32		totalVertexCount = 0;				// 全頂点数
	s32		totalIndexCount  = 0;				// 全インデックス数
	s32*	pPrimCount       = NULL;			// １クラスター内のプリミティブ数
	s32*	pVertexCount     = NULL;			// 　　　〃　　　　頂点数

	AssetModelX::Cluster*	pCluster      = NULL;
	s32						materialIndex = -1;
	Vector4					diffuseColor;
	Vector4					specularColor;
	Vector4					emissiveColor;
	float					specularPower;

	for( u32 f=0; f<_faces.size(); f++ ) {
		AssetModelX::Face&	face =	_faces[f];	

		// マテリアル番号をチェックして変化があった場合にだけ切り替える
		s32	m = face._material;	// マテリアル番号
		if( m != materialIndex ) {
			AssetModelX::Material*	pMaterial = _materials[m];

			// マテリアル更新
			materialIndex = m;								// マテリアル番号
			diffuseColor  = pMaterial->getDiffuseColor();	// ディフューズ色
			specularColor = pMaterial->getSpecularColor();	// スペキュラ色
			emissiveColor = pMaterial->getEmissiveColor();	// 自己照明
			specularPower = pMaterial->getShininess();		// スペキュラパワー

			// クラスターを新規追加作成
			_clusters.push_back( AssetModelX::Cluster() );
			s32		  clusterIndex = _clusters.size() - 1;
			pCluster               = &_clusters[clusterIndex];
			pCluster->_pMaterial   = pMaterial;
			pCluster->_vertexStart = totalVertexCount;
			pCluster->_indexStart  = totalIndexCount;

			// 個数を保存する場所を覚えておく
			pPrimCount   = &pCluster->_primCount;
			pVertexCount = &pCluster->_vertexCount;
		}

		for( s32 triangle=0; triangle<face._vertexCount - 2; triangle++ ) {
			const AssetModelX::Vertex*	pV[3];
			pV[0] = &_vertices[ face._vertexIndex[           0] ];
			pV[1] = &_vertices[ face._vertexIndex[triangle + 1] ];
			pV[2] = &_vertices[ face._vertexIndex[triangle + 2] ];

			Vector3	normal[3];
			if( _normals.size() > 0 ) {
				normal[0] = _normals[ face._normalIndex[           0] ];
				normal[1] = _normals[ face._normalIndex[triangle + 1] ];
				normal[2] = _normals[ face._normalIndex[triangle + 2] ];
			}else{
				normal[0] = Vector3(0,1,0);
				normal[1] = Vector3(0,1,0);
				normal[2] = Vector3(0,1,0);
			}
			
			AssetModelX::GpuVertex*	v = &pVertex[totalVertexCount];
			u32*					i = &pIndex [totalIndexCount];
			for( s32 vi=0; vi<3; vi++ ) {
				v[vi]._position		   = pV[vi]->_position;
				v[vi]._diffuseColor    = diffuseColor;
				v[vi]._specularColor   = specularColor;
				v[vi]._emissiveColor   = emissiveColor;
				v[vi]._shininess	   = specularPower;
				v[vi]._uv       = pV[vi]->_uv;
				v[vi]._normal   = normal[vi];

				for( s32 skin=0; skin<4; skin++ ) {
					//---- スキニングインデックス番号
					if( pV[vi]->_skinFrame[skin] ) {
						v[vi]._blendIndex [skin] = pV[vi]->_skinFrame [skin]->getJointIndex();
						// スキニングウェイト
						v[vi]._blendWeight[skin] = pV[vi]->_skinWeight[skin];
					}
					else {
						v[vi]._blendIndex [skin] = (f32)( _myFrameJointIndex );
						// 動くオブジェクトならweight 1.0f
						f32 weight = _isDynamic ? 1.0f : 0.0f;
						// スキニングウェイト
						v[vi]._blendWeight[skin] = weight;
					}
				}
			}

			i[0] = totalVertexCount + 0;
			i[1] = totalVertexCount + 1;
			i[2] = totalVertexCount + 2;

			totalVertexCount += 3;
			totalIndexCount  += 3;

			*pPrimCount      += 1;
			*pVertexCount    += 3;
		}
	}
		
	//---- マッピング解除
	GLboolean resultElement = glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	GLboolean resultArray   = glUnmapBuffer(GL_ARRAY_BUFFER);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#if 1

	//-------------------------------------------------------------
	// 通常描画のVAO
	//-------------------------------------------------------------
	// VAO作成
	_VAO.genVAO();
	// VAO登録開始
	_VAO.beginRegistContext();
	// 通常描画のコンテキスト取得
	NormalContext NContext = GmShader()->getNormalContext();
	
	GmShader()->setAndEnableVertexAttributeWithHandle( NContext._positionLocation     , 3, GL_FLOAT	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _position		)) );
	GmShader()->setAndEnableVertexAttributeWithHandle( NContext._blendIndicesLocation , 4, GL_FLOAT	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _blendIndex	)) );
	GmShader()->setAndEnableVertexAttributeWithHandle( NContext._blendWeightLocation  , 4, GL_FLOAT	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _blendWeight	)) );
	GmShader()->setAndEnableVertexAttributeWithHandle( NContext._normalLocation		  , 3, GL_FLOAT	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _normal		)) );
	GmShader()->setAndEnableVertexAttributeWithHandle( NContext._uvLocation			  , 2, GL_FLOAT	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _uv			)) );
	GmShader()->setAndEnableVertexAttributeWithHandle( NContext._specularPowerLocation, 3, GL_FLOAT	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _shininess		)) );

	// VAO登録終了
	_VAO.endRegistContext();

	//-------------------------------------------------------------
	// シャドウ描画のVAO
	//-------------------------------------------------------------
	
	// VAO作成
	_shadowVAO.genVAO();
	// VAO登録開始
	_shadowVAO.beginRegistContext();
	// シャドウ描画のコンテキスト取得
	ShadowContext SContext = GmShader()->getShadowContext();

	GmShader()->setAndEnableVertexAttributeWithHandle( SContext._positionLocation     , 3, GL_FLOAT	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _position		)) );
	GmShader()->setAndEnableVertexAttributeWithHandle( SContext._blendIndicesLocation , 4, GL_FLOAT	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _blendIndex	)) );
	GmShader()->setAndEnableVertexAttributeWithHandle( SContext._blendWeightLocation  , 4, GL_FLOAT	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _blendWeight	)) );
	
	/*glVertexAttribPointer(SContext._positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _position	)) );
	glEnableVertexAttribArray(SContext._positionLocation);
	glVertexAttribPointer(SContext._blendWeightLocation, 4, GL_FLOAT, GL_FALSE, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _blendWeight	)) );
	glEnableVertexAttribArray(SContext._blendWeightLocation);
	glVertexAttribPointer(SContext._blendIndicesLocation, 4, GL_FLOAT, GL_FALSE, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _blendIndex	)) );
	glEnableVertexAttribArray(SContext._blendIndicesLocation);*/

	// VAO登録終了
	_shadowVAO.endRegistContext();

#endif
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return true;
}

//-----------------------------------------------------------------------------
//	スキニングヘッダの読み込み
//-----------------------------------------------------------------------------
bool  AssetModelX::Mesh::loadSkinMeshHeader(AssetModelX::Reader* pReader)
{
	GM_CSTR	token;

	pReader->openChunk();
	{
		token = pReader->getToken();
		_skinWeightsPerVertexCountMax = atoi(token);

		token = pReader->getToken();
		_SkinWeightsPerFaceCountMax   = atoi(token);

		//---- 関節ボーン数
		token = pReader->getToken();
		s32	jointCount = atoi(token);
		_joints.resize(jointCount);
	}
	pReader->closeChunk();

	return true;
}

//-----------------------------------------------------------------------------
// スキニングウェイトを読み込み
//-----------------------------------------------------------------------------
bool  AssetModelX::Mesh::loadSkinWeight(AssetModelX::Reader* pReader)
{
	GM_CSTR	token;

	pReader->openChunk();
	{
		//---- 新規作成
		AssetModelX::SkinWeight*	pSkinWeight = new AssetModelX::SkinWeight();
		if( pSkinWeight == NULL ) {
			return false;
		}
		// 配列に登録
		_skinWeights.push_back(pSkinWeight);

		//------------------------------------------------------------
		// フレーム名
		//------------------------------------------------------------
		token = pReader->getToken();
		GM_CHAR	name[MAX_PATH];
		strcpy(name, &token[1]);			// 先頭の「”」記号を除去
		name[ strlen(name) - 1 ] = '\0';	// 末尾の「”」記号を除去
		// スキンウェイトの名前として設定
		pSkinWeight->setName(name);

		//------------------------------------------------------------
		// ウェイト配列個数
		//------------------------------------------------------------
		token = pReader->getToken();
		s32	count = atoi(token);

		pSkinWeight->_data.resize(count);

		//---- 頂点インデックス
		for( s32 i=0; i<count; i++ ) {
			token = pReader->getToken();
			s32	index = atoi(token);

			pSkinWeight->_data[i]._vertexIndex = index;
		}
		//---- 頂点ウェイト値
		for( s32 i=0; i<count; i++ ) {
			token = pReader->getToken();
			f32	weight = (f32)atof(token);

			pSkinWeight->_data[i]._weight = weight;
		}

		//------------------------------------------------------------
		// オフセット行列
		//------------------------------------------------------------
		Matrix	offsetMatrix;
		token = pReader->getToken();	offsetMatrix._m[0]._x =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[0]._y =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[0]._z =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[0]._w =  (f32)atof(token);

		token = pReader->getToken();	offsetMatrix._m[1]._x =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[1]._y =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[1]._z =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[1]._w =  (f32)atof(token);

		token = pReader->getToken();	offsetMatrix._m[2]._x =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[2]._y =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[2]._z =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[2]._w =  (f32)atof(token);

		token = pReader->getToken();	offsetMatrix._m[3]._x =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[3]._y =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[3]._z =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[3]._w =  (f32)atof(token);

		pSkinWeight->setOffsetMatrix(offsetMatrix);
	}
	pReader->closeChunk();

	return true;
}


//=============================================================================
//	AssetModelX::Frame	モデルフレーム
//=============================================================================

//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
AssetModelX::Frame::Frame(void)
: _jointIndex	  (-1)					// 関節番号
, _texRepeat	  ( 1)					// テクスチャのリピート数
, _offsetMatrix   (Matrix::IDENTITY)	// オフセット行列
, _transformMatrix(Matrix::IDENTITY)	// 関節姿勢行列
, _rotationMatrix (Matrix::IDENTITY)	// 回転行列
, _pParent		  (0)					// 親
, _pNext		  (0)					// 兄弟
, _pPrev		  (0)					// 前
, _pChild		  (0)					// 子
, _isDynamic	  (false)				// 動くかどうか
, _isRender		  (true)				// 描画フラグ
{}

//-----------------------------------------------------------------------------
//	デストラクタ
//-----------------------------------------------------------------------------
AssetModelX::Frame::~Frame(void)
{
	//---- メッシュの削除
	for(vector<AssetModelX::Mesh*>::iterator itr = _meshes.begin(); itr != _meshes.end(); )
	{
		AssetModelX::Mesh* pMesh = *itr;
		SAFE_DELETE(pMesh);
		itr = _meshes.erase(itr);
	}
}

//-----------------------------------------------------------------------------
//	子に追加
//!	@param	[in]	p	追加するオブジェクト
//-----------------------------------------------------------------------------
void AssetModelX::Frame::add(AssetModelX::Frame* p)
{
	if( _pChild ) {
		// 子がすでに登録されているときは末尾に追加
		for(Frame* pChild=_pChild; ; pChild=pChild->getNext() ) {
			if( pChild->_pNext ) continue;

			pChild->_pNext = p;
			p     ->_pPrev = pChild;
			break;
		}
	}
	else {
		// 新規登録
		_pChild   = p;
		p->_pPrev = this;
	}
	p->_pParent = this;
}

//-----------------------------------------------------------------------------
//! オフセットを設定
//-----------------------------------------------------------------------------
void AssetModelX::Frame::setOffset(const Vector3& vec)
{
	// 動くかどうかフラグ取得
	if( !_isDynamic ){
		// 動くフラグ設定
		setDynamic(true);
	}

	// 移動行列作成
	Matrix	offsetMatrix = Matrix::translate(vec);
	// 移動行列設定
	setOffsetMatrix(offsetMatrix);
}
//-----------------------------------------------------------------------------
//! オイラー角を設定
//-----------------------------------------------------------------------------
void AssetModelX::Frame::setRotate(const Vector3& rot)
{
	// 動くかどうかフラグ取得
	if( !_isDynamic ){
		// 動くフラグ設定
		setDynamic(true);
	}
	// クォータニオン作成
	Quaternion	rotateQuaternion(rot);
	// 回転行列に変換
	Matrix	rotateMatrix = rotateQuaternion.toRotateMatrix();
	// 回転行列設定
	setRotateMatrix(rotateMatrix);
}

//-----------------------------------------------------------------------------
//	オフセット行列を設定
//-----------------------------------------------------------------------------
void AssetModelX::Frame::setOffsetMatrix(const Matrix& m)
{
	_offsetMatrix = m;
}

//-----------------------------------------------------------------------------
//	姿勢行列を設定
//-----------------------------------------------------------------------------
void AssetModelX::Frame::setTransformMatrix(const Matrix& m)
{
	_transformMatrix = m;
}

//-----------------------------------------------------------------------------
//	回転行列を設定
//-----------------------------------------------------------------------------
void AssetModelX::Frame::setRotateMatrix(const Matrix& m)
{
	_rotationMatrix = m;
}

//-----------------------------------------------------------------------------
//	関節番号を設定
//-----------------------------------------------------------------------------
void AssetModelX::Frame::setJointIndex(f32 index)
{
	_jointIndex = index;
}

//-----------------------------------------------------------------------------
//	オフセット行列を取得
//-----------------------------------------------------------------------------
const Matrix& AssetModelX::Frame::getOffsetMatrix(void) const
{
	return _offsetMatrix;
}
	
//-----------------------------------------------------------------------------
//	姿勢行列を取得
//-----------------------------------------------------------------------------
const Matrix& AssetModelX::Frame::getTransformMatrix(void) const
{
	return _transformMatrix;
}

//-----------------------------------------------------------------------------
//	回転行列を取得
//-----------------------------------------------------------------------------
const Matrix& AssetModelX::Frame::getRotateMatrix(void) const
{
	return _rotationMatrix;
}

//-----------------------------------------------------------------------------
//	関節番号を取得
//-----------------------------------------------------------------------------
f32 AssetModelX::Frame::getJointIndex(void) const
{
	return _jointIndex;
}

//=============================================================================
//	END OF FILE
//=============================================================================
