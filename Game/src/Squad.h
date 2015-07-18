//-----------------------------------------------------------------------------
//!
//!	@file	Squad.h
//!	@brief	����
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------



struct Member
{
public:
	Man*	_me;				// �����̃L�����f�[�^
	Vector3	_offsetPosition;	// ���[�_�[����̃I�t�Z�b�g�l
};

//-----------------------------------------------------------------------------
//! ����
//-----------------------------------------------------------------------------
class Squad
{
public:

	//-------------------------------------------------------------
	//!	@name	������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	Squad()
	{
		Init();
	}


	//! �f�X�g���N�^
	virtual ~Squad()
	{
		list<Member*>::iterator itr;
		for( itr=_pMemberList.begin(); itr!= _pMemberList.end(); )
		{
			Member* mem = *itr;
			SAFE_DELETE(mem);
			itr = _pMemberList.erase(itr);
		}
	}

	//! ������
	void Init()
	{
		_pMemberList.clear();
	}

	//@}
	//-------------------------------------------------------------
	//! @name �ǉ��E�폜
	//-------------------------------------------------------------
	//@{


	//! �����o�[�̒ǉ�
	void	addMenber(Man* member);
	//! �����o�[�̗��E
	void	leftMenber(Man* member);

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! ���[�_�[�̐ݒ�
	void	setLeader(Man* leader)
	{
		_leader = leader;
	}

	//! ���[�_�[�擾
	Man*	getLeader()	const { return _leader; }

	//! �����̃I�t�Z�b�g�ʒu�̎擾
	Vector3	getMyPostion(Man* member)
	{
		Member*	tmpMem = getSameMember(member);
		// ����������
		if( tmpMem )
		{
			// �����̃I�t�Z�b�g�ʒu
			Vector3 myOffSetPosition;
			// �I�t�Z�b�g���Ƃ��Ă���
			myOffSetPosition = tmpMem->_offsetPosition;
			// �v���C���[����̉�]�s��̍쐬
			Matrix	matLeaderRot;
			matLeaderRot = matLeaderRot.rotateY(Radian(_leader->getRotation()._y));
			myOffSetPosition = myOffSetPosition.transform(matLeaderRot);

			//return _leader->getPosition() + tmpMem->_offsetPosition;
			// �I�t�Z�b�g���ʂ�Ԃ�
			return _leader->getPosition() + myOffSetPosition;
		}
	}

	//! �����o�[�̎擾
	Member*	getSameMember(Man* menber)
	{
		// �����o�[�̃��X�g����S����
		for( list<Member*>::iterator itr = _pMemberList.begin(); itr!= _pMemberList.end(); itr++ )
		{
			Member*	tmpMem = *itr;
			// ����������
			if(  tmpMem->_me == menber )
			{
				// ���̃C�e���[�^�[��Ԃ�
				return *itr;
			}
		}
		// ������Ȃ�������NULL��Ԃ�
		return NULL;
	}

	//@}

protected:
	Man*					_leader;						// ������
	list<Member*>			_pMemberList;					// ���������o�[
	static const s32		MEMBER_MAX = 10;				// �����o�[�̍ő吔
	static const Vector3	offsetPosition[MEMBER_MAX];		// ���[�_�[����̃I�t�Z�b�g�l
	
};

//=============================================================================
//	END OF FILE
//=============================================================================