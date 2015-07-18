//-----------------------------------------------------------------------------
//!
//!	@file	gmSystemMain.cpp
//!	@brief	アプリケーションエントリポイント
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

bool	Global::debugMode    = false;
bool	Global::filterEnable = true;
//-----------------------------------------------------------------------------
//! 終了
//-----------------------------------------------------------------------------
void exitGL()
{
}

//-----------------------------------------------------------------------------
//! マウス関数
//-----------------------------------------------------------------------------
void MouseFunc(s32 button_number, s32 state, s32 x, s32 y)
{
	GmMouseMan()->getMouse()->MouseFunc(button_number, state, x, y);
}

//-----------------------------------------------------------------------------
//!	ホイール
//-----------------------------------------------------------------------------
void MouseWheel(s32 wheel_number, s32 direction, s32 x, s32 y)
{
	GmMouseMan()->getMouse()->MouseWheel(wheel_number, direction, x, y);
}

f32			glFps = 0;

//-----------------------------------------------------------------------------
//! パフォーマンスの描画
//-----------------------------------------------------------------------------
void DrawPerformance()
{
	//-------------------------------------------------------------
	// ここから描画(2D)
	//-------------------------------------------------------------
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
		Size<s32>	windowSize = GmWindow()->getResolution();
		glOrtho(0, windowSize._w, windowSize._h, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		// FPSの描画
		float fps = getGLFps();
		if( fps != 0 ){
			glFps = fps;
		}
		stringstream ss;
		ss << "glFps : ";
		ss << glFps;
		glColor4ub(255,255,255, 255);
		render_string(24.0f, 80.0f, ss.str());


//		GmPerformance()->drawPerformanceBar();

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}




//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void update()
{
	getGLError();
	// パフォーマンス記録開始
	gPerfAnalizer.begin();
	{
		GM_PROFILE("全体", Color(0,255,0,128));
		//-------------------------------------------------------------
		// 描画
		//-------------------------------------------------------------
		GmRender()->begin();
		GmScene()->update();
	}
	// パフォーマンス記録終了と描画
	gPerfAnalizer.end();

	//-------------------------------------------------------------
	// 描画の終了と画面更新
	//-------------------------------------------------------------
	GmRender()->end();
//	GmScene()->drawNoFilter();
	if( Global::debugMode ) { 
		gPerfAnalizer.render();
	}
	getGLError();
	GmRender()->flip();
	getGLError();

	// キー入力もしくはシーンが終了フラグをだしたら
	//if( GetKeyState(VK_ESCAPE) & 0x80 || GmScene()->isExit() ) {
	if (GmScene()->isExit()) {

#if GAME_MODE
//		glutLeaveGameMode();
#endif 
		glutLeaveMainLoop();
	}

	//---- デバッグ描画モード切替
	u32 key  = GetKeyState(VK_F8);
	key		|= GetKeyState(VK_F9);
	static u32	oldKey = 0;
	u32			trigger = (key ^ oldKey) & key;
	oldKey = key;	// 次のために保存
	if(GetKeyState(VK_F8) & trigger){
		Global::debugMode = Global::debugMode ^ true;
	}
	if(GetKeyState(VK_F9) & trigger){
		Global::filterEnable = Global::filterEnable ^ true;
	}

	getGLError();
}

//-----------------------------------------------------------------------------
//! タイマー
//-----------------------------------------------------------------------------
void Timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(16, Timer, 0);
}

//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool Initialize()
{
	// 乱数の初期化
	srand((unsigned) time(NULL));
	
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

//	_CrtSetBreakAlloc(74049);

	// コントローラー作成
	GmControlMan()->createController();
	// マウス作成
	GmMouseMan()->createMouse();

	return true;
}

