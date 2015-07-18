//-----------------------------------------------------------------------------
//!
//!	@file	gmAssetModelX.cpp
//!	@brief	3D���f���A�Z�b�g
//!	@author	Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//GM_TYPE_IMPLEMENT(AssetModelX, "3D���f���A�Z�b�g");

//-----------------------------------------------------------------------------
//	�R���X�g���N�^
//-----------------------------------------------------------------------------
AssetModelX::AssetModelX(void)
: _pReader(NULL)
, _minX( FLT_MAX)
, _maxX(-FLT_MAX)
, _minY( FLT_MAX)
, _maxY(-FLT_MAX)
, _minZ( FLT_MAX)
, _maxZ(-FLT_MAX)
{
}

//-----------------------------------------------------------------------------
//	�f�X�g���N�^
//-----------------------------------------------------------------------------
AssetModelX::~AssetModelX(void)
{
	//---- �t���[���̉��
	for( vector<AssetModelX::Frame*>::iterator itr = _frames.begin(); itr != _frames.end(); )
	{
		AssetModelX::Frame* pFrame = *itr;
		SAFE_DELETE(pFrame);
		itr = _frames.erase(itr);
	}
	//---- �}�e���A���̉��
	for( vector<AssetModelX::Material*>::iterator itr = _materials.begin(); itr != _materials.end(); )
	{
		AssetModelX::Material* pMaterial = *itr;
		GM_RELEASE(pMaterial);
		itr = _materials.erase(itr);
	}
	//---- �}�e���A���̉��
	for( vector<AssetModelX::Animation*>::iterator itr = _animations.begin(); itr != _animations.end(); )
	{
		AssetModelX::Animation* pAnimation = *itr;
		SAFE_DELETE(pAnimation);
		itr = _animations.erase(itr);
	}

	delete _pReader;
}

//-----------------------------------------------------------------------------
//	���[�h
//!	@param	[in]	fileName	�t�@�C���p�X��
//!	@retval	true	����
//!	@retval	false	���s
//-----------------------------------------------------------------------------
bool AssetModelX::load(GM_CSTR fileName)
{
	// �t�@�C���p�X���擾
	GM_CHAR	driveName[MAX_PATH];
	GM_CHAR	pathName [MAX_PATH];
	_splitpath(fileName, driveName, pathName, NULL, NULL);
	_makepath (_path   , driveName, pathName, NULL, NULL);

	//-------------------------------------------------------------
	// �\���p�[�T�[���쐬
	//-------------------------------------------------------------
	_pReader = new AssetModelX::Reader();
	if( _pReader->init(fileName) == false ) {
		return false;
	}

	//-------------------------------------------------------------
	// �t���[���̃��[�g�쐬
	//-------------------------------------------------------------
	AssetModelX::Frame*	pFrame;
	{
		pFrame = new AssetModelX::Frame();
	
		_frames.push_back( pFrame );
		s32 index = _frames.size() - 1;
		pFrame->setJointIndex((f32)index);
	}
	//--------------------------------------------------------------
	// �t�@�C�����
	//--------------------------------------------------------------
	enum PHASE {
		PHASE_ROOT,		// ���[�g��͒�
		PHASE_FRAME,	// Frame		�`�����N��͒�
	};

	static const s32	NEST_COUNT_MAX = 256;
	PHASE				phase[NEST_COUNT_MAX];
	s32					nest   = 0;

	phase[nest] = PHASE_ROOT;

	//---- �t�@�C���w�b�_���
	Mesh*				pMesh      = NULL;

	_pReader->getToken();		// "xof"
	_pReader->getToken();		// "0303txt"
	_pReader->getToken();		// "0032"

	//---- �t�@�C���f�[�^���
	while( nest >= 0 ) {

		GM_CSTR	token = _pReader->getToken();
		if( token == NULL ) {
			break;
		}

//for( s32 rr=0;rr<nest; rr++ ) GM_TRACE("-");
//GM_TRACE("%s\n", token);

		//---- �yFrame�z���f���t���[��
		if( _pReader->compare("Frame") ) {
			AssetModelX::Frame*		p = _loadFrame();
			if( p == NULL ) {
				nest = -1;
				break;
			}
			pFrame->add( p );
			pFrame = p;

			phase[++nest] = PHASE_FRAME;

			continue;
		}

		//---- �yFrameTransformMatrix�z�t���[�����̊֐ߎp���s��
		if( _pReader->compare("FrameTransformMatrix") ) {
			_pReader->openChunk();
			
			Matrix	transformMatrix;
			f32		x,y,z,w;
			token = _pReader->getToken(); x = (f32)atof(token);
			token = _pReader->getToken(); y = (f32)atof(token);
			token = _pReader->getToken(); z = (f32)atof(token);
			token = _pReader->getToken(); w = (f32)atof(token);
			transformMatrix._m[0] = Vector4(x, y, z, w);

			token = _pReader->getToken(); x = (f32)atof(token);
			token = _pReader->getToken(); y = (f32)atof(token);
			token = _pReader->getToken(); z = (f32)atof(token);
			token = _pReader->getToken(); w = (f32)atof(token);
			transformMatrix._m[1] = Vector4(x, y, z, w);

			token = _pReader->getToken(); x = (f32)atof(token);
			token = _pReader->getToken(); y = (f32)atof(token);
			token = _pReader->getToken(); z = (f32)atof(token);
			token = _pReader->getToken(); w = (f32)atof(token);
			transformMatrix._m[2] = Vector4(x, y, z, w);

			token = _pReader->getToken(); x = (f32)atof(token);
			token = _pReader->getToken(); y = (f32)atof(token);
			token = _pReader->getToken(); z = (f32)atof(token);
			token = _pReader->getToken(); w = (f32)atof(token);
			transformMatrix._m[3] = Vector4(x, y, z, w);

			pFrame->setTransformMatrix(transformMatrix);

			_pReader->closeChunk();
			continue;
		}

		//---- �yMesh�z���f�����b�V��
		if( _pReader->compare("Mesh") ) {
			pMesh = _loadMesh();
			pMesh->setJointIndex( (s32)(pFrame->getJointIndex()) );
			pFrame->_meshes.push_back(pMesh);
			continue;
		}

		//---- �yMaterial�z�}�e���A��
		if( _pReader->compare("Material") ) {
			_loadMaterial();
			continue;
		}

		//---- �yAnimationSet�z�A�j���[�V����
		if( _pReader->compare("AnimationSet") ) {
			_loadAnimation();
			continue;
		}

		if( _pReader->compare("{") ) {
			_pReader->openChunk();
			nest++;
			continue;
		}
		if( _pReader->compare("}") ) {
			if( phase[nest] == PHASE_FRAME ) {
				// �ЂƂ�̊K�w�ɖ߂�
				pFrame = pFrame->getParent();
			}
			_pReader->closeChunk();
			nest--;
			continue;
		}

		//---- ���g�p/���m�̃`�����N�͓ǂݎ̂Ă�
		_pReader->skipChunk();
	}

	//-------------------------------------------------------------
	// ���b�V���f�[�^���œK��
	//-------------------------------------------------------------
	for( u32 f=0; f<_frames.size(); f++ ) {
		AssetModelX::Frame*	pFrame = _frames[f];

		for( u32 m=0; m<pFrame->_meshes.size(); m++ ) {
			AssetModelX::Mesh*	pMesh = pFrame->_meshes[m];
			pMesh->optimize();
		}
	}

	//-------------------------------------------------------------
	// �A�j���[�V�����֐߂̊֘A�t��
	//-------------------------------------------------------------
	for( u32 a=0; a<_animations.size(); a++ ) {
		AssetModelX::Animation*		pAnimation = _animations[a];
		
		for( u32 bone=0; bone<pAnimation->_bone.size(); bone++ ) {
			AssetModelX::Animation::Bone*	pBone  = pAnimation->_bone[bone];
		
			AssetModelX::Frame*				pFrame = searchFrame( pBone->getName() );

			if( pFrame ) {
				pBone->_jointIndex = (s32)( pFrame->getJointIndex() );
			}
			else {
				// �֘A�t��������Ȃ��ꍇ�͊֐ߔԍ��� -1
				pBone->_jointIndex = -1;

				// MessageBox(NULL, "�A�j���[�V�����֐߂̊֘A�t��������Ă��܂���.", "�x��", MB_OK);
			}
		}
	}
	//=============================================================
	// ���_���̓��C�A�E�g�̍쐬 (���_�錾)
	//=============================================================
	//	Float3		_position;			// �ʒu���W�@       [f32 x 3] 12 bytes
	//	u8			_blendIndex [4];	// �X�L�j���OINDEX  [ u8 x 4]  4 bytes
	//	f32			_blendWeight[4];	// �X�L�j���OWEIGHT [f32 x 4] 16 bytes
	//	Float3		_normal;			// �@���@�@�@       [f32 x 3] 12 bytes
	//	D3DCOLOR	_color;				// �J���[�@�@       [ u8 x 4]  4 bytes
	//	f32			_uv;				// �e�N�X�`��UV     [f32 x 2]  8 bytes
	//D3DVERTEXELEMENT9	element[] = {
	//	{0, GM_OFFSET_OF(AssetModelX::GpuVertex, _position   ), D3DDECLTYPE_FLOAT3  , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION    , 0},
	//	{0, GM_OFFSET_OF(AssetModelX::GpuVertex, _blendIndex ), D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
	//	{0, GM_OFFSET_OF(AssetModelX::GpuVertex, _blendWeight), D3DDECLTYPE_FLOAT4  , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT , 0},
	//	{0, GM_OFFSET_OF(AssetModelX::GpuVertex, _normal     ), D3DDECLTYPE_FLOAT3  , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL      , 0},
	//	{0, GM_OFFSET_OF(AssetModelX::GpuVertex, _color      ), D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR       , 0},
	//	{0, GM_OFFSET_OF(AssetModelX::GpuVertex, _uv         ), D3DDECLTYPE_FLOAT2  , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD    , 0},
	//	//---- ��`�I�[
	//	D3DDECL_END()
	//};

	//IDirect3DDevice9*	pD3DDevice = GmRender()->getD3DDevice();
	//HRESULT	hr = pD3DDevice->CreateVertexDeclaration(&element[0], &_pD3DVertexDecl);
	//if( hr != D3D_OK ) {
	//	MessageBox(NULL, L"���_���̓��C�A�E�g�̍쐬�Ɏ��s���܂���.", L"�G���[", MB_OK);

	//	// �G���[�I��
	//	return false;
	//}

	return true;
}

