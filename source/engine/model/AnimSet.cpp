//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////	

	AnimSet::AnimSet()
	{
		
	}

	AnimSet::~AnimSet() 
	{

	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Load
	====================
	*/
	VOID AnimSet::Load(const GData* data)
	{
		GUARD(AnimSet::load);

		CHECK(data);

		// get the animation config
		GConfigPtr anim_config_ptr = GConfig::Load((CHAR*)data->Ptr()); CHECK(anim_config_ptr);

		// check it if it is a animation?
		CHECK(Str(anim_config_ptr->GetValue()) == "animation");

		// prase the mesh`s children
		const GConfig* anim_child_config_ptr = NULL;
		for(U32 i = 0; anim_child_config_ptr=anim_config_ptr->GetChild(i); i++)
		{
			if(Str(anim_child_config_ptr->GetValue())=="frame")
			{
				const GConfig *frame_config_ptr = (const GConfig*)anim_child_config_ptr;

				// get the frame count
				U32 count = ::atoi(frame_config_ptr->GetAttribute("count"));

				// prase the frame`s children
				const GConfig* frame_child_config_ptr = NULL;
				for(U32 j = 0; frame_child_config_ptr = frame_config_ptr->GetChild(j); j++)
				{
					if( Str(frame_child_config_ptr->GetValue()) == "bone" )
					{
						const GConfig *bone_config_ptr = (const GConfig*)frame_child_config_ptr;

						// build a new bone frame
						BoneFrame* frame = GNEW(BoneFrame); CHECK(frame);

						// get the node name
						frame->SetName(bone_config_ptr->GetAttribute("name"));

						// get the parent name
						Str parent_name = bone_config_ptr->GetAttribute("parent");

						// set the sequence
						frame->mKeys.resize(count);
						std::vector<Str>tokens = GTokenize(bone_config_ptr->GetText());
						CHECK(tokens.size()%count==0 && tokens.size()/count==16);
						for(U32 k = 0; k < count; k++) 
						{
							BoneFrame::KEY&key = frame->mKeys[k];
							Matrix mat;
							for(U32 l = 0; l < 16; l++) mat.ptr()[l] = (F32)::atof(tokens[k*16+l].c_str());
							// key.t = mat.getTranslate(); key.r = mat.getRotate(); key.s = mat.getScale();
							Vector3& row0 = *reinterpret_cast<Vector3*>(&mat.ptr()[0]); 
							Vector3& row1 = *reinterpret_cast<Vector3*>(&mat.ptr()[4]);
							Vector3& row2 = *reinterpret_cast<Vector3*>(&mat.ptr()[8]);
							// Vector3& row3 = *reinterpret_cast<Vector3*>(&mat.ptr()[12]);
							Vector3 scale = mat.getScale();
							row0 *= 1.f / scale[0]; row1 *= 1.f / scale[1]; row2 *= 1.f / scale[2];
							Vector3 in = row0 ^ row1;
							if( (in*row2) < 0 ) { row2 *= -1; scale[2] *= -1; }
							key.t = mat.getTranslate(); key.r = mat.getRotate(); key.s = scale;
						}

						// set the sequence
						if(mRootPtr==NULL)
						{
							mRootPtr = frame;
						}
						else
						{
							BoneFrame* parent = mRootPtr->Find(parent_name.c_str()); CHECK(parent);
							frame->SetParent(parent);
						}
					}
					else
					{
						GAssert( VA("The frame`s keyword(%s) is unknown!", frame_child_config_ptr->GetValue()) );
					}
				}
			}
			// prase the sequence
			else if(Str(anim_child_config_ptr->GetValue()) == "sequence")
			{
				const GConfig*sequence_config_ptr = (const GConfig*)anim_child_config_ptr;

				// create the new sequence
				AnimSequence* sequence = GNEW(AnimSequence); CHECK(sequence);

				// get the sequence name
				Str name = sequence_config_ptr->GetAttribute("name");
				sequence->SetName(name.c_str());

				// prase the sequence`s children
				const GConfig* sequence_child_config_ptr = NULL;
				for(U32 j = 0; sequence_child_config_ptr = sequence_config_ptr->GetChild(j); j++)
				{
					if(Str(sequence_child_config_ptr->GetValue()) == "flags")
					{
						std::vector<Str>tokens = GTokenize(sequence_child_config_ptr->GetText());
						U32 flags = 0;
						for(U32 k = 0; k < tokens.size(); k++)
						{
							if(tokens[k] == "ASF_LOOP")
							{
								flags |= AnimSequence::ASF_LOOP;
							}
							else if(tokens[k] == "ASF_MOVE")
							{
								flags |= AnimSequence::ASF_MOVE;
							}
							else if(tokens[k] == "ASF_IMPACT")
							{
								flags |= AnimSequence::ASF_IMPACT;
							}
						}
						sequence->SetFlags(flags);
					}
					else if(Str(sequence_child_config_ptr->GetValue()) == "first_frame")
					{
						U32 first_frame = ::atoi(sequence_child_config_ptr->GetText());
						sequence->SetFirstFrame(first_frame);
					}
					else if(Str(sequence_child_config_ptr->GetValue()) == "last_frame")
					{
						U32 last_frame = ::atoi(sequence_child_config_ptr->GetText());
						sequence->SetLastFrame(last_frame);
					}
					else if(Str(sequence_child_config_ptr->GetValue()) == "frame_rate")
					{
						F32 frame_rate = (F32)::atof(sequence_child_config_ptr->GetText());
						sequence->SetFrameRate(frame_rate);
					}
					else if(Str(sequence_child_config_ptr->GetValue()) == "in_time")
					{
						F32 in_time = (F32)::atof(sequence_child_config_ptr->GetText());
						sequence->SetInTime(in_time);
					}
					else if(Str(sequence_child_config_ptr->GetValue()) == "out_time")
					{
						F32 out_time = (F32)::atof(sequence_child_config_ptr->GetText());
						sequence->SetOutTime(out_time);
					}
					else
					{
						GAssert( VA("The sequence`s keyword(%s) is unknown!", sequence_child_config_ptr->GetValue()) );
					}
				}

				// add the sequence to the table
				mSequences.push_back(sequence);
			}
			else
			{
				GAssert( VA("The animation`s keyword(%s) is unknown!", anim_child_config_ptr->GetValue()) );
			}
		}

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	CreateBone
	====================
	*/
	AnimBone* AnimSet::CreateBone()
	{
		return CreateBone(mRootPtr.Ptr());
	}

	AnimBone* AnimSet::CreateBone(const BoneFrame* frame)
	{
		// create the new bone
		AnimBone* bone = GNEW(AnimBone); CHECK(bone);		
		bone->name(frame->GetName());
		bone->mFramePtr = (BoneFrame*)frame;

		// compute the children
		const BoneFrame *child = NULL;
		for(U32 i = 0; (child=frame->GetChild(i))!=NULL; i++) 
		{
			AnimBone* bone_child = this->CreateBone(child);
			bone_child->parent(bone);
		}

		return bone;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
