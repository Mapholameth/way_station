#include <cstdlib>
#include <cstdio>
#include "ClassInfo.h"
#include "Test.h"
#include "Reg.h"
using std::pair;

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
	PropertyInfo *positionInfo = ClassInfo::_classInfos["CFoo"]->Properties()["Position"];
	void* position = ClassInfo::_classInfos[positionInfo->TypeName()]->New();
	PropertyInfo *XInfo = ClassInfo::_classInfos[positionInfo->TypeName()]->Properties()["X"];
	PropertyInfo *YInfo = ClassInfo::_classInfos[positionInfo->TypeName()]->Properties()["Y"];
	PropertyInfo *FlagInfo = ClassInfo::_classInfos[positionInfo->TypeName()]->Properties()["Flag"];
	XInfo->SetString(position, "2.0");
	YInfo->SetString(position, "3.0");
	FlagInfo->SetString(position, "true");
	positionInfo->SetValue(foo, position);

	printf("%s\r\n", XInfo->GetString(position).c_str());

	for (map<string, ClassInfo*>::iterator i = ClassInfo::_classInfos.begin(); i != ClassInfo::_classInfos.end(); ++i)
	{
		printf("%s\r\n", i->first.c_str());
		for (map<string, PropertyInfo*>::iterator j = i->second->Properties().begin(); j != i->second->Properties().end(); ++j)
		{
			printf("\t%s\r\n", j->first.c_str());
		}
	}

//	printf("%s\r\n%s\r\n%s\r\n", position->Name(), position->OwnerName(), position->TypeName());
	

	system("pause");
	return EXIT_SUCCESS;
}
