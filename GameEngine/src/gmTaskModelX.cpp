//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskModelX.cpp
//!	@brief	3D���f��(X�t�@�C���`��)
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
//!	����`��
//!	@param	[in]	position	���S�ʒu
//!	@param	[in]	extent		�͈́i���E�����E���s���j
//!	@param	[in]	color		�`��F
//-----------------------------------------------------------------------------
//static void drawBox(const Vector3& position, const Vector3& extent, D3DCOLOR color)
static void drawBox(const Vector3& position, const Vector3& extent, Color color)
{
	static Vertex	v[4];

	//IDirect3DDevice9*	pD3DDevice = GmRender()->getD3DDevice();

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY  );
	glEnableClientState( GL_NORMAL_ARRAY );

	//---- �V��
	{
		v[0]._position = position + extent * Vector3(-1, +1, -1);
		v[0]._color    = color;
		v[0]._normal   = Vector3(0.0f, 1.0f, 0.0f);
	
		v[1]._position = position + extent * Vector3(+1, +1, -1);
		v[1]._color    = color;
		v[1]._normal   = Vector3(0.0f, 1.0f, 0.0f);

		v[2]._position = position + extent * Vector3(-1, +1, +1);
		v[2]._color    = color;
		v[2]._normal   = Vector3(0.0f, 1.0f, 0.0f);

		v[3]._position = position + extent * Vector3(+1, +1, +1);
		v[3]._color    = color;
		v[3]._normal   = Vector3(0.0f, 1.0f, 0.0f);

		//pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &v[0], sizeof(Vertex));
		
		
		glVertexPointer	 (3, GL_FLOAT		 , sizeof(Vertex), &v[0]._position);
		glColorPointer	 (4, GL_UNSIGNED_BYTE, sizeof(Vertex), &v[0]._color);
		//glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &v[0]._uv);
		glNormalPointer  (   GL_FLOAT		 , sizeof(Vertex), &v[0]._normal);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
	}


	//---- �E
	{
		v[0]._position = position + extent * Vector3(+1, +1, +1);
		v[0]._color    = color;
		v[0]._normal   = Vector3(1.0f, 0.0f, 0.0f);
	
		v[1]._position = position + extent * Vector3(+1, +1, -1);
		v[1]._color    = color;
		v[1]._normal   = Vector3(1.0f, 0.0f, 0.0f);

		v[2]._position = position + extent * Vector3(+1, -1, +1);
		v[2]._color    = color;
		v[2]._normal   = Vector3(1.0f, 0.0f, 0.0f);

		v[3]._position = position + extent * Vector3(+1, -1, -1);
		v[3]._color    = color;
		v[3]._normal   = Vector3(1.0f, 0.0f, 0.0f);
			
		//pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &v[0], sizeof(Vertex));

		glVertexPointer	 (3, GL_FLOAT		 , sizeof(Vertex), &v[0]._position);
		glColorPointer	 (4, GL_UNSIGNED_BYTE, sizeof(Vertex), &v[0]._color);
		//glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &v[0]._uv);
		glNormalPointer  (   GL_FLOAT		 , sizeof(Vertex), &v[0]._normal);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	
	//---- ��
	{
		v[0]._position = position + extent * Vector3(-1, +1, -1);
		v[0]._color    = color;
		v[0]._normal   = Vector3(-1.0f, 0.0f, 0.0f);
	
		v[1]._position = position + extent * Vector3(-1, +1, +1);
		v[1]._color    = color;
		v[1]._normal   = Vector3(-1.0f, 0.0f, 0.0f);

		v[2]._position = position + extent * Vector3(-1, -1, -1);
		v[2]._color    = color;
		v[2]._normal   = Vector3(-1.0f, 0.0f, 0.0f);

		v[3]._position = position + extent * Vector3(-1, -1, +1);
		v[3]._color    = color;
		v[3]._normal   = Vector3(-1.0f, 0.0f, 0.0f);

		//pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &v[0], sizeof(Vertex));
		
		glVertexPointer	 (3, GL_FLOAT		 , sizeof(Vertex), &v[0]._position);
		glColorPointer	 (4, GL_UNSIGNED_BYTE, sizeof(Vertex), &v[0]._color);
		//glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &v[0]._uv);
		glNormalPointer  (   GL_FLOAT		 , sizeof(Vertex), &v[0]._normal);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}


	//---- ��
	{
		v[0]._position = position + extent * Vector3(+1, +1, -1);
		v[0]._color    = color;
		v[0]._normal   = Vector3(0.0f, 0.0f, -1.0f);
	
		v[1]._position = position + extent * Vector3(-1, +1, -1);
		v[1]._color    = color;
		v[1]._normal   = Vector3(0.0f, 0.0f, -1.0f);

		v[2]._position = position + extent * Vector3(+1, -1, -1);
		v[2]._color    = color;
		v[2]._normal   = Vector3(0.0f, 0.0f, -1.0f);

		v[3]._position = position + extent * Vector3(-1, -1, -1);
		v[3]._color    = color;
		v[3]._normal   = Vector3(0.0f, 0.0f, -1.0f);

		//pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &v[0], sizeof(Vertex));

		glVertexPointer	 (3, GL_FLOAT		 , sizeof(Vertex), &v[0]._position);
		glColorPointer	 (4, GL_UNSIGNED_BYTE, sizeof(Vertex), &v[0]._color);
		//glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &v[0]._uv);
		glNormalPointer  (   GL_FLOAT		 , sizeof(Vertex), &v[0]._normal);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}


	//---- ��
	{
		v[0]._position = position + extent * Vector3(-1, -1, -1);
		v[0]._color    = color;
		v[0]._normal   = Vector3(0.0f, -1.0f, 0.0f);
	
		v[1]._position = position + extent * Vector3(+1, -1, -1);
		v[1]._color    = color;
		v[1]._normal   = Vector3(0.0f, -1.0f, 0.0f);

		v[2]._position = position + extent * Vector3(-1, -1, +1);
		v[2]._color    = color;
		v[2]._normal   = Vector3(0.0f, -1.0f, 0.0f);

		v[3]._position = position + extent * Vector3(+1, -1, +1);
		v[3]._color    = color;
		v[3]._normal   = Vector3(0.0f, -1.0f, 0.0f);

		//pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &v[0], sizeof(Vertex));

		glVertexPointer	 (3, GL_FLOAT		 , sizeof(Vertex), &v[0]._position);
		glColorPointer	 (4, GL_UNSIGNED_BYTE, sizeof(Vertex), &v[0]._color);
		//glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &v[0]._uv);
		glNormalPointer  (   GL_FLOAT		 , sizeof(Vertex), &v[0]._normal);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	//---- ��O
	{
		v[0]._position = position + extent * Vector3(-1, +1, +1);
		v[0]._color    = color;
		v[0]._normal   = Vector3(0.0f, 0.0f,  1.0f);
	
		v[1]._position = position + extent * Vector3(+1, +1, +1);
		v[1]._color    = color;
		v[1]._normal   = Vector3(0.0f, 0.0f,  1.0f);

		v[2]._position = position + extent * Vector3(-1, -1, +1);
		v[2]._color    = color;
		v[2]._normal   = Vector3(0.0f, 0.0f,  1.0f);

		v[3]._position = position + extent * Vector3(+1, -1, +1);
		v[3]._color    = color;
		v[3]._normal   = Vector3(0.0f, 0.0f,  1.0f);

		//pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &v[0], sizeof(Vertex));

		glVertexPointer	 (3, GL_FLOAT		 , sizeof(Vertex), &v[0]._position);
		glColorPointer	 (4, GL_UNSIGNED_BYTE, sizeof(Vertex), &v[0]._color);
		//glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &v[0]._uv);
		glNormalPointer  (   GL_FLOAT		 , sizeof(Vertex), &v[0]._normal);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}


	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY  );
	glDisableClientState( GL_NORMAL_ARRAY );
}

