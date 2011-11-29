Attribute VB_Name = "modFuncs"
'Application Global Stuff
Declare Function BitBlt Lib "gdi32" (ByVal hDestDC As Long, ByVal X As Long, ByVal Y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal dwRop As Long) As Long
Declare Function SetWindowPos Lib "User32" (ByVal hWnd As Long, ByVal hWndInsertAfter As Long, ByVal X As Long, ByVal Y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long) As Long
Public Const SRCAND = &H8800C6
Public Const SRCCOPY = &HCC0020
Public Const SRCINVERT = &H660046
Public Const HWND_TOP = 0
Public Const HWND_TOPMOST = -1
Public Const HWND_NOTOPMOST = -2
Public Const SWP_NOMOVE = &H2
Public Const SWP_NOSIZE = &H1
Public Const Flags = SWP_NOMOVE Or SWP_NOSIZE
Public Const MMScale = 8
Public Const iViewableX = 640
Public Const iViewableY = 448
Public iNumObjects, iNumEnemies, iNumPickups
Public iLevelWidth, iLevelHeight, iSectorsX, iSectorsY
Public wox, woy
Public ss
Public hx, hy, hd
Public px1, py1, px2, py2
Public mx1, my1, mx2, my2
Public gx1, gy1, gx2, gy2
Public levelfn

Sub StayOnTop(frmForm As Form)
   whocares = SetWindowPos(frmForm.hWnd, HWND_TOPMOST, 0, 0, 0, 0, Flags)
End Sub

Sub NewLevel()
   wox = 0
   woy = (iSectorsY - 1) * 448
   hx = 0
   hy = iLevelHeight - 49 - 96
   frmInLevel.gridPolysData.AddItem "<POLY-" + Format(frmInLevel.listPolys.ListCount) + ">" & vbTab & "4" & vbTab & Format(0) & vbTab & Format(iLevelHeight - 49) & vbTab & Format(iLevelWidth) & vbTab & Format(iLevelHeight - 49) & vbTab & Format(iLevelWidth) & vbTab & Format(iLevelHeight - 39) & vbTab & Format(0) & vbTab & Format(iLevelHeight - 39) & vbTab & "1" & vbTab & "0"
   frmInLevel.listPolys.AddItem "<POLY-" + Format(frmInLevel.listPolys.ListCount) + ">"
   levelfn = ""
End Sub

