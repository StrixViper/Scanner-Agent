#pragma once
#include "Message.h"
#include <map>

struct EmailFilterCriteria {
    bool filterBySender = false;
    std::string sender;

    bool filterByReceiver = false;
    std::string receiver;

    bool filterByDay = false;
    int day;

    bool filterByWeekday = false;
    int weekday;

    bool filterByMonth = false;
    int month;

    bool filterByYear = false;
    int year;

    bool filterBySubject = false;
    std::string subject;

    bool filterByContent = false;
    std::string content;
};

struct SmsFilterCriteria {
    bool filterBySender = false;
    std::string sender;

    bool filterByReceiver = false;
    std::string receiver;

    bool filterByDay = false;
    int day;

    bool filterByWeekday = false;
    int weekday;

    bool filterByMonth = false;
    int month;

    bool filterByYear = false;
    int year;

    bool filterByContent = false;
    std::string content;
};

typedef struct Node {
    std::string sender;
    struct Node* reciever;
};

class MessageManager {
public:
    std::vector<Message> SmsMessages;
    int SmsCount = 0;
    std::vector<Message> EmailMessages;
    int EmailsCount = 0;

    std::vector<Message> filteredEmails;
    int filteredEmailsCount = 0;

    std::vector<Message> filteredSms;
    int filteredSmsCount = 0;

    std::vector<Message> suspiciousEmails;
    int suspiciousEmailsCount = 0;

    std::vector<Message> suspiciousSms;
    int suspiciousSmsCount = 0;

    Message ParseEmailIntoMessage(std::string email) {

        /*working with the RFC 5322 email format
        From: john@example.com
        To: jane@example.com
        Date: Wed, 09 Jul 2025 17:58 : 00 + 0300
        Subject: Meeting
        Hello Jane, let's meet tomorrow.*/

        std::string senderEmail;
        std::string receiverEmail;
        time_t sendDate;
        std::string emailSubject;
        std::string emailContent;

        std::stringstream ss(email);
        std::string line;
        int lineCounter = 1;
        std::string dateString;

        while (std::getline(ss, line)) {
            switch (lineCounter) {
            case 1:  // From: ...
                senderEmail = line.substr(6); // Skips "From: "
                break;
            case 2:  // To: ...
                receiverEmail = line.substr(4); // Skips "To: "
                break;
            case 3:  // Date: ...
                dateString = line.substr(6); // Skips "Date : "
                break;
            case 4:  // Subject: ...
                emailSubject = line.substr(9); // Skips "Subject : "
                break;
            default: // Body content
                if (!emailContent.empty()) emailContent += "\n";
                emailContent += line;
                break;
            }
            lineCounter++;
        }

        // Convert date string into time_t
        // Expected format: "Wed, 09 Jul 2025 17:58:00 +0300"
        std::tm tm = {};
        std::istringstream dateStream(dateString);
        dateStream >> std::get_time(&tm, "%a, %d %b %Y %H:%M:%S");
        sendDate = mktime(&tm); // Converts tm to time_t

        // If the subject is relevant, you might append it to the content
        if (!emailSubject.empty()) {
            emailContent = "Subject: " + emailSubject + "\n" + emailContent;
        }

        return Message(sendDate, emailContent, senderEmail, receiverEmail, emailSubject);
    }

    void ParseEmails(std::string emails) {

        std::string currentMessage = "";
        std::string currentLine = "";
        std::stringstream ss(emails);

        while (std::getline(ss, currentLine))
        {
            if (currentLine == "===")
            {
                if (!currentMessage.empty())
                {
                    Message newParsedMessage = ParseEmailIntoMessage(currentMessage);
                    EmailMessages.push_back(newParsedMessage); 
                    EmailsCount++;
                    currentMessage.clear();
                }
            }
            else
            {
                currentMessage += currentLine + "\n";
            }
        }
        if (!currentMessage.empty())
        {
            Message newParsedMessage = ParseEmailIntoMessage(currentMessage);
            EmailMessages.push_back(newParsedMessage);
            EmailsCount++;
        }

    }

