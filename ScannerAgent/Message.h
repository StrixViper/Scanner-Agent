#pragma once
#include <wx/wx.h>
#include <sstream>
#include <iomanip>
#include <list>

class Message
{
private:
    static int idCounter;
    int messageID;

    std::string sender;
    std::string reciever;
    time_t sendTime;
    std::string messageContent;

    std::string senderPhoneNumber;
    std::string recieverPhoneNumber;

    std::string emailSubject;
    std::string senderEmail;
    std::string recieverEmail;

public:

    Message(std::string Sender, std::string Reciever, time_t SendTime, std::string MessageContent)
    {
        sender = Sender;
        reciever = Reciever;
        sendTime = SendTime;
        messageContent = MessageContent;
        messageID = ++idCounter;
    }
    Message(time_t SendTime, std::string MessageContent, std::string SenderEmail, std::string RecieverEmail, std::string EmailSubject)
    {
        sendTime = SendTime;
        messageContent = MessageContent;
        messageID = ++idCounter;
        senderEmail = SenderEmail;
        recieverEmail = RecieverEmail;
        emailSubject = EmailSubject;
    }
    Message(time_t SendTime, std::string MessageContent, std::string SenderPhoneNumber, std::string RecieverPhoneNumber)
    {
        sendTime = SendTime;
        messageContent = MessageContent;
        messageID = ++idCounter;
        senderPhoneNumber = SenderPhoneNumber;
        recieverPhoneNumber = RecieverPhoneNumber;
    }

    std::string GetSender() {
        return sender;
    }
    std::string GetReciever() {
        return reciever;
    }
    time_t GetSendTime() {
        return sendTime;
    }
    std::string GetMessageContent() {
        return messageContent;
    }

    std::string GetRecieverPhoneNumber() {
        return recieverPhoneNumber;
    }
    std::string GetSenderPhoneNumber() {
        return senderPhoneNumber;
    }
    std::string GetRecieverEmail() {
        return recieverEmail;
    }
    std::string GetSenderEmail() {
        return senderEmail;
    }
    std::string GetEmailSubject() {
        return emailSubject;
    }

    void SetSender(std::string newSender) {
        sender = newSender;
    }
    void SetReciever(std::string newReciever) {
        reciever = newReciever;
    }
    void SetSendTime(time_t newSendTime) {
        sendTime = newSendTime;
    }
    void SetMessageContent(std::string newMessageContent) {
        messageContent = newMessageContent;
    }

    void SetRecieverPhoneNumber(std::string newRecieverPhoneNumber) {
        recieverPhoneNumber = newRecieverPhoneNumber;
    }
    void SetSenderPhoneNumber(std::string newSenderPhoneNumber) {
        senderPhoneNumber = newSenderPhoneNumber;
    }
    void SetRecieverEmail(std::string newRecieverEmail) {
        recieverEmail = newRecieverEmail;
    }
    void SetSenderEmail(std::string newSenderEmail) {
        senderEmail = newSenderEmail;
    }
    void SetEmailSubject(std::string newEmailSubject) {
        emailSubject = newEmailSubject;
    }

    void PrintEmailMessage() {
        std::cout << "Message ID: " << messageID << std::endl;
        std::cout << "Message Type: Email" << std::endl;
        std::cout << "Sender Email: " << senderEmail << std::endl;
        std::cout << "Reciever Email: " << recieverEmail << std::endl;
        std::time_t t = sendTime;
        std::cout << "Send Time: " << std::ctime(&t);
        std::cout << "Email Subject: " << emailSubject << std::endl;
        std::cout << "Message Content: " << messageContent << std::endl;
    }
    void PrintSmsMessage() {
        std::cout << "Message ID: " << messageID << std::endl;
        std::cout << "Message Type: SMS" << std::endl;
        std::cout << "Sender Phone Number: " << senderPhoneNumber << std::endl;
        std::cout << "Reciever Phone Number: " << recieverPhoneNumber << std::endl;
        std::time_t t = sendTime;
        std::cout << "Send Time: " << std::ctime(&t);
        std::cout << "Message Content: " << messageContent << std::endl;
    }

    std::string ReturnEmailMessage() const {
        std::string result = "";
        result += "Message Type: Email\n";
        result += "Message ID: " + std::to_string(messageID) + "\n";
        result += "Message Type: Email\n";
        result += "Sender Email: " + senderEmail + "\n";
        result += "Receiver Email: " + recieverEmail + "\n";

        char timeStr[100];
        std::strftime(timeStr, sizeof(timeStr), "%c", std::localtime(&sendTime));
        result += "Send Time: " + std::string(timeStr) + "\n";

        result += "Email Subject: " + emailSubject + "\n";
        result += "Message Content: " + messageContent + "\n";

        return result;
    }
    
    std::string ReturnSmsMessage() const {
        std::string result = "";
        result += "Message Type: SMS\n";
        result += "Message ID: " + std::to_string(messageID) + "\n";
        result += "Message Type: SMS\n";
        result += "Sender Phone Number: " + senderPhoneNumber + "\n";
        result += "Receiver Phone Number: " + recieverPhoneNumber + "\n";

        char timeStr[100];
        std::strftime(timeStr, sizeof(timeStr), "%c", std::localtime(&sendTime));
        result += "Send Time: " + std::string(timeStr) + "\n";

        result += "Message Content: " + messageContent + "\n";

        return result;
    }

};

