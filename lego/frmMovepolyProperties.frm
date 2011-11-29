VERSION 5.00
Begin VB.Form frmMovepolyProperties 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "LEGO - Movepoly Properties"
   ClientHeight    =   2175
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4110
   ClipControls    =   0   'False
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2175
   ScaleWidth      =   4110
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.TextBox txtID 
      Height          =   495
      Left            =   120
      TabIndex        =   18
      Top             =   2640
      Width           =   1215
   End
   Begin VB.TextBox txtData1 
      Height          =   285
      Left            =   600
      TabIndex        =   16
      Top             =   1080
      Width           =   495
   End
   Begin VB.TextBox txtData2 
      Height          =   285
      Left            =   1200
      TabIndex        =   15
      Top             =   1080
      Width           =   495
   End
   Begin VB.TextBox txtVX 
      Height          =   315
      Left            =   2640
      TabIndex        =   13
      Top             =   600
      Width           =   615
   End
   Begin VB.TextBox txtVY 
      Height          =   315
      Left            =   3360
      TabIndex        =   12
      Top             =   600
      Width           =   615
   End
   Begin VB.ComboBox cmbType 
      Height          =   315
      ItemData        =   "frmMovepolyProperties.frx":0000
      Left            =   600
      List            =   "frmMovepolyProperties.frx":000D
      Style           =   2  'Dropdown List
      TabIndex        =   10
      Top             =   600
      Width           =   1335
   End
   Begin VB.CommandButton cmdDone 
      Caption         =   "Done"
      Height          =   255
      Left            =   240
      TabIndex        =   6
      Top             =   1680
      Width           =   1575
   End
   Begin VB.TextBox txtY 
      Height          =   315
      Left            =   1560
      TabIndex        =   5
      Top             =   120
      Width           =   615
   End
   Begin VB.TextBox txtX 
      Height          =   315
      Left            =   840
      TabIndex        =   4
      Top             =   120
      Width           =   615
   End
   Begin VB.TextBox txtH 
      Height          =   315
      Left            =   3360
      TabIndex        =   3
      Top             =   120
      Width           =   615
   End
   Begin VB.TextBox txtW 
      Height          =   315
      Left            =   2640
      TabIndex        =   2
      Top             =   120
      Width           =   615
   End
   Begin VB.CheckBox chkCanwalkon 
      Caption         =   "Can Walk On?"
      Height          =   255
      Left            =   2160
      TabIndex        =   1
      Top             =   1080
      Width           =   1455
   End
   Begin VB.CheckBox chkCanmovethrough 
      Caption         =   "Can Move Through?"
      Height          =   255
      Left            =   2160
      TabIndex        =   0
      Top             =   1440
      Width           =   1815
   End
   Begin VB.Label Label8 
      AutoSize        =   -1  'True
      Caption         =   "Data:"
      Height          =   195
      Left            =   120
      TabIndex        =   17
      Top             =   1080
      Width           =   390
   End
   Begin VB.Label Label6 
      AutoSize        =   -1  'True
      Caption         =   "Velocity:"
      Height          =   195
      Left            =   2040
      TabIndex        =   14
      Top             =   600
      Width           =   600
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      Caption         =   "Type:"
      Height          =   195
      Left            =   120
      TabIndex        =   11
      Top             =   600
      Width           =   405
   End
   Begin VB.Label Label4 
      AutoSize        =   -1  'True
      Caption         =   "Location:"
      Height          =   195
      Left            =   120
      TabIndex        =   9
      Top             =   120
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
      Left            =   1440
      TabIndex        =   8
      Top             =   -120
      Width           =   135
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "Size:"
      Height          =   195
      Left            =   2280
      TabIndex        =   7
      Top             =   120
      Width           =   345
   End
End
Attribute VB_Name = "frmMovepolyProperties"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdDone_Click()
   id = Val(txtID.Text)
   frmInLevel.gridMovepolysData.TextMatrix(id, 1) = frmMovepolyProperties.txtX
   frmInLevel.gridMovepolysData.TextMatrix(id, 2) = frmMovepolyProperties.txtY
   frmInLevel.gridMovepolysData.TextMatrix(id, 3) = frmMovepolyProperties.txtW
   frmInLevel.gridMovepolysData.TextMatrix(id, 4) = frmMovepolyProperties.txtH
   frmInLevel.gridMovepolysData.TextMatrix(id, 5) = frmMovepolyProperties.cmbType.ListIndex
   frmInLevel.gridMovepolysData.TextMatrix(id, 6) = frmMovepolyProperties.chkCanwalkon.Value
   frmInLevel.gridMovepolysData.TextMatrix(id, 7) = frmMovepolyProperties.chkCanmovethrough.Value
   frmInLevel.gridMovepolysData.TextMatrix(id, 8) = frmMovepolyProperties.txtVX
   frmInLevel.gridMovepolysData.TextMatrix(id, 9) = frmMovepolyProperties.txtVY
   frmInLevel.gridMovepolysData.TextMatrix(id, 10) = frmMovepolyProperties.txtData1
   frmInLevel.gridMovepolysData.TextMatrix(id, 11) = frmMovepolyProperties.txtData2
   Unload Me
End Sub
