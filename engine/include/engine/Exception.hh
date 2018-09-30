#ifndef ENGINE_EXCEPTION_HH
#define ENGINE_EXCEPTION_HH


#define ERR_NONE                     (0x00)
#define ERR_OUT_OF_MEMORY            (0x01)
#define ERR_INVALID_ARGUMENT         (0x02)
#define ERR_NOT_FOUND                (0x03)
#define ERR_IO_OPEN                  (0x04)
#define ERR_IO_READ                  (0x0A)
#define ERR_IO_WRITE                 (0x05)
#define ERR_INVALID_DATA             (0x06)
#define ERR_OUT_OF_BOUNDS            (0x07)
#define ERR_LOCK_ERROR               (0x08)
#define ERR_INVALID_SYNTAX           (0x09)


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <exception>
#include <iostream>
#include <stdint.h>


#define EXCEPTION(code,subcode,msg) \
	( Exception(code, subcode, msg, __FUNCTION__, __FILE__, __LINE__) )


using namespace std;


class Exception : public std::exception
{
	public:
		Exception(
			int code,
			int subCode,
			const string &message,
			const Exception &cause,
			const string &function = "",
			const string &filename = "",
			uint32_t lineNumber = 0,
			bool setException = true );

		Exception(
			int code,
			int subCode,
			const string &message,
			const string &function = "",
			const string &filename = "",
			uint32_t lineNumber = 0,
			bool setException = true );

        Exception(
            const Exception &object );

		virtual ~Exception() throw();

		/**
		 * @brief Returns the error code.
		 */
		int getCode() const throw();

		/**
		 * @brief Returns the custom error message. If no message was provided
		 * to the constructor, an empty string is returned.
		 */
		const std::string &getMessage() const throw();

		/**
		 * @brief Returns the file name where the exception was thrown.
		 */
		const std::string &getFileName() const throw();

		/**
		 * @brief Returns the line number where the exception was thrown.
		 */
		uint32_t getLine() const throw();

		/**
		 * @brief Returns the name of the function which called the @c Exception constructor.
		 */
		 const string &getFunction() const throw();

		/**
		 * @brief Returns a datailed error message.
		 */
		const char* what() const throw();

		/**
		 * @brief Returns a datailed error message.
		 */
		const string &toString() const throw();

		/**
		 * @brief Returns the previous exception in the stack.
		 */
		const Exception &getCause() const throw();

		/**
		 * @brief Returns the internal code.
		 */
		int getSubCode() const throw();

	protected:
		int code;

		string message;

		string function;

		int subCode;

		string fileName;

		uint32_t lineNumber;

		const Exception *cause;

		string text;

		/**
		 * Created a datailed error message for the entire exception stack.
		 */
		const string &generateText(
			bool isCause = false );
};


ostream &operator << (
	ostream &out,
	const Exception &exception );



#endif // ENGINE_EXCEPTION_HH
