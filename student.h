#pragma once
#include"configure.h"
using unit32 = unsigned int;
class student
{
public:
	student(unit32 number, const std::string& name,const std::string& grade,unit32 math,unit32 chinese,unit32 english);
	student();
	std::string formatInfo()const;
	
	
public:
	unit32 number;// student number
	std::string name;
	std::string grade;
	unit32 math;
	unit32 chinese;
	unit32 english;

};

