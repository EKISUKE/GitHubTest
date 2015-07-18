//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskModelX.cpp
//!	@brief	3Dモデル(Xファイル形式)
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
//!	箱を描画
//!	@param	[in]	position	中心位置
//!	@param	[in]	extent		範囲（幅・高さ・奥行き）
//!	@param	[in]	color		描画色
//-----------------------------------------------------------------------------
//static void drawBox(const Vector3& position, const Vector3& extent, D3DCOLOR color)
static void drawBox(const Vector3& position, const Vector3& extent, Color color)
{
	static Vertex	v[4];

	//IDirect3DDevice9*	pD3DDevice = GmRender()->getD3DDevice();

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY  );
	glEnableClientState( GL_NORMAL_ARRAY );

	//---- 天板
	{
		v[0]._position = position + extent * Vector3(-1, +1, -1);
		v[0]._color    = color;
		v[0]._normal   = Vector3(0.0f, 1.0f, 0.0f);
	
		v[1]._position = position + extent * Vector3(+1, +1, -1);
		v[1]._color    = color;
		v[1]._normal   = Vector3(0.0f, 1.0f, 0.0f);

		v[2]._position = position + extent * Vector3(-1, +1, +1);
		v[2]._color    = color;
		v[2]._normal   = Vector3(0.0f, 1.0f, 0.0f);

		v[3]._position = position + extent * Vector3(+1, +1, +1);
		v[3]._color    = color;
		v[3]._normal   = Vector3(0.0f, 1.0f, 0.0f);

		//pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &v[0], sizeof(Vertex));
		
		
		glVertexPointer	 (3, GL_FLOAT		 , sizeof(Vertex), &v[0]._position);
		glColorPointer	 (4, GL_UNSIGNED_BYTE, sizeof(Vertex), &v[0]._color);
		//glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &v[0]._uv);
		glNormalPointer  (   GL_FLOAT		 , sizeof(Vertex), &v[0]._normal);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
	}


	//---- 右
	{
		v[0]._position = position + extent * Vector3(+1, +1, +1);
		v[0]._color    = color;
		v[0]._normal   = Vector3(1.0f, 0.0f, 0.0f);
	
		v[1]._position = position + extent * Vector3(+1, +1, -1);
		v[1]._color    = color;
		v[1]._normal   = Vector3(1.0f, 0.0f, 0.0f);

		v[2]._position = position + extent * Vector3(+1, -1, +1);
		v[2]._color    = color;
		v[2]._normal   = Vector3(1.0f, 0.0f, 0.0f);

		v[3]._position = position + extent * Vector3(+1, -1, -1);
		v[3]._color    = color;
		v[3]._normal   = Vector3(1.0f, 0.0f, 0.0f);
			
		//pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &v[0], sizeof(Vertex));

		glVertexPointer	 (3, GL_FLOAT		 , sizeof(Vertex), &v[0]._position);
		glColorPointer	 (4, GL_UNSIGNED_BYTE, sizeof(Vertex), &v[0]._color);
		//glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &v[0]._uv);
		glNormalPointer  (   GL_FLOAT		 , sizeof(Vertex), &v[0]._normal);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	
	//---- 左
	{
		v[0]._position = position + extent * Vector3(-1, +1, -1);
		v[0]._color    = color;
		v[0]._normal   = Vector3(-1.0f, 0.0f, 0.0f);
	
		v[1]._position = position + extent * Vector3(-1, +1, +1);
		v[1]._color    = color;
		v[1]._normal   = Vector3(-1.0f, 0.0f, 0.0f);

		v[2]._position = position + extent * Vector3(-1, -1, -1);
		v[2]._color    = color;
		v[2]._normal   = Vector3(-1.0f, 0.0f, 0.0f);

		v[3]._position = position + extent * Vector3(-1, -1, +1);
		v[3]._color    = color;
		v[3]._normal   = Vector3(-1.0f, 0.0f, 0.0f);

		//pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &v[0], sizeof(Vertex));
		
		glVertexPointer	 (3, GL_FLOAT		 , sizeof(Vertex), &v[0]._position);
		glColorPointer	 (4, GL_UNSIGNED_BYTE, sizeof(Vertex), &v[0]._color);
		//glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &v[0]._uv);
		glNormalPointer  (   GL_FLOAT		 , sizeof(Vertex), &v[0]._normal);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}


	//---- 奥
	{
		v[0]._position = position + extent * Vector3(+1, +1, -1);
		v[0]._color    = color;
		v[0]._normal   = Vector3(0.0f, 0.0f, -1.0f);
	
		v[1]._position = position + extent * Vector3(-1, +1, -1);
		v[1]._color    = color;
		v[1]._normal   = Vector3(0.0f, 0.0f, -1.0f);

		v[2]._position = position + extent * Vector3(+1, -1, -1);
		v[2]._color    = color;
		v[2]._normal   = Vector3(0.0f, 0.0f, -1.0f);

		v[3]._position = position + extent * Vector3(-1, -1, -1);
		v[3]._color    = color;
		v[3]._normal   = Vector3(0.0f, 0.0f, -1.0f);

		//pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &v[0], sizeof(Vertex));

		glVertexPointer	 (3, GL_FLOAT		 , sizeof(Vertex), &v[0]._position);
		glColorPointer	 (4, GL_UNSIGNED_BYTE, sizeof(Vertex), &v[0]._color);
		//glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &v[0]._uv);
		glNormalPointer  (   GL_FLOAT		 , sizeof(Vertex), &v[0]._normal);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}


	//---- 底
	{
		v[0]._position = position + extent * Vector3(-1, -1, -1);
		v[0]._color    = color;
		v[0]._normal   = Vector3(0.0f, -1.0f, 0.0f);
	
		v[1]._position = position + extent * Vector3(+1, -1, -1);
		v[1]._color    = color;
		v[1]._normal   = Vector3(0.0f, -1.0f, 0.0f);

		v[2]._position = position + extent * Vector3(-1, -1, +1);
		v[2]._color    = color;
		v[2]._normal   = Vector3(0.0f, -1.0f, 0.0f);

		v[3]._position = position + extent * Vector3(+1, -1, +1);
		v[3]._color    = color;
		v[3]._normal   = Vector3(0.0f, -1.0f, 0.0f);

		//pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &v[0], sizeof(Vertex));

		glVertexPointer	 (3, GL_FLOAT		 , sizeof(Vertex), &v[0]._position);
		glColorPointer	 (4, GL_UNSIGNED_BYTE, sizeof(Vertex), &v[0]._color);
		//glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &v[0]._uv);
		glNormalPointer  (   GL_FLOAT		 , sizeof(Vertex), &v[0]._normal);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	//---- 手前
	{
		v[0]._position = position + extent * Vector3(-1, +1, +1);
		v[0]._color    = color;
		v[0]._normal   = Vector3(0.0f, 0.0f,  1.0f);
	
		v[1]._position = position + extent * Vector3(+1, +1, +1);
		v[1]._color    = color;
		v[1]._normal   = Vector3(0.0f, 0.0f,  1.0f);

		v[2]._position = position + extent * Vector3(-1, -1, +1);
		v[2]._color    = color;
		v[2]._normal   = Vector3(0.0f, 0.0f,  1.0f);

		v[3]._position = position + extent * Vector3(+1, -1, +1);
		v[3]._color    = color;
		v[3]._normal   = Vector3(0.0f, 0.0f,  1.0f);

		//pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &v[0], sizeof(Vertex));

		glVertexPointer	 (3, GL_FLOAT		 , sizeof(Vertex), &v[0]._position);
		glColorPointer	 (4, GL_UNSIGNED_BYTE, sizeof(Vertex), &v[0]._color);
		//glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &v[0]._uv);
		glNormalPointer  (   GL_FLOAT		 , sizeof(Vertex), &v[0]._normal);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}


	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY  );
	glDisableClientState( GL_NORMAL_ARRAY );
}

