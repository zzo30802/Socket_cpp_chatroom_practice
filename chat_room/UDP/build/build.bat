git clean -fdx
cmake ../ -G "Visual Studio 15 2017" -A x64
cmake --build . --config Release

ECHO %THIRD_PARTY%
