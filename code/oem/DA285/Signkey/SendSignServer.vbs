'******************************************************************************
'* Lenovo Confidential
'* 
'* Microcode Source Materials
'*
'* ThinkPad Embedded Controller Microcode
'*
'* COPYRIGHT LENOVO 2014, 2015 ALL RIGHTS RESERVED
'*
'* FileName: SendSignServer.vbs
'*
'* Description:
'*
'*      This is used to send a package to the sign server.
'*         the 1st argument : Path of a package(a zip file)
'*
'* History:
'*    08/31/2015  Mitsuaki Sano
'*           Change to write the product path in the subject of the mail.
'*    10/27/2014  Mitsuaki Sano
'*           Initialize.
'*
'******************************************************************************

'
' Define Constant
'
olMailItem = 0
olAppointmentItem = 1
olContactItem = 2
olTaskItem = 3
olJournalItem = 4
olNoteItem = 5
olPostItem = 6
olDistributionListItem = 7

olAttachmentFilePath = 0

Dim Application
Set Application = CreateObject("Outlook.Application")
SendSignServer
'
' Send a Mail to the Sign Server
'
Public Sub SendSignServer()

    Dim msgItem
    Set msgItem = Application.CreateItem(olMailItem)
    Dim objWshShell
    Set objWshShell = WScript.CreateObject("Wscript.Shell")
    Dim Argu
    Set Argu = WScript.Arguments
    Dim strUnsignedFilePath
    Dim strProductPath

    strUnsignedFilePath = Argu(olAttachmentFilePath)
    strProductPath = Left(strUnsignedFilePath, InStrRev(strUnsignedFilePath, "\"))

    msgItem.To = "swqagent@lenovo.com"
    msgItem.CC = ""
    msgItem.BCC = ""
    msgItem.Subject = "[Sign] BIOS DA275"
    msgItem.Attachments.add Argu(olAttachmentFilePath)
    msgItem.Body = ""

    msgItem.Send

    Set Argu = Nothing
    Set objWshShell = Nothing
    Set msgItem = Nothing

End Sub