//-----------------------------------------------------------------------------
//!	ラインを描画
//-----------------------------------------------------------------------------
//static void drawLineG(const Vector3& p1, const Vector3& p2, D3DCOLOR c1, D3DCOLOR c2)
static void drawLineG(const Vector3& p1, const Vector3& p2, Color c1, Color c2)
{
	static Vertex	v[2];
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY  );
	{
		v[0]._position = p1;
		v[0]._color    = c1;
	
		v[1]._position = p2;
		v[1]._color    = c2;

		glVertexPointer	 (3, GL_FLOAT		 , sizeof(Vertex), &v[0]._position);
		glColorPointer	 (4, GL_UNSIGNED_BYTE, sizeof(Vertex), &v[0]._color);
		glDrawArrays(GL_LINES, 0, 2);
	}
	glDisableClientState( GL_COLOR_ARRAY  );
	glDisableClientState( GL_VERTEX_ARRAY );
	
}


//=============================================================================
//!	TaskModelX	モデル
//=============================================================================

//-----------------------------------------------------------------------------
// 静的変数の宣言
//-----------------------------------------------------------------------------
GLuint TaskModelX::_depthTexHandle;				//!< デプステクスチャのハンドル
Matrix TaskModelX::_biasMatrix;					//!< シャドウのビュー行列
Matrix TaskModelX::_shadowView[_splitCount];	//!< シャドウのビュー行列
Matrix TaskModelX::_shadowProj[_splitCount];	//!< シャドウのプロジェクション行列


