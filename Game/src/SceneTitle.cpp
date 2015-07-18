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
GM_TYPE_IMPLEMENT(SceneTitle, "タイトルシーン");

static const int	ITEM_COUNT_MAX = 3;

//! 項目が選択されてるかどうか
bool		isSelect[ITEM_COUNT_MAX] = {
	false,
	false,
	false,
};

//! タイトルの項目のポジションリスト
Float2		TitleItemPosList[ITEM_COUNT_MAX] = {
	Float2(50.0f,500.0f),
	Float2(50.0f,650.0f),
	Float2(50.0f,800.0f),
};
Float2		TitleItemSizeList[ITEM_COUNT_MAX] = {
	Float2(387.0f, 71.0f),
	Float2(199.0f, 71.0f),
	Float2(165.0f, 71.0f),
};

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
SceneTitle::SceneTitle()
: _backGroundImage(nullptr)
, _title		  (nullptr)
, _Abutton		  (nullptr)
{
	for( s32 i=0; i<3; ++i ){
		_font[i] = nullptr;
		SAFE_NEW(_font[i]);
	}
	if( !_font[0]->loadAndTexImage2D("../Image/gameStart.png") ){
		MessageBox(NULL, L"ゲームスタートボタン画像読み込み失敗", L"エラー", MB_OK);
	}
	if( !_font[1]->loadAndTexImage2D("../Image/option.png") ){
		MessageBox(NULL, L"オプション文字画像失敗", L"エラー", MB_OK);
	}
	if( !_font[2]->loadAndTexImage2D("../Image/exit.png") ){
		MessageBox(NULL, L"Exit文字画像読み込み失敗", L"エラー", MB_OK);
	}

	SAFE_NEW(_title);

	
	if( !_title->loadAndTexImage2D("../Image/Title.png") ){
		MessageBox(NULL, L"タイトル文字画像読み込み失敗", L"エラー", MB_OK);
	}

	SAFE_NEW(_Abutton);
	if( !_Abutton->loadAndTexImage2D("../Image/titleAButton.png") ) {
		MessageBox(NULL, L"Aボタン説明用画像読み込み失敗", L"エラー", MB_OK);
	}

	// 選択中の項目初期化
	_select = SELECT_NEWGAME;
	// 画像クラスのインスタンス
	SAFE_NEW(_backGroundImage);
	// フォントのインスタンス
	_titleFont = new GLFont(L"MS明朝", 40);

	//if (!_backGroundImage->loadAndTexImage2D("../Image/test.jpg")){
	if( !_backGroundImage->loadAndTexImage2D("../Image/back_ground.jpg") ){
	//if (!_backGroundImage->loadAndTexImage2D("../Image/darkCastle.jpg")){
		MessageBox(NULL, L"背景の画像の読み込みに失敗しました。", L"エラー", MB_OK);
	}
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
SceneTitle::~SceneTitle()
{
	// サウンド停止
	//ISoundManager()->stopMusic(SoundPlayerManager::BGM_TITLE);
	SAFE_DELETE(_titleFont);
	SAFE_DELETE(_title);
	SAFE_DELETE(_Abutton);
	SAFE_DELETE(_backGroundImage);
	for( s32 i=0; i<3; ++i ){
		SAFE_DELETE(_font[i]);
	}
	delete _titleFont;
	delete _backGroundImage;
}
//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool SceneTitle::init()
{
	//if( _backGroundImage->load("../Image/TitleBackGround.jpg") == false){
	
	// ひとつ前の音楽停止
	IBGMManager()->stopPrevMusic();
	// タイトルのBGM再生
	IBGMManager()->playMusic(BGMSoundPlayerManager::BGM_TITLE);

	// 終了フラグOFF
	_isExit = false;

	// アルファ比較値設定
	glAlphaFunc(GL_GEQUAL, 0.1f);

	for(s32 i=0; i<SELECT_MAX; ++i){
		_cantSelect[i] = false;
	}

	// オプションは作ってないので選択できないようにしている
	_cantSelect[SELECT_OPTION] = false;

	return true;
}

//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void SceneTitle::update()
{

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
void SceneTitle::checkKey(Controller& control)
{
	enum {
		KEY_U = (1 << 0),		// 1 : 0001
		KEY_D = (1 << 1),		// 2 : 0010
		KEY_E = (1 << 2),		// 4 : 0100		
		KEY_B = (1 << 3),		// 8 : 1000		
	};

	u32	key = 0;
	key |= (GetKeyState(VK_UP) & 0x80) ? KEY_U : 0;
	key |= (GetKeyState(VK_DOWN) & 0x80) ? KEY_D : 0;
	key |= (GetKeyState(VK_RETURN) & 0x80) ? KEY_E : 0;
	key |= (GetKeyState(VK_ESCAPE) & 0x80) ? KEY_B : 0;

	static u32 oldKey = 0;
	u32		   trigger = (key ^ oldKey) & key;

	oldKey = key;


	_keyUp		=	control.GetPushState(Controller::PAD_LEFT_U_STICK) |
					control.GetPushState(Controller::PAD_UP) |
					((trigger & KEY_U) ? true : false);
	_keyDown	=	control.GetPushState(Controller::PAD_LEFT_D_STICK) |
					control.GetPushState(Controller::PAD_DOWN) |
					((trigger & KEY_D) ? true : false);

	_keySelect	=	control.GetPushState(Controller::PAD_A) |
					((trigger & KEY_E) ? true : false);

	_keyBack	=	control.GetPushState(Controller::PAD_B) |
					((trigger & KEY_B) ? true : false);


}

//-----------------------------------------------------------------------------
//! 選択しているものによる更新
//-----------------------------------------------------------------------------
void SceneTitle::updateSelect()
{
	// 選択しているものによって処理分岐
	switch (_select)
	{
		case SELECT_NEWGAME:
		{
			// 選択音再生
			ISEManager()->playMusic(SESoundPlayerManager::SE_BUTTON);
			// ゲームシーンへ
			GmScene()->jump(SceneGame::TYPE);
			break;
		}
		case SELECT_OPTION:
		{
			//オプションへ
			GmScene()->jump(SceneOption::TYPE);
			break;
		}
		case SELECT_EXIT:
		{
			// 終了フラグ立てる
			_isExit = true;
			break;
		}
	}

}


//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void SceneTitle::update(Controller& control)
{
	// キー判定
	checkKey(control);

	// キー処理による移動	
	do{
		if( _keyDown ){
			_select = SELECT_STATE(_select + 1);
		}else if( _keyUp ){
			_select = SELECT_STATE(_select - 1);
		}
	}while(_cantSelect[_select]);

	// 範囲外指定
	if( _select < SELECT_NEWGAME ) _select = SELECT_EXIT;
	if( _select > SELECT_EXIT	 ) _select = SELECT_NEWGAME;
	// すべてfalseに
	for(int i=0; i<ITEM_COUNT_MAX; ++i){
		isSelect[i] = false;
	}
	// 選択されているところをtrueに
	isSelect[_select] = true;

	// 選択ボタンが押されたら
	if(_keySelect){
		// 選択されているもの別の更新
		updateSelect();
	}
	
	// 戻るキーをおしたらEXITボタンを選択する
	if(_keyBack){
		_select = SELECT_EXIT;
	}
	
	// 背景描画
	render();
}

//-----------------------------------------------------------------------------
//! フィルターの影響しない描画
//-----------------------------------------------------------------------------
void SceneTitle::drawNoFilter()
{
}

//-----------------------------------------------------------------------------
//! 描画
//-----------------------------------------------------------------------------
void SceneTitle::render()
{
#if 1
	// 背景描画
	Size<s32> windowSize = GmWindow()->getResolution();
	GmRender()->putTexture(_backGroundImage->getTextureHandle(), 0, 0, windowSize, Color::WHITE);



	setBlendMode(BM_BLEND_ALPHA);

	
	// 最大解像度との比率
	Size<f32> resScale = GmWindow()->getRateResMax();
	Size<s32> size = Size<s32>(750.0f * resScale._w, 153.0f * resScale._h );

	// タイトル文字表示
	GmRender()->putTexture(_title->getTextureHandle(), 550 * resScale._w, 200 * resScale._h, size, Color::WHITE, Color::DKGRAY);

	//---- タイトルの選択項目の描画
	for(int i=0; i<ITEM_COUNT_MAX; ++i){
		// 項目の座標取得
		Float2 v = TitleItemPosList[i];
		// 項目のサイズ取得
		Float2 s = TitleItemSizeList[i];
		// 選択されているかどうかによって色を変える
		Color maskColor;
		if( isSelect[i] ) {
			maskColor = Color::RED;				// 赤色
		}else{
			if( !_cantSelect[i] ){
				maskColor = Color::WHITE;		// 白色
			}else{
				maskColor = Color(64, 64, 64);	// 灰色
			}
		}

		size = Size<s32>(s._x * resScale._w, s._y * resScale._h);
		GmRender()->putTexture(_font[i]->getTextureHandle(), v._x * resScale._w, v._y * resScale._h, size, maskColor, Color::DKGRAY);
	}

	size = Size<s32>(100.0f * resScale._w, 25.0f * resScale._h);

	// Aボタン説明用
	GmRender()->putTexture(_Abutton->getTextureHandle(), 50.0f * resScale._w, 1000.0f * resScale._h, size);

	setBlendMode(BM_NONE);
#endif
}

//=============================================================================
//	END OF FILE
//=============================================================================