; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLyricsEditorDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "LyricsEditor.h"

ClassCount=4
Class1=CLyricsEditorApp
Class2=CLyricsEditorDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_LYRICSEDITOR_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=CAdvancedDialog
Resource4=IDD_DIALOG_ADVANCED

[CLS:CLyricsEditorApp]
Type=0
HeaderFile=LyricsEditor.h
ImplementationFile=LyricsEditor.cpp
Filter=N

[CLS:CLyricsEditorDlg]
Type=0
HeaderFile=LyricsEditorDlg.h
ImplementationFile=LyricsEditorDlg.cpp
Filter=W
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CLyricsEditorDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=LyricsEditorDlg.h
ImplementationFile=LyricsEditorDlg.cpp
Filter=D
LastObject=IDC_STATIC_EMAIL
BaseClass=CDialog
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=7
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC_EMAIL,static,1342308608

[DLG:IDD_LYRICSEDITOR_DIALOG]
Type=1
Class=CLyricsEditorDlg
ControlCount=14
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_SOURCEFILE,edit,1350631552
Control3=IDC_BUTTON_BROWSESC,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_LYRICS,edit,1353781444
Control6=IDC_BUTTON_GETLYRICS,button,1342242816
Control7=IDC_BUTTON_IMPORTLRC,button,1342242816
Control8=IDC_BUTTON_EXPORTTOLRC,button,1342242816
Control9=IDC_BUTTON_SAVE,button,1342242816
Control10=IDC_BUTTON_ADVANCED,button,1342242816
Control11=IDC_BUTTON_ABOUT,button,1342242816
Control12=IDCANCEL,button,1342242816
Control13=IDC_STATIC,button,1342210055
Control14=IDC_STATIC_INFO,static,1342308352

[DLG:IDD_DIALOG_ADVANCED]
Type=1
Class=CAdvancedDialog
ControlCount=14
Control1=IDC_STATIC,button,1342177287
Control2=IDC_CHECK_MEDIAPLAYERPATCH,button,1342242819
Control3=IDC_EDIT_PATCH,edit,1350631552
Control4=IDC_SPIN_PATCH,msctls_updown32,1342177332
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_LYRICSTITLE,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_LYRICS_OFFSET,edit,1350631552
Control9=IDC_CHECK_UNSYNCHRONISED_LYRICS,button,1342242819
Control10=IDOK,button,1342242817
Control11=IDCANCEL,button,1342242816
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_BUTTON_RESET,button,1342242816

[CLS:CAdvancedDialog]
Type=0
HeaderFile=AdvancedDialog.h
ImplementationFile=AdvancedDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CAdvancedDialog

