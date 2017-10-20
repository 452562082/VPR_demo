#include "rpc_kvp_tool.h"
#include <QDebug>
#include <functional>

namespace {
    const std::wstring rpc_dll_path = L"./libs/test_rpc.dll";
    const char* kvp_ip = "192.168.1.16";
    const int kvp_port = 9191;
}
RPC_Kvp_Tool* RPC_Kvp_Tool::m_instance = nullptr;
RPC_Kvp_Tool::RPC_Kvp_Tool():
  m_is_open(false)
{
    m_rpc_dllHandle = nullptr;
    m_rpc_dllHandle = LoadLibrary(rpc_dll_path.c_str());
    if(!m_rpc_dllHandle){
        qDebug() << "Error: rpc_dll load failed";
    }
}

RPC_Kvp_Tool::~RPC_Kvp_Tool()
{
    if(m_is_open){
        KvpServiceClient_close();
    }
    if(m_rpc_dllHandle != nullptr) {
        FreeLibrary(m_rpc_dllHandle);
        m_rpc_dllHandle = nullptr;
    }
    if (RPC_Kvp_Tool::m_instance != nullptr) {
        delete RPC_Kvp_Tool::m_instance;
        RPC_Kvp_Tool::m_instance = nullptr;
    }
}

RPC_Kvp_Tool* RPC_Kvp_Tool::GetInstance()
{
    if(m_instance == nullptr){
        m_instance = new RPC_Kvp_Tool();
    }
    return m_instance;
}

bool RPC_Kvp_Tool::KvpServiceClient_Open(const char* ip,int port)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        typedef void* (*KvpServiceClient_Open)(const char* ip,int port);
        KvpServiceClient_Open KvpServiceClient_OpenFunc = (KvpServiceClient_Open)GetProcAddress(m_rpc_dllHandle, "KvpServiceClient_Open");
        if(!KvpServiceClient_OpenFunc){
            qDebug() << "Error: KvpServiceClient_Open 函数获取失败";
            return false;
        }
        KvpServiceClient_OpenFunc(ip, port);
        m_is_open = true;
        return true;
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
        return false;
    }
}

bool RPC_Kvp_Tool::KvpServiceClient_close()
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        typedef void* (*KvpServiceClient_close)();
        KvpServiceClient_close KvpServiceClient_closeFunc = (KvpServiceClient_close)GetProcAddress(m_rpc_dllHandle, "KvpServiceClient_close");
        if(!KvpServiceClient_closeFunc){
            qDebug() << "Error: KvpServiceClient_close 函数获取失败";
            return false;
        }
        KvpServiceClient_closeFunc();
        m_is_open = false;
        return true;
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
        return false;
    }
}

bool RPC_Kvp_Tool::KvpInsertNode(int32_t &ret, const char* node_name)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(kvp_ip,kvp_port))
                return false;
        }

        typedef int32_t (*KvpInsertNode)(const char* node_name);
        KvpInsertNode KvpInsertNodeFunc = (KvpInsertNode)GetProcAddress(m_rpc_dllHandle, "KvpInsertNode");
        if(!KvpInsertNodeFunc){
            qDebug() << "Error: KvpInsertNode 函数获取失败";
            return false;
        }
        ret = KvpInsertNodeFunc(node_name);
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
    return true;
}

bool RPC_Kvp_Tool::KvpDeleteNode(int32_t &ret, const char* node_name)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(kvp_ip,kvp_port))
                return false;
        }

        typedef int32_t (*KvpDeleteNode)(const char* node_name);
        KvpDeleteNode KvpDeleteNodeFunc = (KvpDeleteNode)GetProcAddress(m_rpc_dllHandle, "KvpDeleteNode");
        if(!KvpDeleteNodeFunc){
            qDebug() << "Error: KvpDeleteNode 函数获取失败";
            return false;
        }
        ret = KvpDeleteNodeFunc(node_name);
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
    return true;
}

