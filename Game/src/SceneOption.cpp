//-----------------------------------------------------------------------------
//!
//!	@file	SceneTitle.cpp
//!	@brief	�^�C�g�����
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

//-----------------------------------------------------------------------------
//	�^�C�g���V�[�� SceneTitle
//-----------------------------------------------------------------------------
GM_TYPE_IMPLEMENT(SceneOption, "�I�v�V����");



namespace Option {

static const int	ITEM_COUNT_MAX = SceneOption::SELECT_MAX;

//! ���ڂ��I������Ă邩�ǂ���
bool		isSelect[ITEM_COUNT_MAX] = {
	false,
	false,
	false,
	false,
};

//! �^�C�g���̍��ڂ̃|�W�V�������X�g
Float2		ItemPosList[ITEM_COUNT_MAX] = {
	Float2(50.0f,350.0f),
	Float2(50.0f,500.0f),
	Float2(50.0f,650.0f),
	Float2(50.0f,800.0f),
};


//! �^�C�g�����ڂ̃T�C�Y���X�g
Float2		ItemSizeList[ITEM_COUNT_MAX] = {
	Float2(600.0f, 143.0f),
	Float2(600.0f,  95.0f),
	Float2(600.0f,  98.0f),
	Float2(600.0f, 118.0f),
};

//! �Q�[�W�̃T�C�Y���X�g
Float2		GaugeSizeList[ITEM_COUNT_MAX] = {
	Float2( 50.0f, 50.0f),
	Float2(800.0f, 50.0f),
	Float2(800.0f, 50.0f),
	Float2(800.0f, 50.0f),
};

}	// namespace Option

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
SceneOption::SceneOption()
: _nowJump			(true)
, _isFullScreen		(false)
, _pBackGround		(nullptr)
, _pSettingBar		(nullptr)
, _pSettingCheckBox	(nullptr)
, _pTitle			(nullptr)
{
	for (s32 i=0; i<SELECT_MAX; ++i){
		_font[i] = nullptr;
		SAFE_NEW(_font[i]);
	}

	if (!_font[0]->loadAndTexImage2D("../Image/UI/fullScreen.png")){
		MessageBox(NULL, L"�t���X�N���[�������摜�ǂݍ��ݎ��s", L"�G���[", MB_OK);
	}
	if (!_font[1]->loadAndTexImage2D("../Image/UI/MasterVolume.png")){
		MessageBox(NULL, L"�}�X�^�[�{�����[�������摜���s", L"�G���[", MB_OK);
	}
	if (!_font[2]->loadAndTexImage2D("../Image/UI/BGMVolume.png")){
		MessageBox(NULL, L"BGM�{�����[�������摜�ǂݍ��ݎ��s", L"�G���[", MB_OK);
	}
	if (!_font[3]->loadAndTexImage2D("../Image/UI/SEVolume.png")){
		MessageBox(NULL, L"SE�{�����[�������摜�ǂݍ��ݎ��s", L"�G���[", MB_OK);
	}

	SAFE_NEW(_pBackGround);

	Size<s32>	windowSize = GmWindow()->getResolution();
	if (!_pBackGround->createRenderTarget(windowSize._w, windowSize._h, GL_RGBA16F_ARB, GL_RGBA, GL_HALF_FLOAT_ARB, true)){
		MessageBoxA(NULL, "RenderTarget�̍쐬�Ɏ��s���܂����B"__FILE__, "ERROR", MB_OK);
	}


	SAFE_NEW(_pTitle);
	//! �^�C�g�������ǂݍ���
	if(!_pTitle->loadAndTexImage2D("../Image/option.png")){
		MessageBoxA(nullptr, "�I�v�V�����^�C�g�������摜�ǂݍ��ݎ��s", "�摜�̓ǂݍ��݃G���[", MB_OK);
	}

	//! �Z�b�e�B���O�o�[�ǂݍ���
	SAFE_NEW(_pSettingBar);
	if(!_pSettingBar->loadAndTexImage2D("../Image/UI/SettingBar.png")){
		MessageBoxA(nullptr, "�ݒ�̃Q�[�W�p�o�[", "�摜�̓ǂݍ��݃G���[", MB_OK);
	}
	//! �`�F�b�N�{�b�N�X�ǂݍ���
	SAFE_NEW(_pSettingCheckBox);
	if(!_pSettingCheckBox->loadAndTexImage2D("../Image/UI/SettingCheckBox.png")){
		MessageBoxA(nullptr, "�ݒ�̃`�F�b�N�{�b�N�X", "�摜�̓ǂݍ��݃G���[", MB_OK);
	}

	_select = SELECT_MASTER_VOLUME;

	// �t�B���^�[�̏�����
	if(!_gaussianFilter.setup()){
		MessageBoxA(nullptr, __FILE__"�K�E�V�A���t�B���^�̏������Ɏ��s", "�t�B���^�������G���[", MB_OK);
	}

}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
SceneOption::~SceneOption()
{
	SAFE_DELETE(_pSettingCheckBox);
	SAFE_DELETE(_pSettingBar);
	SAFE_DELETE(_pTitle);
	SAFE_DELETE(_pBackGround);

	for( s32 i=0; i<3; ++i ){
		SAFE_DELETE(_font[i]);
	}

	// �t�B���^�[�̉��
	_gaussianFilter.cleanup();
}
//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool SceneOption::init()
{
	// �A���t�@��r�l�ݒ�
	glAlphaFunc(GL_GEQUAL, 0.1f);

	// �I���t���OOFF
	_isExit = false;
	

	return true;
}