//-----------------------------------------------------------------------------
//!	���C����`��
//-----------------------------------------------------------------------------
//static void drawLineG(const Vector3& p1, const Vector3& p2, D3DCOLOR c1, D3DCOLOR c2)
static void drawLineG(const Vector3& p1, const Vector3& p2, Color c1, Color c2)
{
	static Vertex	v[2];
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY  );
	{
		v[0]._position = p1;
		v[0]._color    = c1;
	
		v[1]._position = p2;
		v[1]._color    = c2;

		glVertexPointer	 (3, GL_FLOAT		 , sizeof(Vertex), &v[0]._position);
		glColorPointer	 (4, GL_UNSIGNED_BYTE, sizeof(Vertex), &v[0]._color);
		glDrawArrays(GL_LINES, 0, 2);
	}
	glDisableClientState( GL_COLOR_ARRAY  );
	glDisableClientState( GL_VERTEX_ARRAY );
	
}


//=============================================================================
//!	TaskModelX	���f��
//=============================================================================

//-----------------------------------------------------------------------------
// �ÓI�ϐ��̐錾
//-----------------------------------------------------------------------------
GLuint TaskModelX::_depthTexHandle;				//!< �f�v�X�e�N�X�`���̃n���h��
Matrix TaskModelX::_biasMatrix;					//!< �V���h�E�̃r���[�s��
Matrix TaskModelX::_shadowView[_splitCount];	//!< �V���h�E�̃r���[�s��
Matrix TaskModelX::_shadowProj[_splitCount];	//!< �V���h�E�̃v���W�F�N�V�����s��


//-----------------------------------------------------------------------------
//	�R���X�g���N�^
//-----------------------------------------------------------------------------
TaskModelX::TaskModelX(void)
: _scale			  (1.0f)					// �X�P�[���l
, _pModel			  (nullptr)					// ���f��
, _pVAO				  (nullptr)					// ���_���̓��C�A�E�g
, _rotateMatrix		  (Matrix::IDENTITY)		// ��]�s��
, _worldMatrix        (Matrix::IDENTITY)		// �p���s��
, _isAnimationRepeat  (false)					// �A�j���[�V���������s�[�g�Đ����邩�ǂ���
, _animationPlayType  (ANIMATION_PLAY_DEFAULT)	// �A�j���[�V�����̑���
, _animationNumber    (-1)						// �A�j���[�V�����ԍ�
, _animationTime      (0.0f)					// �A�j���[�V�����̍Đ����̎���
, _animationSpeed     (1.0f)					// �A�j���[�V�����Đ����x
, _animationBlendRatio(1.0f)					// �A�j���[�V�����u�����h�W��
, _animationRate	  (0.0f)					// �A�j���[�V�����Đ��䗦
, _shadowEnable		  (false)					// �V���h�E�}�b�s���O�L�����t���O

