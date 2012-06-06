#include <cstdlib>
#include <cstdio>
#include "ClassInfo.h"
#include "Test.h"
#include "Reg.h"

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
	PropertyInfo *positionInfo = ClassInfo::_classInfos["CFoo"]->_props["Position"];
	void* position = ClassInfo::_classInfos[positionInfo->TypeName()]->New();
	PropertyInfo *XInfo = ClassInfo::_classInfos[positionInfo->TypeName()]->_props["X"];
	PropertyInfo *YInfo = ClassInfo::_classInfos[positionInfo->TypeName()]->_props["Y"];
	XInfo->SetString(position, "2.0");
	YInfo->SetString(position, "3.0");
	positionInfo->SetValue(foo, position);

//	printf("%s\r\n%s\r\n%s\r\n", position->Name(), position->OwnerName(), position->TypeName());
	

	system("pause");
	return EXIT_SUCCESS;
}
