#include "rpc_kvp_tool.h"
#include <functional>
#include "utils/logger.h"

namespace {
    const std::wstring rpc_dll_path = L"./libs/test_rpc.dll";
//    const char* kvp_ip = "192.168.1.16";
//    const int kvp_port = 9191;
}
RPC_Kvp_Tool* RPC_Kvp_Tool::m_instance = nullptr;
RPC_Kvp_Tool::RPC_Kvp_Tool():
  m_is_open(false)
{
    m_rpc_dllHandle = nullptr;
    m_rpc_dllHandle = LoadLibrary(rpc_dll_path.c_str());
    if(!m_rpc_dllHandle){
        Logger::Error("RPC - rpc_dll load failed");
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

void RPC_Kvp_Tool::setKvpIp(const std::string &kvp_ip)
{
    m_kvp_ip = kvp_ip;
}

void RPC_Kvp_Tool::setKvpPort(int kvp_port)
{
    m_kvp_port = kvp_port;
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
            Logger::Error("RPC - KvpServiceClient_Open 函数获取失败");
            return false;
        }
        KvpServiceClient_OpenFunc(ip, port);
        Logger::Info("RPC - KvpServiceClient_Open success.");
        m_is_open = true;
        return true;
    }catch(std::exception ex)
    {
        Logger::Error(QString("RPC - KvpServiceClient_Open exception - ").arg(ex.what()));
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
            Logger::Error("RPC - KvpServiceClient_close 函数获取失败");
            return false;
        }
        KvpServiceClient_closeFunc();
        Logger::Info("RPC - KvpServiceClient_close success.");
        m_is_open = false;
        return true;
    }catch(std::exception ex)
    {
        Logger::Error(QString("RPC - KvpServiceClient_close exception - ").arg(ex.what()));
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
            if(!KvpServiceClient_Open(m_kvp_ip.c_str(),m_kvp_port))
                return false;
        }

        typedef int32_t (*KvpInsertNode)(const char* node_name);
        KvpInsertNode KvpInsertNodeFunc = (KvpInsertNode)GetProcAddress(m_rpc_dllHandle, "KvpInsertNode");
        if(!KvpInsertNodeFunc){
            Logger::Error("RPC - KvpInsertNode 函数获取失败");
            return false;
        }
        ret = KvpInsertNodeFunc(node_name);
        if(!ret){
            Logger::Info("RPC - KvpInsertNode success.");
        }else{
            Logger::Error("RPC - KvpInsertNode failed.");
        }
        return true;
    }catch(std::exception ex)
    {
        Logger::Error(QString("RPC - KvpInsertNode exception - ").arg(ex.what()));
        return false;
    }
}

bool RPC_Kvp_Tool::KvpDeleteNode(int32_t &ret, const char* node_name)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(m_kvp_ip.c_str(),m_kvp_port))
                return false;
        }

        typedef int32_t (*KvpDeleteNode)(const char* node_name);
        KvpDeleteNode KvpDeleteNodeFunc = (KvpDeleteNode)GetProcAddress(m_rpc_dllHandle, "KvpDeleteNode");
        if(!KvpDeleteNodeFunc){
            Logger::Error("RPC - KvpDeleteNode 函数获取失败");
            return false;
        }
        ret = KvpDeleteNodeFunc(node_name);
        if(ret == 0){
            Logger::Info("RPC - KvpDeleteNode success.");
        }else{
            Logger::Error("RPC - KvpDeleteNode failed.");
        }
        return true;
    }catch(std::exception ex)
    {
        Logger::Error(QString("RPC - KvpDeleteNode exception - ").arg(ex.what()));
        return false;
    }
}

bool RPC_Kvp_Tool::KvpModelRemoveBySpkid(int32_t &ret, const char* vp_node, const char* vp_dir, const char* spk_id)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(m_kvp_ip.c_str(),m_kvp_port))
                return false;
        }

        typedef int32_t (*KvpModelRemoveBySpkid)(const char* vp_node, const char* vp_dir, const char* spk_id);
        KvpModelRemoveBySpkid KvpModelRemoveBySpkidFunc = (KvpModelRemoveBySpkid)GetProcAddress(m_rpc_dllHandle, "KvpModelRemoveBySpkid");
        if(!KvpModelRemoveBySpkidFunc){
            Logger::Error("RPC - KvpModelRemoveBySpkid 函数获取失败");
            return false;
        }
        ret = KvpModelRemoveBySpkidFunc(vp_node,vp_dir,spk_id);
        if(ret == 0){
            Logger::Info("RPC - KvpModelRemoveBySpkid success.");
        }else{
            Logger::Error("RPC - KvpModelRemoveBySpkid failed.");
        }
        return true;
    }catch(std::exception ex)
    {
        Logger::Error(QString("RPC - KvpModelRemoveBySpkid exception - ").arg(ex.what()));
        return false;
    }
}

