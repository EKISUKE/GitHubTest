//-----------------------------------------------------------------------------
//!
//!	@file	gmGeometry.h
//!	@brief	�`��
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
//!	����	Plane / Surface
//-----------------------------------------------------------------------------
struct Plane
{
public:

	//-------------------------------------------------------------
	//!	@name ������
	//-------------------------------------------------------------
	//@{

	//!	�f�t�H���g�R���X�g���N�^
	Plane() {}

	//! �R���X�g���N�^(3�_�w��)
	//!	@param [in]		p1	���ʏ�̓_1
	//!	@param [in]		p2	���ʏ�̓_2
	//!	@param [in]		p3	���ʏ�̓_3
	Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3)
	{
		//        �^�_(p1)
		//      �^b   �_
		//(p2)�^        �_(p3)
		//  +--------------+
		//         a

		// �@��_normal(A,B,C)�����߂�
		Vector3	a = p3 - p2;
		Vector3	b = p1 - p2;
		// �O�ς��g���ĕ��ʂɐ����Ȑ�(�@��)�����߂�
		_normal = Vector3::cross(a, b).normalize();

		// ���݈ʒu���猴�_�܂ł̋���D�����߂�
		// ���ʂ̕�����
		// �@���ƕ��ʏ�̓_�̓���+������0
		// Ax + By + Cz + D = 0
		//				  D = -(Ax + By + Cz)
		_distance = - Vector3::dot(_normal, p1);

	}

	//!	�R���X�g���N�^(���ʃp�����[�^�w��)
	//! @param	[in]	normal		�ʂ̖@��
	//!	@param	[in]	distance	���_�܂ł̋���
	Plane(const Vector3& normal, f32 distance)
	: _normal	(normal)
	, _distance	(distance)
	{
	}

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//!	�@���̎擾
	Vector3		getNormal() const { return _normal; }

	//! �����Ȑ��ƌ�������ꏊ��Y���W���擾
	f32			getHeight(f32 x, f32 z)
	{
		// Ax + By + Cz + D = 0
		//		By			= -(Ax + Cz + D)
		//		y			= -(Ax + Cz + D)/B

		f32	A = _normal._x;
		f32	B = _normal._y;
		f32	C = _normal._z;
		f32	D = _distance;

		return -(A*x + C*z + D) / B;
	}

	//! �ڐG�_���擾
	Vector3			getHitPos(Vector3 pos)
	{

		Vector3 result;

		// Ax + By + Cz + D = 0
		//		By			= -(Ax + Cz + D)
		//		y			= -(Ax + Cz + D)/B
		f32 x = pos._x;
		f32 y = pos._y;
		f32 z = pos._z;
		f32	A = _normal._x;
		f32	B = _normal._y;
		f32	C = _normal._z;
		f32	D = _distance;

		result._x = -(B*y + C*z + D) / A;
		result._y = -(A*x + C*z + D) / B;
		result._z = -(A*x + B*y + D) / C;

		return result;
	}

	//----------------------------------------------------------------------------
	//! ����
	//! ���� vs �_
	//! @param	[in]	position	�_�̈ʒu
	//!	@return	�ŒZ����(�\���͐��̒l, �����͕��̒l)
	//----------------------------------------------------------------------------
	f32	getDistance(const Vector3& position) const
	{
		// �����́u Ax + By + Cz + D �v�Ōv�Z
		return Vector3::dot(_normal, position) + _distance;
	}
	
	//@}
public:
	// �y���ʂ̕����� equation�zAx + By + Cz + D = 0
	Vector3		_normal;		// �ʂ̖@��		  (ABC)
	f32			_distance;		// ���_�܂ł̋��� (D)
	//bool		_hit;			// �������Ă邩�ǂ���
};


//-----------------------------------------------------------------------------
//!	�O�p�`	Triangle
//!	@note	�O�p�`�͖������ʂ̎O�����𐧌���������
//-----------------------------------------------------------------------------
struct	Triangle : public Plane
{
public:

	//-------------------------------------------------------------
	//!	@name ������
	//-------------------------------------------------------------
	//@{

	//!	�f�t�H���g�R���X�g���N�^
	Triangle() {}

	//! �R���X�g���N�^(3�_�w��)
	//!	@param [in]		p1	���ʏ�̓_1
	//!	@param [in]		p2	���ʏ�̓_2
	//!	@param [in]		p3	���ʏ�̓_3
	Triangle(const Vector3& p1, const Vector3& p2, const Vector3& p3)
	: Plane(p1, p2, p3)		//	�e�N���X
	{
		_position[0] = p1;
		_position[1] = p2;
		_position[2] = p3;
	}