Sub LoadLevel(ByVal sLoca As String)
   Open sLoca For Input As #1
      'read info block
      Line Input #1, buf '<INFO>
      Line Input #1, buf
      iLevelWidth = Val(buf)
      Line Input #1, buf
      iLevelHeight = Val(buf)
      Line Input #1, buf
      iSectorsX = Val(buf)
      Line Input #1, buf
      iSectorsY = Val(buf)
      Line Input #1, buf
      hx = Val(buf)
      Line Input #1, buf
      hy = Val(buf) - 96
      Line Input #1, buf
      wox = Val(buf)
      Line Input #1, buf
      woy = Val(buf)
      Line Input #1, buf
      ecount = Val(buf)
      Line Input #1, buf
      mpcount = Val(buf)
      Line Input #1, buf
      pcount = Val(buf)
      Line Input #1, buf '</INFO>
      
      'read gate
      Line Input #1, buf '<GATE>
      Line Input #1, gx1
      Line Input #1, gy1
      Line Input #1, gx2
      Line Input #1, gy2
      Line Input #1, buf '</GATE>
      
      'read movepolys
      For i = 0 To mpcount - 1
         frmInLevel.listMovepolys.AddItem "<MOVEPOLY-" + Format(frmInLevel.listMovepolys.ListCount) + ">"
         Line Input #1, buf '<MOVEPOLY>
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
         Line Input #1, buf '</MOVEPOLY>
         frmInLevel.gridMovepolysData.AddItem "<MOVEPOLY-" + Format(frmInLevel.listMovepolys.ListCount - 1) + ">" & vbTab & buf1 & vbTab & buf2 & vbTab & buf3 & vbTab & buf4 & vbTab & buf5 & vbTab & buf6 & vbTab & buf7 & vbTab & buf8 & vbTab & buf9 & vbTab & buf10 & vbTab & buf11
      Next i
      
      'read objects
      Line Input #1, buf
      While buf = "<OBJECT>"
         Line Input #1, buf1
         frmInLevel.listObjects.AddItem frmStuff.gridObjects.TextMatrix(Val(buf1) + 1, 0)
         Line Input #1, buf2
         Line Input #1, buf3
         Line Input #1, buf4
         Line Input #1, buf5
         Line Input #1, buf '</OBJECT>
         frmInLevel.gridObjectsData.AddItem frmStuff.gridObjects.TextMatrix(Val(buf1) + 1, 0) & vbTab & buf1 & vbTab & buf2 & vbTab & buf3 & vbTab & buf4 & vbTab & buf5
         Line Input #1, buf
      Wend
      
      'read enemies
      For i = 0 To ecount - 1
         Line Input #1, buf1
         frmInLevel.listEnemies.AddItem frmStuff.gridEnemies.TextMatrix(Val(buf1) + 1, 0)
         Line Input #1, buf2
         Line Input #1, buf3
         Line Input #1, buf4
         Line Input #1, buf5
         Line Input #1, buf6
         Line Input #1, buf '</ENEMY>
         frmInLevel.gridEnemiesData.AddItem frmStuff.gridEnemies.TextMatrix(Val(buf1) + 1, 0) & vbTab & buf1 & vbTab & buf2 & vbTab & buf3 & vbTab & buf4 & vbTab & buf5 & vbTab & buf6
         Line Input #1, buf
      Next i
      
      'read polys
      While buf = "<POLY>"
         frmInLevel.listPolys.AddItem "<POLY-" + Format(frmInLevel.listPolys.ListCount) + ">"
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
         Line Input #1, buf '</POLY>
         frmInLevel.gridPolysData.AddItem "<POLY-" + Format(frmInLevel.listPolys.ListCount - 1) + ">" & vbTab & buf1 & vbTab & buf2 & vbTab & buf3 & vbTab & buf4 & vbTab & buf5 & vbTab & buf6 & vbTab & buf7 & vbTab & buf8 & vbTab & buf9 & vbTab & buf10 & vbTab & buf11 & vbTab & buf12
         Line Input #1, buf
      Wend
      
      'read pickups
      For i = 0 To pcount - 1
         Line Input #1, buf1
         frmInLevel.listPickups.AddItem frmStuff.gridPickups.TextMatrix(Val(buf1) + 1, 0)
         Line Input #1, buf2
         Line Input #1, buf3
         Line Input #1, buf '</PICKUP>
         frmInLevel.gridPickupsData.AddItem frmStuff.gridPickups.TextMatrix(Val(buf1) + 1, 0) & vbTab & buf1 & vbTab & buf2 & vbTab & buf3
         If i <> (pcount - 1) Then Line Input #1, buf
      Next i
   Close #1
End Sub

