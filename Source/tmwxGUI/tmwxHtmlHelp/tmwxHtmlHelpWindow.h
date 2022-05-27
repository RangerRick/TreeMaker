/*******************************************************************************
File:         tmwxHtmlHelpWindow.h
Project:      TreeMaker 5.x
Purpose:      Header file for TreeMaker help window class
Author:       Robert J. Lang
Modified by:  Benjamin M. Reed
Created:      2022-11-27
Copyright:    ©2005 Robert J. Lang. All Rights Reserved.
*******************************************************************************/

#ifndef _TMWXHTMLHELPWINDOW_H_
#define _TMWXHTMLHELPWINDOW_H_

// Specific wxWidgets headers
#include "wx/html/helpwnd.h"

class tmwxHtmlHelpWindow : public wxHtmlHelpWindow
{
  DECLARE_DYNAMIC_CLASS(tmwxHtmlHelpWindow)

public:
  wxHtmlEasyPrinting* GetPrinter();
  void SetPrinter(wxHtmlEasyPrinting* printer);
};

#endif // _TMWXHTMLHELPWINDOW_H_