{
}

//-----------------------------------------------------------------------------
//	�f�X�g���N�^
//-----------------------------------------------------------------------------
TaskModelX::~TaskModelX(void)
{
	SAFE_DELETE(_pVAO);
}

//-----------------------------------------------------------------------------
//	������
//-----------------------------------------------------------------------------
bool TaskModelX::init(void)
{
	if(_pModel->_animations.size() != 0) {
		setAnimation(0);
		update();
	}

	calcMinMax();

	return true;
}

//-----------------------------------------------------------------------------
//	����񓯊����s
//-----------------------------------------------------------------------------
void TaskModelX::asyncUpdate(void)
{
}

//-----------------------------------------------------------------------------
//	���񓯊����s
//-----------------------------------------------------------------------------
void TaskModelX::update(void)
{
	AssetModelX*			pModel = getModel();
	if( _animationNumber != -1 ) 
	{

		AssetModelX::Animation*	pAnimation    = pModel->_animations[_animationNumber];
		f32						animationTime = _animationTime;

		//------------------------------------------------------------
		// �A�j���[�V�������X�V
		//------------------------------------------------------------
		_animationName = pAnimation->getName();

		//------------------------------------------------------------
		// ���Ԃ�i�߂�
		//------------------------------------------------------------
		_animationTime += 3600.0f/60.0f * (_animationSpeed * Global::deltaTime);

		//------------------------------------------------------------
		// �Đ��䗦�v�Z
		//------------------------------------------------------------
		_animationRate = LinearInterpolation(0.0f, 1.0f, _animationTime, (f32)pAnimation->getTotalTime());

		//------------------------------------------------------------
		// ���Đ����Ԃ𒴂����ꍇ�̐U�镑���i���s�[�g�Đ��E�P�̍Đ��j
		//------------------------------------------------------------
		//if( _animationTime > (f32)pAnimation->getTotalTime() ) {
		if( _animationRate >= 1.0f ) {
		
			if( _isAnimationRepeat ) {	//---- �J��Ԃ����߂Ɋ����߂�
				_animationTime -= (f32)pAnimation->getTotalTime();
			}
			else {						//---- �I�[�ŃX�g�b�v
				_animationTime  = (f32)pAnimation->getTotalTime();
			}
			// �A�j���[�V�����I���t���O�𗧂Ă�
			_animationNumber = -1;
		}

		//------------------------------------------------------------
		// �A�j���[�V�������m�̕�Ԕ䗦
		//------------------------------------------------------------
		f32			blendRatio = _animationBlendRatio;

		//---- �A�j���[�V�������m�̕�Ԕ䗦��i�߂� 0.0f��1.0f
		f32			blendFrame = 10;
		blendRatio += 1.0f / blendFrame;
		if( blendRatio > 1.0f ) {
			blendRatio = 1.0f;
		}
		_animationBlendRatio = blendRatio;

		//------------------------------------------------------------
		// �S�ẴA�j���[�V�����֐߂̍s����X�V
		//------------------------------------------------------------
		//for( s32 bone=0; bone<pAnimation->_bone.getCount(); bone++ ) {
		for( u32 bone=0; bone<pAnimation->_bone.size(); bone++ ) {
			AssetModelX::Animation::Bone*	pBone      = pAnimation->_bone[bone];
			s32								jointIndex = pBone->_jointIndex;

			//---- �֐߂Ƀ}�b�s���O����Ă��Ȃ��{�[���͖���
			if( jointIndex == -1 ) continue;

			TaskModelX::Joint&		joint = _joints[jointIndex];

			//-----------------------------------------------------------
			//	��Ԃ��ꂽ�A�j���[�V�����f�[�^���擾
			//-----------------------------------------------------------
			//---- �L�[�t���[���̊Ԃ���
			Quaternion	rotation = pBone->getRotation(animationTime);
			Vector3		scale    = pBone->getScale   (animationTime);
			Vector3		position = pBone->getPosition(animationTime);

			//---- ����ɈȑO�̃A�j���[�V�����Ƃ̊Ԃ���
			Quaternion	blendedRotation = Quaternion::slerp(joint._blendRotation, rotation, blendRatio);
			Vector3		blendedPosition = (position - joint._blendPosition) * blendRatio + joint._blendPosition;
			Vector3		blendedScale    = (scale    - joint._blendScale   ) * blendRatio + joint._blendScale;
	
			//-----------------------------------------------------------
			// ���ꂼ��̕�Ԍ�̗v�f����s��𐶐�����
			//-----------------------------------------------------------
			Matrix		matRotate    = blendedRotation.toRotateMatrix();	// ��]
			Matrix		matTranslate = Matrix::translate(blendedPosition);	// �ړ�
			Matrix		matScale     = Matrix::scale    (blendedScale);		// �X�P�[��

			joint._rotation          = blendedRotation;
			joint._position          = blendedPosition;
			joint._scale             = blendedScale;
			joint._animationMatrix   = matScale * matRotate * matTranslate;
	
		}

	}else{
		_animationName = "none";
	}

	//-------------------------------------------------------------
	// �e�֐߂̃��[�J���s��ɔ��f
	//-------------------------------------------------------------
	_skinLocalMatrices[0] = Matrix::IDENTITY;

	if( _animationNumber != -1 || pModel->haveDynamicFrame() ){


		//for(s32 i=0; i<pModel->_frames.getCount(); i++ ) {
		for(u32 i=0; i<pModel->_frames.size(); i++ ) {
			const AssetModelX::Frame*	pFrame       = pModel->_frames[i];
			const AssetModelX::Frame*	pFrameParent = pFrame->getParent();

			const s32					jointIndex   = (s32)( pFrame->getJointIndex() );
			TaskModelX::Joint&			joint        = _joints[jointIndex ];

			//---- ���[�J���s��쐬
			// ���_�ł̕ϊ��s��́A�������g�̃A�j���[�V�����ƁA�e�̎p�������������́B
			joint._transformMatrix = joint._animationMatrix;

			if( pFrameParent ) {
				const s32			parentIndex = (s32)( pFrameParent->getJointIndex() );
				TaskModelX::Joint&	parent = _joints[parentIndex];

				joint._transformMatrix     = joint._transformMatrix * parent._transformMatrix;
				//joint._transformMatrix     = parent._transformMatrix;
			}

			// �֐߃��[�J���s�� = (���_�Ɉړ�������s��) �~ (�֐ߎp���s��)
			_skinLocalMatrices[jointIndex] = pFrame->getOffsetMatrix() * joint._transformMatrix * pFrame->getRotateMatrix();
		}
	}
}

