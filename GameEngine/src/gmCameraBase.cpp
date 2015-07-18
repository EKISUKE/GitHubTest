//-----------------------------------------------------------------------------
//!
//!	@file	gmCameraBase.cpp
//!	@brief	カメラ制御
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
CameraBase::CameraBase()
: _position			 (0.0f)
, _cameraOffset		 (0.0f)
, _lookAt			 (0,0,-1)
, _worldUp			 (0,1,0)
, _rot				 (0,0,0)
, _radius			 (20.0f)
, _lookAtLength		 (200.0f)
, _lookAtLengthMax	 (200.0f)
{
}

//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool CameraBase::init(Vector3& lookAt, Vector3& _rot)
{
	_isVib = false;
	return true;
}
//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void CameraBase::update(Controller &control,Vector3& PlayerPos, const f32& PlayerRot, POINT	mouseOffset)
{

	
	static Vector3	prevPosition;

	
	//---- カメラが徐々にプレイヤーの後ろを追いかけるプログラム
	{

		// カメラ注始点の更新
		_lookAt = PlayerPos;
	
		// 一旦0にする
		//_position._y = 0.0f;
		//_lookAt._y	 = 0.0f;

	

		// 新しく決まった見る座標から今のカメラ座標への方向ベクトル
		Vector3 dir;
		dir = _position - _lookAt;

		// 一定の長さにする
		dir = dir.normalize();
		dir *= _lookAtLength;
		// 新しくカメラ座標に設定する
		_position = _lookAt + dir;
		

		_lookAt = PlayerPos;
		_lookAt._y += 30.0f;

		//_lookAt._y += 10.0f;

		// Ｙ座標を少しあげる
		//_position._y = 30.0f;

		// Ｙ座標を少しあげる
		//_position._y = _lookAt._y + 20.0f;

		//---- カメラの角度を計算
		// カメラのZ座標を求める
		Vector3 CameraBaseAxisZ	 = (_position - _lookAt);
		f32	 x = CameraBaseAxisZ._x;
		f32  y = CameraBaseAxisZ._y;
		f32	 z = CameraBaseAxisZ._z;

		// カメラの角度を求める
		_rot._y	= TO_DEGREE( atan2f( -z, -x ) );
		_rot._x = TO_DEGREE( atan2f( sqrtf( x * x + z * z ), y ) );
	}

	//---- カメラ回転処理
	{

		f32	rx = 0.0f;
		f32	ry = 0.0f;

		/*if( GetKeyState(VK_RBUTTON) & 0x80 ) {
			ry += (mouseOffset.x * 0.5f);
			rx += (mouseOffset.y * 0.5f);
		}*/

		if( control.GetPadState( control.PAD_RIGHT_U_STICK ) ) { rx -= 3.5f; }
		if( control.GetPadState( control.PAD_RIGHT_D_STICK ) ) { rx += 3.5f; }
		if( control.GetPadState( control.PAD_RIGHT_R_STICK ) ) { ry -= 3.5f; }
		if( control.GetPadState( control.PAD_RIGHT_L_STICK ) ) { ry += 3.5f; }

		// 変更前の角度
		Vector3 _prevRot = _rot;
		
		// 角度変更処理
		// 地面に当たってなかったら変更
		_rot._y += ry * Global::deltaTime;
		_rot._x += rx * Global::deltaTime;


		// 範囲外指定
		if( _rot._x <=   10.0f )    _rot._x =   10.0f;
		if( _rot._x >=   110.0f )   _rot._x =   110.0f;

		// 回転角度の差分
		Vector3 	changeRot = _rot - _prevRot;

		// 座標移動量
		Vector3		offset = Vector3(0.0f, 0.0f, 0.0f);
	
		//---- カメラの回転処理プログラム
		{
				//-------------------------------------------------------------
				// 左右首振り(Ｙ軸中心の回転)
				//-------------------------------------------------------------
				Matrix	rotHead = Matrix::translate(-_lookAt) * 
								  Matrix::rotateY  (Degree(changeRot._y) ) *
								  Matrix::translate(_lookAt);

				// 回転を適用(Apply Matrix)
				//_lookAt = _lookAt.transform(rotHead);
				_position = _position.transform(rotHead);
			

			

				//-------------------------------------------------------------
				// 左右首振り(Ｙ軸中心回転適用後の回転)
				//-------------------------------------------------------------
				Vector3 axis;		// 回転中心軸

				Vector3 lookAt = _lookAt - _position;

				// 視点方向へのベクトルと真上のベクトルとの外積
				axis = Vector3::cross(Vector3(0,1,0), lookAt).normalize();

				/*Matrix	rotPitch = Matrix::translate(-_position) *
								   Matrix::rotateAxis(axis, rx * 3.1415f / 180.0f) *
								   Matrix::translate(_position);*/
				Matrix	rotPitch = Matrix::translate(-_lookAt) *
								   Matrix::rotateAxis(axis, Degree(changeRot._x)) *
								   Matrix::translate(_lookAt);

				// 回転を適用(Apply Matrix)
			//	_lookAt = _lookAt.transform(rotPitch);
				_position = _position.transform(rotPitch);
				/*if( atan2f( _position._x, _position._z ) > 0.0f ) {
					_position = _position.transform(rotHead);
				}*/
		}
	}
	//-------------------------------------------------------------
	// 姿勢行列を生成
	//-------------------------------------------------------------

	//---- Z軸
	Vector3 axisZ = (_position - _lookAt).normalize();

	//---- X軸
	Vector3 axisX = Vector3::cross(_worldUp, axisZ).normalize();

	//---- Y軸
	Vector3 axisY = Vector3::cross(axisZ, axisX).normalize();
	
	//---- 行列へセット		 xyz		w
	_matrix._m[0] = Vector4(axisX	 , 0.0f);
	_matrix._m[1] = Vector4(axisY	 , 0.0f);
	_matrix._m[2] = Vector4(axisZ	 , 0.0f);
	_matrix._m[3] = Vector4(_position, 1.0f);

	//-------------------------------------------------------------
	// ビュー行列を生成
	//-------------------------------------------------------------
	_viewMatrix._m[0]	= Vector4(axisX._x, axisY._x, axisZ._x , 0.0f);
	_viewMatrix._m[1]	= Vector4(axisX._y, axisY._y, axisZ._y , 0.0f);
	_viewMatrix._m[2]	= Vector4(axisX._z, axisY._z, axisZ._z , 0.0f);

	_viewMatrix._m[3]._x = Vector3::dot(_position, axisX) * -1.0f;
	_viewMatrix._m[3]._y = Vector3::dot(_position, axisY) * -1.0f;
	_viewMatrix._m[3]._z = Vector3::dot(_position, axisZ) * -1.0f;
	_viewMatrix._m[3]._w = 1.0f;

}




