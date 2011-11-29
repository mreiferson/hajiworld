VERSION 5.00
Object = "{0ECD9B60-23AA-11D0-B351-00A0C9055D8E}#6.0#0"; "MSHFLXGD.OCX"
Begin VB.Form frmStuff 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "LEGO - Stuff"
   ClientHeight    =   8325
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5175
   ClipControls    =   0   'False
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   8325
   ScaleWidth      =   5175
   Begin VB.OptionButton opnGate 
      Caption         =   "Place Gate"
      Height          =   255
      Left            =   3720
      TabIndex        =   11
      Top             =   7920
      Width           =   1695
   End
   Begin VB.OptionButton opnMovePoly 
      Caption         =   "Place Movepoly"
      Height          =   255
      Left            =   2280
      TabIndex        =   9
      Top             =   7920
      Width           =   1695
   End
   Begin VB.OptionButton opnStuff 
      Caption         =   "Place Stuff"
      Height          =   255
      Left            =   960
      TabIndex        =   8
      Top             =   0
      Value           =   -1  'True
      Width           =   1575
   End
   Begin VB.OptionButton opnPoly 
      Caption         =   "Place Poly"
      Height          =   255
      Left            =   1200
      TabIndex        =   7
      Top             =   7920
      Width           =   1215
   End
   Begin VB.OptionButton opnHaji 
      Caption         =   "Place Haji"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   7920
      Width           =   1215
   End
   Begin MSHierarchicalFlexGridLib.MSHFlexGrid gridObjects 
      Height          =   2175
      Left            =   120
      TabIndex        =   0
      Top             =   360
      Width           =   4935
      _ExtentX        =   8705
      _ExtentY        =   3836
      _Version        =   393216
      Cols            =   7
      FixedCols       =   0
      AllowBigSelection=   0   'False
      FocusRect       =   2
      HighLight       =   2
      SelectionMode   =   1
      MergeCells      =   3
      AllowUserResizing=   1
      FormatString    =   "<Name                                 |<Filename          |>Frames|>Width|>Height|>AnmDel"
      _NumberOfBands  =   1
      _Band(0).Cols   =   7
      _Band(0).GridLinesBand=   1
      _Band(0).TextStyleBand=   0
      _Band(0).TextStyleHeader=   0
   End
   Begin MSHierarchicalFlexGridLib.MSHFlexGrid gridEnemies 
      Height          =   2175
      Left            =   120
      TabIndex        =   1
      Top             =   3000
      Width           =   4935
      _ExtentX        =   8705
      _ExtentY        =   3836
      _Version        =   393216
      Cols            =   13
      FixedCols       =   0
      AllowBigSelection=   0   'False
      FocusRect       =   2
      HighLight       =   2
      SelectionMode   =   1
      MergeCells      =   3
      AllowUserResizing=   1
      FormatString    =   $"frmStuff.frx":0000
      _NumberOfBands  =   1
      _Band(0).Cols   =   13
      _Band(0).GridLinesBand=   1
      _Band(0).TextStyleBand=   0
      _Band(0).TextStyleHeader=   0
   End
   Begin MSHierarchicalFlexGridLib.MSHFlexGrid gridPickups 
      Height          =   2175
      Left            =   120
      TabIndex        =   2
      Top             =   5640
      Width           =   4935
      _ExtentX        =   8705
      _ExtentY        =   3836
      _Version        =   393216
      Cols            =   9
      FixedCols       =   0
      AllowBigSelection=   0   'False
      FocusRect       =   2
      HighLight       =   2
      SelectionMode   =   1
      MergeCells      =   3
      AllowUserResizing=   1
      FormatString    =   "<Name                                 |<Filename          |>Frames|>Width|>Height|>Anim FS|>Anim FE|>Anim Del|>Data"
      _NumberOfBands  =   1
      _Band(0).Cols   =   9
      _Band(0).GridLinesBand=   1
      _Band(0).TextStyleBand=   0
      _Band(0).TextStyleHeader=   0
   End
   Begin VB.Label lblLoc 
      AutoSize        =   -1  'True
      Caption         =   "X,Y"
      BeginProperty Font 
         Name            =   "Verdana"
         Size            =   14.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H000000FF&
      Height          =   345
      Left            =   3000
      TabIndex        =   10
      Top             =   0
      Width           =   540
   End
   Begin VB.Label Label3 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Pickups:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   240
      Left            =   120
      TabIndex        =   5
      Top             =   5400
      Width           =   795
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Enemies:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   240
      Left            =   120
      TabIndex        =   4
      Top             =   2760
      Width           =   870
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Objects:"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   240
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   765
   End
End
Attribute VB_Name = "frmStuff"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub chkPlaceHaji_Click()
   If chkPlaceHaji.Value = 1 Then
      gridObjects.Enabled = False
      gridEnemies.Enabled = False
      gridPickups.Enabled = False
      ss = 3
   ElseIf chkPlaceHaji.Value = 0 Then
      gridObjects.Enabled = True
      gridEnemies.Enabled = True
      gridPickups.Enabled = True
      ss = 0
   End If
End Sub

