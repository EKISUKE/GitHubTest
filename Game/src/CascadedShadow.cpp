//-----------------------------------------------------------------------------
//!
//!	@file	CascadedShadow.cpp
//!	@brief	カスケードシャドウ
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"



//=============================================================================
// カスケードシャドウの実装
//=============================================================================

//---- 静的変数の初期化
const Matrix CascadedShadow::_biasMatrix = Matrix(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
);

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
CascadedShadow::CascadedShadow()
: _currentShadowNum	(0)
, _split_weight		(0.75f)
, _modelMatrix		(Matrix::IDENTITY)
, _depthTexture		(nullptr)
{
	for( u32 i=0; i<6; ++i ) {
		_MinMax[i] = 0.0f;
	}

	// シャドウマップ初期化とプロジェクション行列初期化
	for( u32 i=0; i<SPLIT_COUNT; ++i ) {
		_pShadow[i] = nullptr;
		// シャドウマップ初期化
		SAFE_NEW(_pShadow[i]);
		// 視錐台初期化
		/*f[i]._fov   = TO_RADIAN(120.0f);
		f[i]._ratio = (f32)WIDTH/(f32)HEIGHT;*/
		// プロジェクション行列初期化
		_depthProj[i]  = Matrix::IDENTITY;
	}

	// テクスチャの横幅は分割数分取る
	_depthTexSize = Size<s32>(DEPTH_SIZE * SPLIT_COUNT, DEPTH_SIZE);

	// シャドウテクスチャを作成
	if( !createShadowTexture() ) {
		MessageBoxA(NULL, "シャドウテクスチャ作成に失敗しました", "エラー", MB_OK);
	}

	// 分割数をシェーダに渡しておく
	GmShader()->changeShader(SystemShader::SHADER_TEXTURE);
	GmShader()->setUniform1f("gSplitCount", (GLfloat)SPLIT_COUNT);
	GmShader()->beginPrevShader();

	TaskModelX::setBiasMatrix(_biasMatrix);

	_isSetRender = false;
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
CascadedShadow::~CascadedShadow()
{
	for( u32 i=0; i<SPLIT_COUNT; ++i ) {
		_pShadow[i] = nullptr;
		// シャドウマップ初期化
		SAFE_NEW(_pShadow[i]);
	}
}

//-----------------------------------------------------------------------------
//! 正射影行列の初期化
//-----------------------------------------------------------------------------
void CascadedShadow::initOrthoMatrix()
{
	CameraBase*	cam = GmCameraMan()->getCurrentCamera();
	Vector3		camPos = cam->getPosition();
	Matrix		camMat = cam->getMatrix();

	// 分割距離の更新
	updateSplitDist(0.01f, 3000.0f);

	for (s32 i = 0; i<SPLIT_COUNT; ++i)
	{
		// 視錐台の8点を更新
		updateFrustumPoints(f[i], camPos, camMat);

		// 視線方向を求める
		Vector3 viewDir = -Vector3(camMat._m[2]).normalize();

		// 中心までの距離を求める
		f32 centerOfClip = f[i]._fard - f[i]._neard / 2.0f;
		_centerLength[i]  = centerOfClip + f[i]._neard;
		
		// 視線方向を中点までの距離にし、カメラ座標に足す
		Vector3 center = (viewDir * _centerLength[i]) + camPos;



		// 近クリップ面から遠クリップ面までの距離を求める
		Vector3 p1 = f[i]._point[0];
		Vector3 p2 = f[i]._point[4];
		Vector3 p3 = f[i]._point[6];

		Sphere  clipShpere = calcCircumscribeCircle(p1,p2,p3);

		// 半径を求める(半径がまだ正しくない模様現段階は少し大きめにする)
		f32	 rad = clipShpere.getRadius() * 1.2f;

		_depthProj[i] = Matrix::ortho(-rad, rad, -rad, rad, -rad, rad);
	}

	TaskModelX::setShadowProj(_depthProj);
}

//-----------------------------------------------------------------------------
//! ビュー行列を計算して作る
//! @param	[in]	lightDir	ライト方向
//! @param	[in]	viewDir		ビュー方向
//! @param	[in]	camPos		カメラ座標
//-----------------------------------------------------------------------------
void CascadedShadow::calcAndSetViewMatrix(Vector3& lightDir, Vector3& viewDir, Vector3& camPos)
{
	for(s32 i=0; i<SPLIT_COUNT; ++i) {
		// 視線方向を中点までの距離にし、カメラ座標に足す
		Vector3 center = (viewDir * _centerLength[i]) + camPos;
		Vector3 lightLength = lightDir * Vector3(450.0f);
		_depthView[i] = Matrix::view(lightLength + center, center, Vector3(0, 1, 0));
	}
	// タスクモデルにビュー行列を渡しておく
	TaskModelX::setShadowViewMatrix(_depthView);
}

//-----------------------------------------------------------------------------
//! 余弦定理を計算する
//! ⊿ABCがあると場合
//! @param	[in]	a	辺BCの長さ
//! @param	[in]	b	辺CAの長さ
//! @param	[in]	c	辺ABの長さ
//! @retval	∠CABの値
//-----------------------------------------------------------------------------
Radian calcLawOfCosines(f32& a, f32& b, f32& c)
{
	return Radian( acos( (b*b+c*c-a*a)/(2.0f*b*c) ) );
}


//-----------------------------------------------------------------------------
//! クラメルの公式で外心を求める
//! @param	[in]	p1	座標1
//! @param	[in]	p2	座標2
//! @param	[in]	p3	座標3
//!	@retval	外心の座標
//-----------------------------------------------------------------------------
Vector3& calcCircleCenter(Vector3& p1, Vector3&p2, Vector3& p3)
{
	// 外心をoとすると o - p1 = r = o - p2
	// 外心(x,y)は以下のようになる
	// クラメルの公式のための準備
	f32 x1 = p1._x;
	f32 y1 = p1._y;
	f32 z1 = p1._z;

	f32 x2 = p2._x;
	f32 y2 = p2._y;
	f32 z2 = p2._z;

	f32 x3 = p3._x;
	f32 y3 = p3._y;
	f32 z3 = p3._z;

	f32 A1 = 2.0f * (x2 - x1);
	f32 B1 = 2.0f * (y2 - y1);
	f32 C1 = 2.0f * (z2 - z1);
	
	f32 a = (x2*x2 - x1*x1);
	f32 b = (y2*y2 - y1*y1);
	f32 c = (z2*z2 - z1*z1);

	f32 D1 = (x2*x2 - x1*x1) + (y2*y2 - y1*y1) + (z2*z2 - z1*z1);

	f32 A2 = 2.0f * (x3 - x2);
	f32 B2 = 2.0f * (y3 - y2);
	f32 C2 = 2.0f * (z3 - z2);
	f32 D2 = (x3*x3 - x2*x2) + (y3*y3 - y2*y2) + (z3*z3 - z3*z2);

	f32 A3 = 2.0f * (x1 - x3);
	f32 B3 = 2.0f * (y1 - y3);
	f32 C3 = 2.0f * (z1 - z3);
	f32 D3 = (x1*x1 - x3*x3) + (y1*y1 - y3*y3) + (z1*z1 - z3*z3);

	// 解(分母)
	f32 Delta = (A1*B2*C3) + (A3*B1*C2) + (A2*B3*C1) - (A3*B2*C1) - (A2*B1*C3) - (A1*B3*C2);

	// クラメルの公式を使いx,y,zを求める
	http://www.ee.fit.ac.jp/~kudou/1mathA/01/01-2.html
	f32 x	  = (D1*B2*C3) + (D3*B1*C2) + (D2*B3*C1) - (D3*B2*C1) - (D2*B1*C3) - (D1*B3*C2) / Delta;
	f32 y	  = (A1*D2*C3) + (A3*D1*C2) + (A2*D3*C1) - (A3*D2*C1) - (A2*D1*C3) - (A1*D3*C2) / Delta;
	f32 z	  = (A1*B2*D3) + (A3*B1*D2) + (A2*B3*D1) - (A3*B2*D1) - (A2*B1*D3) - (A1*B3*D2) / Delta;

	return Vector3(x,y,z);

}


//-----------------------------------------------------------------------------
//! 外接円を計算する
//! @param	[in]	p1	座標1
//! @param	[in]	p2	座標2
//! @param	[in]	p3	座標3
//!	@retval	球構造体
//-----------------------------------------------------------------------------
Sphere&	CascadedShadow::calcCircumscribeCircle(Vector3& p1, Vector3& p2, Vector3& p3)
{

	//---- 外心を求める
	Vector3 center = calcCircleCenter(p1, p2, p3);

#if 1 
	//---- 半径を求める
	// 3辺の長さを計算
	Vector3 A = (p1 - p2);
	Vector3 B = (p2 - p3);
	Vector3 C = (p3 - p1);

	f32 a = A.length();
	f32 b = B.length();
	f32 c = C.length();

	// 余弦定理で角度を求める
	Radian radA = calcLawOfCosines(a, b, c);
	// 正弦定理で半径を求める
	// 2R = a / sinA
	f32 R = (a / sinf(radA)) / 2.0f;
#else
	// 求めた外心から半径を計算する
	f32 R = (center - p1).length();
#endif
	
	// 求まった球の情報を返す
	return Sphere(center, R);
}
//-----------------------------------------------------------------------------
//! シャドウテクスチャを作成
//-----------------------------------------------------------------------------
bool CascadedShadow::createShadowTexture()
{
	SAFE_NEW(_depthTexture);

	// 描画ターゲットを作成(複数のシャドウテクスチャを一枚にするのでWIDTHはデプスの横幅×分割数)
	if (!_depthTexture->createRenderTarget(DEPTH_SIZE * SPLIT_COUNT, DEPTH_SIZE, GL_RGB, GL_RGB, GL_FLOAT, false, true))
	{
		MessageBoxA(nullptr, "シャドウテクスチャ作成に失敗しました", "エラー", MB_OK);
	}

	// タスクモデルでデプステクスチャに書き込むので初回のみデプスハンドルをセットする
	TaskModelX::setDepthTexHandle(_depthTexture->getDepthBuffer());

	//// テクスチャ作成
	//glGenTextures(1, &_depthTexture);
	//// ひも付け
	//glBindTexture(GL_TEXTURE_2D, _depthTexture);
	////---- 転送
	//glTexImage2D(GL_TEXTURE_2D,
	//			 0,								// ミップマップ段数
	//			 GL_DEPTH_COMPONENT,			// コンポーネント数 3:RGB 4:RGBA
	//			 (GLsizei)_depthTexSize._w,		// 幅
	//			 (GLsizei)_depthTexSize._h,		// 高さ
	//			 0,								// ボーダーカラー 1:あり 0:なし
	//			 GL_DEPTH_COMPONENT,			// [転送元] コンポーネント
	//			 GL_FLOAT,						// [転送元] の1要素あたりのサイズ
	//			 NULL);							// [転送元] イメージのメモリの先頭
	//// テクスチャの品質設定
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	//glBindTexture(GL_TEXTURE_2D, 0);

	////---- フレームバッファをテクスチャとデプスバッファと再編成する
	//glGenFramebuffers(1, &_depthFrameBuffer);
	//// フレームバッファを割り当て(ひも付け)
	//glBindFramebuffer(GL_FRAMEBUFFER, _depthFrameBuffer);

	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);

	//// デプステクスチャをFBOに接続
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture, 0);


	//// フレームバッファが作成できていなければ失敗
	//if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
	//return false;
	//
	//// タスクモデルにテクスチャハンドルを渡しておく
	//TaskModelX::setDepthTexHandle(_depthTexture);
	//
	//// バッファバインド開放
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