	//@}
	//-------------------------------------------------------------
	//!	@name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �f�o�b�O�`��
	void			debugRender(Color& col);

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//!	�w�肵�����W�̓_���O�p�`�Ɋ܂܂�邩�ǂ������e�X�g
	bool		isHit(const Vector3& p) const
	{
		// �_A,B,C���炻�ꂼ��_P�Ɍ������x�N�g�����v�Z
		//          A�^|�_				
		//         �^  |  �_				A��P
		//       �^    |    �_				B��P
		//     �^   _ -��-__  �_			C��P
		// B �^__---   P    ---_�_ C
		// +-----------------------+
		//
		static const s32		nextIndex[3] = {1, 2, 0};
		for( s32 i=0; i<3; i++ ) {
			Vector3 dirEdge = _position[ nextIndex[i] ] - _position[i]; // �O�������𔽎��v���
			Vector3	dirToP	= p - _position[i];							// P�Ɍ������x�N�g��


			Vector3 n = Vector3::cross(dirEdge, dirToP);				// ��]������\���@��

			//---- �@���̗��\���� (�ʂ̖@���Ɠ��������������Ă��邩�ǂ����𔻒�)
			if( Vector3::dot(n, getNormal()) < 0.0f ) return false;
		}
		return true;
	}

	//! �w�肵�����W�����ԋ߂��O�p�`�̓_��Ԃ�
	Vector3		getHitPos(const Vector3& p) const
	{
		Vector3 result = p;
		// �_A,B,C���炻�ꂼ��_P�Ɍ������x�N�g�����v�Z
		//          A�^|�_				
		//         �^  |  �_				A��P
		//       �^    |    �_				B��P
		//     �^   _ -��-__  �_			C��P
		// B �^__---   P    ---_�_ C
		// +-----------------------+
		//
		static const s32		nextIndex[3] = {1, 2, 0};
		for( s32 i=0; i<3; i++ ) {
			Vector3 dirEdge = _position[ nextIndex[i] ] - _position[i]; // �O�������𔽎��v���
			Vector3	dirToP	= p - _position[i];							// P�Ɍ������x�N�g��


			Vector3 n = Vector3::cross(dirEdge, dirToP);				// ��]������\���@��

			//---- �@���̗��\���� (�ʂ̖@���Ɠ��������������Ă��邩�ǂ����𔻒�)
			if( Vector3::dot(n, getNormal()) < 0.0f ){
				Vector3 tmp = dirEdge;
				// ���K��
				dirEdge = dirEdge.normalize();
				//dirToP	= dirToP.normalize();
				// �����ĂȂ�������O�p�`�ɋ߂��_�܂ł̋������擾
				f32 length = Vector3::dot(dirEdge, dirToP);

				f32	persent = length / tmp.length();

				// 1.0f�ȏ�Ȃ�[�̓_��Ԃ�
				if( persent > 1.0f ){
					result = _position[ nextIndex[i] ];
					return result;
				}else if( persent < 0.0f )
				{
					result = _position[i];
					return result;
				}
				// ���̒����ɒ����ēn��
				result = dirEdge * length;
				result = _position[i] + result;
				return result;
			}
		}
		return result;
	}

	//! �d�S���擾
	Vector3		getCenter() const
	{
		Vector3	center;

		center._x = (_position[0]._x + _position[1]._x + _position[2]._x) / 3.0f;
		center._y = (_position[0]._y + _position[1]._y + _position[2]._y) / 3.0f;
		center._z = (_position[0]._z + _position[1]._z + _position[2]._z) / 3.0f;

		return center;
	}

	//@}
	
public:
	Vector3		_position[3];		//!< 3�_�̈ʒu���W
};


//-----------------------------------------------------------------------------
//!	����	Line
//-----------------------------------------------------------------------------
struct Line
{
public:
	//---- �����̎��
	enum TYPE {
		TYPE_LINE,		//!< ���C��	(Line)
		TYPE_RAY,		//!< ����	(Ray)
		TYPE_SEGMENT	//!< ����	(Line Segment)
	};

	//-------------------------------------------------------------
	//!	@name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	Line()
	{
		_type = Line::TYPE_LINE;
	}

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �p�����[�^t�Ŏw�肷��ʒu�̍��W���v�Z
	Vector3 getPosition(f32 t) const
	{
		// P' = (E - S) * t + S;
		return (_position[1] - _position[0]) * t + _position[0];
	}

	//@}
	//-------------------------------------------------------------
	//!	@name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �f�o�b�O�p�`��
	void	DebugRender();

	//@}
public:
	Vector3		_position[2];			//!< �n�_�E�I�_�̂Q�_
	Line::TYPE	_type;
};

//-----------------------------------------------------------------------------
//!	����	Ray
//-----------------------------------------------------------------------------
struct Ray : public Line
{
public:

	//-------------------------------------------------------------
	//!	@name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	Ray()
	{
		_type = Line::TYPE_RAY;
	}

	//@}
public:
};

//-----------------------------------------------------------------------------
//!	����	Line Segment
//-----------------------------------------------------------------------------
struct LineSegment : public Line
{
public:

	//-------------------------------------------------------------
	//!	@name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	LineSegment()
	{
		_type = Line::TYPE_SEGMENT;
	}

	//@}
public:
};


namespace collision {


	//! @param [in]		triangle		�O�p�`
	//! @param [in]		line			����
	//! @param [in]		pT				�Փ˂����_�ƌ������钼���̃p�����[�^�[t���󂯎��ꏊ
	//! @param [in]		pHitPosition	�Փ˓_
	//! @param [in]		pHitNormal		�Փ˓_�̖@��
	void		checkHitPos(const Triangle& triangle,
							const Line&	    line,
							Vector3*		pHitPosition = NULL,
							f32*			pT			 = NULL,
							Vector3*		pHitNormal	 = NULL	);

