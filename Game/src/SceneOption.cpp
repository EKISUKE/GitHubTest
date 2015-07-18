//-----------------------------------------------------------------------------
//!
//!	@file	SceneTitle.cpp
//!	@brief	タイトル画面
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

//-----------------------------------------------------------------------------
//	タイトルシーン SceneTitle
//-----------------------------------------------------------------------------
GM_TYPE_IMPLEMENT(SceneOption, "オプション");



namespace Option {

static const int	ITEM_COUNT_MAX = SceneOption::SELECT_MAX;

//! 項目が選択されてるかどうか
bool		isSelect[ITEM_COUNT_MAX] = {
	false,
	false,
	false,
	false,
};

//! タイトルの項目のポジションリスト
Float2		ItemPosList[ITEM_COUNT_MAX] = {
	Float2(50.0f,350.0f),
	Float2(50.0f,500.0f),
	Float2(50.0f,650.0f),
	Float2(50.0f,800.0f),
};


//! タイトル項目のサイズリスト
Float2		ItemSizeList[ITEM_COUNT_MAX] = {
	Float2(600.0f, 143.0f),
	Float2(600.0f,  95.0f),
	Float2(600.0f,  98.0f),
	Float2(600.0f, 118.0f),
};

//! ゲージのサイズリスト
Float2		GaugeSizeList[ITEM_COUNT_MAX] = {
	Float2( 50.0f, 50.0f),
	Float2(800.0f, 50.0f),
	Float2(800.0f, 50.0f),
	Float2(800.0f, 50.0f),
};

}	// namespace Option

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
SceneOption::SceneOption()
: _nowJump			(true)
, _isFullScreen		(false)
, _pBackGround		(nullptr)
, _pSettingBar		(nullptr)
, _pSettingCheckBox	(nullptr)
, _pTitle			(nullptr)
{
	for (s32 i=0; i<SELECT_MAX; ++i){
		_font[i] = nullptr;
		SAFE_NEW(_font[i]);
	}

	if (!_font[0]->loadAndTexImage2D("../Image/UI/fullScreen.png")){
		MessageBox(NULL, L"フルスクリーン文字画像読み込み失敗", L"エラー", MB_OK);
	}
	if (!_font[1]->loadAndTexImage2D("../Image/UI/MasterVolume.png")){
		MessageBox(NULL, L"マスターボリューム文字画像失敗", L"エラー", MB_OK);
	}
	if (!_font[2]->loadAndTexImage2D("../Image/UI/BGMVolume.png")){
		MessageBox(NULL, L"BGMボリューム文字画像読み込み失敗", L"エラー", MB_OK);
	}
	if (!_font[3]->loadAndTexImage2D("../Image/UI/SEVolume.png")){
		MessageBox(NULL, L"SEボリューム文字画像読み込み失敗", L"エラー", MB_OK);
	}

	SAFE_NEW(_pBackGround);

	Size<s32>	windowSize = GmWindow()->getResolution();
	if (!_pBackGround->createRenderTarget(windowSize._w, windowSize._h, GL_RGBA16F_ARB, GL_RGBA, GL_HALF_FLOAT_ARB, true)){
		MessageBoxA(NULL, "RenderTargetの作成に失敗しました。"__FILE__, "ERROR", MB_OK);
	}


	SAFE_NEW(_pTitle);
	//! タイトル文字読み込み
	if(!_pTitle->loadAndTexImage2D("../Image/option.png")){
		MessageBoxA(nullptr, "オプションタイトル文字画像読み込み失敗", "画像の読み込みエラー", MB_OK);
	}

	//! セッティングバー読み込み
	SAFE_NEW(_pSettingBar);
	if(!_pSettingBar->loadAndTexImage2D("../Image/UI/SettingBar.png")){
		MessageBoxA(nullptr, "設定のゲージ用バー", "画像の読み込みエラー", MB_OK);
	}
	//! チェックボックス読み込み
	SAFE_NEW(_pSettingCheckBox);
	if(!_pSettingCheckBox->loadAndTexImage2D("../Image/UI/SettingCheckBox.png")){
		MessageBoxA(nullptr, "設定のチェックボックス", "画像の読み込みエラー", MB_OK);
	}

	_select = SELECT_MASTER_VOLUME;

	// フィルターの初期化
	if(!_gaussianFilter.setup()){
		MessageBoxA(nullptr, __FILE__"ガウシアンフィルタの初期化に失敗", "フィルタ初期化エラー", MB_OK);
	}

}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
SceneOption::~SceneOption()
{
	SAFE_DELETE(_pSettingCheckBox);
	SAFE_DELETE(_pSettingBar);
	SAFE_DELETE(_pTitle);
	SAFE_DELETE(_pBackGround);

	for( s32 i=0; i<3; ++i ){
		SAFE_DELETE(_font[i]);
	}

	// フィルターの解放
	_gaussianFilter.cleanup();
}
//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool SceneOption::init()
{
	// アルファ比較値設定
	glAlphaFunc(GL_GEQUAL, 0.1f);

	// 終了フラグOFF
	_isExit = false;
	

	return true;
}

