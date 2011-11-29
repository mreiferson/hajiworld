VERSION 5.00
Begin VB.Form frmMiniMap 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "LEGO - MiniMap"
   ClientHeight    =   495
   ClientLeft      =   5370
   ClientTop       =   540
   ClientWidth     =   5895
   ClipControls    =   0   'False
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   495
   ScaleWidth      =   5895
   Begin VB.PictureBox picMM 
      AutoRedraw      =   -1  'True
      BackColor       =   &H00000000&
      Height          =   510
      Left            =   0
      ScaleHeight     =   30
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   390
      TabIndex        =   0
      Top             =   0
      Width           =   5910
   End
End
Attribute VB_Name = "frmMiniMap"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub picMM_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
   leftc = Int((X * 8) - (iViewableX / 2))
   topc = Int((Y * 8) - (iViewableY / 2))
   If (leftc + iViewableX) > iLevelWidth Then
      leftc = leftc - (leftc + iViewableX - iLevelWidth)
   End If
   If (topc + iViewableY) > iLevelHeight Then
      topc = topc - (topc + iViewableY - iLevelHeight)
   End If
   If leftc < 0 Then
      leftc = 0
   End If
   If topc < 0 Then
      topc = 0
   End If
   wox = leftc
   woy = topc
   Call RedrawMainView
   Call RedrawMMView
End Sub
