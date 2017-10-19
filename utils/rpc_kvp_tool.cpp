#include "rpc_kvp_tool.h"
#include <QDebug>
#include <functional>

namespace {
    const std::wstring rpc_dll_path = L"./libs/test_rpc.dll";
    const char* kvp_ip = "192.168.1.16";
    const int kvp_port = 9191;
}
RPC_Kvp_Tool* RPC_Kvp_Tool::m_instance = nullptr;
RPC_Kvp_Tool::RPC_Kvp_Tool():m_transport_ptr(nullptr),
  m_kvp_client_ptr(nullptr),
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
    deleteTransport();
    deleteKvpServiceClient();
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
    if(!createTransport(ip, port)) {
        qDebug() << "Error: createTransport failed";
        return false;
    }

    if(!createKvpServiceClient()){
        qDebug() << "Error: createKvpServiceClient failed";
        return false;
    }

    transportOpen();
    m_is_open = true;
    return true;
}

void RPC_Kvp_Tool::KvpServiceClient_close()
{
    transportClose();
    m_is_open = false;
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

        typedef int32_t (*KvpInsertNode)(void* client_ptr, const char* node_name);
        KvpInsertNode KvpInsertNodeFunc = (KvpInsertNode)GetProcAddress(m_rpc_dllHandle, "KvpInsertNode");
        if(!KvpInsertNodeFunc){
            qDebug() << "Error: KvpInsertNode 函数获取失败";
            return false;
        }
        ret = KvpInsertNodeFunc(m_kvp_client_ptr,node_name);
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

        typedef int32_t (*KvpDeleteNode)(void* client_ptr, const char* node_name);
        KvpDeleteNode KvpDeleteNodeFunc = (KvpDeleteNode)GetProcAddress(m_rpc_dllHandle, "KvpDeleteNode");
        if(!KvpDeleteNodeFunc){
            qDebug() << "Error: KvpDeleteNode 函数获取失败";
            return false;
        }
        ret = KvpDeleteNodeFunc(m_kvp_client_ptr,node_name);
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

        typedef int32_t (*KvpModelRemoveBySpkid)(void* client_ptr, const char* vp_node, const char* vp_dir, const char* spk_id);
        KvpModelRemoveBySpkid KvpModelRemoveBySpkidFunc = (KvpModelRemoveBySpkid)GetProcAddress(m_rpc_dllHandle, "KvpModelRemoveBySpkid");
        if(!KvpModelRemoveBySpkidFunc){
            qDebug() << "Error: KvpModelRemoveBySpkid 函数获取失败";
            return false;
        }
        ret = KvpModelRemoveBySpkidFunc(m_kvp_client_ptr,vp_node,vp_dir,spk_id);
        return true;
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
}

bool RPC_Kvp_Tool::KvpRegisterSpeakerByStream(_Rpc_ModelInfo* ret, int16_t* utt, int utt_size, const char* vp_node, const char* vp_dir, const char* spk_id)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(kvp_ip,kvp_port))
                return false;
        }

        typedef void (*KvpRegisterSpeakerByStream)(void* client_ptr, _Rpc_ModelInfo* ret, int16_t* utt, int utt_size, const char* vp_node, const char* vp_dir, const char* spk_id);
        KvpRegisterSpeakerByStream KvpRegisterSpeakerByStreamFunc = (KvpRegisterSpeakerByStream)GetProcAddress(m_rpc_dllHandle, "KvpRegisterSpeakerByStream");
        if(!KvpRegisterSpeakerByStreamFunc){
            qDebug() << "Error: KvpRegisterSpeakerByStream 函数获取失败";
            return false;
        }
        KvpRegisterSpeakerByStreamFunc(m_kvp_client_ptr,ret,utt,utt_size,vp_node,vp_dir,spk_id);
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
    return true;
}

bool RPC_Kvp_Tool::KvpIdentifyTopSpeakerByStream(_Rpc_TopSpeakerInfo* ret, int16_t* utt, int utt_size, const char** vp_node_arr, int vp_node_arr_size, int node_num, int top_n, int utt_type)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(kvp_ip,kvp_port))
                return false;
        }

        typedef void (*KvpIdentifyTopSpeakerByStream)(void* client_ptr, _Rpc_TopSpeakerInfo* ret, int16_t* utt, int utt_size, const char** vp_node_arr, int vp_node_arr_size, int node_num, int top_n, int utt_type);
        KvpIdentifyTopSpeakerByStream KvpIdentifyTopSpeakerByStreamFunc = (KvpIdentifyTopSpeakerByStream)GetProcAddress(m_rpc_dllHandle, "KvpIdentifyTopSpeakerByStream");
        if(!KvpIdentifyTopSpeakerByStreamFunc){
            qDebug() << "Error: KvpIdentifyTopSpeakerByStream 函数获取失败";
            return false;
        }
        KvpIdentifyTopSpeakerByStreamFunc(m_kvp_client_ptr,ret,utt,utt_size,vp_node_arr,vp_node_arr_size,node_num,top_n,utt_type);
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
    return true;
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

        typedef void (*KvpGetFingerprint)(void* client_ptr, char* fingerprint);
        KvpGetFingerprint KvpGetFingerprintFunc = (KvpGetFingerprint)GetProcAddress(m_rpc_dllHandle, "KvpGetFingerprint");
        if(!KvpGetFingerprintFunc){
            qDebug() << "Error: KvpGetFingerprint 函数获取失败";
            return false;
        }
        KvpGetFingerprintFunc(m_kvp_client_ptr,fingerprint);
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

        typedef bool (*KvpIsLicenceValid)(void* client_ptr);
        KvpIsLicenceValid KvpIsLicenceValidFunc = (KvpIsLicenceValid)GetProcAddress(m_rpc_dllHandle, "KvpIsLicenceValid");
        if(!KvpIsLicenceValidFunc){
            qDebug() << "Error: KvpIsLicenceValid 函数获取失败";
            return false;
        }
        flag = KvpIsLicenceValidFunc(m_kvp_client_ptr);
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
    return flag;
}