//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void SceneOption::update()
{
	if(_nowJump){
		// 背景の作成
		createBackGroundImage();
		_nowJump = false;
	}

	//f32 test = log(8.0f) / log(2.0f);
	//f32 t = log10(10.0f);
	//f32 f = log(1.0000002);

	// 1Pのコントローラー取得
	Controller* control = GmControlMan()->getController(1);
	update(control[0]);
//	render();
}

//-----------------------------------------------------------------------------
//! キー判定
//-----------------------------------------------------------------------------
void SceneOption::checkKey(Controller& control)
{
	enum {
		KEY_U = (1 << 0),		// 1 : 0001
		KEY_D = (1 << 1),		// 2 : 0010
		KEY_E = (1 << 2),		// 4 : 0100		
		KEY_B = (1 << 3),		// 8 : 1000		
	};

	u32	key = 0;
	key |= (GetKeyState(VK_UP)		& 0x80)	? KEY_U : 0;
	key |= (GetKeyState(VK_DOWN)	& 0x80)	? KEY_D : 0;
	key |= (GetKeyState(VK_RETURN)	& 0x80)	? KEY_E : 0;
	key |= (GetKeyState(VK_ESCAPE)  & 0x80) ? KEY_B : 0;
	
	static u32 oldKey = 0;
	u32		   trigger = (key ^ oldKey) & key;

	oldKey = key;


	_keyUp     = control.GetPushState(Controller::PAD_LEFT_U_STICK) |
			     control.GetPushState(Controller::PAD_UP)			|
			     ((trigger & KEY_U) ? true : false);
	_keyDown   = control.GetPushState(Controller::PAD_LEFT_D_STICK) |
			     control.GetPushState(Controller::PAD_DOWN)		    |
			     ((trigger & KEY_D) ? true : false);
	if(_keyUp || _keyDown){
		_keyLeft  = false;
		_keyRight = false;
	}else{
		_keyLeft  = control.GetPadState(Controller::PAD_LEFT_L_STICK) |
				    control.GetPushState(Controller::PAD_LEFT)		  |
				    ((GetKeyState(VK_LEFT)  & 0x80) ? true : false);
		_keyRight = control.GetPadState(Controller::PAD_LEFT_R_STICK) |
					control.GetPushState(Controller::PAD_RIGHT)		  |
					((GetKeyState(VK_RIGHT) & 0x80) ? true : false);
	}

	_keySelect = control.GetPushState(Controller::PAD_A)			|
				 ((trigger & KEY_E) ? true : false);

	_keyBack   = control.GetPushState(Controller::PAD_B) |
				 ((trigger & KEY_B) ? true : false);
		


}

