//-----------------------------------------------------------------------------
//!
//!	@file	Status.cpp
//!	@brief	基本ステータス
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

//! テクスチャ付き2Dゲージ表示
void drawTexture2D(f32 x, f32 y, f32 w, f32 h, f32 gaugeW, f32 gaugeH, Color& gaugeColor, Texture* pTexture)
{
	setBlendMode(BM_BLEND_ALPHA);
	
	glDepthMask(GL_FALSE);	// Z無効
	//---- ゲージの描画
	glBegin(GL_TRIANGLE_STRIP);
		
		glColor4ubv((GLubyte*)&gaugeColor);
		glVertex3f(  x,   y, 0);

		glVertex3f(x+w,   y, 0);

		glVertex3f(  x, y+h, 0);

		glVertex3f(x+w, y+h, 0);
			
	glEnd();

	if( pTexture != nullptr ) {
		//---- ゲージアウトラインの描画
		Texture::set(pTexture, 0);	//---- テクスチャON
		glBegin(GL_TRIANGLE_STRIP);
		
			glColor4ub(255,255,255,255);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(		  x,		y, 0);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(x+gaugeW,		y, 0);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(		  x, y+gaugeH, 0);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(x+gaugeW, y+gaugeH, 0);
			
		glEnd();
		Texture::set(NULL);	//---- テクスチャOFF
	}

	setBlendMode(BM_NONE);
	glDepthMask(GL_TRUE);	// Z有効
}

//-----------------------------------------------------------------------------
//! パラメータ値初期化
//-----------------------------------------------------------------------------
void Status::resetParam()
{
}

