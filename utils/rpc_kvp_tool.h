#ifndef RPC_KVP_TOOL_H
#define RPC_KVP_TOOL_H

#include <Windows.h>
#include <string>
#include "test_rpc.h"

class RPC_Kvp_Tool
{
public:
    static RPC_Kvp_Tool* GetInstance();
    void setKvpIp(const std::string &kvp_ip);
    void setKvpPort(int kvp_port);
    bool KvpServiceClient_Open(const char* ip,int port);
    bool KvpServiceClient_close();

    /// @brief 向总库中插入某个库节点
    bool KvpInsertNode(int32_t &ret, const char* node_name);
    /// @brief 从总库中删除某个库节点
    bool KvpDeleteNode(int32_t &ret, const char* node_name);
    /// @brief 将某说话人ID从某个库节点中删除。
    /// @param [in] vp_node 库节点名称。
    /// @param [in] vp_dir 库节点本地路径。
    /// @param [in] spk_id 说话人ID。
    bool KvpModelRemoveBySpkid(int32_t &ret, const char* vp_node, const char* vp_dir, const char* spk_id);
    /// @brief 注册说话人(二进制流格式)
    /// @param [in] utt 语音流。
    /// @param [in] vp_node 说话人待注册库节点名称。
    /// @param [in] vp_dir 声纹库路径。(--------该参数被废弃--------)
    /// @param [in] spk_id 说话人ID。(如果存在返回错误码102)
    /// @return Rpc_ModelInfo 说话人模型信息
    bool KvpRegisterSpeakerByStream(_Rpc_ModelInfo* &ret, int16_t* utt, int utt_size, const char* vp_node, const char* vp_dir, const char* spk_id);
    /// @brief 说话人辨认(二进制流格式)
    /// @param [in] utt 语音流。
    /// @param [in] node_list 库节点列表。
    /// @param [in] node_num 库节点数目。
    /// @param [in] top_n Top n数目。
    /// @param [in] utt_type 语音场景类型。
    /// @return Rpc_TopSpeakerInfo Top n得分信息
    bool KvpIdentifyTopSpeakerByStream(_Rpc_TopSpeakerInfo* &ret, int16_t* utt, int utt_size, const char** vp_node_arr, int vp_node_arr_size, int node_num, int top_n, int utt_type);
    /// @brief 获取机器指纹
    bool KvpGetFingerprint(char* fingerprint);
    /// @brief 获取授权状态
    bool KvpIsLicenceValid();
    /// @brief 获取授权信息
    bool KvpGetLicenceInfo(_Rpc_LicenceInfo* &ret);
    /// @brief 动态设置授权码
    bool KvpSetLicence(int32_t &ret, const char* licence);

    /// @brief 删除_Rpc_ModelInfo指针
    void Delete_Rpc_ModelInfo(_Rpc_ModelInfo* ptr);
    /// @brief 删除_Rpc_TopSpeakerInfo指针
    void Delete_Rpc_TopSpeakerInfo(_Rpc_TopSpeakerInfo* ptr);
    /// @brief 删除_Rpc_LicenceInfo指针
    void Delete_Rpc_LicenceInfo(_Rpc_LicenceInfo* ptr);
private:
    RPC_Kvp_Tool();
    ~RPC_Kvp_Tool();

private:
    static RPC_Kvp_Tool* m_instance;
    HINSTANCE m_rpc_dllHandle;
    bool m_is_open;
    std::string m_kvp_ip;
    int m_kvp_port;
};

#endif // RPC_KVP_TOOL_H
