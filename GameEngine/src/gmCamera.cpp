//-----------------------------------------------------------------------------
//!
//!	@file	Camera.cpp
//!	@brief	�J��������
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
Camera::Camera()
: _myLandScapeGridNum(0)
{
}


//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool Camera::init(Vector3& lookAt, Vector3& _rot)
{
	_isVib = false;

	_lookAt = lookAt;

	// �J�����ւ̕����쐬
	Vector3 dir = Vector3(0.0f, 0.2f, 1.0f);

	// ��]�N�H�[�^�j�I�����쐬
	Quaternion rotQuaternion(_rot);
	dir = rotQuaternion.rotate(dir);
	//���K��
	dir = dir.normalize();
	// �J�����̒����ɕύX
	dir *= _lookAtLength;
	
	// �V�����J�������W�ɐݒ肷��
	_position = _lookAt + dir;
	//moveGoal(_lookAt + dir);
	
	// �����䏉����
	_viewFrustum.setStatus(0.1f, 5000.0f, Degree(85.0f), GmWindow()->getWindowRatio());

	//-------------------------------------------------------------
	// �p���s��𐶐�
	//-------------------------------------------------------------

	_matrix = Matrix::lookAt(_position, _lookAt);

	return true;
}
f32 debugRX = 0.0f;

