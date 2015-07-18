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
GM_TYPE_IMPLEMENT(SceneTitle, "�^�C�g���V�[��");

static const int	ITEM_COUNT_MAX = 3;

//! ���ڂ��I������Ă邩�ǂ���
bool		isSelect[ITEM_COUNT_MAX] = {
	false,
	false,
	false,
};

//! �^�C�g���̍��ڂ̃|�W�V�������X�g
Float2		TitleItemPosList[ITEM_COUNT_MAX] = {
	Float2(50.0f,500.0f),
	Float2(50.0f,650.0f),
	Float2(50.0f,800.0f),
};
Float2		TitleItemSizeList[ITEM_COUNT_MAX] = {
	Float2(387.0f, 71.0f),
	Float2(199.0f, 71.0f),
	Float2(165.0f, 71.0f),
};

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
SceneTitle::SceneTitle()
: _backGroundImage(nullptr)
, _title		  (nullptr)
, _Abutton		  (nullptr)
{
	for( s32 i=0; i<3; ++i ){
		_font[i] = nullptr;
		SAFE_NEW(_font[i]);
	}
	if( !_font[0]->loadAndTexImage2D("../Image/gameStart.png") ){
		MessageBox(NULL, L"�Q�[���X�^�[�g�{�^���摜�ǂݍ��ݎ��s", L"�G���[", MB_OK);
	}
	if( !_font[1]->loadAndTexImage2D("../Image/option.png") ){
		MessageBox(NULL, L"�I�v�V���������摜���s", L"�G���[", MB_OK);
	}
	if( !_font[2]->loadAndTexImage2D("../Image/exit.png") ){
		MessageBox(NULL, L"Exit�����摜�ǂݍ��ݎ��s", L"�G���[", MB_OK);
	}

	SAFE_NEW(_title);

	
	if( !_title->loadAndTexImage2D("../Image/Title.png") ){
		MessageBox(NULL, L"�^�C�g�������摜�ǂݍ��ݎ��s", L"�G���[", MB_OK);
	}

	SAFE_NEW(_Abutton);
	if( !_Abutton->loadAndTexImage2D("../Image/titleAButton.png") ) {
		MessageBox(NULL, L"A�{�^�������p�摜�ǂݍ��ݎ��s", L"�G���[", MB_OK);
	}

	// �I�𒆂̍��ڏ�����
	_select = SELECT_NEWGAME;
	// �摜�N���X�̃C���X�^���X
	SAFE_NEW(_backGroundImage);
	// �t�H���g�̃C���X�^���X
	_titleFont = new GLFont(L"MS����", 40);

	//if (!_backGroundImage->loadAndTexImage2D("../Image/test.jpg")){
	if( !_backGroundImage->loadAndTexImage2D("../Image/back_ground.jpg") ){
	//if (!_backGroundImage->loadAndTexImage2D("../Image/darkCastle.jpg")){
		MessageBox(NULL, L"�w�i�̉摜�̓ǂݍ��݂Ɏ��s���܂����B", L"�G���[", MB_OK);
	}
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
SceneTitle::~SceneTitle()
{
	// �T�E���h��~
	//ISoundManager()->stopMusic(SoundPlayerManager::BGM_TITLE);
	SAFE_DELETE(_titleFont);
	SAFE_DELETE(_title);
	SAFE_DELETE(_Abutton);
	SAFE_DELETE(_backGroundImage);
	for( s32 i=0; i<3; ++i ){
		SAFE_DELETE(_font[i]);
	}
	delete _titleFont;
	delete _backGroundImage;
}
//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool SceneTitle::init()
{
	//if( _backGroundImage->load("../Image/TitleBackGround.jpg") == false){
	
	// �ЂƂO�̉��y��~
	IBGMManager()->stopPrevMusic();
	// �^�C�g����BGM�Đ�
	IBGMManager()->playMusic(BGMSoundPlayerManager::BGM_TITLE);

	// �I���t���OOFF
	_isExit = false;

	// �A���t�@��r�l�ݒ�
	glAlphaFunc(GL_GEQUAL, 0.1f);

	for(s32 i=0; i<SELECT_MAX; ++i){
		_cantSelect[i] = false;
	}

	// �I�v�V�����͍���ĂȂ��̂őI���ł��Ȃ��悤�ɂ��Ă���
	_cantSelect[SELECT_OPTION] = false;

	return true;
}

//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void SceneTitle::update()
{

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
void SceneTitle::checkKey(Controller& control)
{
	enum {
		KEY_U = (1 << 0),		// 1 : 0001
		KEY_D = (1 << 1),		// 2 : 0010
		KEY_E = (1 << 2),		// 4 : 0100		
		KEY_B = (1 << 3),		// 8 : 1000		
	};

	u32	key = 0;
	key |= (GetKeyState(VK_UP) & 0x80) ? KEY_U : 0;
	key |= (GetKeyState(VK_DOWN) & 0x80) ? KEY_D : 0;
	key |= (GetKeyState(VK_RETURN) & 0x80) ? KEY_E : 0;
	key |= (GetKeyState(VK_ESCAPE) & 0x80) ? KEY_B : 0;

	static u32 oldKey = 0;
	u32		   trigger = (key ^ oldKey) & key;

	oldKey = key;


	_keyUp		=	control.GetPushState(Controller::PAD_LEFT_U_STICK) |
					control.GetPushState(Controller::PAD_UP) |
					((trigger & KEY_U) ? true : false);
	_keyDown	=	control.GetPushState(Controller::PAD_LEFT_D_STICK) |
					control.GetPushState(Controller::PAD_DOWN) |
					((trigger & KEY_D) ? true : false);

	_keySelect	=	control.GetPushState(Controller::PAD_A) |
					((trigger & KEY_E) ? true : false);

	_keyBack	=	control.GetPushState(Controller::PAD_B) |
					((trigger & KEY_B) ? true : false);


}

//-----------------------------------------------------------------------------
//! �I�����Ă�����̂ɂ��X�V
//-----------------------------------------------------------------------------
void SceneTitle::updateSelect()
{
	// �I�����Ă�����̂ɂ���ď�������
	switch (_select)
	{
		case SELECT_NEWGAME:
		{
			// �I�����Đ�
			ISEManager()->playMusic(SESoundPlayerManager::SE_BUTTON);
			// �Q�[���V�[����
			GmScene()->jump(SceneGame::TYPE);
			break;
		}
		case SELECT_OPTION:
		{
			//�I�v�V������
			GmScene()->jump(SceneOption::TYPE);
			break;
		}
		case SELECT_EXIT:
		{
			// �I���t���O���Ă�
			_isExit = true;
			break;
		}
	}

}


//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void SceneTitle::update(Controller& control)
{
	// �L�[����
	checkKey(control);

	// �L�[�����ɂ��ړ�	
	do{
		if( _keyDown ){
			_select = SELECT_STATE(_select + 1);
		}else if( _keyUp ){
			_select = SELECT_STATE(_select - 1);
		}
	}while(_cantSelect[_select]);

	// �͈͊O�w��
	if( _select < SELECT_NEWGAME ) _select = SELECT_EXIT;
	if( _select > SELECT_EXIT	 ) _select = SELECT_NEWGAME;
	// ���ׂ�false��
	for(int i=0; i<ITEM_COUNT_MAX; ++i){
		isSelect[i] = false;
	}
	// �I������Ă���Ƃ����true��
	isSelect[_select] = true;

	// �I���{�^���������ꂽ��
	if(_keySelect){
		// �I������Ă�����̕ʂ̍X�V
		updateSelect();
	}
	
	// �߂�L�[����������EXIT�{�^����I������
	if(_keyBack){
		_select = SELECT_EXIT;
	}
	
	// �w�i�`��
	render();
}

//-----------------------------------------------------------------------------
//! �t�B���^�[�̉e�����Ȃ��`��
//-----------------------------------------------------------------------------
void SceneTitle::drawNoFilter()
{
}

//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void SceneTitle::render()
{
#if 1
	// �w�i�`��
	Size<s32> windowSize = GmWindow()->getResolution();
	GmRender()->putTexture(_backGroundImage->getTextureHandle(), 0, 0, windowSize, Color::WHITE);



	setBlendMode(BM_BLEND_ALPHA);

	
	// �ő�𑜓x�Ƃ̔䗦
	Size<f32> resScale = GmWindow()->getRateResMax();
	Size<s32> size = Size<s32>(750.0f * resScale._w, 153.0f * resScale._h );

	// �^�C�g�������\��
	GmRender()->putTexture(_title->getTextureHandle(), 550 * resScale._w, 200 * resScale._h, size, Color::WHITE, Color::DKGRAY);

	//---- �^�C�g���̑I�����ڂ̕`��
	for(int i=0; i<ITEM_COUNT_MAX; ++i){
		// ���ڂ̍��W�擾
		Float2 v = TitleItemPosList[i];
		// ���ڂ̃T�C�Y�擾
		Float2 s = TitleItemSizeList[i];
		// �I������Ă��邩�ǂ����ɂ���ĐF��ς���
		Color maskColor;
		if( isSelect[i] ) {
			maskColor = Color::RED;				// �ԐF
		}else{
			if( !_cantSelect[i] ){
				maskColor = Color::WHITE;		// ���F
			}else{
				maskColor = Color(64, 64, 64);	// �D�F
			}
		}

		size = Size<s32>(s._x * resScale._w, s._y * resScale._h);
		GmRender()->putTexture(_font[i]->getTextureHandle(), v._x * resScale._w, v._y * resScale._h, size, maskColor, Color::DKGRAY);
	}

	size = Size<s32>(100.0f * resScale._w, 25.0f * resScale._h);

	// A�{�^�������p
	GmRender()->putTexture(_Abutton->getTextureHandle(), 50.0f * resScale._w, 1000.0f * resScale._h, size);

	setBlendMode(BM_NONE);
#endif
}

//=============================================================================
//	END OF FILE
//=============================================================================