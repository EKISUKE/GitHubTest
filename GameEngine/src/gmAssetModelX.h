//-----------------------------------------------------------------------------
//!
//!	@file	gmAssetModelX.h
//!	@brief	3D���f���A�Z�b�g
//!	@author	Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	3D�A�Z�b�g���f���N���X
//-----------------------------------------------------------------------------
class AssetModelX : public AssetBase
{
public:
	//! �^���
	//GM_ASSET_TYPE(AssetModelX)

	class	Name;			//!< ���O������f�[�^
	struct	Vertex;			//!< ���_
	struct	GpuVertex;		//!< GPU���_�v�f
	struct	Face;			//!< �|���S��
	class	SkinWeight;		//!< �X�L���E�F�C�g
	struct	Joint;			//!< �{�[���֐�
	class	Reader;			//!< �\���p�[�T�[
	class	Material;		//!< �}�e���A��
	struct	Cluster;		//!< �`��O���[�v�N���X�^�[
	class	Mesh;			//!< ���b�V��
	class	Frame;			//!< ���f���t���[��
	class	Animation;		//!< �A�j���[�V����


	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	AssetModelX(void);
	//! �f�X�g���N�^
	virtual ~AssetModelX(void);

	//@}

	//!	���[�h
	//!	@param	[in]	fileName	�t�@�C����
	virtual bool					load(GM_CSTR fileName);
	//virtual bool					load(const c8* fileName);

	//!	�t���[���𖼑O�Ō���
	//!	@param	[in]	name		�t���[����
	AssetModelX::Frame*				searchFrame(GM_CSTR name);
	
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{


	//! �t���[�����ړ�������
	//!	@param	[in]	frameName	�K�p����t���[����
	//!	@param	[in]	offset		�ړ���
	bool							setOffsetByName(GM_CSTR frameName, Vector3& offset);

	//! �t���[������]������
	//!	@param	[in]	frameName	�K�p����t���[����
	//!	@param	[in]	rotate		��]��(Radian)
	bool							setRotateByName(GM_CSTR frameName, Vector3& rotate);

	//! �t���[�����������ǂ����ݒ�
	//!	@param	[in]	frameName	�K�p����t���[����
	//!	@param	[in]	isDynamic	�������ǂ����t���O
	bool							setDynamicByName(GM_CSTR frameName, bool& isDynamic);

	//! �ő�ŏ��l�ݒ�
	void							setMinMax(Float2 x, Float2 y, Float2 z){
		_minX = x._x;
		_maxX = x._y;
		_minY = y._x;
		_maxY = y._y;
		_minZ = z._x;
		_maxZ = z._y;
	}

	//! X�̍ŏ��l�擾
	f32								getMinX() const { return _minX; }
	//! X�̍ő�l�擾
	f32								getMaxX() const { return _maxX; }
	//! Y�̍ŏ��l�擾
	f32								getMinY() const { return _minY; }
	//! Y�̍ő�l�擾
	f32								getMaxY() const { return _maxY; }
	//! Z�̍ŏ��l�擾
	f32								getMinZ() const { return _minZ; }
	//! Z�̍ő�l�擾
	f32								getMaxZ() const { return _maxZ; }
	//! ���I�t���[���������Ă��邩�ǂ���
	bool							haveDynamicFrame();
	//@}

public:
	//! �t���[���̃|�C���^���X�g
	//Array<AssetModelX::Frame*>		_frames;
	//Array<AssetModelX::Material*>	_materials;
	//Array<AssetModelX::Animation*>	_animations;		//!< �A�j���[�V����
	vector<AssetModelX::Frame*>		_frames;
	vector<AssetModelX::Material*>	_materials;
	vector<AssetModelX::Animation*>	_animations;		//!< �A�j���[�V����

private:
	//! �t���[���̓ǂݍ���
	AssetModelX::Frame*				_loadFrame(void);
	//! ���b�V���̓ǂݍ���
	AssetModelX::Mesh*				_loadMesh(void);
	//! �}�e���A���ǂݍ���
	AssetModelX::Material*		 	_loadMaterial(void);
	//! �A�j���[�V�����ǂݍ���
	AssetModelX::Animation*		 	_loadAnimation(void);
private:
	AssetModelX::Reader*			_pReader;			//!< �\���p�[�T�[
	GM_CHAR							_path[MAX_PATH];	//!< ���f���̃t�@�C���p�X��
	//c8								_path[MAX_PATH];	//!< ���f���̃t�@�C���p�X��
	f32								_minX;
	f32								_maxX;
	f32								_minY;
	f32								_maxY;
	f32								_minZ;
	f32								_maxZ;

	//IDirect3DVertexDeclaration9*	_pD3DVertexDecl;	//!< ���_���̓��C�A�E�g
};

//-----------------------------------------------------------------------------
//!	���O������f�[�^
//-----------------------------------------------------------------------------
class AssetModelX::Name
{
public:
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	Name(GM_CSTR name="");

