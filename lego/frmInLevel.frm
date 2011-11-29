VERSION 5.00
Object = "{0ECD9B60-23AA-11D0-B351-00A0C9055D8E}#6.0#0"; "MSHFLXGD.OCX"
Begin VB.Form frmInLevel 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "LEGO - In Level"
   ClientHeight    =   1455
   ClientLeft      =   3510
   ClientTop       =   9345
   ClientWidth     =   11520
   ClipControls    =   0   'False
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1455
   ScaleWidth      =   11520
   Begin VB.CommandButton cmdDeleteMovepoly 
      Caption         =   "Delete"
      Height          =   255
      Left            =   10320
      TabIndex        =   22
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdMovepolyProperties 
      Caption         =   "Properties"
      Height          =   255
      Left            =   9240
      TabIndex        =   21
      Top             =   1080
      Width           =   1095
   End
   Begin VB.ListBox listMovepolys 
      Height          =   645
      Left            =   9240
      TabIndex        =   20
      Top             =   360
      Width           =   2175
   End
   Begin VB.ListBox listPolys 
      Height          =   645
      Left            =   6960
      TabIndex        =   17
      Top             =   360
      Width           =   2175
   End
   Begin VB.CommandButton cmdPolyProperties 
      Caption         =   "Properties"
      Height          =   255
      Left            =   6960
      TabIndex        =   16
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdDeletePoly 
      Caption         =   "Delete"
      Height          =   255
      Left            =   8040
      TabIndex        =   15
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdDeletePickup 
      Caption         =   "Delete"
      Height          =   255
      Left            =   5760
      TabIndex        =   14
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdDeleteEnemy 
      Caption         =   "Delete"
      Height          =   255
      Left            =   3480
      TabIndex        =   13
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdDeleteObject 
      Caption         =   "Delete"
      Height          =   255
      Left            =   1200
      TabIndex        =   12
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdPickupProperties 
      Caption         =   "Properties"
      Height          =   255
      Left            =   4680
      TabIndex        =   11
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdEnemyProperties 
      Caption         =   "Properties"
      Height          =   255
      Left            =   2400
      TabIndex        =   10
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdObjectProperties 
      Caption         =   "Properties"
      Height          =   255
      Left            =   120
      TabIndex        =   9
      Top             =   1080
      Width           =   1095
   End
   Begin VB.ListBox listPickups 
      Height          =   645
      Left            =   4680
      TabIndex        =   8
      Top             =   360
      Width           =   2175
   End
   Begin VB.ListBox listEnemies 
      Height          =   645
      Left            =   2400
      TabIndex        =   7
      Top             =   360
      Width           =   2175
   End
   Begin VB.ListBox listObjects 
      Height          =   645
      Left            =   120
      TabIndex        =   6
      Top             =   360
      Width           =   2175
   End
   Begin MSHierarchicalFlexGridLib.MSHFlexGrid gridObjectsData 
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   1800
      Width           =   2175
      _ExtentX        =   3836
      _ExtentY        =   450
      _Version        =   393216
      Rows            =   0
      Cols            =   6
      FixedRows       =   0
      FixedCols       =   0
      AllowBigSelection=   0   'False
      FocusRect       =   2
      HighLight       =   2
      SelectionMode   =   1
      MergeCells      =   3
      AllowUserResizing=   1
      _NumberOfBands  =   1
      _Band(0).Cols   =   6
      _Band(0).GridLinesBand=   1
      _Band(0).TextStyleBand=   0
      _Band(0).TextStyleHeader=   0
   End
   Begin MSHierarchicalFlexGridLib.MSHFlexGrid gridEnemiesData 
      Height          =   255
      Left            =   2400
      TabIndex        =   1
      Top             =   1800
      Width           =   2175
      _ExtentX        =   3836
      _ExtentY        =   450
      _Version        =   393216
      Rows            =   0
      Cols            =   7
      FixedRows       =   0
      FixedCols       =   0
      AllowBigSelection=   0   'False
      FocusRect       =   2
      HighLight       =   2
      SelectionMode   =   1
      MergeCells      =   3
      AllowUserResizing=   1
      _NumberOfBands  =   1
      _Band(0).Cols   =   7
      _Band(0).GridLinesBand=   1
      _Band(0).TextStyleBand=   0
      _Band(0).TextStyleHeader=   0
   End
   Begin MSHierarchicalFlexGridLib.MSHFlexGrid gridPickupsData 
      Height          =   255
      Left            =   4680
      TabIndex        =   2
      Top             =   1800
      Width           =   2175
      _ExtentX        =   3836
      _ExtentY        =   450
      _Version        =   393216
      Rows            =   0
      Cols            =   9
      FixedRows       =   0
      FixedCols       =   0
      AllowBigSelection=   0   'False
      FocusRect       =   2
      HighLight       =   2
      SelectionMode   =   1
      MergeCells      =   3
      AllowUserResizing=   1
      _NumberOfBands  =   1
      _Band(0).Cols   =   9
      _Band(0).GridLinesBand=   1
      _Band(0).TextStyleBand=   0
      _Band(0).TextStyleHeader=   0
   End
   Begin MSHierarchicalFlexGridLib.MSHFlexGrid gridPolysData 
      Height          =   255
      Left            =   6960
      TabIndex        =   19
      Top             =   1800
      Width           =   2175
      _ExtentX        =   3836
      _ExtentY        =   450
      _Version        =   393216
      Rows            =   0
      Cols            =   13
      FixedRows       =   0
      FixedCols       =   0
      AllowBigSelection=   0   'False
      FocusRect       =   2
      HighLight       =   2
      SelectionMode   =   1
      MergeCells      =   3
      AllowUserResizing=   1
      _NumberOfBands  =   1
      _Band(0).Cols   =   13
      _Band(0).GridLinesBand=   1
      _Band(0).TextStyleBand=   0
      _Band(0).TextStyleHeader=   0
   End
   Begin MSHierarchicalFlexGridLib.MSHFlexGrid gridMovepolysData 
      Height          =   255
      Left            =   9240
      TabIndex        =   24
      Top             =   1800
      Width           =   2175
      _ExtentX        =   3836
      _ExtentY        =   450
      _Version        =   393216
      Rows            =   0
      Cols            =   12
      FixedRows       =   0
      FixedCols       =   0
      AllowBigSelection=   0   'False
      FocusRect       =   2
      HighLight       =   2
      SelectionMode   =   1
      MergeCells      =   3
      AllowUserResizing=   1
      _NumberOfBands  =   1
      _Band(0).Cols   =   12
      _Band(0).GridLinesBand=   1
      _Band(0).TextStyleBand=   0
      _Band(0).TextStyleHeader=   0
   End
   Begin VB.Label Label5 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Movepoly's:"
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
      Left            =   9240
      TabIndex        =   23
      Top             =   120
      Width           =   1110
   End
   Begin VB.Label Label4 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Poly's:"
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
      Left            =   6960
      TabIndex        =   18
      Top             =   120
      Width           =   615
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
      Left            =   4680
      TabIndex        =   5
      Top             =   120
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
      Left            =   2400
      TabIndex        =   4
      Top             =   120
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
Attribute VB_Name = "frmInLevel"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub cmdDeleteEnemy_Click()
   retu = MsgBox("Are You Sure You Want To Delete " + gridEnemiesData.TextMatrix(listEnemies.ListIndex, 0), vbYesNo, "Delete Object")
   If retu = 6 Then
      gridEnemiesData.RemoveItem listEnemies.ListIndex
      listEnemies.RemoveItem listEnemies.ListIndex
   End If