    Message ParseSmsIntoMessage(std::string smsRaw) {

        //working with the format:
        //From: 05045040604
        //To: 0535839535
        //Time: 15:24
        //Message: ....

        std::string senderPhoneNumber;
        std::string receiverPhoneNumber;
        std::string sendDateStr;
        std::string messageContent;

        std::stringstream ss(smsRaw);
        std::string line;
        int lineCounter = 1;

        while (std::getline(ss, line)) {
            switch (lineCounter) {
            case 1: // From: ...
                senderPhoneNumber = line.substr(6); // after "From: "
                break;
            case 2: // To: ...
                receiverPhoneNumber = line.substr(4); // after "To: "
                break;
            case 3: // Time: ...
                sendDateStr = line.substr(6); // after "Time: "
                break;
            case 4: // Message: ...
                messageContent = line.substr(9); // after "Message: "
                break;
            default:
                // If message spans multiple lines, append them
                messageContent += "\n" + line;
                break;
            }
            lineCounter++;
        }

        // Convert sendDateStr to time_t
        // Expected format: "2025-07-09 17:58:00"
        std::tm tm = {};
        std::istringstream dateStream(sendDateStr);
        dateStream >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        time_t sendTime = mktime(&tm); // Local time

        return Message(sendTime, messageContent, senderPhoneNumber, receiverPhoneNumber);
    }

    void ParseSMSs(std::string manySMS) {

        std::string currentMessage = "";
        std::string currentLine = "";
        std::stringstream ss(manySMS);

        while (std::getline(ss, currentLine))
        {
            if (currentLine == "===")
            {
                if (!currentMessage.empty())
                {
                    Message newParsedMessage = ParseSmsIntoMessage(currentMessage);
                    SmsMessages.push_back(newParsedMessage);
                    SmsCount++;
                    currentMessage.clear();
                }
            }
            else
            {
                currentMessage += currentLine + "\n";
            }
        }
        if (!currentMessage.empty())
        {
            Message newParsedMessage = ParseSmsIntoMessage(currentMessage);
            SmsMessages.push_back(newParsedMessage);
            SmsCount++;
        }

    }

