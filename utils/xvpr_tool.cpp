#include "xvpr_tool.h"
#include "utils/logger.h"

namespace {
    const std::wstring xvpr_dll_path = L"./libs/xvpr.dll";
}

XVPR_Tool* XVPR_Tool::m_instance = nullptr;
XVPR_Tool::XVPR_Tool()
{
    m_xvpr_dllHandle = nullptr;
    m_xvpr_dllHandle = LoadLibrary(xvpr_dll_path.c_str());
    if(!m_xvpr_dllHandle){
        Logger::Error("XVPR - xvpr_dll load failed");
    }
}

XVPR_Tool::~XVPR_Tool()
{
    if(m_xvpr_dllHandle != nullptr) {
        FreeLibrary(m_xvpr_dllHandle);
        m_xvpr_dllHandle = nullptr;
    }
    if(XVPR_Tool::m_instance != nullptr) {
        delete XVPR_Tool::m_instance;
        XVPR_Tool::m_instance = nullptr;
    }
}

XVPR_Tool* XVPR_Tool::GetInstance()
{
    if(m_instance == nullptr){
        m_instance = new XVPR_Tool();
    }
    return m_instance;
}

bool XVPR_Tool::xvpr_vad_create_vad(size_t sr,size_t max_length,XVAD* &xvad_handle)
{
    try{
        if(m_xvpr_dllHandle == nullptr) {
            return false;
        }

        typedef XVAD* (*xvpr_vad_create_vad)(size_t sr,size_t max_length);
        xvpr_vad_create_vad xvpr_vad_create_vadFunc = (xvpr_vad_create_vad)GetProcAddress(m_xvpr_dllHandle, "xvpr_vad_create_vad");
        if(!xvpr_vad_create_vadFunc){
            Logger::Error("XVPR - xvpr_vad_create_vad 函数获取失败");
            return false;
        }
        xvad_handle = xvpr_vad_create_vadFunc(sr,max_length);
        return true;
    }catch(std::exception ex)
    {
        Logger::Error(QString("XVPR - xvpr_vad_create_vad exception - %1").arg(ex.what()));
        return false;
    }
}

bool XVPR_Tool::xvpr_vad_detect_valid_speech(XVAD *handle,short *input,size_t length,short **output,size_t &valid_size)
{
    try{
        if(m_xvpr_dllHandle == nullptr) {
            return false;
        }

        typedef size_t (*xvpr_vad_detect_valid_speech)(XVAD *handle, short *input, size_t length, short **output);
        xvpr_vad_detect_valid_speech xvpr_vad_detect_valid_speechFunc = (xvpr_vad_detect_valid_speech)GetProcAddress(m_xvpr_dllHandle, "xvpr_vad_detect_valid_speech");
        if(!xvpr_vad_detect_valid_speechFunc){
            Logger::Error("XVPR - xvpr_vad_detect_valid_speech 函数获取失败");
            return false;
        }
        valid_size = xvpr_vad_detect_valid_speechFunc(handle,input,length,output);
        return true;
    }catch(std::exception ex)
    {
        Logger::Error(QString("XVPR - xvpr_vad_detect_valid_speech exception - %1").arg(ex.what()));
        return false;
    }
}

bool XVPR_Tool::xvpr_vad_is_speaking(XVAD *handle,short *stream,size_t buf_len,int &status)
{
    try{
        if(m_xvpr_dllHandle == nullptr) {
            return false;
        }

        typedef int (*xvpr_vad_is_speaking)(XVAD *handle, short *stream, size_t buf_len);
        xvpr_vad_is_speaking xvpr_vad_is_speakingFunc = (xvpr_vad_is_speaking)GetProcAddress(m_xvpr_dllHandle, "xvpr_vad_is_speaking");
        if(!xvpr_vad_is_speakingFunc){
            Logger::Error("XVPR - xvpr_vad_is_speaking 函数获取失败");
            return false;
        }
        status = xvpr_vad_is_speakingFunc(handle,stream,buf_len);
        return true;
    }catch(std::exception ex)
    {
        Logger::Error(QString("XVPR - xvpr_vad_is_speaking exception - %1").arg(ex.what()));
        return false;
    }
}

bool XVPR_Tool::xvpr_vad_reset_vad(XVAD *handle)
{
    try{
        if(m_xvpr_dllHandle == nullptr) {
            return false;
        }

        typedef void (*xvpr_vad_reset_vad)(XVAD *handle);
        xvpr_vad_reset_vad xvpr_vad_reset_vadFunc = (xvpr_vad_reset_vad)GetProcAddress(m_xvpr_dllHandle, "xvpr_vad_reset_vad");
        if(!xvpr_vad_reset_vadFunc){
            Logger::Error("XVPR - xvpr_vad_reset_vad 函数获取失败");
            return false;
        }
        xvpr_vad_reset_vadFunc(handle);
        return true;
    }catch(std::exception ex)
    {
        Logger::Error(QString("XVPR - xvpr_vad_reset_vad exception - %1").arg(ex.what()));
        return false;
    }
}
