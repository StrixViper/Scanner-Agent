#pragma once
#include <wx/wx.h>
#include "MessageManager.h"

class MainFrame : public wxFrame
{
public: 
	MainFrame(const wxString& title);
private:
	wxTextCtrl* textCtrl;  // Store the input box here
	wxChoice* choice;       // Add this for the dropdown
	MessageManager messageManager;

	wxPanel* panel = nullptr;
	wxFrame* parsedWindow = nullptr; // Instead of wxPanel
	// Email filters
	wxCheckBox* bySenderEmails = nullptr;
	wxCheckBox* byRecieverEmails = nullptr;
	wxCheckBox* byDayEmails = nullptr;
	wxCheckBox* byDayInWeekEmails = nullptr;
	wxCheckBox* byMonthEmails = nullptr;
	wxCheckBox* byYearEmails = nullptr;
	wxCheckBox* bySubjectEmails = nullptr;
	wxCheckBox* byContentEmails = nullptr;
	wxTextCtrl* emailFilterSenderEmailTextCtrl = nullptr;
	wxTextCtrl* emailFilterRecieverEmailTextCtrl = nullptr;
	wxTextCtrl* emailFilterSubjectTextCtrl = nullptr;
	wxTextCtrl* emailFilterContentTextCtrl = nullptr;
	wxTextCtrl* emailFilterDayTextCtrl = nullptr;
	wxTextCtrl* emailFilterDayInWeekTextCtrl = nullptr;
	wxTextCtrl* emailFilterMonthTextCtrl = nullptr;
	wxTextCtrl* emailFilterYearTextCtrl = nullptr;

	// SMS filters
	wxCheckBox* bySenderSms = nullptr;
	wxCheckBox* byRecieverSms = nullptr;
	wxCheckBox* byDaySms = nullptr;
	wxCheckBox* byDayInWeekSms = nullptr;
	wxCheckBox* byMonthSms = nullptr;
	wxCheckBox* byYearSms = nullptr;
	wxCheckBox* bySubjectSms = nullptr;
	wxCheckBox* byContentSms = nullptr;
	wxTextCtrl* smsFilterSenderPhoneNumberTextCtrl = nullptr;
	wxTextCtrl* smsFilterRecieverPhoneNumberTextCtrl = nullptr;
	wxTextCtrl* smsFilterContentTextCtrl = nullptr;
	wxTextCtrl* smsFilterDayTextCtrl = nullptr;
	wxTextCtrl* smsFilterDayInWeekTextCtrl = nullptr;
	wxTextCtrl* smsFilterMonthTextCtrl = nullptr;
	wxTextCtrl* smsFilterYearTextCtrl = nullptr;

	wxTextCtrl* emailFilterResultsBox = nullptr;
	wxTextCtrl* smsFilterResultsBox = nullptr;


	void OnButtonClicked(wxCommandEvent& evt);
	void OnTextChanged(wxCommandEvent& evt);
	void OnChoiceChanged(wxCommandEvent& evt);

	void OnFilterButtonClicked(wxCommandEvent& evt);
	void OnSuspiciousButtonClicked(wxCommandEvent& evt);
	void ClearFilterBoxButtonClicked(wxCommandEvent& evt);

	void PrintSmsButtonClicked(wxCommandEvent& evt);
	void PrintEmailButtonClicked(wxCommandEvent& evt);


	wxString GetText() const;
	wxString GetSelectedChoice() const;

	void OpenParsedPanel(); // function to create and show the parsed panel

	void HandleUserInput(wxString choice, wxString input);

	wxDECLARE_EVENT_TABLE();
};

