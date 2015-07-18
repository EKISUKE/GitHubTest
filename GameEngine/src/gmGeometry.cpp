//-----------------------------------------------------------------------------
//!
//!	@file	gmGeometry.cpp
//!	@brief	�`��
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

namespace collision {

	//----------------------------------------------------------------------------
	//! ����
	//! ���� vs �_
	//! @param	[in]	plane		����
	//! @param	[in]	position	�_�̈ʒu
	//!	@return	�ŒZ����(�\���͐��̒l, �����͕��̒l)
	//----------------------------------------------------------------------------
	f32	getDistance(const Plane& plane, const Vector3& position)
	{
		// �����́u Ax + By + Cz + D �v�Ōv�Z
		return plane.getDistance(position);
	}

	//----------------------------------------------------------------------------
	//! @param [in]		triangle		�O�p�`
	//! @param [in]		line			����
	//! @param [in]		pT				�Փ˂����_�ƌ������钼���̃p�����[�^�[t���󂯎��ꏊ
	//! @param [in]		pHitPosition	�Փ˓_
	//! @param [in]		pHitNormal		�Փ˓_�̖@��
	//----------------------------------------------------------------------------
	void		checkHitPos(const Triangle& triangle,
							const Line&	    line,
							Vector3*		pHitPosition,
							f32*			pT,
							Vector3*		pHitNormal	)
	{
		//------------------------------------------------------------
		// ���ʂƂ̏Փ˔���ƌ����ʒu���v�Z
		//------------------------------------------------------------
		f32		t;
		bool	hit = collision::isHit( ( const Plane)triangle, line, &t);

		if( !hit ) return;

		//---- �p�����[�^�[t������W���v�Z
		Vector3 hitPosition = line.getPosition(t);

		//------------------------------------------------------------
		// �O�p�`�̓_�Ɋ܂܂�ĂȂ���Έ�ԋ߂��_��Ԃ�
		//------------------------------------------------------------
		hitPosition = triangle.getHitPos( hitPosition );
		
		//------------------------------------------------------------
		// ���ʂ̕ۑ��o��
		//------------------------------------------------------------
		if( pT			 )  *pT			  = t;
		if( pHitPosition )  *pHitPosition = hitPosition;
		if( pHitNormal	 )  *pHitNormal	  = triangle.getNormal();
	}

	//----------------------------------------------------------------------------
	//! �Փˌv�Z
	//! �O�p�` vs ����
	//! @param [in]		triangle		�O�p�`
	//! @param [in]		line			����
	//! @param [in]		pT				�Փ˂����_�ƌ������钼���̃p�����[�^�[t���󂯎��ꏊ
	//! @param [in]		pHitPosition	�Փ˓_
	//! @param [in]		pHitNormal		�Փ˓_�̖@��
	//----------------------------------------------------------------------------
	bool		isHit( const Triangle& triangle,
					   const Line&	   line,
					   Vector3*		   pHitPosition,
					   f32*			   pT,
					   Vector3*		   pHitNormal	)
	{
		//------------------------------------------------------------
		// ���ʂƂ̏Փ˔���ƌ����ʒu���v�Z
		//------------------------------------------------------------
		f32		t;
		bool	hit = collision::isHit( ( const Plane)triangle, line, &t);

		if( !hit ) return false;

		//---- �p�����[�^�[t������W���v�Z
		Vector3 hitPosition = line.getPosition(t);

		//------------------------------------------------------------
		// �O�p�`�̒��ɏՓ˓_���܂܂�邩�ǂ������`�F�b�N
		//------------------------------------------------------------
		hit = triangle.isHit( hitPosition );

		if( !hit ) {
			return false;
		}

		//------------------------------------------------------------
		// ���ʂ̕ۑ��o��
		//------------------------------------------------------------
		if( pT			 )  *pT			  = t;
		if( pHitPosition )  *pHitPosition = hitPosition;
		if( pHitNormal	 )  *pHitNormal	  = triangle.getNormal();

		return true;
	}