Sub SaveLevel(ByVal sLoca As String)
   Open sLoca For Output As #1
      'write info block
      Print #1, "<INFO>"
      Print #1, Format(iLevelWidth)
      Print #1, Format(iLevelHeight)
      Print #1, Format(iSectorsX)
      Print #1, Format(iSectorsY)
      Print #1, Format(hx)
      Print #1, Format(hy + 96)
      Print #1, Format(wox)
      Print #1, Format(woy)
      Print #1, Format(frmInLevel.listEnemies.ListCount)
      Print #1, Format(frmInLevel.listMovepolys.ListCount)
      Print #1, Format(frmInLevel.listPickups.ListCount)
      Print #1, "</INFO>"
      
      'write gate
      Print #1, "<GATE>"
      Print #1, Format(gx1)
      Print #1, Format(gy1)
      Print #1, Format(gx2)
      Print #1, Format(gy2)
      Print #1, "</GATE>"
      
      'write movepolys
      For i = 0 To frmInLevel.listMovepolys.ListCount - 1
         Print #1, "<MOVEPOLY>"
         Print #1, frmInLevel.gridMovepolysData.TextMatrix(i, 1)
         Print #1, frmInLevel.gridMovepolysData.TextMatrix(i, 2)
         Print #1, frmInLevel.gridMovepolysData.TextMatrix(i, 3)
         Print #1, frmInLevel.gridMovepolysData.TextMatrix(i, 4)
         Print #1, frmInLevel.gridMovepolysData.TextMatrix(i, 5)
         Print #1, frmInLevel.gridMovepolysData.TextMatrix(i, 6)
         Print #1, frmInLevel.gridMovepolysData.TextMatrix(i, 7)
         Print #1, frmInLevel.gridMovepolysData.TextMatrix(i, 8)
         Print #1, frmInLevel.gridMovepolysData.TextMatrix(i, 9)
         Print #1, frmInLevel.gridMovepolysData.TextMatrix(i, 10)
         Print #1, frmInLevel.gridMovepolysData.TextMatrix(i, 11)
         Print #1, "</MOVEPOLY>"
      Next i
      
      'write objects
      For i = 0 To frmInLevel.listObjects.ListCount - 1
         Print #1, "<OBJECT>"
         Print #1, frmInLevel.gridObjectsData.TextMatrix(i, 1)
         Print #1, frmInLevel.gridObjectsData.TextMatrix(i, 2)
         Print #1, frmInLevel.gridObjectsData.TextMatrix(i, 3)
         Print #1, frmInLevel.gridObjectsData.TextMatrix(i, 4)
         Print #1, frmInLevel.gridObjectsData.TextMatrix(i, 5)
         Print #1, "</OBJECT>"
      Next i
      
      'write enemies
      For i = 0 To frmInLevel.listEnemies.ListCount - 1
         Print #1, "<ENEMY>"
         Print #1, frmInLevel.gridEnemiesData.TextMatrix(i, 1)
         Print #1, frmInLevel.gridEnemiesData.TextMatrix(i, 2)
         Print #1, frmInLevel.gridEnemiesData.TextMatrix(i, 3)
         Print #1, frmInLevel.gridEnemiesData.TextMatrix(i, 4)
         Print #1, frmInLevel.gridEnemiesData.TextMatrix(i, 5)
         Print #1, frmInLevel.gridEnemiesData.TextMatrix(i, 6)
         Print #1, "</ENEMY>"
      Next i
      
      'write polys
      For i = 0 To frmInLevel.listPolys.ListCount - 1
         Print #1, "<POLY>"
         Print #1, frmInLevel.gridPolysData.TextMatrix(i, 1)
         Print #1, frmInLevel.gridPolysData.TextMatrix(i, 2)
         Print #1, frmInLevel.gridPolysData.TextMatrix(i, 3)
         Print #1, frmInLevel.gridPolysData.TextMatrix(i, 4)
         Print #1, frmInLevel.gridPolysData.TextMatrix(i, 5)
         Print #1, frmInLevel.gridPolysData.TextMatrix(i, 6)
         Print #1, frmInLevel.gridPolysData.TextMatrix(i, 7)
         Print #1, frmInLevel.gridPolysData.TextMatrix(i, 8)
         Print #1, frmInLevel.gridPolysData.TextMatrix(i, 9)
         Print #1, frmInLevel.gridPolysData.TextMatrix(i, 10)
         Print #1, frmInLevel.gridPolysData.TextMatrix(i, 11)
         Print #1, frmInLevel.gridPolysData.TextMatrix(i, 12)
         Print #1, "</POLY>"
      Next i
      
      'write pickups
      For i = 0 To frmInLevel.listPickups.ListCount - 1
         Print #1, "<PICKUP>"
         Print #1, frmInLevel.gridPickupsData.TextMatrix(i, 1)
         Print #1, frmInLevel.gridPickupsData.TextMatrix(i, 2)
         Print #1, frmInLevel.gridPickupsData.TextMatrix(i, 3)
         Print #1, "</PICKUP>"
      Next i
   Close #1
End Sub

