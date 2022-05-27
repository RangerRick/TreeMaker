/*******************************************************************************
File:         tmwxHtmlHelpWindow.cpp
Project:      TreeMaker 5.x
Purpose:      Implementation file for class tmwxHtmlHelpWindow
Author:       Robert J. Lang
Modified by:  Benjamin M. Reed
Created:      2022-05-27
Copyright:    ©2022 Robert J. Lang. All Rights Reserved.
*******************************************************************************/

#include "tmwxHtmlHelpWindow.h"
#include "tmwxApp.h"

wxHtmlEasyPrinting* tmwxHtmlHelpWindow::GetPrinter()
{
  TMASSERT(m_Printer);
  return m_Printer;
}

void tmwxHtmlHelpWindow::SetPrinter(wxHtmlEasyPrinting* printer)
{
  if (m_Printer) delete m_Printer;
  m_Printer = printer;
}