//-----------------------------------------------------------------------------
//! ���b�V����`��
//!	@attention ���̊֐����͕`��p���_�t�H�[�}�b�g���ύX�ɂȂ��Ă��邽�߁A
//!	@attention AssetModelX::GpuVertex���g�p���ĕ`�悷��K�v������B
//-----------------------------------------------------------------------------
void TaskModelX::_renderMesh(AssetModelX::Mesh* pMesh)
{
	/*IDirect3DDevice9*	pD3DDevice = GmRender()->getD3DDevice();
	ID3DXEffect*		pD3DEffect = GmShader()->getEffect();

	D3DXHANDLE			hTexture   = pD3DEffect->GetParameterByName(NULL, "BaseTexture");


	s32		modelP0 = 0;
	s32		modelP1 = 1;

	if( pMesh->isSkined() ) {
		D3DXHANDLE			hTechnique = pD3DEffect->GetTechnique(0);
		D3DXTECHNIQUE_DESC	techniqueDesc;
		pD3DEffect->GetTechniqueDesc(hTechnique, &techniqueDesc);

		for( u32 pass=0; pass<techniqueDesc.Passes; pass++ ) {
			D3DXHANDLE			hPass      = pD3DEffect->GetPass(hTechnique, pass);

			D3DXPASS_DESC	passDesc;
			pD3DEffect->GetPassDesc(hPass, &passDesc);
			if( strcmp(passDesc.Name, "MODEL_SKIN_P0") == 0 ) {
				modelP0 = pass;
				continue;
			}
			if( strcmp(passDesc.Name, "MODEL_SKIN_P1") == 0 ) {
				modelP1 = pass;
				continue;
			}
		}
	}*/

	
#if 1
	//=============================================================
	// ���_�o�b�t�@���g���ĕ`�攭�s�̍œK�����s�����ꍇ
	// �� DrawPrimitive�̓}�e���A���̌��������Ă΂��B
	//=============================================================

	//---- VAO�ŕ`��
	VertexArrayObject vao = pMesh->getVertexArrayObject(_shadowEnable);

	// �`�揀��
	vao.beginRender();

	// �C���f�b�N�X�o�b�t�@�ݒ�
	GLuint IBO = pMesh->getIndexBuffer();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	for( u32 c=0; c<pMesh->_clusters.size(); c++ ) {
		AssetModelX::Cluster&	cluster   = pMesh->_clusters[c];
		AssetModelX::Material*	pMaterial = cluster._pMaterial;

		if(!_shadowEnable) {
			//---- �e�N�X�`���̐ݒ�
			// �e�N�X�`���ݒ�
			pMaterial->BindTextures();
		}
		
		// �N���X�^�[���̃I�t�Z�b�g�l
		GLint offset = cluster._vertexStart * sizeof(u32);

		glDrawElements(GL_TRIANGLES, cluster._vertexCount, GL_UNSIGNED_INT, (GLvoid*)offset);

		pMaterial->UnBindTextures();

	}

	// �C���f�b�N�X�o�b�t�@����
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// �`��I��
	vao.endRender();

#else

	//---- VBO�ŕ`��(VAO�Ȃ�)

	GLuint	pVB = pMesh->getVertexBuffer();
	GLuint	pIB = pMesh->getIndexBuffer();

	// ���_�o�b�t�@��ݒ�
	glBindBuffer(GL_ARRAY_BUFFER, pVB);			// ���ꂩ��̏����͂���ID�ɔ��f���܂��B
	// �C���f�b�N�X�o�b�t�@��ݒ�
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pIB);	// ���ꂩ��̏����͂���ID�ɔ��f���܂��B
	
	//---- �f�o�b�O�p�R�[�h
	
	AssetModelX::GpuVertex* vertex = (AssetModelX::GpuVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

	for( s32 c=0; c<pMesh->_clusters.size(); c++ ) {
		AssetModelX::Cluster&	cluster   = pMesh->_clusters[c];
		for( s32 i=0; i < cluster._vertexCount; i++ )
		{
			AssetModelX::GpuVertex	 debug = vertex[i];
			for( s32 w=0; w<4; w++ ) {
				f32			 weight	= debug._blendWeight[w];
				if( weight > 0.0f )
				{
					s32 a = 0;
				}
			}

		}
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	

	//------------------------------------------------------------
	// �}�e���A���P�ʂŕ`�攭�s
	//------------------------------------------------------------
	//for( s32 c=0; c<pMesh->_clusters.getCount(); c++ ) {
	for( s32 c=0; c<pMesh->_clusters.size(); c++ ) {
		AssetModelX::Cluster&	cluster   = pMesh->_clusters[c];
		AssetModelX::Material*	pMaterial = cluster._pMaterial;

		//---- �e�N�X�`���̐ݒ�
		// �e�N�X�`���ݒ�
		pMaterial->BindTextures();

		// �f�v�X�e�N�X�`���]��
		//GLuint depthTexHandle = _shadow->getDepthTextureHandle();
		//GLuint depthTexHandle = _shadow->getColorTextureHandle();
		//GLuint depthTexHandle = debugTex.getTextureHandle();
		
		//GmShader()->setTexture("ShadowMap", depthTexHandle, GL_TEXTURE4, 4);

		//s32				pass = 0;
		//if( pTexture ) {	// ����
		//	/*pD3DEffect->SetTexture(hTexture, pTexture->getTexture()->getD3DTexture());
		//	pass = modelP1;*/
		//	
		//}
		//else {				// �Ȃ�
		//	//pass = modelP0;
		//	
		//}

		//------------------------------------------------------------
		// �`��
		//------------------------------------------------------------

		// �N���X�^�[���̃I�t�Z�b�g�l
		GLuint offset = cluster._vertexStart * sizeof(AssetModelX::GpuVertex);

		//---- VertexShader�ɒ��_����]�����L����
		GmShader()->setAndEnableVertexAttribute( "position"		, 3, GL_FLOAT			, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _position		) + offset) );
		GmShader()->setAndEnableVertexAttribute( "blendIndices" , 4, GL_FLOAT			, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _blendIndex	) + offset) );
		GmShader()->setAndEnableVertexAttribute( "blendWeight"  , 4, GL_FLOAT			, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _blendWeight	) + offset) );
		GmShader()->setAndEnableVertexAttribute( "normal"		, 3, GL_FLOAT			, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _normal		) + offset) );
		GmShader()->setAndEnableVertexAttribute( "color"		, 4, GL_UNSIGNED_BYTE	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _diffuseColor	) + offset) );
		GmShader()->setAndEnableVertexAttribute( "uv"			, 2, GL_FLOAT			, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _uv			) + offset) );
		GmShader()->setAndEnableVertexAttribute( "specularPower", 3, GL_FLOAT			, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _shininess		) + offset) );
		
		// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���g���ĕ`��
		//glDrawElements(GL_TRIANGLE_STRIP, cluster._vertexCount, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, cluster._vertexCount, GL_UNSIGNED_INT, 0);

		//---- VertexShader�̒��_���𖳌���
		GmShader()->DisableVertexAttribute( "position"		 );
		GmShader()->DisableVertexAttribute( "blendIndices"	 );
		GmShader()->DisableVertexAttribute( "blendWeight"	 );
		GmShader()->DisableVertexAttribute( "normal"		 );
		GmShader()->DisableVertexAttribute( "color"			 );
		GmShader()->DisableVertexAttribute( "uv"			 );
		GmShader()->DisableVertexAttribute( "specularPower"  );

		pMaterial->UnBindTextures();

	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);			// �o�C���h���O��
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// �o�C���h���O��
	
