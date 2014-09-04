//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
/*!
 * \file     
 * \brief    An example of yat::Task (and related classes) usage. .
 * \author   N. Leclercq, J. Malik - Synchrotron SOLEIL
 */

#ifndef _MY_TASK_H_
#define _MY_TASK_H_

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include <iostream>
#include <yat/threading/Task.h>

// ============================================================================
// SOME USER DEFINED MESSAGES
// ============================================================================
#define kDUMMY_MSG (yat::FIRST_USER_MSG + 1000)

// ============================================================================
// SOME USER DEFINED MESSAGE PRIORITIES
// ============================================================================
#define kDUMMY_MSG_PRIORITY adtb::MAX_USER_PRIORITY

// ============================================================================
// class: Consumer
// ============================================================================
class Consumer: public yat::Task
{
public:

	//- ctor ---------------------------------
	Consumer (size_t lo_wm, size_t hi_wm);

	//- dtor ---------------------------------
	virtual ~Consumer (void);

protected:
	//- handle_message -----------------------
	virtual void handle_message (yat::Message& msg);

private:
  //- num of ctrl msg received
  unsigned long ctrl_msg_counter;
  //- num of user msg received
  unsigned long user_msg_counter;

#if defined (YAT_DEBUG)
  //- id of the last received msg
  yat::Message::MessageID last_msg_id;
  //- num of lost msg 
  unsigned long lost_msg_counter;
  //- num msg received in wrong order
  unsigned long wrong_order_msg_counter;
#endif

};

#endif // _MY_TASK_H_
