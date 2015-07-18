//-----------------------------------------------------------------------------
//!
//!	@file	gmCameraBase.cpp
//!	@brief	�J��������
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//! �R���X�g���N�^
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
//! ������
//-----------------------------------------------------------------------------
bool CameraBase::init(Vector3& lookAt, Vector3& _rot)
{
	_isVib = false;
	return true;
}
//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void CameraBase::update(Controller &control,Vector3& PlayerPos, const f32& PlayerRot, POINT	mouseOffset)
{

	
	static Vector3	prevPosition;

	
	//---- �J���������X�Ƀv���C���[�̌���ǂ�������v���O����
	{

		// �J�������n�_�̍X�V
		_lookAt = PlayerPos;
	
		// ��U0�ɂ���
		//_position._y = 0.0f;
		//_lookAt._y	 = 0.0f;

	

		// �V�������܂���������W���獡�̃J�������W�ւ̕����x�N�g��
		Vector3 dir;
		dir = _position - _lookAt;

		// ���̒����ɂ���
		dir = dir.normalize();
		dir *= _lookAtLength;
		// �V�����J�������W�ɐݒ肷��
		_position = _lookAt + dir;
		

		_lookAt = PlayerPos;
		_lookAt._y += 30.0f;

		//_lookAt._y += 10.0f;

		// �x���W������������
		//_position._y = 30.0f;

		// �x���W������������
		//_position._y = _lookAt._y + 20.0f;

		//---- �J�����̊p�x���v�Z
		// �J������Z���W�����߂�
		Vector3 CameraBaseAxisZ	 = (_position - _lookAt);
		f32	 x = CameraBaseAxisZ._x;
		f32  y = CameraBaseAxisZ._y;
		f32	 z = CameraBaseAxisZ._z;

		// �J�����̊p�x�����߂�
		_rot._y	= TO_DEGREE( atan2f( -z, -x ) );
		_rot._x = TO_DEGREE( atan2f( sqrtf( x * x + z * z ), y ) );
	}

	//---- �J������]����
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

		// �ύX�O�̊p�x
		Vector3 _prevRot = _rot;
		
		// �p�x�ύX����
		// �n�ʂɓ������ĂȂ�������ύX
		_rot._y += ry * Global::deltaTime;
		_rot._x += rx * Global::deltaTime;


		// �͈͊O�w��
		if( _rot._x <=   10.0f )    _rot._x =   10.0f;
		if( _rot._x >=   110.0f )   _rot._x =   110.0f;

		// ��]�p�x�̍���
		Vector3 	changeRot = _rot - _prevRot;

		// ���W�ړ���
		Vector3		offset = Vector3(0.0f, 0.0f, 0.0f);
	
		//---- �J�����̉�]�����v���O����
		{
				//-------------------------------------------------------------
				// ���E��U��(�x�����S�̉�])
				//-------------------------------------------------------------
				Matrix	rotHead = Matrix::translate(-_lookAt) * 
								  Matrix::rotateY  (Degree(changeRot._y) ) *
								  Matrix::translate(_lookAt);

				// ��]��K�p(Apply Matrix)
				//_lookAt = _lookAt.transform(rotHead);
				_position = _position.transform(rotHead);
			

			

				//-------------------------------------------------------------
				// ���E��U��(�x�����S��]�K�p��̉�])
				//-------------------------------------------------------------
				Vector3 axis;		// ��]���S��

				Vector3 lookAt = _lookAt - _position;

				// ���_�����ւ̃x�N�g���Ɛ^��̃x�N�g���Ƃ̊O��
				axis = Vector3::cross(Vector3(0,1,0), lookAt).normalize();

				/*Matrix	rotPitch = Matrix::translate(-_position) *
								   Matrix::rotateAxis(axis, rx * 3.1415f / 180.0f) *
								   Matrix::translate(_position);*/
				Matrix	rotPitch = Matrix::translate(-_lookAt) *
								   Matrix::rotateAxis(axis, Degree(changeRot._x)) *
								   Matrix::translate(_lookAt);

				// ��]��K�p(Apply Matrix)
			//	_lookAt = _lookAt.transform(rotPitch);
				_position = _position.transform(rotPitch);
				/*if( atan2f( _position._x, _position._z ) > 0.0f ) {
					_position = _position.transform(rotHead);
				}*/
		}
	}
	//-------------------------------------------------------------
	// �p���s��𐶐�
	//-------------------------------------------------------------

	//---- Z��
	Vector3 axisZ = (_position - _lookAt).normalize();

	//---- X��
	Vector3 axisX = Vector3::cross(_worldUp, axisZ).normalize();

	//---- Y��
	Vector3 axisY = Vector3::cross(axisZ, axisX).normalize();
	
	//---- �s��փZ�b�g		 xyz		w
	_matrix._m[0] = Vector4(axisX	 , 0.0f);
	_matrix._m[1] = Vector4(axisY	 , 0.0f);
	_matrix._m[2] = Vector4(axisZ	 , 0.0f);
	_matrix._m[3] = Vector4(_position, 1.0f);

	//-------------------------------------------------------------
	// �r���[�s��𐶐�
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
//! �o�C�u���[�V�����L����
//!	@param	[in] vibrationPower �o�C�u���[�V������
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
//! �o�C�u���[�V����������
//-----------------------------------------------------------------------------
void CameraBase::Disablevibration()
{
	// �U�����Ȃ���Ώ������Ȃ�
	if(!_isVib)	return;
	_isVib	      = false;
	_cameraOffset = Vector3(0.0f);
}

//-----------------------------------------------------------------------------
//! �������ƖڕW���W�܂ňړ�����
//! @param	[in]	�ڕW���W
//! @return	true: �ړ����� false: �ړ���
//-----------------------------------------------------------------------------
bool CameraBase::moveGoal(Vector3& goalPos)
{
	// ���̍��W����ڕW���W�܂ł̋����𑪒�
	Vector3		dir			= goalPos - _position;
	f32			dirLength	= dir.length();
	static f32	frame		= 0.0f;

	frame += 0.05f;
	
	// ���`��Ԃňړ�������
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