	//! �Փˌv�Z
	//! �O�p�` vs ����
	//! @param [in]		triangle		�O�p�`
	//! @param [in]		line			����
	//! @param [in]		pT				�Փ˂����_�ƌ������钼���̃p�����[�^�[t���󂯎��ꏊ
	//! @param [in]		pHitPosition	�Փ˓_
	//! @param [in]		pHitNormal		�Փ˓_�̖@��
	bool		isHit( const Triangle& triangle,
					   const Line&	   line,
					   Vector3*		   pHitPosition = NULL,
					   f32*			   pT			= NULL,
					   Vector3*		   pHitNormal	= NULL );

	//! �Փˌv�Z
	//! ���� vs ����
	//! @param [in]		plane			����
	//! @param [in]		line			����
	//! @param [in]		pT				�Փ˂����_�ƌ������钼���̃p�����[�^�[t���󂯎��ꏊ
	bool		isHit( const Plane& plane, const Line& line, f32* pT);

};



//-----------------------------------------------------------------------------
//!	��	Sphere
//-----------------------------------------------------------------------------
struct Sphere 
{
public:

	//-------------------------------------------------------------
	//!	@name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	Sphere(){}
	//! �R���X�g���N�^
	Sphere(Vector3	position, f32	radius)
	: _position(position)
	, _radius(radius)
	{
	}

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �f�o�b�O�\��
	void		DebugRender();

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//---- �ݒ�
	//! ���W�ݒ�
	void		setPosition(Vector3& pos)
	{
		_position = pos;
	}
	//! ���a�ݒ�
	void		setRadius(f32 radius)
	{
		_radius = radius;
	}

	//---- �擾
	//! ���W�擾
	Vector3		getPosition() { return _position; }
	//! ���a�擾
	f32			getRadius()	  { return _radius;	  }
	
	//! �Փˌv�Z
	//! �����m
	//! @param	[in]	sphere �����蔻��̂�������
	bool		isHit(Sphere& sphere);

	//@}
private:
	Vector3		_position;	//!< ���W
	f32			_radius;	//!< ���a
};

//-----------------------------------------------------------------------------
//!	�~	Circle
//-----------------------------------------------------------------------------
struct Circle
{
public:

	//-------------------------------------------------------------
	//!	@name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	Circle(){}
	//! �R���X�g���N�^
	Circle(Vector3 position, f32 radius)
	: _position(position)
	, _radius(radius)
	{
	}

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{
	
	//! �f�o�b�O�\��
	void		DebugRender();

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//---- �ݒ�
	//! ���W�ݒ�
	void		setPosition(Vector3& pos)
	{
		_position = pos;
	}
	//! ���a�ݒ�
	void		setRadius(f32 radius)
	{
		_radius = radius;
	}

	//---- �擾
	//! ���W�擾
	Vector3		getPosition() { return _position; }
	//! ���a�擾
	f32			getRadius()	  { return _radius;	  }
	
	//! �Փˌv�Z
	//! �~���m
	//! @param	[in]	circle �����蔻��̂������~
	bool		isHit(Circle& circle);
	
	//@}
private:
	Vector3		_position;		//!< ���W
	f32			_radius;		//!< ���a
};

//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
struct Frustum
{
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	Frustum()
	: _neard(0.0f)
	, _fard (0.0f)
	, _fov  ( Radian( Degree(85.0f) ) )
	, _ratio( GmWindow()->getWindowRatio() )
	{
		// ���W�̏�����
		for(s32 i=0; i<8; ++i){
			_point[i] = Vector3(0.0f);
		}
	}
	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �������8�_�̍X�V
	//! @param	[in]	center		���S���W
	//! @param	[in]	matrix		���ƂȂ�p���s��
	void		updateFrustumPoints(Vector3& center, Matrix& matrix);

	//! �������6�ʂ̍X�V
	void		updateFrustumPlane();

	//! �f�o�b�O�`��
	void		debugRender(Color& col) const;

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �Փˌv�Z
	//! �����m
	//! @param	[in]	sphere �����蔻��̂�������
	bool		isHit(Sphere& sphere);

	//! ���ݒ�
	//! @param	[in]	neard	z�ߋ���
	//! @param	[in]	fard	z������
	//!	@param	[in]	fov		��p(Degree)
	//!	@param	[in]	aspect	�A�X�y�N�g��
	void		setStatus(f32 neard, f32 fard, Degree fov, f32 aspect)
	{
		_neard	= neard;
		_fard	= fard;
		_fov	= Radian(fov);
		_ratio	= aspect;
	}

	//@}

	//---- �����o�[�ϐ�
	f32		_neard;
	f32		_fard;
	f32		_fov;
	f32		_ratio;
	Vector3 _point[8];

	Vector3 _position;
	Plane	_plane[6];
};


//=============================================================================
//	END OF FILE
//=============================================================================