//-----------------------------------------------------------------------------
//!
//!	@file	Status.h
//!	@brief	基本ステータス
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

class IStatus
{
public:
	struct ParamBase {};

	virtual ~IStatus() {}

	//virtual ParamBase*	getParamBase() = 0;

	//virtual void	setHp() = 0;

	virtual void	loadCSV(const char* fileName) = 0;
protected:
};


//-----------------------------------------------------------------------------
//! オブジェクト用ステータス
//-----------------------------------------------------------------------------
class ObjectStatus
{
public:

	//-------------------------------------------------------------
	//!	@name	初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	ObjectStatus()
	{
	}
	//! コンストラクタ
	ObjectStatus(s32 hp, s32 hpMax)
	: _isActive(true)
	{
		setStatus(hp, hpMax);
	}

	//@}
	//-------------------------------------------------------------
	//!	@name タスク関数
	//-------------------------------------------------------------
	//@{

	//! ダメージ関数
	void	damage(u32 damage)
	{
		if( _isActive ){
			_hp -= damage;
		}
		if( _hp <= 0 ){
			_isActive = false;
		}
	}

	//@}
	//-------------------------------------------------------------
	//!	@name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! ステータス設定
	void	setStatus(s32 hp, s32 hpMax)
	{
		_hp    = hp;
		_hpMax = hpMax;
		_isActive = true;
	}

	//@}

	
	s32		_hp;
	s32		_hpMax;
	bool	_isActive;
};

//-----------------------------------------------------------------------------
//! キャラクター用ステータス
//-----------------------------------------------------------------------------
class Status : public IStatus
{
public:

	struct Param : public  ParamBase
	{
	/*	f32&	operator [] (STATUS_TYPE index)
		{
			return (f32)(*this)[index];
		}*/

		//f32			_InfOfParam[STATUS_MAX];	//!< ステータスの情報
		f32			_HP;						//!< ヒットポイント
		f32			_HPMAX;						//!< ヒットポイントの最大値
		f32			_MP;						//!< マナポイント
		f32			_MPMAX;						//!< マナポイントの最大値
		f32			_speedMAX;					//!< 移動速度
		f32			_atkPow;					//!< 攻撃力
		f32			_atkDelay;					//!< 攻撃のディレイ時間
		f32			_sklDelay;					//!< スキルのディレイ時間
		f32			_jumpPow;					//!< ジャンプ力
		f32			_GaugeScaleH;				//!< ゲージサイズH
		f32			_GaugeScaleW;				//!< ゲージサイズW
		BillBoard	_GaugeBoard;				//!< ヒットポイントゲージ描画用
		//! 描画
		void		Render(bool draw2D, Color& gaugeColor, Vector3 drawPosition, Texture* pTexture);
	};

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	Status() {}

	//! デストラクタ
	virtual ~Status() {}

	//! パラメータ値初期化
	virtual void	resetParam();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 描画
	virtual void Render(bool draw2D, Color& gaugeColor, Vector3& drawPosition = Vector3(0,0,0), Texture* pTexture = nullptr)
	{
		_param.Render(draw2D, gaugeColor, drawPosition, pTexture);
	}


	//! CSVファイルの読み込み
	virtual void	loadCSV(const char* fileName);


	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//!	ステータスを設定
	void				setParam(const Param& param)
	{
		_param = param;
	}

	//! ステータス設定(ステータスタイプで設定)
	virtual void		setParam(StatusAssets::STATUS_TYPE type)
	{
		_param = *IStatusAssets()->getStatus(type)->getParam();
	}

	//! パラメータ取得
	virtual Param*		getParam()
	{
		return &_param;
	}

	//@}
protected:
	Param	_param;
};

//Status::Param* p = (Status::Param*)gpStatus->getParam();
//f32 hp = p._HP;

//-----------------------------------------------------------------------------
//! キャラクター用ステータス(ヒーラー用)
//-----------------------------------------------------------------------------
class StatusHeal : public Status
{
public:
	
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{


	struct ParamHeal : public Param
	{
		f32 _healPow;
		//f32			_InfOfParam[STATUS_MAX];	//!< ステータスの情報
	};

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 描画
	virtual void Render(bool draw2D, Color& gaugeColor, Vector3& drawPosition = Vector3(0,0,0), Texture* pTexture = nullptr)
	{
		_paramHeal.Render(draw2D, gaugeColor, drawPosition, pTexture);
	}

	//! CSVファイルの読み込み
	virtual void loadCSV(const char* fileName);

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! ステータス設定(ステータスタイプで設定)
	virtual void		setParam(StatusAssets::STATUS_TYPE type)
	{
		_paramHeal = *(ParamHeal*)IStatusAssets()->getStatus(type)->getParam();
	}

	virtual Param*		getParam()
	{
		return &_paramHeal;
	}

	//@}
protected:
	ParamHeal	_paramHeal;
};


//=============================================================================
//	END OF FILE
//=============================================================================