bool RPC_Kvp_Tool::KvpGetLicenceInfo(_Rpc_LicenceInfo* ret)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        if(!m_is_open){
            if(!KvpServiceClient_Open(kvp_ip,kvp_port))
                return false;
        }

        typedef void (*KvpGetLicenceInfo)(void *client_ptr, _Rpc_LicenceInfo* licence);
        KvpGetLicenceInfo KvpGetLicenceInfoFunc = (KvpGetLicenceInfo)GetProcAddress(m_rpc_dllHandle, "KvpGetLicenceInfo");
        if(!KvpGetLicenceInfoFunc){
            qDebug() << "Error: KvpGetLicenceInfo 函数获取失败";
            return false;
        }
        KvpGetLicenceInfoFunc(m_kvp_client_ptr, ret);
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
    return true;
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

        typedef int32_t (*KvpSetLicence)(void *client_ptr, const char* licence);
        KvpSetLicence KvpSetLicenceFunc = (KvpSetLicence)GetProcAddress(m_rpc_dllHandle, "KvpSetLicence");
        if(!KvpSetLicenceFunc){
            qDebug() << "Error: KvpSetLicence 函数获取失败";
            return false;
        }
        flag = KvpSetLicenceFunc(m_kvp_client_ptr, licence);
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
    return flag;
}

bool RPC_Kvp_Tool::createTransport(const char* ip,int port)
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        typedef void* (*CreateTransport)(const char* ip,int port);
        CreateTransport CreateTransportFunc = (CreateTransport)GetProcAddress(m_rpc_dllHandle, "CreateTransport");
        if(!CreateTransportFunc){
            qDebug() << "Error: CreateTransport 函数获取失败";
            return false;
        }
        m_transport_ptr = CreateTransportFunc(ip, port);
        return true;
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
        return false;
    }
}

bool RPC_Kvp_Tool::createKvpServiceClient()
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return false;
        }

        typedef void* (*CreateKvpServiceClient)(void* transport_ptr);
        CreateKvpServiceClient CreateKvpServiceClientFunc = (CreateKvpServiceClient)GetProcAddress(m_rpc_dllHandle, "CreateKvpServiceClient");
        if(!CreateKvpServiceClientFunc){
            qDebug() << "Error: CreateKvpServiceClient 函数获取失败";
            return false;
        }
        m_kvp_client_ptr = CreateKvpServiceClientFunc(m_transport_ptr);
        return true;
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
        return false;
    }
}

void RPC_Kvp_Tool::transportOpen()
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return;
        }

        typedef void (*TransportOpen)(void* transport_ptr);
        TransportOpen TransportOpenFunc = (TransportOpen)GetProcAddress(m_rpc_dllHandle, "TransportOpen");
        if(!TransportOpenFunc){
            qDebug() << "Error: TransportOpen 函数获取失败";
            return;
        }
        TransportOpenFunc(m_transport_ptr);
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
}

void RPC_Kvp_Tool::transportClose()
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return;
        }

        typedef void (*TransportClose)(void* transport_ptr);
        TransportClose TransportCloseFunc = (TransportClose)GetProcAddress(m_rpc_dllHandle, "TransportClose");
        if(!TransportCloseFunc){
            qDebug() << "Error: TransportClose 函数获取失败";
            return;
        }
        TransportCloseFunc(m_transport_ptr);
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
    }
}

bool RPC_Kvp_Tool::deleteKvpServiceClient()
{ 
    try{
        if(m_rpc_dllHandle == nullptr) {
            return true;
        }

        typedef void (*DeleteKvpServiceClient)(void* client_ptr);
        DeleteKvpServiceClient DeleteKvpServiceClientFunc = (DeleteKvpServiceClient)GetProcAddress(m_rpc_dllHandle, "DeleteKvpServiceClient");
        if(!DeleteKvpServiceClientFunc){
            qDebug() << "Error: DeleteKvpServiceClient 函数获取失败";
            return false;
        }
        DeleteKvpServiceClientFunc(m_kvp_client_ptr);
        return true;
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
        return false;
    }
}

bool RPC_Kvp_Tool::deleteTransport()
{
    try{
        if(m_rpc_dllHandle == nullptr) {
            return true;
        }

        typedef void (*DeleteTransport)(void* client_ptr);
        DeleteTransport DeleteTransportFunc = (DeleteTransport)GetProcAddress(m_rpc_dllHandle, "DeleteTransport");
        if(!DeleteTransportFunc){
            qDebug() << "Error: DeleteTransport 函数获取失败";
            return false;
        }
        DeleteTransportFunc(m_transport_ptr);
        return true;
    }catch(std::exception ex)
    {
        qDebug() << "Error:" << ex.what();
        return false;
    }
}
