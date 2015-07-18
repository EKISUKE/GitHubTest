//-----------------------------------------------------------------------------
//!
//!	@file	gmTimer.h
//!	@brief	�^�C�}�[�֘A
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

extern u64		getCounter();
extern u64		getFrequency();


//-----------------------------------------------------------------------------
//!	�^�C�}�[���
//-----------------------------------------------------------------------------
class TimerBase
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	TimerBase(){}

	//! �f�X�g���N�^
	virtual ~TimerBase(){}

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! ���Z�b�g
	virtual void	Reset() = 0;

	//! �^�C�}�[�̊J�n
	virtual void StartTimer() = 0;

	//! �^�C�}�[�̏I��
	virtual void EndTimer() = 0;


	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �g�[�^���^�C���擾
	virtual s64		getTotalTime() const = 0;

	//@}
};


//-----------------------------------------------------------------------------
//!	CPU�f�o�b�O�p�^�C�}�[���
//-----------------------------------------------------------------------------
class DebugCPUTimerBase
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	DebugCPUTimerBase()
	{
		// �J�E���^�[���g�����L�^
		QueryPerformanceFrequency((LARGE_INTEGER*)&_Frequency);
		// ������
		Reset();
	}

	//! �f�X�g���N�^
	virtual ~DebugCPUTimerBase(){}

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! ���Z�b�g
	virtual void	Reset()
	{
		_startTime	= 0;
		_endTime	= 0;
	}

	//! �^�C�}�[�̊J�n
	virtual void StartTimer()
	{
		// �J�n���ԋL�^
		QueryPerformanceCounter((LARGE_INTEGER*)&_startTime);
	}

	//! �^�C�}�[�̏I��
	virtual void EndTimer()
	{
		// �I�����ԋL�^
		QueryPerformanceCounter((LARGE_INTEGER*)&_endTime);
		// �J�E���^���g���ɉ����L�^����Ă���
		if( _Frequency != 0 ){
			// �S�̎��ԋL�^
			_totalTime = (_endTime - _startTime) * 1000 / _Frequency;
		}
		Reset();
	}

	//! �S�̎��Ԃ̕`��
	virtual void DrawTotalTime(Vector3 drawPos, char* str)
	{
		stringstream ss;
		ss << str;
		ss << _totalTime;
		render_string(drawPos._x, drawPos._y, ss.str());
	}

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �g�[�^���^�C���擾
	virtual s64		getTotalTime() const { return _totalTime; }

	//@}
protected:
	s64		_startTime;		//!< �^�C�}�[�J�n����
	s64		_endTime;		//!< �^�C�}�[�I������
	s64		_totalTime;		//!< �S�̂̎���
	s64		_Frequency;		//!< �J�E���^���g��
};

//-----------------------------------------------------------------------------
//!	CPU�f�o�b�O�p�^�C�}�[
//-----------------------------------------------------------------------------
class DebugCPUTimer : public DebugCPUTimerBase
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	DebugCPUTimer(){}
	//! �f�X�g���N�^
	virtual ~DebugCPUTimer(){}

	//@}
	//-------------------------------------------------------------
	//! @name �\��
	//-------------------------------------------------------------
	//@{

	void	popTotalTime()
	{
		char str[255];
		sprintf_s(str, "ToatlTime is %i millseconds", _totalTime);
		MessageBoxA(NULL, str, "ToatlTime", MB_OK);
	}

	//@}
};

// �v���g�^�C�v�錾
class Performance;

//-----------------------------------------------------------------------------
//!	�p�t�H�[�}���X�}�l�[�W���[
//-----------------------------------------------------------------------------
class SystemPerformance
{
private:
	//-------------------------------------------------------------
	//!	@name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	SystemPerformance()
	: _maxSecond(16)
	{
	}

	//! �R�s�[�R���X�g���N�^
	SystemPerformance(SystemPerformance& src){}

	//! �f�X�g���N�^
	virtual ~SystemPerformance();
public:
	//@}
	//-------------------------------------------------------------
	//!	@name �擾
	//-------------------------------------------------------------
	//@{

	//! �C���X�^���X�擾
	static SystemPerformance*	getInstance()
	{
		static SystemPerformance	_myInstance;
		return &_myInstance;
	}

	//! �����p�t�H�[�}���X���擾
	Performance*	getSamePerformance(Performance* perform);

	

	//! �ő�~���b��Ԃ�
	s64		getFrame()
	{
		return _maxSecond;
	}

	
	//@}
	//-------------------------------------------------------------
	//!	@name �o�^�E����
	//-------------------------------------------------------------
	//@{

	//! �p�t�H�[�}���X�̒ǉ�
	void	addPeform(Performance* perform);

	//! �p�t�H�[�}���X�̍폜
	void	deletePeform(Performance* peform);

	//@}
	

	//! �`��
	void	drawPerformanceBar();
	
protected:
	list<Performance*>	_pPerformList;	//!< �p�t�H�[�}���X���X�g
	s64					_maxSecond;		//!< �ő�~���b
};

//-----------------------------------------------------------------------------
// �A�N�Z�X�C���^�[�t�F�C�X
//-----------------------------------------------------------------------------
__forceinline
SystemPerformance*	GmPerformance()
{
	return SystemPerformance::getInstance();
}


//-----------------------------------------------------------------------------
//! �p�t�H�[�}���X
//-----------------------------------------------------------------------------
class Performance  : public DebugCPUTimerBase
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	Performance();

	//! �f�X�g���N�^
	virtual ~Performance();

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �X�^�[�g
	void	StartPerformance()
	{
		StartTimer();
	}
	//! �I��
	void	EndPerformance()
	{
		// �I�����ԋL�^
		EndTimer();
	}

	//! ���Z�b�g
	void	ResetPerformance()
	{
		Reset();
	}

	//! �`��
	void	DrawPerformance(Vector3 drawPos, f32& parentHeight, s64& maxFrame);

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �����p�t�H�[�}���X���擾
	Performance*	getSamePerformance(Performance* perform);

	//! �����̎擾
	f32		getHeight()	{ return _height; }

	//! �F�̐ݒ�
	void	setColor(Color& col) { _drawColor = col; }

	//! ���C���̃p�t�H�[�}���X�ɐݒ�
	void	setMainPerformance();

	//@}
	//-------------------------------------------------------------
	//!	@name �o�^�E����
	//-------------------------------------------------------------
	//@{

	//! �p�t�H�[�}���X�̒ǉ�
	void	addPeform(Performance* perform);

	//! �p�t�H�[�}���X�̍폜
	void	deletePeform(Performance* peform);

	//@}

private:
	f32						_height;		//!< �`��̍���
	Color					_drawColor;		//!< �`��F
	list<Performance*>		_childPeform;	//!< �q�̃p�t�H�[�}���X
};


//=============================================================================
//	END OF FILE
//=============================================================================