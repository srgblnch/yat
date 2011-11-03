//----------------------------------------------------------------------------
// YAT LIBRARY
//----------------------------------------------------------------------------
//
// Copyright (C) 2006-2011  N.Leclercq & The Tango Community
//
// Part of the code comes from the ACE Framework (i386 asm bytes swaping code)
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

#ifndef _TASK_H_
#define _TASK_H_

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include <yat/threading/Thread.h>
#include <yat/threading/MessageQ.h>

// ============================================================================
// CONSTs
// ============================================================================
#define kDEFAULT_TASK_TMO_MSECS         5000
#define kDEFAULT_THD_PERIODIC_TMO_MSECS 1000
//-----------------------------------------------------------------------------

namespace yat
{

// ============================================================================
// class: Task
// ============================================================================
class YAT_DECL Task : public yat::Thread
{
  //! a yat::Task = an undetached yat::Thread + a yat::MessageQ
  //-TODO: write some doc!

public:

  //! yat::Task configuration class
  struct YAT_DECL Config
  {
    //- enable TIMEOUT messages
    bool enable_timeout_msg;
    //- timeout msg period in msec
    size_t timeout_msg_period_ms;
    //- enable PERIODIC messages
    bool enable_periodic_msg;
    //- periodic msg period in msec
    size_t periodic_msg_period_ms;
    //- should we process msg under critical section?
    //- not recommended! for backward compatibility only.
    bool lock_msg_handling;
    //- msgQ low water mark
    size_t lo_wm;
    //- msgQ high water mark
    size_t hi_wm;
    //- throw exception on post message timeout
    bool throw_on_post_tmo;
    //- user data (passed back in all msg)
    Thread::IOArg user_data;
    //- default ctor
    Config ();
    //- ctor
    Config (bool   enable_timeout_msg,
            size_t timeout_msg_period_ms,
            bool   enable_periodic_msg,
            size_t periodic_msg_period_ms,
            bool   lock_msg_handling,
            size_t lo_wm,
            size_t hi_wm,
            bool   throw_on_post_tmo,
            Thread::IOArg user_data);
  };

  //- default ctor
  Task ();
  
  //- config ctor
  Task (const Config& cfg);

  //- dtor
  virtual ~Task ();

  //- starts the task (same as go_synchronously - backward compatibility)
  virtual void go (size_t tmo_msecs = kDEFAULT_MSG_TMO_MSECS)
    throw (Exception);
    
  //- starts the task (same as go_synchronously - backward compatibility)
  //- an exception is thrown in case the specified message:
  //-   * is not of type TASK_INIT
  //-   * is not "waitable"
  virtual void go (Message * msg, size_t tmo_msecs = kDEFAULT_MSG_TMO_MSECS)
    throw (Exception);
  
  //- starts the task synchronously (i.e. wait for the INIT msg to be handled)
  virtual void go_synchronously (size_t tmo_msecs = kDEFAULT_MSG_TMO_MSECS)
    throw (Exception);
    
  //- starts the task synchronously (i.e. wait for the INIT msg to be handled) 
  //- an exception is thrown in case the specified message:
  //-   * is not of type TASK_INIT
  //-   * is not "waitable"
  virtual void go_synchronously (Message * msg, size_t tmo_msecs = kDEFAULT_MSG_TMO_MSECS)
    throw (Exception);

  //- starts the task asynchronously (i.e. does NOT wait for the INIT msg to be handled)
  virtual void go_asynchronously (size_t tmo_msecs = kDEFAULT_MSG_TMO_MSECS)
    throw (Exception);
    
  //- starts the task asynchronously (i.e.  does NOT wait for the INIT msg to be handled) 
  //- an exception is thrown in case the specified message:
  //-   * is not of type TASK_INIT
  virtual void go_asynchronously (Message * msg, size_t tmo_msecs = kDEFAULT_MSG_TMO_MSECS)
    throw (Exception);

  //! aborts the task (join with the underlying thread before returning).
  //! provides an implementation to the Thread::exit pure virtual method.
  virtual void exit ()
    throw (Exception);

  //- posts a message to the task
  void post (Message * msg, size_t tmo_msecs = kDEFAULT_POST_MSG_TMO)
    throw (Exception);
    
  //- posts the specified msg to the task then returns immediately (asynchronous approach)
  void post (size_t msg_type, size_t tmo_msecs = kDEFAULT_MSG_TMO_MSECS)
    throw (Exception);

  //- posts the specified data to the task then returns immediately (asynchronous approach)
  template <typename T> void post (size_t msg_type, 
                                   T * data, 
                                   bool transfer_ownership = true,  
                                   size_t tmo_msecs = kDEFAULT_MSG_TMO_MSECS)
    throw (Exception);

  //- posts the specified data to the task then returns immediately (asynchronous approach)
  template <typename T> void post (size_t msg_type,  
                                   const T & data,  
                                   size_t tmo_msecs = kDEFAULT_MSG_TMO_MSECS)
    throw (Exception);

  //- posts the specified msg to the task then waits for this message to be handled (synchronous approach)
  void wait_msg_handled (Message * msg, size_t tmo_msecs = kDEFAULT_MSG_TMO_MSECS)
    throw (Exception);

  //- posts the specified msg to the task then waits for this message to be handled (synchronous approach)
  void wait_msg_handled (size_t msg_type, size_t tmo_msecs = kDEFAULT_MSG_TMO_MSECS)
    throw (Exception);

  //- posts the specified data to the task then waits for the associated message to be handled (synchronous approach)
  template <typename T> void wait_msg_handled (size_t msg_type, 
                                               T * data, 
                                               bool transfer_ownership = true,  
                                               size_t tmo_msecs = kDEFAULT_MSG_TMO_MSECS)
    throw (Exception);