#endif
}


//-----------------------------------------------------------------------------
//! �t���[���ɑ����郁�b�V����`��
//!	@param	[in]	�`��Ώۂ̃��f���t���[��
//-----------------------------------------------------------------------------
void TaskModelX::_renderFrame(AssetModelX::Frame* pFrame)
{

	if( pFrame->isRender() && pFrame->_meshes.size() != 0 ) {
		//---- �����x���A���t�@�u�����h�L���Ŕ��f
		setBlendMode(BM_BLEND_ALPHA);

		// ���ʃJ�����O ON
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);

		// ���e�X�g�œ��������͂y���������܂Ȃ�
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GEQUAL, 0.5f);

		//---- ���̃t���[���̕ϊ��s������[���h�s��ɂ���
		TaskModelX::Joint*	pJoint = &_joints[ (s32)( pFrame->getJointIndex() ) ];

		f32	scale = _scale;
		Matrix	matWorld = pJoint->_transformMatrix * Matrix::scale(Vector3(scale, scale, -scale)) * _rotateMatrix * _worldMatrix;

		CameraBase*	camera = GmCameraMan()->getCurrentCamera();
		Matrix	matView = camera->getViewMatrix();
		Matrix	matProj = camera->getProjMatrix();
		
		//---- �s����V�F�[�_�ɓ]��
		Matrix sendMatrixData[] =
		{
			matWorld,
			matView,
			matProj,
		};

		GmShader()->setUniformMatrixBuffer(sendMatrixData, sizeof(sendMatrixData));
		

		if( !_shadowEnable ) {
			// ���f���r���[�v���W�F�N�V������]��
			Matrix depthMatrices[_splitCount];

			for( u32 i=0; i<_splitCount; ++i ) { 
				Matrix shadowProj = _shadowProj[i];
				Matrix shadowView = _shadowView[i];

				depthMatrices[i]  = matWorld * shadowView * shadowProj * _biasMatrix;
			}

			GmShader()->setMatrix4x4("depthBiasMVP", _splitCount, GL_FALSE, (GLfloat*)&depthMatrices[0]._m);
		
					
			GmShader()->bindTexture("ShadowMap", _depthTexHandle, 4);
		}
		GmShader()->setUniform1f("texRepeat", (GLfloat)(pFrame->getTexRepeat()) );
		
		getGLError();
	
		//------------------------------------------------------------
		// ���b�V���f�[�^�������Ă�����`�悷��
		//------------------------------------------------------------
		for( u32 mesh=0; mesh<pFrame->_meshes.size(); mesh++ ) {
			_renderMesh( pFrame->_meshes[mesh] );
		}

		GmShader()->unbindTexture();

		getGLError();

	}

	//-------------------------------------------------------------
	// �ċA�Ăяo���Ŏq�ƌZ������s
	//-------------------------------------------------------------
	AssetModelX::Frame*	pNext  = pFrame->getNext();
	AssetModelX::Frame*	pChild = pFrame->getChild();

	if( pNext ) {				//---- �Z��
		_renderFrame( pNext );
	}
	if( pChild ) {				//---- �q
		_renderFrame( pChild );
	}

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_CULL_FACE);
	setBlendMode(BM_NONE);
}	