	//@}
	//-------------------------------------------------------------
	//! @name �擾/�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �������ݒ�
	void							setName(GM_CSTR name);
	
	//! ��������擾
	GM_CSTR							getName(void) const;
	
	//@}
private:
	GM_CHAR							_name[MAX_PATH];		//!< ���O
};

//-----------------------------------------------------------------------------
//!	���_�f�[�^
//-----------------------------------------------------------------------------
struct AssetModelX::Vertex {
	// �X�L�j���O���̍ő��
	static const s32	SKIN_WEIGHT_COUNT_MAX = 4;

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	Vertex(void)
	//: _position(Vector3::ZERO)
	: _position(0.0f, 0.0f, 0.0f)
	{
		for( s32 i=0; i<SKIN_WEIGHT_COUNT_MAX; i++ ) {
			_skinFrame [i] = NULL;
			_skinWeight[i] = 0.0f;
		}
	}

	//@}

	//---- �����o�[�ϐ�
	Vector3				_position;							//!< ���W
	Float2				_uv;								//!< UV

	Frame*				_skinFrame [SKIN_WEIGHT_COUNT_MAX];	//!< �X�L���t���[��
	f32					_skinWeight[SKIN_WEIGHT_COUNT_MAX];	//!< �X�L���E�F�C�g
};

//-----------------------------------------------------------------------------
//!	���_�f�[�^
//-----------------------------------------------------------------------------
struct AssetModelX::GpuVertex {
	Float3		_position;			//!< �ʒu���W�@       [f32 x 3] 12 bytes
	//u8			_blendIndex [4];	//!< �X�L�j���OINDEX  [ u8 x 4]  4 bytes
	f32			_blendIndex [4];	//!< �X�L�j���OINDEX  [f32 x 4] 16 bytes
	f32			_blendWeight[4];	//!< �X�L�j���OWEIGHT [f32 x 4] 16 bytes
	Float3		_normal;			//!< �@���@�@�@       [f32 x 3] 12 bytes
	Vector4		_diffuseColor;		//!< �f�t���[�Y�F (diffuse)
	Vector4		_specularColor;		//!< �X�y�L�����F(specular)
	Vector4		_emissiveColor;		//!< ���ȏƖ��F(emissive)
	float		_shininess;			//!< �p���[�l(shininess)
	Float2		_uv;				//!< �e�N�X�`��UV     [f32 x 2]  8 bytes
};

//-----------------------------------------------------------------------------
//!	�|���S���f�[�^
//-----------------------------------------------------------------------------
struct AssetModelX::Face {
	static const u32	VERTEX_COUNT_MAX = 4;
	s32					_material;
	s32					_vertexCount;
	s32					_vertexIndex[VERTEX_COUNT_MAX];
	s32					_normalIndex[VERTEX_COUNT_MAX];

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	// �R���X�g���N�^
	Face(void)
	: _material(-1)
	, _vertexCount(0)
	{
		for( u32 i=0; i<VERTEX_COUNT_MAX; i++ ) {
			_vertexIndex[i] = 0;
			_normalIndex[i] = 0;
		}
	}

