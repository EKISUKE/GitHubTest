//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilter.h
//!	@brief	�|�X�g�G�t�F�N�g�t�B���^���
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	�|�X�g�G�t�F�N�g�t�B���^�[���
//-----------------------------------------------------------------------------
class TaskFilter : public TaskBase
{
public:
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	TaskFilter();

	//! �f�X�g���N�^
	virtual ~TaskFilter();

	//@}
	//-------------------------------------------------------------
	//!	@name	�^�X�N�֐�
	//! �֐����p�����ăJ�X�^�����܂��B
	//-------------------------------------------------------------
	//@{

	//! ������
	virtual bool		setup(Size<s32>& workTexResolution = GmWindow()->getResolution());

	//! ���
	virtual void		cleanup();

	//! ����X�V
	virtual void		asyncUpdate(){}

	//! ����X�V
	virtual void		update(){}

	//! �`��
	virtual void		render();

	//@}

	//! ���[�N�e�N�X�`�����擾
	Texture*			getWorkTexture(s32 index) { return _pWorkTexture[index]; }

protected:
	//-------------------------------------------------------------
	//!	@name	�����֐�
	//-------------------------------------------------------------
	//@{

	//! �o�b�N�o�b�t�@���烏�[�N�e�N�X�`���ɃR�s�[
	//! @param	[in]	shaderPass  �R�s�[����Ƃ��Ɏg���V�F�[�_�ԍ�
	//!	@param	[in]	mipLevel	�R�s�[��̃~�b�v���x��
	//!	@param	[in]	isPrevFrame	1�t���[���O�̂��̂��R�s�[���邩�ǂ���
	void				copyToWorkTexture(s32 shaderPass, s32 mipLevel = 0, bool isPrevFrame = false);

	//! ���[�N�e�N�X�`����S��ʂɒ���t��
	//! @param	[in]	index				���[�N�e�N�X�`���̔ԍ�(0 or 1)
	//! @param	[in]	mipLevel			�~�b�v���x��
	//!	@param	[in]	shaderPass			�V�F�[�_�[�p�X�ԍ�(-1�Ȃ炱�̒��ŃV�F�[�_���J�n�A�I�����Ȃ�)
	//!	@param	[in]	pSubTexture			�T�u�e�N�X�`��
	void				put(s32			index, s32 mipLevel, s32 shaderPass = -1, Texture* pSubTexture=nullptr);
	void				put(Texture* pTexture, s32 mipLevel, s32 shaderPass = -1, Texture* pSubTexture=nullptr);

	//! �`��^�[�Q�b�g�̐ݒ�
	//! @param	[in]	index		���[�N�e�N�X�`���̔ԍ�(0 or 1)
	//!	@param	[in]	mipLevel	�~�b�v���x��
	void				setRenderTarget(s32 index, s32 mipLevel);
	void				setRenderTarget(Texture* pRenderTarget, s32 mipLevel=0);

	//! �`��^�[�Q�b�g���o�b�N�o�b�t�@�ɖ߂�
	void				setBackBuffer();

	//@}
protected:
	static const s32	REDUCTION_LEVEL = 7;

private:
	Texture*			_pWorkTexture[2];	//!< �ꎞ���[�N�e�N�X�`��(RenderTarget)
};


//=============================================================================
//	END OF FILE
//=============================================================================