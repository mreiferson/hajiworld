VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmMain 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "LEGO - Main View"
   ClientHeight    =   6780
   ClientLeft      =   5490
   ClientTop       =   2625
   ClientWidth     =   9660
   ClipControls    =   0   'False
   ControlBox      =   0   'False
   Icon            =   "frmMain.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6780
   ScaleWidth      =   9660
   Begin VB.PictureBox picMain 
      AutoRedraw      =   -1  'True
      BackColor       =   &H00000000&
      Height          =   6780
      Left            =   0
      ScaleHeight     =   448
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   640
      TabIndex        =   0
      Top             =   0
      Width           =   9660
      Begin MSComDlg.CommonDialog CommonDialog1 
         Left            =   7800
         Top             =   240
         _ExtentX        =   847
         _ExtentY        =   847
         _Version        =   393216
         CancelError     =   -1  'True
      End
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuSaveLevel 
         Caption         =   "&Save Level"
      End
      Begin VB.Menu mnuSaveLevelAs 
         Caption         =   "Save Level &As..."
      End
      Begin VB.Menu mnuClose 
         Caption         =   "&Close"
      End
   End
   Begin VB.Menu mnuRun 
      Caption         =   "&Run"
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_GotFocus()
AppActivate "LEGO - Buttons"
AppActivate "LEGO - In Level"
AppActivate "LEGO - MiniMap"
AppActivate "LEGO - Stuff"
AppActivate "LEGO - Main View"
End Sub

Private Sub mnuClose_Click()
   Unload frmMiniMap
   Unload frmInLevel
   Unload frmButtons
   Unload frmStuff
   Unload Me
   frmStartup.Enabled = True
   AppActivate "LEGO"
End Sub