	//@}
};

//-----------------------------------------------------------------------------
//!	�X�L���E�F�C�g
//-----------------------------------------------------------------------------
class AssetModelX::SkinWeight : public AssetModelX::Name
{
public:
	//---- ���_�E�F�C�g�f�[�^
	struct Data {
		s32				_vertexIndex;		//!< ���_�ԍ�
		f32				_weight;			//!< �E�F�C�g�l
	};

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	SkinWeight(void);
	//! �f�X�g���N�^
	virtual ~SkinWeight(void);

	//@}
	//-------------------------------------------------------------
	//! @name �擾/�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �I�t�Z�b�g�s���ݒ�
	void				setOffsetMatrix(const Matrix& m) { _offsetMatrix = m; }
	//! �I�t�Z�b�g�s����擾
	const Matrix&		getOffsetMatrix(void) const { return _offsetMatrix; }

	//@}

public:
	vector<AssetModelX::SkinWeight::Data>	_data;
private:
	Matrix				_offsetMatrix;		//! �I�t�Z�b�g�ړ���
};

//-----------------------------------------------------------------------------
//!	�{�[���֐�
//-----------------------------------------------------------------------------
struct AssetModelX::Joint {

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	Joint(void)
	: _jointIndex(-1)
	,_parentIndex(-1)
	, _pFrame(NULL)							// �Ή�����t���[��
	, _offsetMatrix   (Matrix::IDENTITY)	// �{�[���̋�ԂɃ��b�V���̒��_��ϊ�����s��i�{�[���̕ϊ��s��ƘA������ƃ{�[���̉e�����󂯂郁�b�V���̃��[���h��ԍ��W��������j
	, _transformMatrix(Matrix::IDENTITY)
	{
	}

	//@}

	//s32					_jointIndex;		//!< �֐ߔԍ�
	f32					_jointIndex;		//!< �֐ߔԍ�
	s32					_parentIndex;		//!< �e�̊֐ߔԍ�
	AssetModelX::Frame*	_pFrame;			//!< �Ή�����t���[��

	Matrix				_offsetMatrix;		//!< �{�[���̋�ԂɃ��b�V���̒��_��ϊ�����s��i�{�[���̕ϊ��s��ƘA������ƃ{�[���̉e�����󂯂郁�b�V���̃��[���h��ԍ��W��������j
	Matrix				_transformMatrix;	//!< �{�[����Ԃ���̑��Ύp��
};

//-----------------------------------------------------------------------------
//!	�\���p�[�T�[
//-----------------------------------------------------------------------------
class AssetModelX::Reader
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	Reader(void);
	//! �f�X�g���N�^
	virtual ~Reader(void);

	//! ������
	bool				init(GM_CSTR fileName);
	//! ���
	void				cleanup(void);

	//@}
	//-------------------------------------------------------------
	//! @name �擾/�ݒ�
	//-------------------------------------------------------------
	//@{

	//! ���̃g�[�N�����P�擾
	GM_CSTR				getToken(void);

	//@}

	//! ���݂̃g�[�N���ƕ������r
	bool				compare(GM_CSTR token);
	//! �`�����N���X�L�b�v
	bool				skipChunk(void);
	//! �`�����N���J��
	bool				openChunk(void);
	//! �`�����N�����
	bool				closeChunk(void);

	//! ���̃g�[�N���̏o���܂Ői�߂�
	bool				searchNextToken(GM_CSTR token);


private:
	static const s32	TOKEN_LENGTH_MAX = 512;
	GM_CHAR				_token[TOKEN_LENGTH_MAX];

	GM_STR				_pCurrent;

	GM_STR				_pBuffer;
	s32					_bufferSize;
};

