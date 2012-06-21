#include <cstdlib>
#include <cstdio>
#include <sys/stat.h>
#include "ClassInfo.h"
#include "Test.h"
#include "Reg.h"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

using std::pair;

/**
 *	goals:
 *	X	RTTI by name
 *	X	property by name
 *	X	ineritance info for props
 *	X	integral type properties
 *		safety
 *	-	named cast	-	practically impossible to achieve
 *	X	array properies
 *		ptr to type properties
 *		multiple (de)serialization methods
 *		multiple inheritance
 *		RTTI::IsKindOf(foo, bar)
 *		Casted call
 */

/**
 *	One more case. This one shall be handled properly.
 *	In CGameObject we have vector<CGameObject*> children; member
 *	This one is array of ptr to CGameObject wich could be anything, like CPlaceable or CRenderable.
 *	"Children" : [ {...}, {...}, {...} ] - will look like this in JSON
 *	So. Where shall I put type information for each array member?
 *	Apparently inside object.
 *	How?
 *	Apparently like { "Type" : "CPlaceableObject", ... }
 *	Such solution is somewhat kludgy and ugly to me, but there is nothing better I could think of.
 *	So rule shall be somewhat like: "If property is ptr to something and if that somthing has injected type info
 *	(which means it is a class) and (optionally) that class has descendant(s) then it's json-object representation-table
 *	has reserved key "Type" or somewhat like that, which describes actual type of the object.
 *	and if that type is actually present and is derived from our property type so ptr to it could be casted back, then
 *	we can. Yes. We. Can.
 */


/* shall be handy for some test

typedef void (*FooCall)(const CFoo &foo);

void TestFooCall(const CFoo &foo)
{
	printf( "does nothing\r\n");
}

*/

void ShowToFile(void *o, char* name, char* filename)
{

	class CStateInfo
	{
	public:
		int depth;
		string s;

		CStateInfo() : depth(0), s("") {}
		void SaveToFile(const string& filename)
		{
			FILE* fo = fopen(filename.c_str(), "w");
			fprintf(fo, "%s", s.c_str());
			fclose(fo);
		}
	};

	struct T
	{
		static string RepeatString(string str, unsigned count)
		{
			string r = "";
			while (count)
				r += str, count--;
			return r;
		}

		static void Helper(CStateInfo& state, void* next, const string &nextName)
		{
			state.s += RepeatString("\t", state.depth);
			state.s += "{\n";
			state.depth++;

			TypeInfo *typeInfo = TypeInfo::GetTypeInfo(nextName);
			while (typeInfo)
			{
				map<string, PropertyInfo*> &props = typeInfo->Properties();
				for (map<string, PropertyInfo*>::iterator i = props.begin(); i != props.end(); ++i)
				{
					map<string, PropertyInfo*>::iterator nextIter = ++i;
					i--;

					if (i->second->Integral())
					{
						void *value = i->second->GetValue(next);
						std::string stringTypeName = i->second->TypeName();
						TypeInfo* _typeInfo = TypeInfo::GetTypeInfo(stringTypeName);
						std::string stringValue = _typeInfo->GetString(value);

						state.s += RepeatString("\t", state.depth) +
								   "\"" +
								   i->second->Name() +
								   "\" : \"" +
								   stringValue.c_str() +
								   "\"" +
								   RepeatString(",", typeInfo->BaseInfo() != 0 || nextIter != props.end()) +
									"\n";
					}
					else if (i->second->IsArray())
					{
						state.s += RepeatString("\t", state.depth) +
								   "\"" +
								   i->second->Name() +
								   "\" : \n" +
								   RepeatString("\t", state.depth) +
								   "[\n";
						state.depth++;
						for (int j = 0; j < i->second->GetArraySize(next); j++)
						{
							void *value = i->second->GetValue(next, j);
							char floatStr[256];
							sprintf(floatStr, "%f", *(static_cast<float*>(value)));

							state.s += RepeatString("\t", state.depth) +
									   "\"" +
									   floatStr +
									   "\"" +
									   RepeatString(",", j != i->second->GetArraySize(next) - 1 ) +
									   "\n";
							delete value;
						}
						state.depth--;
						state.s += RepeatString("\t", state.depth) +
								   "]" +
								   RepeatString(",", nextIter != props.end() || typeInfo->BaseInfo()) +
								   "\n";
					}
					else
					{
						state.s += RepeatString("\t", state.depth) +
								   "\"" +
								   i->second->Name() +
								   "\" : \n";
						void *value = i->second->GetValue(next);
						Helper(state, value, i->second->TypeName());
						delete value;
					}
				}
				typeInfo = typeInfo->BaseInfo();
			}
			state.depth--;
			state.s += RepeatString( "\t", state.depth );
			if (state.depth == 0)
				state.s += "}\n";
			else
				state.s += "},\n";
		}
	};

	CStateInfo state;
	//state.s += (string)"\"" + name + "\" : \n";
	T::Helper(state, o, name);
	state.SaveToFile(filename);
}