bool RPC_Kvp_Tool::KvpRegisterSpeakerByStream(_Rpc_ModelInfo* &ret, int16_t* utt, int utt_size, const char* vp_node, const char* vp_dir, const char* spk_id)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(m_kvp_ip.c_str(),m_kvp_port))
                return false;
        }

        typedef _Rpc_ModelInfo* (*KvpRegisterSpeakerByStream)(int16_t* utt, int utt_size, const char* vp_node, const char* vp_dir, const char* spk_id);
        KvpRegisterSpeakerByStream KvpRegisterSpeakerByStreamFunc = (KvpRegisterSpeakerByStream)GetProcAddress(m_rpc_dllHandle, "KvpRegisterSpeakerByStream");
        if(!KvpRegisterSpeakerByStreamFunc){
            Logger::Error("RPC - KvpRegisterSpeakerByStream 函数获取失败");
            return false;
        }
        ret = KvpRegisterSpeakerByStreamFunc(utt,utt_size,vp_node,vp_dir,spk_id);
        if(ret == 0){
            Logger::Info("RPC - KvpRegisterSpeakerByStream success.");
        }else{
            Logger::Error("RPC - KvpRegisterSpeakerByStream failed.");
        }
        return true;
    }catch(std::exception ex)
    {
        Logger::Error(QString("RPC - KvpRegisterSpeakerByStream exception - ").arg(ex.what()));
        return false;
    }
}

void RPC_Kvp_Tool::Delete_Rpc_ModelInfo(_Rpc_ModelInfo* ptr)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(m_kvp_ip.c_str(),m_kvp_port))
                return;
        }

        typedef void (*Delete_Rpc_ModelInfo)(_Rpc_ModelInfo *ptr);
        Delete_Rpc_ModelInfo Delete_Rpc_ModelInfoFunc = (Delete_Rpc_ModelInfo)GetProcAddress(m_rpc_dllHandle, "Delete_Rpc_ModelInfo");
        if(!Delete_Rpc_ModelInfoFunc){
            Logger::Error("RPC - Delete_Rpc_ModelInfo 函数获取失败");
            return;
        }
        Delete_Rpc_ModelInfoFunc(ptr);
        Logger::Info("RPC - Delete_Rpc_ModelInfo success.");
    }catch(std::exception ex)
    {
        Logger::Error(QString("RPC - Delete_Rpc_ModelInfo exception - ").arg(ex.what()));
    }
}

bool RPC_Kvp_Tool::KvpIdentifyTopSpeakerByStream(_Rpc_TopSpeakerInfo* &ret, int16_t* utt, int utt_size, const char** vp_node_arr, int vp_node_arr_size, int node_num, int top_n, int utt_type)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(m_kvp_ip.c_str(),m_kvp_port))
                return false;
        }

        typedef _Rpc_TopSpeakerInfo* (*KvpIdentifyTopSpeakerByStream)(int16_t* utt, int utt_size, const char** vp_node_arr, int vp_node_arr_size, int node_num, int top_n, int utt_type);
        KvpIdentifyTopSpeakerByStream KvpIdentifyTopSpeakerByStreamFunc = (KvpIdentifyTopSpeakerByStream)GetProcAddress(m_rpc_dllHandle, "KvpIdentifyTopSpeakerByStream");
        if(!KvpIdentifyTopSpeakerByStreamFunc){
            Logger::Error("RPC - KvpIdentifyTopSpeakerByStream 函数获取失败");
            return false;
        }
        ret = KvpIdentifyTopSpeakerByStreamFunc(utt,utt_size,vp_node_arr,vp_node_arr_size,node_num,top_n,utt_type);
        if(ret == 0){
            Logger::Info("RPC - KvpIdentifyTopSpeakerByStream success.");
        }else{
            Logger::Error("RPC - KvpIdentifyTopSpeakerByStream failed.");
        }
        return true;
    }catch(std::exception ex)
    {
        Logger::Error(QString("RPC - KvpIdentifyTopSpeakerByStream exception - ").arg(ex.what()));
        return false;
    }
}

void RPC_Kvp_Tool::Delete_Rpc_TopSpeakerInfo(_Rpc_TopSpeakerInfo* ptr)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(m_kvp_ip.c_str(),m_kvp_port))
                return;
        }

        typedef void (*Delete_Rpc_TopSpeakerInfo)(_Rpc_TopSpeakerInfo *ptr);
        Delete_Rpc_TopSpeakerInfo Delete_Rpc_TopSpeakerInfoFunc = (Delete_Rpc_TopSpeakerInfo)GetProcAddress(m_rpc_dllHandle, "Delete_Rpc_TopSpeakerInfo");
        if(!Delete_Rpc_TopSpeakerInfoFunc){
            Logger::Error("RPC - Delete_Rpc_TopSpeakerInfo 函数获取失败");
            return;
        }
        Delete_Rpc_TopSpeakerInfoFunc(ptr);
        Logger::Info("RPC - Delete_Rpc_TopSpeakerInfo success.");
    }catch(std::exception ex)
    {
        Logger::Error(QString("RPC - Delete_Rpc_TopSpeakerInfo exception - ").arg(ex.what()));
    }
}