//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void SceneOption::update()
{
	if(_nowJump){
		// �w�i�̍쐬
		createBackGroundImage();
		_nowJump = false;
	}

	//f32 test = log(8.0f) / log(2.0f);
	//f32 t = log10(10.0f);
	//f32 f = log(1.0000002);

	// 1P�̃R���g���[���[�擾
	Controller* control = GmControlMan()->getController(1);
	update(control[0]);
//	render();
}

//-----------------------------------------------------------------------------
//! �L�[����
//-----------------------------------------------------------------------------
void SceneOption::checkKey(Controller& control)
{
	enum {
		KEY_U = (1 << 0),		// 1 : 0001
		KEY_D = (1 << 1),		// 2 : 0010
		KEY_E = (1 << 2),		// 4 : 0100		
		KEY_B = (1 << 3),		// 8 : 1000		
	};

	u32	key = 0;
	key |= (GetKeyState(VK_UP)		& 0x80)	? KEY_U : 0;
	key |= (GetKeyState(VK_DOWN)	& 0x80)	? KEY_D : 0;
	key |= (GetKeyState(VK_RETURN)	& 0x80)	? KEY_E : 0;
	key |= (GetKeyState(VK_ESCAPE)  & 0x80) ? KEY_B : 0;
	
	static u32 oldKey = 0;
	u32		   trigger = (key ^ oldKey) & key;

	oldKey = key;


	_keyUp     = control.GetPushState(Controller::PAD_LEFT_U_STICK) |
			     control.GetPushState(Controller::PAD_UP)			|
			     ((trigger & KEY_U) ? true : false);
	_keyDown   = control.GetPushState(Controller::PAD_LEFT_D_STICK) |
			     control.GetPushState(Controller::PAD_DOWN)		    |
			     ((trigger & KEY_D) ? true : false);
	if(_keyUp || _keyDown){
		_keyLeft  = false;
		_keyRight = false;
	}else{
		_keyLeft  = control.GetPadState(Controller::PAD_LEFT_L_STICK) |
				    control.GetPushState(Controller::PAD_LEFT)		  |
				    ((GetKeyState(VK_LEFT)  & 0x80) ? true : false);
		_keyRight = control.GetPadState(Controller::PAD_LEFT_R_STICK) |
					control.GetPushState(Controller::PAD_RIGHT)		  |
					((GetKeyState(VK_RIGHT) & 0x80) ? true : false);
	}

	_keySelect = control.GetPushState(Controller::PAD_A)			|
				 ((trigger & KEY_E) ? true : false);

	_keyBack   = control.GetPushState(Controller::PAD_B) |
				 ((trigger & KEY_B) ? true : false);
		


}

