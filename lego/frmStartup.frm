VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmStartup 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "LEGO - The Editor For Haji World"
   ClientHeight    =   555
   ClientLeft      =   4005
   ClientTop       =   6345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   555
   ScaleWidth      =   4680
   StartUpPosition =   2  'CenterScreen
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   2400
      Top             =   480
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton cmdGo 
      Caption         =   "Go!"
      Height          =   375
      Left            =   3960
      TabIndex        =   1
      Top             =   120
      Width           =   615
   End
   Begin VB.ComboBox cmbSelectFunction 
      Height          =   315
      ItemData        =   "frmStartup.frx":0000
      Left            =   120
      List            =   "frmStartup.frx":0016
      Style           =   2  'Dropdown List
      TabIndex        =   0
      Top             =   120
      Width           =   3735
   End
End
Attribute VB_Name = "frmStartup"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdGo_Click()
   If cmbSelectFunction.ListIndex = 0 Then
      iLevelWidth = Val(InputBox("Enter the width for the level:", "Level Width", "5120"))
      iLevelWidth = Int(iLevelWidth / iViewableX) * iViewableX
      iLevelHeight = Val(InputBox("Enter the height for the level:", "Level Height", "896"))
      iLevelHeight = Int(iLevelHeight / iViewableY) * iViewableY
      iSectorsX = iLevelWidth / 640
      iSectorsY = iLevelHeight / 448
      frmMain.Show
      frmInLevel.Show
      frmButtons.Show
      frmMiniMap.Show
      frmStuff.Show
      difx = frmMiniMap.Width - frmMiniMap.picMM.Width
      dify = frmMiniMap.Height - frmMiniMap.picMM.Height
      frmMiniMap.picMM.Width = iLevelWidth / MMScale * Screen.TwipsPerPixelX
      frmMiniMap.picMM.Height = iLevelHeight / MMScale * Screen.TwipsPerPixelY
      frmMiniMap.Width = frmMiniMap.picMM.Width + difx
      frmMiniMap.Height = frmMiniMap.picMM.Height + dify
      ss = 0
      Call NewLevel
      Call RedrawMainView
      Call RedrawMMView
      Me.Enabled = False
   ElseIf cmbSelectFunction.ListIndex = 1 Then
      On Error GoTo canceled
      CommonDialog1.InitDir = GetSetting(App.Title, "InitialDirs", "OpenLevel")
      CommonDialog1.Filter = "Level File|*.lev"
      CommonDialog1.FilterIndex = 1
      CommonDialog1.ShowOpen
      levelfn = CommonDialog1.FileName
      sTmp = CommonDialog1.FileName
      For i = Len(sTmp) To 0 Step -1
         c = Mid(sTmp, i, 1)
         If c = "\" Then GoTo out
      Next i
out:
      SaveSetting App.Title, "InitialDirs", "OpenLevel", Mid(sTmp, 1, i)
      frmMain.Show
      frmInLevel.Show
      frmButtons.Show
      frmMiniMap.Show
      frmStuff.Show
      Call LoadLevel(CommonDialog1.FileName)
      difx = frmMiniMap.Width - frmMiniMap.picMM.Width
      dify = frmMiniMap.Height - frmMiniMap.picMM.Height
      frmMiniMap.picMM.Width = iLevelWidth / MMScale * Screen.TwipsPerPixelX
      frmMiniMap.picMM.Height = iLevelHeight / MMScale * Screen.TwipsPerPixelY
      frmMiniMap.Width = frmMiniMap.picMM.Width + difx
      frmMiniMap.Height = frmMiniMap.picMM.Height + dify
      ss = 0
      Call RedrawMainView
      Call RedrawMMView
      Me.Enabled = False
canceled:
   ElseIf cmbSelectFunction.ListIndex = 3 Then
      
   ElseIf cmbSelectFunction.ListIndex = 4 Then
      
   ElseIf cmbSelectFunction.ListIndex = 5 Then
      
   End If
End Sub

Private Sub Form_Load()
   cmbSelectFunction.ListIndex = 0
End Sub

Private Sub Form_Unload(Cancel As Integer)
   End
End Sub