//-----------------------------------------------------------------------------
//!	�}�e���A���f�[�^
//-----------------------------------------------------------------------------
class AssetModelX::Material : public AssetModelX::Name, public RefCounter
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	Material(void);

	//@}
	//-------------------------------------------------------------
	//! @name �擾/�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �f�B�t���[�Y�F���擾
	Vector4				getDiffuseColor(void) const;
	Vector4				getSpecularColor(void) const;
	Vector4				getEmissiveColor(void) const;
	float				getShininess(void) const;
	//! �e�N�X�`�����擾
	//AssetTexture*		getTexture(void) const;
	Texture*			getTexture(void) const;
	Texture*			getSpecularMap(void) const;
	Texture*			getNormalMap(void) const;

	//! �f�B�t���[�Y�F��ݒ�
	void				setDiffuseColor (Vector4 color);
	void				setSpecularColor(Vector4 color);
	void				setEmissiveColor(Vector4 color);
	void				setShininess(f32 shininess);
	//! �e�N�X�`����ݒ�
	//void				setTexture(AssetTexture* pTexture);
	void				setTexture(Texture* pTexture);
	void				setSpecularMap(Texture* pSpecularMap);
	void				setNormalMap(Texture* pNormalMap);

	//@}

	//! �V�F�\�_�̃e�N�X�`����ݒ�
	void				BindTextures();
	//! �V�F�\�_�̃e�N�X�`���̐ݒ����
	void				UnBindTextures();

private:
	//! �f�X�g���N�^ (���ڌĂяo�����֎~����)
	~Material(void);
private:
	//AssetTexture*		_pTexture;		//!< �e�N�X�`��
	Vector4				_diffuseColor;	//!< �f�t���[�Y�F (diffuse)
	Vector4				_specularColor;	//!< �X�y�L�����F(specular)
	Vector4				_emissiveColor; //!< ���ȏƖ��F(emissive)
	float				_shininess;		//!< �p���[�l(shininess)
	Texture*			_pTexture;		//!< �e�N�X�`��
	Texture*			_pSpecularMap;	//!< �X�y�L�����}�b�v
	Texture*			_pNormalMap;	//!< �m�[�}���}�b�v
};

//-----------------------------------------------------------------------------
//!	�`��O���[�v�N���X�^�[
//-----------------------------------------------------------------------------
struct AssetModelX::Cluster {

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	// �R���X�g���N�^
	Cluster(void)
	: _pMaterial(NULL)
	, _vertexStart(NULL)
	, _vertexCount(NULL)
	, _indexStart(NULL)
	, _primCount(NULL)
	{}

	//@}

	AssetModelX::Material*	_pMaterial;			//!< �֘A�t�����Ă���}�e���A��
	s32						_vertexStart;
	s32						_vertexCount;
	s32						_indexStart;
	s32						_primCount;			//!< �v���~�e�B�u���i�|���S�����j
};


//-----------------------------------------------------------------------------
//!	�A�j���[�V����
//-----------------------------------------------------------------------------
class AssetModelX::Animation : public AssetModelX::Name
{
public:
	template <typename T> class Key;	// �A�j���[�V�����L�[
	class Bone;							// �{�[��

	//-------------------------------------------------------------
	//!	@name	������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	Animation(void)
	: _totalTime(0)
	, _isRepeat(false)
	{}

	//!	�f�X�g���N�^
	virtual ~Animation(void);

	//@}
	//-------------------------------------------------------------
	//!	@name	�擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! ���Đ����Ԃ��擾
	s32						getTotalTime(void) const { return _totalTime; }
	//! ���s�[�g�Đ����ǂ������擾
	bool					isRepeat(void) const { return _isRepeat; }

	//! ���s�[�g�Đ��t���O��ݒ�
	void					setRepeat(bool repeat) { _isRepeat = repeat; }

	//@}
public:
	//! �{�[��/�L�[�t���[������ǂݍ���
	bool					_loadKeyData(AssetModelX::Reader* pReader);
public:
	s32										_totalTime;		//!< ���Đ�����
	bool									_isRepeat;		//!< true:���s�[�g�Đ� false:�P�ƍĐ�
	//Array<AssetModelX::Animation::Bone*>	_bone;
	vector<AssetModelX::Animation::Bone*>	_bone;
};