    int returnEmailOrSms(std::string message) {
        //check if the fourth line is start with Subject:
        std::stringstream ss(message);
        std::string line;
        int lineCounter = 1;
        std::string newLine;

        while (std::getline(ss, line)) {
            switch (lineCounter) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:  // Subject: ...
                newLine = line.substr(0, 8);
                if (newLine == "Subject:")
                {
                    return 1;
                }
                else
                {
                    return 2;
                }
                break;
            default:
                break;
            }
            lineCounter++;
        }
        return -1;
    }

    void ParseCombinedSmsAndEmail(std::string smsAndEmail) {

        std::string currentMessage = "";
        std::string currentLine = "";
        std::stringstream ss(smsAndEmail);

        while (std::getline(ss, currentLine))
        {
            if (currentLine == "===")
            {
                if (!currentMessage.empty())
                {
                    int type = returnEmailOrSms(currentMessage);
                    if (type == 1)
                    {
                        Message newParsedMessage = ParseEmailIntoMessage(currentMessage);
                        EmailMessages.push_back(newParsedMessage);
                        EmailsCount++;
                        currentMessage.clear();
                    }
                    else if (type == 2)
                    {
                        Message newParsedMessage = ParseSmsIntoMessage(currentMessage);
                        SmsMessages.push_back(newParsedMessage);
                        SmsCount++;
                        currentMessage.clear();
                    }
                }
            }
            else
            {
                currentMessage += currentLine + "\n";
            }
        }
        if (!currentMessage.empty())
        {
            int type = returnEmailOrSms(currentMessage);
            if (type == 1)
            {
                Message newParsedMessage = ParseEmailIntoMessage(currentMessage);
                EmailMessages.push_back(newParsedMessage);
                EmailsCount++;
                currentMessage.clear();
            }
            else if (type == 2)
            {
                Message newParsedMessage = ParseSmsIntoMessage(currentMessage);
                SmsMessages.push_back(newParsedMessage);
                SmsCount++;
                currentMessage.clear();
            }
        }
    }

    //filter functions for email
    void FilterEmailBySenderEmail(std::string senderEmail) {

        for (int i = 0; i < EmailsCount; i++)
        {
            if (EmailMessages.at(i).GetSenderEmail() == senderEmail)
            {
                filteredEmails.push_back(EmailMessages.at(i));
                filteredEmailsCount++;
            }
        }
    }
    void FilterEmailByRecieverEmail(std::string recieverEmail) {

        for (int i = 0; i < EmailsCount; i++)
        {
            if (EmailMessages.at(i).GetRecieverEmail() == recieverEmail)
            {
                filteredEmails.push_back(EmailMessages.at(i));
                filteredEmailsCount++;
            }
        }
    }
    void FilterEmailByDay(int Day) {
        for (int i = 0; i < EmailsCount; i++) {
            time_t sendTime = EmailMessages.at(i).GetSendTime();
            std::tm* timeinfo = std::localtime(&sendTime);
            if (timeinfo && timeinfo->tm_mday == Day) {
                filteredEmails.push_back(EmailMessages.at(i));
                filteredEmailsCount++;
            }
        }
    }
    void FilterEmailByDayInWeek(int weekDay) {
        for (int i = 0; i < EmailsCount; i++) {
            time_t sendTime = EmailMessages.at(i).GetSendTime();
            std::tm* timeinfo = std::localtime(&sendTime);
            if (timeinfo && timeinfo->tm_wday == weekDay) {
                filteredEmails.push_back(EmailMessages.at(i));
                filteredEmailsCount++;
            }
        }
    }
    void FilterEmailMonth(int Month) {
        for (int i = 0; i < EmailsCount; i++) {
            time_t sendTime = EmailMessages.at(i).GetSendTime();
            std::tm* timeinfo = std::localtime(&sendTime);
            if (timeinfo && timeinfo->tm_mon + 1 == Month) {
                filteredEmails.push_back(EmailMessages.at(i));
                filteredEmailsCount++;
            }
        }
    }
    void FilterEmailYear(int Year) {
        for (int i = 0; i < EmailsCount; i++) {
            time_t sendTime = EmailMessages.at(i).GetSendTime();
            std::tm* timeinfo = std::localtime(&sendTime);
            if (timeinfo && timeinfo->tm_year + 1 == Year) {
                filteredEmails.push_back(EmailMessages.at(i));
                filteredEmailsCount++;
            }
        }
    }
    void FilterEmailBySubject(std::string Subject) {

        for (int i = 0; i < EmailsCount; i++)
        {
            if (EmailMessages.at(i).GetEmailSubject() == Subject)
            {
                filteredEmails.push_back(EmailMessages.at(i));
                filteredEmailsCount++;
            }
        }
    }
    void FilterEmailByContent(std::string Content) {

        for (int i = 0; i < filteredEmailsCount; i++)
        {
            if (EmailMessages.at(i).GetMessageContent().find(Content) != std::string::npos)
            {
                filteredEmails.push_back(EmailMessages.at(i));
                filteredEmailsCount++;
            }
        }
    }
    void ClearEmailFilters() {
        filteredEmails.clear();
        filteredEmailsCount = 0;
    }

    //optimized email filtering function
    void FilterEmails(const EmailFilterCriteria& criteria) {
        filteredEmails.clear();
        filteredEmailsCount = 0;

        for (int i = 0; i < EmailsCount; i++) {
            bool match = true;

            if (criteria.filterBySender && EmailMessages.at(i).GetSenderEmail() != criteria.sender)
                match = false;
            if (criteria.filterByReceiver && EmailMessages.at(i).GetRecieverEmail() != criteria.receiver)
                match = false;

            time_t sendTime = EmailMessages.at(i).GetSendTime();
            std::tm* tm = std::localtime(&sendTime);

            if (criteria.filterByDay && tm->tm_mday != criteria.day)
                match = false;
            if (criteria.filterByWeekday && tm->tm_wday != criteria.weekday)
                match = false;
            if (criteria.filterByMonth && (tm->tm_mon + 1) != criteria.month)
                match = false;
            if (criteria.filterByYear && (tm->tm_year + 1900) != criteria.year)
                match = false;

            if (criteria.filterBySubject && EmailMessages.at(i).GetEmailSubject() != criteria.subject)
                match = false;
            if (criteria.filterByContent && EmailMessages.at(i).GetMessageContent().find(criteria.content) == std::string::npos)
                match = false;

            if (match) {
                filteredEmails.push_back(EmailMessages.at(i));
                filteredEmailsCount++;
            }
        }
    }

    //filter functions for sms
    void FilterSmsBySenderPhoneNumber(std::string senderPhoneNumber) {

        for (int i = 0; i < SmsCount; i++)
        {
            if (SmsMessages.at(i).GetSenderPhoneNumber() == senderPhoneNumber)
            {
                filteredSms.push_back(SmsMessages.at(i));
                filteredSmsCount++;
            }
        }
    }
    void FilterSmsByRecieverPhoneNumber(std::string recieverPhoneNumber) {

        for (int i = 0; i < SmsCount; i++)
        {
            if (SmsMessages.at(i).GetRecieverPhoneNumber() == recieverPhoneNumber)
            {
                filteredSms.push_back(SmsMessages.at(i));
                filteredSmsCount++;
            }
        }
    }
    void FilterSmsByDay(int Day) {
        for (int i = 0; i < SmsCount; i++) {
            time_t sendTime = SmsMessages.at(i).GetSendTime();
            std::tm* timeinfo = std::localtime(&sendTime);
            if (timeinfo && timeinfo->tm_mday == Day) {
                filteredSms.push_back(SmsMessages.at(i));
                filteredSmsCount++;
            }
        }
    }
    void FilterSmsByDayInWeek(int weekDay) {
        for (int i = 0; i < SmsCount; i++) {
            time_t sendTime = SmsMessages.at(i).GetSendTime();
            std::tm* timeinfo = std::localtime(&sendTime);
            if (timeinfo && timeinfo->tm_wday == weekDay) {
                filteredSms.push_back(SmsMessages.at(i));
                filteredSmsCount++;
            }
        }
    }
    void FilterSmsMonth(int Month) {
        for (int i = 0; i < SmsCount; i++) {
            time_t sendTime = SmsMessages.at(i).GetSendTime();
            std::tm* timeinfo = std::localtime(&sendTime);
            if (timeinfo && timeinfo->tm_mon + 1 == Month) {
                filteredSms.push_back(SmsMessages.at(i));
                filteredSmsCount++;
            }
        }
    }
    void FilterSmsYear(int Year) {
        for (int i = 0; i < SmsCount; i++) {
            time_t sendTime = SmsMessages.at(i).GetSendTime();
            std::tm* timeinfo = std::localtime(&sendTime);
            if (timeinfo && timeinfo->tm_year + 1 == Year) {
                filteredSms.push_back(SmsMessages.at(i));
                filteredSmsCount++;
            }
        }
    }
    void FilterSmsByContent(std::string Content) {

        for (int i = 0; i < filteredSmsCount; i++)
        {
            if (SmsMessages.at(i).GetMessageContent().find(Content) != std::string::npos)
            {
                filteredSms.push_back(SmsMessages.at(i));
                filteredSmsCount++;
            }
        }
    }
    void ClearSmsFilters() {
        filteredSms.clear();
        filteredSmsCount = 0;
    }


    //optimized sms filtering function
    void FilterSms(const SmsFilterCriteria& criteria) {
        filteredSms.clear();
        filteredSmsCount = 0;

        for (int i = 0; i < SmsCount; i++) {
            bool match = true;

            if (criteria.filterBySender && SmsMessages.at(i).GetSenderPhoneNumber() != criteria.sender)
                match = false;
            if (criteria.filterByReceiver && SmsMessages.at(i).GetRecieverPhoneNumber() != criteria.receiver)
                match = false;

            time_t sendTime = SmsMessages.at(i).GetSendTime();
            std::tm* tm = std::localtime(&sendTime);

            if (criteria.filterByDay && tm->tm_mday != criteria.day)
                match = false;
            if (criteria.filterByWeekday && tm->tm_wday != criteria.weekday)
                match = false;
            if (criteria.filterByMonth && (tm->tm_mon + 1) != criteria.month)
                match = false;
            if (criteria.filterByYear && (tm->tm_year + 1900) != criteria.year)
                match = false;
            if (criteria.filterByContent && SmsMessages.at(i).GetMessageContent().find(criteria.content) == std::string::npos)
                match = false;

            if (match) {
                filteredSms.push_back(SmsMessages.at(i));
                filteredSmsCount++;
            }
        }
    }

    //suspicious messages functions
    void toLowerVector(std::vector<Message>& vec) {
        for (Message& msg : vec) {
            std::string loweredContent = msg.GetMessageContent();
            std::transform(loweredContent.begin(), loweredContent.end(), loweredContent.begin(), ::tolower);
            msg.SetMessageContent(loweredContent); // Requires setter

            if (!msg.GetEmailSubject().empty())
            {
                std::string loweredSubject = msg.GetEmailSubject();
                std::transform(loweredSubject.begin(), loweredSubject.end(), loweredSubject.begin(), ::tolower);
                msg.SetEmailSubject(loweredSubject); // Requires setter
            }
        }

    }
    void TrackSuspiciousMessages() {

        const std::map<std::string, int> suspiciousWordsMap = {
            // Financial / Scam
            {"urgent transfer", 1}, {"lottery winner", 1}, {"unclaimed funds", 1}, {"bank account", 1},
            {"verify your account", 1}, {"confirm your identity", 1}, {"free money", 1},
            {"investment opportunity", 1}, {"wire transfer", 1}, {"Nigerian prince", 1},
            {"inheritance", 1}, {"PayPal alert", 1}, {"credit card required", 1},
            {"ATM withdrawal", 1}, {"unusual activity", 1}, {"financial compensation", 1},

            // Security / Phishing
            {"your account is compromised", 1}, {"click to verify", 1}, {"reset your password", 1},
            {"unauthorized login", 1}, {"login alert", 1}, {"account suspended", 1},
            {"security notice", 1}, {"urgent update", 1}, {"verify now", 1},
            {"your account has been locked", 1}, {"click this link", 1},

            // Pharmaceutical / Black Market
            {"cheap meds", 1}, {"no prescription", 1}, {"Viagra", 1}, {"Xanax", 1}, {"Oxycodone", 1},
            {"pain relief", 1}, {"weight loss", 1}, {"muscle gain", 1}, {"online pharmacy", 1},
            {"steroids", 1}, {"male enhancement", 1},

            // Malware / Hacking
            {"download now", 1}, {"free software", 1}, {"hacking tool", 1}, {"crack available", 1},
            {"keylogger", 1}, {"payload", 1}, {"exploit", 1}, {"remote access", 1}, {"backdoor", 1},
            {"botnet", 1}, {"injector", 1},

            // Terrorism / Extremism
            {"bomb", 1}, {"attack", 1}, {"target", 1}, {"explosive", 1}, {"martyr", 1}, {"jihad", 1},
            {"recruit", 1}, {"weapon", 1}, {"mission", 1}, {"strike", 1}, {"militia", 1},

            // Social Engineering
            {"CEO request", 1}, {"confidential", 1}, {"internal only", 1}, {"do not share", 1},
            {"as discussed", 1}, {"immediate action", 1}, {"wire urgently", 1},
            {"outside protocol", 1}, {"HR request", 1}, {"change of payment", 1},

            // Fake Promotions / Clickbait
            {"you've won", 1}, {"free gift", 1}, {"claim your prize", 1}, {"limited offer", 1},
            {"act now", 1}, {"congratulations", 1}, {"click here", 1}, {"exclusive deal", 1},
            {"redeem reward", 1},

            // Suspicious Contact
            {"call me now", 1}, {"urgent reply needed", 1}, {"contact this number", 1},
            {"whatsapp me", 1}, {"telegram only", 1}, {"reply asap", 1}, {"send your details", 1}
        };
        std::vector<Message> emailsDummyVector = EmailMessages;
        toLowerVector(emailsDummyVector);

        std::vector<Message> smsDummyVector = SmsMessages;
        toLowerVector(smsDummyVector);

        //check for the sus emails
        for (int i = 0; i < EmailsCount; i++)
        {
            Message lowerMsg = emailsDummyVector.at(i);
            for (const auto& susMessage : suspiciousWordsMap) {
                std::string lowerPhrase = susMessage.first;
                std::transform(lowerPhrase.begin(), lowerPhrase.end(), lowerPhrase.begin(), ::tolower);

                if (lowerMsg.GetEmailSubject().find(lowerPhrase) != std::string::npos || lowerMsg.GetMessageContent().find(lowerPhrase) != std::string::npos) {
                    suspiciousEmails.push_back(lowerMsg);  // Suspicious phrase found
                    suspiciousEmailsCount++;
                }
            }
        }

        //check for the sus sms
        for (int i = 0; i < SmsCount; i++)
        {
            Message lowerMsg = smsDummyVector.at(i);
            for (const auto& susMessage : suspiciousWordsMap) {
                std::string lowerPhrase = susMessage.first;
                std::transform(lowerPhrase.begin(), lowerPhrase.end(), lowerPhrase.begin(), ::tolower);

                if (lowerMsg.GetMessageContent().find(lowerPhrase) != std::string::npos) {
                    suspiciousSms.push_back(lowerMsg);  // Suspicious phrase found
                    suspiciousSmsCount++;
                }
            }
        }

    }

};