	//----------------------------------------------------------------------------
	//! �Փˌv�Z
	//! ���� vs ����
	//! @param [in]		plane			����
	//! @param [in]		line			����
	//! @param [in]		pT				�Փ˂����_�ƌ������钼���̃p�����[�^�[t���󂯎��ꏊ
	//----------------------------------------------------------------------------
	bool		isHit( const Plane& plane, const Line& line, f32* pT)
	{

		Vector3	S = line._position[0];	//!< �n�_
		Vector3	E = line._position[1];	//!< �I�_

		//-------------------------------------------------------------
		// ���ʂ̕��������璼���̎n�_S�ƏI�_E�̍��� (HS, HE)���v�Z
		//-------------------------------------------------------------
		//
		// - - -��S
		// ��     �_
		//          �_
		// HS         �_
		// ��           �_
		// ---------------��----------
		// ��               �_ 
		// ��HE               �_
		// - - - - - - - - - - -��E
		f32	HS = collision::getDistance(plane, S);		//!< �n�_���畽�ʂ܂ł̋���
		f32	HE = collision::getDistance(plane, E);		//!< �I�_���畽�ʂ܂ł̋���

		// �n�_�I�_�������Ȃ�I��
		if( HS == HE ) return false;

		//---- HS,HE 2�̔䗦����p�����[�^�[t���v�Z
		// - - -��S
		// �� �@ |�_
		//       |  �_
		// HS    |    �_
		// ��    |      �_
		// ------+--------��-------------------
		// ��    |          �_ 
		// ��HE  |            �_
		// - - - + - - - - - - -��E
		// �p�����[�^t�̌v�Z (0��t��1)
		// t = 
		// ���`��Ԃ̂���HS�̊��������߂�
		f32	t = HS / (HS - HE);

		//---- ����Ԃ�
		if( pT ) *pT = t;

		return true;
	}

}	// namespace collision

//=============================================================================
// �g���C�A���O��(�O�p�`)�̎���
//=============================================================================
//-----------------------------------------------------------------------------
//! �f�o�b�O�p�`��
//-----------------------------------------------------------------------------
void Triangle::debugRender(Color& col)
{
	//----
	// �|���S���I�t�Z�b�g�@�\�ɂ���ă|���S�����������ւ��炷
	// (���C���Ƃ�Z�t�@�C�g�h�~)
	glPolygonOffset(-5.0f, 1.0f);
	glEnable(GL_POLYGON_OFFSET_FILL);

	// �J���[
	glColor4ubv((GLubyte*)&col);

	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, (GLfloat*)&col);

	glBegin(GL_TRIANGLES);
		glVertex3fv((GLfloat*)&_position[0]);
		glVertex3fv((GLfloat*)&_position[1]);
		glVertex3fv((GLfloat*)&_position[2]);
	glEnd();


	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	Vector3 center = getCenter();
	glBegin(GL_LINES);
		glVertex3fv((GLfloat*)&center);
		glVertex3fv((GLfloat*)&(center + _normal * 10) );
	glEnd();

	glDisable(GL_POLYGON_OFFSET_FILL);
}

//=============================================================================
// ���C��(����)�̎���
//=============================================================================
//-----------------------------------------------------------------------------
//! �f�o�b�O�p�`��
//-----------------------------------------------------------------------------
void Line::DebugRender()
{
	glPushMatrix();
		glBegin(GL_LINES);
		glVertex3fv((GLfloat*)&_position[0]);
		glVertex3fv((GLfloat*)&_position[1]);
		glEnd();
	glPopMatrix();
}



//=============================================================================
// ���̎���
//=============================================================================

//-----------------------------------------------------------------------------
//! �Փˌv�Z
//! �~���m
//! @param	[in]	sphere �����蔻��̂�������
//-----------------------------------------------------------------------------
bool Sphere::isHit(Sphere& sphere)
{
	return GmSystemCollision()->HitSphere(_position, sphere.getPosition(), _radius, sphere.getRadius());
}

//-----------------------------------------------------------------------------
//! �f�o�b�O�\��
//-----------------------------------------------------------------------------
void Sphere::DebugRender()
{
	glPushMatrix();
		glTranslatef( _position._x, _position._y, _position._z );
		setBlendMode(BM_ADD_ALPHA);		//================================ �y�����������J�n�z
		glutSolidSphere(_radius,16,16);
		setBlendMode(BM_NONE);			//================================ �y�����������I���z
	glPopMatrix();
}

//=============================================================================
// �~�̎���
//=============================================================================
//-----------------------------------------------------------------------------
//! �Փˌv�Z
//! �~���m
//! @param	[in]	circle �����蔻��̂������~
//-----------------------------------------------------------------------------
bool Circle::isHit(Circle& circle)
{
	return GmSystemCollision()->HitCircle(_position, circle.getPosition(), _radius, circle.getRadius());
}
	