//-----------------------------------------------------------------------------
//! 視錐台の8点の更新
//-----------------------------------------------------------------------------
void CascadedShadow::updateFrustumPoints(Frustum& f, Vector3& center, Matrix& cameraMatrix)
{
	f.updateFrustumPoints(center, cameraMatrix);
}

//-----------------------------------------------------------------------------
//! 分割距離の更新
//-----------------------------------------------------------------------------
void CascadedShadow::updateSplitDist(f32 nd, f32 fd)
{
	f32	lambda  = _split_weight;
	f32	ratio   = fd/nd;
	f[0]._neard = nd;

	// 分割数分だけ近クリップ距離、遠クリップ距離を求める
	for( s32 i=1; i<SPLIT_COUNT; i++ )
	{
		f32 si = i / (f32)SPLIT_COUNT;

		f[i  ]._neard = lambda*(nd*powf(ratio,si)) + (1-lambda)*(nd + (fd - nd)*si);
		f[i-1]._fard  = f[i]._neard * 1.005f;
	}
	// 最後は一番遠い距離なので最遠距離を入れる
	f[SPLIT_COUNT-1]._fard = fd;
}


//-----------------------------------------------------------------------------
//! クリップ行列の適用
//-----------------------------------------------------------------------------
f32 CascadedShadow::applyCropMatrix(Frustum &f, LightBase* pLight)
{
	f32 maxX = -1000.0f;
	f32 maxY = -1000.0f;
	f32	maxZ = -FLT_MAX;
	f32 minX =  1000.0f;
	f32 minY =  1000.0f;
	f32	minZ =  FLT_MAX;
	

	Vector4 transf;	// 変換一時保存用

	//-------------------------------------------------------------
	// シャドウの行列作成
	//-------------------------------------------------------------

	// モデルビュー行列作成
	Matrix shadowModelView = _modelMatrix * _depthView[0];

	
	/*transf = Vector4(f._point[0], 1.0f).transform( shadowModelView );
	minZ = transf._z;
	maxZ = transf._z;*/
	// 最大, 最小を視錐台の残り7点から求める
	for(s32 i=0; i<8; ++i) 
	{
		transf = Vector4(f._point[i], 1.0f).transform( shadowModelView );
		maxZ = max(maxZ, transf._z);
		minZ = min(minZ, transf._z);

		/*maxX = max(maxX, transf._x);
		minX = min(minX, transf._x);
		maxY = max(maxY, transf._y);
		minY = min(minY, transf._y);*/
	}
	
	//-------------------------------------------------------------
	// Zの最大最小を算出
	//-------------------------------------------------------------
	// マップの最大最小と比べる
	// マップの四隅
	//	→ x
	//z↓
	//	0 +------+ 1 
	//	  |		 |
	//	  |		 |
	//	2 *------+ 3
	static const Vector3 objectsPos[4] = {
		Vector3(_MinMax[0], 0.0f, _MinMax[4]),	// minX, minY, minZ
		Vector3(_MinMax[1], 0.0f, _MinMax[4]),	// maxX, minY, minZ
		Vector3(_MinMax[0], 0.0f, _MinMax[5]),	// minX, minY, maxZ
		Vector3(_MinMax[1], 0.0f, _MinMax[5]),	// maxX, minY, maxZ

	};

	for( s32 i=0; i<4; ++i ) 
	{
		Vector3 objPos = objectsPos[i];
		transf = Vector4(objPos, 1.0f).transform( shadowModelView );
		maxZ = max(maxZ, transf._z);
		minZ = min(minZ, transf._z);
	}
	

	Matrix shadowProj = Matrix::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -maxZ, -minZ);
	Matrix shadowMVP  = shadowModelView * shadowProj;
	
	for(s32 i=0; i<8; ++i) {
		transf = Vector4(f._point[i], 1.0f).transform(shadowMVP);

		transf._x /= transf._w;
		transf._y /= transf._w;

		maxX = max(maxX, transf._x);
		minX = min(minX, transf._x);
		maxY = max(maxY, transf._y);
		minY = min(minY, transf._y);
	}

