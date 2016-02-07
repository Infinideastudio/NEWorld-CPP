# Notice
Since the owner of this branch has been kicked from ths studio, the Linux version is deprecated.

# NEWorld-CPP
At linux braunh  

## Goal
Make NEWorld-CPP easier to build & run on Linux.  

## Installion
So far, there's nothing meaningful in this project.  
But you can compile and run it.  
Then you'll see a window with a white triangle.  

### Reqirements
1.boost:filesystem, system, signals2  
2.SDL2, SDL2_image  
3.OpenGL, GLEW, GLM  
4.Only tested on clang 3.6  

### Compilion
To compile it, run:

```shell
./build.py [-O3]
```

run it:

```shell
./run.sh
```

or

```shell
cd ./release/
./neworld.out
```

```shell
cd ./release
./neworld.out
```

I think you can't succeed in doing that...  
I'll improve that in the future...  

## Status
Global progress: (  7%)  
Version: (No stable) (0.0.1 in progress)  
Platforms: Linux(Ubuntu)  
Build Status:

| LInux(Ubuntu) |
|:-------------:|
|      FAIL     |

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
(O K) OpenGL math -> GLM
(N/A) OpenGL render frontend -> (No)
(O K) OpenGL -> OpenGL 3.3
(...) Render module
(O K) Multithreading -> std::thread
(O K) Settings
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

## TODO-List
Nothing

## Known Bugs
```
(#001) (UNKNOWN) GLContext::MakeCurrent causes context deletion failed. -> deprecate it.
(#002) (FIXED)(NEED IMPROVE) LogSystem can't write to files. -> Not my falut...
```