//-----------------------------------------------------------------------------
//! �t���[�����ړ�������
//!	@param	[in]	frameName	�K�p����t���[����
//!	@param	[in]	offset		�ړ���
//-----------------------------------------------------------------------------
bool AssetModelX::setOffsetByName(GM_CSTR frameName, Vector3& offset)
{
	// �t���[���擾
	AssetModelX::Frame* pFrame = searchFrame(frameName);
	if( !pFrame ) return false;
	// �������ǂ����t���O�擾
	if( !pFrame->isDynamic() ){
		// �����t���O�ݒ�
		pFrame->setDynamic(true);
	}

	// �ړ��s��쐬
	Matrix	offsetMatrix = Matrix::translate(offset);
	// �ړ��s��ݒ�
	pFrame->setOffsetMatrix(offsetMatrix);

	return true;
}

//-----------------------------------------------------------------------------
//! �t���[������]������
//!	@param	[in]	frameName	�K�p����t���[����
//!	@param	[in]	rotate		��]��(Radian)
//-----------------------------------------------------------------------------
bool AssetModelX::setRotateByName(GM_CSTR frameName, Vector3& rotate)
{
	// �t���[���擾
	AssetModelX::Frame* pFrame = searchFrame(frameName);
	if( !pFrame ) return false;
	// �������ǂ����t���O�擾
	if( !pFrame->isDynamic() ){
		// �����t���O�ݒ�
		pFrame->setDynamic(true);
	}
	// �N�H�[�^�j�I���쐬
	Quaternion	rotateQuaternion(rotate);
	// ��]�s��ɕϊ�
	Matrix	rotateMatrix = rotateQuaternion.toRotateMatrix();
	// ��]�s��ݒ�
	pFrame->setRotateMatrix(rotateMatrix);
	
	return true;
}

//-----------------------------------------------------------------------------
//! �t���[�����������ǂ����ݒ�
//!	@param	[in]	frameName	�K�p����t���[����
//!	@param	[in]	isDynamic	�������ǂ����t���O
//-----------------------------------------------------------------------------
bool AssetModelX::setDynamicByName(GM_CSTR frameName, bool& isDynamic)
{
	// �t���[���擾
	AssetModelX::Frame* pFrame = searchFrame(frameName);
	if( !pFrame ) return false;
	// �t���O�ݒ�
	pFrame->setDynamic(isDynamic);
	return true;
}

