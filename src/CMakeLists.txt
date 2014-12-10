cmake_minimum_required(VERSION 2.4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -Werror -O2 -g -DBOOST_FILESYSTEM_NO_DEPRECATED -DBOOST_SYSTEM_NO_DEPRECATED -DBOOST_THREAD_PROVIDES_FUTURE -DBOOST_THREAD_USE_LIB")

include_directories("c:/MyLibs/SDL2-2.0.3/include"
    "C:/MyLibs/SDL2_image-2.0.0/i686-w64-mingw32/include/SDL2"
    "c:/MyLibs/rapidjson-0.11/include")

include_directories(SYSTEM "c:/MyLibs/boost_1_52_0")

# Must appear before the add_executable line.
link_directories("C:/MyLibs/SDL2-2.0.3/i686-w64-mingw32/lib"
    "C:/MyLibs/SDL2_image-2.0.0/i686-w64-mingw32/lib"
    "c:/MyLibs/boost_1_52_0/lib")

set(EXE game)
set(SRC SdlTexture.cpp SdlWindow.cpp SimpleMap.cpp sdl_utils.cpp voronoi.cpp
    main.cpp)
add_executable(${EXE} ${SRC})

# Must appear after add_executable line.
target_link_libraries(${EXE} mingw32 SDL2main SDL2 SDL2_image
    boost_filesystem-mgw47-s-1_52 boost_system-mgw47-s-1_52)

#set(EXE_MAPVIEW mapview)
#set(SRC_MAPVIEW AdventureMap.cpp HexGrid.cpp MapView.cpp SdlTexture.cpp
#    SdlTextureAtlas.cpp SdlWindow.cpp json_utils.cpp sdl_utils.cpp
#    map_viewer.cpp)
#add_executable(${EXE_MAPVIEW} ${SRC_MAPVIEW})

# Must appear after add_executable line.
#target_link_libraries(${EXE_MAPVIEW} mingw32 SDL2main SDL2 SDL2_image
#    boost_thread-mgw47-mt-s-1_52 boost_filesystem-mgw47-s-1_52
#    boost_system-mgw47-s-1_52)