bool RPC_Kvp_Tool::KvpModelRemoveBySpkid(int32_t &ret, const char* vp_node, const char* vp_dir, const char* spk_id)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(kvp_ip,kvp_port))
                return false;
        }

        typedef int32_t (*KvpModelRemoveBySpkid)(const char* vp_node, const char* vp_dir, const char* spk_id);
        KvpModelRemoveBySpkid KvpModelRemoveBySpkidFunc = (KvpModelRemoveBySpkid)GetProcAddress(m_rpc_dllHandle, "KvpModelRemoveBySpkid");
        if(!KvpModelRemoveBySpkidFunc){
            qDebug() << "Error: KvpModelRemoveBySpkid 函数获取失败";
            return false;
        }
        ret = KvpModelRemoveBySpkidFunc(vp_node,vp_dir,spk_id);
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
    return true;
}

bool RPC_Kvp_Tool::KvpRegisterSpeakerByStream(_Rpc_ModelInfo* &ret, int16_t* utt, int utt_size, const char* vp_node, const char* vp_dir, const char* spk_id)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(kvp_ip,kvp_port))
                return false;
        }

        typedef _Rpc_ModelInfo* (*KvpRegisterSpeakerByStream)(int16_t* utt, int utt_size, const char* vp_node, const char* vp_dir, const char* spk_id);
        KvpRegisterSpeakerByStream KvpRegisterSpeakerByStreamFunc = (KvpRegisterSpeakerByStream)GetProcAddress(m_rpc_dllHandle, "KvpRegisterSpeakerByStream");
        if(!KvpRegisterSpeakerByStreamFunc){
            qDebug() << "Error: KvpRegisterSpeakerByStream 函数获取失败";
            return false;
        }
        ret = KvpRegisterSpeakerByStreamFunc(utt,utt_size,vp_node,vp_dir,spk_id);
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
    return true;
}

void RPC_Kvp_Tool::Delete_Rpc_ModelInfo(_Rpc_ModelInfo* ptr)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(kvp_ip,kvp_port))
                return;
        }

        typedef void (*Delete_Rpc_ModelInfo)(_Rpc_ModelInfo *ptr);
        Delete_Rpc_ModelInfo Delete_Rpc_ModelInfoFunc = (Delete_Rpc_ModelInfo)GetProcAddress(m_rpc_dllHandle, "Delete_Rpc_ModelInfo");
        if(!Delete_Rpc_ModelInfoFunc){
            qDebug() << "Error: Delete_Rpc_ModelInfo 函数获取失败";
            return;
        }
        Delete_Rpc_ModelInfoFunc(ptr);
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
}

bool RPC_Kvp_Tool::KvpIdentifyTopSpeakerByStream(_Rpc_TopSpeakerInfo* &ret, int16_t* utt, int utt_size, const char** vp_node_arr, int vp_node_arr_size, int node_num, int top_n, int utt_type)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(kvp_ip,kvp_port))
                return false;
        }

        typedef _Rpc_TopSpeakerInfo* (*KvpIdentifyTopSpeakerByStream)(int16_t* utt, int utt_size, const char** vp_node_arr, int vp_node_arr_size, int node_num, int top_n, int utt_type);
        KvpIdentifyTopSpeakerByStream KvpIdentifyTopSpeakerByStreamFunc = (KvpIdentifyTopSpeakerByStream)GetProcAddress(m_rpc_dllHandle, "KvpIdentifyTopSpeakerByStream");
        if(!KvpIdentifyTopSpeakerByStreamFunc){
            qDebug() << "Error: KvpIdentifyTopSpeakerByStream 函数获取失败";
            return false;
        }
        ret = KvpIdentifyTopSpeakerByStreamFunc(utt,utt_size,vp_node_arr,vp_node_arr_size,node_num,top_n,utt_type);
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
    return true;
}