//-----------------------------------------------------------------------------
//! �L�[�t���[�����i�e���v���[�g�j
//-----------------------------------------------------------------------------
template <typename T>
class AssetModelX::Animation::Key
{
public:
	//-------------------------------------------------------------
	//!	@name	������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	Key(void) : _time(0) {}
	//! �R���X�g���N�^
	Key(s32 t, T data) : _time(t), _data(data) {}

	//@}
public:
	s32						_time;		//!< �L�[����
	T						_data;		//!< �L�[�f�[�^(�^�Ɉˑ�)
};

//-----------------------------------------------------------------------------
//!	�{�[�����(�e�{�[�����ɃL�[�z��)
//-----------------------------------------------------------------------------
class AssetModelX::Animation::Bone : public AssetModelX::Name
{
public:
	//-------------------------------------------------------------
	//!	@name	������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	Bone(void)
	: _jointIndex(-1)
	{}

	//@}
	//-------------------------------------------------------------
	//!	@name	�擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! ��]���擾		�i��Ԃ���j
	Quaternion				getRotation(f32 frameTime) const;
	//! �X�P�[�����擾	�i��Ԃ���j
	Vector3					getScale   (f32 frameTime) const;
	//! �ʒu���擾		�i��Ԃ���j
	Vector3					getPosition(f32 frameTime) const;

	//@}
public:
	s32													_jointIndex;	//!< �Ώۂ̊֐ߔԍ�
	//Array< AssetModelX::Animation::Key<Quaternion> >	_keyRotations;	//!< ��]
	//Array< AssetModelX::Animation::Key<Vector3>    >	_keyScales;		//!< �X�P�[��
	//Array< AssetModelX::Animation::Key<Vector3>    >	_keyPositions;	//!< �ʒu
	vector< AssetModelX::Animation::Key<Quaternion> >	_keyRotations;	//!< ��]
	vector< AssetModelX::Animation::Key<Vector3>    >	_keyScales;		//!< �X�P�[��
	vector< AssetModelX::Animation::Key<Vector3>    >	_keyPositions;	//!< �ʒu
};

//-----------------------------------------------------------------------------
//!	���b�V���f�[�^
//-----------------------------------------------------------------------------
class AssetModelX::Mesh
{
public:
	//-------------------------------------------------------------
	// �|���S�����
	//-------------------------------------------------------------
	//@{

	//Array<AssetModelX::Vertex>		_vertices;		//!< ���_
	//Array<Vector3>					_normals;		//!< �@��
	//Array<AssetModelX::Face>		_faces;			//!< �|���S��
	//Array<AssetModelX::Cluster>		_clusters;		//!< �`��O���[�v�N���X�^�[
	//Array<AssetModelX::Joint>		_joints;		//!< �֐�
	//Array<AssetModelX::Material*>	_materials;		//!< �ގ�
	//Array<AssetModelX::SkinWeight*>	_skinWeights;	//!< �X�L�j���O�E�F�C�g
	vector<AssetModelX::Vertex>			_vertices;		//!< ���_
	vector<Vector3>						_normals;		//!< �@��
	vector<AssetModelX::Face>			_faces;			//!< �|���S��
	vector<AssetModelX::Cluster>		_clusters;		//!< �`��O���[�v�N���X�^�[
	vector<AssetModelX::Joint>			_joints;		//!< �֐�
	vector<AssetModelX::Material*>		_materials;		//!< �ގ�
	vector<AssetModelX::SkinWeight*>	_skinWeights;	//!< �X�L�j���O�E�F�C�g

	//@}
	//-------------------------------------------------------------
	//!	@name	������
	//-------------------------------------------------------------
	//@{
public:
	//! �R���X�g���N�^
	Mesh(AssetModelX* pParent=NULL);
	//! �f�X�g���N�^
	virtual ~Mesh(void);

