//----------------------------------------------------------------------------
// Copyright (c) 2004-2015 Synchrotron SOLEIL
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the GNU Lesser Public License v3
// which accompanies this distribution, and is available at
// http://www.gnu.org/licenses/lgpl.html
//----------------------------------------------------------------------------
/*!
 * \file     
 * \brief    An advanced yat example
 * \author   N. Leclercq - Synchrotron SOLEIL
 */

#pragma once

// ============================================================================
// Dependencies
// ============================================================================
#include <iostream>
#include <yat/threading/Task.h>

// ============================================================================
// SOME USER DEFINED MESSAGES
// ============================================================================


// ============================================================================
// class: Producer
// ============================================================================
class Producer: public yat::Task
{
public:

	//- ctor ---------------------------------
	Producer ();

	//- dtor ---------------------------------
	virtual ~Producer (void);

protected:
	//- handle_message -----------------------
	virtual void handle_message (yat::Message& msg);

private:
};
