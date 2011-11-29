VERSION 5.00
Begin VB.Form frmEnemyProperties 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "LEGO - Enemy Properties"
   ClientHeight    =   1725
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   3720
   ClipControls    =   0   'False
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1725
   ScaleWidth      =   3720
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.TextBox txtData2 
      Height          =   285
      Left            =   3000
      TabIndex        =   11
      Top             =   840
      Width           =   495
   End
   Begin VB.TextBox txtID 
      Height          =   285
      Left            =   360
      Locked          =   -1  'True
      TabIndex        =   5
      Top             =   240
      Width           =   495
   End
   Begin VB.ComboBox cmbDirection 
      Height          =   315
      ItemData        =   "frmEnemyProperties.frx":0000
      Left            =   1680
      List            =   "frmEnemyProperties.frx":000A
      Style           =   2  'Dropdown List
      TabIndex        =   4
      Top             =   240
      Width           =   1815
   End
   Begin VB.TextBox txtX 
      Height          =   315
      Left            =   600
      TabIndex        =   3
      Top             =   840
      Width           =   495
   End
   Begin VB.TextBox txtY 
      Height          =   315
      Left            =   1200
      TabIndex        =   2
      Top             =   840
      Width           =   495
   End
   Begin VB.TextBox txtData1 
      Height          =   285
      Left            =   2400
      TabIndex        =   1
      Top             =   840
      Width           =   495
   End
   Begin VB.CommandButton cmdDone 
      Caption         =   "Done"
      Height          =   255
      Left            =   1080
      TabIndex        =   0
      Top             =   1320
      Width           =   1575
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "ID:"
      Height          =   195
      Left            =   120
      TabIndex        =   10
      Top             =   240
      Width           =   210
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      Caption         =   "Direction:"
      Height          =   195
      Left            =   960
      TabIndex        =   9
      Top             =   240
      Width           =   675
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
      Left            =   1080
      TabIndex        =   8
      Top             =   600
      Width           =   135
   End
   Begin VB.Label Label4 
      AutoSize        =   -1  'True
      Caption         =   "Origin:"
      Height          =   195
      Left            =   120
      TabIndex        =   7
      Top             =   840
      Width           =   450
   End
   Begin VB.Label Label5 
      AutoSize        =   -1  'True
      Caption         =   "Data:"
      Height          =   195
      Left            =   1920
      TabIndex        =   6
      Top             =   840
      Width           =   390
   End
End
Attribute VB_Name = "frmEnemyProperties"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdDone_Click()
   id = Val(txtID.Text)
   frmInLevel.gridEnemiesData.TextMatrix(id, 2) = txtX.Text
   frmInLevel.gridEnemiesData.TextMatrix(id, 3) = txtY.Text
   frmInLevel.gridEnemiesData.TextMatrix(id, 4) = cmbDirection.ListIndex
   frmInLevel.gridEnemiesData.TextMatrix(id, 5) = txtData1.Text
   frmInLevel.gridEnemiesData.TextMatrix(id, 6) = txtData2.Text
   Unload Me
End Sub
