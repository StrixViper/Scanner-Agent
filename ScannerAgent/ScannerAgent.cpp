#include <wx/wx.h>
#include <wx/file.h>
#include "MainFrame.h"
#include <sstream>
#include <iomanip>
#include <list>


enum IDs {
    BUTTON_ID = 2,
    CHOICE_ID = 3,
    TEXT_ID = 4,
    FILTER_BUTTON_ID = 5,
    SUSPICIOUS_BUTTON_ID = 6,
    CLEAR_BOX_BUTTON_ID = 7,
    PRINT_SMS_BUTTON_ID = 8,
    PRINT_EMAIL_BUTTON_ID = 9
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(BUTTON_ID, MainFrame::OnButtonClicked)
EVT_TEXT(TEXT_ID, MainFrame::OnTextChanged)
EVT_CHOICE(CHOICE_ID,MainFrame::OnChoiceChanged)
EVT_BUTTON(FILTER_BUTTON_ID, MainFrame::OnFilterButtonClicked)
EVT_BUTTON(SUSPICIOUS_BUTTON_ID,MainFrame::OnSuspiciousButtonClicked)
EVT_BUTTON(CLEAR_BOX_BUTTON_ID,MainFrame::ClearFilterBoxButtonClicked)
EVT_BUTTON(PRINT_SMS_BUTTON_ID,MainFrame::PrintSmsButtonClicked)
EVT_BUTTON(PRINT_EMAIL_BUTTON_ID, MainFrame::PrintEmailButtonClicked)
wxEND_EVENT_TABLE();


MainFrame::MainFrame(const wxString& title): wxFrame(nullptr,wxID_ANY,title) {
    
    panel = new wxPanel(this);

    wxButton* button = new wxButton(panel, BUTTON_ID, "Analyze", wxPoint(590, 147), wxSize(100, 35));

    wxStaticText* staticText = new wxStaticText(panel, wxID_ANY, "Agent Scanner", wxPoint(315, 50));

    // Create a font and set its size
    wxFont font = staticText->GetFont();
    font.SetPointSize(20); // Set to a larger size (you can adjust this number)
    staticText->SetFont(font);

   //wxTextCtrl* textCtrl = new wxTextCtrl(panel, TEXT_ID, "Insert Your Data Here: ", wxPoint(300, 145), wxSize(200, 100));
    textCtrl = new wxTextCtrl(panel, TEXT_ID, "Insert Your Data Here: ", wxPoint(230, 145), wxSize(350, 200), wxTE_MULTILINE);

    wxArrayString choices;
    choices.Add("SMS");
    choices.Add("Email");
    choices.Add("Both");
    choice = new wxChoice(panel, CHOICE_ID, wxPoint(130, 147), wxSize(100, -1), choices);

    CreateStatusBar();
}

void MainFrame::OpenParsedPanel() {
    if (!parsedWindow) {
        // Create a new window (frame)
        parsedWindow = new wxFrame(this, wxID_ANY, "Parsed Messages", wxDefaultPosition, wxSize(850, 900));
        parsedWindow->SetBackgroundColour(*wxWHITE);

        // Add a panel inside the new frame
        wxPanel* innerPanel = new wxPanel(parsedWindow);

        new wxStaticText(innerPanel, wxID_ANY, "Filter Messages", wxPoint(300, 10));

        wxStaticText* emailStaticText = new wxStaticText(innerPanel, wxID_ANY, "Filter Emails", wxPoint(50, 50));
        bySenderEmails = new wxCheckBox(innerPanel, wxID_ANY, "by sender email", wxPoint(50, 100));
        emailFilterSenderEmailTextCtrl = new wxTextCtrl(innerPanel, wxID_ANY, "sender email:", wxPoint(160, 100), wxSize(200, 30), wxTE_MULTILINE);
        byRecieverEmails = new wxCheckBox(innerPanel, wxID_ANY, "by reciever email", wxPoint(50, 150));
        emailFilterRecieverEmailTextCtrl = new wxTextCtrl(innerPanel, wxID_ANY, "reciever email:", wxPoint(160, 150), wxSize(200, 30), wxTE_MULTILINE);
        byDayEmails = new wxCheckBox(innerPanel, wxID_ANY, "by day email", wxPoint(50, 200));
        emailFilterDayTextCtrl = new wxTextCtrl(innerPanel, wxID_ANY, "", wxPoint(170, 200), wxSize(40, 30), wxTE_MULTILINE);
        byDayInWeekEmails = new wxCheckBox(innerPanel, wxID_ANY, "by day in week email", wxPoint(50, 250));
        emailFilterDayInWeekTextCtrl = new wxTextCtrl(innerPanel, wxID_ANY, "", wxPoint(190, 250), wxSize(40, 30), wxTE_MULTILINE);
        byMonthEmails = new wxCheckBox(innerPanel, wxID_ANY, "by month email", wxPoint(50, 300));
        emailFilterMonthTextCtrl = new wxTextCtrl(innerPanel, wxID_ANY, "", wxPoint(170, 300), wxSize(40, 30), wxTE_MULTILINE);
        byYearEmails = new wxCheckBox(innerPanel, wxID_ANY, "by year email", wxPoint(50, 350));
        emailFilterYearTextCtrl = new wxTextCtrl(innerPanel, wxID_ANY, "", wxPoint(170, 350), wxSize(40, 30), wxTE_MULTILINE);
        bySubjectEmails = new wxCheckBox(innerPanel, wxID_ANY, "by subject email", wxPoint(50, 400));
        emailFilterSubjectTextCtrl = new wxTextCtrl(innerPanel, wxID_ANY, "Subject: ", wxPoint(180, 400), wxSize(200, 40), wxTE_MULTILINE);
        byContentEmails = new wxCheckBox(innerPanel, wxID_ANY, "by content email", wxPoint(50, 450));
        emailFilterContentTextCtrl = new wxTextCtrl(innerPanel, wxID_ANY, "Content: ", wxPoint(180, 450), wxSize(200, 40), wxTE_MULTILINE);

        wxStaticText* smsStaticText = new wxStaticText(innerPanel, wxID_ANY, "Filter SMS", wxPoint(450, 50));
        bySenderSms = new wxCheckBox(innerPanel, wxID_ANY, "by sender SMS", wxPoint(450, 100));
        smsFilterSenderPhoneNumberTextCtrl = new wxTextCtrl(innerPanel, wxID_ANY, "sender phone number", wxPoint(570, 100), wxSize(200, 30), wxTE_MULTILINE);
        byRecieverSms = new wxCheckBox(innerPanel, wxID_ANY, "by reciever SMS", wxPoint(450, 150));
        smsFilterRecieverPhoneNumberTextCtrl = new wxTextCtrl(innerPanel, wxID_ANY, "reciever phone number", wxPoint(570, 150), wxSize(200, 30), wxTE_MULTILINE);
        byDaySms = new wxCheckBox(innerPanel, wxID_ANY, "by day SMS", wxPoint(450, 200));
        smsFilterDayTextCtrl = new wxTextCtrl(innerPanel, wxID_ANY, "", wxPoint(570, 200), wxSize(40, 30), wxTE_MULTILINE);
        byDayInWeekSms = new wxCheckBox(innerPanel, wxID_ANY, "by day In Week SMS:", wxPoint(450, 250));
        smsFilterDayInWeekTextCtrl = new wxTextCtrl(innerPanel, wxID_ANY, "", wxPoint(600, 250), wxSize(40, 30), wxTE_MULTILINE);
        byMonthSms = new wxCheckBox(innerPanel, wxID_ANY, "by month SMS", wxPoint(450, 300));
        smsFilterMonthTextCtrl = new wxTextCtrl(innerPanel, wxID_ANY, "", wxPoint(580, 300), wxSize(40, 30), wxTE_MULTILINE);
        byYearSms = new wxCheckBox(innerPanel, wxID_ANY, "by year SMS", wxPoint(450, 350));
        smsFilterYearTextCtrl = new wxTextCtrl(innerPanel, wxID_ANY, "", wxPoint(580, 350), wxSize(40, 30), wxTE_MULTILINE);
        byContentSms = new wxCheckBox(innerPanel, wxID_ANY, "by content SMS", wxPoint(450, 400));
        smsFilterContentTextCtrl = new wxTextCtrl(innerPanel, wxID_ANY, "Content: ", wxPoint(580, 400), wxSize(200, 40), wxTE_MULTILINE);

        wxButton* filterButton = new wxButton(innerPanel, FILTER_BUTTON_ID, "Filter", wxPoint(310, 530));
        wxButton* trackButton = new wxButton(innerPanel, SUSPICIOUS_BUTTON_ID, "Track", wxPoint(385, 530));
        wxButton* clearButton = new wxButton(innerPanel, CLEAR_BOX_BUTTON_ID, "Clear", wxPoint(460, 530));

        emailFilterResultsBox = new wxTextCtrl(innerPanel, wxID_ANY, "", wxPoint(50, 600), wxSize(700, 70), wxTE_MULTILINE | wxTE_READONLY);
        smsFilterResultsBox = new wxTextCtrl(innerPanel, wxID_ANY, "", wxPoint(50, 700), wxSize(700, 70), wxTE_MULTILINE | wxTE_READONLY);

        wxButton* printEmailButton = new wxButton(innerPanel, PRINT_EMAIL_BUTTON_ID, "Print Email", wxPoint(355, 780));
        wxButton* printSmsButton = new wxButton(innerPanel, PRINT_SMS_BUTTON_ID, "Print Sms", wxPoint(430, 780));
    }
    wxLogMessage("Total parsed emails: %d\nTotal parsed messages: %d", messageManager.EmailsCount,messageManager.SmsCount);

    parsedWindow->Center();
    parsedWindow->Show();     // Show the new window
    parsedWindow->Raise();    // Bring to front
}

void MainFrame::OnFilterButtonClicked(wxCommandEvent& evt) {
    wxLogMessage("Filter button clicked.");
    #pragma region Filtering
    //filtering with the optimization approach:
    EmailFilterCriteria emailCriteria;
    if (bySenderEmails->IsChecked()) {
        emailCriteria.filterBySender = true;
        emailCriteria.sender = emailFilterSenderEmailTextCtrl->GetValue().ToStdString();
    }
    if (byRecieverEmails->IsChecked())
    {
        emailCriteria.filterByReceiver = true;
        emailCriteria.receiver = emailFilterRecieverEmailTextCtrl->GetValue().ToStdString();
    }
    if (byDayEmails->IsChecked()) {
        emailCriteria.filterByDay = true;
        emailCriteria.day = std::stoi(emailFilterDayTextCtrl->GetValue().ToStdString());
    }
    if (byDayInWeekEmails->IsChecked()) {
        emailCriteria.filterByWeekday = true;
        emailCriteria.weekday = std::stoi(emailFilterDayInWeekTextCtrl->GetValue().ToStdString());
    }
    if (byMonthEmails->IsChecked()) {
        emailCriteria.filterByMonth = true;
        emailCriteria.month = std::stoi(emailFilterMonthTextCtrl->GetValue().ToStdString());
    }
    if (byYearEmails->IsChecked()) {
        emailCriteria.filterByYear = true;
        emailCriteria.year = std::stoi(emailFilterYearTextCtrl->GetValue().ToStdString());
    }
    if (bySubjectEmails->IsChecked()) {
        emailCriteria.filterBySubject = true;
        emailCriteria.subject = emailFilterSubjectTextCtrl->GetValue().ToStdString();
    }
    if (byContentEmails->IsChecked()) {
        emailCriteria.filterByContent = true;
        emailCriteria.content = emailFilterContentTextCtrl->GetValue().ToStdString();
    }
    messageManager.FilterEmails(emailCriteria);

    //filtering with the optimized sms function
    SmsFilterCriteria smsCriteria;
    if (bySenderSms->IsChecked()) {
        smsCriteria.filterBySender = true;
        smsCriteria.sender = smsFilterSenderPhoneNumberTextCtrl->GetValue().ToStdString();
    }
    if (byRecieverSms->IsChecked())
    {
        smsCriteria.filterByReceiver = true;
        smsCriteria.receiver = smsFilterRecieverPhoneNumberTextCtrl->GetValue().ToStdString();
    }
    if (byDaySms->IsChecked()) {
        smsCriteria.filterByDay = true;
        smsCriteria.day = std::stoi(smsFilterDayTextCtrl->GetValue().ToStdString());
    }
    if (byDayInWeekSms->IsChecked()) {
        smsCriteria.filterByWeekday = true;
        smsCriteria.weekday = std::stoi(smsFilterDayInWeekTextCtrl->GetValue().ToStdString());
    }
    if (byMonthSms->IsChecked()) {
        smsCriteria.filterByMonth = true;
        smsCriteria.month = std::stoi(smsFilterMonthTextCtrl->GetValue().ToStdString());
    }
    if (byYearSms->IsChecked()) {
        smsCriteria.filterByYear = true;
        smsCriteria.year = std::stoi(smsFilterYearTextCtrl->GetValue().ToStdString());
    }
    if (byContentSms->IsChecked()) {
        smsCriteria.filterByContent = true;
        smsCriteria.content = smsFilterContentTextCtrl->GetValue().ToStdString();
    }
    messageManager.FilterSms(smsCriteria);
#pragma endregion
    std::string emailResult;
    for (int i = 0; i < messageManager.filteredEmailsCount; i++)
    {
        emailResult += messageManager.filteredEmails.at(i).ReturnEmailMessage() + "\n-----------------\n";
    }
    std::string smsResult;
    for (int i = 0; i < messageManager.filteredSmsCount; i++)
    {
        smsResult += messageManager.filteredSms.at(i).ReturnSmsMessage() + "\n-----------------\n";
    }

    emailFilterResultsBox->SetValue(emailResult.empty() ? "No matching emails." : wxString::FromUTF8(emailResult));
    smsFilterResultsBox->SetValue(smsResult.empty() ? "No matching sms." : wxString::FromUTF8(smsResult));

}

void MainFrame::OnSuspiciousButtonClicked(wxCommandEvent& evt) {
    
    messageManager.TrackSuspiciousMessages();
    std::string emailResult;
    for (int i = 0; i < messageManager.suspiciousEmailsCount; i++)
    {
        emailResult += messageManager.suspiciousEmails.at(i).ReturnEmailMessage() + "\n-----------------\n";
    }
    std::string smsResult;
    for (int i = 0; i < messageManager.suspiciousSmsCount; i++)
    {
        smsResult += messageManager.suspiciousSms.at(i).ReturnSmsMessage() + "\n-----------------\n";
    }
    emailFilterResultsBox->SetValue(emailResult.empty() ? "No matching emails." : wxString::FromUTF8(emailResult));
    smsFilterResultsBox->SetValue(smsResult.empty() ? "No matching sms." : wxString::FromUTF8(smsResult));
}

void MainFrame::ClearFilterBoxButtonClicked(wxCommandEvent& evt) {
    emailFilterResultsBox->SetValue("");
    smsFilterResultsBox->SetValue("");
}

void MainFrame::OnButtonClicked(wxCommandEvent& evt) {

    if (choice->GetSelection() == wxNOT_FOUND) {
        wxLogError("Please select a message type.");
        return;
    }

    wxString input = GetText();              // from textCtrl
    wxString selectedType = GetSelectedChoice(); // from dropdown

    HandleUserInput(selectedType, input);
    OpenParsedPanel();
}

void MainFrame::OnChoiceChanged(wxCommandEvent& event)
{
    int selection = event.GetSelection();
    wxString selected = event.GetString();
}

void MainFrame::OnTextChanged(wxCommandEvent& evt) {
    wxString str = wxString::Format("Text: %s", evt.GetString());
}

wxString MainFrame::GetText() const {
    return textCtrl->GetValue();
}

wxString MainFrame::GetSelectedChoice() const {
    return choice->GetString(choice->GetSelection());
}

void MainFrame::HandleUserInput(wxString choice, wxString input) {
    std::string inputStr = input.ToStdString();
    std::string typeStr = choice.ToStdString();

    std::string output;

    if (typeStr == "SMS") {
        messageManager.ParseSMSs(inputStr);
        for (const Message& msg : messageManager.SmsMessages) {
            output += msg.ReturnSmsMessage() + "\n";
        }
    }
    else if (typeStr == "Email") {
        messageManager.ParseEmails(inputStr);
        for (const Message& msg : messageManager.EmailMessages) {
            output += msg.ReturnEmailMessage() + "\n";
        }
    }
    else if (typeStr == "Both") {
        messageManager.ParseCombinedSmsAndEmail(inputStr);
    
        for (const Message& msg : messageManager.SmsMessages) {
            output += msg.ReturnSmsMessage() + "\n";
        }
        for (const Message& msg : messageManager.EmailMessages) {
            output += msg.ReturnEmailMessage() + "\n";
        }
    }
    else
    {
        wxMessageBox("No valid Option was choosed.", "Error", wxOK | wxICON_ERROR);
    }
    
    if(output.empty()) {
        wxMessageBox("No valid messages were parsed.", "Error", wxOK | wxICON_ERROR);
    }
}

void PrintToPDF_Windows(const wxString& filePath) {
    wxString command = wxString::Format(
        "powershell -Command \"Start-Process -FilePath '%s' -Verb Print\"",
        filePath);

    wxExecute(command, wxEXEC_SYNC);
}

void ExportTextToFile(const wxString& message) {
    wxString tempFile = "temp_output.txt";

    // Step 1: Save the wxString to a file
    wxFile file;
    if (file.Create(tempFile, true)) {
        file.Write(message);
        file.Close();
    }
    else {
        wxLogError("Failed to create temporary file.");
        return;
    }

    // Step 2: Send to default printer (should be Microsoft Print to PDF)
    PrintToPDF_Windows(tempFile);
}

void MainFrame::PrintEmailButtonClicked(wxCommandEvent& evt) {
    ExportTextToFile(emailFilterResultsBox->GetValue());
    PrintToPDF_Windows("temp_output.txt");
}

void MainFrame::PrintSmsButtonClicked(wxCommandEvent& evt) {
    ExportTextToFile(smsFilterResultsBox->GetValue());
    PrintToPDF_Windows("temp_output.txt");
}