//-----------------------------------------------------------------------------
//! �X�V
//!	@param	[in]	control		�R���g���[���[
//!	@param	[in]	PlayerPos	�v���C���[���W
//!	@param	[in]	mouseOffset	�}�E�X���W
//-----------------------------------------------------------------------------
void Camera::update(Controller &control,Vector3& PlayerPos, POINT	mouseOffset)
{

	
	static Vector3	prevPosition;

	
	//---- �J���������X�Ƀv���C���[�̌���ǂ�������v���O����
	if( !_isVib )
	{

		// �J�������n�_�̍X�V
		_lookAt = PlayerPos;
	
		_lookAt._y += 10.0f;
		
		// �V�������܂���������W���獡�̃J�������W�ւ̕����x�N�g��
		Vector3 dir;
		dir = _position - _lookAt;

		// ���̒����ɂ���
		dir = dir.normalize();
		dir *= _lookAtLength;
		// �V�����J�������W�ɐݒ肷��
		//_position = _lookAt + dir;

		moveGoal(_lookAt + dir);
		
		//_lookAt = PlayerPos;

		

		//---- �J�����̊p�x���v�Z
		// �J������Z���W�����߂�
		Vector3 CameraAxisZ	 = (_position - _lookAt);
		f32	 x = CameraAxisZ._x;
		f32  y = CameraAxisZ._y;
		f32	 z = CameraAxisZ._z;

		// �J�����̊p�x�����߂�
		_rot._y	= TO_DEGREE( atan2f( -z, -x ) );
		_rot._x = TO_DEGREE( atan2f( sqrtf( x * x + z * z ), y ) );


		f32	rx = 0.0f;
		f32	ry = 0.0f;


		// �E�X�e�B�b�N�����擾
		f32	KnockSize	= 0.0f;
		// �E�X�e�B�b�N�̃x�N�g��
		Vector3 rightStickVector = control.getStickState(false, KnockSize);
		rightStickVector = rightStickVector.normalize();

		//---- �J���������ɂ���ăJ�����̉�]���x��ύX����
		// ��]�X�s�[�h
		f32	rotateSpeed = LinearInterpolation(3.5f, 1.5f, _lookAtLength - 200.0f, 400.0f);
		// �J�����̉�]�p�x�����߂�
		ry += -rightStickVector._x * rotateSpeed * KnockSize;
		rx +=  rightStickVector._z * rotateSpeed * KnockSize;

		// �p�b�h���͂��Ȃ���΃}�E�X�ňړ�
		if(rightStickVector.length() <= 0.01f){
			Mouse* pMouse = GmMouseMan()->getMouse();
			if(pMouse->isRightDown()) {
				ry +=  -mouseOffset.x * 0.3f;
				rx +=  -mouseOffset.y * 0.3f;
			}

		}

		
		// �ύX�O�̊p�x
		Vector3 _prevRot = _rot;
	
		
		// �p�x�ύX����
		// �n�ʂɓ������ĂȂ�������ύX
		if( !_hitGround ) {
			_rot._y += ry * Global::deltaTime;
			_rot._x += rx * Global::deltaTime;
		}
	

		// �͈͊O�w��
		if( _rot._x <=   10.0f )    _rot._x =   10.0f;
		if( _rot._x >=   110.0f )   _rot._x =   110.0f;

		// ��]�p�x�̍���
		Vector3 	changeRot = _rot - _prevRot;
	
		debugRX = _rot._x;

		// ���W�ړ���
		Vector3		offset = Vector3(0.0f, 0.0f, 0.0f);
	
		//---- �J�����̉�]�����v���O����
		{
				//-------------------------------------------------------------
				// ���E��U��(�x�����S�̉�])
				//-------------------------------------------------------------
				Matrix	rotHead = Matrix::translate(-_lookAt) * 
								  Matrix::rotateY  (Degree(changeRot._y)) *
								  Matrix::translate(_lookAt);

				// ��]��K�p(Apply Matrix)
				_position = _position.transform(rotHead);
			

			

				//-------------------------------------------------------------
				// ���E��U��(�x�����S��]�K�p��̉�])
				//-------------------------------------------------------------
				Vector3 axis;		// ��]���S��

				Vector3 lookAt = _lookAt - _position;

				// ���_�����ւ̃x�N�g���Ɛ^��̃x�N�g���Ƃ̊O��
				axis = Vector3::cross(Vector3(0,1,0), lookAt).normalize();

				Matrix	rotPitch = Matrix::translate(-_lookAt) *
								   Matrix::rotateAxis(axis, Degree(changeRot._x)) *
								   Matrix::translate(_lookAt);

				// ��]��K�p(Apply Matrix)
				_position = _position.transform(rotPitch);
		}

		_hitGround = false;

		//---- �}�b�v�Ƃ̓����蔻��
		GmSystemCollision()->checkCollisionModel(_position, _radius, _hitGround);
		
		// �J�������������Ɉړ����Ă��炩�n�ʂɂ������Ă���
		if( rx > 0.0f && _hitGround){
			// �J�����������k�߂�
			if( _lookAtLength > 2.0f ){
				_lookAtLength -= 1.0f * Global::deltaTime;
			}else{
				_lookAtLength  = 2.0f;
			}
		}else if( abs((prevPosition - PlayerPos).length()) > 1.0f){	// �v���C���[���ړ����Ă���
			// 55�x�ȏ�Ȃ�	
			if( _rot._x > 30.0f ){
				// y���W��������
				_position._y += 1.0f * Global::deltaTime;
			}
			// ���W���X�V
			prevPosition = PlayerPos;
			
			// �J�������������΂�
			if( _lookAtLength < _lookAtLengthMax ){
				_lookAtLength += 1.5f * Global::deltaTime;
			}
		}

		// �ő勗�����傫����΃J�����������k�߂�
		if( _lookAtLength > _lookAtLengthMax ) {
			_lookAtLength -= 4.5f * Global::deltaTime;
		}
	
	}
	//-------------------------------------------------------------
	// �p���s��𐶐�
	//-------------------------------------------------------------

	//--- �J�����Ԃ�ǉ�
	Vector3 position	= _position + _cameraOffset;
	Vector3	lookAt		= _lookAt   + _cameraOffset;

	_matrix = Matrix::lookAt(position, lookAt);

	//-------------------------------------------------------------
	// �r���[�s��𐶐�
	//-------------------------------------------------------------
	_viewMatrix = Matrix::view(position, lookAt);

	//-------------------------------------------------------------
	// ���e�s��𐶐�
	//-------------------------------------------------------------
	_projMatrix = Matrix::projection(Degree(85.0f), GmWindow()->getWindowRatio(), 0.1f, 5000.0f);

	//-------------------------------------------------------------
	// ������̍X�V
	//-------------------------------------------------------------
	_viewFrustum.updateFrustumPoints(_position, _matrix);

}




void Camera::DebugRender()
{
	
	//---- �f�o�b�O�\��
	//static GLFont font(L"MS����", 24);

	

	/*wstringstream ss;*/
	stringstream ss;

	// �����̐��x�𗎂Ƃ�
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