//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
TaskModelX::TaskModelX(void)
: _scale			  (1.0f)					// スケール値
, _pModel			  (nullptr)					// モデル
, _pVAO				  (nullptr)					// 頂点入力レイアウト
, _rotateMatrix		  (Matrix::IDENTITY)		// 回転行列
, _worldMatrix        (Matrix::IDENTITY)		// 姿勢行列
, _isAnimationRepeat  (false)					// アニメーションをリピート再生するかどうか
, _animationPlayType  (ANIMATION_PLAY_DEFAULT)	// アニメーションの属性
, _animationNumber    (-1)						// アニメーション番号
, _animationTime      (0.0f)					// アニメーションの再生中の時間
, _animationSpeed     (1.0f)					// アニメーション再生速度
, _animationBlendRatio(1.0f)					// アニメーションブレンド係数
, _animationRate	  (0.0f)					// アニメーション再生比率
, _shadowEnable		  (false)					// シャドウマッピング有効化フラグ

{
}

//-----------------------------------------------------------------------------
//	デストラクタ
//-----------------------------------------------------------------------------
TaskModelX::~TaskModelX(void)
{
	SAFE_DELETE(_pVAO);
}

//-----------------------------------------------------------------------------
//	初期化
//-----------------------------------------------------------------------------
bool TaskModelX::init(void)
{
	if(_pModel->_animations.size() != 0) {
		setAnimation(0);
		update();
	}

	calcMinMax();

	return true;
}

//-----------------------------------------------------------------------------
//	並列非同期実行
//-----------------------------------------------------------------------------
void TaskModelX::asyncUpdate(void)
{
}

//-----------------------------------------------------------------------------
//	直列同期実行
//-----------------------------------------------------------------------------
void TaskModelX::update(void)
{
	AssetModelX*			pModel = getModel();
	if( _animationNumber != -1 ) 
	{

		AssetModelX::Animation*	pAnimation    = pModel->_animations[_animationNumber];
		f32						animationTime = _animationTime;

		//------------------------------------------------------------
		// アニメーション名更新
		//------------------------------------------------------------
		_animationName = pAnimation->getName();

		//------------------------------------------------------------
		// 時間を進める
		//------------------------------------------------------------
		_animationTime += 3600.0f/60.0f * (_animationSpeed * Global::deltaTime);

		//------------------------------------------------------------
		// 再生比率計算
		//------------------------------------------------------------
		_animationRate = LinearInterpolation(0.0f, 1.0f, _animationTime, (f32)pAnimation->getTotalTime());

		//------------------------------------------------------------
		// 総再生時間を超えた場合の振る舞い（リピート再生・単体再生）
		//------------------------------------------------------------
		//if( _animationTime > (f32)pAnimation->getTotalTime() ) {
		if( _animationRate >= 1.0f ) {
		
			if( _isAnimationRepeat ) {	//---- 繰り返すために巻き戻す
				_animationTime -= (f32)pAnimation->getTotalTime();
			}
			else {						//---- 終端でストップ
				_animationTime  = (f32)pAnimation->getTotalTime();
			}
			// アニメーション終了フラグを立てる
			_animationNumber = -1;
		}

		//------------------------------------------------------------
		// アニメーション同士の補間比率
		//------------------------------------------------------------
		f32			blendRatio = _animationBlendRatio;

		//---- アニメーション同士の補間比率を進める 0.0f→1.0f
		f32			blendFrame = 10;
		blendRatio += 1.0f / blendFrame;
		if( blendRatio > 1.0f ) {
			blendRatio = 1.0f;
		}
		_animationBlendRatio = blendRatio;

		//------------------------------------------------------------
		// 全てのアニメーション関節の行列を更新
		//------------------------------------------------------------
		//for( s32 bone=0; bone<pAnimation->_bone.getCount(); bone++ ) {
		for( u32 bone=0; bone<pAnimation->_bone.size(); bone++ ) {
			AssetModelX::Animation::Bone*	pBone      = pAnimation->_bone[bone];
			s32								jointIndex = pBone->_jointIndex;

			//---- 関節にマッピングされていないボーンは無視
			if( jointIndex == -1 ) continue;

			TaskModelX::Joint&		joint = _joints[jointIndex];

			//-----------------------------------------------------------
			//	補間されたアニメーションデータを取得
			//-----------------------------------------------------------
			//---- キーフレームの間を補間
			Quaternion	rotation = pBone->getRotation(animationTime);
			Vector3		scale    = pBone->getScale   (animationTime);
			Vector3		position = pBone->getPosition(animationTime);

			//---- さらに以前のアニメーションとの間を補間
			Quaternion	blendedRotation = Quaternion::slerp(joint._blendRotation, rotation, blendRatio);
			Vector3		blendedPosition = (position - joint._blendPosition) * blendRatio + joint._blendPosition;
			Vector3		blendedScale    = (scale    - joint._blendScale   ) * blendRatio + joint._blendScale;
	
			//-----------------------------------------------------------
			// それぞれの補間後の要素から行列を生成する
			//-----------------------------------------------------------
			Matrix		matRotate    = blendedRotation.toRotateMatrix();	// 回転
			Matrix		matTranslate = Matrix::translate(blendedPosition);	// 移動
			Matrix		matScale     = Matrix::scale    (blendedScale);		// スケール

			joint._rotation          = blendedRotation;
			joint._position          = blendedPosition;
			joint._scale             = blendedScale;
			joint._animationMatrix   = matScale * matRotate * matTranslate;
	
		}

	}else{
		_animationName = "none";
	}

	//-------------------------------------------------------------
	// 各関節のローカル行列に反映
	//-------------------------------------------------------------
	_skinLocalMatrices[0] = Matrix::IDENTITY;

	if( _animationNumber != -1 || pModel->haveDynamicFrame() ){


		//for(s32 i=0; i<pModel->_frames.getCount(); i++ ) {
		for(u32 i=0; i<pModel->_frames.size(); i++ ) {
			const AssetModelX::Frame*	pFrame       = pModel->_frames[i];
			const AssetModelX::Frame*	pFrameParent = pFrame->getParent();

			const s32					jointIndex   = (s32)( pFrame->getJointIndex() );
			TaskModelX::Joint&			joint        = _joints[jointIndex ];

			//---- ローカル行列作成
			// 原点での変換行列は、自分自身のアニメーションと、親の姿勢を加えたもの。
			joint._transformMatrix = joint._animationMatrix;

			if( pFrameParent ) {
				const s32			parentIndex = (s32)( pFrameParent->getJointIndex() );
				TaskModelX::Joint&	parent = _joints[parentIndex];

				joint._transformMatrix     = joint._transformMatrix * parent._transformMatrix;
				//joint._transformMatrix     = parent._transformMatrix;
			}

			// 関節ローカル行列 = (原点に移動させる行列) × (関節姿勢行列)
			_skinLocalMatrices[jointIndex] = pFrame->getOffsetMatrix() * joint._transformMatrix * pFrame->getRotateMatrix();
		}
	}
}

