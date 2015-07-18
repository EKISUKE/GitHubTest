//-----------------------------------------------------------------------------
//!
//!	@file	gmAABBTree.h
//!	@brief	AABB-Tree
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//!	���C���̕`�� (Flat)
//!	@param	[in]	p1		���W1
//!	@param	[in]	p1		���W2
//!	@param	[in]	color	�J���[
void	drawLineF(const Vector3& p1, const Vector3& p2, const Vector4& color);


//-----------------------------------------------------------------------------
//! �o�E���f�B���O�{�b�N�X
//-----------------------------------------------------------------------------
struct AABB
{
	//! ��̃o�E���f�B���O�{�b�N�X���쐬
	void		setEmpty()
	{
		_min = Vector3(+FLT_MAX, +FLT_MAX, +FLT_MAX);
		_max = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	}

	//! �w����W���܂ރo�E���f�B���O�Ɋg��
	void		expand(const Vector3& position)
	{
		_min._x = min(_min._x, position._x);
		_min._y = min(_min._y, position._y);
		_min._z = min(_min._z, position._z);
	
		_max._x = max(_max._x, position._x);
		_max._y = max(_max._y, position._y);
		_max._z = max(_max._z, position._z);
	}

	Vector3		_min;
	Vector3		_max;
};

namespace AABBCollision {

	inline

	//!	AABB vs AABB
	//!	@param	[in]	a	AABB�I�u�W�F�N�g
	//!	@param	[in]	b	AABB�I�u�W�F�N�g
	bool isHit(const AABB& a, const AABB& b)
	{
		if( a._max._x < b._min._x ) return false;
		if( a._max._y < b._min._y ) return false;
		if( a._max._z < b._min._z ) return false;

		if( b._max._x < a._min._x ) return false;
		if( b._max._y < a._min._y ) return false;
		if( b._max._z < a._min._z ) return false;

		return true;
	}

	inline

	//! �߂荞�ݗʂ��擾����
	//!	@param	[in]	a	AABB�I�u�W�F�N�g
	//!	@param	[in]	b	AABB�I�u�W�F�N�g
	Vector3 getSinkVal(const AABB& a, const AABB& b)
	{
		Vector3 result = Vector3(0,0,0);

		if( isHit(a, b) )
		{
			//---- ���ׂĂ̍��������߂�
			f32 sinkX1 = a._max._x - b._min._x;
			f32 sinkX2 = b._min._x - a._max._x;
			f32 sinkX3 = a._min._x - b._max._x;
			f32 sinkX4 = b._max._x - a._min._x;

			/*f32 sinkY1 = a._max._y - b._min._y;
			f32 sinkY2 = b._min._y - a._max._y;
			f32 sinkY3 = a._min._y - b._max._y;
			f32 sinkY4 = b._max._y - a._min._y;*/

			f32 sinkZ1 = a._max._z - b._min._z;
			f32 sinkZ2 = b._min._z - a._max._z;
			f32 sinkZ3 = a._min._z - b._max._z;
			f32 sinkZ4 = b._max._z - a._min._z;

			//--- ���ꂼ��ň�ԏ������߂荞�ݗʂ����߂�
			f32 sinkMinX = min( min(abs(sinkX1), abs(sinkX2))
							  , min(abs(sinkX3), abs(sinkX4)) );
			////--- ���ꂼ��ň�ԏ������߂荞�ݗʂ����߂�
			//f32 sinkMinY = min( min(abs(sinkY1), abs(sinkY2))
			//				  , min(abs(sinkY3), abs(sinkY4)) );

			f32 sinkMinZ = min( min(abs(sinkZ1), abs(sinkZ2))
							  , min(abs(sinkZ3), abs(sinkZ4)) );

			//f32 minVal = min( min(sinkMinX, sinkMinY), sinkMinZ );
			f32 minVal = min( sinkMinX, sinkMinZ );

			//---- ��Βl�Ŕ�r���Ă���̂Œl��߂�
			sinkMinX = math::getSameValofFour(sinkMinX, sinkX1, sinkX2, sinkX3, sinkX4);
			//sinkMinY = math::getSameValofFour(sinkMinY, sinkY1, sinkY2, sinkY3, sinkY4);
			sinkMinZ = math::getSameValofFour(sinkMinZ, sinkZ1, sinkZ2, sinkZ3, sinkZ4);

			//---- ��ԏ������߂荞�ݗʂ������Ă���ɂ���
			if( minVal == abs(sinkMinX) ) {
				result = Vector3(sinkMinX,		  0,		0);
			}else{
				result = Vector3(       0,		  0, sinkMinZ);
			}

		}

		return result;

	}

}	// namespace AABBCollision

extern void	drawLineF(const Vector3& p1, const Vector3& p2, const Vector4& color);

extern void drawAABB(const AABB& aabb, const Vector4& color, const Vector3& offset = Vector3(0,0,0));


struct CollisionData;

//-----------------------------------------------------------------------------
//!	AABB-Tree
//-----------------------------------------------------------------------------
class AABBTree
{
public:
	struct Node {
		//! �R���X�g���N�^
		Node()
		{
			_aabb.setEmpty();
			_pChild[0] = nullptr;
			_pChild[1] = nullptr;
		}

		//vector<Triangle> _triangles;				//!< �O�p�`�̔z��
		//vector<CollisionData> _colData;				//!< �����蔻����
		CollisionData*		  _col;					//!< �����蔻����
		AABB				  _aabb;				//!< ���ׂĂ��͂ރo�E���f�B���O�{�b�N�X
		s32					  _dataCount;			//!< �ŏI�m�[�h�ȊO�̓f�[�^�������Ȃ�

		std::unique_ptr<AABBTree::Node>	_pChild[2];	//!< �q�m�[�h

		//! �O�p�`���X�g����m�[�h�����\�z
		//! @param	[in]	colData	�����蔻����̔z��f�[�^
		bool			build(vector<CollisionData> colData);


		// aabb�ƏՓ˃`�F�b�N
		//!	@param	[in]	aabb	�����蔻����s��AABB
		//!	@param	[in]	hitFunc	�O�p�`�Ƃ̏Փ˔���p�̊֐��|�C���^
		bool			hitCheck(const AABB& aabb, bool(*hitFunc)(Triangle* tri));


		//! �f�o�b�O�`��
		//!	@param	[in]	nest	�ǂ̐[����`�悷�邩
		void			debugDraw(u32 nest);
	};

	//! �R���X�g���N�^
	AABBTree();

	//! �f�X�g���N�^
	virtual ~AABBTree();

	//! �c���[�\�z
	bool				build(vector<CollisionData>& colData);

	//! �f�o�b�O�p�̏���
	bool				debugSetup();

	//! �f�o�b�O�`��
	void				debugRender();

private:

	AABBTree::Node		_rootNode;		//!< ���[�g�m�[�h
};


//===========================================================================
// END OF FILE
//===========================================================================
