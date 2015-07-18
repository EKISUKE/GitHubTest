//-----------------------------------------------------------------------------
//!
//!	@file	SceneGame.cpp
//!	@brief	ゲーム中
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"



//-----------------------------------------------------------------------------
//	ゲームシーン SceneGame
//-----------------------------------------------------------------------------
GM_TYPE_IMPLEMENT(SceneGame, "ゲーム");


//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
DebugCPUTimer loopTimer;	// ループタイマー
DebugCPUTimer updateTimer;	// 更新用タイマー
DebugCPUTimer renderTimer;	// 描画用タイマー
DebugCPUTimer debugTimer;	// それぞれのデバッグ用タイマー

f32	Global::deltaTime;

//-----------------------------------------------------------------------------
//! 味方を追加
//-----------------------------------------------------------------------------
bool SceneGame::addAlly(AllyBase*	pAlly)
{
	for( s32 i=0; i<ALLY_COUNT_MAX; i++ )
	{
		if( _pAlly[i] == NULL )
		{
			_pAlly[i] = pAlly;
			// 分隊に登録
			_pPlayerSquad->addMenber(_pAlly[i]);
			// 分隊情報を設定
			_pAlly[i]->setSquad(_pPlayerSquad);
			// キー情報設定
			IObjDataManager()->addAllyData(_pAlly[i]);
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
//! 敵を追加
//-----------------------------------------------------------------------------
bool SceneGame::addEnemy(EnemyBase*	pEnemy)
{
	static s32	squadNum = 0;
	static s32  eneCount = 0;

	s32	   index = eneCount + (ENEMY_SQUAD_MEM_MAX * squadNum);

	if( _pEnemy[index] == nullptr )
	{
		_pEnemy[index] = pEnemy;
		// 分隊に登録
		_pEnemySquad[squadNum]->addMenber(_pEnemy[index]);
		// 分隊情報を設定
		_pEnemy[index]->setSquad(_pEnemySquad[squadNum]);
		// キー情報設定
		IObjDataManager()->addEnemyData(_pEnemy[index]);
		// 登録したのでカウントを増やす
		eneCount++;
		if( eneCount >= ENEMY_SQUAD_MEM_MAX ) {
			squadNum++;
			eneCount = 0;
		}
		return true;
	}
	return false;
}


//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
SceneGame::SceneGame()
: _pMouse				(nullptr)	// マウス
, _pCamera				(nullptr)	// カメラ
, _pControl				(nullptr)	// コントローラー
, _pFont				(nullptr)	// フォント
, _pPlayer				(nullptr)	// プレイヤー
, _pPlayerSquad			(nullptr)	// プレイヤーの分隊
//, _pEnemyLeader			(nullptr)	// 敵リーダー
//, _pEnemySquad			(nullptr)	// 敵の分隊(テスト)
, _pLight				(nullptr)	// ライト
, _pSkyBox				(nullptr)	// スカイボックス
, _pGameClearWord		(nullptr)	// ゲームクリア文字
, _pGameOverWord		(nullptr)	// ゲームオーバー文字
, _pXButtonImage		(nullptr)	// XButton画像
, _pXButtonUI			(nullptr)	// XButton用
, _pUpdatePerformance	(nullptr)	// 更新のパフォーマンス
, _pRenderPerformance	(nullptr)	// 描画のパフォーマンス
, _isShadow				(false)		// シャドウ描画フラグ
{
	
	for( s32 i=0; i<ENEMY_COUNT_MAX; i++ )
	{
		_pEnemy[i] = nullptr;
	}
	for( s32 i=0; i<ALLY_COUNT_MAX; i++ )
	{
		_pAlly[i] = nullptr;
	}
	_pMap = nullptr;

	for( s32 i=0; i<2; i++ )
	{
		_pMissionWord[i] = nullptr;
		SAFE_NEW(_pMissionWord[i]);
	}

	// マウスの初期化
	_pMouse	  = GmMouseMan()->getMouse();
	
	
	// コントローラー初期化
	_pControl = GmControlMan()->getController(1);
	

	// カメラの初期化
	SAFE_NEW(_pCamera);

	// 作成したカメラをストック
	GmCameraMan()->createCamera(_pCamera);

	// フォントの初期化
	_pFont = new GLFont(L"MS明朝", 24);

	// もう一度当たり判定管理クラス初期化
	GmCollisionMan()->Init();
	

	// プレイヤー初期化
	SAFE_NEW(_pPlayer);
	// キーの情報設定
	IObjDataManager()->setPlayer(_pPlayer);


	// プレイヤーの分隊の初期化
	SAFE_NEW(_pPlayerSquad);
	// プレイヤーを分隊長に
	_pPlayerSquad->setLeader(_pPlayer);


	//---- 敵初期化
	for( s32 i=0; i<ENEMY_SQUAD_MAX; i++ ) {
		_pEnemyLeader[i] = nullptr;
		_pEnemySquad[i]  = nullptr;
		// 敵リーダーの初期化
		SAFE_NEW(_pEnemyLeader[i]);
		_pEnemyLeader[i]->setPosition(Vector3(0.0f));
		// 敵の分隊初期化
		SAFE_NEW(_pEnemySquad[i]);
		_pEnemySquad[i]->setLeader(_pEnemyLeader[i]);
	}


	// ライトの初期化
	SAFE_NEW(_pLight);
	
	// スカイボックスの初期化
	SAFE_NEW(_pSkyBox);
	_pSkyBox->init();

#if 1
	/*_pSkyBox->loadSkyBox("../Image/SkyBoxTexture/skybox/",
						 "front.jpg",
						 "back.jpg",
						 "left.jpg",
						 "right.jpg",
						 "top.jpg",
						 "top.jpg");*/

	/*_pSkyBox->loadSkyBox("../Image/SkyBoxTexture/skybox/hdr/",
						"cubemap_intensity10Test._c04.hdr",
						"cubemap_intensity10Test._c05.hdr",
						"cubemap_intensity10Test._c01.hdr",
						"cubemap_intensity10Test._c00.hdr",
						"cubemap_intensity10Test._c02.hdr",
						"cubemap_intensity10Test._c02.hdr");*/

	_pSkyBox->loadSkyBox("../Image/SkyBoxTexture/h_skyboxpack2_lagoon/hdr/",
						"cubemap_c04.hdr",
						"cubemap_c05.hdr",
						"cubemap_c01.hdr",
						"cubemap_c00.hdr",
						"cubemap_c02.hdr",
						"cubemap_c03.hdr");
#else
	_pSkyBox->loadSkyBox("../Image/SkyBoxTexture/h_skyboxpack2_lagoon/",
						 "lagoon_north.bmp",
						 "lagoon_south.bmp",
						 "lagoon_west.bmp",
						 "lagoon_east.bmp",
						 "lagoon_up.bmp",
						 "lagoon_down.bmp");
#endif


	//---- 敵の初期化
	for( s32 j=0; j<ENEMY_SQUAD_MAX; ++j ) {
		// 攻撃タイプ
		for( s32 i=0; i<ENEMY_ATTACKER; ++i )
		{
			addEnemy(new EnemyAttacker());
		}

		// 防御タイプ
		for( s32 i=0; i<ENEMY_DEFFENDER; ++i )
		{
			addEnemy(new EnemyDeffender());
		}
	}


	//---- 味方初期化

	// 攻撃タイプ
	for( s32 i=0; i<ALLY_ATTACKER; ++i )
	{
		addAlly(new AllyAttacker());
	}
	// 防御タイプ
	for( s32 i=0; i<ALLY_DEFFENDER; ++i )
	{
		addAlly(new AllyDeffender());
	}
	// 回復タイプ
	for( s32 i=0; i<ALLY_HEALER; ++i )
	{
		addAlly(new AllyHealer());
	}

	//---- ゲームオーバークリア文字読み込み
	SAFE_NEW(_pGameClearWord);
	_pGameClearWord->loadAndTexImage2D("../Image/gameClear.png");
	SAFE_NEW(_pGameOverWord);
	_pGameOverWord->loadAndTexImage2D("../Image/gameOver.png");

	_pMissionWord[0]->loadAndTexImage2D("../Image/mission1.png");
	_pMissionWord[1]->loadAndTexImage2D("../Image/mission2.png");

	SAFE_NEW(_pXButtonImage);
	_pXButtonImage->loadAndTexImage2D("../Image/XButton.png", true);
	SAFE_NEW(_pXButtonUI   );
	_pXButtonUI->setTexture(_pXButtonImage);
	_pXButtonUI->setSize(Vector3(26.0f, 26.0f, 0.0f));
	_pXButtonUI->setScale(Vector3(0.3f, 0.3f, 0.0f));

	SAFE_NEW(_pMap);

	//---- パフォーマンス関連初期化

	// パフォーマンスの初期化
	SAFE_NEW(_pUpdatePerformance);
	SAFE_NEW(_pRenderPerformance);
	// 更新は赤
	_pUpdatePerformance->setColor(Color(255, 0, 0));
	// 描画は黄
	_pRenderPerformance->setColor(Color(255, 255, 0));

	// メインに設定
	_pUpdatePerformance->setMainPerformance();
	_pRenderPerformance->setMainPerformance();

	// デルタタイム用
	Global::deltaTime = 0;
	
	
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
SceneGame::~SceneGame()
{

	// フォントの解放
	SAFE_DELETE(_pFont);
	
	// プレイヤーの解放
	SAFE_DELETE(_pPlayer);

	// ゲームクリアオーバー文字解放
	SAFE_DELETE(_pGameClearWord);
	SAFE_DELETE(_pGameOverWord);

	SAFE_DELETE(_pXButtonImage);
	SAFE_DELETE(_pXButtonUI);


	for( s32 i=0; i<ENEMY_COUNT_MAX; i++ ){
		SAFE_DELETE(_pEnemy[i]);
	}

	for( s32 i=0; i<ALLY_COUNT_MAX; i++ ){
		SAFE_DELETE(_pAlly[i]);
	}

	for( s32 i=0; i<2; i++ )
	{
		SAFE_DELETE(_pMissionWord[i]);
	}

	// マップの解放
	SAFE_DELETE(_pMap);

	// あたり判定クラスの解放
	GmSystemCollision()->deleteObject();
}
//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool SceneGame::init()
{
	glEnable(GL_DEPTH_TEST);	// Ｚバッファを有効にする
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDepthMask(GL_TRUE);

	// フォントの初期化
	_pFont->Initialize(L"MS明朝", 24);
	// プレイヤー初期化
	_pPlayer->Initialize();

	// カメラの初期化
	Vector3 rot = _pPlayer->getRotation();
	rot._x = TO_RADIAN(rot._x);
	rot._y = TO_RADIAN(rot._y + 180.0f);
	rot._z = TO_RADIAN(rot._z);

	_pCamera->init(_pPlayer->getPosition(), rot);

	// カスケードシャドウのOrtho行列初期化
	ICasCaded()->initOrthoMatrix();

	// ひとつ前の音楽停止
	IBGMManager()->stopPrevMusic();
	// タイトルのBGM再生
	IBGMManager()->playMusic(BGMSoundPlayerManager::BGM_GAME);


	// 敵の初期化
	for( s32 i=0; i<ENEMY_SQUAD_MAX; ++i ) {
		_pEnemyLeader[i]->Initialize();
	}

	for( s32 i=0; i<ENEMY_COUNT_MAX; i++ ){
//		_pEnemy[i]->setLeaderPos(_pEnemyLeader->getPosition());
		_pEnemy[i]->Initialize();
	}
	// 味方の初期化
	for( s32 i=0; i<ALLY_COUNT_MAX; i++ ){
//		_pAlly[i]->setLeaderPos(_pPlayer->getPosition());
		_pAlly[i]->Initialize();
	}
	// マップの初期化
	_pMap->Initialize();

	_gameClear	 = false;
	_gameOver	 = false;
	_isExit		 = false;

	// 城に近いフラグOFF
	_pPlayer->setNearCastle(false);

	_waitGoTitle = 150.0f;

	_allyRevivalCountMax  = 30;
	_enemyRevivalCountMax = 20;
	

	loopTimer.Reset();

	glFrontFace(GL_CW);

	return true;
}

//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void SceneGame::update()
{
	
	// ループタイマー
	loopTimer.EndTimer();
	// デルタタイム取得
	Global::deltaTime = (f32)( loopTimer.getTotalTime() * 0.0625f );

	// 地面をすり抜ける可能性があるので、制限する
	if( Global::deltaTime >= 10.0f ){
		Global::deltaTime = 10.0f;
	}
	loopTimer.StartTimer();

	
	
	// タイマーの開始
	updateTimer.StartTimer();

	_pUpdatePerformance->StartPerformance();

	s32 state = 0;

	// ゲーム更新
	{
		GM_PROFILE("ゲーム更新", Color(255,0,0,255));
		update(state);
	}

	// ゲームクリアかオーバー時のみ戻れる
	if( _gameClear || _gameOver ) {
		_waitGoTitle -= 1.0f * Global::deltaTime;
		if( _waitGoTitle <= 0 )
		{
			GmScene()->jump(SceneTitle::TYPE);
		}
	}

	// ステートによってゲーム終了決定
	if( state == 1 && !_gameOver) {
		_gameClear = true;
	}
	// ゲームクリアしてないかつ、城がアクティブ状態なら
	if( state == 2 && !_gameClear && _pMap->getCastle()->isActive()) {
		_gameOver  = true;
	}

	_pUpdatePerformance->EndPerformance();

	// タイマーの終了
	updateTimer.EndTimer();


	
	
	_pRenderPerformance->StartPerformance();
	
	// タイマーの開始
	renderTimer.StartTimer();

	// ゲーム描画
	{
		GM_PROFILE("ゲーム更新", Color(0,0,255,255));
		render();
	}

	if (GetKeyState(VK_ESCAPE) & 0x80){
		_isExit = true;
	}

	// タイマーの終了
	renderTimer.EndTimer();
	_pRenderPerformance->EndPerformance();

}

//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void SceneGame::update(s32& state)
{
	_pMouse->Update();

	//-------------------------------------------------------------
	// 行列の初期設定
	//-------------------------------------------------------------
	//---- 投影行列の初期化
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();				// 初期化(リセット)

	//視界角、アスペクト比(ウィンドウの幅/高さ), 描画する範囲(最も近い距離,最も遠い距離)
	glLoadMatrixf((GLfloat*)&GmCameraMan()->getCurrentCamera()->getProjMatrix());
	//---- ワールド/ビュー行列の初期化
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();				// 初期化(リセット)

	// プレイヤーの更新
	_pPlayer->Update();
	
	// 味方更新
	// 一番近い敵を狙う
	for( s32 i=0; i<ALLY_COUNT_MAX; i++ )
	{
		// 目標座標(自分の分隊での標準位置を取得)
		_pAlly[i]->Update();
	}

	// 敵更新
	for( s32 i=0; i<ENEMY_SQUAD_MAX; i++ ) { 
		_pEnemyLeader[i]->Update();
	}

	// 一番近い敵を狙う
	static Vector3	 diffencePos = Vector3(0,0,0);
	for( s32 i=0; i<ENEMY_COUNT_MAX; i++ )
	{
//		Vector3 leaderPos = _pEnemySquad->getMyPostion(_pEnemy[i]);
//		_pEnemy[i]->setLeaderPos(leaderPos);
		_pEnemy[i]->Update();
	}

	// 仮マップのアップデート
	_pMap->Update();


	GmCollisionMan()->Update();


	// 城に近いフラグ更新
	bool isNear = ICastleCol()->isNearCastleObject(_pPlayer->getColAABB(), _pPlayer->getPosition());
	_pPlayer->setNearCastle( isNear );


	// ゲーム終了処理
	if( _pPlayer->getGameOver() )
	{
		state = 2;
	}else if( _pMap->getCastle()->isCrash() ){
		// 城が壊れたら
		state = 1;
	}



	// プレイヤー情報一時保存
	static Vector3  PlayerPos;
	static f32		PlayerRot;
	static Vector3  PlayerAttackPos;
	static f32		PlayerAttackRad;
	
	// 注視点との距離を設定
	_pCamera->changeLookAtLength(_pMouse->getChangeWheel() * 3.0f);
	// カメラの更新
	_pCamera->update(_pControl[0], _pPlayer->getPosition(), _pMouse->getOffset());


	Matrix	viewMatrix = _pCamera->getViewMatrix();
	
	GmScene()->setViewMatrix(viewMatrix);

	// ライトの更新
	_pLight->update();
	Matrix identity = Matrix::IDENTITY;
	Vector3 lightDir = _pLight->getLightDir();
	ICasCaded()->update(identity);

	debugTimer.StartTimer();

	// パーティクル更新
	IParticleMan()->update();

	// 敵味方復活
	revival();

	debugTimer.EndTimer();

}

//-----------------------------------------------------------------------------
//! 3D描画
//-----------------------------------------------------------------------------
void SceneGame::draw3D()
{
	GLfloat white[]	 = { 1.0f, 1.0f, 1.0f, 1.0f };

	Vector3 CamPos = _pCamera->getPosition();


	// マップの描画
	_pMap->Render(_isShadow);

	// 今までの色を初期化
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);

	// 味方描画
	for( s32 i=0; i<ALLY_COUNT_MAX; i++ )
	{
		_pAlly[i]->Render(_isShadow);
	}

	// 敵描画
	for( s32 i=0; i<ENEMY_COUNT_MAX; i++ )
	{
		_pEnemy[i]->Render(_isShadow);
	}

	// プレイヤーの描画
	_pPlayer->Render(_isShadow);
	
	// 敵リーダー描画
	for( s32 i=0; i<ENEMY_SQUAD_MAX; i++ ) { 
		_pEnemyLeader[i]->Render(_isShadow);
	}

	// 今までの色を初期化
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);

}

//-----------------------------------------------------------------------------
//! 2D描画
//-----------------------------------------------------------------------------
void SceneGame::draw2D()
{
	// プレイヤーのステータス描画
	_pPlayer->StatusRender();
	
	gameEndRender();

	missionRender();
}

//-----------------------------------------------------------------------------
//! ミッション文字描画
//-----------------------------------------------------------------------------
void SceneGame::missionRender()
{
	glDisable(GL_ALPHA_TEST);
	setBlendMode(BM_BLEND_ALPHA);
	// 描画するミッション番号
	static u8   popMissionNum = 0;
	// カウント
	static s32	count = 0;
	static f32  w,h,x,y;
	// 移動カウント
	static s32	moveCount = 0;
	// 移動カウント最大値
	static const s32		moveCountMax = 10;
	// 大きいときの描画位置
	static const Vector3	popPos  = Vector3( 800.0f, 400.0f, 0.0f);
	// 小さいときの描画位置
	static const Vector3	sidePos = Vector3(1500.0f, 100.0f, 0.0f);

	
	// 城、城壁のインスタンス取得
	CastleWall* wall	= _pMap->getWall();
	Castle*		castle  = _pMap->getCastle();
	Texture*	drawTex = nullptr;
	// 城、城壁の状態からミッション文字変更
	if( !wall->isCrash() ){
		if( popMissionNum == 0 ){
			count	  = 100;
			moveCount = 0;
		}
		popMissionNum = 1;
		drawTex = _pMissionWord[0];
	}else if( !castle->isCrash()) {
		if( popMissionNum == 1 ){
			count	  = 100;
			moveCount = 0;
		}
		popMissionNum = 0;
		drawTex = _pMissionWord[1];
	}

	// 最大解像度との比率
	Size<f32> resScale = GmWindow()->getRateResMax();

	// 0以上なら
	if( count > 0 ){
		// 大きく画面の真ん中に描画
		count = (s32)( ceil( (f32)(count) - ( 1.0f * Global::deltaTime ) ) );
		if(drawTex != nullptr) {
			w = 389.0f	  * resScale._w;
			h = 83.0f	  * resScale._h;
			x = popPos._x * resScale._w;
			y = popPos._y * resScale._h;
			GmRender()->putTexture(drawTex->getTextureHandle(), x, y, Size<s32>( (s32)w, (s32)h ) );
		}
	}else{
		// 以下なら縮小しながら右上に移動
		if( moveCount < moveCountMax ){
			moveCount++;
		}
		if(drawTex != nullptr) {
			w = LinearInterpolation(	389.0f,		140.0f, (f32)moveCount, (f32)moveCountMax) * resScale._w;
			h = LinearInterpolation(	 83.0f,		 30.0f, (f32)moveCount, (f32)moveCountMax) * resScale._h;
			x = LinearInterpolation( popPos._x, sidePos._x, (f32)moveCount, (f32)moveCountMax) * resScale._w;
			y = LinearInterpolation( popPos._y, sidePos._y, (f32)moveCount, (f32)moveCountMax) * resScale._h;
			GmRender()->putTexture(drawTex->getTextureHandle(), x, y, Size<s32>( (s32)w, (s32)h ) );
		}
	}
	setBlendMode(BM_NONE);
	glEnable(GL_ALPHA_TEST);
}
//-----------------------------------------------------------------------------
//! ゲームクリアオーバー文字描画
//-----------------------------------------------------------------------------
void SceneGame::gameEndRender()
{
	// フラグが足ってなかったら処理しない
	if(!_gameClear && !_gameOver) return;

	setBlendMode(BM_BLEND_ALPHA);

	// 最大解像度との比率
	Size<f32> resScale = GmWindow()->getRateResMax();
	// フラグによって文字描画
	if( _gameClear ) {
		
		GmRender()->putTexture(_pGameClearWord->getTextureHandle(),
								500 * resScale._w, 400 * resScale._h,
								Size<s32>(886 * resScale._w, 127 * resScale._h));
	}else if( _gameOver ) {
		GmRender()->putTexture(_pGameOverWord->getTextureHandle(),
								550 * resScale._w, 400 * resScale._h,
								Size<s32>( 738 * resScale._w, 106 * resScale._h ) );
	}

	setBlendMode(BM_NONE);
}


//-----------------------------------------------------------------------------
//! 復活関数
//-----------------------------------------------------------------------------
void SceneGame::revival()
{
	s32	nowTime = timeGetTime();

	bool isLeaderDead = false; 

	//--- 敵復活
	static s32	oldEneRevivalTime = 0;
	// 一定時間たったら
	if( nowTime - oldEneRevivalTime > 10000 ) {

		for( s32 i=0; i<ENEMY_SQUAD_MAX; i++ ) {
			// リーダーが死亡していたら
			if( _pEnemyLeader[i]->isDead() ) {
				revivalEnemy(_pEnemyLeader[i]);
				oldEneRevivalTime = nowTime;
				isLeaderDead = true;
			}
		}

		// 死亡していなければ
		if( !isLeaderDead ) {
			
			for(s32 i=0; i<ENEMY_COUNT_MAX; ++i) {
				if( _pEnemy[i]->isDead() ) {
					revivalEnemy(_pEnemy[i]);
					oldEneRevivalTime = nowTime;
					break;
				}
			}
		}
	}

	

	//---- 味方復活
	static s32	oldAllyRevivalTime = 0;
	// 一定時間たったら
	if( nowTime - oldAllyRevivalTime > 5000 ) {
		for(s32 i=0; i<ALLY_COUNT_MAX; ++i) {
			// 死亡フラグが立っていたら
			if( _pAlly[i]->isDead() ) {
				revivalAlly(_pAlly[i]);
				// タイム更新
				oldAllyRevivalTime = nowTime;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//! 敵復活関数
//-----------------------------------------------------------------------------
void SceneGame::revivalEnemy(Man*	pEnemy)
{
	if(_enemyRevivalCountMax <= 0)	return;
	// 初期化する
	pEnemy->Initialize();
	// 復活回数を減らす
	_enemyRevivalCountMax--;
}

//-----------------------------------------------------------------------------
//! 味方復活関数
//-----------------------------------------------------------------------------
void SceneGame::revivalAlly(Man* pAlly)
{
	if(_allyRevivalCountMax <= 0)	return;
	// 初期化する
	pAlly->Initialize();
	// 復活回数を減らす
	_allyRevivalCountMax--;
}

//-----------------------------------------------------------------------------
//! ステータスなどフィルターの影響しないものの描画
//-----------------------------------------------------------------------------
void SceneGame::drawNoFilter()
{
	// おもにデバッグ描画
	//glDisable(GL_LIGHTING);

	
		

if( Global::debugMode ) {
	

	/*
	float length = 100.0f;

	Line axisX;
	axisX._position[0] = Vector3(0,0,0);
	axisX._position[1] = Vector3(length,0,0);
	glColor4f(1,0,0,1);
	axisX.DebugRender();

	Line axisY;
	axisY._position[0] = Vector3(0,0,0);
	axisY._position[1] = Vector3(0,length,0);
	glColor4f(0,1,0,1);
	axisY.DebugRender();

	Line axisZ;
	axisZ._position[0] = Vector3(0,0,0);
	axisZ._position[1] = Vector3(0,0,length);
	glColor4f(0,0,1,1);
	axisZ.DebugRender();

	_pLight->DebugRender();

	glColor4f(1,1,1,1);

	*/
	
	_pPlayer->debugRender();

	for( s32 i=0; i<ALLY_COUNT_MAX; ++i )
	{
		_pAlly[i]->debugRender();
	}

	for( s32 i=0; i<ENEMY_COUNT_MAX; ++i )
	{
		_pEnemy[i]->debugRender();
	}
	
	for( s32 i=0; i<ENEMY_SQUAD_MAX; ++i ) {
		_pEnemyLeader[i]->debugRender();
	}		
	_pMap->debugRender();

	ICasCaded()->debugShadow();

	_pCamera->getViewFrustum().debugRender(Color(255, 0, 0));

	// デバッグタイマー描画
	drawDebugTimer();
}

	//-------------------------------------------------------------
	// ここから描画(2D)
	//-------------------------------------------------------------
	draw2D();

	/*glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);*/
}

//-----------------------------------------------------------------------------
//! タイマーの描画
//-----------------------------------------------------------------------------
void SceneGame::drawDebugTimer()
{

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
		Size<s32> windowSize = GmWindow()->getResolution();
		glOrtho(0, windowSize._w, windowSize._h, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

			// タイマーの描画
			char str[255];

			// 更新時間の描画
			sprintf_s( str, "Loop Time is " );
			loopTimer.DrawTotalTime(Vector3( 600.0f,  50.0f, 0.0f ), str );
			// 更新時間の描画
			sprintf_s( str, "Update Time is " );
			updateTimer.DrawTotalTime(Vector3( 600.0f, 100.0f, 0.0f ), str );
			// 描画時間の描画
			sprintf_s( str, "Render Time is " );
			renderTimer.DrawTotalTime(Vector3( 600.0f, 150.0f, 0.0f ), str );

			sprintf_s( str, "Debug Time is " );
			debugTimer.DrawTotalTime(Vector3( 600.0f, 200.0f, 0.0f ), str );
		
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

}

//-----------------------------------------------------------------------------
//! 描画
//-----------------------------------------------------------------------------
void SceneGame::render()
{	

	//---- 不透明(モデル等)の描画
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	
	//-------------------------------------------------------------
	// ここから描画(3D)
	//-------------------------------------------------------------
#if 1			
	

	
	//---- シャドウ描画
	GmShader()->changeShader(SystemShader::SHADER_SHADOW);

	_isShadow = true;

	s32 splitCount = ICasCaded()->getSplitCount();

	
	// 分割数分描画
	for( s32 i=0; i<splitCount; ++i )
	{
		ICasCaded()->begin(_pLight);
		
		draw3D();
		
	}
	ICasCaded()->end();
	
	//---- スカイボックスの描画
	GmShader()->changeShader(SystemShader::SHADER_DRAWCUBEMAP);
	_pSkyBox->render();


	//---- 通常描画
	GmShader()->changeShader(SystemShader::SHADER_TEXTURE);

	_isShadow = false;
	//---- シェーダプログラムに情報転送
	// ライト設定
	Vector3 sendLight = _pLight->getPosition();
	GLfloat lightPos[] = { sendLight._x, sendLight._y, sendLight._z };
	GmShader()->setVertex3fv("lightPos", lightPos);
	// カメラ座標
	Vector3 cameraPos = _pCamera->getPosition();
	GmShader()->setVertex3fv("eyePos", (GLfloat*)&cameraPos);
	
	draw3D();

	//---- マトリックス転送
	CameraBase* cam   = GmCameraMan()->getCurrentCamera();
	//Matrix matWorld   = Matrix::translate(position);
	Matrix matWorld   = Matrix::IDENTITY;
	Matrix matView	  = cam->getViewMatrix();
	Matrix matProj	  = cam->getProjMatrix();

	Matrix sendMatrix[] =
	{
		matWorld,
		matView,
		matProj,
	};

	GmShader()->setUniformMatrixBuffer(sendMatrix, sizeof(sendMatrix));

	//---- パーティクル描画
	GmShader()->changeShader(SystemShader::SHADER_STANDARD);
	{
		GmRender()->saveBackBuffer("パーティクル描画前");
		IParticleMan()->render();
		GmRender()->saveBackBuffer("パーティクル描画後");
	}
	// 味方のステータスの描画
	{
		
		for( s32 i=0; i<ALLY_COUNT_MAX; i++ )
		{
			_pAlly[i]->StatusRender();
		}
	}
	// 敵のステータスの描画
	{
		
		for( s32 i=0; i<ENEMY_COUNT_MAX; i++ )
		{
			_pEnemy[i]->StatusRender();
		}

		for( s32 i=0; i<ENEMY_SQUAD_MAX; i++ ) {
			_pEnemyLeader[i]->StatusRender();
		}
	}
	GmShader()->beginPrevShader();	// こっちでも描画される(NVIDIAはMatrixを勝手に初期化してくれる？)


	// Xボタン画像表示
	if(_pPlayer->getNearCastle()){
		Vector3 playerPos = _pPlayer->getPosition();
		playerPos._y += 50.0f;
		_pXButtonUI->setPosition(playerPos);
		_pXButtonUI->draw(Color::WHITE);
	}

	GmShader()->beginPrevShader();

	//---- 固定パイプラインでの描画
	// シェーダの終了
	GmShader()->endShader();


#endif 

	_pPlayer->glutRender();

	//---- 攻撃の描画
	for( s32 i=0; i<ALLY_COUNT_MAX; i++ )
	{
		_pAlly[i]->glutRender();
	}

	for( s32 i=0; i<ENEMY_COUNT_MAX; i++ )
	{
		_pEnemy[i]->glutRender();
	}


}

//=============================================================================
//	END OF FILE
//=============================================================================