//-----------------------------------------------------------------------------
//! 左右キーによる音量調節
//-----------------------------------------------------------------------------
bool SceneOption::changeVolumeByKey(f32& volume)
{
	f32 beforeChange = volume;
	if (_keyLeft){
		volume -= 0.01f;
	}
	else if (_keyRight){
		volume += 0.01f;
	}

	// 範囲外指定
	volume = min(1.0f, volume);
	volume = max(0.0f, volume);

	// 値が変更されていたらtrue
	return (beforeChange - volume != 0.0f);
}
//-----------------------------------------------------------------------------
//! 選択しているものによる更新
//-----------------------------------------------------------------------------
void SceneOption::updateSelect()
{
	// 選択しているものによって処理分岐
	switch(_select)
	{
		case SELECT_FULLSCREEN:
		{
			if(_keySelect){
				_isFullScreen ^= true;
				GmMain()->fullScreen(_isFullScreen);
			}
			break;
		}
		case SELECT_MASTER_VOLUME:
		{
			f32 volume = ISoundManager()->getMasterVolume();
			if(changeVolumeByKey(volume)){
				ISoundManager()->setMasterVolume(volume);
			}
			break;
		}
		case SELECT_BGM_VOLUME:
		{
			f32 volume = IBGMManager()->getBGMVolume();
			if (changeVolumeByKey(volume)){
				IBGMManager()->setBGMVolume(volume);
			}
			break;
		}
		case SELECT_SE_VOLUME:
		{
			f32 volume = ISEManager()->getSEVolume();
			if (changeVolumeByKey(volume)){
				ISEManager()->setSEVolume(volume);
			}
			break;
		}
	}
	
}

//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void SceneOption::update(Controller& control)
{
	// キー判定
	checkKey(control);

	if(_keyDown){
		_select = SELECT_STATE(_select + 1);
	}else if(_keyUp){
		_select = SELECT_STATE(_select - 1);
	}

	// 範囲外指定
	if (_select < SELECT_FULLSCREEN)	_select = SELECT_SE_VOLUME;
	if (_select > SELECT_SE_VOLUME)		_select = SELECT_FULLSCREEN;
	// すべてfalseに
	for (int i = 0; i<Option::ITEM_COUNT_MAX; ++i){
		Option::isSelect[i] = false;
	}
	// 選択されているところをturuに
	Option::isSelect[_select] = true;

	// 選択されているもの別の更新
	updateSelect();

	if(_keyBack){
		_nowJump = true;
		GmScene()->jump(SceneTitle::TYPE);
	}

	// 背景描画
	render();
}

//-----------------------------------------------------------------------------
//! フィルターの影響しない描画
//-----------------------------------------------------------------------------
void SceneOption::drawNoFilter()
{
}

//-----------------------------------------------------------------------------
//! 今現在のバックバッファをコピーしてぼかして背景に使う
//-----------------------------------------------------------------------------
void SceneOption::createBackGroundImage()
{
	_gaussianFilter.render(GmRender()->getBackBuffer(), _pBackGround);
}

