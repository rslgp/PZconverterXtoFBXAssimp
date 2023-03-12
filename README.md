# PZconverterXtoFBXAssimp
PZ converter from .X files to FBX using Assimp

on windows
setup assimp lib and inclue using vcpkg

```
mkdir E:\Microsoft Visual Studio\vcpkg
cd E:\Microsoft Visual Studio\vcpkg
git clone https://github.com/microsoft/vcpkg
"E:\Microsoft Visual Studio\vcpkg\vcpkg\bootstrap-vcpkg.bat"
"E:\Microsoft Visual Studio\vcpkg\vcpkg\vcpkg.exe" integrate install
"E:\Microsoft Visual Studio\vcpkg\vcpkg\vcpkg.exe" install assimp:x64-windows
"E:\Microsoft Visual Studio\vcpkg\vcpkg\vcpkg.exe" install assimp
```

create a new empty project on visual studio, c++ console

right click project name, add new item, and paste the cpp from this repo

need to do a better sanitizer function, feel free to fork this