//-----------------------------------------------------------------------------
//! ���I�t���[���������Ă��邩�ǂ���
//-----------------------------------------------------------------------------
bool AssetModelX::haveDynamicFrame() {
	// �����̃t���[���ɃA�N�e�B�u�����邩�ǂ���
	for( u32 i=0; i<_frames.size(); ++i ) {
		if( _frames.at(i)->isDynamic() ){
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
//! �t���[���𖼑O�Ō���
//-----------------------------------------------------------------------------
AssetModelX::Frame* AssetModelX::searchFrame(GM_CSTR name)
{
	AssetModelX::Frame*	pFrame = NULL;

	for( u32 i=0; i<_frames.size(); i++ ) {
		AssetModelX::Frame*	p = _frames[i];
		
		if( strcmp(p->getName(), name) ) continue;
		
		pFrame = p;
		break;
	}
	return pFrame;
}


//-----------------------------------------------------------------------------
//	�t���[���̓ǂݍ���
//!	@return	�m�ۂ��ꂽ�t���[���̃C���f�b�N�X�ԍ��B-1�̏ꍇ�̓������s���Ŏ��s
//-----------------------------------------------------------------------------
AssetModelX::Frame* AssetModelX::_loadFrame(void)
{
	//---- �V�K�ǉ�
	AssetModelX::Frame*	p = new AssetModelX::Frame();

	_frames.push_back( p );
	s32 jointIndex = _frames.size() - 1;
	
	if( jointIndex == -1 ) {
		delete p;
		return NULL;
	}
	p->setJointIndex( (f32)(jointIndex) );

	//---- ���O���擾
	GM_CSTR	name = _pReader->getToken();
	if( _pReader->compare("{") == false ) {
		// ���O����`����Ă���ꍇ�͓o�^
		p->setName(name);
	}
	else {
		// ���O���Ȃ��ꍇ�͋󕶎��� "" �����蓖�Ă�
		p->setName(name + 1);
	}
	_pReader->openChunk();
			
	return p;
}

//-----------------------------------------------------------------------------
//	���b�V���̓ǂݍ���
//!	@return	�쐬���ꂽ�I�u�W�F�N�g�BNULL�̏ꍇ�̓������s���Ŏ��s
//-----------------------------------------------------------------------------
AssetModelX::Mesh* AssetModelX::_loadMesh(void)
{
	AssetModelX::Mesh*	pMesh = new AssetModelX::Mesh(this);
	if( pMesh == NULL ) {
		return NULL;
	}

	GM_CSTR	token;

	_pReader->openChunk();

	//---- ���_�� �yn;�z
	token = _pReader->getToken();
	s32	vertexCount = atoi(token);

	// ���_���ɍ��킹�Ē��_�z����m��
	pMesh->_vertices.resize(vertexCount);

	//---- ���_�f�[�^(xyz)���������݂���yx;y;z;,�z
	// �y�����Ӂ��z���̂Ƃ��̍��W�n�́u������W�n�v
	for( s32 i=0; i<vertexCount; i++ ) {
		token = _pReader->getToken();
		f32	x = static_cast<f32>( atof(token) );
		token = _pReader->getToken();
		f32	y = static_cast<f32>( atof(token) );
		token = _pReader->getToken();
		f32	z = static_cast<f32>( atof(token) );

		// ���_�ɏ�������
		pMesh->_vertices[i]._position = Vector3(x, y, z);		
	}

	//---- �|���S�����yn;�z
	token = _pReader->getToken();
	s32	faceCount = atoi(token);
	pMesh->_faces.resize(faceCount);

	//---- �yn�p�`; index1, index2, index3;,�z
	for( s32 i=0; i<faceCount; i++ ) {
		Face&	face = pMesh->_faces[i];

		// n�p�`
		token = _pReader->getToken();
		s32	indexCount = atoi(token);

		// �z�񐔂��I�[�o�[���Ă���|���S���̓J�b�g����
		// �����I�ɂ͎O�p�`�����őΉ��\
		if( indexCount > Face::VERTEX_COUNT_MAX ) {
//			GM_TRACE("���p�`����������܂����B�����_�ł͖��Ή��ł��B\n");
			indexCount = Face::VERTEX_COUNT_MAX - 1;
		}
		face._vertexCount = indexCount;
		// ���_�C���f�b�N�X���擾
		for( s32 n=0; n<indexCount; n++ ) {
			token = _pReader->getToken();
			s32	index = atoi(token);
			face._vertexIndex[n] = index;
		}
	}

	while( token = _pReader->getToken() ) {
		//---- �`�����N�Q�̏I��
		if( _pReader->compare("}") ) {
			break;
		}
//GM_TRACE("=");
//GM_TRACE("%s\n", token);
		//------------------------------------------------------------
		// �@��
		//------------------------------------------------------------
		if( _pReader->compare("MeshNormals") ) {
			_pReader->openChunk();

			//---- �@���z��
			token = _pReader->getToken();
			s32	count = atoi(token);
			pMesh->_normals.resize(count);

			// �@���f�[�^XYZ
			for( s32 i=0; i<count; i++ ) {
				token = _pReader->getToken();
				f32	x = (f32)atof(token);
				token = _pReader->getToken();
				f32	y = (f32)atof(token);
				token = _pReader->getToken();
				f32	z = (f32)atof(token);

				pMesh->_normals[i] = Vector3(x,y,z);
			}

			//---- �@���C���f�b�N�X�z��
			token = _pReader->getToken();
			s32	indexCount = atoi(token);

			// �@���C���f�b�N�X�f�[�^
			for( s32 i=0; i<indexCount; i++ ) {

				token = _pReader->getToken();
				s32	vertexCount = atoi(token);

				for( s32 vi=0; vi<vertexCount; vi++ ) {
					token = _pReader->getToken();
					s32		normalIndex = atoi(token);

					pMesh->_faces[i]._normalIndex[vi] = normalIndex;
				}
			}

			_pReader->closeChunk();
			continue;
		}

		//------------------------------------------------------------
		// �e�N�X�`��UV���W
		//------------------------------------------------------------
		if( _pReader->compare("MeshTextureCoords") ) {
			_pReader->openChunk();

			//---- UV��
			token = _pReader->getToken();
			s32	count = atoi(token);
			// UV���W���X�g(���_��ɑΉ�)
			for( s32 i=0; i<count; i++ ) {
				token = _pReader->getToken();
				f32	u = (f32)atof(token);
				token = _pReader->getToken();
				f32	v = (f32)atof(token);

				pMesh->_vertices[i]._uv = Float2(u, v);
			}
			_pReader->closeChunk();
			continue;
		}

		//------------------------------------------------------------
		// �e�N�X�`�����W�Ɩ@���̃f�[�^�iMeshTextureCoords��MeshNormals�́j
		//------------------------------------------------------------
		if(_pReader->compare("DeclData") ) {
			_pReader->openChunk();

			token = _pReader->getToken();
			signed int elements = atoi(token);
			signed int type[3];

			if(elements > 3) {
				MessageBoxA(NULL, "DeclData contains more than 3 elements, please redo code or change the model.", "WARNING!", MB_OK | MB_ICONERROR);
			}
			
			// ���킩���Ă���̂Ŏ擾���Ȃ��Ă���
			token = _pReader->getToken();
			token = _pReader->getToken();
			token = _pReader->getToken();
			type[0] = atoi(token);
			token = _pReader->getToken();

			token = _pReader->getToken();
			token = _pReader->getToken();
			token = _pReader->getToken();
			type[1] = atoi(token);
			token = _pReader->getToken();

			if(elements > 2) {
				token = _pReader->getToken();
				token = _pReader->getToken();
				token = _pReader->getToken();
				type[2] = atoi(token);
				token = _pReader->getToken();
			}

			token = _pReader->getToken();
			signed int count = atoi(token);
			//�@�� : 3xDWORD �^���W�F���g: 3xDWORD �e�N�X�`�����W : 2xDWORD = 8x DWORD
			signed int divVal;
			signed int size;
			if(elements == 2) {
				if(type[1] == 6) {
					divVal = 6;
					size = (count / divVal);

					//pMesh->_tangents.resize(size);
				}
				if(type[1] == 5) {
					divVal = 5;
					size = (count / divVal);
				}
			}else {
				divVal = 8;
				size = (count / divVal);

				//pMesh->_tangents.resize(size);
			}			

			pMesh->_normals.resize(size);
			
			signed int elementNumber = 0;

			float x, y, z;
			x = y = z = 0.0f;
			float tx, ty, tz;
			tx = ty = tz = 0.0f;
			float u, v;
			u = v = 0.0f;
			signed int arrNum = 0;
			//	���Ԃ͖@���� x->y->z �^���W�F���g�� x->y->z �e�N�X�`���� u->v
			for(signed int i = 0; i < count; ++i) {

				if(i == count-1) {
					int a = 0;
				}

				token = _pReader->getToken();
				DWORD val = strtoul(token, 0, 0);
				switch(elementNumber) {
				case 0:	//---- 0 = x
					x = *(float*)&val;
					break;
				case 1:	//---- 1 = y
					y = *(float*)&val;
					break;
				case 2:	//---- 2 = z
					z = *(float*)&val;
					pMesh->_normals[arrNum] = Vector3(x,y,z);
					x = y = z = 0.0f;
					break;
				case 3:	//---- 3 = tx
					tx = *(float*)&val;
					break;
				case 4:	//---- 4 = ty
					ty = *(float*)&val;
					break;
				case 5:	//---- 5 = tz
					tz = *(float*)&val;
					//pMesh->_tangents[arrNum] = Vector3(tx,ty,tz);
					tx = ty = tz = 0.0f;
					break;
				case 6:	//---- 6 = u
					u = *(float*)&val;
					break;
				case 7:	//---- 4 = v
					v = *(float*)&val;

					pMesh->_vertices[arrNum]._uv = Float2(u, v);
					u = v = 0.0f;
					break;
				};
				elementNumber++;
				//---- �^���W�F���g�̗v�f�Ȃ��ꍇ�e�N�X�`�����W�ɃX�L�b�v����
				if(elements == 2 && divVal == 5 && elementNumber == 3) {
					elementNumber = 6;
				}
				//---- �e�N�X�`�����W�̗v�f�Ȃ��ꍇ�͖@���ɃX�L�b�v����
				if(elements == 2 && divVal == 6 && elementNumber == 6) {
					elementNumber = 8;
				}

				if(elementNumber == 8) {
					arrNum++;
					elementNumber = 0;
				}
			}

			signed int faceCount = pMesh->_faces.size();
			for(int i = 0; i < faceCount; ++i) {
				for(int j = 0; j < pMesh->_faces[i]._vertexCount; ++j) {
					signed int normIdx = pMesh->_faces[i]._vertexIndex[j];
					pMesh->_faces[i]._normalIndex[j] = normIdx;
				}
			}

			_pReader->closeChunk();
			continue;
		}

		//------------------------------------------------------------
		// �X�L���A�j���[�V����
		//------------------------------------------------------------
		if( _pReader->compare("XSkinMeshHeader") ) {	//---- �w�b�_
			pMesh->loadSkinMeshHeader(_pReader);
			continue;
		}

		if( _pReader->compare("SkinWeights") ) {		//---- �X�L���E�F�C�g
			pMesh->loadSkinWeight(_pReader);
			continue;
		}

		//------------------------------------------------------------
		// �}�e���A�����i�e�N�X�`�����j
		//------------------------------------------------------------
		if( _pReader->compare("MeshMaterialList") ) {
			_pReader->openChunk();

			//---- �}�e���A����
			token = _pReader->getToken();
			s32	materialCount = atoi(token);

			//---- �}�e���A���ԍ���
			token = _pReader->getToken();
			s32	count = atoi(token);

			//s32	faceCount = pMesh->_faces.getCount();
			s32	faceCount = pMesh->_faces.size();
		
			// �}�e���A���ԍ����X�g
			if( count == 1 ) {
				token = _pReader->getToken();
				s32	index = atoi(token);

				for( s32 i=0; i<faceCount; i++ ) {
				
					// �|���S���Ƀ}�e���A���ԍ���ݒ�
					pMesh->_faces[i]._material = index;
				}
			}
			else {
				for( s32 i=0; i<count; i++ ) {
					token = _pReader->getToken();
					s32	index = atoi(token);
				
					// �|���S���Ƀ}�e���A���ԍ���ݒ�
					pMesh->_faces[i]._material = index;
				}
			}
			//---- �}�e���A��
			for( s32 i=0; i<materialCount; i++ ) {
				// �}�e���A���̍ŏ��̃g�[�N����ǂݍ���ł���
				_pReader->getToken();
				
				AssetModelX::Material*	pMaterial = _loadMaterial();
				//pMesh->_materials.push(pMaterial);
				pMesh->_materials.push_back(pMaterial);
			}
			_pReader->getToken();		// ���O�̃}�e���A���� "}"���X�L�b�v���Ă���

			_pReader->closeChunk();
			continue;
		}

		//---- ���m�̃`�����N�͓ǂݎ̂Ă�
		_pReader->skipChunk();
	}
	_pReader->closeChunk();

	return pMesh;
}

//-----------------------------------------------------------------------------
//	�}�e���A���ǂݍ���
//!	@return	�쐬���ꂽ�I�u�W�F�N�g�BNULL�̏ꍇ�̓������s���Ŏ��s
//!	@note	���̐錾�ƃ��x���w��̏ꍇ�ŋ������قȂ�A���x���̏ꍇ��
//!			�����̃}�e���A�����疼�O�������ĊY������}�e���A����Ԃ��B
//-----------------------------------------------------------------------------
AssetModelX::Material* AssetModelX::_loadMaterial(void)
{
	Material*	pMaterial = NULL;

	//---- ���x��
	//	�y�����z	{ �}�e���A���� }
	if( _pReader->compare("{") ) {
		_pReader->openChunk();

		GM_CSTR	materialName = _pReader->getToken();
		//------------------------------------------------------------
		// �}�e���A�����X�g���疼�O�Ō���
		//------------------------------------------------------------
		for( u32 i=0; i<_materials.size(); i++ ) {
			AssetModelX::Material* p = _materials[i];
			if( strcmp(materialName, p->getName() ) != 0 ) continue;
		
			pMaterial = p;
			break;
		}
		if( pMaterial == NULL ) {
			MessageBox(NULL, "�}�e���A�����ɑΉ�����f�[�^�����o�ł��܂���ł����B", NULL, MB_OK);
		}
		_pReader->closeChunk();
	}
	else {
		//------------------------------------------------------------
		// �}�e���A����V�K�擾
		//------------------------------------------------------------
		GM_CSTR	token = _pReader->getToken();
		
		pMaterial = new AssetModelX::Material();

		//---- �}�e���A�������擾
		if( _pReader->compare("{") ) {	//---- �}�e���A�����Ȃ�
			pMaterial->setName("");
		}
		else {							//---- �}�e���A��������
			// �}�e���A�����̒�`������ꍇ�͖��O�o�^����
			pMaterial->setName(token);
		}

		//------------------------------------------------------------
		// Material�`�����N�̓ǎ��
		//------------------------------------------------------------
		_pReader->openChunk();
		{
			//---- �}�e���A�����
			f32	r,g,b,a;

			// faceColor
			token = _pReader->getToken(); r = (f32)atof(token);
			token = _pReader->getToken(); g = (f32)atof(token);
			token = _pReader->getToken(); b = (f32)atof(token);
			token = _pReader->getToken(); a = (f32)atof(token);

			//D3DCOLOR	faceColor = D3DCOLOR_COLORVALUE(r, g, b, a);
			Vector4	faceColor = Vector4(r, g, b, a);
			pMaterial->setDiffuseColor(faceColor);

			f32 specPow;
			// specularPower
			token = _pReader->getToken(); specPow = (f32)atof(token);
			pMaterial->setShininess(specPow);
			// specularColor
			token = _pReader->getToken(); r = (f32)atof(token);
			token = _pReader->getToken(); g = (f32)atof(token);
			token = _pReader->getToken(); b = (f32)atof(token);
			Vector4	specularColor = Vector4(r, g, b, a);
			pMaterial->setSpecularColor(specularColor);
			// emissiveColor
			token = _pReader->getToken(); r = (f32)atof(token);
			token = _pReader->getToken(); g = (f32)atof(token);
			token = _pReader->getToken(); b = (f32)atof(token);
			Vector4	emissiveColor = Vector4(r, g, b, a);
			pMaterial->setEmissiveColor(emissiveColor);

			//---- TextureFileName�`�����N
			token = _pReader->getToken();
			if( _pReader->compare("TextureFilename") || _pReader->compare("TextureFileName") ) {
				_pReader->openChunk();
				{
					// �e�N�X�`���t�@�C����������
					// �O��� ""�L������
					token = _pReader->getToken();
					
					s32		length   = strlen(token);
					GM_STR	filePath = new char[ length ];	// �h + ������ + �h
					
					// ���[��""�L������������
					strcpy(filePath, token + 1);
					filePath[length - 2] = '\0';

					// X�t�@�C���̈ʒu����̑��Έʒu���쐬
					GM_CHAR		fullPath[MAX_PATH];
					_makepath(fullPath, NULL, _path, filePath, NULL);

					// �e�N�X�`���t�@�C���̓ǂݍ���
					Texture*	pTexture = new Texture();
					if( !pTexture->loadAndTexImage2D(fullPath, true) )
					{
						ASSERTMSG(false, "�f�B�t���[�Y�e�N�X�`���̓ǂݍ��ݎ��s");
					}
					pMaterial->setTexture( pTexture );
					
					// �X�y�L�����}�b�v�̓ǂݍ���
					string newPath = fullPath;
					s32 under = newPath.rfind("_d");
					if( under > 0 ){
						under++; // _�̕����Ȃ̂Ŏ���
						newPath[under] = 's';
						Texture* pSpecularMap = new Texture();
						if( pSpecularMap->loadAndTexImage2D(newPath.c_str(), true) ){
							pMaterial->setSpecularMap(pSpecularMap);
						}else{
							SAFE_DELETE(pSpecularMap);
						}
					}

					under = newPath.rfind("_s");
					if( under > 0 ){
						under++;
						newPath[under] = 'n';
						Texture* pNormalMap = new Texture();
						if( pNormalMap->loadAndTexImage2D(newPath.c_str(), true) ) {
							pMaterial->setNormalMap(pNormalMap);
						}else{
							SAFE_DELETE(pNormalMap);
						}
					}

					delete [] filePath;
				}
				_pReader->closeChunk();
				_pReader->getToken();		// �A���Ŋ��ʂ���邽�߁A�g�[�N����i�߂Ă���
			}			
			//-----------------------------------------------------------
			// �}�e���A����S�̂̃��X�g�ɓo�^
			//-----------------------------------------------------------
			//_materials.push(pMaterial);
			_materials.push_back(pMaterial);
		}
		_pReader->closeChunk();
	}
	return pMaterial;
}

//-----------------------------------------------------------------------------
//	�A�j���[�V�����ǂݍ���
//-----------------------------------------------------------------------------
AssetModelX::Animation* AssetModelX::_loadAnimation(void)
{
	//---- �V�K�o�^
	AssetModelX::Animation*	pAnimation = new AssetModelX::Animation();
	if( pAnimation == NULL ) {
		return NULL;
	}
	_animations.push_back(pAnimation);

	//-------------------------------------------------------------
	// �A�j���[�V���������擾
	//-------------------------------------------------------------
	{
		GM_CSTR	animationSetName = _pReader->getToken();	// �A�j���[�V������
		pAnimation->setName(animationSetName);
	}

	//-------------------------------------------------------------
	// �A�j���[�V�����L�[�t���[���f�[�^���擾
	//-------------------------------------------------------------
	_pReader->openChunk();
	for( ;; ) {
		GM_CSTR	token = _pReader->getToken();
		if( _pReader->compare("}") ) {				//---- �I�[
			_pReader->closeChunk();
			break;
		}

		//---- �A�j���[�V�����L�[�t���[���f�[�^
		if( _pReader->compare("Animation") ) {
			GM_CSTR	animationName = _pReader->getToken();

			pAnimation->_loadKeyData(_pReader);
			continue;
		}

		//---- ���g�p/���m�̃`�����N�͓ǂݎ̂Ă�
		_pReader->skipChunk();
	}

	return pAnimation;
}

//=============================================================================
//! AssetModelX::Name			���O������f�[�^
//=============================================================================

//-----------------------------------------------------------------------------
//	�R���X�g���N�^
//-----------------------------------------------------------------------------
AssetModelX::Name::Name(GM_CSTR name)
{
	// ������o�b�t�@�����Ȃ��悤�ɖ����ɂ� \0 �����Ă����B
	_name[0] = '\0';						// �擪
	_name[ sizeof(_name) - 1 ] = '\0';		// ����

	setName(name);
}

//-----------------------------------------------------------------------------
//	�������ݒ�
//-----------------------------------------------------------------------------
void AssetModelX::Name::setName(GM_CSTR name)
{
	// �R�s�[�T�C�Y���v�Z�B \0���܂ރT�C�Y�ƃo�b�t�@�T�C�Y�ŏ����������g�p�B
	// �� ���̏ꍇ�A�o�b�t�@�̖����͉󂳂Ȃ��悤�ɂ���(1�������Ȃ��R�s�[)�B
	//    �o�b�t�@�I�[�o�[�t���[�̑΍�ɃR���X�g���N�^�� \0���Z�b�g���Ă���B
	s32	copySize = min(strlen(name) + 1,  sizeof(_name) - 1);
	memcpy(_name, name, copySize);
}

//-----------------------------------------------------------------------------
//	��������擾
//-----------------------------------------------------------------------------
GM_CSTR AssetModelX::Name::getName(void) const
{
	return _name;
}


//=============================================================================
//	AssetModelX::SkinWeight		�X�L���E�F�C�g
//=============================================================================

//-----------------------------------------------------------------------------
//	�R���X�g���N�^
//-----------------------------------------------------------------------------
AssetModelX::SkinWeight::SkinWeight(void)
: _offsetMatrix(Matrix::IDENTITY)
{
}

//-----------------------------------------------------------------------------
//	�f�X�g���N�^
//-----------------------------------------------------------------------------
AssetModelX::SkinWeight::~SkinWeight(void)
{
}

//=============================================================================
//	AssetModelX::Reader			�\���p�[�T�[
//=============================================================================

//-----------------------------------------------------------------------------
//	�R���X�g���N�^
//-----------------------------------------------------------------------------
AssetModelX::Reader::Reader(void)
: _pCurrent(NULL)
, _pBuffer(NULL)
, _bufferSize(0)
{
}

//-----------------------------------------------------------------------------
//	�f�X�g���N�^
//-----------------------------------------------------------------------------
AssetModelX::Reader::~Reader(void)
{
	cleanup();
}

//-----------------------------------------------------------------------------
//	������
//-----------------------------------------------------------------------------
bool AssetModelX::Reader::init(GM_CSTR fileName)
{
	//-------------------------------------------------------------
	// �t�@�C����ǂݍ���
	//-------------------------------------------------------------
	HANDLE	hFile = CreateFileA(
						fileName,
						GENERIC_READ,
						FILE_SHARE_READ,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_READONLY,
						NULL);

	if( hFile == INVALID_HANDLE_VALUE ) {
		MessageBoxA(NULL, fileName , "�t�@�C���ǂݍ��݂Ɏ��s���܂����B", MB_OK);
		return false;
	}

	// �t�@�C���T�C�Y���擾
	u32		size    = GetFileSize(hFile, NULL);
	_pBuffer = new GM_CHAR [ size + 1 ];
	_pBuffer[size] = ' ';

	_bufferSize = size + 1;
	// ��������̃o�b�t�@�Ɉꊇ�ǂݍ���
	DWORD	readSize;
	ReadFile(hFile, _pBuffer, size, &readSize, NULL);

	// �t�@�C�������
	CloseHandle(hFile);


	_pCurrent = _pBuffer;
	return true;
}

//-----------------------------------------------------------------------------
//	���
//-----------------------------------------------------------------------------
void AssetModelX::Reader::cleanup(void)
{
	if( _pBuffer ) {
		delete [] _pBuffer;
		_pBuffer = NULL;
	}
}

//-----------------------------------------------------------------------------
//! ���̃g�[�N�����P�擾
//!	@return �g�[�N��������̐擪�i����getToken()�����s����Ə��ł���ꎞ�I�Ȃ��́j
//!	@return	NULL�Ȃ�t�@�C���I�[
//-----------------------------------------------------------------------------
GM_CSTR AssetModelX::Reader::getToken(void)
{
	GM_STR&	p     = _pCurrent;
	GM_STR	pTail = _pBuffer + _bufferSize;

	if( p >= pTail ) return NULL;

	//---- ��؂�L���A�󔒁A���s���X�L�b�v
	while(
		*p == ' ' || *p == '\t' || *p == 0x0a || *p == 0x0d || *p == ';' || *p == ','
	) {
		p++;
		if( p >= pTail ) return NULL;
	}

	s32	count = 0;

	//---- �R�����g���̌��m
	// �R�����g�͉��s�܂œǂݔ�΂�
	if( p[0] == '/' && p[1] == '/' ) {
		while(
			!(*p == 0x0a || *p == 0x0d)
		) {
			p++;
			if( p >= pTail ) return NULL;
		}

		//---- ��؂�L���A�󔒁A���s���X�L�b�v
		while(
			*p == ' ' || *p == '\t' || *p == 0x0a || *p == 0x0d || *p == ';' || *p == ','
		) {
			p++;
			if( p >= pTail ) return NULL;
		}
	}

	//---- ���̋�؂�܂Ō���
	do {
		_token[count] = *p;
		
		// �����ʂ͂���_token�ɓ���邽��break?(���̏������ɓ���Ă���������Ȃ��H)
		// ���ɂ��ꂽ��if(count == 0)����Ȃ��Ȃ�񂶂�Ȃ��H
		if( *p == '{' || *p == '}' ) {
			break;	
		}
		count++;
		p++;
		if( p >= pTail ) return NULL;

	} while ( !(
		 *p == ' ' || *p == '\t' || *p == 0x0a || *p == 0x0d || *p == ';' || *p == ','
	) );

	if( count == 0 ) {
		count++;
		p++;
	}
	_token[count]=  '\0';

	return _token;
}

//-----------------------------------------------------------------------------
//	���݂̃g�[�N���ƕ������r
//-----------------------------------------------------------------------------
bool AssetModelX::Reader::compare(GM_CSTR token)
{
	return strncmp(_token, token, strlen(_token)) == 0;
}

//-----------------------------------------------------------------------------
//	�`�����N���X�L�b�v
//!	@retval	true	�����i����I���j
//!	@retval	false	���s�i�G���[�I���j
//!
//!	{}�ň͂܂ꂽ�`�����N�f�[�^����e�ɂ�����炸�X�L�b�v���ēǂݎ̂Ă܂��B
//-----------------------------------------------------------------------------
bool AssetModelX::Reader::skipChunk(void)
{
	//----- �ŏ��̃`�����N�I�[�v��������
	if( searchNextToken("{") == false ) {
		return false;
	}

	//---- �`�����N�����܂Ō���
	s32	nest = 1;
	do {
		GM_CSTR token = getToken();

		// �g�[�N�����r�؂ꂽ��G���[�I��
		if( token == NULL ) {
			return false;
		}

		// ����q�\���i�l�X�g�j���J�E���g���ē���K�w������܂Ō���
		if     ( compare("{") ) nest++;
		else if( compare("}") ) nest--;

	} while( 0 < nest );

	// ����
	return true;
}

//-----------------------------------------------------------------------------
//	�`�����N���J��
//-----------------------------------------------------------------------------
bool AssetModelX::Reader::openChunk(void)
{
	return searchNextToken("{");
}

//-----------------------------------------------------------------------------
//	�`�����N�����
//-----------------------------------------------------------------------------
bool AssetModelX::Reader::closeChunk(void)
{
	return searchNextToken("}");
}

//-----------------------------------------------------------------------------
//	���̃g�[�N���̏o���܂Ői�߂�
//-----------------------------------------------------------------------------
bool AssetModelX::Reader::searchNextToken(GM_CSTR token)
{
	while( compare(token) == false ) {
		if( getToken() == NULL ) {
			return false;
		}
	}
	return true;
}

//=============================================================================
//	AssetModelX::Material		�}�e���A���f�[�^
//=============================================================================

//-----------------------------------------------------------------------------
//	�R���X�g���N�^
//-----------------------------------------------------------------------------
AssetModelX::Material::Material(void)
: _diffuseColor ( 255,255,255,255 )
, _pTexture		(nullptr)
, _pSpecularMap (nullptr)
, _pNormalMap	(nullptr)
{
}

//-----------------------------------------------------------------------------
//	�f�X�g���N�^ (���ڌĂяo�����֎~����)
//-----------------------------------------------------------------------------
AssetModelX::Material::~Material(void)
{
	SAFE_DELETE(_pTexture);
	SAFE_DELETE(_pSpecularMap);
	SAFE_DELETE(_pNormalMap);
}

//-----------------------------------------------------------------------------
//	�f�B�t���[�Y�F���擾
//-----------------------------------------------------------------------------
Vector4 AssetModelX::Material::getDiffuseColor(void) const
{
	return _diffuseColor;
}

//-----------------------------------------------------------------------------
//	�X�y�L�����F���擾
//-----------------------------------------------------------------------------
Vector4 AssetModelX::Material::getSpecularColor(void) const
{
	return _specularColor;
}

//-----------------------------------------------------------------------------
//	���ȏƖ��F���擾
//-----------------------------------------------------------------------------
Vector4 AssetModelX::Material::getEmissiveColor(void) const
{
	return _emissiveColor;
}

//-----------------------------------------------------------------------------
//	�p���[�l���擾
//-----------------------------------------------------------------------------
float AssetModelX::Material::getShininess(void) const
{
	return _shininess;
}
	


//-----------------------------------------------------------------------------
//	�e�N�X�`�����擾
//-----------------------------------------------------------------------------
Texture* AssetModelX::Material::getTexture(void) const
{
	return _pTexture;
}

//-----------------------------------------------------------------------------
//	�X�y�L�����}�b�v���擾
//-----------------------------------------------------------------------------
Texture* AssetModelX::Material::getSpecularMap(void) const
{
	return _pSpecularMap;
}

//-----------------------------------------------------------------------------
//	�m�[�}���}�b�v���擾
//-----------------------------------------------------------------------------
Texture* AssetModelX::Material::getNormalMap(void) const
{
	return _pNormalMap;
}

//-----------------------------------------------------------------------------
//	�f�B�t���[�Y�F��ݒ�
//-----------------------------------------------------------------------------
void AssetModelX::Material::setDiffuseColor(Vector4 color)
{
	_diffuseColor = color;
}


//-----------------------------------------------------------------------------
//	�X�y�L�����F��ݒ�
//-----------------------------------------------------------------------------
void AssetModelX::Material::setSpecularColor(Vector4 color)
{
	_specularColor = color;
}

//-----------------------------------------------------------------------------
//	���ȏƖ��F��ݒ�
//-----------------------------------------------------------------------------
void AssetModelX::Material::setEmissiveColor(Vector4 color)
{
	_emissiveColor = color;
}

//-----------------------------------------------------------------------------
//	�p���[�l��ݒ�
//-----------------------------------------------------------------------------
void AssetModelX::Material::setShininess(f32 shininess)
{
	_shininess = shininess;
}


//-----------------------------------------------------------------------------
//	�e�N�X�`����ݒ�
//-----------------------------------------------------------------------------
void AssetModelX::Material::setTexture(Texture* pTexture)
{
	_pTexture = pTexture;
}

//-----------------------------------------------------------------------------
//	�X�y�L�����}�b�v��ݒ�
//-----------------------------------------------------------------------------
void AssetModelX::Material::setSpecularMap(Texture* pSpecularMap)
{
	_pSpecularMap = pSpecularMap;
}

//-----------------------------------------------------------------------------
//	�m�[�}���}�b�v��ݒ�
//-----------------------------------------------------------------------------
void AssetModelX::Material::setNormalMap(Texture* pNormalMap)
{
	_pNormalMap = pNormalMap;
}

//-----------------------------------------------------------------------------
//! �V�F�\�_�̃e�N�X�`����ݒ�
//-----------------------------------------------------------------------------
void AssetModelX::Material::BindTextures()
{
	// ���ݎg�p���̃V�F�\�_�擾
	GLuint shader   = GmShader()->getShaderProgram(SystemShader::SHADER_TEXTURE);
	// �V�F�\�_�e�N�X�`���n���h���擾
	GLuint hTexture = glGetUniformLocation(shader, "BaseTexture");
	// �e�N�X�`���O��L����
	glActiveTexture(GL_TEXTURE0);
	// �e�N�X�`���n���h���擾
	GLuint	textureHandle;
	// �e�N�X�`�����Ȃ����
	if( _pTexture != nullptr ){
			textureHandle = _pTexture->getTextureHandle();
	}else{
			textureHandle = GmShader()->getNullTexHandle();
	}
	// �e�N�X�`�����蓖��
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	// �n���h���ɔԍ����Ђ��t��
	glUniform1i(hTexture, 0);
	// �e�N�X�`���̕i���ݒ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
		
	GLuint hSpecular = glGetUniformLocation(shader, "SpecularMap");
	glActiveTexture(GL_TEXTURE1);
	GLuint	specularHandle;
	// �e�N�X�`�����Ȃ����
	if( _pSpecularMap != nullptr ) {
		specularHandle = _pSpecularMap->getTextureHandle();
	}else{
		specularHandle = GmShader()->getNullSpecHandle();
	}
	glBindTexture(GL_TEXTURE_2D, specularHandle);
	// �n���h���ɔԍ����Ђ��t��
	glUniform1i(hSpecular, 1);
	// �e�N�X�`���̕i���ݒ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
	GLuint hNormal = glGetUniformLocation(shader, "NormalMap");
	glActiveTexture(GL_TEXTURE2);
	GLuint	normalHandle;
	if( _pNormalMap != nullptr ) {
		normalHandle = _pNormalMap->getTextureHandle();
	}else{
		normalHandle = GmShader()->getNullNormHandle();
	}
	glBindTexture(GL_TEXTURE_2D, normalHandle);
	// �n���h���ɔԍ����Ђ��t��
	glUniform1i(hNormal, 2);
	// �e�N�X�`���̕i���ݒ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//-----------------------------------------------------------------------------
//! �V�F�\�_�̃e�N�X�`���̐ݒ������
//-----------------------------------------------------------------------------
void AssetModelX::Material::UnBindTextures()
{
	Texture::set(nullptr);
}


//=============================================================================
//	AssetModelX::Animation
//=============================================================================

//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
AssetModelX::Animation::~Animation(void)
{
	//---- �{�[���̉��
	vector<AssetModelX::Animation::Bone*>::iterator itr;
	for( itr = _bone.begin(); itr != _bone.end(); )
	{
		AssetModelX::Animation::Bone* pBone = *itr;
		SAFE_DELETE(pBone);
		itr = _bone.erase(itr);
	}
}

//-----------------------------------------------------------------------------
//	�{�[��/�L�[�t���[������ǂݍ���
//-----------------------------------------------------------------------------
bool AssetModelX::Animation::_loadKeyData(AssetModelX::Reader* pReader)
{
	pReader->openChunk();

	// �{�[�������i����̓A�j���[�V�����N���X���s�Ȃ��j
	AssetModelX::Animation::Bone*	pBone = new AssetModelX::Animation::Bone();
	_bone.push_back(pBone);

	//--------------------------------------------------------------
	// �A�j���[�V���`�����N���̉��
	//--------------------------------------------------------------
	for( ;; ) {
		GM_CSTR		token = pReader->getToken();

		//---- �I�[�`�F�b�N
		if( pReader->compare("}") ) {
			pReader->closeChunk();
			break;
		}

		//---- Frame�֐ߖ�	{���O}
		if( pReader->compare("{") ) {
			pReader->openChunk();
			GM_CSTR	jointName = pReader->getToken();

			pBone->setName(jointName);

			pReader->closeChunk();
			continue;
		}

		//---- �L�[�f�[�^ AnimationKey
		if( pReader->compare("AnimationKey") ) {
			pReader->openChunk();

			// �L�[�̎��
			s32		keyType  = atoi( pReader->getToken() );

			// �L�[�̌�
			s32		keyCount = atoi( pReader->getToken() );

			// �L�[�f�[�^�z��
			for( s32 i=0; i<keyCount; i++ ) {
				// �L�[�t���[���ԍ�
				s32		t = atoi( pReader->getToken() );
				_totalTime = max(_totalTime, t);

				//----------------------------------------------------------
				// �L�[�f�[�^��ǂݍ��ށiType�ɂ���ăT�C�Y�Ɠ��e�͈قȂ�j
				//----------------------------------------------------------
				s32		dataCount   = atoi( pReader->getToken() );		// �f�[�^��

				f32		data[16];	// ��肤��ő���͍s���16�v�f
				for( s32 d=0; d<dataCount; d++ ) {
					data[d] = (f32)atof( pReader->getToken() );
				}

				//---- �L�[�ɔ��f
				switch( keyType ) {
				case 0:		// ��]�N�H�[�^�j�I��	[Quaternion]
					// X�t�@�C���̃N�H�[�^�j�I����(w, x, y, z)�̏��ŕ���ł��邽�߁A���ёւ��ă��[�h����
					{
						Quaternion	rotation = Quaternion(data[1], data[2], data[3], data[0]).normalize();
						pBone->_keyRotations.push_back( AssetModelX::Animation::Key<Quaternion>(t, rotation) );
					}
					break;
				case 1:		// �X�P�[��			[Vector3]
					{
						Vector3	scale = Vector3(data[0], data[1], data[2]);
						pBone->_keyScales.push_back( AssetModelX::Animation::Key<Vector3>(t, scale) );
					}
					break;
				case 2:		// �ʒu				[Vector3]
					{
						Vector3	position = Vector3(data[0], data[1], data[2]);
						pBone->_keyPositions.push_back( AssetModelX::Animation::Key<Vector3>(t, position) );
					}
					break;
				case 4:		// �s��				[Matrix]
					{
						Matrix	m( *(Matrix*)data );
						
						// �s��́u�ʒu�E�X�P�[���E��]�v�̂R�v�f�ɕ���
						f32	sx = Vector3( m._m[0] ).length();
						f32	sy = Vector3( m._m[1] ).length();
						f32	sz = Vector3( m._m[2] ).length();
						Vector3	scale    = Vector3(sx, sy, sz);

						Vector3	position = m._m[3];

						m._m[0] = Vector4( Vector3(m._m[0]).normalize(), 0.0f );
						m._m[1] = Vector4( Vector3(m._m[1]).normalize(), 0.0f );
						m._m[2] = Vector4( Vector3(m._m[2]).normalize(), 0.0f );
						m._m[3] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

						Quaternion	rotation;
						bool	success = rotation.fromRotateMatrix(m);
						// �����Ŏ��s�����ꍇ�ɂ͉����������Ȃ��B
						// �L�[�f�[�^�ɂ͍ŏ�����f�t�H���g�l (�ʒu(0,0,0) �X�P�[��(0,0,0) ��]�Ȃ�)�������Ă���
						ASSERTMSG(success, "�s��ϊ����ʂ��s���ł��B");
						// �N�H�[�^�j�I���͐��K�����Ă���
						rotation = rotation.normalize();

						//---- �L�[�t���[���Ƃ��ĕۑ�
						pBone->_keyRotations.push_back( AssetModelX::Animation::Key<Quaternion>(t, rotation) );
						pBone->_keyScales   .push_back( AssetModelX::Animation::Key<Vector3   >(t, scale   ) );
						pBone->_keyPositions.push_back( AssetModelX::Animation::Key<Vector3   >(t, position) );
					}
					break;
				default:
					MessageBox(NULL, "���m�̃A�j���[�V�����L�[TYPE�ł��B", "�t�@�C����̓G���[", MB_OK);
					break;
				}
			}

			pReader->closeChunk();
			continue;
		}

		//---- �L�[��Ԑݒ� AnimationOptions
		if( pReader->compare("AnimationOptions") ) {
			pReader->openChunk();
				// ��P�p�����[�^ = ���s�[�g(0)/�񃊃s�[�g(1);
				token = pReader->getToken();
				bool	repeat = (token[0] == '0');

				// ��Q�p�����[�^ = �X�v���C�����(0)/���`���(1);
				token = pReader->getToken();
				bool	linear = (token[0] == '0');
			pReader->closeChunk();
			continue;
		}

		//---- ���g�p/���m�̃`�����N�͓ǂݎ̂Ă�
		pReader->skipChunk();
	}
	// �A�j���[�V�����`�����N�I��
	pReader->closeChunk();

	return true;
}


//=============================================================================
//	AssetModelX::Animation::Bone
//=============================================================================

//-----------------------------------------------------------------------------
//	��]���擾(��Ԃ���)
//-----------------------------------------------------------------------------
Quaternion AssetModelX::Animation::Bone::getRotation(f32 frameTime) const
{
	//-------------------------------------------------------------
	// ��ԑΏۂ̋�Ԃ�����
	//-------------------------------------------------------------
	const AssetModelX::Animation::Key<Quaternion>* 	pKeyOld = &_keyRotations[0];
	const AssetModelX::Animation::Key<Quaternion>* 	pKey    = &_keyRotations[0];

	for( u32 i=1; i<_keyRotations.size(); i++ ) {
		const AssetModelX::Animation::Key<Quaternion>*	p = &_keyRotations[i];

		pKeyOld = pKey;
		pKey    = p;

		if( p->_time > frameTime ) {
			break;
		}
	}

	//-------------------------------------------------------------
	//	��]�p�����N�H�[�^�j�I�����
	//-------------------------------------------------------------
	Quaternion	rotation;
	if( pKeyOld->_time <= frameTime && frameTime <= pKey->_time ) {
		// ��ԓ��̈ʒu��0.0�`1.0�Ńp�����[�^t�����߂�
		f32	range = (f32)(pKey->_time - pKeyOld->_time);
		f32	t = 0.0f;

		if( range > 0.0f ) {	// ���ꂪ0���Z�ɂȂ�̂�h�~
			t = (frameTime - (f32)pKeyOld->_time) / range;
		}
		rotation = Quaternion::slerp(pKeyOld->_data, pKey->_data, t);
	}
	else {
		// ��ԋ�ԂɂȂ��ꍇ�͕�ԂȂ�
		rotation = pKeyOld->_data;
	}

	return rotation;
}

//-----------------------------------------------------------------------------
//	�X�P�[�����擾(��Ԃ���)
//-----------------------------------------------------------------------------
Vector3 AssetModelX::Animation::Bone::getScale(f32 frameTime) const
{
	//-------------------------------------------------------------
	// ��ԑΏۂ̋�Ԃ�����
	//-------------------------------------------------------------
	const AssetModelX::Animation::Key<Vector3>* 	pKeyOld = &_keyScales[0];
	const AssetModelX::Animation::Key<Vector3>* 	pKey    = &_keyScales[0];

	for( u32 i=1; i<_keyScales.size(); i++ ) {
		const AssetModelX::Animation::Key<Vector3>*	p = &_keyScales[i];

		pKeyOld = pKey;
		pKey    = p;

		if( p->_time > frameTime ) {
			break;
		}
	}

	//-------------------------------------------------------------
	//	�ʒu����`��� (����ɍ��i���ɂ��邽�߂ɂ̓X�v���C����Ԃɂ���)
	//-------------------------------------------------------------
	Vector3	scale;
	if( pKeyOld->_time <= frameTime && frameTime <= pKey->_time ) {
		// ��ԓ��̈ʒu��0.0�`1.0�Ńp�����[�^t�����߂�
		f32	range = (f32)(pKey->_time - pKeyOld->_time);
		f32	t = 0.0f;

		if( range > 0.0f ) {	// ���ꂪ0���Z�ɂȂ�̂�h�~
			t = (frameTime - (f32)pKeyOld->_time) / range;
		}
		scale = pKeyOld->_data * (1.0f - t) + pKey->_data * t;
	}
	else {
		// ��ԋ�ԂɂȂ��ꍇ�͕�ԂȂ�
		scale = pKeyOld->_data;
	}

	return scale;
}

//-----------------------------------------------------------------------------
//	�ʒu���擾(��Ԃ���)
//-----------------------------------------------------------------------------
Vector3 AssetModelX::Animation::Bone::getPosition(f32 frameTime) const
{
	//-------------------------------------------------------------
	// ��ԑΏۂ̋�Ԃ�����
	//-------------------------------------------------------------
	const AssetModelX::Animation::Key<Vector3>* pKeyOld = &_keyPositions[0];
	const AssetModelX::Animation::Key<Vector3>* pKey    = &_keyPositions[0];

	for( u32 i=1; i<_keyPositions.size(); i++ ) {
		const AssetModelX::Animation::Key<Vector3>*	p = &_keyPositions[i];

		pKeyOld = pKey;
		pKey    = p;

		if( p->_time > frameTime ) {
			break;
		}
	}

	//-------------------------------------------------------------
	//	�ʒu����`��� (����ɍ��i���ɂ��邽�߂ɂ̓X�v���C����Ԃɂ���)
	//-------------------------------------------------------------
	Vector3	position;
	if( pKeyOld->_time <= frameTime && frameTime <= pKey->_time ) {
		// ��ԓ��̈ʒu��0.0�`1.0�Ńp�����[�^t�����߂�
		f32	range = (f32)(pKey->_time - pKeyOld->_time);
		f32	t = 0.0f;

		if( range > 0.0f ) {	// ���ꂪ0���Z�ɂȂ�̂�h�~
			t = (frameTime - (f32)pKeyOld->_time) / range;
		}
		position = pKeyOld->_data * (1.0f - t) + pKey->_data * t;
	}
	else {
		// ��ԋ�ԂɂȂ��ꍇ�͕�ԂȂ�
		position = pKeyOld->_data;
	}

	return position;
}

//=============================================================================
//	AssetModelX::Mesh	���b�V���f�[�^
//=============================================================================

//-----------------------------------------------------------------------------
//	�R���X�g���N�^
//-----------------------------------------------------------------------------
AssetModelX::Mesh::Mesh(AssetModelX* pParent)
: _pParent(pParent)					// �e�̃��f��
, _pVertexBuffer(0)					// ���_�o�b�t�@
, _pIndexBuffer (0)					// �C���f�b�N�X�o�b�t�@
, _skinWeightsPerVertexCountMax(0)	// ���b�V�����̒��_�ɉe������g�����X�t�H�[���̍ő吔�B
, _SkinWeightsPerFaceCountMax(0)	// �C�ӂ̖ʂ� 3 �̒��_�ɉe�������ӂ̃g�����X�t�H�[���̍ő吔�B
, _myFrameJointIndex(0)				// �����̑����t���[���ԍ�
, _isDynamic(false)					// �������ǂ���
{
}

//-----------------------------------------------------------------------------
//	�f�X�g���N�^
//-----------------------------------------------------------------------------
AssetModelX::Mesh::~Mesh(void)
{
	glDeleteBuffers(1, &_pVertexBuffer);
	glDeleteBuffers(1, &_pIndexBuffer);

	//---- �X�L���E�F�C�g�̉��
	for(vector<AssetModelX::SkinWeight*>::iterator itr = _skinWeights.begin(); itr != _skinWeights.end(); )
	{
		AssetModelX::SkinWeight* pSkinWeight = *itr;
		SAFE_DELETE(pSkinWeight);
		itr = _skinWeights.erase(itr);
	}
}

//---- �|���S���̃N�C�b�N�\�[�g�p�R�[���o�b�N�֐�
int	sortFunc(const void* p1, const void* p2)
{
	AssetModelX::Face*	pFace1 = (AssetModelX::Face*)p1;
	AssetModelX::Face*	pFace2 = (AssetModelX::Face*)p2;

	if(pFace1->_material == pFace2->_material) return 0;
	return (pFace1->_material < pFace2->_material) ? -1 : +1;
}

//-----------------------------------------------------------------------------
//!	�����t���O�ݒ�
//! ���������}�b�s���O����̂ŌĂԉ񐔂͏��Ȃ�����
//-----------------------------------------------------------------------------
void AssetModelX::Mesh::setDynamic(bool isDynamic)
{
	// �n���ꂽ�l�������Ȃ�ύX���Ȃ�
	if(_isDynamic == isDynamic ) return;
	_isDynamic = isDynamic;

	// GL_ARRAY_BUFFER��_pVertexbuffer�����蓖��
	glBindBuffer(GL_ARRAY_BUFFER, _pVertexBuffer);
	AssetModelX::GpuVertex*	pVertex = nullptr;
	u32*					pIndex  = nullptr;
	//---- ���_�o�b�t�@/�C���f�b�N�X�o�b�t�@���}�b�s���O���ď�������
	pVertex = (AssetModelX::GpuVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	// �����t���O�ɂ���ăE�F�C�g����
	f32 weight = _isDynamic ? 1.0f : 0.0f;
	
	s32		totalVertexCount = 0;				// �S���_��

	for( u32 f=0; f<_faces.size(); f++ ) {
		AssetModelX::Face&	face =	_faces[f];	
	
		for( s32 triangle=0; triangle<face._vertexCount - 2; triangle++ ) {
			AssetModelX::GpuVertex*	v = &pVertex[totalVertexCount];
			for( s32 vi=0; vi<3; vi++ ) {
				for( s32 skin=0; skin<4; skin++ ) {
					if( v[vi]._blendIndex[skin] == _myFrameJointIndex ){
						// �X�L�j���O�E�F�C�g
						v[vi]._blendWeight[skin] = weight;
					}
				}
			}
		}
		totalVertexCount += 3;
	}

	//---- �}�b�s���O����
	GLboolean resultArray = glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//-----------------------------------------------------------------------------
//	���f���f�[�^���œK��
//-----------------------------------------------------------------------------
bool AssetModelX::Mesh::optimize(void)
{
	//-------------------------------------------------------------
	// �X�L�j���O�E�F�C�g�����쐬�\�z
	//-------------------------------------------------------------
	for( u32 i=0; i<_skinWeights.size(); i++ ) {
		AssetModelX::SkinWeight*	pSkinWeight = _skinWeights[i];
		AssetModelX::Frame*			pFrame      = _pParent->searchFrame( pSkinWeight->getName() );

		if( pFrame == NULL ) {
			MessageBoxA(NULL, pSkinWeight->getName(), "�X�L���E�G�C�g�̃o�C���h�Ɏ��s���܂���.", MB_OK);
			return false;
		}

		pFrame->setOffsetMatrix( pSkinWeight->getOffsetMatrix() );

		for( u32 s=0; s<pSkinWeight->_data.size(); s++ ) {
			AssetModelX::SkinWeight::Data*	p = &pSkinWeight->_data[s];

			s32	index  = p->_vertexIndex;
			f32	weight = p->_weight;

			AssetModelX::Vertex	*	pVertex = &_vertices[index];
			pVertex->_skinFrame [3] = pVertex->_skinFrame [2];
			pVertex->_skinFrame [2] = pVertex->_skinFrame [1];
			pVertex->_skinFrame [1] = pVertex->_skinFrame [0];
			pVertex->_skinFrame [0] = pFrame;

			pVertex->_skinWeight[3] = pVertex->_skinWeight[2];
			pVertex->_skinWeight[2] = pVertex->_skinWeight[1];
			pVertex->_skinWeight[1] = pVertex->_skinWeight[0];
			pVertex->_skinWeight[0] = weight;
			
		}
	}

	//-------------------------------------------------------------
	// �L���Ȋ֐߂𒊏o
	//-------------------------------------------------------------
	for( u32 i=0; i<_skinWeights.size(); i++ ) {
		AssetModelX::SkinWeight*	pSkinWeight = _skinWeights[i];
		AssetModelX::Joint*			pJoint      = &_joints[i];
		AssetModelX::Frame*			pFrame      = _pParent->searchFrame( pSkinWeight->getName() );

		pJoint->_jointIndex      = pFrame->getJointIndex();
		pJoint->_parentIndex     = -1;
		pJoint->_pFrame          = pFrame;
		pJoint->_offsetMatrix    = pSkinWeight->getOffsetMatrix();
		pJoint->_transformMatrix = pFrame     ->getTransformMatrix();

		//---- �e�̊֐ߔԍ����擾
		AssetModelX::Frame*	pParentFrame = pFrame->getParent();
		if( pParentFrame ) {
			pJoint->_parentIndex = (s32)( pParentFrame->getJointIndex() );
		}
	}

	//-------------------------------------------------------------
	// �}�e���A���P�ʂŏ����ɕ��ёւ�
	//-------------------------------------------------------------
	qsort(&_faces[0], _faces.size(), sizeof(AssetModelX::Face), &sortFunc);
#if 0
	// �}�e���A���P�ʂŕ��ёւ��i���쌟�ؗp�o�u���\�[�g�B���ɒᑬ�j
	for( s32 face=getFaceCount()-1; face>=1; face-- ) {
		for( s32 i=0; i<face; i++ ) {
			AssetModelX::Face*	pFace1 = getFace(i);
			AssetModelX::Face*	pFace2 = getFace(i + 1);

			if( pFace1->_material > pFace2->_material ) {
				AssetModelX::Face	temp = *pFace1;
				*pFace1 = *pFace2;
				*pFace2 = temp;
			}
		}
	}
#endif
	//-------------------------------------------------------------
	// ���b�V���S�̂̃|���S�����𐔂���B
	// (�R�p�`�͂P�|���S���A�S�p�`�͂Q�|���S���Ƃ��Đ�����)
	//-------------------------------------------------------------
	s32	triangleCount = 0;
	for( u32 f=0; f<_faces.size(); f++ ) {
		// �R�p�`=1�A�S�p�`=2
		triangleCount += _faces[f]._vertexCount - 2;
	}

	//-------------------------------------------------------------
	// GPU���VRAM�ɒ��_�o�b�t�@���m��
	//-------------------------------------------------------------
	u32 vertexBufferSize = sizeof(AssetModelX::GpuVertex) * triangleCount * 3;
	u32	indexBufferSize = sizeof(u32) * triangleCount * 3;

	//---- ���_�o�b�t�@
	{
		glGenBuffers(1, &_pVertexBuffer);				// ���������m��
		glBindBuffer(GL_ARRAY_BUFFER, _pVertexBuffer);	// ���ꂩ��ARRAY_BUFFER�ɍs�������͂���ID�ɔ��f����B
		glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, NULL, GL_STATIC_DRAW);
		
	}
	{
		//---- �C���f�b�N�X�o�b�t�@
		glGenBuffers(1, &_pIndexBuffer);						// ���������m��
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _pIndexBuffer);	// ���ꂩ��ELEMENT_ARRAY_BUFFER�ɍs�������͂���ID�ɔ��f����B
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, NULL, GL_STATIC_DRAW);
		
	}



	//-------------------------------------------------------------
	// ���_�̃X�L�j���O�C���f�b�N�X�ԍ����E�F�C�g�����ɕ��ёւ��Ă���
	// (4�E�F�C�g�̕`���1�E�F�C�g�̕`�旼���ɑΉ��ł���悤�ɂ��邽��)
	//-------------------------------------------------------------
	for( u32 i=0; i<_vertices.size(); i++ ) {
		AssetModelX::Vertex&	v = _vertices[i];

		for( s32 n1=4-2; n1>=0; n1-- )
		for( s32 n2=4-2; n2>=0; n2-- ) {
			if( v._skinWeight[n2] >= v._skinWeight[n2 + 1] ) continue;	

			//---- �X�L���C���f�b�N�X���E�F�C�g�l���Z�b�g�Ō���
			f32					weight = v._skinWeight[n2];
			AssetModelX::Frame*	pFrame = v._skinFrame [n2];

			v._skinWeight[n2] = v._skinWeight[n2 + 1];
			v._skinFrame [n2] = v._skinFrame [n2 + 1];

			v._skinWeight[n2 + 1] = weight;
			v._skinFrame [n2 + 1] = pFrame;
		}	
	}

	//-------------------------------------------------------------
	// ���_�o�b�t�@/�C���f�b�N�X�o�b�t�@�Ƀf�[�^����������
	//-------------------------------------------------------------
	AssetModelX::GpuVertex*	pVertex = NULL;
	u32*					pIndex  = NULL;
	//---- ���_�o�b�t�@/�C���f�b�N�X�o�b�t�@�����C���������Ƀ}�b�s���O
	pVertex = (AssetModelX::GpuVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	pIndex  = (u32*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

	s32		totalVertexCount = 0;				// �S���_��
	s32		totalIndexCount  = 0;				// �S�C���f�b�N�X��
	s32*	pPrimCount       = NULL;			// �P�N���X�^�[���̃v���~�e�B�u��
	s32*	pVertexCount     = NULL;			// �@�@�@�V�@�@�@�@���_��

	AssetModelX::Cluster*	pCluster      = NULL;
	s32						materialIndex = -1;
	Vector4					diffuseColor;
	Vector4					specularColor;
	Vector4					emissiveColor;
	float					specularPower;

	for( u32 f=0; f<_faces.size(); f++ ) {
		AssetModelX::Face&	face =	_faces[f];	

		// �}�e���A���ԍ����`�F�b�N���ĕω����������ꍇ�ɂ����؂�ւ���
		s32	m = face._material;	// �}�e���A���ԍ�
		if( m != materialIndex ) {
			AssetModelX::Material*	pMaterial = _materials[m];

			// �}�e���A���X�V
			materialIndex = m;								// �}�e���A���ԍ�
			diffuseColor  = pMaterial->getDiffuseColor();	// �f�B�t���[�Y�F
			specularColor = pMaterial->getSpecularColor();	// �X�y�L�����F
			emissiveColor = pMaterial->getEmissiveColor();	// ���ȏƖ�
			specularPower = pMaterial->getShininess();		// �X�y�L�����p���[

			// �N���X�^�[��V�K�ǉ��쐬
			_clusters.push_back( AssetModelX::Cluster() );
			s32		  clusterIndex = _clusters.size() - 1;
			pCluster               = &_clusters[clusterIndex];
			pCluster->_pMaterial   = pMaterial;
			pCluster->_vertexStart = totalVertexCount;
			pCluster->_indexStart  = totalIndexCount;

			// ����ۑ�����ꏊ���o���Ă���
			pPrimCount   = &pCluster->_primCount;
			pVertexCount = &pCluster->_vertexCount;
		}

		for( s32 triangle=0; triangle<face._vertexCount - 2; triangle++ ) {
			const AssetModelX::Vertex*	pV[3];
			pV[0] = &_vertices[ face._vertexIndex[           0] ];
			pV[1] = &_vertices[ face._vertexIndex[triangle + 1] ];
			pV[2] = &_vertices[ face._vertexIndex[triangle + 2] ];

			Vector3	normal[3];
			if( _normals.size() > 0 ) {
				normal[0] = _normals[ face._normalIndex[           0] ];
				normal[1] = _normals[ face._normalIndex[triangle + 1] ];
				normal[2] = _normals[ face._normalIndex[triangle + 2] ];
			}else{
				normal[0] = Vector3(0,1,0);
				normal[1] = Vector3(0,1,0);
				normal[2] = Vector3(0,1,0);
			}
			
			AssetModelX::GpuVertex*	v = &pVertex[totalVertexCount];
			u32*					i = &pIndex [totalIndexCount];
			for( s32 vi=0; vi<3; vi++ ) {
				v[vi]._position		   = pV[vi]->_position;
				v[vi]._diffuseColor    = diffuseColor;
				v[vi]._specularColor   = specularColor;
				v[vi]._emissiveColor   = emissiveColor;
				v[vi]._shininess	   = specularPower;
				v[vi]._uv       = pV[vi]->_uv;
				v[vi]._normal   = normal[vi];

				for( s32 skin=0; skin<4; skin++ ) {
					//---- �X�L�j���O�C���f�b�N�X�ԍ�
					if( pV[vi]->_skinFrame[skin] ) {
						v[vi]._blendIndex [skin] = pV[vi]->_skinFrame [skin]->getJointIndex();
						// �X�L�j���O�E�F�C�g
						v[vi]._blendWeight[skin] = pV[vi]->_skinWeight[skin];
					}
					else {
						v[vi]._blendIndex [skin] = (f32)( _myFrameJointIndex );
						// �����I�u�W�F�N�g�Ȃ�weight 1.0f
						f32 weight = _isDynamic ? 1.0f : 0.0f;
						// �X�L�j���O�E�F�C�g
						v[vi]._blendWeight[skin] = weight;
					}
				}
			}

			i[0] = totalVertexCount + 0;
			i[1] = totalVertexCount + 1;
			i[2] = totalVertexCount + 2;

			totalVertexCount += 3;
			totalIndexCount  += 3;

			*pPrimCount      += 1;
			*pVertexCount    += 3;
		}
	}
		
	//---- �}�b�s���O����
	GLboolean resultElement = glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	GLboolean resultArray   = glUnmapBuffer(GL_ARRAY_BUFFER);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#if 1

	//-------------------------------------------------------------
	// �ʏ�`���VAO
	//-------------------------------------------------------------
	// VAO�쐬
	_VAO.genVAO();
	// VAO�o�^�J�n
	_VAO.beginRegistContext();
	// �ʏ�`��̃R���e�L�X�g�擾
	NormalContext NContext = GmShader()->getNormalContext();
	
	GmShader()->setAndEnableVertexAttributeWithHandle( NContext._positionLocation     , 3, GL_FLOAT	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _position		)) );
	GmShader()->setAndEnableVertexAttributeWithHandle( NContext._blendIndicesLocation , 4, GL_FLOAT	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _blendIndex	)) );
	GmShader()->setAndEnableVertexAttributeWithHandle( NContext._blendWeightLocation  , 4, GL_FLOAT	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _blendWeight	)) );
	GmShader()->setAndEnableVertexAttributeWithHandle( NContext._normalLocation		  , 3, GL_FLOAT	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _normal		)) );
	GmShader()->setAndEnableVertexAttributeWithHandle( NContext._uvLocation			  , 2, GL_FLOAT	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _uv			)) );
	GmShader()->setAndEnableVertexAttributeWithHandle( NContext._specularPowerLocation, 3, GL_FLOAT	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _shininess		)) );

	// VAO�o�^�I��
	_VAO.endRegistContext();

	//-------------------------------------------------------------
	// �V���h�E�`���VAO
	//-------------------------------------------------------------
	
	// VAO�쐬
	_shadowVAO.genVAO();
	// VAO�o�^�J�n
	_shadowVAO.beginRegistContext();
	// �V���h�E�`��̃R���e�L�X�g�擾
	ShadowContext SContext = GmShader()->getShadowContext();

	GmShader()->setAndEnableVertexAttributeWithHandle( SContext._positionLocation     , 3, GL_FLOAT	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _position		)) );
	GmShader()->setAndEnableVertexAttributeWithHandle( SContext._blendIndicesLocation , 4, GL_FLOAT	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _blendIndex	)) );
	GmShader()->setAndEnableVertexAttributeWithHandle( SContext._blendWeightLocation  , 4, GL_FLOAT	, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _blendWeight	)) );
	
	/*glVertexAttribPointer(SContext._positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _position	)) );
	glEnableVertexAttribArray(SContext._positionLocation);
	glVertexAttribPointer(SContext._blendWeightLocation, 4, GL_FLOAT, GL_FALSE, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _blendWeight	)) );
	glEnableVertexAttribArray(SContext._blendWeightLocation);
	glVertexAttribPointer(SContext._blendIndicesLocation, 4, GL_FLOAT, GL_FALSE, sizeof(AssetModelX::GpuVertex), (GLvoid*)(offsetof(AssetModelX::GpuVertex, _blendIndex	)) );
	glEnableVertexAttribArray(SContext._blendIndicesLocation);*/

	// VAO�o�^�I��
	_shadowVAO.endRegistContext();