Private Sub Form_Load()
   Open "..\Data\objects.hwd" For Input As #1
   Input #1, iNumObjects
   Line Input #1, buf1
   gridObjects.TextMatrix(1, 0) = buf1
   Line Input #1, buf2
   gridObjects.TextMatrix(1, 1) = buf2
   Line Input #1, buf3
   gridObjects.TextMatrix(1, 2) = buf3
   Line Input #1, buf4
   gridObjects.TextMatrix(1, 3) = buf4
   Line Input #1, buf5
   gridObjects.TextMatrix(1, 4) = buf5
   Line Input #1, buf6
   gridObjects.TextMatrix(1, 5) = buf6
   Line Input #1, buf7
   gridObjects.TextMatrix(1, 6) = buf7
   Line Input #1, buf
   For i = 2 To iNumObjects
      Line Input #1, buf1
      Line Input #1, buf2
      Line Input #1, buf3
      Line Input #1, buf4
      Line Input #1, buf5
      Line Input #1, buf6
      Line Input #1, buf7
      gridObjects.AddItem (buf1 & vbTab & buf2 & vbTab & buf3 & vbTab & buf4 & vbTab & buf5 & vbTab & buf6 & vbTab & buf7)
      Line Input #1, buf
   Next i
   Close #1
   Open "..\Data\enemies.hwd" For Input As #1
   Input #1, iNumEnemies
   Line Input #1, buf1
   gridEnemies.TextMatrix(1, 0) = buf1
   Line Input #1, buf2
   gridEnemies.TextMatrix(1, 1) = buf2
   Line Input #1, buf3
   gridEnemies.TextMatrix(1, 2) = buf3
   Line Input #1, buf4
   gridEnemies.TextMatrix(1, 3) = buf4
   Line Input #1, buf5
   gridEnemies.TextMatrix(1, 4) = buf5
   Line Input #1, buf6
   gridEnemies.TextMatrix(1, 5) = buf6
   Line Input #1, buf7
   gridEnemies.TextMatrix(1, 6) = buf7
   Line Input #1, buf8
   gridEnemies.TextMatrix(1, 7) = buf8
   Line Input #1, buf9
   gridEnemies.TextMatrix(1, 8) = buf9
   Line Input #1, buf10
   gridEnemies.TextMatrix(1, 9) = buf10
   Line Input #1, buf11
   gridEnemies.TextMatrix(1, 10) = buf11
   Line Input #1, buf12
   gridEnemies.TextMatrix(1, 11) = buf12
   Line Input #1, buf13
   gridEnemies.TextMatrix(1, 12) = buf13
   Line Input #1, buf
   For i = 2 To iNumEnemies
      Line Input #1, buf1
      Line Input #1, buf2
      Line Input #1, buf3
      Line Input #1, buf4
      Line Input #1, buf5
      Line Input #1, buf6
      Line Input #1, buf7
      Line Input #1, buf8
      Line Input #1, buf9
      Line Input #1, buf10
      Line Input #1, buf11
      Line Input #1, buf12
      Line Input #1, buf13
      gridEnemies.AddItem (buf1 & vbTab & buf2 & vbTab & buf3 & vbTab & buf4 & vbTab & buf5 & vbTab & buf6 & vbTab & buf7 & vbTab & buf8 & vbTab & buf9 & vbTab & buf10 & vbTab & buf11 & vbTab & buf12 & vbTab & buf13)
      Line Input #1, buf
   Next i
   Close #1
   Open "..\Data\pickups.hwd" For Input As #1
   Input #1, iNumPickups
   Line Input #1, buf1
   gridPickups.TextMatrix(1, 0) = buf1
   Line Input #1, buf2
   gridPickups.TextMatrix(1, 1) = buf2
   Line Input #1, buf3
   gridPickups.TextMatrix(1, 2) = buf3
   Line Input #1, buf4
   gridPickups.TextMatrix(1, 3) = buf4
   Line Input #1, buf5
   gridPickups.TextMatrix(1, 4) = buf5
   Line Input #1, buf6
   gridPickups.TextMatrix(1, 5) = buf6
   Line Input #1, buf7
   gridPickups.TextMatrix(1, 6) = buf7
   Line Input #1, buf8
   gridPickups.TextMatrix(1, 7) = buf8
   Line Input #1, buf9
   gridPickups.TextMatrix(1, 8) = buf9
   Line Input #1, buf
   For i = 2 To iNumPickups
      Line Input #1, buf1
      Line Input #1, buf2
      Line Input #1, buf3
      Line Input #1, buf4
      Line Input #1, buf5
      Line Input #1, buf6
      Line Input #1, buf7
      Line Input #1, buf8
      Line Input #1, buf9
      gridPickups.AddItem (buf1 & vbTab & buf2 & vbTab & buf3 & vbTab & buf4 & vbTab & buf5 & vbTab & buf6 & vbTab & buf7 & vbTab & buf8 & vbTab & buf9)
      Line Input #1, buf
   Next i
   Close #1
End Sub

Private Sub gridEnemies_Click()
   ss = 1
End Sub

Private Sub gridObjects_Click()
   ss = 0
End Sub

Private Sub gridPickups_Click()
   ss = 2
End Sub

Private Sub opnGate_Click()
   gx1 = -1
   gy1 = -1
   gx2 = -1
   gy2 = -1
End Sub

Private Sub opnMovePoly_Click()
   mx1 = -1
   my1 = -1
   mx2 = -1
   my2 = -1
End Sub

Private Sub opnPoly_Click()
   px1 = -1
   py1 = -1
   px2 = -1
   py2 = -1
End Sub