Sub RedrawMainView()
   frmMain.picMain.Cls
   X = 0
   Y = 397 + (iLevelHeight - (woy + ((iSectorsY - 1) * 448)))
   frmMain.picMain.Line (X, Y)-(X + 640, Y), RGB(0, 128, 0)
   For i = 0 To 640 Step 10
      frmMain.picMain.Line (i, Y)-(i - 10, Y + 83), RGB(0, 128, 0)
   Next i
   For i = 0 To frmInLevel.listObjects.ListCount - 1
      X = frmInLevel.gridObjectsData.TextMatrix(i, 3) - wox
      Y = frmInLevel.gridObjectsData.TextMatrix(i, 4) - woy
      id = frmInLevel.gridObjectsData.TextMatrix(i, 1)
      W = frmStuff.gridObjects.TextMatrix(id + 1, 3)
      H = frmStuff.gridObjects.TextMatrix(id + 1, 4)
      frmMain.picMain.Line (X, Y)-(X + W, Y + H), RGB(128, 128, 128), BF
      frmMain.picMain.Line (X, Y)-(X + W, Y + H), RGB(255, 255, 255), B
      frmMain.picMain.ForeColor = RGB(255, 255, 0)
      frmMain.picMain.CurrentX = X
      frmMain.picMain.CurrentY = Y
      frmMain.picMain.Print i
   Next i
   For i = 0 To frmInLevel.listEnemies.ListCount - 1
      X = frmInLevel.gridEnemiesData.TextMatrix(i, 2) - wox
      Y = frmInLevel.gridEnemiesData.TextMatrix(i, 3) - woy
      id = frmInLevel.gridEnemiesData.TextMatrix(i, 1)
      W = frmStuff.gridEnemies.TextMatrix(id + 1, 3)
      H = frmStuff.gridEnemies.TextMatrix(id + 1, 4)
      frmMain.picMain.Line (X, Y)-(X + W, Y + H), RGB(0, 0, 255), BF
      frmMain.picMain.Line (X, Y)-(X + W, Y + H), RGB(255, 255, 255), B
      frmMain.picMain.ForeColor = RGB(255, 255, 0)
      frmMain.picMain.CurrentX = X
      frmMain.picMain.CurrentY = Y
      frmMain.picMain.Print i
   Next i
   For i = 0 To frmInLevel.listPickups.ListCount - 1
      X = frmInLevel.gridPickupsData.TextMatrix(i, 2) - wox
      Y = frmInLevel.gridPickupsData.TextMatrix(i, 3) - woy
      id = frmInLevel.gridPickupsData.TextMatrix(i, 1)
      W = frmStuff.gridPickups.TextMatrix(id + 1, 3)
      H = frmStuff.gridPickups.TextMatrix(id + 1, 4)
      frmMain.picMain.Line (X, Y)-(X + W, Y + H), RGB(255, 255, 0), BF
      frmMain.picMain.Line (X, Y)-(X + W, Y + H), RGB(255, 255, 255), B
      frmMain.picMain.ForeColor = RGB(0, 0, 0)
      frmMain.picMain.CurrentX = X
      frmMain.picMain.CurrentY = Y
      frmMain.picMain.Print i
   Next i
   For i = 0 To frmInLevel.listPolys.ListCount - 1
      X = frmInLevel.gridPolysData.TextMatrix(i, 2) - wox
      Y = frmInLevel.gridPolysData.TextMatrix(i, 3) - woy
      id = i
      W = frmInLevel.gridPolysData.TextMatrix(i, 6) - wox - X
      H = frmInLevel.gridPolysData.TextMatrix(i, 7) - woy - Y
      frmMain.picMain.Line (X, Y)-(X + W, Y + H), RGB(255, 0, 0), B
      frmMain.picMain.ForeColor = RGB(255, 255, 0)
      frmMain.picMain.CurrentX = X + W - 10
      frmMain.picMain.CurrentY = Y
      frmMain.picMain.Print i
   Next i
   For i = 0 To frmInLevel.listMovepolys.ListCount - 1
      X = frmInLevel.gridMovepolysData.TextMatrix(i, 1) - wox
      Y = frmInLevel.gridMovepolysData.TextMatrix(i, 2) - woy
      id = i
      W = frmInLevel.gridMovepolysData.TextMatrix(i, 3)
      H = frmInLevel.gridMovepolysData.TextMatrix(i, 4)
      frmMain.picMain.Line (X, Y)-(X + W, Y + H), RGB(128, 0, 0), B
      frmMain.picMain.ForeColor = RGB(255, 255, 0)
      frmMain.picMain.CurrentX = X + W - 10
      frmMain.picMain.CurrentY = Y
      frmMain.picMain.Print i
   Next i
   X = gx1 - wox
   Y = gy1 - woy
   W = gx2 - gx1
   H = gy2 - gy1
   frmMain.picMain.Line (X, Y)-(X + W, Y + H), RGB(128, 255, 0), BF
   frmMain.picMain.Line (X, Y)-(X + W, Y + H), RGB(255, 255, 255), B
   frmMain.picMain.ForeColor = RGB(128, 255, 128)
   frmMain.picMain.Line (hx - wox, hy - woy)-(hx - wox + 58, hy - woy + 96), RGB(0, 255, 255), BF
   frmMain.picMain.Line (hx - wox, hy - woy)-(hx - wox + 58, hy - woy + 96), RGB(255, 255, 255), B
   frmMain.picMain.Refresh
End Sub

