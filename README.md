# NEWorld-CPP
At linux braunh  

# Goal
Make NEWorld-CPP easier to build & run on Linux.  

# Status
Global progress: (  2%)  
Version: (No stable) (0.0.1 In progress)  
Platforms: Linux(Ubuntu)  
Build Status:  
| Linux |  
|:-----:|  
|  PASS |  

Details:
```
(O K) Compiler -> clang/msvc
(O K) Build system -> build.py
(O K) Syntax check -> check.py
(O K) Unit test -> (Unknown: Gtest or by myself)
(O K) File IO -> Boost.Filesystem & std::fstream
(O K) Logging -> LogSystem.hpp
(O K) Config(json) -> rapidjson
(O K) Windows/context manager -> SDL2
(O K) Event system -> Boost.Signal2
(O K) Graphics API -> OpenGL
(...) OpenGL math -> GLM
(N/A) OpenGL render frontend -> (No)
(O K) OpenGL -> OpenGL 3.3
( X ) Render module
(...) Multithreading -> std::thread
( X ) Picture loader -> SDL2-image
( X ) TrueType support -> SDL2-TTF
( X ) Core algorithms -> By @Null and @qiaozhanrong
( X ) Core features -> By studio
( X ) NEWorld (This is hard work...)
(N/A) Math algorithmd -> On hte internet...
( X ) Platforms -> Windows/Linux/(OSX)
(N/A) Networking -> (Unknown: Boost.Asio or SDL2-network)
(N/A) Server -> (By studio)
(N/A) Sound/music ->fmod
(N/A) Physics -> (Unknown: ODE or by @qiaozhanrong)
(N/A) Unicode support -> ICU **
(N/A) Mod -> (Unknown: luajit or python3)
(...) Optimize the program forever...
(...) Keep update forever...
(...) Complete docs...

**: Hmm... Maybe wstring...
```

# Known Bugs
```
(#001) (UNKNOWN) GLContext::MakeCurrent causes context deletion failed. -> deprecate it.  
```
