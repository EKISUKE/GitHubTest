//-----------------------------------------------------------------------------
//!
//!	@file	SceneGame.cpp
//!	@brief	�Q�[����
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"



//-----------------------------------------------------------------------------
//	�Q�[���V�[�� SceneGame
//-----------------------------------------------------------------------------
GM_TYPE_IMPLEMENT(SceneGame, "�Q�[��");


//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
DebugCPUTimer loopTimer;	// ���[�v�^�C�}�[
DebugCPUTimer updateTimer;	// �X�V�p�^�C�}�[
DebugCPUTimer renderTimer;	// �`��p�^�C�}�[
DebugCPUTimer debugTimer;	// ���ꂼ��̃f�o�b�O�p�^�C�}�[

f32	Global::deltaTime;

//-----------------------------------------------------------------------------
//! ������ǉ�
//-----------------------------------------------------------------------------
bool SceneGame::addAlly(AllyBase*	pAlly)
{
	for( s32 i=0; i<ALLY_COUNT_MAX; i++ )
	{
		if( _pAlly[i] == NULL )
		{
			_pAlly[i] = pAlly;
			// �����ɓo�^
			_pPlayerSquad->addMenber(_pAlly[i]);
			// ��������ݒ�
			_pAlly[i]->setSquad(_pPlayerSquad);
			// �L�[���ݒ�
			IObjDataManager()->addAllyData(_pAlly[i]);
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
//! �G��ǉ�
//-----------------------------------------------------------------------------
bool SceneGame::addEnemy(EnemyBase*	pEnemy)
{
	static s32	squadNum = 0;
	static s32  eneCount = 0;

	s32	   index = eneCount + (ENEMY_SQUAD_MEM_MAX * squadNum);

	if( _pEnemy[index] == nullptr )
	{
		_pEnemy[index] = pEnemy;
		// �����ɓo�^
		_pEnemySquad[squadNum]->addMenber(_pEnemy[index]);
		// ��������ݒ�
		_pEnemy[index]->setSquad(_pEnemySquad[squadNum]);
		// �L�[���ݒ�
		IObjDataManager()->addEnemyData(_pEnemy[index]);
		// �o�^�����̂ŃJ�E���g�𑝂₷
		eneCount++;
		if( eneCount >= ENEMY_SQUAD_MEM_MAX ) {
			squadNum++;
			eneCount = 0;
		}
		return true;
	}
	return false;
}


//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
SceneGame::SceneGame()
: _pMouse				(nullptr)	// �}�E�X
, _pCamera				(nullptr)	// �J����
, _pControl				(nullptr)	// �R���g���[���[
, _pFont				(nullptr)	// �t�H���g
, _pPlayer				(nullptr)	// �v���C���[
, _pPlayerSquad			(nullptr)	// �v���C���[�̕���
//, _pEnemyLeader			(nullptr)	// �G���[�_�[
//, _pEnemySquad			(nullptr)	// �G�̕���(�e�X�g)
, _pLight				(nullptr)	// ���C�g
, _pSkyBox				(nullptr)	// �X�J�C�{�b�N�X
, _pGameClearWord		(nullptr)	// �Q�[���N���A����
, _pGameOverWord		(nullptr)	// �Q�[���I�[�o�[����
, _pXButtonImage		(nullptr)	// XButton�摜
, _pXButtonUI			(nullptr)	// XButton�p
, _pUpdatePerformance	(nullptr)	// �X�V�̃p�t�H�[�}���X
, _pRenderPerformance	(nullptr)	// �`��̃p�t�H�[�}���X
, _isShadow				(false)		// �V���h�E�`��t���O
{
	
	for( s32 i=0; i<ENEMY_COUNT_MAX; i++ )
	{
		_pEnemy[i] = nullptr;
	}
	for( s32 i=0; i<ALLY_COUNT_MAX; i++ )
	{
		_pAlly[i] = nullptr;
	}
	_pMap = nullptr;

	for( s32 i=0; i<2; i++ )
	{
		_pMissionWord[i] = nullptr;
		SAFE_NEW(_pMissionWord[i]);
	}

	// �}�E�X�̏�����
	_pMouse	  = GmMouseMan()->getMouse();
	
	
	// �R���g���[���[������
	_pControl = GmControlMan()->getController(1);
	

	// �J�����̏�����
	SAFE_NEW(_pCamera);

	// �쐬�����J�������X�g�b�N
	GmCameraMan()->createCamera(_pCamera);

	// �t�H���g�̏�����
	_pFont = new GLFont(L"MS����", 24);

	// ������x�����蔻��Ǘ��N���X������
	GmCollisionMan()->Init();
	

	// �v���C���[������
	SAFE_NEW(_pPlayer);
	// �L�[�̏��ݒ�
	IObjDataManager()->setPlayer(_pPlayer);


	// �v���C���[�̕����̏�����
	SAFE_NEW(_pPlayerSquad);
	// �v���C���[�𕪑�����
	_pPlayerSquad->setLeader(_pPlayer);


	//---- �G������
	for( s32 i=0; i<ENEMY_SQUAD_MAX; i++ ) {
		_pEnemyLeader[i] = nullptr;
		_pEnemySquad[i]  = nullptr;
		// �G���[�_�[�̏�����
		SAFE_NEW(_pEnemyLeader[i]);
		_pEnemyLeader[i]->setPosition(Vector3(0.0f));
		// �G�̕���������
		SAFE_NEW(_pEnemySquad[i]);
		_pEnemySquad[i]->setLeader(_pEnemyLeader[i]);
	}


	// ���C�g�̏�����
	SAFE_NEW(_pLight);
	
	// �X�J�C�{�b�N�X�̏�����
	SAFE_NEW(_pSkyBox);
	_pSkyBox->init();

#if 1
	/*_pSkyBox->loadSkyBox("../Image/SkyBoxTexture/skybox/",
						 "front.jpg",
						 "back.jpg",
						 "left.jpg",
						 "right.jpg",
						 "top.jpg",
						 "top.jpg");*/

	/*_pSkyBox->loadSkyBox("../Image/SkyBoxTexture/skybox/hdr/",
						"cubemap_intensity10Test._c04.hdr",
						"cubemap_intensity10Test._c05.hdr",
						"cubemap_intensity10Test._c01.hdr",
						"cubemap_intensity10Test._c00.hdr",
						"cubemap_intensity10Test._c02.hdr",
						"cubemap_intensity10Test._c02.hdr");*/

	_pSkyBox->loadSkyBox("../Image/SkyBoxTexture/h_skyboxpack2_lagoon/hdr/",
						"cubemap_c04.hdr",
						"cubemap_c05.hdr",
						"cubemap_c01.hdr",
						"cubemap_c00.hdr",
						"cubemap_c02.hdr",
						"cubemap_c03.hdr");
#else
	_pSkyBox->loadSkyBox("../Image/SkyBoxTexture/h_skyboxpack2_lagoon/",
						 "lagoon_north.bmp",
						 "lagoon_south.bmp",
						 "lagoon_west.bmp",
						 "lagoon_east.bmp",
						 "lagoon_up.bmp",
						 "lagoon_down.bmp");
#endif


	//---- �G�̏�����
	for( s32 j=0; j<ENEMY_SQUAD_MAX; ++j ) {
		// �U���^�C�v
		for( s32 i=0; i<ENEMY_ATTACKER; ++i )
		{
			addEnemy(new EnemyAttacker());
		}

		// �h��^�C�v
		for( s32 i=0; i<ENEMY_DEFFENDER; ++i )
		{
			addEnemy(new EnemyDeffender());
		}
	}


	//---- ����������

	// �U���^�C�v
	for( s32 i=0; i<ALLY_ATTACKER; ++i )
	{
		addAlly(new AllyAttacker());
	}
	// �h��^�C�v
	for( s32 i=0; i<ALLY_DEFFENDER; ++i )
	{
		addAlly(new AllyDeffender());
	}
	// �񕜃^�C�v
	for( s32 i=0; i<ALLY_HEALER; ++i )
	{
		addAlly(new AllyHealer());
	}

	//---- �Q�[���I�[�o�[�N���A�����ǂݍ���
	SAFE_NEW(_pGameClearWord);
	_pGameClearWord->loadAndTexImage2D("../Image/gameClear.png");
	SAFE_NEW(_pGameOverWord);
	_pGameOverWord->loadAndTexImage2D("../Image/gameOver.png");

	_pMissionWord[0]->loadAndTexImage2D("../Image/mission1.png");
	_pMissionWord[1]->loadAndTexImage2D("../Image/mission2.png");

	SAFE_NEW(_pXButtonImage);
	_pXButtonImage->loadAndTexImage2D("../Image/XButton.png", true);
	SAFE_NEW(_pXButtonUI   );
	_pXButtonUI->setTexture(_pXButtonImage);
	_pXButtonUI->setSize(Vector3(26.0f, 26.0f, 0.0f));
	_pXButtonUI->setScale(Vector3(0.3f, 0.3f, 0.0f));

	SAFE_NEW(_pMap);

	//---- �p�t�H�[�}���X�֘A������

	// �p�t�H�[�}���X�̏�����
	SAFE_NEW(_pUpdatePerformance);
	SAFE_NEW(_pRenderPerformance);
	// �X�V�͐�
	_pUpdatePerformance->setColor(Color(255, 0, 0));
	// �`��͉�
	_pRenderPerformance->setColor(Color(255, 255, 0));

	// ���C���ɐݒ�
	_pUpdatePerformance->setMainPerformance();
	_pRenderPerformance->setMainPerformance();

	// �f���^�^�C���p
	Global::deltaTime = 0;
	
	
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
SceneGame::~SceneGame()
{

	// �t�H���g�̉��
	SAFE_DELETE(_pFont);
	
	// �v���C���[�̉��
	SAFE_DELETE(_pPlayer);

	// �Q�[���N���A�I�[�o�[�������
	SAFE_DELETE(_pGameClearWord);
	SAFE_DELETE(_pGameOverWord);

	SAFE_DELETE(_pXButtonImage);
	SAFE_DELETE(_pXButtonUI);


	for( s32 i=0; i<ENEMY_COUNT_MAX; i++ ){
		SAFE_DELETE(_pEnemy[i]);
	}

	for( s32 i=0; i<ALLY_COUNT_MAX; i++ ){
		SAFE_DELETE(_pAlly[i]);
	}

	for( s32 i=0; i<2; i++ )
	{
		SAFE_DELETE(_pMissionWord[i]);
	}

	// �}�b�v�̉��
	SAFE_DELETE(_pMap);

	// �����蔻��N���X�̉��
	GmSystemCollision()->deleteObject();
}
//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool SceneGame::init()
{
	glEnable(GL_DEPTH_TEST);	// �y�o�b�t�@��L���ɂ���
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDepthMask(GL_TRUE);

	// �t�H���g�̏�����
	_pFont->Initialize(L"MS����", 24);
	// �v���C���[������
	_pPlayer->Initialize();

	// �J�����̏�����
	Vector3 rot = _pPlayer->getRotation();
	rot._x = TO_RADIAN(rot._x);
	rot._y = TO_RADIAN(rot._y + 180.0f);
	rot._z = TO_RADIAN(rot._z);

	_pCamera->init(_pPlayer->getPosition(), rot);

	// �J�X�P�[�h�V���h�E��Ortho�s�񏉊���
	ICasCaded()->initOrthoMatrix();

	// �ЂƂO�̉��y��~
	IBGMManager()->stopPrevMusic();
	// �^�C�g����BGM�Đ�
	IBGMManager()->playMusic(BGMSoundPlayerManager::BGM_GAME);


	// �G�̏�����
	for( s32 i=0; i<ENEMY_SQUAD_MAX; ++i ) {
		_pEnemyLeader[i]->Initialize();
	}

	for( s32 i=0; i<ENEMY_COUNT_MAX; i++ ){
//		_pEnemy[i]->setLeaderPos(_pEnemyLeader->getPosition());
		_pEnemy[i]->Initialize();
	}
	// �����̏�����
	for( s32 i=0; i<ALLY_COUNT_MAX; i++ ){
//		_pAlly[i]->setLeaderPos(_pPlayer->getPosition());
		_pAlly[i]->Initialize();
	}
	// �}�b�v�̏�����
	_pMap->Initialize();

	_gameClear	 = false;
	_gameOver	 = false;
	_isExit		 = false;

	// ��ɋ߂��t���OOFF
	_pPlayer->setNearCastle(false);

	_waitGoTitle = 150.0f;

	_allyRevivalCountMax  = 30;
	_enemyRevivalCountMax = 20;
	

	loopTimer.Reset();

	glFrontFace(GL_CW);

	return true;
}

//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void SceneGame::update()
{
	
	// ���[�v�^�C�}�[
	loopTimer.EndTimer();
	// �f���^�^�C���擾
	Global::deltaTime = (f32)( loopTimer.getTotalTime() * 0.0625f );

	// �n�ʂ����蔲����\��������̂ŁA��������
	if( Global::deltaTime >= 10.0f ){
		Global::deltaTime = 10.0f;
	}
	loopTimer.StartTimer();

	
	
	// �^�C�}�[�̊J�n
	updateTimer.StartTimer();

	_pUpdatePerformance->StartPerformance();

	s32 state = 0;

	// �Q�[���X�V
	{
		GM_PROFILE("�Q�[���X�V", Color(255,0,0,255));
		update(state);
	}

	// �Q�[���N���A���I�[�o�[���̂ݖ߂��
	if( _gameClear || _gameOver ) {
		_waitGoTitle -= 1.0f * Global::deltaTime;
		if( _waitGoTitle <= 0 )
		{
			GmScene()->jump(SceneTitle::TYPE);
		}
	}

	// �X�e�[�g�ɂ���ăQ�[���I������
	if( state == 1 && !_gameOver) {
		_gameClear = true;
	}
	// �Q�[���N���A���ĂȂ����A�邪�A�N�e�B�u��ԂȂ�
	if( state == 2 && !_gameClear && _pMap->getCastle()->isActive()) {
		_gameOver  = true;
	}

	_pUpdatePerformance->EndPerformance();

	// �^�C�}�[�̏I��
	updateTimer.EndTimer();


	
	
	_pRenderPerformance->StartPerformance();
	
	// �^�C�}�[�̊J�n
	renderTimer.StartTimer();

	// �Q�[���`��
	{
		GM_PROFILE("�Q�[���X�V", Color(0,0,255,255));
		render();
	}

	if (GetKeyState(VK_ESCAPE) & 0x80){
		_isExit = true;
	}

	// �^�C�}�[�̏I��
	renderTimer.EndTimer();
	_pRenderPerformance->EndPerformance();

}

//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void SceneGame::update(s32& state)
{
	_pMouse->Update();

	//-------------------------------------------------------------
	// �s��̏����ݒ�
	//-------------------------------------------------------------
	//---- ���e�s��̏�����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();				// ������(���Z�b�g)

	//���E�p�A�A�X�y�N�g��(�E�B���h�E�̕�/����), �`�悷��͈�(�ł��߂�����,�ł���������)
	glLoadMatrixf((GLfloat*)&GmCameraMan()->getCurrentCamera()->getProjMatrix());
	//---- ���[���h/�r���[�s��̏�����
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();				// ������(���Z�b�g)

	// �v���C���[�̍X�V
	_pPlayer->Update();
	
	// �����X�V
	// ��ԋ߂��G��_��
	for( s32 i=0; i<ALLY_COUNT_MAX; i++ )
	{
		// �ڕW���W(�����̕����ł̕W���ʒu���擾)
		_pAlly[i]->Update();
	}

	// �G�X�V
	for( s32 i=0; i<ENEMY_SQUAD_MAX; i++ ) { 
		_pEnemyLeader[i]->Update();
	}

	// ��ԋ߂��G��_��
	static Vector3	 diffencePos = Vector3(0,0,0);
	for( s32 i=0; i<ENEMY_COUNT_MAX; i++ )
	{
//		Vector3 leaderPos = _pEnemySquad->getMyPostion(_pEnemy[i]);
//		_pEnemy[i]->setLeaderPos(leaderPos);
		_pEnemy[i]->Update();
	}

	// ���}�b�v�̃A�b�v�f�[�g
	_pMap->Update();


	GmCollisionMan()->Update();


	// ��ɋ߂��t���O�X�V
	bool isNear = ICastleCol()->isNearCastleObject(_pPlayer->getColAABB(), _pPlayer->getPosition());
	_pPlayer->setNearCastle( isNear );


	// �Q�[���I������
	if( _pPlayer->getGameOver() )
	{
		state = 2;
	}else if( _pMap->getCastle()->isCrash() ){
		// �邪��ꂽ��
		state = 1;
	}



	// �v���C���[���ꎞ�ۑ�
	static Vector3  PlayerPos;
	static f32		PlayerRot;
	static Vector3  PlayerAttackPos;
	static f32		PlayerAttackRad;
	
	// �����_�Ƃ̋�����ݒ�
	_pCamera->changeLookAtLength(_pMouse->getChangeWheel() * 3.0f);
	// �J�����̍X�V
	_pCamera->update(_pControl[0], _pPlayer->getPosition(), _pMouse->getOffset());


	Matrix	viewMatrix = _pCamera->getViewMatrix();
	
	GmScene()->setViewMatrix(viewMatrix);

	// ���C�g�̍X�V
	_pLight->update();
	Matrix identity = Matrix::IDENTITY;
	Vector3 lightDir = _pLight->getLightDir();
	ICasCaded()->update(identity);

	debugTimer.StartTimer();

	// �p�[�e�B�N���X�V
	IParticleMan()->update();

	// �G��������
	revival();

	debugTimer.EndTimer();

}

//-----------------------------------------------------------------------------
//! 3D�`��
//-----------------------------------------------------------------------------
void SceneGame::draw3D()
{
	GLfloat white[]	 = { 1.0f, 1.0f, 1.0f, 1.0f };

	Vector3 CamPos = _pCamera->getPosition();


	// �}�b�v�̕`��
	_pMap->Render(_isShadow);

	// ���܂ł̐F��������
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);

	// �����`��
	for( s32 i=0; i<ALLY_COUNT_MAX; i++ )
	{
		_pAlly[i]->Render(_isShadow);
	}

	// �G�`��
	for( s32 i=0; i<ENEMY_COUNT_MAX; i++ )
	{
		_pEnemy[i]->Render(_isShadow);
	}

	// �v���C���[�̕`��
	_pPlayer->Render(_isShadow);
	
	// �G���[�_�[�`��
	for( s32 i=0; i<ENEMY_SQUAD_MAX; i++ ) { 
		_pEnemyLeader[i]->Render(_isShadow);
	}

	// ���܂ł̐F��������
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);

}

//-----------------------------------------------------------------------------
//! 2D�`��
//-----------------------------------------------------------------------------
void SceneGame::draw2D()
{
	// �v���C���[�̃X�e�[�^�X�`��
	_pPlayer->StatusRender();
	
	gameEndRender();

	missionRender();
}

//-----------------------------------------------------------------------------
//! �~�b�V���������`��
//-----------------------------------------------------------------------------
void SceneGame::missionRender()
{
	glDisable(GL_ALPHA_TEST);
	setBlendMode(BM_BLEND_ALPHA);
	// �`�悷��~�b�V�����ԍ�
	static u8   popMissionNum = 0;
	// �J�E���g
	static s32	count = 0;
	static f32  w,h,x,y;
	// �ړ��J�E���g
	static s32	moveCount = 0;
	// �ړ��J�E���g�ő�l
	static const s32		moveCountMax = 10;
	// �傫���Ƃ��̕`��ʒu
	static const Vector3	popPos  = Vector3( 800.0f, 400.0f, 0.0f);
	// �������Ƃ��̕`��ʒu
	static const Vector3	sidePos = Vector3(1500.0f, 100.0f, 0.0f);

	
	// ��A��ǂ̃C���X�^���X�擾
	CastleWall* wall	= _pMap->getWall();
	Castle*		castle  = _pMap->getCastle();
	Texture*	drawTex = nullptr;
	// ��A��ǂ̏�Ԃ���~�b�V���������ύX
	if( !wall->isCrash() ){
		if( popMissionNum == 0 ){
			count	  = 100;
			moveCount = 0;
		}
		popMissionNum = 1;
		drawTex = _pMissionWord[0];
	}else if( !castle->isCrash()) {
		if( popMissionNum == 1 ){
			count	  = 100;
			moveCount = 0;
		}
		popMissionNum = 0;
		drawTex = _pMissionWord[1];
	}

	// �ő�𑜓x�Ƃ̔䗦
	Size<f32> resScale = GmWindow()->getRateResMax();

	// 0�ȏ�Ȃ�
	if( count > 0 ){
		// �傫����ʂ̐^�񒆂ɕ`��
		count = (s32)( ceil( (f32)(count) - ( 1.0f * Global::deltaTime ) ) );
		if(drawTex != nullptr) {
			w = 389.0f	  * resScale._w;
			h = 83.0f	  * resScale._h;
			x = popPos._x * resScale._w;
			y = popPos._y * resScale._h;
			GmRender()->putTexture(drawTex->getTextureHandle(), x, y, Size<s32>( (s32)w, (s32)h ) );
		}
	}else{
		// �ȉ��Ȃ�k�����Ȃ���E��Ɉړ�
		if( moveCount < moveCountMax ){
			moveCount++;
		}
		if(drawTex != nullptr) {
			w = LinearInterpolation(	389.0f,		140.0f, (f32)moveCount, (f32)moveCountMax) * resScale._w;
			h = LinearInterpolation(	 83.0f,		 30.0f, (f32)moveCount, (f32)moveCountMax) * resScale._h;
			x = LinearInterpolation( popPos._x, sidePos._x, (f32)moveCount, (f32)moveCountMax) * resScale._w;
			y = LinearInterpolation( popPos._y, sidePos._y, (f32)moveCount, (f32)moveCountMax) * resScale._h;
			GmRender()->putTexture(drawTex->getTextureHandle(), x, y, Size<s32>( (s32)w, (s32)h ) );
		}
	}
	setBlendMode(BM_NONE);
	glEnable(GL_ALPHA_TEST);
}
//-----------------------------------------------------------------------------
//! �Q�[���N���A�I�[�o�[�����`��
//-----------------------------------------------------------------------------
void SceneGame::gameEndRender()
{
	// �t���O�������ĂȂ������珈�����Ȃ�
	if(!_gameClear && !_gameOver) return;

	setBlendMode(BM_BLEND_ALPHA);

	// �ő�𑜓x�Ƃ̔䗦
	Size<f32> resScale = GmWindow()->getRateResMax();
	// �t���O�ɂ���ĕ����`��
	if( _gameClear ) {
		
		GmRender()->putTexture(_pGameClearWord->getTextureHandle(),
								500 * resScale._w, 400 * resScale._h,
								Size<s32>(886 * resScale._w, 127 * resScale._h));
	}else if( _gameOver ) {
		GmRender()->putTexture(_pGameOverWord->getTextureHandle(),
								550 * resScale._w, 400 * resScale._h,
								Size<s32>( 738 * resScale._w, 106 * resScale._h ) );
	}

	setBlendMode(BM_NONE);
}


//-----------------------------------------------------------------------------
//! �����֐�
//-----------------------------------------------------------------------------
void SceneGame::revival()
{
	s32	nowTime = timeGetTime();

	bool isLeaderDead = false; 

	//--- �G����
	static s32	oldEneRevivalTime = 0;
	// ��莞�Ԃ�������
	if( nowTime - oldEneRevivalTime > 10000 ) {

		for( s32 i=0; i<ENEMY_SQUAD_MAX; i++ ) {
			// ���[�_�[�����S���Ă�����
			if( _pEnemyLeader[i]->isDead() ) {
				revivalEnemy(_pEnemyLeader[i]);
				oldEneRevivalTime = nowTime;
				isLeaderDead = true;
			}
		}

		// ���S���Ă��Ȃ����
		if( !isLeaderDead ) {
			
			for(s32 i=0; i<ENEMY_COUNT_MAX; ++i) {
				if( _pEnemy[i]->isDead() ) {
					revivalEnemy(_pEnemy[i]);
					oldEneRevivalTime = nowTime;
					break;
				}
			}
		}
	}

	

	//---- ��������
	static s32	oldAllyRevivalTime = 0;
	// ��莞�Ԃ�������
	if( nowTime - oldAllyRevivalTime > 5000 ) {
		for(s32 i=0; i<ALLY_COUNT_MAX; ++i) {
			// ���S�t���O�������Ă�����
			if( _pAlly[i]->isDead() ) {
				revivalAlly(_pAlly[i]);
				// �^�C���X�V
				oldAllyRevivalTime = nowTime;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//! �G�����֐�
//-----------------------------------------------------------------------------
void SceneGame::revivalEnemy(Man*	pEnemy)
{
	if(_enemyRevivalCountMax <= 0)	return;
	// ����������
	pEnemy->Initialize();
	// �����񐔂����炷
	_enemyRevivalCountMax--;
}

//-----------------------------------------------------------------------------
//! ���������֐�
//-----------------------------------------------------------------------------
void SceneGame::revivalAlly(Man* pAlly)
{
	if(_allyRevivalCountMax <= 0)	return;
	// ����������
	pAlly->Initialize();
	// �����񐔂����炷
	_allyRevivalCountMax--;
}

//-----------------------------------------------------------------------------
//! �X�e�[�^�X�Ȃǃt�B���^�[�̉e�����Ȃ����̂̕`��
//-----------------------------------------------------------------------------
void SceneGame::drawNoFilter()
{
	// �����Ƀf�o�b�O�`��
	//glDisable(GL_LIGHTING);

	
		

if( Global::debugMode ) {
	

	/*
	float length = 100.0f;

	Line axisX;
	axisX._position[0] = Vector3(0,0,0);
	axisX._position[1] = Vector3(length,0,0);
	glColor4f(1,0,0,1);
	axisX.DebugRender();

	Line axisY;
	axisY._position[0] = Vector3(0,0,0);
	axisY._position[1] = Vector3(0,length,0);
	glColor4f(0,1,0,1);
	axisY.DebugRender();

	Line axisZ;
	axisZ._position[0] = Vector3(0,0,0);
	axisZ._position[1] = Vector3(0,0,length);
	glColor4f(0,0,1,1);
	axisZ.DebugRender();

	_pLight->DebugRender();

	glColor4f(1,1,1,1);

	*/
	
	_pPlayer->debugRender();

	for( s32 i=0; i<ALLY_COUNT_MAX; ++i )
	{
		_pAlly[i]->debugRender();
	}

	for( s32 i=0; i<ENEMY_COUNT_MAX; ++i )
	{
		_pEnemy[i]->debugRender();
	}
	
	for( s32 i=0; i<ENEMY_SQUAD_MAX; ++i ) {
		_pEnemyLeader[i]->debugRender();
	}		
	_pMap->debugRender();

	ICasCaded()->debugShadow();

	_pCamera->getViewFrustum().debugRender(Color(255, 0, 0));

	// �f�o�b�O�^�C�}�[�`��
	drawDebugTimer();
}

	//-------------------------------------------------------------
	// ��������`��(2D)
	//-------------------------------------------------------------
	draw2D();

	/*glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);*/
}

//-----------------------------------------------------------------------------
//! �^�C�}�[�̕`��
//-----------------------------------------------------------------------------
void SceneGame::drawDebugTimer()
{

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
		Size<s32> windowSize = GmWindow()->getResolution();
		glOrtho(0, windowSize._w, windowSize._h, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

			// �^�C�}�[�̕`��
			char str[255];

			// �X�V���Ԃ̕`��
			sprintf_s( str, "Loop Time is " );
			loopTimer.DrawTotalTime(Vector3( 600.0f,  50.0f, 0.0f ), str );
			// �X�V���Ԃ̕`��
			sprintf_s( str, "Update Time is " );
			updateTimer.DrawTotalTime(Vector3( 600.0f, 100.0f, 0.0f ), str );
			// �`�掞�Ԃ̕`��
			sprintf_s( str, "Render Time is " );
			renderTimer.DrawTotalTime(Vector3( 600.0f, 150.0f, 0.0f ), str );

			sprintf_s( str, "Debug Time is " );
			debugTimer.DrawTotalTime(Vector3( 600.0f, 200.0f, 0.0f ), str );
		
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

}

//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void SceneGame::render()
{	

	//---- �s����(���f����)�̕`��
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	
	//-------------------------------------------------------------
	// ��������`��(3D)
	//-------------------------------------------------------------
#if 1			
	

	
	//---- �V���h�E�`��
	GmShader()->changeShader(SystemShader::SHADER_SHADOW);

	_isShadow = true;

	s32 splitCount = ICasCaded()->getSplitCount();

	
	// ���������`��
	for( s32 i=0; i<splitCount; ++i )
	{
		ICasCaded()->begin(_pLight);
		
		draw3D();
		
	}
	ICasCaded()->end();
	
	//---- �X�J�C�{�b�N�X�̕`��
	GmShader()->changeShader(SystemShader::SHADER_DRAWCUBEMAP);
	_pSkyBox->render();


	//---- �ʏ�`��
	GmShader()->changeShader(SystemShader::SHADER_TEXTURE);

	_isShadow = false;
	//---- �V�F�[�_�v���O�����ɏ��]��
	// ���C�g�ݒ�
	Vector3 sendLight = _pLight->getPosition();
	GLfloat lightPos[] = { sendLight._x, sendLight._y, sendLight._z };
	GmShader()->setVertex3fv("lightPos", lightPos);
	// �J�������W
	Vector3 cameraPos = _pCamera->getPosition();
	GmShader()->setVertex3fv("eyePos", (GLfloat*)&cameraPos);
	
	draw3D();

	//---- �}�g���b�N�X�]��
	CameraBase* cam   = GmCameraMan()->getCurrentCamera();
	//Matrix matWorld   = Matrix::translate(position);
	Matrix matWorld   = Matrix::IDENTITY;
	Matrix matView	  = cam->getViewMatrix();
	Matrix matProj	  = cam->getProjMatrix();

	Matrix sendMatrix[] =
	{
		matWorld,
		matView,
		matProj,
	};

	GmShader()->setUniformMatrixBuffer(sendMatrix, sizeof(sendMatrix));

	//---- �p�[�e�B�N���`��
	GmShader()->changeShader(SystemShader::SHADER_STANDARD);
	{
		GmRender()->saveBackBuffer("�p�[�e�B�N���`��O");
		IParticleMan()->render();
		GmRender()->saveBackBuffer("�p�[�e�B�N���`���");
	}
	// �����̃X�e�[�^�X�̕`��
	{
		
		for( s32 i=0; i<ALLY_COUNT_MAX; i++ )
		{
			_pAlly[i]->StatusRender();
		}
	}
	// �G�̃X�e�[�^�X�̕`��
	{
		
		for( s32 i=0; i<ENEMY_COUNT_MAX; i++ )
		{
			_pEnemy[i]->StatusRender();
		}

		for( s32 i=0; i<ENEMY_SQUAD_MAX; i++ ) {
			_pEnemyLeader[i]->StatusRender();
		}
	}
	GmShader()->beginPrevShader();	// �������ł��`�悳���(NVIDIA��Matrix������ɏ��������Ă����H)


	// X�{�^���摜�\��
	if(_pPlayer->getNearCastle()){
		Vector3 playerPos = _pPlayer->getPosition();
		playerPos._y += 50.0f;
		_pXButtonUI->setPosition(playerPos);
		_pXButtonUI->draw(Color::WHITE);
	}

	GmShader()->beginPrevShader();

	//---- �Œ�p�C�v���C���ł̕`��
	// �V�F�[�_�̏I��
	GmShader()->endShader();


#endif 

	_pPlayer->glutRender();

	//---- �U���̕`��
	for( s32 i=0; i<ALLY_COUNT_MAX; i++ )
	{
		_pAlly[i]->glutRender();
	}

	for( s32 i=0; i<ENEMY_COUNT_MAX; i++ )
	{
		_pEnemy[i]->glutRender();
	}


}

//=============================================================================
//	END OF FILE
//=============================================================================