//-----------------------------------------------------------------------------
//! 渡されたフラグによってフルスクリーン<->ウィンドウに変更
//! @param	[in]	isFullScreen フルスクリーンにするかどうか
//-----------------------------------------------------------------------------
void SystemMain::fullScreen(bool isFullScreen)
{
	s32			nMode = 0;
	DEVMODE		devMode;
	Size<s32>	windowSize = GmWindow()->getResolution();
	if(isFullScreen) {
		while(EnumDisplaySettings(nullptr, nMode++, &devMode)){
			// 現在の解像度をサポートしているかどうか
			if(devMode.dmPelsWidth != windowSize._w || devMode.dmPelsHeight != windowSize._h) {
				continue;
			}
			// 32bitカラーサポートしてるかどうか
			if(devMode.dmBitsPerPel != 32) continue;
			// テストして切り替えられるなら
			if(ChangeDisplaySettings(&devMode, CDS_TEST) == DISP_CHANGE_SUCCESSFUL) break;
		}
		// 切り替える
		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
		glutFullScreen();
	}else{
		ChangeDisplaySettings(nullptr, 0);

		//------------------------------------------------------------
		// ウィンドウサイズをクライアントサイズから計算
		//------------------------------------------------------------
		Size<s32>	windowSize = GmWindow()->getResolution();
		RECT		rect = { 0, 0, windowSize._w, windowSize._h };
		u32			style = WS_OVERLAPPEDWINDOW;

		//---- 画面中央にセンタリングする
		s32		w = rect.right - rect.left;
		s32		h = rect.bottom - rect.top;
		s32		x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
		s32		y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

		// ウィンドウの初期座標
		glutInitWindowPosition(x, y);
		glutReshapeWindow(w, h);

	}
	
}

//-----------------------------------------------------------------------------
//! メインループ
//!	@param	[in]	windowClassName	ウィンドウの名前
//!	@param	[in]	windowTitleName	ウィンドウのタイトル名
//-----------------------------------------------------------------------------
int SystemMain::mainLoop(const char windowClassName[], const char windowTitleName[])
{

	//------------------------------------------------------------
	// ウィンドウサイズをクライアントサイズから計算
	//------------------------------------------------------------
	Size<s32>	windowSize	= GmWindow()->getResolution();
	RECT		rect		= { 0, 0, windowSize._w, windowSize._h};
	u32			style		= WS_OVERLAPPEDWINDOW;

	//AdjustWindowRect(&rect, style, false);

	//---- 画面中央にセンタリングする
	s32		w = rect.right  - rect.left;
	s32		h = rect.bottom - rect.top ;
	s32		x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
	s32		y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

	// 終了関数の登録
	atexit(exitGL);

	//---- GLの初期化
	// ウィンドウの初期座標
	glutInitWindowPosition(x, y);
	// ウィンドウの幅,高さ
	glutInitWindowSize(w, h);
	glutInit(&__argc, __argv);
	// ウィンドウの×ボタンを押したらglutMainLoopから戻ってくる設定
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	// ウィンドウを作成
	glutCreateWindow(windowTitleName);



#if GAME_MODE
	fullScreen(true);
#endif

	

	// 描画方法の指定
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	getGLError();

	char buf[255];

	sprintf_s(buf, "OpenGLVersion : (%s)", glGetString(GL_VERSION));

	sprintf_s(buf, "OpenGLVersion : (%s)", glGetString(GL_SHADING_LANGUAGE_VERSION));
	

	//-------------------------------------------------------------
	// システムの初期化
	//-------------------------------------------------------------
	if( GmShader()->init() == false ) return false;		// シェーダー
	getGLError();
	if( GmRender()->init() == false ) return false;		// 描画
	getGLError();
	if( GmScene ()->init() == false ) return false;		// シーン
	getGLError();

	// 初期化コールバック
	onInitialize();

	//---- メインループ
	if( Initialize() ) {

		getGLError();
		// 更新関数設定
		glutDisplayFunc(update);
		getGLError();
		// マウスホイール関数の設定
		glutMouseWheelFunc(MouseWheel);
		getGLError();
		glutMouseFunc(MouseFunc);
		getGLError();
		// タイマー関数設定
		glutTimerFunc(16, Timer, 0);
		getGLError();
		glutMainLoop();

	}

	// 解放コールバック
	onFinalize();

	GmScene ()->cleanup();
	GmShader()->cleanup();	// シェーダー
	GmRender()->cleanup();	// 描画


	// 終了 (ウィンドウメッセージのwparamを戻り値にする)
	return 0;
}

//=============================================================================
//	END OF FILE
//=============================================================================
