//-----------------------------------------------------------------------------
//!
//!	@file	Camera.cpp
//!	@brief	カメラ制御
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
Camera::Camera()
: _myLandScapeGridNum(0)
{
}


//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool Camera::init(Vector3& lookAt, Vector3& _rot)
{
	_isVib = false;

	_lookAt = lookAt;

	// カメラへの方向作成
	Vector3 dir = Vector3(0.0f, 0.2f, 1.0f);

	// 回転クォータニオンを作成
	Quaternion rotQuaternion(_rot);
	dir = rotQuaternion.rotate(dir);
	//正規化
	dir = dir.normalize();
	// カメラの長さに変更
	dir *= _lookAtLength;
	
	// 新しくカメラ座標に設定する
	_position = _lookAt + dir;
	//moveGoal(_lookAt + dir);
	
	// 視錐台初期化
	_viewFrustum.setStatus(0.1f, 5000.0f, Degree(85.0f), GmWindow()->getWindowRatio());

	//-------------------------------------------------------------
	// 姿勢行列を生成
	//-------------------------------------------------------------

	_matrix = Matrix::lookAt(_position, _lookAt);

	return true;
}
f32 debugRX = 0.0f;

//-----------------------------------------------------------------------------
//! 更新
//!	@param	[in]	control		コントローラー
//!	@param	[in]	PlayerPos	プレイヤー座標
//!	@param	[in]	mouseOffset	マウス座標
//-----------------------------------------------------------------------------
void Camera::update(Controller &control,Vector3& PlayerPos, POINT	mouseOffset)
{

	
	static Vector3	prevPosition;

	
	//---- カメラが徐々にプレイヤーの後ろを追いかけるプログラム
	if( !_isVib )
	{

		// カメラ注始点の更新
		_lookAt = PlayerPos;
	
		_lookAt._y += 10.0f;
		
		// 新しく決まった見る座標から今のカメラ座標への方向ベクトル
		Vector3 dir;
		dir = _position - _lookAt;

		// 一定の長さにする
		dir = dir.normalize();
		dir *= _lookAtLength;
		// 新しくカメラ座標に設定する
		//_position = _lookAt + dir;

		moveGoal(_lookAt + dir);
		
		//_lookAt = PlayerPos;

		

		//---- カメラの角度を計算
		// カメラのZ座標を求める
		Vector3 CameraAxisZ	 = (_position - _lookAt);
		f32	 x = CameraAxisZ._x;
		f32  y = CameraAxisZ._y;
		f32	 z = CameraAxisZ._z;

		// カメラの角度を求める
		_rot._y	= TO_DEGREE( atan2f( -z, -x ) );
		_rot._x = TO_DEGREE( atan2f( sqrtf( x * x + z * z ), y ) );


		f32	rx = 0.0f;
		f32	ry = 0.0f;


		// 右スティック情報を取得
		f32	KnockSize	= 0.0f;
		// 右スティックのベクトル
		Vector3 rightStickVector = control.getStickState(false, KnockSize);
		rightStickVector = rightStickVector.normalize();

		//---- カメラ距離によってカメラの回転速度を変更する
		// 回転スピード
		f32	rotateSpeed = LinearInterpolation(3.5f, 1.5f, _lookAtLength - 200.0f, 400.0f);
		// カメラの回転角度を求める
		ry += -rightStickVector._x * rotateSpeed * KnockSize;
		rx +=  rightStickVector._z * rotateSpeed * KnockSize;

		// パッド入力がなければマウスで移動
		if(rightStickVector.length() <= 0.01f){
			Mouse* pMouse = GmMouseMan()->getMouse();
			if(pMouse->isRightDown()) {
				ry +=  -mouseOffset.x * 0.3f;
				rx +=  -mouseOffset.y * 0.3f;
			}

		}

		
		// 変更前の角度
		Vector3 _prevRot = _rot;
	
		
		// 角度変更処理
		// 地面に当たってなかったら変更
		if( !_hitGround ) {
			_rot._y += ry * Global::deltaTime;
			_rot._x += rx * Global::deltaTime;
		}
	

		// 範囲外指定
		if( _rot._x <=   10.0f )    _rot._x =   10.0f;
		if( _rot._x >=   110.0f )   _rot._x =   110.0f;

		// 回転角度の差分
		Vector3 	changeRot = _rot - _prevRot;
	
		debugRX = _rot._x;

		// 座標移動量
		Vector3		offset = Vector3(0.0f, 0.0f, 0.0f);
	
		//---- カメラの回転処理プログラム
		{
				//-------------------------------------------------------------
				// 左右首振り(Ｙ軸中心の回転)
				//-------------------------------------------------------------
				Matrix	rotHead = Matrix::translate(-_lookAt) * 
								  Matrix::rotateY  (Degree(changeRot._y)) *
								  Matrix::translate(_lookAt);

				// 回転を適用(Apply Matrix)
				_position = _position.transform(rotHead);
			

			

				//-------------------------------------------------------------
				// 左右首振り(Ｙ軸中心回転適用後の回転)
				//-------------------------------------------------------------
				Vector3 axis;		// 回転中心軸

				Vector3 lookAt = _lookAt - _position;

				// 視点方向へのベクトルと真上のベクトルとの外積
				axis = Vector3::cross(Vector3(0,1,0), lookAt).normalize();

				Matrix	rotPitch = Matrix::translate(-_lookAt) *
								   Matrix::rotateAxis(axis, Degree(changeRot._x)) *
								   Matrix::translate(_lookAt);

				// 回転を適用(Apply Matrix)
				_position = _position.transform(rotPitch);
		}

		_hitGround = false;

		//---- マップとの当たり判定
		GmSystemCollision()->checkCollisionModel(_position, _radius, _hitGround);
		
		// カメラが下向きに移動してたらかつ地面にあたってたら
		if( rx > 0.0f && _hitGround){
			// カメラ距離を縮める
			if( _lookAtLength > 2.0f ){
				_lookAtLength -= 1.0f * Global::deltaTime;
			}else{
				_lookAtLength  = 2.0f;
			}
		}else if( abs((prevPosition - PlayerPos).length()) > 1.0f){	// プレイヤーが移動してたら
			// 55度以上なら	
			if( _rot._x > 30.0f ){
				// y座標をあげる
				_position._y += 1.0f * Global::deltaTime;
			}
			// 座標を更新
			prevPosition = PlayerPos;
			
			// カメラ距離を延ばす
			if( _lookAtLength < _lookAtLengthMax ){
				_lookAtLength += 1.5f * Global::deltaTime;
			}
		}

		// 最大距離より大きければカメラ距離を縮める
		if( _lookAtLength > _lookAtLengthMax ) {
			_lookAtLength -= 4.5f * Global::deltaTime;
		}
	
	}
	//-------------------------------------------------------------
	// 姿勢行列を生成
	//-------------------------------------------------------------

	//--- カメラぶれ追加
	Vector3 position	= _position + _cameraOffset;
	Vector3	lookAt		= _lookAt   + _cameraOffset;

	_matrix = Matrix::lookAt(position, lookAt);

	//-------------------------------------------------------------
	// ビュー行列を生成
	//-------------------------------------------------------------
	_viewMatrix = Matrix::view(position, lookAt);

	//-------------------------------------------------------------
	// 投影行列を生成
	//-------------------------------------------------------------
	_projMatrix = Matrix::projection(Degree(85.0f), GmWindow()->getWindowRatio(), 0.1f, 5000.0f);

	//-------------------------------------------------------------
	// 視錐台の更新
	//-------------------------------------------------------------
	_viewFrustum.updateFrustumPoints(_position, _matrix);

}




void Camera::DebugRender()
{
	
	//---- デバッグ表示
	//static GLFont font(L"MS明朝", 24);

	

	/*wstringstream ss;*/
	stringstream ss;

	// 小数の精度を落とす
	ss.precision(3);

	ss.str("");
	ss.clear();

	ss << " Rotation of Camera's AxisX : ";
	ss << debugRX;

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	//font.DrawStringW( 24.0, 50.0, 0.0, (wchar_t*)ss.str().c_str());
	render_string(24.0f, 50.0f, ss.str());
		
}

//=============================================================================
//	END OF FILE
//=============================================================================