#if 1 
	_depthProj[_currentShadowNum] = Matrix::ortho(minX, maxX, minY, maxY, -maxZ, -minZ);
#else
	f32 scaleX = 2.0f/(maxX - minX);
	f32 scaleY = 2.0f/(maxX - minX);
	f32 offsetX = -0.5f*(maxX + minX)*scaleX;
	f32 offsetY = -0.5f*(maxY + minY)*scaleY;

	shadowMVP = Matrix::IDENTITY;
	shadowMVP._m[0][0] = scaleX;
	shadowMVP._m[1][1] = scaleY;
	shadowMVP._m[3][0] = offsetX;
	shadowMVP._m[3][1] = offsetY;
	//Matrix shadowCrop = shadowMVP.transpose();
	Matrix shadowCrop = shadowMVP;
	_depthProj[_currentShadowNum] = shadowCrop * shadowProj;
#endif 	

	// モデルビュープロジェクション行列作成
	_depthMVP					  = shadowModelView * _depthProj[_currentShadowNum];

	return minZ;	
}

//-----------------------------------------------------------------------------
//! マトリックス等の更新
//-----------------------------------------------------------------------------
void CascadedShadow::update(Matrix& modelMatrix)
{
	_modelMatrix = modelMatrix;
	_pLight		 = ILight()->getLight(0);
}