void* EatShitFromJSONToCFoo(char* typeName, char* filename)
{
	void *result = NULL;
	rapidjson::Document document;
	char* buffer;
	FILE* fi = fopen(filename, "rb");
	struct stat FileStat;
	stat(filename, &FileStat);
	buffer = new char [FileStat.st_size + 1];
	buffer[FileStat.st_size] = 0;
	fread(buffer, 1, FileStat.st_size, fi);
	fclose(fi);
	document.Parse<0>(buffer);
	//bool isObject = document.IsObject();
	class T
	{
	public:
		// Properly working breakpoints on windows with msvc10 and cdb inside static-function-inside-class-inside-function?
		// Qt-Creator: "No, not heard."
		static void* Helper(rapidjson::Document::ValueType* document, char *nextName)
		{
			bool isObject = document->IsObject();
			TypeInfo *typeInfo = TypeInfo::GetTypeInfo(nextName);
			void *next = typeInfo->New();
			for (rapidjson::Document::ValueType::MemberIterator i = document->MemberBegin(); i != document->MemberEnd(); ++i)
			{
				std::string propertyName = i->name.GetString();
				PropertyInfo *prop = typeInfo->FindProperty(propertyName);
				if (i->value.IsObject())
				{
					prop->SetValue(next, Helper(&(i->value), prop->TypeName()));
				}
				else if( i->value.IsArray())
				{
					for (int j = 0; j < i->value.Size(); j++)
					{
						TypeInfo *tempTypeInfo = TypeInfo::GetTypeInfo(prop->TypeName());
						void *temp = tempTypeInfo->New();
						tempTypeInfo->SetString(temp, i->value[j].GetString());
						prop->PushValue(next, temp);
						delete temp;
					}
				}
				else
				{
					TypeInfo *tempTypeInfo = TypeInfo::GetTypeInfo(prop->TypeName());
					void *temp = tempTypeInfo->New();
					tempTypeInfo->SetString(temp, i->value.GetString());
					prop->SetValue(next, temp);
					delete temp;
				}
			}
			return next;
		}
	};

	result = T::Helper(&document, typeName);
	return result;
}

int main(int argc, char* argv[])
{	
	/*
	{
		"type" : "CFoo",
		"position" : 
		{
			"x" : "2.0f",
			"y" : "3.0f",
		}
	}
	*/
	CFoo *foo = (CFoo*)TypeInfo::GetTypeInfo("CFoo")->New();
	/*
	PropertyInfo *positionInfo = ClassInfo::_classInfos["CFoo"]->Properties()["Position"];
	void* position = ClassInfo::_classInfos[positionInfo->TypeName()]->New();
	PropertyInfo *XInfo = ClassInfo::_classInfos[positionInfo->TypeName()]->Properties()["X"];
	PropertyInfo *YInfo = ClassInfo::_classInfos[positionInfo->TypeName()]->Properties()["Y"];
	PropertyInfo *FlagInfo = ClassInfo::_classInfos[positionInfo->TypeName()]->Properties()["Flag"];
	PropertyInfo *StrInfo = ClassInfo::_classInfos[positionInfo->TypeName()]->Properties()["Str"];
	//PropertyInfo *ptrPositionInfo = ClassInfo::_classInfos["CFoo"]->Properties()["PtrPosition"];
	XInfo->SetString(position, "2.0");
	YInfo->SetString(position, "3.0");
	FlagInfo->SetString(position, "true");
	StrInfo->SetString(position, "ololo");
	positionInfo->SetValue(foo, position);

	Vector2 pos = foo->GetPosition();
	pos.PushToArray(1.0f);
	pos.PushToArray(2.0f);
	pos.PushToArray(3.0f);
	pos.PushToArray(4.0f);
	foo->SetPosition(pos);
//	ptrPositionInfo->SetValue(foo, &position);
	*/
	//printf("%s\r\n", XInfo->GetString(position).c_str());
	/*
	for (map<string, ClassInfo*>::iterator i = ClassInfo::_classInfos.begin(); i != ClassInfo::_classInfos.end(); ++i)
	{
		printf("%s\r\n", i->first.c_str());
		for (map<string, PropertyInfo*>::iterator j = i->second->Properties().begin(); j != i->second->Properties().end(); ++j)
		{
			printf("\t%s\t%s\r\n", j->second->TypeName(), j->first.c_str());
		}
	}
	*/
	//Show(foo, "CFoo");

	CBarDerived bar;
	bar.PushToArray(1);
	bar.PushToArray(2);
	bar.PushToArray(3);
	bar.PushToArray(4);
	try
	{
		ShowToFile(&bar, "CBarDerived", "dump.json");
		void* value = EatShitFromJSONToCFoo("CBarDerived", "dump.json");
		ShowToFile(value, "CBarDerived", "diff.json");
	}
	catch(char* pizdec)
	{
		printf("PIZDEC: %s\r\n", pizdec);
	}

	//Show(&bar, "CFoo");
	//	printf("%s\r\n%s\r\n%s\r\n", position->Name(), position->OwnerName(), position->TypeName());
	printf( "%s\n%s\n",
			CFoo().GetTypeInfo()->Name(),
			CBarDerived().GetTypeInfo()->Name());

	return EXIT_SUCCESS;
}
