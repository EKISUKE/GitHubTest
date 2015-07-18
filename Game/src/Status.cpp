//-----------------------------------------------------------------------------
//!
//!	@file	Status.cpp
//!	@brief	��{�X�e�[�^�X
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

//! �e�N�X�`���t��2D�Q�[�W�\��
void drawTexture2D(f32 x, f32 y, f32 w, f32 h, f32 gaugeW, f32 gaugeH, Color& gaugeColor, Texture* pTexture)
{
	setBlendMode(BM_BLEND_ALPHA);
	
	glDepthMask(GL_FALSE);	// Z����
	//---- �Q�[�W�̕`��
	glBegin(GL_TRIANGLE_STRIP);
		
		glColor4ubv((GLubyte*)&gaugeColor);
		glVertex3f(  x,   y, 0);

		glVertex3f(x+w,   y, 0);

		glVertex3f(  x, y+h, 0);

		glVertex3f(x+w, y+h, 0);
			
	glEnd();

	if( pTexture != nullptr ) {
		//---- �Q�[�W�A�E�g���C���̕`��
		Texture::set(pTexture, 0);	//---- �e�N�X�`��ON
		glBegin(GL_TRIANGLE_STRIP);
		
			glColor4ub(255,255,255,255);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(		  x,		y, 0);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(x+gaugeW,		y, 0);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(		  x, y+gaugeH, 0);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(x+gaugeW, y+gaugeH, 0);
			
		glEnd();
		Texture::set(NULL);	//---- �e�N�X�`��OFF
	}

	setBlendMode(BM_NONE);
	glDepthMask(GL_TRUE);	// Z�L��
}

//-----------------------------------------------------------------------------
//! �p�����[�^�l������
//-----------------------------------------------------------------------------
void Status::resetParam()
{
}

