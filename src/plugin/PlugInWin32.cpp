//----------------------------------------------------------------------------
// Copyright (c) 2004-2014 Synchrotron SOLEIL
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the GNU Lesser Public License v3
// which accompanies this distribution, and is available at
// http://www.gnu.org/licenses/lgpl.html
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// YAT LIBRARY
//----------------------------------------------------------------------------
//
// Copyright (C) 2006-2014 The Tango Community
//
// Part of the code comes from the ACE Framework (asm bytes swaping code)
// see http://www.cs.wustl.edu/~schmidt/ACE.html for more about ACE
//
// The thread native implementation has been initially inspired by omniThread
// - the threading support library that comes with omniORB. 
// see http://omniorb.sourceforge.net/ for more about omniORB.
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

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include <yat/plugin/PlugIn.h>

#ifdef YAT_WIN32


namespace yat
{

//-----------------------------------------------------------------------------
// PlugIn::do_load_library
//-----------------------------------------------------------------------------
PlugIn::LibraryHandle PlugIn::do_load_library( const std::string &library_file_name )
{
  PlugIn::LibraryHandle h = ::LoadLibrary( library_file_name.c_str() );
  if( NULL == h )
  {
    THROW_YAT_ERROR(std::string("SHAREDLIBRARY_ERROR"),
                    get_last_error_detail(),
                    std::string("PlugIn::load_library"));
  }
  return h;
}

//-----------------------------------------------------------------------------
// PlugIn::do_release_library
//-----------------------------------------------------------------------------
void PlugIn::do_release_library()
{
  ::FreeLibrary( static_cast<HINSTANCE>(m_libraryHandle) );
}

//-----------------------------------------------------------------------------
// PlugIn::do_find_symbol
//-----------------------------------------------------------------------------
PlugIn::Symbol PlugIn::do_find_symbol( const std::string &symbol )
{
  FARPROC far_proc = ::GetProcAddress( static_cast<HINSTANCE>(m_libraryHandle), 
                                       symbol.c_str() );
  return static_cast<PlugIn::Symbol>(far_proc);
}

//-----------------------------------------------------------------------------
// PlugIn::get_last_error_detail
//-----------------------------------------------------------------------------
std::string PlugIn::get_last_error_detail() const
{
  LPVOID lp_msg_buf;
  ::FormatMessage( 
                  FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                  FORMAT_MESSAGE_FROM_SYSTEM | 
                  FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL,
                  ::GetLastError(),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                  (LPTSTR) &lp_msg_buf,
                  0,
                  NULL 
                  );

  std::string message = (LPCTSTR)lp_msg_buf;

  // Free the buffer.
  ::LocalFree( lp_msg_buf );

  return message;
}

}


#endif