	//@}
	//-------------------------------------------------------------
	//!	@name	�擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �X�L�j���O���f�����ǂ���
	bool					isSkined       (void) const { return _skinWeights.size() > 0; }
	//! �W���C���g�ԍ��ݒ�
	void					setJointIndex(s32 index)	{ _myFrameJointIndex = index; }
	//!	�����t���O�ݒ�
	//! ���������}�b�s���O����̂ŌĂԉ񐔂͏��Ȃ�����
	void					setDynamic(bool isDynamic);
	//! ���_�o�b�t�@���擾
	GLuint					getVertexBuffer(void) const { return _pVertexBuffer; }
	//! �C���f�b�N�X�o�b�t�@���擾
	GLuint					getIndexBuffer (void) const { return _pIndexBuffer; }
	//! ���_���̓��C�A�E�g�擾
	VertexArrayObject		getVertexArrayObject(bool isShadow) const { return isShadow ? _shadowVAO : _VAO; }

	//! �����̃W���C���g�ԍ��擾
	s32						getJointIndex  (void) const { return _myFrameJointIndex; }

	//@}
	//-------------------------------------------------------------
	//!	@name	���b�V���Ǘ�
	//-------------------------------------------------------------
	//@{

	//! ���f���f�[�^���œK��
	bool					optimize(void);

	//@}

	//! �X�L�j���O�w�b�_�̓ǂݍ���
	bool					loadSkinMeshHeader(AssetModelX::Reader* pReader);
	// �X�L�j���O�E�F�C�g��ǂݍ���
	bool					loadSkinWeight(AssetModelX::Reader* pReader);

private:
	//-------------------------------------------------------------
	// �A�j���[�V�������
	//-------------------------------------------------------------
	// ���ϐ��̐�����MSDN��蔲��
	
	s32						_skinWeightsPerVertexCountMax;	// ���b�V�����̒��_�ɉe������g�����X�t�H�[���̍ő吔�B
	s32						_SkinWeightsPerFaceCountMax;	// �C�ӂ̖ʂ� 3 �̒��_�ɉe�������ӂ̃g�����X�t�H�[���̍ő吔�B

	s32						_myFrameJointIndex;				//!< �����̃t���[���̃W���C���g�ԍ�
	bool					_isDynamic;						//!< �������ǂ���
	
	VertexArrayObject		_VAO;							//!< ���_���̓��C�A�E�g
	VertexArrayObject		_shadowVAO;						//!< �V���h�E�̎��̃��C�A�E�g
	GLuint					_pVertexBuffer;					//!< ���_�o�b�t�@
	GLuint					_pIndexBuffer;					//!< �C���f�b�N�X�o�b�t�@
	//---- �e�̃��f��
	AssetModelX*			_pParent;
};

//-----------------------------------------------------------------------------
//!	���f���t���[��
//-----------------------------------------------------------------------------
class AssetModelX::Frame : public AssetModelX::Name
{
public:
	//-------------------------------------------------------------
	//!	@name	������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	Frame(void);
	//!	�f�X�g���N�^
	virtual ~Frame(void);

	//@}
	//-------------------------------------------------------------
	//!	@name	�K�w�\��
	//-------------------------------------------------------------
	//@{

	//! �q�ɒǉ�
	void						add(AssetModelX::Frame* p);
	//! �e���擾
	AssetModelX::Frame*			getParent(void) const { return _pParent; }
	//! ���̌Z����擾
	AssetModelX::Frame*			getNext  (void) const { return _pNext;   }
	//! �q���擾
	AssetModelX::Frame*			getChild (void) const { return _pChild;  }
	//! �O���擾
	AssetModelX::Frame*			getPrev  (void) const { return _pPrev;   }
	
	//@}
	//-------------------------------------------------------------
	//!	@name	�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �e�N�X�`���̃��s�[�g���ݒ�
	void						setTexRepeat(s32 repeat) { _texRepeat = repeat; }
	//! �I�t�Z�b�g��ݒ�
	void						setOffset(const Vector3& vec);
	//! �I�C���[�p��ݒ�
	void						setRotate(const Vector3& rot);

