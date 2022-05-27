/*******************************************************************************
File:         tmwxHtmlHelpFrame.cpp
Project:      TreeMaker 5.x
Purpose:      Implementation file for class tmwxHtmlHelpFrame
Author:       Robert J. Lang
Modified by:  
Created:      2005-11-23
Copyright:    ©2005 Robert J. Lang. All Rights Reserved.
*******************************************************************************/

#include "tmwxHtmlHelpFrame.h"
#include "tmwxHtmlHelpWindow.h"
#include "tmwxApp.h"

/*****
Constructor
*****/
tmwxHtmlHelpFrame::tmwxHtmlHelpFrame(wxHtmlHelpData* data) : 
  wxHtmlHelpFrame(data)
{
}


/*****
Handle an activate event
*****/
void tmwxHtmlHelpFrame::OnActivate(wxActivateEvent& event)
{
  wxHtmlHelpFrame::OnActivate(event);
  if (event.GetActive()) {
    wxTheApp->SetTopWindow(this);
  }
}


/*****
Set the printer that goes with this frame. This lets us configure the printer
from within the tmwxHtmlHelpController class.
*****/
void tmwxHtmlHelpFrame::SetHtmlHelpPrinter(wxHtmlEasyPrinting* printer)
{
  tmwxHtmlHelpWindow* const parent = (tmwxHtmlHelpWindow*)this->GetParent();
  parent->SetPrinter(printer);
}


/*****
Enable the File->Print... command
*****/
void tmwxHtmlHelpFrame::OnPrintUpdateUI(wxUpdateUIEvent& event)
{
  event.Enable(true);
}


/*****
Perform the File->Print... command
*****/
void tmwxHtmlHelpFrame::OnPrint(wxCommandEvent& event)
{
  tmwxHtmlHelpWindow* const parent = (tmwxHtmlHelpWindow*)this->GetParent();
  wxHtmlWindow* htmlWin = parent->GetHtmlWindow();
  TMASSERT(!!htmlWin->GetOpenedPage());
  wxHtmlEasyPrinting* printer = parent->GetPrinter();
  printer->PrintFile(htmlWin->GetOpenedPage());
}


/*****
Enable the File->Print Preview... command
*****/
void tmwxHtmlHelpFrame::OnPrintPreviewUpdateUI(wxUpdateUIEvent& event)
{
  event.Enable(true);
}


/*****
Perform the File->Print Preview... command
*****/
void tmwxHtmlHelpFrame::OnPrintPreview(wxCommandEvent& event)
{
  tmwxHtmlHelpWindow* const parent = (tmwxHtmlHelpWindow*)this->GetParent();
  wxHtmlWindow* htmlWin = parent->GetHtmlWindow();
  TMASSERT(!!htmlWin->GetOpenedPage());
  wxHtmlEasyPrinting* printer = parent->GetPrinter();
  printer->PreviewFile(htmlWin->GetOpenedPage());
}


/*****
Event table
*****/
BEGIN_EVENT_TABLE(tmwxHtmlHelpFrame, wxHtmlHelpFrame)
  EVT_ACTIVATE(tmwxHtmlHelpFrame::OnActivate)
  EVT_UPDATE_UI(wxID_PREVIEW, tmwxHtmlHelpFrame::OnPrintPreviewUpdateUI)
  EVT_MENU(wxID_PREVIEW, tmwxHtmlHelpFrame::OnPrintPreview)
  EVT_UPDATE_UI(wxID_PRINT, tmwxHtmlHelpFrame::OnPrintUpdateUI)
  EVT_MENU(wxID_PRINT, tmwxHtmlHelpFrame::OnPrint)
END_EVENT_TABLE()