Private Sub mnuRun_Click()
   If levelfn <> "" Then
      myPath = CurDir("C:\")
      ChDir "C:\DevStudio\MyProjects\HajiWorld\Build\"
      sTmp = levelfn
      For i = Len(sTmp) To 0 Step -1
         c = Mid(sTmp, i, 1)
         If c = "\" Then GoTo out2
      Next i
out2:
      Shell "hajiworld.exe " + Mid(sTmp, i + 1), vbNormalFocus
      ChDir myPath
   End If
End Sub

Private Sub mnuSaveLevel_Click()
   If levelfn <> "" Then
      Call SaveLevel(levelfn)
   End If
End Sub

Private Sub mnuSaveLevelAs_Click()
   On Error GoTo canceled
   CommonDialog1.InitDir = GetSetting(App.Title, "InitialDirs", "SaveLevel")
   CommonDialog1.Filter = "Level File|*.lev"
   CommonDialog1.FilterIndex = 1
   CommonDialog1.ShowSave
   Call SaveLevel(CommonDialog1.FileName)
   levelfn = CommonDialog1.FileName
   sTmp = CommonDialog1.FileName
   For i = Len(sTmp) To 0 Step -1
      c = Mid(sTmp, i, 1)
      If c = "\" Then GoTo out
   Next i
out:
   SaveSetting App.Title, "InitialDirs", "SaveLevel", Mid(sTmp, 1, i)
canceled:
End Sub

Private Sub picMain_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
   If frmStuff.opnStuff.Value = True Then
      If ss = 0 Then
         oname = frmStuff.gridObjects.TextMatrix(frmStuff.gridObjects.Row, 0)
         oid = frmStuff.gridObjects.Row - 1
         ox = X + wox
         oy = Y + woy
         mpnum = -1
         frmInLevel.gridObjectsData.AddItem oname & vbTab & Format(oid) & vbTab & Format(0) & vbTab & Format(ox) & vbTab & Format(oy) & vbTab & Format(mpnum)
         frmInLevel.listObjects.AddItem oname
      ElseIf ss = 1 Then
         ename = frmStuff.gridEnemies.TextMatrix(frmStuff.gridEnemies.Row, 0)
         eid = frmStuff.gridEnemies.Row - 1
         ex = X + wox
         ey = Y + woy
         edire = 0
         edata1 = 0
         edata2 = 0
         frmInLevel.gridEnemiesData.AddItem ename & vbTab & Format(eid) & vbTab & Format(ex) & vbTab & Format(ey) & vbTab & Format(edire) & vbTab & Format(edata1) & vbTab & Format(edata2)
         frmInLevel.listEnemies.AddItem ename
      ElseIf ss = 2 Then
         pname = frmStuff.gridPickups.TextMatrix(frmStuff.gridPickups.Row, 0)
         pid = frmStuff.gridPickups.Row - 1
         px = X + wox
         py = Y + woy
         frmInLevel.gridPickupsData.AddItem pname & vbTab & Format(pid) & vbTab & Format(px) & vbTab & Format(py)
         frmInLevel.listPickups.AddItem pname
      End If
   ElseIf frmStuff.opnHaji.Value = True Then
      hx = X + wox
      hy = Y + woy
      hd = 0
      frmStuff.opnStuff.Value = True
   ElseIf frmStuff.opnPoly.Value = True Then
      If (px1 = -1) And (py1 = -1) Then
         px1 = X + wox
         py1 = Y + woy
      Else
         px2 = X + wox
         py2 = Y + woy
         W = px2 - px1
         H = py2 - py1
         frmInLevel.gridPolysData.AddItem "<POLY-" + Format(frmInLevel.listPolys.ListCount) + ">" & vbTab & "4" & vbTab & Format(px1) & vbTab & Format(py1) & vbTab & Format(px1 + W) & vbTab & Format(py1) & vbTab & Format(px2) & vbTab & Format(py2) & vbTab & Format(px1) & vbTab & Format(py1 + H) & vbTab & "1" & vbTab & "1"
         frmInLevel.listPolys.AddItem "<POLY-" + Format(frmInLevel.listPolys.ListCount) + ">"
         frmStuff.opnStuff.Value = True
      End If
   ElseIf frmStuff.opnMovePoly.Value = True Then
      If (mx1 = -1) And (my1 = -1) Then
         mx1 = X + wox
         my1 = Y + woy
      Else
         mx2 = X + wox
         my2 = Y + woy
         W = mx2 - mx1
         H = my2 - my1
         frmInLevel.gridMovepolysData.AddItem "<MOVEPOLY-" + Format(frmInLevel.listMovepolys.ListCount) + ">" & vbTab & Format(mx1) & vbTab & Format(my1) & vbTab & Format(W) & vbTab & Format(H) & vbTab & Format(0) & vbTab & Format(1) & vbTab & Format(1) & vbTab & Format(0) & vbTab & Format(0) & vbTab & Format(-1) & vbTab & Format(-1)
         frmInLevel.listMovepolys.AddItem "<MOVEPOLY-" + Format(frmInLevel.listMovepolys.ListCount) + ">"
         frmStuff.opnStuff.Value = True
      End If
   ElseIf frmStuff.opnGate.Value = True Then
      If (gx1 = -1) And (gy1 = -1) Then
         gx1 = X + wox
         gy1 = Y + woy
      Else
         gx2 = X + wox
         gy2 = Y + woy
         W = gx2 - gx1
         H = gy2 - gy1
         frmStuff.opnStuff.Value = True
      End If
   End If
End Sub

Private Sub picMain_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
   frmStuff.lblLoc.Caption = Format(X + wox) + "," + Format(Y + woy)
   If frmStuff.opnStuff.Value = True Then
      If ss = 0 Then
         W = frmStuff.gridObjects.TextMatrix(frmStuff.gridObjects.Row, 3)
         H = frmStuff.gridObjects.TextMatrix(frmStuff.gridObjects.Row, 4)
      ElseIf ss = 1 Then
         W = frmStuff.gridEnemies.TextMatrix(frmStuff.gridEnemies.Row, 3)
         H = frmStuff.gridEnemies.TextMatrix(frmStuff.gridEnemies.Row, 4)
      ElseIf ss = 2 Then
         W = frmStuff.gridPickups.TextMatrix(frmStuff.gridPickups.Row, 3)
         H = frmStuff.gridPickups.TextMatrix(frmStuff.gridPickups.Row, 4)
      End If
   ElseIf frmStuff.opnHaji.Value = True Then
      W = 58
      H = 96
   ElseIf frmStuff.opnPoly.Value = True Then
      If (px1 <> -1) And (py1 <> -1) Then
         W = (X + wox) - px1
         H = (Y + woy) - py1
         X = px1 - wox
         Y = py1 - woy
      End If
   ElseIf frmStuff.opnMovePoly.Value = True Then
      If (mx1 <> -1) And (my1 <> -1) Then
         W = (X + wox) - mx1
         H = (Y + woy) - my1
         X = mx1 - wox
         Y = my1 - woy
      End If
   ElseIf frmStuff.opnGate.Value = True Then
      If (gx1 <> -1) And (gy1 <> -1) Then
         W = (X + wox) - gx1
         H = (Y + woy) - gy1
         X = gx1 - wox
         Y = gy1 - woy
      End If
   End If
   
   Call RedrawMainView
   frmMain.picMain.Line (X, Y)-(X + W, Y + H), RGB(0, 255, 0), B
   frmMain.picMain.Refresh
End Sub