void RPC_Kvp_Tool::Delete_Rpc_TopSpeakerInfo(_Rpc_TopSpeakerInfo* ptr)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(kvp_ip,kvp_port))
                return;
        }

        typedef void (*Delete_Rpc_TopSpeakerInfo)(_Rpc_TopSpeakerInfo *ptr);
        Delete_Rpc_TopSpeakerInfo Delete_Rpc_TopSpeakerInfoFunc = (Delete_Rpc_TopSpeakerInfo)GetProcAddress(m_rpc_dllHandle, "Delete_Rpc_TopSpeakerInfo");
        if(!Delete_Rpc_TopSpeakerInfoFunc){
            qDebug() << "Error: Delete_Rpc_TopSpeakerInfo 函数获取失败";
            return;
        }
        Delete_Rpc_TopSpeakerInfoFunc(ptr);
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
}

bool RPC_Kvp_Tool::KvpGetFingerprint(char* fingerprint)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(kvp_ip,kvp_port))
                return false;
        }

        typedef const char* (*KvpGetFingerprint)();
        KvpGetFingerprint KvpGetFingerprintFunc = (KvpGetFingerprint)GetProcAddress(m_rpc_dllHandle, "KvpGetFingerprint");
        if(!KvpGetFingerprintFunc){
            qDebug() << "Error: KvpGetFingerprint 函数获取失败";
            return false;
        }
        const char* str = KvpGetFingerprintFunc();
        strcpy(fingerprint,str);
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
    return true;
}

bool RPC_Kvp_Tool::KvpIsLicenceValid()
{
    bool flag = false;
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(kvp_ip,kvp_port))
                return false;
        }

        typedef bool (*KvpIsLicenceValid)();
        KvpIsLicenceValid KvpIsLicenceValidFunc = (KvpIsLicenceValid)GetProcAddress(m_rpc_dllHandle, "KvpIsLicenceValid");
        if(!KvpIsLicenceValidFunc){
            qDebug() << "Error: KvpIsLicenceValid 函数获取失败";
            return false;
        }
        flag = KvpIsLicenceValidFunc();
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
    return flag;
}

bool RPC_Kvp_Tool::KvpGetLicenceInfo(_Rpc_LicenceInfo* &ret)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(kvp_ip,kvp_port))
                return false;
        }

        typedef _Rpc_LicenceInfo* (*KvpGetLicenceInfo)();
        KvpGetLicenceInfo KvpGetLicenceInfoFunc = (KvpGetLicenceInfo)GetProcAddress(m_rpc_dllHandle, "KvpGetLicenceInfo");
        if(!KvpGetLicenceInfoFunc){
            qDebug() << "Error: KvpGetLicenceInfo 函数获取失败";
            return false;
        }
        ret = KvpGetLicenceInfoFunc();
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
    return true;
}

void RPC_Kvp_Tool::Delete_Rpc_LicenceInfo(_Rpc_LicenceInfo* ptr)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(kvp_ip,kvp_port))
                return;
        }

        typedef void (*Delete_Rpc_LicenceInfo)(_Rpc_LicenceInfo *ptr);
        Delete_Rpc_LicenceInfo Delete_Rpc_LicenceInfoFunc = (Delete_Rpc_LicenceInfo)GetProcAddress(m_rpc_dllHandle, "Delete_Rpc_LicenceInfo");
        if(!Delete_Rpc_LicenceInfoFunc){
            qDebug() << "Error: Delete_Rpc_LicenceInfo 函数获取失败";
            return;
        }
        Delete_Rpc_LicenceInfoFunc(ptr);
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
}

int32_t RPC_Kvp_Tool::KvpSetLicence(const char* licence)
{
    bool flag = false;
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(kvp_ip,kvp_port))
                return false;
        }

        typedef int32_t (*KvpSetLicence)(const char* licence);
        KvpSetLicence KvpSetLicenceFunc = (KvpSetLicence)GetProcAddress(m_rpc_dllHandle, "KvpSetLicence");
        if(!KvpSetLicenceFunc){
            qDebug() << "Error: KvpSetLicence 函数获取失败";
            return false;
        }
        flag = KvpSetLicenceFunc(licence);
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
    return flag;
}
