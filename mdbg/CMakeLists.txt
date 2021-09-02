cmake_minimum_required(VERSION 3.16)
project(tinydbg)

# 编译EXE,DLL不依赖运行库
if(MSVC)     
    # Use the static C library for all build types
    foreach(var 
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
      )
      if(${var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${var} "${${var}}") 
      endif()
    endforeach()    
endif(MSVC)


# 生成的DLL,EXE文件带PDB调试信息
if (WIN32)
        string(REPLACE "/Ob2" "/Ob1" out "${CMAKE_CXX_FLAGS_RELEASE}")
        string(REPLACE "/Ob2" "/Ob1" out "${CMAKE_C_FLAGS_RELEASE}")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /DRELEASE")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /DRELEASE")
        SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SUBSYSTEM:CONSOLE /DEBUG")
endif()

# 增加宏定义
# add_definitions(-DUNICODE -D_UNICODE)
# ADD_DEFINITIONS(-D_TEST_)

# 包括目录头文件
# include_directories(${protest}/test/include)

# 自动搜索指定目录下的文件列到目录下
# file(GLOB_RECURSE PUBLIC_HDR_DIR *.h ${protest}/test/include/*.h ${protest}/protest/include/*.h)
# file(GLOB_RECURSE SRCS  *.cpp ${protest}/test/src/*.cpp  ${protest}/test/src/*.cpp)

# 设置编译器版本
# set(CXX_STANDARD "-std=c++14")

set(SDR ${PROJECT_SOURCE_DIR})

set(DIRSRC ${SDR}/application.cpp
${SDR}/application.h
${SDR}/mdbg.cpp
${SDR}/mdbg.h
${SDR}/processdlg.cpp
${SDR}/processdlg.h
${SDR}/consolemgr.cpp
${SDR}/consolemgr.h
${SDR}/chook.cpp
${SDR}/chook.h
${SDR}/dbghelper.cpp
${SDR}/dbghelper.h
${SDR}/mdbg.rc)

ADD_EXECUTABLE(tinydbg WIN32 ${DIRSRC})