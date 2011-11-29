VERSION 5.00
Begin VB.Form frmPolyProperties 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "LEGO - Poly Properties"
   ClientHeight    =   1455
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4020
   ClipControls    =   0   'False
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1455
   ScaleWidth      =   4020
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.CheckBox chkCanJumpDown 
      Caption         =   "Can Jump Down?"
      Height          =   255
      Left            =   1920
      TabIndex        =   12
      Top             =   1200
      Width           =   1815
   End
   Begin VB.TextBox txtID 
      Height          =   285
      Left            =   240
      Locked          =   -1  'True
      TabIndex        =   11
      Top             =   1680
      Width           =   495
   End
   Begin VB.CheckBox chkCanmovethrough 
      Caption         =   "Can Move Through?"
      Height          =   255
      Left            =   1920
      TabIndex        =   10
      Top             =   840
      Width           =   1815
   End
   Begin VB.CheckBox chkCanwalkon 
      Caption         =   "Can Walk On?"
      Height          =   255
      Left            =   1920
      TabIndex        =   9
      Top             =   480
      Width           =   1455
   End
   Begin VB.TextBox txtW 
      Height          =   315
      Left            =   2640
      TabIndex        =   7
      Top             =   120
      Width           =   615
   End
   Begin VB.TextBox txtH 
      Height          =   315
      Left            =   3360
      TabIndex        =   6
      Top             =   120
      Width           =   615
   End
   Begin VB.TextBox txtX 
      Height          =   315
      Left            =   840
      TabIndex        =   2
      Top             =   120
      Width           =   615
   End
   Begin VB.TextBox txtY 
      Height          =   315
      Left            =   1560
      TabIndex        =   1
      Top             =   120
      Width           =   615
   End
   Begin VB.CommandButton cmdDone 
      Caption         =   "Done"
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   840
      Width           =   1575
   End
   Begin VB.Label Label5 
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
      Left            =   3240
      TabIndex        =   8
      Top             =   -120
      Width           =   135
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "Size:"
      Height          =   195
      Left            =   2280
      TabIndex        =   5
      Top             =   120
      Width           =   345
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
      TabIndex        =   4
      Top             =   -120
      Width           =   135
   End
   Begin VB.Label Label4 
      AutoSize        =   -1  'True
      Caption         =   "Location:"
      Height          =   195
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   660
   End
End
Attribute VB_Name = "frmPolyProperties"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdDone_Click()
   id = Val(txtID.Text)
   frmInLevel.gridPolysData.TextMatrix(id, 2) = txtX.Text
   frmInLevel.gridPolysData.TextMatrix(id, 3) = txtY.Text
   frmInLevel.gridPolysData.TextMatrix(id, 4) = Format(Val(txtX.Text) + Val(txtW.Text))
   frmInLevel.gridPolysData.TextMatrix(id, 5) = txtY.Text
   frmInLevel.gridPolysData.TextMatrix(id, 6) = Format(Val(txtX.Text) + Val(txtW.Text))
   frmInLevel.gridPolysData.TextMatrix(id, 7) = Format(Val(txtY.Text) + Val(txtH.Text))
   frmInLevel.gridPolysData.TextMatrix(id, 8) = txtX.Text
   frmInLevel.gridPolysData.TextMatrix(id, 9) = Format(Val(txtY.Text) + Val(txtH.Text))
   frmInLevel.gridPolysData.TextMatrix(id, 10) = chkCanwalkon.Value
   frmInLevel.gridPolysData.TextMatrix(id, 11) = chkCanmovethrough.Value
   frmInLevel.gridPolysData.TextMatrix(id, 12) = chkCanJumpDown.Value
   Unload Me
End Sub
