//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilterLensFlare.cpp
//!	@brief	レンズフレア
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
TaskFilterLensFlare::TaskFilterLensFlare()
: _lensColorTex		(nullptr)
, _lensDirtTex		(nullptr)
, _lensStarBurstTex	(nullptr)
{
	SAFE_NEW(_lensColorTex);
	SAFE_NEW(_lensDirtTex);
	SAFE_NEW(_lensStarBurstTex);

	_lensColorTex->loadAndTexImage2D("../Image/LensFlare/lenscolor.png",	false, false, GL_TEXTURE_1D);
	_lensDirtTex->loadAndTexImage2D("../Image/LensFlare/lensdirt.png",		false);
	//_lensDirtTex->loadAndTexImage2D("../Image/LensFlare/dust2.jpg",			false);
	_lensStarBurstTex->loadAndTexImage2D("../Image/LensFlare/lensstar.png", false, true);

}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
TaskFilterLensFlare::~TaskFilterLensFlare()
{
	SAFE_DELETE(_lensColorTex);
	SAFE_DELETE(_lensDirtTex);
	SAFE_DELETE(_lensStarBurstTex);
}

//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool TaskFilterLensFlare::setup()
{
	Size<s32>	windowSize = GmWindow()->getResolution();
	Size<s32>	workTexResolution;
	workTexResolution._w = windowSize._w * (1.0f/8.0f);
	workTexResolution._h = windowSize._h;
	// 親クラスの初期化を呼ぶ
	TaskFilter::setup(workTexResolution);

	//---- テクスチャのメモリ確保
	SAFE_NEW(_accumTexture);
	if(_accumTexture == nullptr) return false;
	SAFE_NEW(_stockTexture);
	if(_stockTexture == nullptr) return false;


	//---- 空テクスチャの作成
	if (!_accumTexture->createRenderTarget(windowSize._w, windowSize._h, GL_RGBA16F_ARB, GL_RGBA, GL_HALF_FLOAT_ARB, true)) {
		MessageBox(NULL, "RenderTargetの作成に失敗しました。"__FILE__, "ERROR", MB_OK);
		return false;
	}

	if (!_stockTexture->createRenderTarget(windowSize._w, windowSize._h, GL_RGBA16F_ARB, GL_RGBA, GL_HALF_FLOAT_ARB, true)) {
		MessageBox(NULL, "RenderTargetの作成に失敗しました。"__FILE__, "ERROR", MB_OK);
		return false;
	}

	for(u32 i=0; i<2; ++i){
		SAFE_NEW(_bilenarExpandTex[i]);
		if (_bilenarExpandTex[i] == nullptr) return false;
		// ワークテクスチャの2倍の幅のテクスチャ
		if (!_bilenarExpandTex[i]->createRenderTarget(workTexResolution._w * 2.0f, workTexResolution._h, GL_RGBA16F_ARB, GL_RGBA, GL_HALF_FLOAT_ARB, true)) {
			MessageBox(NULL, "RenderTargetの作成に失敗しました。"__FILE__, "ERROR", MB_OK);
			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
//! 解放
//-----------------------------------------------------------------------------
void TaskFilterLensFlare::cleanup()
{
	TaskFilter::cleanup();
	for (u32 i = 0; i<2; ++i){
		SAFE_DELETE(_bilenarExpandTex[i]);
	}

}

//-----------------------------------------------------------------------------
//! 描画
//-----------------------------------------------------------------------------
void TaskFilterLensFlare::render()
{

	static const s32	SHADER_STANDARD	   = SystemShader::SHADER_FILTER;
	static const s32	SHADER_SUBTRACT	   = SystemShader::SHADER_SUBTRACT;
	static const s32	SHADER_EXPAND_BLUR = SystemShader::SHADER_EXPAND_BLUR;

	//-------------------------------------------------------------
	// (1) バックバッファからワークバッファにコピー
	// グレアの場合はぼかすので1/2バッファの縮小しておくことで
	// 後段パスが軽量化される(バイリニアで4ピクセル平均)
	//-------------------------------------------------------------
	copyToWorkTexture(SHADER_STANDARD, 0);

	// 半透明無効化
	setBlendMode(BM_NONE);
	//-------------------------------------------------------------
	// (2)高輝度の成分を抽出
	// 画像から閾値(Threshold)の分を減算
	//-------------------------------------------------------------
	s32 index = 1;
	s32 mip	  = 0;
	

	//static float threshold = 0.9f;	// タイトルで確認する時用
	static float threshold = 2.5f;
	//static float threshold = 9.1f;
	static float scale = 1000.0f;

	Controller* control = GmControlMan()->getController(1);

	if(control->GetPadState(Controller::PAD_RB)){

		threshold += 0.01f;

	}else if (control->GetPadState(Controller::PAD_LB)){
		threshold -= 0.01f;
	}

	threshold = max(0.0f, threshold);

	GmShader()->changeShader(SystemShader::SHADER_SUBTRACT);

	GmShader()->setUniform1fv("gThreshold", (GLfloat*)&threshold);
	GmShader()->setUniform1fv("scale", (GLfloat*)&scale);

	setRenderTarget(index, mip);
	//setBackBuffer();
	put(0, 0);	// texuter[0] mip=0 : SUBTRACT


	GmShader()->beginPrevShader();

	GmRender()->saveBackBuffer("レンズフレア差分");


	//-------------------------------------------------------------
	// (3) アナモルフィックレンズのゴースト作成(線対称)
	//-------------------------------------------------------------
	/*GmShader()->changeShader(SystemShader::SHADER_LENS_FLARE);

	GmShader()->bindTexture("lensColorTex", _lensColorTex->getTextureHandle(), 1, GL_TEXTURE_1D);

	GmShader()->setUniform1i("isAnamolphic", (GLint)true);

	setRenderTarget(index ^ 1, mip);
	put(index, mip);
	index ^= 1;

	GmShader()->unbindTexture(GL_TEXTURE_1D);

	GmRender()->saveBackBuffer("レンズフレア作成後");
*/

	
	

	//-------------------------------------------------------------
	// (3) 横方向ブラー&青着色
	//-------------------------------------------------------------
	//// 左右にブラーを掛けるため二回している
	//for (s32 d = 0; d<2; ++d){

	//	// 光の方向を指定
	//	f32 x = 1;
	//	f32 y = 0.0f;

	//	if(d == 1) {
	//		x = -1;
	//		//---- 一時保存しておいたテクスチャからコピー
	//		setRenderTarget(index, mip);
	//		put(_stockTexture, mip, SHADER_STANDARD);
	//	}

	//	GmShader()->changeShader(SystemShader::SHADER_GLARE);

	//	// ブラー方向設定
	//	GmShader()->setUniform1f("gAttenuation", 0.99f);

	//	// ブラー方向設定
	//	GmShader()->setUniform2fv("gDir2D", (GLfloat*)&Vector3(x, y, 0.0f));


	//	for (s32 i = 0; i<3; i++)
	//	{

	//		// ピクセルの間隔(1, 8, 64)
	//		GmShader()->setUniform1f("gStride", (GLfloat)(pow(8, (f32)i)));

	//		//---- 水平ブラー
	//		setRenderTarget(index ^ 1, mip);
	//		put(index, mip);
	//		index ^= 1;
	//	}

	//	// 元に戻す
	//	GmShader()->beginPrevShader();

	//	// 蓄積テクスチャに保存
	//	GmRender()->setRenderTarget(_accumTexture, Float2(0, 0), 0);

	//	if(d==0){
	//		glClear(GL_COLOR_BUFFER_BIT);
	//	}

	//	setBlendMode(BM_ADD_NOALPHA);
	//	put(index, mip, SHADER_STANDARD);
	//}


	// 光の方向を指定
	f32 x = 1;
	f32 y = 0.0f;

	GmShader()->changeShader(SystemShader::SHADER_ANAMOLPHIC_FLARE);

	// ブラー方向設定
	GmShader()->setUniform1f("gAttenuation", 0.98f);

	// ブラー方向設定
	GmShader()->setUniform2fv("gDir2D", (GLfloat*)&Vector3(x, y, 0.0f));


	for (s32 i = 0; i<3; i++)
	{

		// ピクセルの間隔(1, 8, 64)
		GmShader()->setUniform1f("gStride", (GLfloat)(pow(8, (f32)i)));

		//---- 水平ブラー
		setRenderTarget(index ^ 1, mip);
		put(index, mip);
		index ^= 1;
	}

	// 元に戻す
	GmShader()->beginPrevShader();

	GmRender()->saveBackBuffer("レンズフレアブラー後 1/4サイズ");

	//---- 2倍にバイリニアで拡大
	{
		GmRender()->setRenderTarget(_bilenarExpandTex[0], Float2(0, 0), 0);
		put(index, mip, SHADER_EXPAND_BLUR);
		GmRender()->saveBackBuffer("バイリニアで2倍に拡大");
		//---- 実行確認のため保存
		GmRender()->setRenderTarget(_stockTexture, Float2(0, 0), 0);
		put(_bilenarExpandTex[0], mip, SHADER_STANDARD);
		GmRender()->saveBackBuffer("バイリニアで4倍に拡大");
	}

	//---- 4点0.25フェッチでわずかにぼかしてバイリニアで拡大
	{
		static f32 offset = 0.5f;

		if (GetKeyState(VK_OEM_PLUS) & 0x80){
			offset += 0.01f;
		}
		else if (GetKeyState(VK_OEM_MINUS) & 0x80){
			offset -= 0.01f;
		}

		GmShader()->changeShader(SystemShader::SHADER_EXPAND_BLUR);
		
		GmShader()->setUniform1f("gUVOffset", offset);
		GmRender()->setRenderTarget(_stockTexture, Float2(0, 0), 0);
		put(_bilenarExpandTex[0], mip);
		GmRender()->saveBackBuffer("ストックテクスチャ");

		GmShader()->beginPrevShader();
	}

	//-------------------------------------------------------------
	// (3) 通常レンズでのフレア&HALO作成
	//-------------------------------------------------------------


	
#if 0 
	GmShader()->changeShader(SystemShader::SHADER_LENS_FLARE);
	
	GmShader()->bindTexture("lensColorTex", _lensColorTex->getTextureHandle(), 1, GL_TEXTURE_1D);

	GmShader()->setUniform1i("isAnamolphic", (GLint)true);

	setRenderTarget(index^1, mip);
	put(index, mip);
	index ^= 1;

	GmShader()->unbindTexture(GL_TEXTURE_1D);

	GmRender()->saveBackBuffer("レンズフレア作成後");

#endif
	
	//-------------------------------------------------------------
	// (4) ガウシアンブラー→縮小を繰り返し、多段バッファ生成
	//-------------------------------------------------------------
# if 0
	//---- 水平ブラー
	setRenderTarget(index ^ 1, mip);
	put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_H);
	index ^= 1;

	//---- 垂直ブラー
	setRenderTarget(index ^ 1, mip);
	put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_V);
	index ^= 1;

	while (mip<4) {

		//---- ミップ段数を一段下にコピー
		setRenderTarget(index, mip + 1);
		put(index, mip, SHADER_STANDARD);
		mip++;

		//---- 水平ブラー
		setRenderTarget(index ^ 1, mip);
		put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_H);
		index ^= 1;

		////---- 垂直ブラー
		//setRenderTarget(index ^ 1, mip);
		//put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_V);
		//index ^= 1;

	}
#endif

#define IS_DIRT_AND_BURST 0
	//-------------------------------------------------------------
	// (5) レンズ汚れ、スターバーストをつける
	//-------------------------------------------------------------
#if IS_DIRT_AND_BURST
	GmShader()->changeShader(SystemShader::SHADER_LENS_DIRT_STAR);

	Matrix	viewMatrix		= GmScene()->getViewMatrix();
	Vector3 right			=  Vector3(viewMatrix._m[0]);
	Vector3 viewDir			= -Vector3(viewMatrix._m[2]);

	float	camRot			= Vector3::dot(right, Vector3(0,0,1)) + Vector3::dot(viewDir, Vector3(0,1,0));

	Matrix	cameraRotZMat	= Matrix::rotateZ(Radian(camRot));

	static Matrix scaleBias1 = Matrix(
		2.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 2.0f, -1.0f, 0.0f,
		0.0f, 0.0f,  1.0f, 0.0f,
		0.0f, 0.0f,  0.0f, 1.0f
	);
	static Matrix scaleBias2 = Matrix(
		0.5f, 0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	Matrix lensStarMatrix = scaleBias2 * cameraRotZMat * scaleBias1;
	//Matrix lensStarMatrix = cameraRotZMat;

	GmShader()->setMatrix4x4("lensStarMatrix", 1, GL_TRUE, (GLfloat*)&lensStarMatrix);
	
	
	GmShader()->bindTexture("lensStarTex", _lensStarBurstTex->getTextureHandle(), 3);
	GmShader()->bindTexture("lensDirtTex", _lensDirtTex->getTextureHandle(), 2);
	

	// 汚れテクスチャはmip 0 でやらないとぼやけてしまうため mip 0に対して行っている
	setRenderTarget(index^1, 0);
	put(index, mip);
	index ^= 1;

	GmShader()->unbindTexture();
	GmShader()->beginPrevShader();


#endif

	//-------------------------------------------------------------
	// (6) 加算合成で全てをバックバッファに合成する
	//-------------------------------------------------------------
	setBackBuffer();

	s32	workTextureIndex = index;
	s32 mipLevel = mip;

	// 汚れフィルタを通る場合はmipLevel 0でやっているためmipLevelを変更
	if (IS_DIRT_AND_BURST == 1) {
		mipLevel		 = 0;
	}

	static bool isResult = false;

	// BACKボタンでレンズフレアのみ描画か加算合成かを切り替え
	if (control->GetPushState(Controller::PAD_BACK)){
		isResult ^= true;
	}

	if (isResult){
		setBlendMode(BM_NONE);
		//put(workTextureIndex, mipLevel, SHADER_STANDARD);
		put(_stockTexture, mipLevel, SHADER_STANDARD);
	}else{
		setBlendMode(BM_ADD_NOALPHA);
		put(_stockTexture, mipLevel, SHADER_STANDARD);
		setBlendMode(BM_NONE);
	}
	

}


//=============================================================================
//	END OF FILE
//=============================================================================