#endif
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return true;
}

//-----------------------------------------------------------------------------
//	�X�L�j���O�w�b�_�̓ǂݍ���
//-----------------------------------------------------------------------------
bool  AssetModelX::Mesh::loadSkinMeshHeader(AssetModelX::Reader* pReader)
{
	GM_CSTR	token;

	pReader->openChunk();
	{
		token = pReader->getToken();
		_skinWeightsPerVertexCountMax = atoi(token);

		token = pReader->getToken();
		_SkinWeightsPerFaceCountMax   = atoi(token);

		//---- �֐߃{�[����
		token = pReader->getToken();
		s32	jointCount = atoi(token);
		_joints.resize(jointCount);
	}
	pReader->closeChunk();

	return true;
}

//-----------------------------------------------------------------------------
// �X�L�j���O�E�F�C�g��ǂݍ���
//-----------------------------------------------------------------------------
bool  AssetModelX::Mesh::loadSkinWeight(AssetModelX::Reader* pReader)
{
	GM_CSTR	token;

	pReader->openChunk();
	{
		//---- �V�K�쐬
		AssetModelX::SkinWeight*	pSkinWeight = new AssetModelX::SkinWeight();
		if( pSkinWeight == NULL ) {
			return false;
		}
		// �z��ɓo�^
		_skinWeights.push_back(pSkinWeight);

		//------------------------------------------------------------
		// �t���[����
		//------------------------------------------------------------
		token = pReader->getToken();
		GM_CHAR	name[MAX_PATH];
		strcpy(name, &token[1]);			// �擪�́u�h�v�L��������
		name[ strlen(name) - 1 ] = '\0';	// �����́u�h�v�L��������
		// �X�L���E�F�C�g�̖��O�Ƃ��Đݒ�
		pSkinWeight->setName(name);

		//------------------------------------------------------------
		// �E�F�C�g�z���
		//------------------------------------------------------------
		token = pReader->getToken();
		s32	count = atoi(token);

		pSkinWeight->_data.resize(count);

		//---- ���_�C���f�b�N�X
		for( s32 i=0; i<count; i++ ) {
			token = pReader->getToken();
			s32	index = atoi(token);

			pSkinWeight->_data[i]._vertexIndex = index;
		}
		//---- ���_�E�F�C�g�l
		for( s32 i=0; i<count; i++ ) {
			token = pReader->getToken();
			f32	weight = (f32)atof(token);

			pSkinWeight->_data[i]._weight = weight;
		}

		//------------------------------------------------------------
		// �I�t�Z�b�g�s��
		//------------------------------------------------------------
		Matrix	offsetMatrix;
		token = pReader->getToken();	offsetMatrix._m[0]._x =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[0]._y =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[0]._z =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[0]._w =  (f32)atof(token);

		token = pReader->getToken();	offsetMatrix._m[1]._x =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[1]._y =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[1]._z =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[1]._w =  (f32)atof(token);

		token = pReader->getToken();	offsetMatrix._m[2]._x =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[2]._y =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[2]._z =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[2]._w =  (f32)atof(token);

		token = pReader->getToken();	offsetMatrix._m[3]._x =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[3]._y =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[3]._z =  (f32)atof(token);
		token = pReader->getToken();	offsetMatrix._m[3]._w =  (f32)atof(token);

		pSkinWeight->setOffsetMatrix(offsetMatrix);
	}
	pReader->closeChunk();

	return true;
}