//-----------------------------------------------------------------------------
//! CSVファイルの読み込み
//-----------------------------------------------------------------------------
void Status::loadCSV(const char* fileName)
{
	//Param* result = new Param();

	FILE*	fp;
	
	char	str[1024];

	// 文字コードをワイド文字列に変換
    // 【注意】本来はこの箇所は文字列バッファ長の考慮の他に文字列終端コードを処理するよりセキュアな対応が好ましいです。
	wchar_t		DebugStr[ MAX_PATH ];
    size_t      pathLength = 0;

	// ロケール指定
	setlocale(LC_ALL, "japanese");

	fopen_s(&fp, fileName, "rt");
	
	if( fp == NULL )
	{
		sprintf_s(str, sizeof(str), "ステータスファイルの読み込みに失敗しました\n ファイル名 = %s", fileName);

		// charからwchar_tへの変換関数
		mbstowcs_s( &pathLength,            // [out]    変換された文字数
					&DebugStr[0],           // [out]    変換されたワイド文字列を格納するバッファのアドレス(変換先)
					MAX_PATH,               // [in]     出力側のバッファのサイズ(単位:文字数)
					str,		            // [in]     マルチバイト文字列のアドレス(変換元)
					_TRUNCATE );		    // [in]     出力先に格納するワイド文字の最大数
		
		MessageBox(NULL, DebugStr, L"ステータス LOAD", MB_OK);

		return;
	}

#if 0
	char* compareStr[] =
	{
		"HPMAX",
		"HP",
		"MPMAX",
		"MP",
		"ATKDELAY",
		"SKLDELAY",
		"SPEED",
		"ATKPOW",
		"JUMPPOW",
		"GAUGESCALE"
	};
	
	// fgets : 文字列にファイルの一行を格納する
	while( fgets(str, sizeof(str), fp) ) {
		for( s32 i=0; i<Status::STATUS_MAX; i++ ){
			if( strstr( str, compareStr[i] ) )
			{
				f32 temp;
				stringstream ss;
				ss << compareStr[i];
				ss << ",%f";
				sscanf_s(str, ss.c_str(), &temp);
				_param._InfOfParam[i] = temp;
				// 見つかったら次の行へ
				break;
			}
		}

		if( strstr( str, "GAUGESCALE") ) {	// ゲージサイズ
			f32 scaleX, scaleY;
			sscanf_s(str, "GAUGESCALE,%f,%f", &scaleX, &scaleY);
			_param._GaugeBoard.setSize(Vector3(scaleX, scaleY, 0.0f));
		}
	}
#else
	// fgets : 文字列にファイルの一行を格納する
	while( fgets(str, sizeof(str), fp) ) {
		// 文字列と比較してデータを抽出
		if( strstr( str, "HPMAX") )	{				// HPMAX
			f32 HPMAX;
			sscanf_s(str, "HPMAX,%f", &HPMAX);
			_param._HPMAX = HPMAX;
		}else if ( strstr( str, "HP") )	{			// HP
			f32 HP;
			sscanf_s(str, "HP,%f", &HP);
			_param._HP = HP;
		}else if( strstr( str, "MPMAX") ) {			// MPMAX
			f32 MPMAX;
			sscanf_s(str, "MPMAX ,%f", &MPMAX);
			_param._MPMAX = MPMAX;
		}else if ( strstr( str, "MP") )	{			// MP
			f32 MP;
			sscanf_s(str, "MP ,%f", &MP);
			_param._MP = MP;
		}else if( strstr( str, "ATKDELAY") ){		// 攻撃間隔
			f32 ATKDELAY;
			sscanf_s(str, "ATKDELAY ,%f", &ATKDELAY);
			_param._atkDelay = ATKDELAY;
		}else if( strstr( str, "SKLDELAY") ){		// スキル間隔
			f32 SKLDELAY;
			sscanf_s(str, "SKLDELAY ,%f", &SKLDELAY);
			_param._sklDelay = SKLDELAY;
		}else if( strstr( str, "SPEED") ){			// スピード最大値
			f32 SPEED;
			sscanf_s(str, "SPEED, %f", &SPEED);
			_param._speedMAX= SPEED;
		}else if( strstr( str, "ATKPOW") ) {		// 攻撃力
			f32 ATKPOW;
			sscanf_s(str, "ATKPOW, %f", &ATKPOW);
			_param._atkPow= ATKPOW;
		}else if( strstr( str, "JUMPPOW") ) {		// ジャンプ力
			f32 JUMPPOW;
			sscanf_s(str, "JUMPPOW, %f", &JUMPPOW);
			_param._jumpPow= JUMPPOW;
		}else if( strstr( str, "GAUGESCALE") ) {	// ゲージサイズ
			f32 scaleX, scaleY;
			sscanf_s(str, "GAUGESCALE,%f,%f", &scaleX, &scaleY);
			_param._GaugeScaleH = scaleY;
			_param._GaugeScaleW = scaleX;
			_param._GaugeBoard.setSize(Vector3(scaleX, scaleY, 0.0f));
			//result->setGaugeScale(scaleX, scaleY);
		}
	}
#endif 
	// ファイルを閉じる
	fclose(fp);
}
//-----------------------------------------------------------------------------
//! 描画
//-----------------------------------------------------------------------------
void Status::Param::Render(bool draw2D, Color& gaugeColor, Vector3 drawPosition, Texture* pTexture)
{
	

	f32 HP, HPMAX;


	HP	  = _HP;
	HPMAX = _HPMAX;
	// ゲージの座標を取得
	f32	w = (_GaugeScaleW)* (HP / HPMAX);

	// 体力ゲージ描画

	// 最大解像度との比率
	Size<f32> resScale = GmWindow()->getRateResMax();
	Size<s32> size = Size<s32>( _GaugeScaleW * resScale._w, _GaugeScaleH * resScale._h);

	if( draw2D ) {
		drawPosition *= Vector3(resScale._w, resScale._h, 1.0f);
		drawRect(drawPosition._x, drawPosition._y, w * resScale._w, size._h, gaugeColor);
		setBlendMode(BM_BLEND_ALPHA);
		GmRender()->putTexture(pTexture->getTextureHandle(),
							   drawPosition._x, drawPosition._y,
							   size);
		setBlendMode(BM_NONE);
		
		//drawTexture2D(drawPosition._x, drawPosition._y, GaugeSizeW, _GaugeScaleH, _GaugeScaleW, _GaugeScaleH, gaugeColor, pTexture);
	}else{
		_GaugeBoard.setTexture(pTexture);
		_GaugeBoard.setPosition(drawPosition);
		_GaugeBoard.setSize(Vector3(_GaugeScaleW, _GaugeScaleH, 0.0f));
		_GaugeBoard.setScale(Vector3(1.0f, 1.0f, 0.0f));
		_GaugeBoard.draw(gaugeColor);
	}
}



