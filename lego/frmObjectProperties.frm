VERSION 5.00
Begin VB.Form frmObjectProperties 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "LEGO - Object Properties"
   ClientHeight    =   1605
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   3615
   ClipControls    =   0   'False
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1605
   ScaleWidth      =   3615
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton cmdDone 
      Caption         =   "Done"
      Height          =   255
      Left            =   1080
      TabIndex        =   10
      Top             =   1200
      Width           =   1575
   End
   Begin VB.TextBox txtMpnum 
      Height          =   285
      Left            =   2880
      TabIndex        =   8
      Top             =   720
      Width           =   615
   End
   Begin VB.TextBox txtY 
      Height          =   315
      Left            =   1440
      TabIndex        =   5
      Top             =   720
      Width           =   495
   End
   Begin VB.TextBox txtX 
      Height          =   315
      Left            =   840
      TabIndex        =   4
      Top             =   720
      Width           =   495
   End
   Begin VB.ComboBox cmbPlane 
      Height          =   315
      ItemData        =   "frmObjectProperties.frx":0000
      Left            =   1440
      List            =   "frmObjectProperties.frx":000A
      Style           =   2  'Dropdown List
      TabIndex        =   2
      Top             =   120
      Width           =   2055
   End
   Begin VB.TextBox txtID 
      Height          =   285
      Left            =   360
      Locked          =   -1  'True
      TabIndex        =   0
      Top             =   120
      Width           =   495
   End
   Begin VB.Label Label5 
      AutoSize        =   -1  'True
      Caption         =   "MP Num:"
      Height          =   195
      Left            =   2160
      TabIndex        =   9
      Top             =   720
      Width           =   660
   End
   Begin VB.Label Label4 
      AutoSize        =   -1  'True
      Caption         =   "Location:"
      Height          =   195
      Left            =   120
      TabIndex        =   7
      Top             =   720
      Width           =   660
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
      Left            =   1320
      TabIndex        =   6
      Top             =   480
      Width           =   135
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      Caption         =   "Plane:"
      Height          =   195
      Left            =   960
      TabIndex        =   3
      Top             =   120
      Width           =   450
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "ID:"
      Height          =   195
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   210
   End
End
Attribute VB_Name = "frmObjectProperties"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdDone_Click()
   id = Val(txtID.Text)
   frmInLevel.gridObjectsData.TextMatrix(id, 2) = cmbPlane.ListIndex
   frmInLevel.gridObjectsData.TextMatrix(id, 3) = txtX.Text
   frmInLevel.gridObjectsData.TextMatrix(id, 4) = txtY.Text
   frmInLevel.gridObjectsData.TextMatrix(id, 5) = txtMpnum.Text
   Unload Me
End Sub
