

///// Project : Shared Memory for DLL //////

// 0 step : TARGET Struct // 

// 1 step : sharedmemory.hpp //

#ifndef SHARED_MEMORY_H
#define SHAREd_MEMORY_H

#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <atomic>

#include <TARGET.hpp>

typedef void(*StartDllDataProcessingPtr)();

sruct SharedMemory
{
    TARGETSTRUCT target_;
    
    StartDllDataProcessingPtr startDllDataProcesing { null ptr };
    
    std::mutex lock;
    
    bool isInitialized = false;
}
#endif

// 2. SharedMemoryDll.hpp // 
#ifndef SHARE_MEMORY_DLL_H
#define SHARE_MEMORY_DLL_H

#include <SharedMemory.hpp>
#include <string>

void InitSharedMemory();
void StartDllDataProcessing();

#define DllExport extern "C" __declspec(dllexport)
DllExport SharedMemory* GetInstanceOfSharedMemory(void);
#endif

// 3. dllmain.cpp //

#include <SharedMemory.hpp>
#include <SharedMemoryDll.hpp>
#include <windows.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

SharedMemory instance;

void InitSharedMemory()
{
    instance.startDllDataProcessing = StartDllDataprocessing;
}

bool CallBackFunctionFromHostToDll()
{
    std::cout << "this DLL function was called from Host" << "\n";
    return true;
}

void StartDllDataProcessing()
{
    std::unique_lock<std::mutex> lck(instance.lock);
    if(!instance.isInitialized)
    {
        instance.TARGET_.FUNCTION(); // TARGET !!
        instance.isInitialized= true;
    }
}

SharedMemory* GetInstanceOfSharedMemory(void)
{
    return &instance;
}

extern "C" BOOL APIENTRY DllMain(HMODULE, DWORD reason, LPVOID)
{
    if(reason == DLL_PROCESS_ATTACH)
    {
        InitSharedMemory();
    }
    if (reason == DLL_PROCESS_DETACH)
    {
        
    }
    return true;
}


// 4. MAIN // 

SharedMemory* sharedMemoryPtr = nullptr;

int main()
{
    ...
    int calc(int argc, char*argv[]);
}


int calc(int argc, char*argv[])
{
    try
    {
        if(argc > 1)
        {
            HINSTANCE dllHandle = LoadLibrary("LSharedMemoryDLL.dll");
            if(dllHandle==NULL)
            {
                std::error << "Loading of Library Failed!" << "\n";
                return 1;
            }
            
            GetInstanceOfSharedMemory getInstanceOfSharedMemory =(GetInstanceOfSharedMemor) GetProcAddress(dllHandle, "GetInstanceOfSharedMemory");
            
            if(getInstanceOfSharedMemory == NULL)
            {
                std::cerror << " Cannot get proc address of \"getInstanceOfSharedMemory\"" << "\n";
                return 2;
            }
            sharedMemoryPtr = getInstanceOfSharedMemory();
            sharedMemoryPtr -> startDllDataProcessing();
            ....
            }
        }
    }
}






