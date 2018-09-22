#include <engine/Exception.hh>
#include <iostream>
#include <iomanip>
#include <sstream>


Exception::Exception(
	int code,
	int subCode,
	const string &message,
	const Exception &cause,
	const string &function,
	const string &fileName,
	uint32_t lineNumber,
	bool setException ) : code(code), message(""), fileName(""),
		lineNumber(0), cause(NULL)
{
	this->cause = new Exception(cause);
	generateText(false);
}


Exception::Exception(
	int code,
	int subCode,
	const string &message,
	const string &function,
	const string &fileName,
	uint32_t lineNumber,
	bool setException ) : code(code), message(message), subCode(subCode),
		fileName(fileName), lineNumber(lineNumber), cause(NULL)
{
	generateText(false);
}


Exception::Exception(
    const Exception &object )
{
    code       = object.code;
    subCode    = object.subCode;
    message    = object.message;
    function   = object.function;
    fileName   = object.fileName;
    lineNumber = object.lineNumber;
    if (object.cause != NULL)
		cause = new Exception(*object.cause);
	else
		cause = NULL;

	generateText(false);

    // Note: we never set the last exception in the copy constructor!
}


Exception::~Exception() throw ()
{
	if (cause != NULL) delete cause;
}


int Exception::getCode() const throw()
{
	return code;
}


const std::string &Exception::getMessage() const throw()
{
	return message;
}


const Exception &Exception::getCause() const throw()
{
	return *cause;
}


const char* Exception::what() const throw()
{
	return text.c_str();
}


const std::string &Exception::getFileName() const throw()
{
	return fileName;
}


const uint32_t Exception::getLine() const throw()
{
	return lineNumber;
}


const string &Exception::getFunction() const throw()
{
	return function;
}


int Exception::getSubCode() const throw()
{
	return subCode;
}


const string &Exception::toString() const throw()
{
	return text;
}


const string &Exception::generateText(
	bool isCause )
{
	stringstream ss;

	if (isCause)
		ss << "  caused by ";
	else
		ss << "Exception ";

	ss << " (" << uppercase << setw(4) << hex << setfill('0') <<
		getCode() << ':' << getSubCode() << ')';

	ss << ' ' << getMessage() << std::endl;

	ss << "  from ";
	if (!function.empty())
		ss << getFunction();
	ss << ' ' << " [" << getFileName() << ':' << getLine() << ']' << std::endl;

	if (cause != NULL)
		ss << generateText(false);

	text = ss.str();
	return text;
}


ostream &operator << (
	ostream &out,
	const Exception &ex )
{
	out << ex.toString();
	return out;
}


