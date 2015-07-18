//-----------------------------------------------------------------------------
//!
//!	@file	SceneGame.h
//!	@brief	ゲーム中
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------
#define		ENEMY_ATTACKER		2									//!< 敵攻撃タイプの最大数
#define		ENEMY_DEFFENDER		2									//!< 敵防御タイプの最大数
#define		ENEMY_SQUAD_MEM_MAX (ENEMY_ATTACKER + ENEMY_DEFFENDER)	//!< 敵分隊のメンバー最大数
#define		ENEMY_SQUAD_MAX		2									//!< 敵分隊の最大数
#define		ENEMY_COUNT_MAX		(ENEMY_SQUAD_MEM_MAX) * 2			//!< 敵全体の最大数



#define		ALLY_ATTACKER	4
#define		ALLY_DEFFENDER	6
#define		ALLY_HEALER		4
//#define		ALLY_ATTACKER	1
//#define		ALLY_DEFFENDER	1
//#define		ALLY_HEALER		1
#define		ALLY_COUNT_MAX	ALLY_ATTACKER + ALLY_DEFFENDER + ALLY_HEALER
#define		CHARA_SCALE	5.0f			// キャラのスケール値

//-----------------------------------------------------------------------------
//! ゲーム中
//-----------------------------------------------------------------------------
//class SceneGame : public Singleton<SceneGame>
class SceneGame : public SceneBase
{
public:
	GM_TYPE(SceneGame)

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	SceneGame();

	//! デストラクタ
	virtual			~SceneGame();
	

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 初期化
	bool	init();
	//! 更新
	void	update();
	//! 更新
	void	update(s32& state);
	//! 3D描画
	void	draw3D();
	//! 2D描画
	void	draw2D();
	//! フィルターの影響しない描画
	void	drawNoFilter();
	//! タイマーの描画
	void	drawDebugTimer();
	//! 描画
	void	render();
	//! ミッション描画
	void	missionRender();
	//! ゲームクリアオーバー文字描画
	void	gameEndRender();

	//! 復活関数
	void	revival();
	//! 味方復活関数
	void	revivalAlly(Man* pAlly);
	//! 敵復活関数
	void	revivalEnemy(Man* pEnemy);
	
	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! 味方の追加
	bool			addAlly(AllyBase* pAlly);
	//! 敵の追加
	bool			addEnemy(EnemyBase* pEnemy);

	//! マウスの情報取得
	Mouse*			getMouse()	 { return _pMouse; }
	//! カメラの情報取得
	Camera*			getCamera()  { return _pCamera; }
	//! コントローラーの情報取得
	Controller*		getControl() { return _pControl; }

	//@}
private:
	Mouse*			_pMouse;						//!< マウス
	Camera*			_pCamera;						//!< カメラ
	Controller*		_pControl;						//!< コントローラー
	GLFont*			_pFont;							//!< 文字表示用
	Player*			_pPlayer;						//!< プレイヤー
	EnemyBase*		_pEnemy[ENEMY_COUNT_MAX];		//!< 敵

	AllyBase*		_pAlly[ALLY_COUNT_MAX];			//!< 味方
	Map*			_pMap;							//!< マップ

	Squad*			_pPlayerSquad;					//!< プレイヤーの分隊

	EnemyLeader*	_pEnemyLeader[ENEMY_SQUAD_MAX];	//!< 敵のリーダー(テスト)
	Squad*			_pEnemySquad[ENEMY_SQUAD_MAX];	//!< 敵の分隊(テスト)

	bool			_isShadow;						//!< シャドウ描画フラグ
	bool			_gameClear;						//!< ゲームクリアフラグ
	bool			_gameOver;						//!< ゲームオーバーフラグ

	Texture*		_pGameClearWord;				//!< ゲームクリア文字
	Texture*		_pGameOverWord;					//!< ゲームオーバー文字
	Texture*		_pMissionWord[2];				//!< ミッション文字
	Texture*		_pXButtonImage;					//!< Xボタン画像
	BillBoard*		_pXButtonUI;					//!< XボタンUI用


	Light*			_pLight;						//!< ライト
	SkyBox*			_pSkyBox;						//!< スカイボックス

	f32				_waitGoTitle;					//!< ゲーム終了からタイトル移行までの待ち時間

	Performance*	_pUpdatePerformance;			//!< 更新のパフォーマンス
	Performance*	_pRenderPerformance;			//!< 描画のパフォーマンス

	s32				_allyRevivalCountMax;			//!< 味方復活最大数
	s32				_enemyRevivalCountMax;			//!< 敵復活最大数
};


//=============================================================================
//	END OF FILE
//=============================================================================