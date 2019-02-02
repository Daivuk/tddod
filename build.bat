REM Clone onut
git submodule update --init

REM Create build dir
mkdir build

REM cd to build dir
cd build

REM We want to use hunter and we want to build the stand alone (onut.exe)
cmake -DCMAKE_BUILD_TYPE=Release ..

REM Compile
MSBuild.exe tddod.sln /t:tddod /p:Configuration=Release /p:Platform=Win32

cd ../../..