//-----------------------------------------------------------------------------
//! CSV�t�@�C���̓ǂݍ���
//-----------------------------------------------------------------------------
void Status::loadCSV(const char* fileName)
{
	//Param* result = new Param();

	FILE*	fp;
	
	char	str[1024];

	// �����R�[�h�����C�h������ɕϊ�
    // �y���Ӂz�{���͂��̉ӏ��͕�����o�b�t�@���̍l���̑��ɕ�����I�[�R�[�h������������Z�L���A�ȑΉ����D�܂����ł��B
	wchar_t		DebugStr[ MAX_PATH ];
    size_t      pathLength = 0;

	// ���P�[���w��
	setlocale(LC_ALL, "japanese");

	fopen_s(&fp, fileName, "rt");
	
	if( fp == NULL )
	{
		sprintf_s(str, sizeof(str), "�X�e�[�^�X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���\n �t�@�C���� = %s", fileName);

		// char����wchar_t�ւ̕ϊ��֐�
		mbstowcs_s( &pathLength,            // [out]    �ϊ����ꂽ������
					&DebugStr[0],           // [out]    �ϊ����ꂽ���C�h��������i�[����o�b�t�@�̃A�h���X(�ϊ���)
					MAX_PATH,               // [in]     �o�͑��̃o�b�t�@�̃T�C�Y(�P��:������)
					str,		            // [in]     �}���`�o�C�g������̃A�h���X(�ϊ���)
					_TRUNCATE );		    // [in]     �o�͐�Ɋi�[���郏�C�h�����̍ő吔
		
		MessageBox(NULL, DebugStr, L"�X�e�[�^�X LOAD", MB_OK);

		return;
	}

#if 0
	char* compareStr[] =
	{
		"HPMAX",
		"HP",
		"MPMAX",
		"MP",
		"ATKDELAY",
		"SKLDELAY",
		"SPEED",
		"ATKPOW",
		"JUMPPOW",
		"GAUGESCALE"
	};
	
	// fgets : ������Ƀt�@�C���̈�s���i�[����
	while( fgets(str, sizeof(str), fp) ) {
		for( s32 i=0; i<Status::STATUS_MAX; i++ ){
			if( strstr( str, compareStr[i] ) )
			{
				f32 temp;
				stringstream ss;
				ss << compareStr[i];
				ss << ",%f";
				sscanf_s(str, ss.c_str(), &temp);
				_param._InfOfParam[i] = temp;
				// ���������玟�̍s��
				break;
			}
		}

		if( strstr( str, "GAUGESCALE") ) {	// �Q�[�W�T�C�Y
			f32 scaleX, scaleY;
			sscanf_s(str, "GAUGESCALE,%f,%f", &scaleX, &scaleY);
			_param._GaugeBoard.setSize(Vector3(scaleX, scaleY, 0.0f));
		}
	}
#else
	// fgets : ������Ƀt�@�C���̈�s���i�[����
	while( fgets(str, sizeof(str), fp) ) {
		// ������Ɣ�r���ăf�[�^�𒊏o
		if( strstr( str, "HPMAX") )	{				// HPMAX
			f32 HPMAX;
			sscanf_s(str, "HPMAX,%f", &HPMAX);
			_param._HPMAX = HPMAX;
		}else if ( strstr( str, "HP") )	{			// HP
			f32 HP;
			sscanf_s(str, "HP,%f", &HP);
			_param._HP = HP;
		}else if( strstr( str, "MPMAX") ) {			// MPMAX
			f32 MPMAX;
			sscanf_s(str, "MPMAX ,%f", &MPMAX);
			_param._MPMAX = MPMAX;
		}else if ( strstr( str, "MP") )	{			// MP
			f32 MP;
			sscanf_s(str, "MP ,%f", &MP);
			_param._MP = MP;
		}else if( strstr( str, "ATKDELAY") ){		// �U���Ԋu
			f32 ATKDELAY;
			sscanf_s(str, "ATKDELAY ,%f", &ATKDELAY);
			_param._atkDelay = ATKDELAY;
		}else if( strstr( str, "SKLDELAY") ){		// �X�L���Ԋu
			f32 SKLDELAY;
			sscanf_s(str, "SKLDELAY ,%f", &SKLDELAY);
			_param._sklDelay = SKLDELAY;
		}else if( strstr( str, "SPEED") ){			// �X�s�[�h�ő�l
			f32 SPEED;
			sscanf_s(str, "SPEED, %f", &SPEED);
			_param._speedMAX= SPEED;
		}else if( strstr( str, "ATKPOW") ) {		// �U����
			f32 ATKPOW;
			sscanf_s(str, "ATKPOW, %f", &ATKPOW);
			_param._atkPow= ATKPOW;
		}else if( strstr( str, "JUMPPOW") ) {		// �W�����v��
			f32 JUMPPOW;
			sscanf_s(str, "JUMPPOW, %f", &JUMPPOW);
			_param._jumpPow= JUMPPOW;
		}else if( strstr( str, "GAUGESCALE") ) {	// �Q�[�W�T�C�Y
			f32 scaleX, scaleY;
			sscanf_s(str, "GAUGESCALE,%f,%f", &scaleX, &scaleY);
			_param._GaugeScaleH = scaleY;
			_param._GaugeScaleW = scaleX;
			_param._GaugeBoard.setSize(Vector3(scaleX, scaleY, 0.0f));
			//result->setGaugeScale(scaleX, scaleY);
		}
	}
#endif 
	// �t�@�C�������
	fclose(fp);
}
//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void Status::Param::Render(bool draw2D, Color& gaugeColor, Vector3 drawPosition, Texture* pTexture)
{
	

	f32 HP, HPMAX;


	HP	  = _HP;
	HPMAX = _HPMAX;
	// �Q�[�W�̍��W���擾
	f32	w = (_GaugeScaleW)* (HP / HPMAX);

	// �̗̓Q�[�W�`��

	// �ő�𑜓x�Ƃ̔䗦
	Size<f32> resScale = GmWindow()->getRateResMax();
	Size<s32> size = Size<s32>( _GaugeScaleW * resScale._w, _GaugeScaleH * resScale._h);

	if( draw2D ) {
		drawPosition *= Vector3(resScale._w, resScale._h, 1.0f);
		drawRect(drawPosition._x, drawPosition._y, w * resScale._w, size._h, gaugeColor);
		setBlendMode(BM_BLEND_ALPHA);
		GmRender()->putTexture(pTexture->getTextureHandle(),
							   drawPosition._x, drawPosition._y,
							   size);
		setBlendMode(BM_NONE);
		
		//drawTexture2D(drawPosition._x, drawPosition._y, GaugeSizeW, _GaugeScaleH, _GaugeScaleW, _GaugeScaleH, gaugeColor, pTexture);
	}else{
		_GaugeBoard.setTexture(pTexture);
		_GaugeBoard.setPosition(drawPosition);
		_GaugeBoard.setSize(Vector3(_GaugeScaleW, _GaugeScaleH, 0.0f));
		_GaugeBoard.setScale(Vector3(1.0f, 1.0f, 0.0f));
		_GaugeBoard.draw(gaugeColor);
	}
}