//-----------------------------------------------------------------------------
//! メッシュを描画
//!	@attention この関数内は描画用頂点フォーマットが変更になっているため、
//!	@attention AssetModelX::GpuVertexを使用して描画する必要がある。
//-----------------------------------------------------------------------------
void TaskModelX::_renderMesh(AssetModelX::Mesh* pMesh)
{
	/*IDirect3DDevice9*	pD3DDevice = GmRender()->getD3DDevice();
	ID3DXEffect*		pD3DEffect = GmShader()->getEffect();

	D3DXHANDLE			hTexture   = pD3DEffect->GetParameterByName(NULL, "BaseTexture");


	s32		modelP0 = 0;
	s32		modelP1 = 1;

	if( pMesh->isSkined() ) {
		D3DXHANDLE			hTechnique = pD3DEffect->GetTechnique(0);
		D3DXTECHNIQUE_DESC	techniqueDesc;
		pD3DEffect->GetTechniqueDesc(hTechnique, &techniqueDesc);

		for( u32 pass=0; pass<techniqueDesc.Passes; pass++ ) {
			D3DXHANDLE			hPass      = pD3DEffect->GetPass(hTechnique, pass);

			D3DXPASS_DESC	passDesc;
			pD3DEffect->GetPassDesc(hPass, &passDesc);
			if( strcmp(passDesc.Name, "MODEL_SKIN_P0") == 0 ) {
				modelP0 = pass;
				continue;
			}
			if( strcmp(passDesc.Name, "MODEL_SKIN_P1") == 0 ) {
				modelP1 = pass;
				continue;
			}
		}
	}*/

	
#if 1
	//=============================================================
	// 頂点バッファを使って描画発行の最適化を行った場合
	// ※ DrawPrimitiveはマテリアルの個数分だけ呼ばれる。
	//=============================================================

	//---- VAOで描画
	VertexArrayObject vao = pMesh->getVertexArrayObject(_shadowEnable);

	// 描画準備
	vao.beginRender();

	// インデックスバッファ設定
	GLuint IBO = pMesh->getIndexBuffer();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	for( u32 c=0; c<pMesh->_clusters.size(); c++ ) {
		AssetModelX::Cluster&	cluster   = pMesh->_clusters[c];
		AssetModelX::Material*	pMaterial = cluster._pMaterial;

		if(!_shadowEnable) {
			//---- テクスチャの設定
			// テクスチャ設定
			pMaterial->BindTextures();
		}
		
		// クラスター分のオフセット値
		GLint offset = cluster._vertexStart * sizeof(u32);

		glDrawElements(GL_TRIANGLES, cluster._vertexCount, GL_UNSIGNED_INT, (GLvoid*)offset);

		pMaterial->UnBindTextures();

	}

	// インデックスバッファ解除
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// 描画終了
	vao.endRender();

#else

	//---- VBOで描画(VAOなし)

	GLuint	pVB = pMesh->getVertexBuffer();
	GLuint	pIB = pMesh->getIndexBuffer();

	// 頂点バッファを設定
	glBindBuffer(GL_ARRAY_BUFFER, pVB);			// これからの処理はこのIDに反映します。
	// インデックスバッファを設定
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pIB);	// これからの処理はこのIDに反映します。
	
	//---- デバッグ用コード
	
	AssetModelX::GpuVertex* vertex = (AssetModelX::GpuVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

	for( s32 c=0; c<pMesh->_clusters.size(); c++ ) {
		AssetModelX::Cluster&	cluster   = pMesh->_clusters[c];
		for( s32 i=0; i < cluster._vertexCount; i++ )
		{
			AssetModelX::GpuVertex	 debug = vertex[i];
			for( s32 w=0; w<4; w++ ) {
				f32			 weight	= debug._blendWeight[w];
				if( weight > 0.0f )
				{
					s32 a = 0;
				}
			}

		}
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	

	//------------------------------------------------------------
	// マテリアル単位で描画発行
	//------------------------------------------------------------
	//for( s32 c=0; c<pMesh->_clusters.getCount(); c++ ) {
	for( s32 c=0; c<pMesh->_clusters.size(); c++ ) {
		AssetModelX::Cluster&	cluster   = pMesh->_clusters[c];
		AssetModelX::Material*	pMaterial = cluster._pMaterial;

		//---- テクスチャの設定
		// テクスチャ設定
		pMaterial->BindTextures();

		// デプステクスチャ転送
		//GLuint depthTexHandle = _shadow->getDepthTextureHandle();
		//GLuint depthTexHandle = _shadow->getColorTextureHandle();
		//GLuint depthTexHandle = debugTex.getTextureHandle();
		
		//GmShader()->setTexture("ShadowMap", depthTexHandle, GL_TEXTURE4, 4);

		//s32				pass = 0;
		//if( pTexture ) {	// あり
		//	/*pD3DEffect->SetTexture(hTexture, pTexture->getTexture()->getD3DTexture());
		//	pass = modelP1;*/
		//	
		//}
		//else {				// なし
		//	//pass = modelP0;
		//	
		//}

		//------------------------------------------------------------
		// 描画
		//------------------------------------------------------------

		// クラスター分のオフセット値
		GLuint offset = cluster._vertexStart * sizeof(AssetModelX::GpuVertex);

		//---- VertexShaderに頂点情報を転送し有効化
		GmShader()->setAndEnableVertexAttribute( "position"		, 3, GL_FLOAT			, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _position		) + offset) );
		GmShader()->setAndEnableVertexAttribute( "blendIndices" , 4, GL_FLOAT			, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _blendIndex	) + offset) );
		GmShader()->setAndEnableVertexAttribute( "blendWeight"  , 4, GL_FLOAT			, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _blendWeight	) + offset) );
		GmShader()->setAndEnableVertexAttribute( "normal"		, 3, GL_FLOAT			, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _normal		) + offset) );
		GmShader()->setAndEnableVertexAttribute( "color"		, 4, GL_UNSIGNED_BYTE	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _diffuseColor	) + offset) );
		GmShader()->setAndEnableVertexAttribute( "uv"			, 2, GL_FLOAT			, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _uv			) + offset) );
		GmShader()->setAndEnableVertexAttribute( "specularPower", 3, GL_FLOAT			, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _shininess		) + offset) );
		
		// 頂点バッファとインデックスバッファを使って描画
		//glDrawElements(GL_TRIANGLE_STRIP, cluster._vertexCount, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, cluster._vertexCount, GL_UNSIGNED_INT, 0);

		//---- VertexShaderの頂点情報を無効化
		GmShader()->DisableVertexAttribute( "position"		 );
		GmShader()->DisableVertexAttribute( "blendIndices"	 );
		GmShader()->DisableVertexAttribute( "blendWeight"	 );
		GmShader()->DisableVertexAttribute( "normal"		 );
		GmShader()->DisableVertexAttribute( "color"			 );
		GmShader()->DisableVertexAttribute( "uv"			 );
		GmShader()->DisableVertexAttribute( "specularPower"  );

		pMaterial->UnBindTextures();

	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);			// バインドを外す
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// バインドを外す
	