//-----------------------------------------------------------------------------
//! ���E�L�[�ɂ�鉹�ʒ���
//-----------------------------------------------------------------------------
bool SceneOption::changeVolumeByKey(f32& volume)
{
	f32 beforeChange = volume;
	if (_keyLeft){
		volume -= 0.01f;
	}
	else if (_keyRight){
		volume += 0.01f;
	}

	// �͈͊O�w��
	volume = min(1.0f, volume);
	volume = max(0.0f, volume);

	// �l���ύX����Ă�����true
	return (beforeChange - volume != 0.0f);
}
//-----------------------------------------------------------------------------
//! �I�����Ă�����̂ɂ��X�V
//-----------------------------------------------------------------------------
void SceneOption::updateSelect()
{
	// �I�����Ă�����̂ɂ���ď�������
	switch(_select)
	{
		case SELECT_FULLSCREEN:
		{
			if(_keySelect){
				_isFullScreen ^= true;
				GmMain()->fullScreen(_isFullScreen);
			}
			break;
		}
		case SELECT_MASTER_VOLUME:
		{
			f32 volume = ISoundManager()->getMasterVolume();
			if(changeVolumeByKey(volume)){
				ISoundManager()->setMasterVolume(volume);
			}
			break;
		}
		case SELECT_BGM_VOLUME:
		{
			f32 volume = IBGMManager()->getBGMVolume();
			if (changeVolumeByKey(volume)){
				IBGMManager()->setBGMVolume(volume);
			}
			break;
		}
		case SELECT_SE_VOLUME:
		{
			f32 volume = ISEManager()->getSEVolume();
			if (changeVolumeByKey(volume)){
				ISEManager()->setSEVolume(volume);
			}
			break;
		}
	}
	
}

//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void SceneOption::update(Controller& control)
{
	// �L�[����
	checkKey(control);

	if(_keyDown){
		_select = SELECT_STATE(_select + 1);
	}else if(_keyUp){
		_select = SELECT_STATE(_select - 1);
	}

	// �͈͊O�w��
	if (_select < SELECT_FULLSCREEN)	_select = SELECT_SE_VOLUME;
	if (_select > SELECT_SE_VOLUME)		_select = SELECT_FULLSCREEN;
	// ���ׂ�false��
	for (int i = 0; i<Option::ITEM_COUNT_MAX; ++i){
		Option::isSelect[i] = false;
	}
	// �I������Ă���Ƃ����turu��
	Option::isSelect[_select] = true;

	// �I������Ă�����̕ʂ̍X�V
	updateSelect();

	if(_keyBack){
		_nowJump = true;
		GmScene()->jump(SceneTitle::TYPE);
	}

	// �w�i�`��
	render();
}

//-----------------------------------------------------------------------------
//! �t�B���^�[�̉e�����Ȃ��`��
//-----------------------------------------------------------------------------
void SceneOption::drawNoFilter()
{
}

//-----------------------------------------------------------------------------
//! �����݂̃o�b�N�o�b�t�@���R�s�[���Ăڂ����Ĕw�i�Ɏg��
//-----------------------------------------------------------------------------
void SceneOption::createBackGroundImage()
{
	_gaussianFilter.render(GmRender()->getBackBuffer(), _pBackGround);
}

