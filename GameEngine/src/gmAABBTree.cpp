//-----------------------------------------------------------------------------
//!
//!	@file	gmAABBTree.cpp
//!	@brief	AABB-Tree
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
//!	���C���̕`�� (Flat)
//!	@param	[in]	p1		���W1
//!	@param	[in]	p1		���W2
//!	@param	[in]	color	�J���[
//! TODO:���͂Ȃ��������`�悳��Ȃ��̂Ō������l����
//---------------------------------------------------------------------------
void	drawLineF(const Vector3& p1, const Vector3& p2, const Vector4& color)
{
	GmShader()->changeShader(SystemShader::SHADER_COLOR);

	// ���_�̍쐬
	const Vertex v[2] = {
		{ Float3( p1 ),  color },
		{ Float3( p2 ),  color },
	};

	static GLuint VB;
	static bool Initialized = false;
	if( !Initialized ) {
		glGenBuffers(1, &VB);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 2, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		Initialized = true;
	}


	// ���_�o�b�t�@�o�C���h
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	// �f�[�^�]��
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 2, v);

	GmShader()->setAndEnableVertexAttribute("position"		, 3, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, _position));
	GmShader()->setAndEnableVertexAttribute("diffuseColor"	, 4, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, _color));

	glDrawArrays(GL_LINES, 0, 2);

	GmShader()->DisableVertexAttribute("position");
	GmShader()->DisableVertexAttribute("diffuseColor");

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GmShader()->beginPrevShader();

}

