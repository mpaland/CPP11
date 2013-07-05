///////////////////////////////////////////////////////////////////////////////
/// \author (c) Christopher Kormanyos, Marco Paland (info@paland.com)
///             2010-2012 Reutlingen, Hannover, Germany
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
/// \brief nullptr support
///
/// This class provides nullptr support for compilers which do not support it
///
///////////////////////////////////////////////////////////////////////////////

#ifndef _NULLPTR_H_
#define _NULLPTR_H_

// define this if your c++ compiler has no native nullptr keyword
#define _NONULLPTR

// known compilers which have native nullptr support
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
  // MSVC++ >= 10.0
  #undef _NONULLPTR
#endif

#if defined(__GNUC__) && ((__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || (__GNUC__ >= 5))
  // GCC >= 4.6
  #undef _NONULLPTR
#endif


#if defined(_NONULLPTR)

// provide a nullptr workaround for C++ compilers not yet supporting it
class nullptr_implementation
{
public:
  template<typename T>
  operator T*() const { return static_cast<T*>(0U); }

  template<typename C, typename T>
  operator T C::*() const { return static_cast<T C::*>(0U); }
private:
  void operator&() const;
};

template<typename T> inline bool operator==(const T* p, const nullptr_implementation&) { return (static_cast<const T*>(0U) == p); }
template<typename T> inline bool operator!=(const T* p, const nullptr_implementation&) { return (static_cast<const T*>(0U) != p); }

template<typename T> inline bool operator==(T* p, const nullptr_implementation&) { return (static_cast<T*>(0U) == p); }
template<typename T> inline bool operator!=(T* p, const nullptr_implementation&) { return (static_cast<T*>(0U) != p); }

template<typename T> inline bool operator==(const nullptr_implementation&, const T* p) { return (static_cast<const T*>(0U) == p); }
template<typename T> inline bool operator!=(const nullptr_implementation&, const T* p) { return (static_cast<const T*>(0U) != p); }

template<typename T> inline bool operator==(const nullptr_implementation&, T* p) { return (static_cast<T*>(0U) == p); }
template<typename T> inline bool operator!=(const nullptr_implementation&, T* p) { return (static_cast<T*>(0U) != p); }

const nullptr_implementation nullptr = { };

#endif // _NONULLPTR

#endif // _NULLPTR_H_