//=============================================================================
//	AssetModelX::Frame	���f���t���[��
//=============================================================================

//-----------------------------------------------------------------------------
//	�R���X�g���N�^
//-----------------------------------------------------------------------------
AssetModelX::Frame::Frame(void)
: _jointIndex	  (-1)					// �֐ߔԍ�
, _texRepeat	  ( 1)					// �e�N�X�`���̃��s�[�g��
, _offsetMatrix   (Matrix::IDENTITY)	// �I�t�Z�b�g�s��
, _transformMatrix(Matrix::IDENTITY)	// �֐ߎp���s��
, _rotationMatrix (Matrix::IDENTITY)	// ��]�s��
, _pParent		  (0)					// �e
, _pNext		  (0)					// �Z��
, _pPrev		  (0)					// �O
, _pChild		  (0)					// �q
, _isDynamic	  (false)				// �������ǂ���
, _isRender		  (true)				// �`��t���O
{}

//-----------------------------------------------------------------------------
//	�f�X�g���N�^
//-----------------------------------------------------------------------------
AssetModelX::Frame::~Frame(void)
{
	//---- ���b�V���̍폜
	for(vector<AssetModelX::Mesh*>::iterator itr = _meshes.begin(); itr != _meshes.end(); )
	{
		AssetModelX::Mesh* pMesh = *itr;
		SAFE_DELETE(pMesh);
		itr = _meshes.erase(itr);
	}
}

