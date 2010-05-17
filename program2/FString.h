/* this is something that was given to me in 305
	Basically, it allows you to create c++ string objects
	that are formatted using printf style rules
 */
#include <stdarg.h>
#include <string>
#include <stdio.h>
class FString : public std::string {
	public:
		FString(const char *fmt, ...) {
			static const int bufLen = 1024;
			static char buf[bufLen];
			va_list args;

			va_start(args, fmt); 
			vsnprintf(buf, bufLen, fmt, args);
			va_end(args);

			*(std::string *)this = std::string(buf);
		}
};
