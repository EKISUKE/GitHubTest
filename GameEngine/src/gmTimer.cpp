//-----------------------------------------------------------------------------
//!
//!	@file	Timer.cpp
//!	@brief	�^�C�}�[�֘A
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


u64	getCounter()
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);

	return counter.QuadPart;
}

u64 getFrequency()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);

	return freq.QuadPart;
}


//=============================================================================
//	�p�t�H�[�}���X�}�l�[�W���[����
//=============================================================================

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
SystemPerformance::~SystemPerformance()
{
	// ���X�g�폜
	for( list<Performance*>::iterator itr = _pPerformList.begin(); itr != _pPerformList.end(); )
	{
		Performance* performance = *itr;
		SAFE_DELETE(performance);
		itr = _pPerformList.erase(itr);
	}
}

//-----------------------------------------------------------------------------
//! �����p�t�H�[�}���X���擾
//-----------------------------------------------------------------------------
Performance* SystemPerformance::getSamePerformance(Performance* perform)
{
	for( list<Performance*>::iterator itr = _pPerformList.begin(); itr != _pPerformList.end(); itr++ )
	{
		Performance*	tmpPef = *itr;
		// ����������
		if( tmpPef == perform )
		{
			return *itr;
		}
	}
	// ������Ȃ�����
	return NULL;
}


//-----------------------------------------------------------------------------	
//! �p�t�H�[�}���X�̒ǉ�
//-----------------------------------------------------------------------------
void SystemPerformance::addPeform(Performance* perform)
{
	if( !getSamePerformance(perform) )
	{
		// �o�^
		_pPerformList.push_back(perform);
	}
}

//-----------------------------------------------------------------------------
//! �p�t�H�[�}���X�̍폜
//-----------------------------------------------------------------------------
void SystemPerformance::deletePeform(Performance* perform)
{
	// �������̂��폜����
	_pPerformList.remove(perform);
}

//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void SystemPerformance::drawPerformanceBar()
{

	glDepthMask(GL_FALSE);
	setBlendMode(BM_BLEND_ALPHA);

	// �`����W
	Vector3 drawPos = Vector3(1860.0f, 0.0f, 0.0f);

	// �t���[���̐�
	static u32 frameCount = 3;
	// ���`�⊮����
	static f32 height = 1000.0f;
	// �P�t���[�����̍���
	static f32 oneFrameHeight = height / (f32)frameCount;
	

	// �g��`��
	glColor4f(0.0f,0.0f,0.0f, 0.8f);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(drawPos._x	     , drawPos._y	 	  , 0.0f);
		glVertex3f(drawPos._x + 40.0f, drawPos._y		  , 0.0f);
		glVertex3f(drawPos._x		 , drawPos._y + height, 0.0f);
		glVertex3f(drawPos._x + 40.0f, drawPos._y + height, 0.0f);
	glEnd();

	// �g�̕��������炷
	drawPos._x += 5.0f;

	

	// �`��
	for( list<Performance*>::iterator itr = _pPerformList.begin(); itr != _pPerformList.end(); itr++ )
	{
		Performance*	drawPef = *itr;
		// �`��
		drawPef->DrawPerformance(drawPos, oneFrameHeight, _maxSecond);
		// ���̕`��ʒu�����߂�
		drawPos._y += drawPef->getHeight();
	}

	//---- �t���[���̕������̕`��
	drawPos._x -= 5.0f;
	glBegin(GL_LINES);
	glColor4f(0.0f, 1.0f, 0.5f, 1.0f);
	for( u32 i=0; i<frameCount; ++i ){
		glVertex3f(drawPos._x		 , i * oneFrameHeight, 0.0f);
		glVertex3f(drawPos._x + 40.0f, i * oneFrameHeight, 0.0f);
	}
	glEnd();

	// ���Őݒ肵�Ȃ���
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	setBlendMode(BM_NONE);
	glDepthMask(GL_TRUE);
}


//=============================================================================
//	�p�t�H�[�}���X����
//=============================================================================

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
Performance::Performance()
: _height	(0.0f)
{
	
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
Performance::~Performance() 
{
	// ���X�g�폜(�������ō폜����ƃG���[���N����)
	//for( list<Performance*>::iterator itr = _childPeform.begin(); itr != _childPeform.end(); )
	//{
	//	Performance* performance = *itr;
	//	SAFE_DELETE(performance);
	//	itr = _childPeform.erase(itr);
	//}
}

//-----------------------------------------------------------------------------
//! �����p�t�H�[�}���X���擾
//-----------------------------------------------------------------------------
Performance* Performance::getSamePerformance(Performance* perform)
{
	for( list<Performance*>::iterator itr = _childPeform.begin(); itr != _childPeform.end(); itr++ )
	{
		Performance*	tmpPef = *itr;
		// ����������
		if( tmpPef == perform )
		{
			return *itr;
		}
	}
	// ������Ȃ�����
	return NULL;
}

//-----------------------------------------------------------------------------
//! ���C���̃p�t�H�[�}���X�ɐݒ�
//-----------------------------------------------------------------------------
void Performance::setMainPerformance()
{
	// �o�^
	GmPerformance()->addPeform(this);
}

//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void Performance::DrawPerformance(Vector3 drawPos, f32& parentHeight, s64& maxFrame)
{
	// ���`�⊮����
	_height = LinearInterpolation(0.0f, parentHeight, (f32)_totalTime, (f32)maxFrame);
	// �����̃o�[�̕`��	
	glBegin(GL_TRIANGLE_STRIP);
		glColor4ubv((GLubyte*)&_drawColor);
		glVertex3f(drawPos._x	     , drawPos._y	 	   , 0.0f);
		glVertex3f(drawPos._x + 30.0f, drawPos._y		   , 0.0f);
		glVertex3f(drawPos._x		 , drawPos._y + _height, 0.0f);
		glVertex3f(drawPos._x + 30.0f, drawPos._y + _height, 0.0f);
	glEnd();

	// �q�̃p�t�H�[�}���X�̕`��
	for( list<Performance*>::iterator itr = _childPeform.begin(); itr != _childPeform.end(); itr++ )
	{
		Performance*	drawPef = *itr;
		// �`��
		drawPef->DrawPerformance(drawPos, _height, _totalTime);
		// ���̕`��ʒu�����߂�
		drawPos._y += drawPef->getHeight();
	}

}


//-----------------------------------------------------------------------------	
//! �p�t�H�[�}���X�̒ǉ�
//-----------------------------------------------------------------------------
void Performance::addPeform(Performance* perform)
{
	if( !getSamePerformance(perform) )
	{
		// �o�^
		_childPeform.push_back(perform);
	}
}

//-----------------------------------------------------------------------------
//! �p�t�H�[�}���X�̍폜
//-----------------------------------------------------------------------------
void Performance::deletePeform(Performance* perform)
{
	// �������̂��폜����
	_childPeform.remove(perform);
}



//=============================================================================
//	END OF FILE
//=============================================================================