//-----------------------------------------------------------------------------
//	�q�ɒǉ�
//!	@param	[in]	p	�ǉ�����I�u�W�F�N�g
//-----------------------------------------------------------------------------
void AssetModelX::Frame::add(AssetModelX::Frame* p)
{
	if( _pChild ) {
		// �q�����łɓo�^����Ă���Ƃ��͖����ɒǉ�
		for(Frame* pChild=_pChild; ; pChild=pChild->getNext() ) {
			if( pChild->_pNext ) continue;

			pChild->_pNext = p;
			p     ->_pPrev = pChild;
			break;
		}
	}
	else {
		// �V�K�o�^
		_pChild   = p;
		p->_pPrev = this;
	}
	p->_pParent = this;
}

//-----------------------------------------------------------------------------
//! �I�t�Z�b�g��ݒ�
//-----------------------------------------------------------------------------
void AssetModelX::Frame::setOffset(const Vector3& vec)
{
	// �������ǂ����t���O�擾
	if( !_isDynamic ){
		// �����t���O�ݒ�
		setDynamic(true);
	}

	// �ړ��s��쐬
	Matrix	offsetMatrix = Matrix::translate(vec);
	// �ړ��s��ݒ�
	setOffsetMatrix(offsetMatrix);
}
//-----------------------------------------------------------------------------
//! �I�C���[�p��ݒ�
//-----------------------------------------------------------------------------
void AssetModelX::Frame::setRotate(const Vector3& rot)
{
	// �������ǂ����t���O�擾
	if( !_isDynamic ){
		// �����t���O�ݒ�
		setDynamic(true);
	}
	// �N�H�[�^�j�I���쐬
	Quaternion	rotateQuaternion(rot);
	// ��]�s��ɕϊ�
	Matrix	rotateMatrix = rotateQuaternion.toRotateMatrix();
	// ��]�s��ݒ�
	setRotateMatrix(rotateMatrix);
}