//-----------------------------------------------------------------------------
//! �f�o�b�O�\��
//-----------------------------------------------------------------------------
void Circle::DebugRender()
{
	Degree rot1, rot2;

	f32 x,y;
	x = _position._x;
	y = _position._z;

	for( rot1._degree = 0.0f; rot1._degree <= 360.0f; rot1._degree += 1.0f )
	{
		rot2._degree = rot1._degree + 10.0f;
		Radian rot1Rad(rot1);
		Radian rot2Rad(rot2);

		f32 x1 = _radius * cosf(rot1Rad);
		f32 y1 = _radius * sinf(rot1Rad);
		f32 x2 = _radius * cosf(rot2Rad);
		f32 y2 = _radius * sinf(rot2Rad);

		glBegin(GL_TRIANGLES);
			glVertex2f(x,y);
			glVertex2f(x1+x, y1+y);
			glVertex2f(x2+x, y2+y);
		glEnd();

	}
}

//=============================================================================
// ������̎���
//=============================================================================



//-----------------------------------------------------------------------------
//! �������8�_�̍X�V
//! @param	[in]	center		���S���W
//! @param	[in]	matrix		���ƂȂ�p���s��
//-----------------------------------------------------------------------------
void Frustum::updateFrustumPoints(Vector3& center, Matrix& matrix)
{

	Vector3 up		=  Vector3(matrix._m[1]);		// ���ƂȂ�r���[�̏�
	Vector3 right	=  Vector3(matrix._m[0]);		// ���ƂȂ�r���[�̉E
	Vector3 viewDir = -Vector3(matrix._m[2]);		// ���ƂȂ�r���[�̑O(��������:�E����W�Ȃ̂Ń}�C�i�X)
	
	// ���N���b�v��
	Vector3 fc = center + viewDir * _fard;
	// �߃N���b�v��
	Vector3 nc = center + viewDir * _neard;
	
	// �߁A���N���b�v�ʂ̕������v�Z
	f32 near_height = tan(_fov/2.0f) * _neard;
	f32 near_width  = near_height	 * _ratio;
	f32 far_height  = tan(_fov/2.0f) * _fard;
	f32 far_width   = far_height	 * _ratio;

	// ���W�̍X�V
	// p[1]/[5]+-------------+p[2]/[6]
	//		   |			 |
	//		   |	nc/fc	 |
	//		   |			 |
	//		   |			 |
	// p[0]/[4]+-------------+p[3]/[7]

	// �߃N���b�v
	_point[0] = nc - up*near_height - right*near_width;
	_point[1] = nc + up*near_height - right*near_width;
	_point[2] = nc + up*near_height + right*near_width;
	_point[3] = nc - up*near_height + right*near_width;

	// ���N���b�v
	_point[4] = fc - up*far_height - right*far_width;
	_point[5] = fc + up*far_height - right*far_width;
	_point[6] = fc + up*far_height + right*far_width;
	_point[7] = fc - up*far_height + right*far_width;

	_position = center;
}

//-----------------------------------------------------------------------------
//! �������6�ʂ̍X�V
//-----------------------------------------------------------------------------
void Frustum::updateFrustumPlane()
{
	// 6�ʂ̍X�V
	// �ߖ�
	_plane[0] = Plane(_point[0], _point[1], _point[2]);
	// ����
	_plane[1] = Plane(_point[6], _point[5], _point[4]);
	// ���
	_plane[2] = Plane(_position, _point[5], _point[6]); 
	// ����
	_plane[3] = Plane(_position, _point[7], _point[4]);
	// �E��
	_plane[4] = Plane(_position, _point[6], _point[7]);
	// ����
	_plane[5] = Plane(_position, _point[4], _point[5]);
}

//-----------------------------------------------------------------------------
//! �f�o�b�O�`��
//-----------------------------------------------------------------------------
void Frustum::debugRender(Color& col) const
{
	for( s32 i=0; i<8-1; ++i ) { 
		drawLineF(_point[i], _point[i+1], col);
	}
}

//-----------------------------------------------------------------------------
//! �Փˌv�Z
//! ������ vs ��
//! @param	[in]	sphere �����蔻��̂�������
//-----------------------------------------------------------------------------
bool Frustum::isHit(Sphere& sphere)
{
	// ������̖ʂ̍X�V
	updateFrustumPlane();
	// ���̍��W
	Vector3 spherePos = sphere.getPosition();
	// ���̔��a
	f32		sphereRad = sphere.getRadius();

	// �ʂƂ̋��������ׂĂ̖ʂ̓����ɂ���΂������Ă���
	for (s32 i = 0; i < 6; ++i){
		f32 distance = _plane[i].getDistance(spherePos);

		// ���ʂ̏ꍇ�����̓}�C�i�X�Ȃ̂Ŕ��a�𑫂��Ă�
		// ���̏ꍇ������̊O���Ŋ����a�ȏ㗣��Ă���
		if (distance + sphereRad < 0.0f){
			return false;
		}
	}

	return true;
}




//=============================================================================
//	END OF FILE
//=============================================================================