//-----------------------------------------------------------------------------
//! カメラからの遠クリップ面距離計算
//-----------------------------------------------------------------------------
void CascadedShadow::calcFarDist()
{
	// 現在のカメラを取得
	CameraBase*	camera  = GmCameraMan()->getCurrentCamera();
	Matrix		camProj = camera->getProjMatrix();

	for( u32 i=0; i<SPLIT_COUNT; ++i ) {
		//_farDist[i] = 0.5f*(-f[i]._fard*camProj._m[2][3]+camProj._m[3][3])/f[2]._fard+0.5f;
		_farDist[i] = f[i]._fard;
	}
}

//-----------------------------------------------------------------------------
//! デバッグ描画
//-----------------------------------------------------------------------------
void CascadedShadow::debugShadow()
{
	static s32 num = 0;

	if( GetKeyState('1') & 0x80 ) {
		num = 0;
	}else if( GetKeyState('2') & 0x80 ) {
		num = 1;
	}else if( GetKeyState('3') & 0x80 ) {
		num = 2;
	}else if( GetKeyState('4') & 0x80 ) {
		num = 4;
	}

	// 視錐台を描画する
	if( num == 0 ) {
		f[0].debugRender(Color(Color::RED));
	}else if( num == 1 ) {
		f[1].debugRender(Color(Color::CYAN));
	}else if( num == 2 ) {
		f[2].debugRender(Color(Color::YELLOW));
	}

	// シャドウマップを描画する。
	GmRender()->putTexture(_depthTexture->getDepthBuffer(), 0, 0, _depthTexSize, Color::WHITE, Color::NONE, 0.05f, -1, 0, true);
}