//-----------------------------------------------------------------------------
//	�I�t�Z�b�g�s���ݒ�
//-----------------------------------------------------------------------------
void AssetModelX::Frame::setOffsetMatrix(const Matrix& m)
{
	_offsetMatrix = m;
}

//-----------------------------------------------------------------------------
//	�p���s���ݒ�
//-----------------------------------------------------------------------------
void AssetModelX::Frame::setTransformMatrix(const Matrix& m)
{
	_transformMatrix = m;
}

//-----------------------------------------------------------------------------
//	��]�s���ݒ�
//-----------------------------------------------------------------------------
void AssetModelX::Frame::setRotateMatrix(const Matrix& m)
{
	_rotationMatrix = m;
}

//-----------------------------------------------------------------------------
//	�֐ߔԍ���ݒ�
//-----------------------------------------------------------------------------
void AssetModelX::Frame::setJointIndex(f32 index)
{
	_jointIndex = index;
}

//-----------------------------------------------------------------------------
//	�I�t�Z�b�g�s����擾
//-----------------------------------------------------------------------------
const Matrix& AssetModelX::Frame::getOffsetMatrix(void) const
{
	return _offsetMatrix;
}
	
//-----------------------------------------------------------------------------
//	�p���s����擾
//-----------------------------------------------------------------------------
const Matrix& AssetModelX::Frame::getTransformMatrix(void) const
{
	return _transformMatrix;
}

//-----------------------------------------------------------------------------
//	��]�s����擾
//-----------------------------------------------------------------------------
const Matrix& AssetModelX::Frame::getRotateMatrix(void) const
{
	return _rotationMatrix;
}

//-----------------------------------------------------------------------------
//	�֐ߔԍ����擾
//-----------------------------------------------------------------------------
f32 AssetModelX::Frame::getJointIndex(void) const
{
	return _jointIndex;
}

//=============================================================================
//	END OF FILE
//=============================================================================