//-----------------------------------------------------------------------------
//! CSVファイルの読み込み
//-----------------------------------------------------------------------------
void StatusHeal::loadCSV(const char* fileName)
{
	FILE*	fp;
	
	char	str[1024];

	// 文字コードをワイド文字列に変換
    // 【注意】本来はこの箇所は文字列バッファ長の考慮の他に文字列終端コードを処理するよりセキュアな対応が好ましいです。
	wchar_t		DebugStr[ MAX_PATH ];
    size_t      pathLength = 0;

	// ロケール指定
	setlocale(LC_ALL, "japanese");

	fopen_s(&fp, fileName, "rt");
	
	if( fp == NULL )
	{
		sprintf_s(str, sizeof(str), "ステータスファイルの読み込みに失敗しました\n ファイル名 = %s", fileName);

		// charからwchar_tへの変換関数
		mbstowcs_s( &pathLength,            // [out]    変換された文字数
					&DebugStr[0],           // [out]    変換されたワイド文字列を格納するバッファのアドレス(変換先)
					MAX_PATH,               // [in]     出力側のバッファのサイズ(単位:文字数)
					str,		            // [in]     マルチバイト文字列のアドレス(変換元)
					_TRUNCATE );		    // [in]     出力先に格納するワイド文字の最大数
		
		MessageBox(NULL, DebugStr, L"ステータス LOAD", MB_OK);

		return;
	}


#if 0 
	char* compareStr[] =
	{
		"HPMAX",
		"HP",
		"MPMAX",
		"MP",
		"ATKDELAY",
		"SKLDELAY",
		"SPEED",
		"ATKPOW",
		"JUMPPOW",
		"HEALPOW",
		"GAUGESCALE"
	};
	
	// fgets : 文字列にファイルの一行を格納する
	while( fgets(str, sizeof(str), fp) ) {
		for( s32 i=0; i<StatusHeal::STATUS_MAX; i++ ){
			if( strstr( str, compareStr[i] ) )
			{
				f32 temp;
				stringstream ss;
				ss << compareStr[i];
				ss << ",%f";
				sscanf_s(str, ss.c_str(), &temp);
				_param._InfOfParam[i] = temp;
				// 見つかったら次の行へ
				break;
			}
		}

		if( strstr( str, "GAUGESCALE") ) {	// ゲージサイズ
			f32 scaleX, scaleY;
			sscanf_s(str, "GAUGESCALE,%f,%f", &scaleX, &scaleY);
			_param._GaugeBoard.setSize(Vector3(scaleX, scaleY, 0.0f));
		}
	}
#else

	// fgets : 文字列にファイルの一行を格納する
	while( fgets(str, sizeof(str), fp) ) {
		// 文字列と比較してデータを抽出
		if( strstr( str, "HPMAX") )	{				// HPMAX
			f32 HPMAX;
			sscanf_s(str, "HPMAX,%f", &HPMAX);
			_paramHeal._HPMAX = HPMAX;
		}else if ( strstr( str, "HP") )	{			// HP
			f32 HP;
			sscanf_s(str, "HP,%f", &HP);
			_paramHeal._HP = HP;
		}else if( strstr( str, "MPMAX") ) {			// MPMAX
			f32 MPMAX;
			sscanf_s(str, "MPMAX ,%f", &MPMAX);
			_paramHeal._MPMAX = MPMAX;
		}else if ( strstr( str, "MP") )	{			// MP
			f32 MP;
			sscanf_s(str, "MP ,%f", &MP);
			_paramHeal._MP = MP;
		}else if( strstr( str, "ATKDELAY") ){		// 攻撃間隔
			f32 ATKDELAY;
			sscanf_s(str, "ATKDELAY ,%f", &ATKDELAY);
			_paramHeal._atkDelay = ATKDELAY;
		}else if( strstr( str, "SKLDELAY") ){		// スキル間隔
			f32 SKLDELAY;
			sscanf_s(str, "SKLDELAY ,%f", &SKLDELAY);
			_paramHeal._sklDelay = SKLDELAY;
		}else if( strstr( str, "SPEED") ){			// スピード最大値
			f32 SPEED;
			sscanf_s(str, "SPEED, %f", &SPEED);
			_paramHeal._speedMAX= SPEED;
		}else if( strstr( str, "ATKPOW") ) {		// 攻撃力
			f32 ATKPOW;
			sscanf_s(str, "ATKPOW, %f", &ATKPOW);
			_paramHeal._atkPow= ATKPOW;
		}else if( strstr( str, "JUMPPOW") ) {		// ジャンプ力
			f32 JUMPPOW;
			sscanf_s(str, "JUMPPOW, %f", &JUMPPOW);
			_paramHeal._jumpPow= JUMPPOW;
		}else if( strstr( str, "HEALPOW" ) ) {
			f32 HEALPOW;
			sscanf_s(str, "HEALPOW, %f", &HEALPOW);
			_paramHeal._healPow = HEALPOW;
		}else if( strstr( str, "GAUGESCALE") ) {	// ゲージサイズ
			f32 scaleX, scaleY;
			sscanf_s(str, "GAUGESCALE,%f,%f", &scaleX, &scaleY);
			_paramHeal._GaugeScaleH = scaleY;
			_paramHeal._GaugeScaleW = scaleX;
			_paramHeal._GaugeBoard.setSize(Vector3(scaleX, scaleY, 0.0f));
			//result->setGaugeScale(scaleX, scaleY);
		}
	}

#endif
	// ファイルを閉じる
	fclose(fp);
}
//=============================================================================
//	END OF FILE
//=============================================================================