#endif
}


//-----------------------------------------------------------------------------
//! フレームに属するメッシュを描画
//!	@param	[in]	描画対象のモデルフレーム
//-----------------------------------------------------------------------------
void TaskModelX::_renderFrame(AssetModelX::Frame* pFrame)
{

	if( pFrame->isRender() && pFrame->_meshes.size() != 0 ) {
		//---- 透明度をアルファブレンド有効で反映
		setBlendMode(BM_BLEND_ALPHA);

		// 裏面カリング ON
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);

		// αテストで透明部分はＺを書き込まない
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GEQUAL, 0.5f);

		//---- このフレームの変換行列をワールド行列にする
		TaskModelX::Joint*	pJoint = &_joints[ (s32)( pFrame->getJointIndex() ) ];

		f32	scale = _scale;
		Matrix	matWorld = pJoint->_transformMatrix * Matrix::scale(Vector3(scale, scale, -scale)) * _rotateMatrix * _worldMatrix;

		CameraBase*	camera = GmCameraMan()->getCurrentCamera();
		Matrix	matView = camera->getViewMatrix();
		Matrix	matProj = camera->getProjMatrix();
		
		//---- 行列をシェーダに転送
		Matrix sendMatrixData[] =
		{
			matWorld,
			matView,
			matProj,
		};

		GmShader()->setUniformMatrixBuffer(sendMatrixData, sizeof(sendMatrixData));
		

		if( !_shadowEnable ) {
			// モデルビュープロジェクションを転送
			Matrix depthMatrices[_splitCount];

			for( u32 i=0; i<_splitCount; ++i ) { 
				Matrix shadowProj = _shadowProj[i];
				Matrix shadowView = _shadowView[i];

				depthMatrices[i]  = matWorld * shadowView * shadowProj * _biasMatrix;
			}

			GmShader()->setMatrix4x4("depthBiasMVP", _splitCount, GL_FALSE, (GLfloat*)&depthMatrices[0]._m);
		
					
			GmShader()->bindTexture("ShadowMap", _depthTexHandle, 4);
		}
		GmShader()->setUniform1f("texRepeat", (GLfloat)(pFrame->getTexRepeat()) );
		
		getGLError();
	
		//------------------------------------------------------------
		// メッシュデータを持っていたら描画する
		//------------------------------------------------------------
		for( u32 mesh=0; mesh<pFrame->_meshes.size(); mesh++ ) {
			_renderMesh( pFrame->_meshes[mesh] );
		}

		GmShader()->unbindTexture();

		getGLError();

	}

	//-------------------------------------------------------------
	// 再帰呼び出しで子と兄弟を実行
	//-------------------------------------------------------------
	AssetModelX::Frame*	pNext  = pFrame->getNext();
	AssetModelX::Frame*	pChild = pFrame->getChild();

	if( pNext ) {				//---- 兄弟
		_renderFrame( pNext );
	}
	if( pChild ) {				//---- 子
		_renderFrame( pChild );
	}

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_CULL_FACE);
	setBlendMode(BM_NONE);
}	