bool RPC_Kvp_Tool::KvpGetFingerprint(char* fingerprint)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(m_kvp_ip.c_str(),m_kvp_port))
                return false;
        }

        typedef const char* (*KvpGetFingerprint)();
        KvpGetFingerprint KvpGetFingerprintFunc = (KvpGetFingerprint)GetProcAddress(m_rpc_dllHandle, "KvpGetFingerprint");
        if(!KvpGetFingerprintFunc){
            Logger::Error("RPC - KvpGetFingerprint 函数获取失败");
            return false;
        }
        const char* str = KvpGetFingerprintFunc();
        strcpy(fingerprint,str);
        Logger::Info("RPC - KvpGetFingerprint success.");
        return true;
    }catch(std::exception ex)
    {
        Logger::Error(QString("RPC - KvpGetFingerprint exception - ").arg(ex.what()));
        return false;
    }
}

bool RPC_Kvp_Tool::KvpIsLicenceValid()
{
    bool flag = false;
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(m_kvp_ip.c_str(),m_kvp_port))
                return false;
        }

        typedef bool (*KvpIsLicenceValid)();
        KvpIsLicenceValid KvpIsLicenceValidFunc = (KvpIsLicenceValid)GetProcAddress(m_rpc_dllHandle, "KvpIsLicenceValid");
        if(!KvpIsLicenceValidFunc){
            Logger::Error("RPC - KvpIsLicenceValid 函数获取失败");
            return false;
        }
        flag = KvpIsLicenceValidFunc();
        Logger::Info("RPC - KvpIsLicenceValid success.");
    }catch(std::exception ex)
    {
        Logger::Error(QString("RPC - KvpIsLicenceValid exception - ").arg(ex.what()));
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
            if(!KvpServiceClient_Open(m_kvp_ip.c_str(),m_kvp_port))
                return false;
        }

        typedef _Rpc_LicenceInfo* (*KvpGetLicenceInfo)();
        KvpGetLicenceInfo KvpGetLicenceInfoFunc = (KvpGetLicenceInfo)GetProcAddress(m_rpc_dllHandle, "KvpGetLicenceInfo");
        if(!KvpGetLicenceInfoFunc){
            Logger::Error("RPC - KvpGetLicenceInfo 函数获取失败");
            return false;
        }
        ret = KvpGetLicenceInfoFunc();
        Logger::Info("RPC - KvpGetLicenceInfo success.");
        return true;
    }catch(std::exception ex)
    {
        Logger::Error(QString("RPC - KvpGetLicenceInfo exception - ").arg(ex.what()));
        return false;
    }
}

void RPC_Kvp_Tool::Delete_Rpc_LicenceInfo(_Rpc_LicenceInfo* ptr)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(m_kvp_ip.c_str(),m_kvp_port))
                return;
        }

        typedef void (*Delete_Rpc_LicenceInfo)(_Rpc_LicenceInfo *ptr);
        Delete_Rpc_LicenceInfo Delete_Rpc_LicenceInfoFunc = (Delete_Rpc_LicenceInfo)GetProcAddress(m_rpc_dllHandle, "Delete_Rpc_LicenceInfo");
        if(!Delete_Rpc_LicenceInfoFunc){
            Logger::Error("RPC - Delete_Rpc_LicenceInfo 函数获取失败");
            return;
        }
        Delete_Rpc_LicenceInfoFunc(ptr);
        Logger::Info("RPC - Delete_Rpc_LicenceInfo success.");
    }catch(std::exception ex)
    {
        Logger::Error(QString("RPC - Delete_Rpc_LicenceInfo exception - ").arg(ex.what()));
    }
}

bool RPC_Kvp_Tool::KvpSetLicence(int32_t &ret,const char* licence)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(m_kvp_ip.c_str(),m_kvp_port))
                return false;
        }

        typedef int32_t (*KvpSetLicence)(const char* licence);
        KvpSetLicence KvpSetLicenceFunc = (KvpSetLicence)GetProcAddress(m_rpc_dllHandle, "KvpSetLicence");
        if(!KvpSetLicenceFunc){
            Logger::Error("RPC - KvpSetLicence 函数获取失败");
            return false;
        }
        ret = KvpSetLicenceFunc(licence);
        if(ret == 0){
            Logger::Info("RPC - KvpSetLicence success.");
        }else{
            Logger::Error("RPC - KvpSetLicence failed.");
        }
        return true;
    }catch(std::exception ex)
    {
        Logger::Error(QString("RPC - KvpSetLicence exception - ").arg(ex.what()));
        return false;
    }
}