//-----------------------------------------------------------------------------
//! CSV�t�@�C���̓ǂݍ���
//-----------------------------------------------------------------------------
void StatusHeal::loadCSV(const char* fileName)
{
	FILE*	fp;
	
	char	str[1024];

	// �����R�[�h�����C�h������ɕϊ�
    // �y���Ӂz�{���͂��̉ӏ��͕�����o�b�t�@���̍l���̑��ɕ�����I�[�R�[�h������������Z�L���A�ȑΉ����D�܂����ł��B
	wchar_t		DebugStr[ MAX_PATH ];
    size_t      pathLength = 0;

	// ���P�[���w��
	setlocale(LC_ALL, "japanese");

	fopen_s(&fp, fileName, "rt");
	
	if( fp == NULL )
	{
		sprintf_s(str, sizeof(str), "�X�e�[�^�X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���\n �t�@�C���� = %s", fileName);

		// char����wchar_t�ւ̕ϊ��֐�
		mbstowcs_s( &pathLength,            // [out]    �ϊ����ꂽ������
					&DebugStr[0],           // [out]    �ϊ����ꂽ���C�h��������i�[����o�b�t�@�̃A�h���X(�ϊ���)
					MAX_PATH,               // [in]     �o�͑��̃o�b�t�@�̃T�C�Y(�P��:������)
					str,		            // [in]     �}���`�o�C�g������̃A�h���X(�ϊ���)
					_TRUNCATE );		    // [in]     �o�͐�Ɋi�[���郏�C�h�����̍ő吔
		
		MessageBox(NULL, DebugStr, L"�X�e�[�^�X LOAD", MB_OK);

		return;
	}


#if 0 
	char* compareStr[] =
	{
		"HPMAX",
		"HP",
		"MPMAX",
		"MP",
		"ATKDELAY",
		"SKLDELAY",
		"SPEED",
		"ATKPOW",
		"JUMPPOW",
		"HEALPOW",
		"GAUGESCALE"
	};
	
	// fgets : ������Ƀt�@�C���̈�s���i�[����
	while( fgets(str, sizeof(str), fp) ) {
		for( s32 i=0; i<StatusHeal::STATUS_MAX; i++ ){
			if( strstr( str, compareStr[i] ) )
			{
				f32 temp;
				stringstream ss;
				ss << compareStr[i];
				ss << ",%f";
				sscanf_s(str, ss.c_str(), &temp);
				_param._InfOfParam[i] = temp;
				// ���������玟�̍s��
				break;
			}
		}

		if( strstr( str, "GAUGESCALE") ) {	// �Q�[�W�T�C�Y
			f32 scaleX, scaleY;
			sscanf_s(str, "GAUGESCALE,%f,%f", &scaleX, &scaleY);
			_param._GaugeBoard.setSize(Vector3(scaleX, scaleY, 0.0f));
		}
	}
#else

	// fgets : ������Ƀt�@�C���̈�s���i�[����
	while( fgets(str, sizeof(str), fp) ) {
		// ������Ɣ�r���ăf�[�^�𒊏o
		if( strstr( str, "HPMAX") )	{				// HPMAX
			f32 HPMAX;
			sscanf_s(str, "HPMAX,%f", &HPMAX);
			_paramHeal._HPMAX = HPMAX;
		}else if ( strstr( str, "HP") )	{			// HP
			f32 HP;
			sscanf_s(str, "HP,%f", &HP);
			_paramHeal._HP = HP;
		}else if( strstr( str, "MPMAX") ) {			// MPMAX
			f32 MPMAX;
			sscanf_s(str, "MPMAX ,%f", &MPMAX);
			_paramHeal._MPMAX = MPMAX;
		}else if ( strstr( str, "MP") )	{			// MP
			f32 MP;
			sscanf_s(str, "MP ,%f", &MP);
			_paramHeal._MP = MP;
		}else if( strstr( str, "ATKDELAY") ){		// �U���Ԋu
			f32 ATKDELAY;
			sscanf_s(str, "ATKDELAY ,%f", &ATKDELAY);
			_paramHeal._atkDelay = ATKDELAY;
		}else if( strstr( str, "SKLDELAY") ){		// �X�L���Ԋu
			f32 SKLDELAY;
			sscanf_s(str, "SKLDELAY ,%f", &SKLDELAY);
			_paramHeal._sklDelay = SKLDELAY;
		}else if( strstr( str, "SPEED") ){			// �X�s�[�h�ő�l
			f32 SPEED;
			sscanf_s(str, "SPEED, %f", &SPEED);
			_paramHeal._speedMAX= SPEED;
		}else if( strstr( str, "ATKPOW") ) {		// �U����
			f32 ATKPOW;
			sscanf_s(str, "ATKPOW, %f", &ATKPOW);
			_paramHeal._atkPow= ATKPOW;
		}else if( strstr( str, "JUMPPOW") ) {		// �W�����v��
			f32 JUMPPOW;
			sscanf_s(str, "JUMPPOW, %f", &JUMPPOW);
			_paramHeal._jumpPow= JUMPPOW;
		}else if( strstr( str, "HEALPOW" ) ) {
			f32 HEALPOW;
			sscanf_s(str, "HEALPOW, %f", &HEALPOW);
			_paramHeal._healPow = HEALPOW;
		}else if( strstr( str, "GAUGESCALE") ) {	// �Q�[�W�T�C�Y
			f32 scaleX, scaleY;
			sscanf_s(str, "GAUGESCALE,%f,%f", &scaleX, &scaleY);
			_paramHeal._GaugeScaleH = scaleY;
			_paramHeal._GaugeScaleW = scaleX;
			_paramHeal._GaugeBoard.setSize(Vector3(scaleX, scaleY, 0.0f));
			//result->setGaugeScale(scaleX, scaleY);
		}
	}

#endif
	// �t�@�C�������
	fclose(fp);
}
//=============================================================================
//	END OF FILE
//=============================================================================