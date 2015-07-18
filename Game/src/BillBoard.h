//-----------------------------------------------------------------------------
//!	
//!	@file	BillBoard.h
//!	@brief	ビルボード
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
//!	ビルボード
//-----------------------------------------------------------------------------
class BillBoard
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	BillBoard()
	: _position	( 0.0f, 0.0f, 0.0f )
	, _scale	( 0.0f, 0.0f, 0.0f )
	, _size		( 0.0f, 0.0f, 0.0f )
	, _rotation	( 0.0f )
	, _texture	( nullptr )
	{
		init();
	}
	//! コンストラクタ
	BillBoard(Vector3		position,
			  Vector3		size,
			  Vector3		scale,
			  Radian		rotation,
			  Texture*		texture )
	{
		setParam(position, size, rotation);
		setTexture(texture);
		_scale = scale;
	}

	//! デストラクタ
	virtual ~BillBoard(){}

	//! 初期化
	bool	init();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! ビルボードのために位置計算
	void	calcBillBoardPosition(Vector3* billBoardPos);

	//! 描画
	void	draw(const Color&	color, const s32& aniCount = 0 );

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! 情報設定
	void	setParam(Vector3		position,
					 Vector3		size,
					 Radian			rotation)
	{
		_position = position;
		_size	  = size;
		_rotation = rotation;
	}

	
	//! 座標設定
	void	setPosition(Vector3& position)	{ _position = position; }
	//! サイズ指定
	void	setSize(Vector3&  size)			{ _size = size; }
	//! 描画サイズ指定
	void	setScale(Vector3& scale)		{ _scale = scale; }
	//! 回転角度設定
	void	setRotate(Radian& rotation)		{ _rotation = rotation; }
	//! テクスチャ設定
	void	setTexture(Texture* texture)    { _texture  = texture;  }

	//! 座標取得
	Vector3	getPosition()					{ return _position; }
	//! 回転取得
	Radian	getRotation()					{ return _rotation; }

	//! テクスチャ取得
	Texture* getTexture()					{ return _texture; }
	//@}
protected:
	Vector3		_position;
	Vector3		_scale;
	Vector3		_size;
	Radian		_rotation;
	Texture*	_texture;
	GLuint		_vertex;
};
//=============================================================================
// END OF FILE
//=============================================================================