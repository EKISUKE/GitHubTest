//-----------------------------------------------------------------------------
//!
//!	@file	Timer.cpp
//!	@brief	タイマー関連
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


u64	getCounter()
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);

	return counter.QuadPart;
}

u64 getFrequency()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);

	return freq.QuadPart;
}


//=============================================================================
//	パフォーマンスマネージャー実装
//=============================================================================

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
SystemPerformance::~SystemPerformance()
{
	// リスト削除
	for( list<Performance*>::iterator itr = _pPerformList.begin(); itr != _pPerformList.end(); )
	{
		Performance* performance = *itr;
		SAFE_DELETE(performance);
		itr = _pPerformList.erase(itr);
	}
}

//-----------------------------------------------------------------------------
//! 同じパフォーマンスを取得
//-----------------------------------------------------------------------------
Performance* SystemPerformance::getSamePerformance(Performance* perform)
{
	for( list<Performance*>::iterator itr = _pPerformList.begin(); itr != _pPerformList.end(); itr++ )
	{
		Performance*	tmpPef = *itr;
		// 見つかったら
		if( tmpPef == perform )
		{
			return *itr;
		}
	}
	// 見つからなかった
	return NULL;
}


//-----------------------------------------------------------------------------	
//! パフォーマンスの追加
//-----------------------------------------------------------------------------
void SystemPerformance::addPeform(Performance* perform)
{
	if( !getSamePerformance(perform) )
	{
		// 登録
		_pPerformList.push_back(perform);
	}
}

//-----------------------------------------------------------------------------
//! パフォーマンスの削除
//-----------------------------------------------------------------------------
void SystemPerformance::deletePeform(Performance* perform)
{
	// 同じものを削除する
	_pPerformList.remove(perform);
}

//-----------------------------------------------------------------------------
//! 描画
//-----------------------------------------------------------------------------
void SystemPerformance::drawPerformanceBar()
{

	glDepthMask(GL_FALSE);
	setBlendMode(BM_BLEND_ALPHA);

	// 描画座標
	Vector3 drawPos = Vector3(1860.0f, 0.0f, 0.0f);

	// フレームの数
	static u32 frameCount = 3;
	// 線形補完する
	static f32 height = 1000.0f;
	// １フレーム分の高さ
	static f32 oneFrameHeight = height / (f32)frameCount;
	

	// 枠を描画
	glColor4f(0.0f,0.0f,0.0f, 0.8f);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(drawPos._x	     , drawPos._y	 	  , 0.0f);
		glVertex3f(drawPos._x + 40.0f, drawPos._y		  , 0.0f);
		glVertex3f(drawPos._x		 , drawPos._y + height, 0.0f);
		glVertex3f(drawPos._x + 40.0f, drawPos._y + height, 0.0f);
	glEnd();

	// 枠の分だけずらす
	drawPos._x += 5.0f;

	

	// 描画
	for( list<Performance*>::iterator itr = _pPerformList.begin(); itr != _pPerformList.end(); itr++ )
	{
		Performance*	drawPef = *itr;
		// 描画
		drawPef->DrawPerformance(drawPos, oneFrameHeight, _maxSecond);
		// 次の描画位置を求める
		drawPos._y += drawPef->getHeight();
	}

	//---- フレームの分割線の描画
	drawPos._x -= 5.0f;
	glBegin(GL_LINES);
	glColor4f(0.0f, 1.0f, 0.5f, 1.0f);
	for( u32 i=0; i<frameCount; ++i ){
		glVertex3f(drawPos._x		 , i * oneFrameHeight, 0.0f);
		glVertex3f(drawPos._x + 40.0f, i * oneFrameHeight, 0.0f);
	}
	glEnd();

	// 白で設定しなおす
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	setBlendMode(BM_NONE);
	glDepthMask(GL_TRUE);
}


//=============================================================================
//	パフォーマンス実装
//=============================================================================

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
Performance::Performance()
: _height	(0.0f)
{
	
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
Performance::~Performance() 
{
	// リスト削除(今ここで削除するとエラーを起こす)
	//for( list<Performance*>::iterator itr = _childPeform.begin(); itr != _childPeform.end(); )
	//{
	//	Performance* performance = *itr;
	//	SAFE_DELETE(performance);
	//	itr = _childPeform.erase(itr);
	//}
}

//-----------------------------------------------------------------------------
//! 同じパフォーマンスを取得
//-----------------------------------------------------------------------------
Performance* Performance::getSamePerformance(Performance* perform)
{
	for( list<Performance*>::iterator itr = _childPeform.begin(); itr != _childPeform.end(); itr++ )
	{
		Performance*	tmpPef = *itr;
		// 見つかったら
		if( tmpPef == perform )
		{
			return *itr;
		}
	}
	// 見つからなかった
	return NULL;
}

//-----------------------------------------------------------------------------
//! メインのパフォーマンスに設定
//-----------------------------------------------------------------------------
void Performance::setMainPerformance()
{
	// 登録
	GmPerformance()->addPeform(this);
}

//-----------------------------------------------------------------------------
//! 描画
//-----------------------------------------------------------------------------
void Performance::DrawPerformance(Vector3 drawPos, f32& parentHeight, s64& maxFrame)
{
	// 線形補完する
	_height = LinearInterpolation(0.0f, parentHeight, (f32)_totalTime, (f32)maxFrame);
	// 自分のバーの描画	
	glBegin(GL_TRIANGLE_STRIP);
		glColor4ubv((GLubyte*)&_drawColor);
		glVertex3f(drawPos._x	     , drawPos._y	 	   , 0.0f);
		glVertex3f(drawPos._x + 30.0f, drawPos._y		   , 0.0f);
		glVertex3f(drawPos._x		 , drawPos._y + _height, 0.0f);
		glVertex3f(drawPos._x + 30.0f, drawPos._y + _height, 0.0f);
	glEnd();

	// 子のパフォーマンスの描画
	for( list<Performance*>::iterator itr = _childPeform.begin(); itr != _childPeform.end(); itr++ )
	{
		Performance*	drawPef = *itr;
		// 描画
		drawPef->DrawPerformance(drawPos, _height, _totalTime);
		// 次の描画位置を求める
		drawPos._y += drawPef->getHeight();
	}

}


//-----------------------------------------------------------------------------	
//! パフォーマンスの追加
//-----------------------------------------------------------------------------
void Performance::addPeform(Performance* perform)
{
	if( !getSamePerformance(perform) )
	{
		// 登録
		_childPeform.push_back(perform);
	}
}

//-----------------------------------------------------------------------------
//! パフォーマンスの削除
//-----------------------------------------------------------------------------
void Performance::deletePeform(Performance* perform)
{
	// 同じものを削除する
	_childPeform.remove(perform);
}



//=============================================================================
//	END OF FILE
//=============================================================================