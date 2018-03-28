#ifndef RpcProxy_H
#define RpcProxy_H

#include <memory>
#include <string>
#include "test_rpc.h"

using namespace std;
class AsvRpcEngine;
class RpcProxy
{
public:
    static RpcProxy* GetInstance();
    _Rpc_UttInfo* KvpGetUttInfo(const string& wav_path);
    int32_t KvpModelRemoveBySpkid(const string& vp_node, const string& spk_id);
    _Rpc_ModelInfo* KvpRegisterSpeakerByFile(const string& utt, const string& vp_node, const string& spk_id, int32_t sp_chan);
    _Rpc_ScoreInfo* KvpVerifySpeakerByFile(const string& utt, const string& spk_id, const string& vp_node, int32_t utt_type, int32_t sp_chan);
    _Rpc_ScoreInfo* KvpTempVerifySpeakerByFile(const string& utt1, int32_t sp_chan1, int32_t utt_type1, const string& utt2, int32_t sp_chan2, int32_t utt_type2);
    _Rpc_TopSpeakerInfo* KvpIdentifyTopSpeakerByFile(const string& utt, const vector<string> vp_node_arr, int32_t node_num, int32_t top_n, int32_t utt_type, int32_t sp_chan);
    int32_t KvpIvectorLoadByFile(const string& vp_node, const string& iv_file);
    int32_t KvpInsertNode(const string& vp_node);
    int32_t KvpDeleteNode(const string& vp_node);
    int32_t KvpMoveNode(const string& spk_id, const string& origin, const string& target);
    string KvpGetFingerprint();
    _Rpc_LicenceInfo* KvpGetLicenceInfo();
    int32_t KvpSetLicence(const string& licence);
    bool KvpIsLicenceValid();
    _Rpc_ModelInfo* KvpRegisterSpeakerByStream(const std::vector<int16_t>& utt, const int32_t samp_rate, const string& vp_node, const string& spk_id);
    _Rpc_TopSpeakerInfo* KvpIdentifyTopSpeakerByStream(const std::vector<int16_t>& utt, const int32_t samp_rate, const std::vector<std::string>& node_list, int node_num, int top_n, int utt_type);
    _Rpc_ScoreInfo* KvpVerifySpeakerByStream(const std::vector<int16_t>& utt, const int32_t samp_rate, const std::string& spk_id, const std::string& vp_node, const int32_t utt_type);
    _Rpc_ScoreInfo* KvpTempVerifySpeakerByStream(const std::vector<int16_t>& utt1, const int32_t samp_rate_1, int32_t utt_type1, std::vector<int16_t> utt2, const int32_t samp_rate_2, int32_t utt_type2);
    void Delete_Asv_Type(_Asv_Type type,void* ptr);

private:
    RpcProxy();
    ~RpcProxy();

private:
    static RpcProxy* m_instance;
    std::shared_ptr<AsvRpcEngine> m_engine;
};

#endif // RpcProxy_H