Sub RedrawMMView()
   frmMiniMap.picMM.Cls
   frmMiniMap.picMM.Line (0, (iLevelHeight - 83) / 8)-(640, iLevelHeight / 8), RGB(0, 128, 0), BF
   For i = 0 To frmInLevel.listObjects.ListCount - 1
      X = frmInLevel.gridObjectsData.TextMatrix(i, 3)
      Y = frmInLevel.gridObjectsData.TextMatrix(i, 4)
      id = frmInLevel.gridObjectsData.TextMatrix(i, 1)
      W = frmStuff.gridObjects.TextMatrix(id + 1, 3)
      H = frmStuff.gridObjects.TextMatrix(id + 1, 4)
      X = X / MMScale
      Y = Y / MMScale
      W = W / MMScale
      H = H / MMScale
      frmMiniMap.picMM.Line (X, Y)-(X + W, Y + H), RGB(128, 128, 128), BF
      frmMiniMap.picMM.Line (X, Y)-(X + W, Y + H), RGB(255, 255, 255), B
   Next i
   For i = 0 To frmInLevel.listEnemies.ListCount - 1
      X = frmInLevel.gridEnemiesData.TextMatrix(i, 2)
      Y = frmInLevel.gridEnemiesData.TextMatrix(i, 3)
      id = frmInLevel.gridEnemiesData.TextMatrix(i, 1)
      W = frmStuff.gridEnemies.TextMatrix(id + 1, 3)
      H = frmStuff.gridEnemies.TextMatrix(id + 1, 4)
      X = X / MMScale
      Y = Y / MMScale
      W = W / MMScale
      H = H / MMScale
      frmMiniMap.picMM.Line (X, Y)-(X + W, Y + H), RGB(0, 0, 255), BF
      frmMiniMap.picMM.Line (X, Y)-(X + W, Y + H), RGB(255, 255, 255), B
   Next i
   For i = 0 To frmInLevel.listPickups.ListCount - 1
      X = frmInLevel.gridPickupsData.TextMatrix(i, 2)
      Y = frmInLevel.gridPickupsData.TextMatrix(i, 3)
      id = frmInLevel.gridPickupsData.TextMatrix(i, 1)
      W = frmStuff.gridPickups.TextMatrix(id + 1, 3)
      H = frmStuff.gridPickups.TextMatrix(id + 1, 4)
      X = X / MMScale
      Y = Y / MMScale
      W = W / MMScale
      H = H / MMScale
      frmMiniMap.picMM.Line (X, Y)-(X + W, Y + H), RGB(255, 255, 0), BF
      frmMiniMap.picMM.Line (X, Y)-(X + W, Y + H), RGB(255, 255, 255), B
   Next i
   For i = 0 To frmInLevel.listPolys.ListCount - 1
      X = frmInLevel.gridPolysData.TextMatrix(i, 2)
      Y = frmInLevel.gridPolysData.TextMatrix(i, 3)
      id = i
      W = frmInLevel.gridPolysData.TextMatrix(i, 6) - X
      H = frmInLevel.gridPolysData.TextMatrix(i, 7) - Y
      X = X / MMScale
      Y = Y / MMScale
      W = W / MMScale
      H = H / MMScale
      frmMiniMap.picMM.Line (X, Y)-(X + W, Y + H), RGB(255, 0, 0), B
   Next i
   For i = 0 To frmInLevel.listMovepolys.ListCount - 1
      X = frmInLevel.gridMovepolysData.TextMatrix(i, 1)
      Y = frmInLevel.gridMovepolysData.TextMatrix(i, 2)
      id = i
      W = frmInLevel.gridMovepolysData.TextMatrix(i, 3)
      H = frmInLevel.gridMovepolysData.TextMatrix(i, 4)
      X = X / MMScale
      Y = Y / MMScale
      W = W / MMScale
      H = H / MMScale
      frmMiniMap.picMM.Line (X, Y)-(X + W, Y + H), RGB(128, 0, 0), B
   Next i
   X = gx1
   Y = gy1
   W = gx2 - gx1
   H = gy2 - gy1
   X = X / MMScale
   Y = Y / MMScale
   W = W / MMScale
   H = H / MMScale
   frmMiniMap.picMM.Line (X, Y)-(X + W, Y + H), RGB(128, 255, 0), BF
   frmMiniMap.picMM.Line (X, Y)-(X + W, Y + H), RGB(255, 255, 255), B
   X = hx
   Y = hy
   W = 58
   H = 96
   X = X / MMScale
   Y = Y / MMScale
   W = W / MMScale
   H = H / MMScale
   frmMiniMap.picMM.Line (X, Y)-(X + W, Y + H), RGB(0, 255, 255), BF
   frmMiniMap.picMM.Line (X, Y)-(X + W, Y + H), RGB(255, 255, 255), B
   X = wox / MMScale
   Y = woy / MMScale
   W = iViewableX / MMScale
   H = iViewableY / MMScale
   frmMiniMap.picMM.Line (X, Y)-(X + W, Y + H), RGB(255, 255, 0), B
   frmMiniMap.picMM.Refresh
End Sub
