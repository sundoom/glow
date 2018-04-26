/*
 *  GConfig.cpp
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include <Glow.h>
#include "xml/tinyxml.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

GConfig ::GConfig ()
{
	GUARD(GConfig::GConfig);

	UNGUARD;
}

GConfig ::~GConfig ()
{
	GUARD(GConfig::~GConfig);

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////

/*
====================
SetValue
====================
*/
VOID GConfig::SetValue(const CHAR* value)
{
	GUARD(GConfig::SetValue);

	mValue = value;

	UNGUARD;
}

/*
====================
GetValue
====================
*/
const CHAR* GConfig::GetValue() const
{
	GUARD(GConfig::GetValue);

	return mValue.c_str();

	UNGUARD;
}

/*
====================
SetText
====================
*/
VOID GConfig::SetText(const CHAR* text)
{
	GUARD(GConfig::SetText);

	mText = text;

	UNGUARD;
}

/*
====================
GetText
====================
*/
const CHAR* GConfig::GetText() const
{
	GUARD(GConfig::GetText);

	return mText.c_str();

	UNGUARD;
}

/*
====================
SetAttribute
====================
*/
VOID GConfig::SetAttribute(const CHAR* name, const CHAR* value)
{
	GUARD(GConfig::SetAttribute);

	std::map<Str,Str>::iterator it = mAttributes.find(name);
	CHECK(it==mAttributes.end()&&"The attribute is existent already!\n");
	mAttributes.insert( std::map<Str, Str>::value_type(name, value) );

	UNGUARD;
}	

/*
====================
GetAttribute
====================
*/
const CHAR* GConfig::GetAttribute(const CHAR* name) const
{
	GUARD(GConfig::GetAttribute);

	std::map<Str,Str>::const_iterator it = mAttributes.find(name);
	if(it!=mAttributes.end()) return it->second.c_str();
	return "";

	UNGUARD;
}

/*
====================
SetChild
====================
*/
VOID GConfig::SetChild(const GConfig* child)
{
	GUARD(GConfig::SetChild);

	CHECK(child);
	mChildren.push_back((GConfig*)child);

	UNGUARD;
}

/*
====================
GetChild
====================
*/
const GConfig* GConfig::GetChild(U32 i) const
{
	GUARD(GConfig::GetChild);

	if(i >= mChildren.size()) return NULL;
	return mChildren[i].Ptr();

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////

/*
====================
parse
====================
*/
static GConfig* parse(const TiXmlElement * element_ptr)
{
	GUARD(parse);

	CHECK(element_ptr);
	GConfig* config_ptr = GNEW(GConfig ); CHECK(config_ptr);

	// parse the value of the element
	config_ptr->SetValue(element_ptr->Value());

	// parse the text of the element
	const CHAR * text = element_ptr->GetText();
	if(text) config_ptr->SetText(text);

	// parse the attribute of the element
	for(const TiXmlAttribute* attribute_ptr = element_ptr->FirstAttribute(); attribute_ptr; attribute_ptr = attribute_ptr->Next())
	{
		config_ptr->SetAttribute(attribute_ptr->Name(), attribute_ptr->Value());
	}

	// parse the children of the element
	for(const TiXmlElement *child_element_ptr = element_ptr->FirstChildElement(); child_element_ptr; child_element_ptr = child_element_ptr->NextSiblingElement()) 
	{
		config_ptr->SetChild(parse(child_element_ptr));
	}

	return config_ptr;

	UNGUARD;
}

/*
====================
load
====================
*/
GConfig* GConfig::Load(const CHAR* config)
{
	GUARD(GConfig::Load);

	CHECK(config);

	// xml parse
	TiXmlDocument document;
	document.Parse(config, NULL, TIXML_ENCODING_UTF8);
	if(document.Error()){GAssert(VA("The config is error: %s(row=%d,col=%d)", document.ErrorDesc(),document.ErrorRow(),document.ErrorCol())); return NULL;}
	return parse( document.RootElement() );

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////
