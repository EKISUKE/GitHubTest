//-----------------------------------------------------------------------------
//!
//!	@file	gmTimer.h
//!	@brief	タイマー関連
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

extern u64		getCounter();
extern u64		getFrequency();


//-----------------------------------------------------------------------------
//!	タイマー基底
//-----------------------------------------------------------------------------
class TimerBase
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	TimerBase(){}

	//! デストラクタ
	virtual ~TimerBase(){}

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! リセット
	virtual void	Reset() = 0;

	//! タイマーの開始
	virtual void StartTimer() = 0;

	//! タイマーの終了
	virtual void EndTimer() = 0;


	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! トータルタイム取得
	virtual s64		getTotalTime() const = 0;

	//@}
};


//-----------------------------------------------------------------------------
//!	CPUデバッグ用タイマー基底
//-----------------------------------------------------------------------------
class DebugCPUTimerBase
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	DebugCPUTimerBase()
	{
		// カウンター周波数を記録
		QueryPerformanceFrequency((LARGE_INTEGER*)&_Frequency);
		// 初期化
		Reset();
	}

	//! デストラクタ
	virtual ~DebugCPUTimerBase(){}

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! リセット
	virtual void	Reset()
	{
		_startTime	= 0;
		_endTime	= 0;
	}

	//! タイマーの開始
	virtual void StartTimer()
	{
		// 開始時間記録
		QueryPerformanceCounter((LARGE_INTEGER*)&_startTime);
	}

	//! タイマーの終了
	virtual void EndTimer()
	{
		// 終了時間記録
		QueryPerformanceCounter((LARGE_INTEGER*)&_endTime);
		// カウンタ周波数に何か記録されてたら
		if( _Frequency != 0 ){
			// 全体時間記録
			_totalTime = (_endTime - _startTime) * 1000 / _Frequency;
		}
		Reset();
	}

	//! 全体時間の描画
	virtual void DrawTotalTime(Vector3 drawPos, char* str)
	{
		stringstream ss;
		ss << str;
		ss << _totalTime;
		render_string(drawPos._x, drawPos._y, ss.str());
	}

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! トータルタイム取得
	virtual s64		getTotalTime() const { return _totalTime; }

	//@}
protected:
	s64		_startTime;		//!< タイマー開始時間
	s64		_endTime;		//!< タイマー終了時間
	s64		_totalTime;		//!< 全体の時間
	s64		_Frequency;		//!< カウンタ周波数
};

//-----------------------------------------------------------------------------
//!	CPUデバッグ用タイマー
//-----------------------------------------------------------------------------
class DebugCPUTimer : public DebugCPUTimerBase
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	DebugCPUTimer(){}
	//! デストラクタ
	virtual ~DebugCPUTimer(){}

	//@}
	//-------------------------------------------------------------
	//! @name 表示
	//-------------------------------------------------------------
	//@{

	void	popTotalTime()
	{
		char str[255];
		sprintf_s(str, "ToatlTime is %i millseconds", _totalTime);
		MessageBoxA(NULL, str, "ToatlTime", MB_OK);
	}

	//@}
};

// プロトタイプ宣言
class Performance;

//-----------------------------------------------------------------------------
//!	パフォーマンスマネージャー
//-----------------------------------------------------------------------------
class SystemPerformance
{
private:
	//-------------------------------------------------------------
	//!	@name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	SystemPerformance()
	: _maxSecond(16)
	{
	}

	//! コピーコンストラクタ
	SystemPerformance(SystemPerformance& src){}

	//! デストラクタ
	virtual ~SystemPerformance();
public:
	//@}
	//-------------------------------------------------------------
	//!	@name 取得
	//-------------------------------------------------------------
	//@{

	//! インスタンス取得
	static SystemPerformance*	getInstance()
	{
		static SystemPerformance	_myInstance;
		return &_myInstance;
	}

	//! 同じパフォーマンスを取得
	Performance*	getSamePerformance(Performance* perform);

	

	//! 最大ミリ秒を返す
	s64		getFrame()
	{
		return _maxSecond;
	}

	
	//@}
	//-------------------------------------------------------------
	//!	@name 登録・解除
	//-------------------------------------------------------------
	//@{

	//! パフォーマンスの追加
	void	addPeform(Performance* perform);

	//! パフォーマンスの削除
	void	deletePeform(Performance* peform);

	//@}
	

	//! 描画
	void	drawPerformanceBar();
	
protected:
	list<Performance*>	_pPerformList;	//!< パフォーマンスリスト
	s64					_maxSecond;		//!< 最大ミリ秒
};

//-----------------------------------------------------------------------------
// アクセスインターフェイス
//-----------------------------------------------------------------------------
__forceinline
SystemPerformance*	GmPerformance()
{
	return SystemPerformance::getInstance();
}


//-----------------------------------------------------------------------------
//! パフォーマンス
//-----------------------------------------------------------------------------
class Performance  : public DebugCPUTimerBase
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	Performance();

	//! デストラクタ
	virtual ~Performance();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! スタート
	void	StartPerformance()
	{
		StartTimer();
	}
	//! 終了
	void	EndPerformance()
	{
		// 終了時間記録
		EndTimer();
	}

	//! リセット
	void	ResetPerformance()
	{
		Reset();
	}

	//! 描画
	void	DrawPerformance(Vector3 drawPos, f32& parentHeight, s64& maxFrame);

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! 同じパフォーマンスを取得
	Performance*	getSamePerformance(Performance* perform);

	//! 高さの取得
	f32		getHeight()	{ return _height; }

	//! 色の設定
	void	setColor(Color& col) { _drawColor = col; }

	//! メインのパフォーマンスに設定
	void	setMainPerformance();

	//@}
	//-------------------------------------------------------------
	//!	@name 登録・解除
	//-------------------------------------------------------------
	//@{

	//! パフォーマンスの追加
	void	addPeform(Performance* perform);

	//! パフォーマンスの削除
	void	deletePeform(Performance* peform);

	//@}

private:
	f32						_height;		//!< 描画の高さ
	Color					_drawColor;		//!< 描画色
	list<Performance*>		_childPeform;	//!< 子のパフォーマンス
};


//=============================================================================
//	END OF FILE
//=============================================================================