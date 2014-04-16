///////////////////////////////////////////////////////////////////////////////
/// \author (c) Marco Paland (info@paland.com)
///             2012, PALANDesign Hannover, Germany
///
/// \license LGPLv3
/// This file is part of the CPP11 library.
/// CPP11 is free software: you can redistribute it and/or modify
/// it under the terms of the GNU Lesser Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// CPP11 is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU Lesser Public License for more details.
/// You should have received a copy of the GNU Lesser Public License
/// along with CPP11. If not, see <http://www.gnu.org/licenses/>.
///
///
/// \brief <chrono> platform specific implementation
///
/// To adapt <chrono> to you own system you need to write your own
/// xxx_clock::now() functions and define your clock accuracy in CLOCKS section.
/// This way you can easily adapt <chrono> to your target system
///
///////////////////////////////////////////////////////////////////////////////

#ifndef _STD_CHRONO_IMPL_
#define _STD_CHRONO_IMPL_

#include <windows.h>    // for OS specific xxx_clock::now() implementation

#include "../chrono"

/////////////////////////////////////////////////////////////////////

namespace std {
namespace chrono {

// PLATFORM AND OS SPECIFIC IMPLEMENTATION
system_clock::time_point system_clock::now()
{
  FILETIME ft;
  SYSTEMTIME st;
  typedef chrono::time_point<system_clock, duration> from;

  (void)::GetSystemTime(&st);               // get UTC system time, resolution is [ms]
  if (!::SystemTimeToFileTime(&st, &ft)) {  // ft is given as a 64-bit value representing the number of 100-nanosecond intervals since January 1, 1601 (UTC)
    return time_point_cast<system_clock::duration>(from(duration(0U)));
  }

  // return time point value in [ms] and epoch of 1.1.1970 (UTC)
  // ft epoch is 1.1.1601 in [0.1µs], so subtract offset of 11644473600 seconds for 1.1.1970 epoch
  LARGE_INTEGER count;
  count.LowPart  = ft.dwLowDateTime;
  count.HighPart = ft.dwHighDateTime;
  return time_point_cast<system_clock::duration>(from(milliseconds((count.QuadPart / 10000U) - (LONGLONG)11644473600000)));
}


// PLATFORM AND OS SPECIFIC IMPLEMENTATION
high_resolution_clock::time_point high_resolution_clock::now()
{
  LARGE_INTEGER count, frequency;
  typedef chrono::time_point<high_resolution_clock, duration> from;

  if (!::QueryPerformanceFrequency(&frequency)) {
    // error - return 0
    return time_point_cast<high_resolution_clock::duration>(from(duration(0U)));
  }
  if (!::QueryPerformanceCounter(&count)) {
    // error - return 0
    return time_point_cast<high_resolution_clock::duration>(from(duration(0U)));
  }

  // return value in [ns] (resolution depends on computer hardware)
  // calculate in double, because value doesn't fit in int64
  return time_point_cast<high_resolution_clock::duration>(from(nanoseconds(static_cast<LONGLONG>((double)count.QuadPart * (double)1000000000 / (double)frequency.QuadPart))));
}

} // namespace chrono
} // namespace std

#endif // _STD_CHRONO_IMPL_