	//! �I�t�Z�b�g�s���ݒ�
	void						setOffsetMatrix(const Matrix& m);
	//! �p���s���ݒ�
	void						setTransformMatrix(const Matrix& m);
	//! ��]�s���ݒ�
	void						setRotateMatrix(const Matrix& m);
	//! �֐ߔԍ���ݒ�
	//void						setJointIndex(s32 index);
	void						setJointIndex(f32 index);
	//!	�����t���O�ݒ�
	void						setDynamic(bool isDynamic)  {
		_isDynamic = isDynamic; 
		for( u32 i=0; i < _meshes.size(); ++i )	{
			_meshes.at(i)->setDynamic(_isDynamic);
		}
	}
	// �`��t���O�ݒ�
	void						setRender(bool	isRender) {
		_isRender = isRender;
	}

	//! �ő�ŏ��l�ݒ�
	void						setMinMax(Float2 x, Float2 y, Float2 z){
		_minX = x._x;
		_maxX = x._y;
		_minY = y._x;
		_maxY = y._y;
		_minZ = z._x;
		_maxZ = z._y;
	}


	//@}
	//-------------------------------------------------------------
	//!	@name	�擾
	//-------------------------------------------------------------
	//@{

	//! �������ǂ���
	bool						isDynamic	   (void) const { return _isDynamic; }
	//! �`�悷�邩�ǂ���
	bool						isRender	   (void) const { return _isRender; }
	//! �I�t�Z�b�g�s����擾
	const Matrix&				getOffsetMatrix(void) const;
	//! �p���s����擾
	const Matrix&				getTransformMatrix(void) const;
	//!	��]�s����擾
	const Matrix&				getRotateMatrix(void) const;
	//! �֐ߔԍ����擾
	//s32							getJointIndex(void) const;
	f32							getJointIndex(void) const;
	//! �e�N�X�`���̃��s�[�g���擾
	s32							getTexRepeat(void) const { return _texRepeat; }
	//! X�̍ŏ��l�擾
	f32							getMinX() const { return _minX; }
	//! X�̍ő�l�擾
	f32							getMaxX() const { return _maxX; }
	//! Y�̍ŏ��l�擾
	f32							getMinY() const { return _minY; }
	//! Y�̍ő�l�擾
	f32							getMaxY() const { return _maxY; }
	//! Z�̍ŏ��l�擾
	f32							getMinZ() const { return _minZ; }
	//! Z�̍ő�l�擾
	f32							getMaxZ() const { return _maxZ; }


	//@}
public:
	//---- �W�I���g���֘A
	//Array<AssetModelX::Mesh*>	_meshes;			//!< ���b�V���f�[�^(���f���f�[�^)
	vector<AssetModelX::Mesh*>	_meshes;			//!< ���b�V���f�[�^(���f���f�[�^)
private:
	//---- �A�j���[�V�����֘A
	//s32							_jointIndex;		//!< �֐ߔԍ�
	f32							_jointIndex;		//!< �֐ߔԍ�
	s32							_texRepeat;			//!< �e�N�X�`���̃��s�[�g��
	Matrix						_offsetMatrix;		//!< �I�t�Z�b�g�ړ��s��
	Matrix						_transformMatrix;	//!< �֐ߎp���s��
	Matrix						_rotationMatrix;	//!< ��]�s��

	bool						_isDynamic;			//!< �������ǂ���
	bool						_isRender;			//!< �`�悷�邩�ǂ���

	//---- �t���[���m�[�h�̏��
	AssetModelX::Frame*			_pParent;			//!< �e	(AssetModelX::Frame)	
	AssetModelX::Frame*			_pNext;				//!< ��	(AssetModelX::Frame)
	AssetModelX::Frame*			_pPrev;				//!< �O	(AssetModelX::Frame)
	AssetModelX::Frame*			_pChild;			//!< �q	(AssetModelX::Frame)

	// �ő�ŏ�
	f32							_minX;
	f32							_maxX;
	f32							_minY;
	f32							_maxY;
	f32							_minZ;
	f32							_maxZ;
};

//=============================================================================
//	END OF FILE
//=============================================================================