End Sub

Private Sub cmdDeleteMovepoly_Click()
   retu = MsgBox("Are You Sure You Want To Delete " + gridMovepolysData.TextMatrix(listMovepolys.ListIndex, 0), vbYesNo, "Delete Object")
   If retu = 6 Then
      gridMovepolysData.RemoveItem listMovepolys.ListIndex
      listMovepolys.RemoveItem listMovepolys.ListIndex
   End If
End Sub

Private Sub cmdDeleteObject_Click()
   retu = MsgBox("Are You Sure You Want To Delete " + gridObjectsData.TextMatrix(listObjects.ListIndex, 0), vbYesNo, "Delete Object")
   If retu = 6 Then
      gridObjectsData.RemoveItem listObjects.ListIndex
      listObjects.RemoveItem listObjects.ListIndex
   End If
End Sub

Private Sub cmdDeletePickup_Click()
   retu = MsgBox("Are You Sure You Want To Delete " + gridPickupsData.TextMatrix(listPickups.ListIndex, 0), vbYesNo, "Delete Object")
   If retu = 6 Then
      gridPickupsData.RemoveItem listPickups.ListIndex
      listPickups.RemoveItem listPickups.ListIndex
   End If
End Sub

Private Sub cmdDeletePoly_Click()
   retu = MsgBox("Are You Sure You Want To Delete " + gridPolysData.TextMatrix(listPolys.ListIndex, 0), vbYesNo, "Delete Object")
   If retu = 6 Then
      gridPolysData.RemoveItem listPolys.ListIndex
      listPolys.RemoveItem listPolys.ListIndex
   End If
End Sub