//-----------------------------------------------------------------------------
//	描画
//-----------------------------------------------------------------------------
void TaskModelX::render(bool isShadow)
{
	AssetModelX*		pModel     = _pModel;

	if( pModel == NULL ) return;

	// シャドウ描画かどうか
	_shadowEnable = isShadow;
	
	//-------------------------------------------------------------
	// スキニング配列をシェーダーに設定
	//-------------------------------------------------------------
	Matrix*		pMatrixArray = &_skinLocalMatrices[0];			// 配列
	s32			matrixCount  = _skinLocalMatrices.size();	// 配列数
	
	GmShader()->setMatrix4x4("gSkinMatrix", matrixCount, GL_TRUE, (GLfloat*)&pMatrixArray[0]._m);
	//-------------------------------------------------------------
	//	ルート(幹)から描画実行
	//-------------------------------------------------------------
	s32 renderCount = 1;

	// フレーム描画
	_renderFrame( pModel->_frames[0] );

	//-------------------------------------------------------------
	// アニメーションの詳細をデバッグ表示
	//-------------------------------------------------------------
#if 0
	//---- カメラビューと投影行列
	CameraBase*	pCamera = ICameraManager()->getCurrentCamera();

	Matrix	matViewProj = Matrix::IDENTITY;
	if( pCamera ) {
		matViewProj = pCamera->getViewProjMatrix();
	}
	else {
		Matrix	matView  = pCamera->getViewMatrix();
		Matrix	matProj  = pCamera->getProjMatrix();

		matViewProj = matView * matProj;
	}

	// シェーダ終了
	GmShader()->endShader();

	//---- 描画
	// キャラクターにボーン表示が埋まってしまうため、Z隠面消去を無効にして重ねて描画する
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	for( u32 i=0; i<pModel->_frames.size(); i++ ) {
		TaskModelX::Joint*		pJoint = &_joints[ (u32)( pModel->_frames[i]->getJointIndex() ) ];
		
		Matrix	jointWorldMatrix = pJoint->_transformMatrix * Matrix::scale(Vector3(_scale, _scale, -_scale)) * _rotateMatrix * _worldMatrix;
		Vector3	p1               = jointWorldMatrix._m[3];

		if( pModel->_frames[i]->getParent() ) {
			TaskModelX::Joint*	pParent = &_joints[ (u32)( pModel->_frames[i]->getParent()->getJointIndex() ) ];
			
			Vector3	p2 = ( pParent->_transformMatrix * Matrix::scale(Vector3(_scale, _scale, -_scale)) * _rotateMatrix * _worldMatrix)._m[3];
			drawLineG(p1, p2, Color(255,128,0,255), Color(255,0,0,255)); 
		}
		f32		boxSize  = 0.02f * _scale;
		drawBox(p1, Vector3(boxSize, boxSize, boxSize), Color(255,255,0,255)); 

		f32		size  = 0.05f * _scale;
		Vector3	axisX = Vector3( jointWorldMatrix._m[0] ).normalize();
		Vector3	axisY = Vector3( jointWorldMatrix._m[1] ).normalize();
		Vector3	axisZ = Vector3( jointWorldMatrix._m[2] ).normalize();
		drawLineG(p1, p1 + axisX * size, Color(255,128,128,255), Color(255,128,128,255));
		drawLineG(p1, p1 + axisY * size, Color(128,255,128,255), Color(128,255,128,255));
		drawLineG(p1, p1 + axisZ * size, Color(128,128,255,255), Color(128,128,255,255));
	}

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	// 一つ前のシェーダの再開
	GmShader()->beginPrevShader();
#endif
}