void CameraBase::DebugRender()
{
	
	
		

}

//-----------------------------------------------------------------------------
//! バイブレーション有効化
//!	@param	[in] vibrationPower バイブレーション量
//-----------------------------------------------------------------------------
void CameraBase::Enablevibration(f32 vibrationPower)
{
	Vector3 cameraOffset;
	_cameraOffset._x = ( rand() % 32768 ) * (1.0f / 32768.0f);
	_cameraOffset._y = ( rand() % 32768 ) * (1.0f / 32768.0f);
	_cameraOffset._z = ( rand() % 32768 ) * (1.0f / 32768.0f);
	
	/*_position += cameraOffset * vibrationPower;
	_lookAt   += cameraOffset * vibrationPower;*/

	_cameraOffset *= vibrationPower;

	_isVib	   = true;
	
}


//-----------------------------------------------------------------------------
//! バイブレーション無効化
//-----------------------------------------------------------------------------
void CameraBase::Disablevibration()
{
	// 振動しなければ処理しない
	if(!_isVib)	return;
	_isVib	      = false;
	_cameraOffset = Vector3(0.0f);
}

//-----------------------------------------------------------------------------
//! ゆっくりと目標座標まで移動する
//! @param	[in]	目標座標
//! @return	true: 移動完了 false: 移動中
//-----------------------------------------------------------------------------
bool CameraBase::moveGoal(Vector3& goalPos)
{
	// 今の座標から目標座標までの距離を測定
	Vector3		dir			= goalPos - _position;
	f32			dirLength	= dir.length();
	static f32	frame		= 0.0f;

	frame += 0.05f;
	
	// 線形補間で移動させる
	Vector3 nextPosition;
	nextPosition._x = LinearInterpolation(_position._x, goalPos._x, frame, 1.0f);
	nextPosition._y = LinearInterpolation(_position._y, goalPos._y, frame, 1.0f);
	nextPosition._z = LinearInterpolation(_position._z, goalPos._z, frame, 1.0f);

	_position = nextPosition;

	Circle	goalCircle(goalPos, 100.0f);
	Circle	myCircle(_position, 10.0f);

	if (goalCircle.isHit(myCircle)){
		frame = 0.0f;
		return true;
	}

	return false;
}

//=============================================================================
//	END OF FILE
//=============================================================================