VERSION 5.00
Begin VB.Form frmButtons 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "LEGO - Buttons"
   ClientHeight    =   1830
   ClientLeft      =   45
   ClientTop       =   9345
   ClientWidth     =   2310
   ClipControls    =   0   'False
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1830
   ScaleWidth      =   2310
   Begin VB.CommandButton cmdUp 
      Caption         =   "Up"
      Height          =   735
      Left            =   120
      Picture         =   "frmButtons.frx":0000
      Style           =   1  'Graphical
      TabIndex        =   3
      Top             =   960
      Width           =   975
   End
   Begin VB.CommandButton cmdDown 
      Caption         =   "Down"
      Height          =   735
      Left            =   1200
      Picture         =   "frmButtons.frx":0442
      Style           =   1  'Graphical
      TabIndex        =   2
      Top             =   960
      Width           =   975
   End
   Begin VB.CommandButton cmdRight 
      Caption         =   "Right"
      Height          =   735
      Left            =   1200
      Picture         =   "frmButtons.frx":0884
      Style           =   1  'Graphical
      TabIndex        =   1
      Top             =   120
      Width           =   975
   End
   Begin VB.CommandButton cmdLeft 
      Caption         =   "Left"
      Height          =   735
      Left            =   120
      Picture         =   "frmButtons.frx":0CC6
      Style           =   1  'Graphical
      TabIndex        =   0
      Top             =   120
      Width           =   975
   End
End
Attribute VB_Name = "frmButtons"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdDown_Click()
   woy = woy + 10
   If (woy + iViewableY) > iLevelHeight Then woy = woy - ((woy + iViewableY) - iLevelHeight)
   Call RedrawMainView
   Call RedrawMMView
End Sub

Private Sub cmdLeft_Click()
   wox = wox - 10
   If wox < 0 Then wox = 0
   Call RedrawMainView
   Call RedrawMMView
End Sub

Private Sub cmdRight_Click()
   wox = wox + 10
   If (wox + iViewableX) > iLevelWidth Then wox = wox - ((wox + iViewableX) - iLevelWidth)
   Call RedrawMainView
   Call RedrawMMView
End Sub

Private Sub cmdUp_Click()
   woy = woy - 10
   If woy < 0 Then woy = 0
   Call RedrawMainView
   Call RedrawMMView
End Sub
