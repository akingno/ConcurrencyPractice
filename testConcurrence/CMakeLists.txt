cmake_minimum_required(VERSION 3.27)
project(testConcurrence)

set(CMAKE_CXX_STANDARD 20)


add_executable(testConcurrence main.cpp
        ThreadPool.h
        ThreadPoolImplement.h
        MyTask.h
        stdafx.h
        Task.h
        SafePriorityQueue.h
        SafeQueue.h)
target_precompile_headers(testConcurrence PRIVATE stdafx.h)
