//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////	

	Pawn::Pawn()
	{
		
	}

	Pawn::~Pawn() 
	{

	}

	/*
	====================
	set/get the animation set
	====================
	*/	
	VOID Pawn::SetAnimSet(const AnimSet* as)
	{
		CHECK(as);
		mAnimSetPtr = (AnimSet*)as;

		// build the root bone
		mRootPtr = mAnimSetPtr->CreateBone();
		CHECK(mRootPtr);
		mRootPtr->blend(0, 1, 0);

		// bind all of the skeletal mesh 
		for(std::map<Str, SkeletalMeshPtr>::iterator it = mSkeletalMeshes.begin(); it != mSkeletalMeshes.end(); ++it)
		{
			if(it->second) it->second->Bind(mRootPtr.Ptr());
		}

		// bind the animation mixer
		mAnimMixer.Bind(mRootPtr.Ptr());

		// add the sequence to the animation mixer
		for(std::list<AnimSequencePtr>::iterator it = mAnimSetPtr->mSequences.begin(); it != mAnimSetPtr->mSequences.end(); ++it )
		{
			AnimSequencePtr& sequence = (*it);
			AnimSequence* new_sequence = GNEW(AnimSequence); CHECK(new_sequence);
			new_sequence->SetName(sequence->GetName());
			new_sequence->SetFlags(sequence->GetFlags());
			new_sequence->SetFirstFrame(sequence->GetFirstFrame());
			new_sequence->SetLastFrame(sequence->GetLastFrame());
			new_sequence->SetFrameRate(sequence->GetFrameRate());
			new_sequence->SetInTime(sequence->GetInTime());
			new_sequence->SetOutTime(sequence->GetOutTime());
			mAnimMixer.mSequences.insert(std::make_pair(new_sequence->GetName(), new_sequence));
		}
	}

	const AnimSet* Pawn::GetAnimSet() const
	{
		return mAnimSetPtr.Ptr();
	}

	/*
	====================
	add/remove the static mesh
	====================
	*/	
	VOID Pawn::AddStaticMesh(const CHAR* name, StaticMesh*mesh)
	{
		// check the mesh
		CHECK(name&&mesh);

		// if this static mesh is existent?
		std::map<Str, StaticMeshPtr>::iterator it = mStaticMeshes.find(name);
		CHECK(it==mStaticMeshes.end());
		mStaticMeshes.insert(std::make_pair(name,mesh));
	}

	VOID Pawn::RemoveStaticMesh(const CHAR* name)
	{
		std::map<Str, StaticMeshPtr>::iterator it = mStaticMeshes.find(name);
		if(it!=mStaticMeshes.end()) mStaticMeshes.erase(it);
	}

	/*
	====================
	add/remove the skeletal mesh
	====================
	*/
	VOID Pawn::AddSkeletalMesh(const CHAR* name, SkeletalMesh*mesh)
	{
		// check the mesh
		CHECK(name&&mesh);

		// if this skeletal mesh is existent?
		std::map<Str, SkeletalMeshPtr>::iterator it = mSkeletalMeshes.find(name);
		CHECK(it==mSkeletalMeshes.end());
		mSkeletalMeshes.insert(std::make_pair(name,mesh));
		if(mRootPtr) mesh->Bind(mRootPtr.Ptr());
	}

	VOID Pawn::RemoveSkeletalMesh(const CHAR* name)
	{
		std::map<Str, SkeletalMeshPtr>::iterator it = mSkeletalMeshes.find(name);
		if(it!=mSkeletalMeshes.end()) mSkeletalMeshes.erase(it);
	}

	/*
	====================
	Play
	====================
	*/
	VOID Pawn::Play(const CHAR* name, BOOL mix)
	{
		if(mix)
		{
			mAnimMixer.Mix(name);
		}
		else
		{
			mAnimMixer.Play(name);
		}
	}

	/*
	====================
	Tick
	====================
	*/
	VOID Pawn::Tick(F32 delta_time)
	{
		mAnimMixer.Tick(delta_time);
	}

	/*
	====================
	Draw
	====================
	*/
	VOID Pawn::Draw(Graph *gc)
	{
		// draw the static mesh
		for(std::map<Str, StaticMeshPtr>::iterator it1 = mStaticMeshes.begin(); it1 != mStaticMeshes.end(); ++it1)
		{
			if(it1->second) it1->second->Draw(gc);
		}

		// draw the skeletal mesh
		for(std::map<Str, SkeletalMeshPtr>::iterator it2 = mSkeletalMeshes.begin(); it2 != mSkeletalMeshes.end(); ++it2)
		{
			if(it2->second) it2->second->Draw(gc);
		}
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
