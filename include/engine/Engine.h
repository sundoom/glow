/*
 *  Engine0.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Engine__
#define __Engine__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Glow.h>
#include <GMath.h>
NAMESPACE_USE(Math)
#include <Render.h>
NAMESPACE_USE(Render)

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	#pragma pack(1)
	struct DVT
	{
		F32 point[3];		// coordinate
		F32 texcoord[2];	// texture coordinate
	};
	struct DVTN
	{
		F32 point[3];		// coordinate
		F32 texcoord[2];	// texture coordinate
		F32 normal[3];		// normal
	};	
	struct DVC
	{
		F32 point[3];		// coordinate
		F32 color[4];		// color : r g b a
	};
	
	struct DVCT
	{
		F32 point[3];		// coordinate
		F32 color[4];		// color : r g b a
		F32 texcoord[2];	// texture coordinate
	};	
	struct DVN
	{
		F32 point[3];		// coordinate
		F32 normal[3];		// normal
	};
	struct DVS
	{
		F32 point[3];		// coordinate
		F32 normal[3];		// normal
		F32 tangent[3];		// tangent
		F32 binormal[3];	// binormal
	};
	struct DVTS
	{
		F32 point[3];		// coordinate
		F32 texcoord[2];	// texture coordinate
		F32 normal[3];		// normal
		F32 tangent[3];		// tangent
		F32 binormal[3];	// binormal
	};
	#pragma pack()

	//////////////////////////////////////////////////////////////////////////

	// unit transformation
	#define U2P(x) ((I32)((x)*64.0f))
	#define P2U(x) ((F32)((x)/64.0f))

	//////////////////////////////////////////////////////////////////////////

	#define TILE_WIDTH	128
	#define TILE_HEIGHT	64
	extern VOID T2P(I32& px, I32& py, I32 tx, I32 ty);
	extern VOID P2T(I32& tx, I32& ty, I32 px, I32 py);
	
//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#include <Key.h>
#include <Graph.h>
#include <Node.h>

//////////////////////////////////////////////////////////////////////////

#include <model/shape/Shape.h>
#include <model/shape/Box.h>
#include <model/shape/Capsule.h>
#include <model/shape/Sphere.h>
#include <model/shape/Cone.h>
#include <model/shape/Cylinder.h>
#include <model/shape/Line.h>

#include <model/BoneFrame.h>
#include <model/AnimBone.h>
#include <model/StaticMesh.h>
#include <model/SkeletalMesh.h>

#include <model/AnimSequence.h>
#include <model/AnimMixer.h>
#include <model/AnimSet.h>
#include <model/Pawn.h>

//////////////////////////////////////////////////////////////////////////

#include <map/Chunk.h>
#include <map/Map.h>

//////////////////////////////////////////////////////////////////////////

#include <map2d/Chunk2D.h>
#include <map2d/Map2D.h>

//////////////////////////////////////////////////////////////////////////

#include <dc/Rect.h>
#include <dc/Bitmap.h>
#include <dc/DC.h>

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////