//-----------------------------------------------------------------------------
//	�`��
//-----------------------------------------------------------------------------
void TaskModelX::render(bool isShadow)
{
	AssetModelX*		pModel     = _pModel;

	if( pModel == NULL ) return;

	// �V���h�E�`�悩�ǂ���
	_shadowEnable = isShadow;
	
	//-------------------------------------------------------------
	// �X�L�j���O�z����V�F�[�_�[�ɐݒ�
	//-------------------------------------------------------------
	Matrix*		pMatrixArray = &_skinLocalMatrices[0];			// �z��
	s32			matrixCount  = _skinLocalMatrices.size();	// �z��
	
	GmShader()->setMatrix4x4("gSkinMatrix", matrixCount, GL_TRUE, (GLfloat*)&pMatrixArray[0]._m);
	//-------------------------------------------------------------
	//	���[�g(��)����`����s
	//-------------------------------------------------------------
	s32 renderCount = 1;

	// �t���[���`��
	_renderFrame( pModel->_frames[0] );

	//-------------------------------------------------------------
	// �A�j���[�V�����̏ڍׂ��f�o�b�O�\��
	//-------------------------------------------------------------
#if 0
	//---- �J�����r���[�Ɠ��e�s��
	CameraBase*	pCamera = ICameraManager()->getCurrentCamera();

	Matrix	matViewProj = Matrix::IDENTITY;
	if( pCamera ) {
		matViewProj = pCamera->getViewProjMatrix();
	}
	else {
		Matrix	matView  = pCamera->getViewMatrix();
		Matrix	matProj  = pCamera->getProjMatrix();

		matViewProj = matView * matProj;
	}

	// �V�F�[�_�I��
	GmShader()->endShader();

	//---- �`��
	// �L�����N�^�[�Ƀ{�[���\�������܂��Ă��܂����߁AZ�B�ʏ����𖳌��ɂ��ďd�˂ĕ`�悷��
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	for( u32 i=0; i<pModel->_frames.size(); i++ ) {
		TaskModelX::Joint*		pJoint = &_joints[ (u32)( pModel->_frames[i]->getJointIndex() ) ];
		
		Matrix	jointWorldMatrix = pJoint->_transformMatrix * Matrix::scale(Vector3(_scale, _scale, -_scale)) * _rotateMatrix * _worldMatrix;
		Vector3	p1               = jointWorldMatrix._m[3];

		if( pModel->_frames[i]->getParent() ) {
			TaskModelX::Joint*	pParent = &_joints[ (u32)( pModel->_frames[i]->getParent()->getJointIndex() ) ];
			
			Vector3	p2 = ( pParent->_transformMatrix * Matrix::scale(Vector3(_scale, _scale, -_scale)) * _rotateMatrix * _worldMatrix)._m[3];
			drawLineG(p1, p2, Color(255,128,0,255), Color(255,0,0,255)); 
		}
		f32		boxSize  = 0.02f * _scale;
		drawBox(p1, Vector3(boxSize, boxSize, boxSize), Color(255,255,0,255)); 

		f32		size  = 0.05f * _scale;
		Vector3	axisX = Vector3( jointWorldMatrix._m[0] ).normalize();
		Vector3	axisY = Vector3( jointWorldMatrix._m[1] ).normalize();
		Vector3	axisZ = Vector3( jointWorldMatrix._m[2] ).normalize();
		drawLineG(p1, p1 + axisX * size, Color(255,128,128,255), Color(255,128,128,255));
		drawLineG(p1, p1 + axisY * size, Color(128,255,128,255), Color(128,255,128,255));
		drawLineG(p1, p1 + axisZ * size, Color(128,128,255,255), Color(128,128,255,255));
	}

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	// ��O�̃V�F�[�_�̍ĊJ
	GmShader()->beginPrevShader();
#endif
}