//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void SceneOption::render()
{
	
	// �w�i�`��
	GmRender()->putTexture(_pBackGround, 0, 0,
						   Color::WHITE, Color::WHITE, 1.0f,
						   SystemShader::SHADER_PUT_TEXTURE, 0, false, true);
	
	Size<s32> size = Size<s32>(411.0f, 153.0f);

	
	setBlendMode(BM_BLEND_ALPHA);	// ������ON

	// �^�C�g�������`��
	Size<f32> resScale = GmWindow()->getRateResMax();
	GmRender()->putTexture(_pTitle->getTextureHandle(), 400.0f * resScale._w, 50.0f * resScale._h, size, Color::WHITE, Color::DKGRAY);
	
	// �ő�𑜓x�Ƃ̔䗦
	f32	volumeScales[Option::ITEM_COUNT_MAX] = {
		(_isFullScreen) ? 1.0f : 0.0f,
		SoundPlayer::getMasterVolume(),
		IBGMManager()->getBGMVolume(),
		ISEManager()->getSEVolume(),
	};
	// �`��e�N�X�`�����X�g
	static Texture*	 pDrawTexList[Option::ITEM_COUNT_MAX] = {
		_pSettingCheckBox,
		_pSettingBar,
		_pSettingBar,
		_pSettingBar,
	};

	// �Q�[�W�̐F���X�g
	static Color	inGaugeColorList[Option::ITEM_COUNT_MAX] = {
		Color(0  , 191, 255, 240),
		Color(0  , 191, 255, 240),
		Color(0  , 191, 255, 240),
		Color(0  , 191, 255, 240),
	};

	// �Q�[�W�̃I�t�Z�b�g���X�g
	static Float2	offsetGauge[Option::ITEM_COUNT_MAX]	= {
		Float2(0.0f, 110.0f),
		Float2(0.0f, 110.0f),
		Float2(0.0f, 110.0f),
		Float2(0.0f, 110.0f),
	};


	for(int i=0; i<Option::ITEM_COUNT_MAX; ++i) {
		Float2 v = Option::ItemPosList[i];
		Float2 s = Option::ItemSizeList[i];
		Color maskColor		= Color::NONE;
		Color emissiveColor = Color::NONE;
		if(Option::isSelect[i]){
			maskColor	  = Color(255,   0,   0, 230);
			emissiveColor = Color::WHITE;
		}else{
			maskColor = Color(255, 255, 255, 230);
		}


		

		// ��ʃT�C�Y�ɍ��킹�Ĉʒu�A�T�C�Y�␳
		v._x *= resScale._w;
		v._y *= resScale._h;
		s._x *= resScale._w;
		s._y *= resScale._h;

		size = Size<s32>(s._x, s._y);
		// �����`��
		GmRender()->putTexture(_font[i]->getTextureHandle(), v._x, v._y, Size<s32>(s._x, s._y), maskColor, emissiveColor);


		//---- �Q�[�W�`��
		s = Option::GaugeSizeList[i];					// �Q�[�W�̃T�C�Y������Ă���
		Size<s32> texSize = pDrawTexList[i]->getSize();	// �Q�[�W�g�̃T�C�Y
		f32	gaugeScaleW	  = s._x / texSize._w;			// �Q�[�W�̃X�P�[���l
		f32 gaugeScaleH	  = s._y / texSize._h;			// �Q�[�W�̃X�P�[���l

		// �Q�[�W�̃I�t�Z�b�g�����炷
		v._x += offsetGauge[i]._x * resScale._w;
		v._y += offsetGauge[i]._y * resScale._h;

		// �Q�[�W���`��
		f32	outLineWidth  = 10.0f;						// �Q�[�W�̘g�̕�
		f32 widthOffset   = outLineWidth * gaugeScaleW;	// �ǂꂾ���Q�[�W�̕`�悩�炸�炷��
		f32 heightOffset  = outLineWidth * gaugeScaleH;	// �ǂꂾ���Q�[�W�̕`�悩�炸�炷��
		// �Q�[�W�`��̂��߂̃I�t�Z�b�g(�Q�[�W�g�Ɏ��܂�悤�Ɍv�Z)
		v._x += (500 + widthOffset) * resScale._w;		
		v._y += heightOffset		* resScale._h;
		f32 w = ((texSize._w * gaugeScaleW) - (widthOffset  * 2.0f)) * resScale._w;
		f32 h = ((texSize._h * gaugeScaleH) - (heightOffset * 2.0f)) * resScale._h;
		size = Size<s32>(w, h);
		// �Q�[�W�̒����l�p�ŕ`��
		drawRect(v._x, v._y, size._w * volumeScales[i], size._h, inGaugeColorList[i]);
		setBlendMode(BM_BLEND_ALPHA);	// drawRect�Ńu�����h���[�h��NONE�ɂȂ�̂Őݒ�
		// �Q�[�W�A�E�g���C��
		// �Q�[�W�g���̃I�t�Z�b�g�����ɖ߂�
		v._x -= widthOffset  * resScale._w;
		v._y -= heightOffset * resScale._h;
		w	 = texSize._w * gaugeScaleW	* resScale._w;
		h	 = texSize._h * gaugeScaleH	* resScale._h;
		size = Size<s32>(w, h);
		GmRender()->putTexture(pDrawTexList[i]->getTextureHandle(), v._x, v._y, size, maskColor);
		

	}

	setBlendMode(BM_NONE);

	
}

//=============================================================================
//	END OF FILE
//=============================================================================