//-----------------------------------------------------------------------------
//! シャドウ書き込み開始
//-----------------------------------------------------------------------------
void CascadedShadow::begin(LightBase* pLight)
{
	// シャドウマップリスト数を上回っていたら処理しない
	if(_currentShadowNum >= SPLIT_COUNT ) return;

	static f32	minMax[6];
	
	// 現在のシャドウを保存
	_pCurrentShadow = _pShadow[_currentShadowNum];

	CameraBase*	cam		= GmCameraMan()->getCurrentCamera();
	Vector3		camPos	= cam->getPosition();
	Matrix		camMat  = cam->getMatrix();

	// 視錐台の8点を更新
//	updateFrustumPoints(f[_currentShadowNum], camPos, camMat);
	// クロップ行列作成
//	f32 minZ = applyCropMatrix(f[_currentShadowNum], pLight);

	

	if( !_isSetRender ) {
		calcAndSetViewMatrix(pLight->getLightDir().normalize(), -Vector3(camMat._m[2]).normalize(), camPos);
		// ビューポートを退避
		glPushAttrib(GL_VIEWPORT_BIT);
		// カメラからの距離リストを計算(コレを元にシェーダでテクスチャを使い分ける)
		calcFarDist();
		// 転送
		GmShader()->changeShader(SystemShader::SHADER_TEXTURE);
		GmShader()->setUniform1fv("farDist", _farDist, SPLIT_COUNT);
		GmShader()->beginPrevShader();

		if (!GmRender()->setRenderTarget(_depthTexture, Float2(0, 0), -1, &Size<s32>(DEPTH_SIZE, DEPTH_SIZE))) {
			MessageBoxA(NULL, "シャドウのバインドに失敗しました", "エラー", MB_OK);
		}
		glClear(GL_DEPTH_BUFFER_BIT);
		_isSetRender = true;	// 描画ターゲット設定フラグON
	}

	// モデルビュープロジェクション行列作成
	_depthMVP = _modelMatrix * _depthView[_currentShadowNum] * _depthProj[_currentShadowNum];

	// 現在のシャドウを描画開始する
	_pCurrentShadow->begin(pLight, _depthMVP, Float2((f32)( DEPTH_SIZE*_currentShadowNum ), 0.0f));
	
	// 次のシャドウマップへ
	++_currentShadowNum;

}

//-----------------------------------------------------------------------------
//! シャドウ書き込み終了
//-----------------------------------------------------------------------------
void CascadedShadow::end()
{
	// シャドウ描画終了処理
	ShadowMap::end();
	// タスクモデルに投影行列を渡しておく
	TaskModelX::setShadowProj(_depthProj);
	glPopAttrib();
	GmRender()->bindPrevRender();
	// 描画ターゲット設定フラグOFF
	_isSetRender = false;
	glClear(GL_DEPTH_BUFFER_BIT);
	// シャドウマップ番号をリセット
	_currentShadowNum = 0;
	
}


//=============================================================================
//	END OF FILE
//=============================================================================