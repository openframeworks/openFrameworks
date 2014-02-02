//
// TextTestResult.h
//
// $Id: //poco/1.4/CppUnit/include/CppUnit/TextTestResult.h#1 $
//


#ifndef CppUnit_TextTestResult_INCLUDED
#define CppUnit_TextTestResult_INCLUDED


#include "CppUnit/CppUnit.h"
#include "CppUnit/TestResult.h"
#include <ostream>


namespace CppUnit {


class CppUnit_API TextTestResult: public TestResult
{
public:
	TextTestResult();
	TextTestResult(std::ostream& ostr);
	
	virtual void addError(Test* test, CppUnitException* e);
	virtual void addFailure(Test* test, CppUnitException* e);
	virtual void startTest(Test* test);
	virtual void print(std::ostream& stream);
	virtual void printErrors(std::ostream& stream);
	virtual void printFailures(std::ostream& stream);
	virtual void printHeader(std::ostream& stream);
	
protected:
	std::string shortName(const std::string& testName);
	
private:
	std::ostream& _ostr;
};


/* insertion operator for easy output */
inline std::ostream& operator<< (std::ostream& stream, TextTestResult& result)
{
	result.print(stream);
	return stream;
}


} // namespace CppUnit


#endif // CppUnit_TextTestResult_INCLUDED
