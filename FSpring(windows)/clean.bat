IF EXIST "x64" (  
    RMDIR /S /Q "x64"
)
IF EXIST "Debug" (  
    RMDIR /S /Q "Debug"
)
IF EXIST ".vs" (  
    RMDIR /S /Q ".vs"
)
IF EXIST "FSpring/x64" (  
    RMDIR /S /Q "FSpring/x64"
)
IF EXIST "FSpring(win64)\\FSpring.ipdb" (  
    DEL  "FSpring(win64)\\FSpring.ipdb"
)
IF EXIST "FSpring(win64)\\FSpring.iobj" (  
    DEL  "FSpring(win64)\\FSpring.iobj"
)
IF EXIST "FSpring(win64)\\FSpring.pdb" (  
    DEL  "FSpring(win64)\\FSpring.pdb"
)
xcopy /Y "3rdparty\bin\win64\*.dll" "FSpring(win64)"