//-----------------------------------------------------------------------------
//	���f���f�[�^��ݒ�
//!	�O�����烂�f���f�[�^��ݒ肷��ۂɓ��I�ɕω�����f�[�^���^�X�N���Ɋm�ۂ���B
//-----------------------------------------------------------------------------
bool TaskModelX::setModel(AssetModelX* pModel)
{
	_pModel = pModel;

	//-------------------------------------------------------------
	// �t���[���֐߃��[�N���m��
	//-------------------------------------------------------------
	s32	frameCount = pModel->_frames.size();

	_joints.resize(frameCount);	// �A�j���[�V�����s��

	// �֐ߗp���[�J���s��
	_skinLocalMatrices   .resize(frameCount);	// ���[�J���s��

	//-------------------------------------------------------------
	// �s��������ݒ�
	//-------------------------------------------------------------
	_joints[0]._animationMatrix = Matrix::IDENTITY;
	_joints[0]._transformMatrix = Matrix::IDENTITY;

	for(s32 i=1; i<frameCount; i++ ) {
		const AssetModelX::Frame*	pFrame       = pModel->_frames[i];
		const AssetModelX::Frame*	pFrameParent = pFrame->getParent();
		const s32					jointIndex   = (s32)( pFrame	  ->getJointIndex() );
		const s32					parentIndex  = (s32)( pFrameParent->getJointIndex() );

		TaskModelX::Joint&			joint       = _joints[jointIndex ];
		TaskModelX::Joint&			parent      = _joints[parentIndex];

		joint._animationMatrix = pFrame->getTransformMatrix();
		joint._transformMatrix = joint._animationMatrix * parent._transformMatrix;

		//---- �֐ߗp���[�J���s��
		_skinLocalMatrices   [i] = pFrame->getOffsetMatrix() * joint._transformMatrix * pFrame->getRotateMatrix();
	}

	return true;
}

