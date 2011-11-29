VERSION 5.00
Begin VB.Form frmPickupProperties 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "LEGO - Pickup Properties"
   ClientHeight    =   975
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   2895
   ClipControls    =   0   'False
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   975
   ScaleWidth      =   2895
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.TextBox txtID 
      Height          =   285
      Left            =   360
      Locked          =   -1  'True
      TabIndex        =   3
      Top             =   120
      Width           =   495
   End
   Begin VB.TextBox txtX 
      Height          =   315
      Left            =   1680
      TabIndex        =   2
      Top             =   120
      Width           =   495
   End
   Begin VB.TextBox txtY 
      Height          =   315
      Left            =   2280
      TabIndex        =   1
      Top             =   120
      Width           =   495
   End
   Begin VB.CommandButton cmdDone 
      Caption         =   "Done"
      Height          =   255
      Left            =   600
      TabIndex        =   0
      Top             =   600
      Width           =   1575
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "ID:"
      Height          =   195
      Left            =   120
      TabIndex        =   6
      Top             =   120
      Width           =   210
   End
   Begin VB.Label Label3 
      AutoSize        =   -1  'True
      Caption         =   ","
      BeginProperty Font 
         Name            =   "News Gothic MT"
         Size            =   24
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   585
      Left            =   2160
      TabIndex        =   5
      Top             =   -120
      Width           =   135
   End
   Begin VB.Label Label4 
      AutoSize        =   -1  'True
      Caption         =   "Location:"
      Height          =   195
      Left            =   960
      TabIndex        =   4
      Top             =   120
      Width           =   660
   End
End
Attribute VB_Name = "frmPickupProperties"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdDone_Click()
   id = Val(txtID.Text)
   frmInLevel.gridPickupsData.TextMatrix(id, 2) = txtX.Text
   frmInLevel.gridPickupsData.TextMatrix(id, 3) = txtY.Text
   Unload Me
End Sub
