#ifndef LOG_H
#define LOG_H

#include <iostream>
#define F_NAME __PRETTY_FUNCTION__
#define LOG_FMT " In:\n" << __FILE__ << ":" << __LINE__ << " " << F_NAME << ":\n\t"
#define LOG_DEBUG std::cout << "\n\nDBG" << LOG_FMT
#define LOG_INFO  std::cout << "\n\nINF" << LOG_FMT
#define LOG_ERROR std::cerr << "\n\nERR" << LOG_FMT
#define LOG_WARN  std::cout << "\n\nWRN" << LOG_FMT
#define LOG_WARNING LOG_WARN
#endif // LOG_H
