//----------------------------------------------------------------------------
// YAT LIBRARY
//----------------------------------------------------------------------------
//
// Copyright (C) 2006-2012  N.Leclercq & The Tango Community
//
// Part of the code comes from the ACE Framework (asm bytes swaping code)
// see http://www.cs.wustl.edu/~schmidt/ACE.html for more about ACE
//
// The thread native implementation has been initially inspired by omniThread
// - the threading support library that comes with omniORB. 
// see http://omniorb.sourceforge.net/ for more about omniORB.
//
// Contributors form the TANGO community:
// See AUTHORS file 
//
// The YAT library is free software; you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by the Free 
// Software Foundation; either version 2 of the License, or (at your option) 
// any later version.
//
// The YAT library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
// Public License for more details.
//
// See COPYING file for license details 
//
// Contact:
//      Nicolas Leclercq
//      Synchrotron SOLEIL
//------------------------------------------------------------------------------
/*!
 * \author See AUTHORS file
 */

#ifndef _YAT_BITS_STREAM_H_
#define _YAT_BITS_STREAM_H_


//=============================================================================
// DEPENDENCIES
//=============================================================================
#include <streambuf>
#include <iostream>
#include <iomanip>
#include <bitset>
#if defined(WIN32) && ! defined(NOMINMAX)
# define NOMINMAX
#endif
#include <limits>

#include <yat/bitsstream/Endianness.h>

namespace yat 
{

//=============================================================================
// BitsStorage type
//=============================================================================
#if defined(YAT_64BITS)
  typedef yat::uint64 BitsStorage;
#else
  typedef yat::uint32 BitsStorage;
#endif

//=============================================================================
// class: BitsStream 
//=============================================================================
class YAT_DECL BitsStream;

//=============================================================================
// class: BitsSet (set of bits abstraction)
//=============================================================================
template <size_t _n, typename T> 
class YAT_DECL BitsSet
{
public:
  //---------------------------------------------
  BitsSet (const T& _value = 0) 
  //---------------------------------------------
    : m_value (_value)
  {
    //- noop ctor
  }
  //---------------------------------------------
  ~BitsSet ()
  //---------------------------------------------
  {
    //- noop dtor
  }
  //---------------------------------------------
  BitsSet<_n, T> & operator= (const BitsSet<_n, T>& _src)
  //---------------------------------------------
  {
    if (this == &_src) return *this;
    m_value = _src.m_value;
    return *this;
  }
  //---------------------------------------------
  BitsSet<_n, T> & operator= (const T& _src)
  //---------------------------------------------
  {
    m_value = _src; 
    return *this;
  }
  //---------------------------------------------
  BitsSet<_n, T> & operator|= (const BitsSet<_n, T>& _src)
  //---------------------------------------------
  {
    m_value |= _src.m_value;
    return *this;
  }
  //---------------------------------------------
  BitsSet<_n, T> & operator|= (const T& _src)
  //---------------------------------------------
  {
    m_value |= _src; 
    return *this;
  }
  //---------------------------------------------
  BitsSet<_n, T> & operator&= (const BitsSet<_n, T>& _src)
  //---------------------------------------------
  {
    m_value &= _src.m_value;
    return *this;
  }
  //---------------------------------------------
  BitsSet<_n, T> & operator&= (const T& _src)
  //---------------------------------------------
  {
    m_value &= _src; 
    return *this;
  }
  //---------------------------------------------
  const T& value () const
  //---------------------------------------------
  {
    return m_value;
  }
  //---------------------------------------------
  T& value ()
  //---------------------------------------------
  {
    return m_value;
  }
  //---------------------------------------------
  const T& operator() () const
  //---------------------------------------------
  {
    return m_value;
  }
  //---------------------------------------------
  std::string to_string () const
  //---------------------------------------------
  {
    std::bitset<_n> bs(static_cast<BitsStorage>(m_value));
    return bs.to_string();
  }

private:
  T m_value;
};

//=============================================================================
// class: BitsStream (input stream of bits abstraction)
//=============================================================================
class YAT_DECL BitsStream
{
public:
  BitsStream (unsigned char * _data = 0, 
              size_t _size = 0,
              const Endianness::ByteOrder& _endianness = Endianness::BO_LITTLE_ENDIAN);

  virtual ~BitsStream ();

  //-------------------------------------------------------------------------
  void input_data (unsigned char * _data, size_t _size)
  //-------------------------------------------------------------------------
  {
    if (_data)
    {
      m_ibuffer = _data;
      m_ibuffer_size = _size;
      m_ibuffer_ptr = 0;
      m_current_byte = 0;
      m_bits_in_current_byte = 0;
    }
  }

  //-------------------------------------------------------------------------
  bool read_bits (unsigned int _num_bits, yat::BitsStorage & _bits)
  //-------------------------------------------------------------------------
  {
    _bits = 0;
    return read_bits_i (_num_bits, &_bits);
  }

