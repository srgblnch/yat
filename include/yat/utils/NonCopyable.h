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

#ifndef _YAT_NONCOPYABLE_H_
#define _YAT_NONCOPYABLE_H_

//=============================================================================
// DEPENDENCIES
//=============================================================================
#include <yat/CommonHeader.h>

namespace yat 
{

// ============================================================================
//! \class NonCopyable 
//! \brief Non "copyable" class.
//!
//! Copy constructor and operator= are private members.
//! Inherit from this class to use its "non copyable" capability.
// ============================================================================
class NonCopyable
{
protected:
  //! Default constructor.
  NonCopyable () {}

  //! Destructor.
 ~NonCopyable () {}

private:
  NonCopyable ( const NonCopyable& );
  const NonCopyable& operator= ( const NonCopyable& );
};
  
} //- namespace

#endif // _YAT_NONCOPYABLE_H_
