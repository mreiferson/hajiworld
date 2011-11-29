# Microsoft Developer Studio Project File - Name="HajiWorld" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=HajiWorld - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HajiWorld.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HajiWorld.mak" CFG="HajiWorld - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HajiWorld - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "HajiWorld - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HajiWorld - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\BuildOutput\"
# PROP Intermediate_Dir ".\BuildOutput\"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib dxguid.lib ddraw.lib dsound.lib dinput.lib winmm.lib digifx.lib /nologo /subsystem:windows /machine:I386 /out:".\Build\HajiWorld.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "HajiWorld - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\BuildOutput\"
# PROP Intermediate_Dir ".\BuildOutput\"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib dxguid.lib ddraw.lib dsound.lib dinput.lib winmm.lib digifx.lib /nologo /subsystem:windows /profile /debug /machine:I386 /nodefaultlib:"libc" /out:".\Build\HajiWorld.exe"

!ENDIF 

# Begin Target

# Name "HajiWorld - Win32 Release"
# Name "HajiWorld - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\hajiworld.cpp
# End Source File
# Begin Source File

SOURCE=.\hwcollision.cpp
# End Source File
# Begin Source File

SOURCE=.\hwddraw.cpp
# End Source File
# Begin Source File

SOURCE=.\hwdigifx.cpp
# End Source File
# Begin Source File

SOURCE=.\hwdinput.cpp
# End Source File
# Begin Source File

SOURCE=.\hwdsound.cpp
# End Source File
# Begin Source File

SOURCE=.\hwenemy.cpp
# End Source File
# Begin Source File

SOURCE=.\hwengine.cpp
# End Source File
# Begin Source File

SOURCE=.\hwexplosion.cpp
# End Source File
# Begin Source File

SOURCE=.\hwgfx.cpp
# End Source File
# Begin Source File

SOURCE=.\hwinput.cpp
# End Source File
# Begin Source File

SOURCE=.\hwmemcpy.cpp
# End Source File
# Begin Source File

SOURCE=.\hwmidi.cpp
# End Source File
# Begin Source File

SOURCE=.\hwobjectile.cpp
# End Source File
# Begin Source File

SOURCE=.\hwpickup.cpp
# End Source File
# Begin Source File

SOURCE=.\hwplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\hwsoundfx.cpp
# End Source File
# Begin Source File

SOURCE=.\hwutil.cpp
# End Source File
# Begin Source File

SOURCE=.\hwworld.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\defines.h
# End Source File
# Begin Source File

SOURCE=.\hajiworld.h
# End Source File
# Begin Source File

SOURCE=.\hwcollision.h
# End Source File
# Begin Source File

SOURCE=.\hwddraw.h
# End Source File
# Begin Source File

SOURCE=.\hwdigifx.h
# End Source File
# Begin Source File

SOURCE=.\hwdinput.h
# End Source File
# Begin Source File

SOURCE=.\hwdsound.h
# End Source File
# Begin Source File

SOURCE=.\hwenemy.h
# End Source File
# Begin Source File

SOURCE=.\hwengine.h
# End Source File
# Begin Source File

SOURCE=.\hwentity.h
# End Source File
# Begin Source File

SOURCE=.\hwexplosion.h
# End Source File
# Begin Source File

SOURCE=.\hwgfx.h
# End Source File
# Begin Source File

SOURCE=.\hwinput.h
# End Source File
# Begin Source File

SOURCE=.\hwlevel.h
# End Source File
# Begin Source File

SOURCE=.\hwmemcpy.h
# End Source File
# Begin Source File

SOURCE=.\hwmidi.h
# End Source File
# Begin Source File

SOURCE=.\hwobjectile.h
# End Source File
# Begin Source File

SOURCE=.\hwpickup.h
# End Source File
# Begin Source File

SOURCE=.\hwplayer.h
# End Source File
# Begin Source File

SOURCE=.\hwsoundfx.h
# End Source File
# Begin Source File

SOURCE=.\hwutil.h
# End Source File
# Begin Source File

SOURCE=.\hwworld.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\HajiWorld.rc
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# End Group
# End Target
# End Project
