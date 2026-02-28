/*******************************************************************************
File:         tmwxOptimizerDialog_gtk.cpp
Project:      TreeMaker 5.x
Purpose:      Source file for class tmwxOptimizerDialog, specialization for GTK
Author:       Carlos A. Furuti (original wx2 GTK version)
Modified by:
Created:      2004-04-22
Copyright:    (c)2004 Carlos A. Furuti. All Rights Reserved.
*******************************************************************************/

#ifdef __WXGTK__

#include "tmwxOptimizerDialog.h"

void tmwxOptimizerDialog::DoStartModal()
{
  ::wxBeginBusyCursor();
  Show(true);
  SetFocus();
}


void tmwxOptimizerDialog::DoEventLoopOnce()
{
  wxYield();
}


void tmwxOptimizerDialog::DoFinishModal()
{
  Show(false);
  ::wxEndBusyCursor();
}

#endif // __WXGTK__