  //-------------------------------------------------------------------------
  bool skip_bits (unsigned int _num_bits)
  //-------------------------------------------------------------------------
  {
    return read_bits_i (_num_bits, 0);
  }

  //-------------------------------------------------------------------------
  const Endianness::ByteOrder & endianness () const
  //-------------------------------------------------------------------------
  {
    return m_endianness;
  }

  //-------------------------------------------------------------------------
  bool is_big_endian () const
  //-------------------------------------------------------------------------
  {
    return m_endianness == Endianness::BO_BIG_ENDIAN;
  }

  //-------------------------------------------------------------------------
  bool is_little_endian () const
  //-------------------------------------------------------------------------
  {
    return m_endianness == Endianness::BO_LITTLE_ENDIAN;
  }

private:

  //- the <tmp> input buffer (use internally for bits reading)
  yat::BitsStorage m_current_byte;
  
  //- the current number of bits in m_current_byte
  int m_bits_in_current_byte;

  //- the data buffer
  unsigned char * m_ibuffer;

  //- the data buffer size
  size_t m_ibuffer_size;

  //- the current byte index in the data buffer 
  size_t m_ibuffer_ptr;

  //- is the input stream big or little endian?
  const Endianness::ByteOrder m_endianness;

  //- print out <v> in binary representation
  template <typename T> void binary_dump (const std::string & n, const T & v )
  {
    yat::BitsSet<8 * sizeof(T), T> bs(v);
    std::cout << "- " 
              << std::setw(25)
              << std::left
              << std::setfill('.')
              << n 
              << bs.to_string() 
              << std::right
              << std::endl;
  }

  //- this extracts _num_bits bits or skips _num_bits if bits == 0
  inline bool read_bits_i (int _num_bits, yat::BitsStorage * _bits)
  {
    bool retval = true;
    
    if ( m_ibuffer != 0 ) 
    {
      //- this part of the algorithm extracts full byte(s)
      while ( _num_bits > m_bits_in_current_byte )
      {
        if ( _bits )
        {
          *_bits |= m_current_byte << ( _num_bits - m_bits_in_current_byte );
        }

        _num_bits -= m_bits_in_current_byte;

        if ( m_ibuffer_ptr != m_ibuffer_size )
        {
          m_current_byte = m_ibuffer[m_ibuffer_ptr++];
          m_bits_in_current_byte = 8;
        }
        else
        {
          m_bits_in_current_byte = 0;
          m_current_byte = 0;
          retval = false;
          break;
        }
      }
      
      //- this part of the algorithm extracts individual bit
      if ( _num_bits > 0 && retval )
      {
        if ( _bits )
        {
          yat::BitsStorage bit_mask = 0x01;
          for ( int i = 1; i < _num_bits; i++ )
          {
            bit_mask = ( bit_mask << 1 ) | static_cast<yat::BitsStorage>(0x01);
          }
          *_bits |= m_current_byte & bit_mask;
        }
   
        m_current_byte = m_current_byte >> _num_bits;
        
        m_bits_in_current_byte -= _num_bits;
      }
    }
    
    return retval;
  }
};

//=============================================================================
// operator>> - extracts _n bits from source then puts result into dest.value
//=============================================================================
template <size_t _n, typename _T>
BitsStream& operator>> (BitsStream& source, BitsSet<_n, _T>& dest)
{
  yat::BitsStorage tmp = 0;

  source.read_bits(_n, tmp);

  _T t = static_cast<_T>(tmp);

  if ( source.endianness() != yat::Endianness::host_endianness )
  {
    switch ( sizeof(_T) )
    {
      case 2:
      {
        Endianness::swap_2(reinterpret_cast<const char*>(&t),
                             reinterpret_cast<char*>(&t));
      }
      break;
      case 4:
      {
        Endianness::swap_4(reinterpret_cast<const char*>(&t),
                           reinterpret_cast<char*>(&t));
      }
      break;
      case 8:
      {
        Endianness::swap_8(reinterpret_cast<const char*>(&t),
                           reinterpret_cast<char*>(&t));
      }
      break;
      case 16:
      {
        Endianness::swap_16(reinterpret_cast<const char*>(&t),
                            reinterpret_cast<char*>(&t));
      }
      break;
    }
  }

  dest.value() = t; 

  return source;
} 

//=============================================================================
// operator>> for array of elements of type T
//=============================================================================
template <typename _T>
BitsStream& operator>> (BitsStream& _source, const _T _dest[])
{
  for (size_t i = 0; i < sizeof(_dest)/sizeof(_T); ++i)
    _source >> _dest[i];
  return _source;
}
  
} //- namespace

#endif //- _BITS_STREAM_H_
