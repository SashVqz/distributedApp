cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "-static")

project(client LANGUAGES CXX)
add_executable(client ./Client/main_fm.cpp ./Client/fileManagerStub/fileManagerStub.h ./Client/fileManagerStub/fileManagerStub.cpp ./Client/localFileManager/localFilemanager.h ./Client/localFileManager/localFilemanager.cpp ./untilsApi/utils.h ./untilsApi/utils.cpp ./operaciones/operaciones.h)
target_link_libraries(client pthread)

project(server LANGUAGES CXX)
add_executable(server ./Server/mainServer.cpp ./Server/fileManagerImp/fileManagerImp.h ./Server/fileManagerImp/fileManagerImp.cpp ./Server/fileManagerClass/filemanager.h ./Server/fileManagerClass/filemanager.cpp ./untilsApi/utils.h ./untilsApi/utils.cpp ./operaciones/operaciones.h)
target_link_libraries(server pthread)