cmake_minimum_required(VERSION 3.27)
project(testConcurrence)

set(CMAKE_CXX_STANDARD 20)


add_executable(testConcurrence main.cpp
        ThreadPool.h
        ThreadPool.cpp
        Globals.h
        MyTask.h
        stdafx.h
        Task.h)
target_precompile_headers(testConcurrence PRIVATE stdafx.h)