//-----------------------------------------------------------------------------
//! 描画
//-----------------------------------------------------------------------------
void SceneOption::render()
{
	
	// 背景描画
	GmRender()->putTexture(_pBackGround, 0, 0,
						   Color::WHITE, Color::WHITE, 1.0f,
						   SystemShader::SHADER_PUT_TEXTURE, 0, false, true);
	
	Size<s32> size = Size<s32>(411.0f, 153.0f);

	
	setBlendMode(BM_BLEND_ALPHA);	// 半透明ON

	// タイトル文字描画
	Size<f32> resScale = GmWindow()->getRateResMax();
	GmRender()->putTexture(_pTitle->getTextureHandle(), 400.0f * resScale._w, 50.0f * resScale._h, size, Color::WHITE, Color::DKGRAY);
	
	// 最大解像度との比率
	f32	volumeScales[Option::ITEM_COUNT_MAX] = {
		(_isFullScreen) ? 1.0f : 0.0f,
		SoundPlayer::getMasterVolume(),
		IBGMManager()->getBGMVolume(),
		ISEManager()->getSEVolume(),
	};
	// 描画テクスチャリスト
	static Texture*	 pDrawTexList[Option::ITEM_COUNT_MAX] = {
		_pSettingCheckBox,
		_pSettingBar,
		_pSettingBar,
		_pSettingBar,
	};

	// ゲージの色リスト
	static Color	inGaugeColorList[Option::ITEM_COUNT_MAX] = {
		Color(0  , 191, 255, 240),
		Color(0  , 191, 255, 240),
		Color(0  , 191, 255, 240),
		Color(0  , 191, 255, 240),
	};

	// ゲージのオフセットリスト
	static Float2	offsetGauge[Option::ITEM_COUNT_MAX]	= {
		Float2(0.0f, 110.0f),
		Float2(0.0f, 110.0f),
		Float2(0.0f, 110.0f),
		Float2(0.0f, 110.0f),
	};


	for(int i=0; i<Option::ITEM_COUNT_MAX; ++i) {
		Float2 v = Option::ItemPosList[i];
		Float2 s = Option::ItemSizeList[i];
		Color maskColor		= Color::NONE;
		Color emissiveColor = Color::NONE;
		if(Option::isSelect[i]){
			maskColor	  = Color(255,   0,   0, 230);
			emissiveColor = Color::WHITE;
		}else{
			maskColor = Color(255, 255, 255, 230);
		}


		

		// 画面サイズに合わせて位置、サイズ補正
		v._x *= resScale._w;
		v._y *= resScale._h;
		s._x *= resScale._w;
		s._y *= resScale._h;

		size = Size<s32>(s._x, s._y);
		// 文字描画
		GmRender()->putTexture(_font[i]->getTextureHandle(), v._x, v._y, Size<s32>(s._x, s._y), maskColor, emissiveColor);


		//---- ゲージ描画
		s = Option::GaugeSizeList[i];					// ゲージのサイズを取ってくる
		Size<s32> texSize = pDrawTexList[i]->getSize();	// ゲージ枠のサイズ
		f32	gaugeScaleW	  = s._x / texSize._w;			// ゲージのスケール値
		f32 gaugeScaleH	  = s._y / texSize._h;			// ゲージのスケール値

		// ゲージのオフセット分ずらす
		v._x += offsetGauge[i]._x * resScale._w;
		v._y += offsetGauge[i]._y * resScale._h;

		// ゲージ中描画
		f32	outLineWidth  = 10.0f;						// ゲージの枠の幅
		f32 widthOffset   = outLineWidth * gaugeScaleW;	// どれだけゲージの描画からずらすか
		f32 heightOffset  = outLineWidth * gaugeScaleH;	// どれだけゲージの描画からずらすか
		// ゲージ描画のためのオフセット(ゲージ枠に収まるように計算)
		v._x += (500 + widthOffset) * resScale._w;		
		v._y += heightOffset		* resScale._h;
		f32 w = ((texSize._w * gaugeScaleW) - (widthOffset  * 2.0f)) * resScale._w;
		f32 h = ((texSize._h * gaugeScaleH) - (heightOffset * 2.0f)) * resScale._h;
		size = Size<s32>(w, h);
		// ゲージの中を四角で描画
		drawRect(v._x, v._y, size._w * volumeScales[i], size._h, inGaugeColorList[i]);
		setBlendMode(BM_BLEND_ALPHA);	// drawRectでブレンドモードがNONEになるので設定
		// ゲージアウトライン
		// ゲージ枠分のオフセットを元に戻す
		v._x -= widthOffset  * resScale._w;
		v._y -= heightOffset * resScale._h;
		w	 = texSize._w * gaugeScaleW	* resScale._w;
		h	 = texSize._h * gaugeScaleH	* resScale._h;
		size = Size<s32>(w, h);
		GmRender()->putTexture(pDrawTexList[i]->getTextureHandle(), v._x, v._y, size, maskColor);
		

	}

	setBlendMode(BM_NONE);

	
}

//=============================================================================
//	END OF FILE
//=============================================================================