//-----------------------------------------------------------------------------
//	モデルデータを設定
//!	外部からモデルデータを設定する際に動的に変化するデータをタスク側に確保する。
//-----------------------------------------------------------------------------
bool TaskModelX::setModel(AssetModelX* pModel)
{
	_pModel = pModel;

	//-------------------------------------------------------------
	// フレーム関節ワークを確保
	//-------------------------------------------------------------
	s32	frameCount = pModel->_frames.size();

	_joints.resize(frameCount);	// アニメーション行列

	// 関節用ローカル行列
	_skinLocalMatrices   .resize(frameCount);	// ローカル行列

	//-------------------------------------------------------------
	// 行列を初期設定
	//-------------------------------------------------------------
	_joints[0]._animationMatrix = Matrix::IDENTITY;
	_joints[0]._transformMatrix = Matrix::IDENTITY;

	for(s32 i=1; i<frameCount; i++ ) {
		const AssetModelX::Frame*	pFrame       = pModel->_frames[i];
		const AssetModelX::Frame*	pFrameParent = pFrame->getParent();
		const s32					jointIndex   = (s32)( pFrame	  ->getJointIndex() );
		const s32					parentIndex  = (s32)( pFrameParent->getJointIndex() );

		TaskModelX::Joint&			joint       = _joints[jointIndex ];
		TaskModelX::Joint&			parent      = _joints[parentIndex];

		joint._animationMatrix = pFrame->getTransformMatrix();
		joint._transformMatrix = joint._animationMatrix * parent._transformMatrix;

		//---- 関節用ローカル行列
		_skinLocalMatrices   [i] = pFrame->getOffsetMatrix() * joint._transformMatrix * pFrame->getRotateMatrix();
	}

	return true;
}