Private Sub cmdEnemyProperties_Click()
   Call Load(frmEnemyProperties)
   frmEnemyProperties.txtID = listEnemies.ListIndex
   frmEnemyProperties.txtX = gridEnemiesData.TextMatrix(listEnemies.ListIndex, 2)
   frmEnemyProperties.txtY = gridEnemiesData.TextMatrix(listEnemies.ListIndex, 3)
   frmEnemyProperties.cmbDirection.ListIndex = Val(gridEnemiesData.TextMatrix(listEnemies.ListIndex, 4))
   frmEnemyProperties.txtData1 = gridEnemiesData.TextMatrix(listEnemies.ListIndex, 5)
   frmEnemyProperties.txtData2 = gridEnemiesData.TextMatrix(listEnemies.ListIndex, 6)
   frmEnemyProperties.Show vbModal
End Sub

Private Sub cmdMovepolyProperties_Click()
   Call Load(frmMovepolyProperties)
   frmMovepolyProperties.txtID = listMovepolys.ListIndex
   frmMovepolyProperties.txtX = gridMovepolysData.TextMatrix(listMovepolys.ListIndex, 1)
   frmMovepolyProperties.txtY = gridMovepolysData.TextMatrix(listMovepolys.ListIndex, 2)
   frmMovepolyProperties.txtW = gridMovepolysData.TextMatrix(listMovepolys.ListIndex, 3)
   frmMovepolyProperties.txtH = gridMovepolysData.TextMatrix(listMovepolys.ListIndex, 4)
   frmMovepolyProperties.cmbType.ListIndex = gridMovepolysData.TextMatrix(listMovepolys.ListIndex, 5)
   frmMovepolyProperties.chkCanwalkon.Value = gridMovepolysData.TextMatrix(listMovepolys.ListIndex, 6)
   frmMovepolyProperties.chkCanmovethrough.Value = gridMovepolysData.TextMatrix(listMovepolys.ListIndex, 7)
   frmMovepolyProperties.txtVX = gridMovepolysData.TextMatrix(listMovepolys.ListIndex, 8)
   frmMovepolyProperties.txtVY = gridMovepolysData.TextMatrix(listMovepolys.ListIndex, 9)
   frmMovepolyProperties.txtData1 = gridMovepolysData.TextMatrix(listMovepolys.ListIndex, 10)
   frmMovepolyProperties.txtData2 = gridMovepolysData.TextMatrix(listMovepolys.ListIndex, 11)
   frmMovepolyProperties.Show vbModal
End Sub

Private Sub cmdObjectProperties_Click()
   Call Load(frmObjectProperties)
   frmObjectProperties.txtID = listObjects.ListIndex
   frmObjectProperties.txtMpnum = gridObjectsData.TextMatrix(listObjects.ListIndex, 5)
   frmObjectProperties.txtX = gridObjectsData.TextMatrix(listObjects.ListIndex, 3)
   frmObjectProperties.txtY = gridObjectsData.TextMatrix(listObjects.ListIndex, 4)
   frmObjectProperties.cmbPlane.ListIndex = Val(gridObjectsData.TextMatrix(listObjects.ListIndex, 2))
   frmObjectProperties.Show vbModal
End Sub

Private Sub cmdPickupProperties_Click()
   Call Load(frmPickupProperties)
   frmPickupProperties.txtID = listPickups.ListIndex
   frmPickupProperties.txtX = gridPickupsData.TextMatrix(listPickups.ListIndex, 2)
   frmPickupProperties.txtY = gridPickupsData.TextMatrix(listPickups.ListIndex, 3)
   frmPickupProperties.Show vbModal
End Sub

Private Sub cmdPolyProperties_Click()
   Call Load(frmPolyProperties)
   frmPolyProperties.txtID = listPolys.ListIndex
   frmPolyProperties.txtX = gridPolysData.TextMatrix(listPolys.ListIndex, 2)
   frmPolyProperties.txtY = gridPolysData.TextMatrix(listPolys.ListIndex, 3)
   frmPolyProperties.txtW = gridPolysData.TextMatrix(listPolys.ListIndex, 6) - frmPolyProperties.txtX
   frmPolyProperties.txtH = gridPolysData.TextMatrix(listPolys.ListIndex, 7) - frmPolyProperties.txtY
   frmPolyProperties.chkCanwalkon.Value = gridPolysData.TextMatrix(listPolys.ListIndex, 10)
   frmPolyProperties.chkCanmovethrough.Value = gridPolysData.TextMatrix(listPolys.ListIndex, 11)
   frmPolyProperties.chkCanJumpDown.Value = gridPolysData.TextMatrix(listPolys.ListIndex, 12)
   frmPolyProperties.Show vbModal
End Sub