//-----------------------------------------------------------------------------
//	���O����A�j���[�V����������
//	@param	[in]	name	�A�j���[�V������
//	@return �A�j���[�V�����ԍ� (-1:��v���閼�O���Ȃ����o�ł��Ȃ�����)
//-----------------------------------------------------------------------------
s32 TaskModelX::searchAnimation(GM_CSTR name)
{
	if( _pModel == NULL ) return -1;

	for( u32 i=0; i<_pModel->_animations.size(); i++ ) {
		AssetModelX::Animation*	pAnimation = _pModel->_animations[i];

		if( strcmp(name, pAnimation->getName()) == 0 ) {
			return i;
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------
//	�A�j���[�V�������Đ�
//-----------------------------------------------------------------------------
bool TaskModelX::setAnimation(s32 animationNumber, ANIMATION_PLAY_TYPE playType, bool interpolate)
{
	if( _pModel == NULL ) {
		return false;
	}

	// �A�j���[�V�����ԍ����K���͈͂��ǂ���
	s32	animationCount = _pModel->_animations.size();
	if( animationNumber < -1 || animationCount <= animationNumber ) {
		return false;
	}

	//-------------------------------------------------------------
	// �A�j���[�V�����w��
	//-------------------------------------------------------------
	if( animationNumber != -1 ) {
		if( playType == ANIMATION_PLAY_REPEAT ) {
			// �������s�[�g�w��
			_isAnimationRepeat = true;
		}
		else {
			// �A�j���[�V�����f�[�^���烊�s�[�g�������擾���Ďg�p
			AssetModelX::Animation*	pAnimation = _pModel->_animations[animationNumber];
			_isAnimationRepeat = pAnimation->isRepeat();
		}

	}
	_animationPlayType = playType;			// �A�j���[�V�����̑���
	_animationNumber   = animationNumber;
	_animationTime     = 0.0f;

	//-------------------------------------------------------------
	// �A�j���[�V������Ԃ̐ݒ�
	// �u�����h�W����0.0f����J�n����1.0f�֕ω����Ă���
	//-------------------------------------------------------------

	// �A�j���[�V�����u�����h�W����������0.0f�ɂ��Ă����Aupdate()����1.0f�܂Ői�߂�B
	// �y��ԕ����z
	// _blendRotation �� _rotation;		// ��]
	// _blendPosition �� _position;		// �ړ�
	// _blendScale    �� _scale;		// �g�k
	if( interpolate ) {
		_animationBlendRatio = 0.0f;
	}
	else {
		_animationBlendRatio = 1.0f;	// ��Ԃ��Ȃ��ꍇ�͍ŏ������Ԍ�̔䗦�ɂ��Ă���
	}

	//---- ���݂̎p����ۑ��B���̎p�������ԊJ�n
	for( u32 i=0; i<_joints.size(); i++ ) {
		TaskModelX::Joint&	joint = _joints[i];
		joint._blendRotation	  = joint._rotation;	// ��]
		joint._blendPosition	  = joint._position;	// �ړ�
		joint._blendScale		  = joint._scale;		// �g�k
	}
	return true;
}

//-----------------------------------------------------------------------------
//! MinMax�v�Z
//-----------------------------------------------------------------------------
void TaskModelX::calcMinMax()
{
	

	f32	scale = _scale;
	
	Float2 X, Y, Z;		// ���f���ł̍ő�A�ŏ�
	
	X = Float2(FLT_MAX, -FLT_MAX);
	Y = Float2(FLT_MAX, -FLT_MAX);
	Z = Float2(FLT_MAX, -FLT_MAX);

	for(u32 f=0; f<_pModel->_frames.size(); ++f)
	{
		Float2 x, y, z;
		x = Float2(FLT_MAX, -FLT_MAX);
		y = Float2(FLT_MAX, -FLT_MAX);
		z = Float2(FLT_MAX, -FLT_MAX);
		AssetModelX::Frame* frame = _pModel->_frames[f];
		//---- ���̃t���[���̕ϊ��s������[���h�s��ɂ���
		TaskModelX::Joint*	pJoint = &_joints[ (u32)( frame->getJointIndex() ) ];
		Matrix			  matWorld = pJoint->_transformMatrix * Matrix::scale(Vector3(scale, scale, -scale)) * _rotateMatrix * _worldMatrix;
		for( u32 m=0; m<frame->_meshes.size(); ++m )
		{
			AssetModelX::Mesh* mesh = frame->_meshes[m];
			for( u32 v=0; v<mesh->_vertices.size(); ++v )
			{
				AssetModelX::Vertex vertex = mesh->_vertices[v];
				Vector3 position = vertex._position;
				position = position.transform(matWorld);
				
				//---- ���_�̒��̍ő�l�ŏ��l�v��
				x._x = min(position._x, x._x);
				x._y = max(position._x, x._y);

				y._x = min(position._y, y._x);
				y._y = max(position._y, y._y);

				z._x = min(position._z, z._x);
				z._y = max(position._z, z._y);
				
			}
		}
		// �t���[���ł̍ő�ŏ�
		frame->setMinMax(x,y,z);
		//---- �t���[���̒��̍ő�l�ŏ��l�v��
		X._x = min(x._x, X._x);
		X._y = max(x._y, X._y);
		Y._x = min(y._x, Y._x);
		Y._y = max(y._y, Y._y);
		Z._x = min(z._x, Z._x);
		Z._y = max(z._y, Z._y);
	}
	// ���f���ł̍ő�ŏ�
	_pModel->setMinMax(X,Y,Z);

}

//-----------------------------------------------------------------------------
//! �f�o�b�O�`��
//-----------------------------------------------------------------------------
void TaskModelX::debugRender()
{
	AssetModelX*	pModel	= getModel();
	f32				scale	= getScale();
	
	for(u32 i = 0; i < pModel->_frames.size(); ++i) {
		TaskModelX::Joint*	pJoint		= &_joints[(s32)pModel->_frames[i]->getJointIndex()];
		Matrix				modelMatrix = pJoint->_transformMatrix * Matrix::scale(Vector3(scale, scale, -scale)) * getRotateMatrix() * getWorldMatrix();

		for(u32 mesh = 0; mesh < pModel->_frames[i]->_meshes.size(); ++mesh) {
			AssetModelX::Mesh* pMesh = pModel->_frames[i]->_meshes[mesh];

			for(u32 norm = 0; norm < pMesh->_normals.size(); ++norm) {
				Vector3 p1 = pMesh->_vertices[norm]._position.transform(modelMatrix);
				Vector3 n1 = pMesh->_normals[norm].transform(getRotateMatrix());
				drawLineF(p1, p1 + n1*8.0f, Vector4(0.0f,1.0f,0.0f,1.0f));
			}
		}
	}
}

//-----------------------------------------------------------------------------
//! �f�o�b�O�`��(2D)
//-----------------------------------------------------------------------------
void TaskModelX::debugRender2D()
{
}

//=============================================================================
//	END OF FILE
//=============================================================================

