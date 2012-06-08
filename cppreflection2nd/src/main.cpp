#include <cstdlib>
#include <cstdio>
#include "ClassInfo.h"
#include "Test.h"
#include "Reg.h"
using std::pair;

void repT(int n)
{
	for(int i = 0; i < n; i++)
		printf("\t");
}

static int ident = 0;

void Show(void *o, char* name)
{
	repT(ident); printf("{\r\n");
	map<string, PropertyInfo*> &props = ClassInfo::_classInfos[name]->Properties();
	for (map<string, PropertyInfo*>::iterator i = props.begin(); i != props.end(); ++i)
	{
		if (i->second->Integral())
		{
			repT(ident+1); printf("\"%s\" : \"%s\",\r\n", i->second->Name(), i->second->GetString(o).c_str());
		}
		else if (i->second->IsArray())
		{
			repT(ident+1); printf("\"%s\" : \r\n", i->second->Name());
			repT(ident+1); printf("[\r\n");
			ident++;
			for (int j = 0; j < i->second->GetArraySize(o); j++)
			{
				void *value = i->second->GetValue(o, j);
				repT(ident+1); printf("\"%f\",\r\n", *(static_cast<float*>(value)));
				delete value;
			}
			ident--;
			repT(ident+1); printf("],\r\n");
		}
		else
		{
			repT(ident+1); printf("\"%s\" : \r\n", i->second->Name());
			void *value = i->second->GetValue(o);
			ident++;
			Show(value, i->second->TypeName());
			ident--;
			delete value;
		}
	}
	if (ClassInfo::_classInfos[name]->BaseInfo())
		Show(o, ClassInfo::_classInfos[name]->BaseInfo()->Name());
	repT(ident); printf("},\r\n");
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
	CFoo *foo = (CFoo*)ClassInfo::_classInfos["CFoo"]->New();
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
	Show(&bar, "CBarDerived");
	//Show(&bar, "CFoo");
//	printf("%s\r\n%s\r\n%s\r\n", position->Name(), position->OwnerName(), position->TypeName());	

	system("pause");
	return EXIT_SUCCESS;
}
