#include "student.h"
#include<sstream>
#include"Management.h"
student::student(unit32 number, const std::string& name,const std::string& grade, unit32 math, unit32 chinese, unit32 english)
	:number(number),name(name),grade(grade),math(math),chinese(chinese),english(english)
{
}
student::student()
{
}

std::string student::formatInfo()const
{
	std::stringstream ss;//������һ������ss����׼д���ַ�
	ss << number << "\t"
		<< name << "\t"
		<< grade << "\t"
		<< math << "\t"
		<< chinese << "\t"
		<< english << std::endl;
	return ss.str();

	










}







