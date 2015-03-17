//----------------------------------------------------------------------------
// Copyright (c) 2004-2015 Synchrotron SOLEIL
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the GNU Lesser Public License v3
// which accompanies this distribution, and is available at
// http://www.gnu.org/licenses/lgpl.html
//----------------------------------------------------------------------------
/*!
 * \file     
 * \brief    An example of yat::any usage.
 * \author   N. Leclercq - Synchrotron SOLEIL
 */

#include <yat/threading/Mutex.h>
#include <yat/threading/Condition.h>
#include <yat/threading/Message.h>
#include <yat/any/Any.h>
#include <iostream>

int main(int argc, char* argv[])
{
  //- yat::Condition test ------------------------------
  //////////////////////////////////////////////////////

  yat::Mutex mx;
  mx.lock();
  yat::Condition cd(mx);

  yat::Timer t;
  cd.timed_wait(213);
  double elapsed_msec = t.elapsed_msec();
  std::cout << "yat::Condition::timed_wait::tmo expired after "
            << elapsed_msec
            << " ms"
            << std::endl;

  yat::Message * m = new yat::Message(0, DEFAULT_MSG_PRIORITY, true);
  t.restart();
  m->wait_processed(246);
  elapsed_msec = t.elapsed_msec();
  std::cout << "yat::Condition::timed_wait::tmo expired after "
            << elapsed_msec
            << " ms"
            << std::endl;
  m->release();


  //- yat::Any test ------------------------------------
  //////////////////////////////////////////////////////

  int a_int = 12345;

  int & a_int_ref = a_int;

  yat::Any any_holding_val(a_int);

  yat::Any any_holding_ptr(&a_int);

  yat::Any any_holding_ref(a_int_ref);

  //- try to extract an <int> from an Any holding an <int>
  //- this should complete successfully
  try 
  {
    int as_int_val = yat::any_cast<int>(any_holding_val);
    std::cout << "ok: yat::Any::any_cast<int> returned " 
              << as_int_val 
              << " as expected"
              << std::endl;
  }
  catch (const yat::Exception&)
  {
    std::cout << "ko: yat::Any::any_cast<int> failed!" << std::endl;
  }

  //- try to extract an <int*> from an Any holding an <int*>
  //- this should complete successfully
  try 
  {
    int * int_ptr = yat::any_cast<int*>(any_holding_ptr);

    std::cout << "ok: yat::Any::any_cast<int*> returned 0x"
              << std::hex 
              << int_ptr 
              << std::dec
              << " - value is: "
              << *int_ptr
              << " as expected"
              << std::endl;

  }
  catch (const yat::Exception&)
  {
    std::cout << "ko: yat::Any::any_cast<int*> failed!" << std::endl;
  }

  //- try to extract an <int> from an Any holding an <int&>
  //- this should complete successfully
  try 
  {
    int int_val = yat::any_cast<int>(any_holding_ref);

    std::cout << "ok: yat::Any::any_cast<int> returned "
              << int_val
              << " as expected"
              << std::endl;

  }
  catch (const yat::Exception&)
  {
    std::cout << "ko: yat::Any::any_cast<int> failed!" << std::endl;
  }

  //- try to extract an <int> from an Any holding an <int&>
  //- this should complete successfully
  try 
  {
    int int_val = yat::any_cast_ext<int>(any_holding_ref);

    std::cout << "ok: yat::Any::any_cast_ext<int> returned "
              << int_val
              << " as expected"
              << std::endl;

  }
  catch (const yat::Exception&)
  {
    std::cout << "ko: yat::Any::any_cast<int> failed!" << std::endl;
  }

  //- try to extract an <int> from an Any holding an <int*>
  //- this should fail!
  try 
  {
    int int_val = yat::any_cast<int>(any_holding_ptr);
    std::cout << "ko: yat::Any::any_cast<int> returned " 
              << int_val 
              << " - should have failed!"
              << std::endl;
  }
  catch (const yat::Exception&)
  {
    std::cout << "ok: yat::Any::any_cast<int> failed as expected" << std::endl;
  }

  //- try to extract an <int *> from an Any holding an <int>
  //- this should fail!
  try 
  {
    int * int_ptr = yat::any_cast<int*>(any_holding_val);

    std::cout << "ko: yat::Any::any_cast<int*> returned 0x"
              << std::hex 
              << int_ptr 
              << std::dec
              << " - value is: "
              << *int_ptr
              << " - should have failed!"
              << std::endl;
  }
  catch (const yat::Exception&)
  {
    std::cout << "ok: yat::Any::any_cast<int*> failed as expected" << std::endl;
  }

  //- try to extract an <int *> from an Any holding an <int&>
  //- this should fail!
  try 
  {
    int * int_ptr = yat::any_cast<int*>(any_holding_ref);

    std::cout << "ko: yat::Any::any_cast<int*> returned 0x"
              << std::hex 
              << int_ptr 
              << std::dec
              << " - value is: "
              << *int_ptr
              << " - should have failed!"
              << std::endl;
  }
  catch (const yat::Exception&)
  {
    std::cout << "ok: yat::Any::any_cast<int*> failed as expected" << std::endl;
  }

  return 0;
}