//-----------------------------------------------------------------------------
//	名前からアニメーションを検索
//	@param	[in]	name	アニメーション名
//	@return アニメーション番号 (-1:一致する名前がなく検出できなかった)
//-----------------------------------------------------------------------------
s32 TaskModelX::searchAnimation(GM_CSTR name)
{
	if( _pModel == NULL ) return -1;

	for( u32 i=0; i<_pModel->_animations.size(); i++ ) {
		AssetModelX::Animation*	pAnimation = _pModel->_animations[i];

		if( strcmp(name, pAnimation->getName()) == 0 ) {
			return i;
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------
//	アニメーションを再生
//-----------------------------------------------------------------------------
bool TaskModelX::setAnimation(s32 animationNumber, ANIMATION_PLAY_TYPE playType, bool interpolate)
{
	if( _pModel == NULL ) {
		return false;
	}

	// アニメーション番号が適正範囲かどうか
	s32	animationCount = _pModel->_animations.size();
	if( animationNumber < -1 || animationCount <= animationNumber ) {
		return false;
	}

	//-------------------------------------------------------------
	// アニメーション指定
	//-------------------------------------------------------------
	if( animationNumber != -1 ) {
		if( playType == ANIMATION_PLAY_REPEAT ) {
			// 強制リピート指定
			_isAnimationRepeat = true;
		}
		else {
			// アニメーションデータからリピート属性を取得して使用
			AssetModelX::Animation*	pAnimation = _pModel->_animations[animationNumber];
			_isAnimationRepeat = pAnimation->isRepeat();
		}

	}
	_animationPlayType = playType;			// アニメーションの属性
	_animationNumber   = animationNumber;
	_animationTime     = 0.0f;

	//-------------------------------------------------------------
	// アニメーション補間の設定
	// ブレンド係数は0.0fから開始して1.0fへ変化していく
	//-------------------------------------------------------------

	// アニメーションブレンド係数をここで0.0fにしておき、update()内で1.0fまで進める。
	// 【補間方向】
	// _blendRotation → _rotation;		// 回転
	// _blendPosition → _position;		// 移動
	// _blendScale    → _scale;		// 拡縮
	if( interpolate ) {
		_animationBlendRatio = 0.0f;
	}
	else {
		_animationBlendRatio = 1.0f;	// 補間しない場合は最初から補間後の比率にしておく
	}

	//---- 現在の姿勢を保存。この姿勢から補間開始
	for( u32 i=0; i<_joints.size(); i++ ) {
		TaskModelX::Joint&	joint = _joints[i];
		joint._blendRotation	  = joint._rotation;	// 回転
		joint._blendPosition	  = joint._position;	// 移動
		joint._blendScale		  = joint._scale;		// 拡縮
	}
	return true;
}

//-----------------------------------------------------------------------------
//! MinMax計算
//-----------------------------------------------------------------------------
void TaskModelX::calcMinMax()
{
	

	f32	scale = _scale;
	
	Float2 X, Y, Z;		// モデルでの最大、最小
	
	X = Float2(FLT_MAX, -FLT_MAX);
	Y = Float2(FLT_MAX, -FLT_MAX);
	Z = Float2(FLT_MAX, -FLT_MAX);

	for(u32 f=0; f<_pModel->_frames.size(); ++f)
	{
		Float2 x, y, z;
		x = Float2(FLT_MAX, -FLT_MAX);
		y = Float2(FLT_MAX, -FLT_MAX);
		z = Float2(FLT_MAX, -FLT_MAX);
		AssetModelX::Frame* frame = _pModel->_frames[f];
		//---- このフレームの変換行列をワールド行列にする
		TaskModelX::Joint*	pJoint = &_joints[ (u32)( frame->getJointIndex() ) ];
		Matrix			  matWorld = pJoint->_transformMatrix * Matrix::scale(Vector3(scale, scale, -scale)) * _rotateMatrix * _worldMatrix;
		for( u32 m=0; m<frame->_meshes.size(); ++m )
		{
			AssetModelX::Mesh* mesh = frame->_meshes[m];
			for( u32 v=0; v<mesh->_vertices.size(); ++v )
			{
				AssetModelX::Vertex vertex = mesh->_vertices[v];
				Vector3 position = vertex._position;
				position = position.transform(matWorld);
				
				//---- 頂点の中の最大値最小値計測
				x._x = min(position._x, x._x);
				x._y = max(position._x, x._y);

				y._x = min(position._y, y._x);
				y._y = max(position._y, y._y);

				z._x = min(position._z, z._x);
				z._y = max(position._z, z._y);
				
			}
		}
		// フレームでの最大最小
		frame->setMinMax(x,y,z);
		//---- フレームの中の最大値最小値計測
		X._x = min(x._x, X._x);
		X._y = max(x._y, X._y);
		Y._x = min(y._x, Y._x);
		Y._y = max(y._y, Y._y);
		Z._x = min(z._x, Z._x);
		Z._y = max(z._y, Z._y);
	}
	// モデルでの最大最小
	_pModel->setMinMax(X,Y,Z);

}

//-----------------------------------------------------------------------------
//! デバッグ描画
//-----------------------------------------------------------------------------
void TaskModelX::debugRender()
{
	AssetModelX*	pModel	= getModel();
	f32				scale	= getScale();
	
	for(u32 i = 0; i < pModel->_frames.size(); ++i) {
		TaskModelX::Joint*	pJoint		= &_joints[(s32)pModel->_frames[i]->getJointIndex()];
		Matrix				modelMatrix = pJoint->_transformMatrix * Matrix::scale(Vector3(scale, scale, -scale)) * getRotateMatrix() * getWorldMatrix();

		for(u32 mesh = 0; mesh < pModel->_frames[i]->_meshes.size(); ++mesh) {
			AssetModelX::Mesh* pMesh = pModel->_frames[i]->_meshes[mesh];

			for(u32 norm = 0; norm < pMesh->_normals.size(); ++norm) {
				Vector3 p1 = pMesh->_vertices[norm]._position.transform(modelMatrix);
				Vector3 n1 = pMesh->_normals[norm].transform(getRotateMatrix());
				drawLineF(p1, p1 + n1*8.0f, Vector4(0.0f,1.0f,0.0f,1.0f));
			}
		}
	}
}

//-----------------------------------------------------------------------------
//! デバッグ描画(2D)
//-----------------------------------------------------------------------------
void TaskModelX::debugRender2D()
{
}

//=============================================================================
//	END OF FILE
//=============================================================================

