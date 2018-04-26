/*
 *  VolumeTexture.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __VolumeTexture__
#define __VolumeTexture__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented base volume texture interface.
	class VolumeTexture : public BaseTexture
	{
	public:		
		// constructor
		VolumeTexture();
		
		// destructor
		virtual ~VolumeTexture();

		// load the image
		VOID Load(const Image* image);

		// get the depth of the texture
		U32 Depth() const;

		// set/get the wrap r of the texture
		VOID WrapR(U32 r);
		U32 WrapR() const;

	private:
		// bind the texture
		VOID Bind(Context * rc, U32 unit = 0);

		// update the texture
		VOID Update(const Image* image, BOOL created);

	private:
		U32	mDepth;
		U32 mWrapR;

	private:friend class Shader;
			friend class Target;
	};

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __VolumeTexture__

//////////////////////////////////////////////////////////////////////////