/*******************************************************************************
File:         tmwxOptimizerDialog_mac.cpp
Project:      TreeMaker 5.x
Purpose:      Source file for class tmwxOptimizerDialog, specialization for Mac
Author:       Robert J. Lang
Modified by:
Created:      2004-04-22
Copyright:    (c)2004 Robert J. Lang. All Rights Reserved.
*******************************************************************************/

#ifdef __WXMAC__

#include "tmwxOptimizerDialog.h"

/**********
class tmwxOptimizerDialog
Dialog for long, cancellable calculations.
wx3 Cocoa implementation replaces the old Carbon-based version.
**********/

void tmwxOptimizerDialog::DoStartModal()
{
  ::wxBeginBusyCursor();
  Show(true);
  SetFocus();
  mWindowDisabler = new wxWindowDisabler(this);
}


void tmwxOptimizerDialog::DoEventLoopOnce()
{
  wxYield();
}


void tmwxOptimizerDialog::DoFinishModal()
{
  delete mWindowDisabler;
  mWindowDisabler = nullptr;
  Show(false);
  ::wxEndBusyCursor();
}

#endif // __WXMAC__
