//-----------------------------------------------------------------------------
//!
//!	@file	CollisionObject.h
//!	@brief	�I�u�W�F�N�g�����蔻��p
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------

class ObjBase;

//-----------------------------------------------------------------------------
//!	�I�u�W�F�N�g�����蔻��p
//-----------------------------------------------------------------------------
class CollisionObject
{
public:
	//! �f�t�H���g�R���X�g���N�^
	CollisionObject(){}
	//! �R���X�g���N�^
	CollisionObject(Vector3& pos, f32 radius)
	:_hitSphere(pos, radius)
	{
	}

	//! �����ݒ�
	void				setSphereParam(Vector3& pos, f32 radius)
	{
		/*Vector3	tmp = pos;
		tmp._y += _hitSphere.getRadius();*/

		//_hitSphere.setPosition(tmp);
		_hitSphere.setPosition(pos);
		_hitSphere.setRadius(radius);
	}

	//! �I�u�W�F�N�g�Ƃ̓����蔻��
	//! @param	[in]	obj			    �����蔻����s�������I�u�W�F�N�g
	bool				isHitObj(ObjBase*	obj);
	//! �I�u�W�F�N�g�ƍU���Ƃ̂����蔻��
	//! @param	[in]	attackSphere	�����蔻����s�������U���̋�
	bool				isHitSphere(Sphere&	attackSphere);
	//! �����蔻��p�̋��̎擾
	Sphere&				getHitSphere() { return _hitSphere; }
protected:
	Sphere				_hitSphere;		//!< �����蔻��p�̋�
};

//=============================================================================
//	END OF FILE
//=============================================================================