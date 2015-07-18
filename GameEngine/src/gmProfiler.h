//-----------------------------------------------------------------------------
//!
//!	@file	gmProfilter.h
//!	@brief	�v���t�@�C���[
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	�p�t�H�[�}���X���
//-----------------------------------------------------------------------------
class PerfAnalizer
{
public:
	class Node;		// �v�����

	//----------------------------------------------------------
	//!	@name ������
	//----------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	PerfAnalizer();

	//! �f�X�g���N�^
	virtual ~PerfAnalizer();

	//@}
	//----------------------------------------------------------
	//! @name �^�X�N�֐�
	//----------------------------------------------------------
	//@{

	//! �m�[�h���J��
	PerfAnalizer::Node* openNode();

	//! �m�[�h�����
	void				closeNode();

	//! �v���J�n
	void				begin();

	//! �v���I��
	void				end();

	//! �`��
	void				render();

private:

	//! �m�[�h�̕`��(�ċA)
	void				drawNode(PerfAnalizer::Node* pNode, s32 nest);

	//@}

	u64					_baseTime;	//!< �S�̊J�n����

	PerfAnalizer::Node*	_pRoot;		//!< �v���c���[�̃g�b�v

	PerfAnalizer::Node*	_pCurrent;	//!< ���ݎg�p���̃m�[�h

};

extern PerfAnalizer	gPerfAnalizer;

//-----------------------------------------------------------------------------
//!	��̓f�[�^�m�[�h
//-----------------------------------------------------------------------------
class PerfAnalizer::Node
{
public:

	//----------------------------------------------------------
	//!	@name ������
	//----------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	Node()
	:_pNext  (nullptr)
	,_pChild (nullptr)
	,_pParent(nullptr)
	{
	}

	//! �f�X�g���N�^
	~Node()
	{
		delete _pNext;
		delete _pChild;
	}

	//@}
	//----------------------------------------------------------
	//! @name �^�X�N�֐�
	//----------------------------------------------------------
	//@{

	//! �m�[�h�����ׂĔj������
	void	cleanup()
	{
		delete _pChild;
		_pChild = nullptr;
	}

	//! �q���Ƀm�[�h��ǉ�����
	void	addChild(Node* pNode)
	{
		if( _pChild ) { //---- �q�̐擪�ɑ}��
			pNode->_pNext = _pChild;
			_pChild		  = pNode;
		}else{			//---- �q�ɒǉ����鏉��
			_pChild = pNode;
		}
		// �e���̒ǉ�
		pNode->_pParent = this;
	}

	//@}
public:
	GM_CSTR		_name;			//!< �^�C�}�[��
	Color		_color;			//!< �`��F
	u64			_startTime;		//!< �J�n����
	u64			_finishTime;	//!< �I������

	Node*		_pNext;			//!< �Z��
	Node*		_pChild;		//!< �q
	Node*		_pParent;		//!< �e
};

//-----------------------------------------------------------------------------
//!	�v���C���^�[�t�F�C�X
//-----------------------------------------------------------------------------
class Profiler
{
public:

	//----------------------------------------------------------
	//!	@name ������
	//----------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	//! @param	[in]	name	�v����
	//! @param	[in]	color	�\���J���[
	Profiler(GM_CSTR name, const Color& color);

	//! �f�X�g���N�^
	~Profiler();

	//@}
private:
	PerfAnalizer::Node*		_pNode;	//!< �v�����ʕۑ���
};

// �ϐ����ɕʂ̂��̂��������Ă��Ԃ�Ȃ��悤�ɂ���
#define GM_I1_CONCAT(a, b) GM_I2_CONCAT( a ## b )
#define GM_I2_CONCAT(ab)   ab
#define GM_CONCAT(a, b)	   GM_I1_CONCAT(a, b)


#define GM_PROFILE(NAME_, COLOR_) Profiler GM_CONCAT(gm_profilter_node_, __LINE__) ( NAME_, COLOR_ );

//=============================================================================
//	END OF FILE
//=============================================================================
