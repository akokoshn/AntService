/**
 *  Tools -- various utilities
 *  Copyright (C) 2017 - 2019 Alex Harsanyi (AlexHarsanyi@gmail.com)
 *                            Alexey Kokoshnikov (alexeikokoshnikov@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the Free
 *  Software Foundation, either version 3 of the License, or (at your option)
 *  any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <vector>
#include <iosfwd>
#include <string>
#include <sstream>
#include <chrono>

#include <string.h>
#include <stdint.h>


//#define DEBUG
//#define DEBUG_DUMP

// .................................................... LibusbError ....

/** Convenience class to throw exception with USB error codes and get proper
 * error names for them.
 */
class LibusbError : public std::exception
{
public:
    LibusbError(const char *who, int error_code)
        : m_Who(who), m_ErrorCode(error_code), m_MessageDone(false)
        {
        }
    virtual ~LibusbError() throw();

    const char* what() const noexcept;
    int error_code() { return m_ErrorCode; }

private:
    std::string m_Who;
    int m_ErrorCode;
    mutable std::string m_Message;
    mutable bool m_MessageDone;
};


// ......................................................... Win32Error ....

/** Convenience class to throw exceptions with Win32 error codes and get
 * proper error names for them.  Works with GetLastError() and
 * WSAGetLastError()
 */
 #ifdef WIN32
class Win32Error : public std::exception
{
public:
    Win32Error (const std::string &who = "", unsigned long error = 0);
    virtual ~Win32Error ();

    const char* what() const noexcept;
    unsigned long error() const { return m_ErrorCode; }

private:
    std::string m_Who;
    unsigned long m_ErrorCode;
    mutable std::string m_Message;
    mutable bool m_MessageDone;
};
#endif


/** Print a hex dump of 'data' to the stream 'o'.  The data is printed on
 * lines with the address, character representation and hex representation on
 * each line.  This hopefully makes it easy to determine the contents of both
 * character and binary data.
 */
void DumpData (const unsigned char *data, int size, std::ostream &o);

/**Print debug message
*/
#if defined(DEBUG)
#define LOG_MSG(msg) printf(msg)
#define LOG_D(val) printf("%d\n", val)
#define LOG_PRINT(f_, ...) printf((f_), __VA_ARGS__)
#else
#define LOG_MSG(msg)
#define LOG_D(val)
#define LOG_PRINT(f_, ...)
#endif


/** Return a timestamps in milliseconds from an unspecified epoch.  Can be
 * used to measure real time from the difference between two subsequent calls
 * to the function.  Timer resolution is unspecified, but it is not very high,
 * measuring times of 50 ms or more should be ok.  Less than that, not so
 * much.
 */
uint32_t CurrentMilliseconds(std::chrono::time_point<std::chrono::system_clock> start);

#if 0
/** Put the current time on the output stream o. */
void PutTimestamp(std::ostream &o);
#endif

/*
  Local Variables:
  mode: c++
  End:
*/
