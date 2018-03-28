#include "rpcproxy.h"
#include "configparser.h"
#include "rpc/asv_rpc.h"

namespace {
    const int interval = 1000;
}

RpcProxy* RpcProxy::m_instance = nullptr;
RpcProxy::RpcProxy()
{
    m_engine = std::make_shared<AsvRpcEngine>(ConfigParser::GetInstance()->GetRPC_Ip().toStdString(),ConfigParser::GetInstance()->GetRPC_Port(),interval);
}

RpcProxy::~RpcProxy()
{
    if(RpcProxy::m_instance != nullptr){
        delete RpcProxy::m_instance;
        RpcProxy::m_instance = nullptr;
    }
}

RpcProxy* RpcProxy::GetInstance()
{
    if(m_instance == nullptr){
        m_instance = new RpcProxy();
    }
    return m_instance;
}

_Rpc_UttInfo* RpcProxy::KvpGetUttInfo(const string& wav_path)
{
    return m_engine.get()->KvpGetUttInfo(wav_path);
}

int32_t RpcProxy::KvpModelRemoveBySpkid(const string& vp_node, const string& spk_id)
{
    return m_engine.get()->KvpModelRemoveBySpkid(vp_node,spk_id);
}

_Rpc_ModelInfo* RpcProxy::KvpRegisterSpeakerByFile(const string& utt, const string& vp_node, const string& spk_id, int32_t sp_chan)
{
    return m_engine.get()->KvpRegisterSpeakerByFile(utt,vp_node,spk_id,sp_chan);
}

_Rpc_ScoreInfo* RpcProxy::KvpVerifySpeakerByFile(const string& utt, const string& spk_id, const string& vp_node, int32_t utt_type, int32_t sp_chan)
{
    return m_engine.get()->KvpVerifySpeakerByFile(utt,spk_id,vp_node,utt_type,sp_chan);
}

_Rpc_ScoreInfo* RpcProxy::KvpTempVerifySpeakerByFile(const string& utt1, int32_t sp_chan1, int32_t utt_type1, const string& utt2, int32_t sp_chan2, int32_t utt_type2)
{
    return m_engine.get()->KvpTempVerifySpeakerByFile(utt1,sp_chan1,utt_type1,utt2,sp_chan2,utt_type2);
}

_Rpc_TopSpeakerInfo* RpcProxy::KvpIdentifyTopSpeakerByFile(const string& utt, const vector<string> vp_node_arr, int32_t node_num, int32_t top_n, int32_t utt_type, int32_t sp_chan)
{
    return m_engine.get()->KvpIdentifyTopSpeakerByFile(utt,vp_node_arr,node_num,top_n,utt_type,sp_chan);
}

int32_t RpcProxy::KvpIvectorLoadByFile(const string& vp_node, const string& iv_file)
{
    return m_engine.get()->KvpIvectorLoadByFile(vp_node,iv_file);
}

int32_t RpcProxy::KvpInsertNode(const string& vp_node)
{
    return m_engine.get()->KvpInsertNode(vp_node);
}

int32_t RpcProxy::KvpDeleteNode(const string& vp_node)
{
    return m_engine.get()->KvpDeleteNode(vp_node);
}

int32_t RpcProxy::KvpMoveNode(const string& spk_id, const string& origin, const string& target)
{
    return m_engine.get()->KvpMoveNode(spk_id,origin,target);
}

string RpcProxy::KvpGetFingerprint()
{
    return m_engine.get()->KvpGetFingerprint();
}

_Rpc_LicenceInfo* RpcProxy::KvpGetLicenceInfo()
{
    return m_engine.get()->KvpGetLicenceInfo();
}

int32_t RpcProxy::KvpSetLicence(const string& licence)
{
    return m_engine.get()->KvpSetLicence(licence);
}

bool RpcProxy::KvpIsLicenceValid()
{
    return m_engine.get()->KvpIsLicenceValid();
}

_Rpc_ModelInfo* RpcProxy::KvpRegisterSpeakerByStream(const std::vector<int16_t>& utt, const int32_t samp_rate, const string& vp_node, const string& spk_id)
{
    return m_engine.get()->KvpRegisterSpeakerByStream(utt,samp_rate,vp_node,spk_id);
}

_Rpc_TopSpeakerInfo* RpcProxy::KvpIdentifyTopSpeakerByStream(const std::vector<int16_t>& utt, const int32_t samp_rate, const std::vector<std::string>& node_list, int node_num, int top_n, int utt_type)
{
    return m_engine.get()->KvpIdentifyTopSpeakerByStream(utt,samp_rate,node_list,node_num,top_n,utt_type);
}

_Rpc_ScoreInfo* RpcProxy::KvpVerifySpeakerByStream(const std::vector<int16_t>& utt, const int32_t samp_rate, const std::string& spk_id, const std::string& vp_node, const int32_t utt_type)
{
    return m_engine.get()->KvpVerifySpeakerByStream(utt,samp_rate,spk_id,vp_node,utt_type);
}

_Rpc_ScoreInfo* RpcProxy::KvpTempVerifySpeakerByStream(const std::vector<int16_t>& utt1, const int32_t samp_rate_1, int32_t utt_type1, std::vector<int16_t> utt2, const int32_t samp_rate_2, int32_t utt_type2)
{
    return m_engine.get()->KvpTempVerifySpeakerByStream(utt1,samp_rate_1,utt_type1,utt2,samp_rate_2,utt_type2);
}

void RpcProxy::Delete_Asv_Type(_Asv_Type type,void* ptr)
{
    return m_engine.get()->Delete_Asv_Type(type,ptr);
}
