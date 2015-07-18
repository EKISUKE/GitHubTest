//-----------------------------------------------------------------------------
//!
//!	@file	gmCollisionManager.cpp
//!	@brief	Õ“Ë”»’èŠÇ—
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//! “o˜^
//!	@param	[in]	regist	“o˜^‚·‚é“–‚½‚è”»’èî•ñ
//-----------------------------------------------------------------------------
void CollisionManager::Regist(Collision* regist)
{
	if( _nowIndex > LIST_MAX ) return;
	if( _collisionList[_nowIndex] == NULL ){
		_collisionList[_nowIndex] = regist;
		_nowIndex++;
	}
}

//-----------------------------------------------------------------------------
//! ‰ğœ
//!	@param	[in]	unRegist	‰ğœ‚·‚é“–‚½‚è”»’èî•ñ
//-----------------------------------------------------------------------------
void CollisionManager::UnRegist(Collision* unRegist)
{
	for( s32 i=0; i<LIST_MAX; ++i )
	{
		Collision* tmpCol = _collisionList[i];
		//	Œ©‚Â‚©‚Á‚½‚ç
		if( tmpCol == unRegist ){
			_collisionList[i] = NULL;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//! XV
//-----------------------------------------------------------------------------
void CollisionManager::Update()
{
	// “–‚½‚è”»’è
	typedef list<Collision*>::iterator	ICollItr;

	for( s32 i=0; i<LIST_MAX; ++i )
	{
		Collision* col1 = _collisionList[i];
		if(col1 == NULL ) continue;
		
		// ‰Šú‰»
		col1->clearHitObj();
	}

	for( s32 i=0; i<LIST_MAX; ++i )
	{
		for( s32 j=0; j<LIST_MAX; ++j )
		{
			Collision* col1 = _collisionList[i];
			Collision* col2 = _collisionList[j];

			if( col1 == NULL || col2 == NULL ) continue;
			if( col1 == col2 ) continue;
			
			Sphere*	sphere1 = col1->getHitSphere();
			Sphere*	sphere2 = col2->getHitSphere();

			if( ( sphere1->getPosition() - sphere2->getPosition() ).length() >= 70.0f ) continue; 


			bool flag = sphere1->isHit(*sphere2);

			if( flag )
			{
				col1->setHitObj(col2);
			}
		}
	}

}



//=============================================================================
//	END OF FILE
//=============================================================================