  //- posts the specified data to the task then waits for the associated message to be handled (synchronous approach)
  template <typename T> void wait_msg_handled (size_t msg_type,  
                                               const T & data,  
                                               size_t tmo_msecs = kDEFAULT_MSG_TMO_MSECS)
    throw (Exception);

  //- timeout msg period mutator
  void set_timeout_msg_period (size_t p_msecs);
  
  //- periodic msg period accessor
  size_t get_timeout_msg_period () const;
  
  //- enable/disable timeout messages
  void enable_timeout_msg (bool enable);

  //- returns timeout messages handling status
  bool timeout_msg_enabled () const;

  //- periodic msg period mutator
  void set_periodic_msg_period (size_t p_msecs);
  
  //- periodic msg period accessor
  size_t get_periodic_msg_period () const;

  //- enable/disable periodic messages
  void enable_periodic_msg (bool enable);

  //- returns period messages handling status
  bool periodic_msg_enabled () const;

  //- MsgQ water marks unit mutator
  void msgq_wm_unit (MessageQ::WmUnit _wmu);

  //- MsgQ water marks unit accessor
  MessageQ::WmUnit msgq_wm_unit () const;

  //- MsgQ low water mark mutator (MsgQ unit dependent)
  void msgq_lo_wm (size_t _lo_wm);
  
  //- MsgQ low water mark accessor (MsgQ unit dependent)
  size_t msgq_lo_wm () const;
  
  //- MsgQ high water mark mutator (MsgQ unit dependent)
  void msgq_hi_wm (size_t _hi_wm);
  
  //- MsgQ high water mark accessor (MsgQ unit dependent)
  size_t msgq_hi_wm () const;

  //- MsgQ high water mark accessor (MsgQ unit dependent)
  const MessageQ::Statistics & msgq_statistics ();
  
  //- Should the underlying MsgQ throw an exception on post msg tmo expiration?
  void throw_on_post_msg_timeout (bool _strategy);

protected:
  //- run_undetached
  virtual Thread::IOArg run_undetached (Thread::IOArg);
  
  //- message handler
  virtual void handle_message (yat::Message& msg)
     throw (yat::Exception) = 0;

  //- returns the underlying msgQ
  MessageQ & message_queue ();

private:
  //- actual_timeout
  size_t actual_timeout () const;

  //- the associated messageQ
  MessageQ msg_q_;

  //- timeout msg control flag
  bool timeout_msg_enabled_;

  //- timeout msg period
  size_t timeout_msg_period_ms_;

  //- periodic msg control flag
  bool periodic_msg_enabled_;

  //- periodic msg period 
  size_t periodic_msg_period_ms_;

  //- user data passed to entry point
  Thread::IOArg user_data_;

  //- should we process msg under critical section?
  bool lock_msg_handling_;

#if defined (YAT_DEBUG)
  //- some statistics counter
  unsigned long next_msg_counter;
  unsigned long user_msg_counter;
  unsigned long ctrl_msg_counter;
#endif
};

// ============================================================================
// Task::post
// ============================================================================
template <typename T> void Task::post (size_t msg_type, 
                                       T * data, 
                                       bool transfer_ownership,  
                                       size_t tmo_msecs)
  throw (Exception)
{
  Message * m = new (std::nothrow) Message(msg_type, DEFAULT_MSG_PRIORITY, false);
  if (! m)
  {
    THROW_YAT_ERROR("OUT_OF_MEMORY",
                    "yat::Message allocation failed",
                    "Task::post");
  }
  m->attach_data(data);
  this->post(m, tmo_msecs);
}
// ============================================================================
// Task::post
// ============================================================================
template <typename T> void Task::post (size_t msg_type,  
                                       const T & data,  
                                       size_t tmo_msecs)
  throw (Exception)
{
  Message * m = new (std::nothrow) Message(msg_type, DEFAULT_MSG_PRIORITY, false);
  if (! m)
  {
    THROW_YAT_ERROR("OUT_OF_MEMORY",
                    "yat::Message allocation failed",
                    "Task::post");
  }
  m->attach_data(data);
  this->post(m, tmo_msecs);
}
// ============================================================================
// Task::wait_msg_handled
// ============================================================================
  template <typename T> void Task::wait_msg_handled (size_t msg_type, 
                                                     T * data, 
                                                     bool transfer_ownership,  
                                                     size_t tmo_msecs)
    throw (Exception)
{
  Message * m = new (std::nothrow) Message(msg_type, DEFAULT_MSG_PRIORITY, true);
  if (! m)
  {
    THROW_YAT_ERROR("OUT_OF_MEMORY",
                    "yat::Message allocation failed",
                    "Task::post");
  }
  m->attach_data(data);
  this->wait_msg_handled(m, tmo_msecs);
}
// ============================================================================
// Task::wait_msg_handled
// ============================================================================
template <typename T> void Task::wait_msg_handled (size_t msg_type,  
                                                   const T & data,  
                                                   size_t tmo_msecs)
  throw (Exception)
{
  Message * m = new (std::nothrow) Message(msg_type, DEFAULT_MSG_PRIORITY, true);
  if (! m)
  {
    THROW_YAT_ERROR("OUT_OF_MEMORY",
                    "yat::Message allocation failed",
                    "Task::post");
  }
  m->attach_data(data);
  this->wait_msg_handled(m, tmo_msecs);
}

} // namespace

#if defined (YAT_INLINE_IMPL)
# include <yat/threading/Task.i>
#endif // YAT_INLINE_IMPL

#endif // _TASK_H_