//-----------------------------------------------------------------------------
//! AABB�`��
//-----------------------------------------------------------------------------
void drawAABB(const AABB& aabb, const Vector4& color, const Vector3& offset )
{
	const Vector3	p[] = {
		Vector3( aabb._min._x, aabb._max._y, aabb._min._z ) + offset,
		Vector3( aabb._max._x, aabb._max._y, aabb._min._z ) + offset,
		Vector3( aabb._max._x, aabb._min._y, aabb._min._z ) + offset,
		Vector3( aabb._min._x, aabb._min._y, aabb._min._z ) + offset,

		Vector3( aabb._min._x, aabb._max._y, aabb._max._z ) + offset,
		Vector3( aabb._max._x, aabb._max._y, aabb._max._z ) + offset,
		Vector3( aabb._max._x, aabb._min._y, aabb._max._z ) + offset,
		Vector3( aabb._min._x, aabb._min._y, aabb._max._z ) + offset
	};

	drawLineF(p[0], p[1], color);
	drawLineF(p[1], p[2], color);
	drawLineF(p[2], p[3], color);
	drawLineF(p[3], p[0], color);

	drawLineF(p[4], p[5], color);
	drawLineF(p[5], p[6], color);
	drawLineF(p[6], p[7], color);
	drawLineF(p[7], p[4], color);

	drawLineF(p[0], p[4], color);
	drawLineF(p[1], p[5], color);
	drawLineF(p[2], p[6], color);
	drawLineF(p[3], p[7], color);
}

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
AABBTree::AABBTree()
{
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
AABBTree::~AABBTree()
{
}


//-----------------------------------------------------------------------------
//! �c���[�\�z
//-----------------------------------------------------------------------------
bool AABBTree::build(vector<CollisionData>& colData)
{
	return _rootNode.build(colData);
}

//-----------------------------------------------------------------------------
//! �O�p�`���X�g����m�[�h�����\�z
//! @param	[in]	colData	�����蔻����̔z��f�[�^
//-----------------------------------------------------------------------------
bool AABBTree::Node::build(vector<CollisionData> colData)
{
	//-------------------------------------------------------------
	// (1) ���ׂẴ|���S�����������ăo�E���f�B���O�{�b�N�X���v�Z
	//-------------------------------------------------------------
	AABB&	aabb = _aabb;

	aabb.setEmpty();

	//for( s32 i=0; i<_triangles.size(); i++ ) 
	for( u32 i=0; i<colData.size(); ++i )
	{
		//Triangle&	triangle = _triangles[i];
		Triangle&	triangle = colData[i]._triangle;

		aabb.expand(triangle._position[0]);
		aabb.expand(triangle._position[1]);
		aabb.expand(triangle._position[2]);
	}

	// �f�[�^�̗ʎ擾
	_dataCount = colData.size();

	//---- �Ō��1���̎O�p�`�������ꍇ�͕��z��Ɗ����I
	//if( _triangles.size() <= 1 ) 
	//if( _colData.size() <= 1 ) 
	if( _dataCount <= 1 )
	{
		// �������m��
		_col = new CollisionData();
		// �f�[�^���i�[
		*_col = colData[0];
		return true;
	}

	//-------------------------------------------------------------
	// (2) �o�E���f�B���O�{�b�N�X�̊e���̒����ōł������������o
	//-------------------------------------------------------------
	s32		axis      = -1;
	f32		maxExtent = -FLT_MAX;

	for( s32 i=0; i<3; i++ ) {		// X, Y, Z
		f32	extent = aabb._max[i] - aabb._min[i];

		// ���̒�������Ԓ����ꍇ
		if( maxExtent < extent ) {
			axis      = i;
			maxExtent = extent;
		}
	}


	//-------------------------------------------------------------
	// (3) �I�������������̂ǂ�����ɕ������邩�����߂�
	//-------------------------------------------------------------
	f32		averagePosition = 0.0f;
	//for( s32 i=0; i<_triangles.size(); i++ ) {
	for( u32 i=0; i<colData.size(); i++ ) {
		//Triangle&	triangle = _triangles[i];
		Triangle&	triangle = colData[i]._triangle;

		averagePosition += triangle._position[0][axis];
		averagePosition += triangle._position[1][axis];
		averagePosition += triangle._position[2][axis];
	}
	//f32		splitPoint = averagePosition / (f32)(_triangles.size() * 3);	// ����
	f32		splitPoint = averagePosition / (f32)(colData.size() * 3);	// ����




	//-------------------------------------------------------------
	// (4) �q�m�[�h�Ɋ�_�����ƂɐU�蕪����
	//-------------------------------------------------------------
	_pChild[0] = std::unique_ptr<AABBTree::Node>( new AABBTree::Node() ); 
	_pChild[1] = std::unique_ptr<AABBTree::Node>( new AABBTree::Node() ); 
	if( _pChild[0].get() == nullptr ) return false;
	if( _pChild[1].get() == nullptr ) return false;

	// �ċA�Ăяo���̂��߂̈���
	vector<CollisionData> child[2];

	//for( s32 i=0; i<_triangles.size(); i++ ) {
	for( u32 i=0; i<colData.size(); i++ ) {
		//Triangle&	triangle = _triangles[i];
		CollisionData& col	 = colData[i];
		Triangle&	triangle = col._triangle;
		Vector3*	p        = &triangle._position[0];

		f32		minPosition = min(min(p[0][axis], p[1][axis]), p[2][axis]);
		f32		maxPosition = max(max(p[0][axis], p[1][axis]), p[2][axis]);
		f32		center = (minPosition + maxPosition) * 0.5f;

		

		if( center < splitPoint ) {
			//_pChild[0]->_triangles.push_back(triangle);
			//_pChild[0]->_colData.push_back(col);
			child[0].push_back(col);
		}
		else {
			//_pChild[1]->_triangles.push_back(triangle);
			//_pChild[1]->_colData.push_back(col);
			child[1].push_back(col);
		}
	}

	// �� ���� ��
	// �ǂ��炩�Е��ɕ΂�ꍇ�����邽�߁A���̏ꍇ�͋ϓ��ɔz�����Ȃ����B
	//if( _pChild[0]->_triangles.size() == 0 || _pChild[1]->_triangles.size() == 0 ) {
	//if( _pChild[0]->_colData.size() == 0 || _pChild[1]->_colData.size() == 0 ) {
	if( child[0].size() == 0 || child[1].size() == 0 ) {
		// �ϓ��ɔ������Ȃ���
		/*_pChild[0]->_triangles.resize(0);
		_pChild[1]->_triangles.resize(0);*/
		/*_pChild[0]->_colData.resize(0);
		_pChild[1]->_colData.resize(0);*/
		child[0].resize(0);
		child[1].resize(0);

		//for( s32 i=0; i<_triangles.size(); i++ ) {
		for( u32 i=0; i<colData.size(); i++ ) {

			//_pChild[i & 1]->_triangles.push_back( _triangles[i] );
			//_pChild[i & 1]->_colData.push_back( _colData[i] );
			child[i & 1].push_back( colData[i] );
		}
	}

	//-------------------------------------------------------------
	// �q���̃m�[�h���ċA�����i�������ăm�[�h�\�z�j
	//-------------------------------------------------------------
	bool	result;
	
	result = _pChild[0]->build(child[0]);
	if( result == false ) return false;
	
	result = _pChild[1]->build(child[1]);
	if( result == false ) return false;

	return true;
}

//-----------------------------------------------------------------------------
//! aabb�ƏՓ˃`�F�b�N
//!	@param	[in]	aabb	�����蔻����s��AABB
//!	@param	[in]	hitFunc	�O�p�`�Ƃ̏Փ˔���p�̊֐��|�C���^
//-----------------------------------------------------------------------------
bool AABBTree::Node::hitCheck(const AABB& aabb, bool(*hitFunc)(Triangle* tri))
{
	//drawAABB(_aabb, Vector4(1.0f, 0.0f, 0.0f, 1.0f));

	bool result = false;

	// �Ō�̃m�[�h�Ȃ�I��
	//if( _triangles.size() <= 1 ) {
	//if( _colData.size() <= 1 ) {
	if( _dataCount <= 1 ) {

		drawAABB(_aabb, Vector4(1.0f, 0.0f, 0.0f, 1.0f));

		//CollisionData& col	 = _colData[0];
		CollisionData& col	 = *_col;
		if( col._isActive )
		{
			//Triangle&	triangle = _triangles[0];
			Triangle&	triangle = col._triangle;

			//------------------------------------------------------------
			// �� ������triangle�Ƃ̏Փ˔�����쐬����
			//	�R�[���o�b�N�֐��Ŏ󂯎��悤�ɂ��邱�Ƃŗ��֐�������
			//------------------------------------------------------------
			result = (*hitFunc)(&triangle);


			return result;
		}else{
			return result;
		}
	}

	//bool	result = false;
	for( s32 i=0; i<2; i++ ) {
		if( AABBCollision::isHit(aabb, _pChild[i]->_aabb) == false ) continue;

		if( _pChild[i]->hitCheck(aabb, (*hitFunc)) ) {
			result = true;
		}
	}
	return result;
}

//-----------------------------------------------------------------------------
//! �f�o�b�O�`��
//!	@param	[in]	nest	�ǂ̐[����`�悷�邩
//-----------------------------------------------------------------------------
void AABBTree::Node::debugDraw(u32 nest)
{
	if( nest == 0 ) {
		// �f�o�b�O�`��
		// AABB
		//drawAABB(_aabb, Vector4(1.0f, 0.5f, 1.0f, 1.0f));
		//return;
	}
	// �Ō�̃m�[�h�Ȃ�I��
	//if( _triangles.size() <= 1 ) return;
	//if( _colData.size() <= 1 ) return;
	if( _dataCount <= 1 ){
		if( _col->_isActive ) {
			Vector4	color = Vector4(1.0f, 1.0f, 0.0f, 1.0f);

			Triangle&	triangle = _col->_triangle;

			drawLineF(triangle._position[0], triangle._position[1], color);
			drawLineF(triangle._position[1], triangle._position[2], color);
			drawLineF(triangle._position[2], triangle._position[0], color);
		}
		return;
	}


	nest--;
	_pChild[0]->debugDraw(nest);
	_pChild[1]->debugDraw(nest);
}





// 0.0f�`1.0f�̗���
#define FRAND()	((f32)rand() * (1.0f / 32767.0f))

//-----------------------------------------------------------------------------
//! �f�o�b�O�p�̏���
//-----------------------------------------------------------------------------
bool AABBTree::debugSetup()
{
	srand(400);

	vector<CollisionData> debugData;

	for( s32 group=0; group<10; group++ ) {
		Vector3		groupCenter;
		groupCenter._x = (FRAND() - 0.5f) * 2.0f * 10.0f;
		groupCenter._y =  FRAND()                *  2.0f;
		groupCenter._z = (FRAND() - 0.5f) * 2.0f * 10.0f;




		for( s32 t=0; t<50; t++ ) {
			Vector3		position[3];


			Vector3		center;
			center._x = FRAND() * 5.0f;
			center._y = FRAND() * 5.0f;
			center._z = FRAND() * 5.0f;


			for( s32 i=0; i<3; i++ ) {
				Vector3&	p = position[i];
	
				p._x = FRAND() * 1.0f;
				p._y = FRAND() * 1.0f;
				p._z = FRAND() * 1.0f;

				p += center + groupCenter;
			}

			Triangle	triangle(position[0], position[1], position[2]);
			CollisionData col;
			col._isActive = true;
			col._myFrameNumber = 0;
			col._triangle = triangle;
			

			//_rootNode._triangles.push_back(triangle);
			//_rootNode._colData.push_back(col);
			debugData.push_back(col);
		}

	}

	return build(debugData);
}



bool	debugColTriangleFunc(Triangle* tri)
{
	return true;
}


//-----------------------------------------------------------------------------
//! �f�o�b�O�`��
//-----------------------------------------------------------------------------
void AABBTree::debugRender()
{
	//for( s32 i=0; i<_rootNode._triangles.size(); i++ ) {
	//for( s32 i=0; i<_rootNode._colData.size(); i++ ) {

	//	CollisionData& col = _rootNode._colData[i];
	//	if( col._isActive ) {
	//		Vector4	color = Vector4(1.0f, 1.0f, 0.0f, 1.0f);

	//		//Triangle&	triangle = _rootNode._triangles[i];
	//		Triangle&	triangle = col._triangle;

	//		drawLineF(triangle._position[0], triangle._position[1], color);
	//		drawLineF(triangle._position[1], triangle._position[2], color);
	//		drawLineF(triangle._position[2], triangle._position[0], color);
	//	}
	//}

	drawAABB(_rootNode._aabb, Vector4(0.0f, 0.5f, 1.0f, 1.0f));


	// �q�m�[�h�̏�Ԃ�\��
#if 1
	static int count = 0;
	count++;

	u32	nest =  (count / (60*1) ) & 15;

	_rootNode.debugDraw(nest);
#endif


	static Vector3 position(0,0,0);

	
	AABB	aabb;
	aabb._min = position - Vector3(1, 1, 1);
	aabb._max = position + Vector3(1, 1, 1);
	drawAABB(aabb, Vector4(1.0f,1.0f,1.0f,1.0f));

	f32	speed = 0.2f;

	if( GetKeyState(VK_NUMPAD6) & 0x8000 ) {
		position._x += speed;
	}
	if( GetKeyState(VK_NUMPAD4) & 0x8000 ) {
		position._x -= speed;
	}

	if( GetKeyState(VK_NUMPAD8) & 0x8000 ) {
		position._z -= speed;
	}
	if( GetKeyState(VK_NUMPAD2) & 0x8000 ) {
		position._z += speed;
	}

	if( GetKeyState(VK_NUMPAD9) & 0x8000 ) {
		position._y += speed;
	}
	if( GetKeyState(VK_NUMPAD3) & 0x8000 ) {
		position._y -= speed;
	}


	_rootNode.hitCheck(aabb, (*debugColTriangleFunc));












}

//===========================================================================
// END OF FILE
//===========================================================================
