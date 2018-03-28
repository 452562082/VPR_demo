#pragma once
#ifdef ASV_RPC_DLL
#define ASV_RPC_API __declspec(dllexport)
#else
#define ASV_RPC_API __declspec(dllimport)
#endif

#include <iostream>
#include "test_rpc.h"

typedef void *HANDLE;

using namespace std;
class ASV_RPC_API AsvRpcEngine
{
public:
	/// @param[in] ip IP地址
	/// @param[in] port 端口号
	/// @param[in] interval 心跳检测时间间隔(ms)
	AsvRpcEngine(const std::string& ip, int port, int interval);
	~AsvRpcEngine();
	/// @brief 解析语音文件信息
	/// @param[in] wavpath 语音文件路径
	/// @return _Rpc_UttInfo 返回语音信息
	_Rpc_UttInfo* KvpGetUttInfo(const string& wav_path);

	/// @brief 将某说话人ID从某个库节点中删除
	/// @param [in] vp_node 库节点名称
	/// @param [in] vp_dir 库节点本地路径
	/// @param [in] spk_id 说话人ID
	/// @return KVP_CODE
	int32_t KvpModelRemoveBySpkid(const string& vp_node, const string& spk_id);

	/// @brief 注册说话人
	/// @param[in] utt 语音路径
	/// @param[in] vp_node 说话人待注册库节点名称
	/// @param[in] spk_id 说话人ID
	/// @param[in] sp_chan 指定处理声道
	/// @return _Rpc_ModelInfo 返回说话人模型信息
	_Rpc_ModelInfo* KvpRegisterSpeakerByFile(const string& utt, const string& vp_node, const string& spk_id, int32_t sp_chan);

	/// @brief 1:1验证
	/// @param[in] utt 语音路径
	/// @param[in] spk_id 说话人ID
	/// @param[in] vp_node 说话人所在库节点名称
	/// @param[in] utt_type 语音场景类型
	/// @param[in] sp_chan 指定处理声道
	/// @return _Rpc_ScoreInfo 1:1验证信息
	_Rpc_ScoreInfo* KvpVerifySpeakerByFile(const string& utt, const string& spk_id, const string& vp_node, int32_t utt_type, int32_t sp_chan);

	/// @brief 1:1验证(给定2段语音进行比较)
	/// @param[in] utt1 第1段语音路径
	/// @param[in] sp_chan1 指定第1段语音节道
	/// @param[in] utt_type1 指定第1段语音场景类型
	/// @param[in] utt2 第2段语音路径
	/// @param[in] sp_chan2 指定第2段语音声道
	/// @param[in] utt_type2 指定第2段语音场景类型
	/// @return _Rpc_ScoreInfo 验证得分信息
	_Rpc_ScoreInfo* KvpTempVerifySpeakerByFile(const string& utt1, int32_t sp_chan1, int32_t utt_type1, const string& utt2, int32_t sp_chan2, int32_t utt_type2);

	/// @brief 说话人辨认(1:N)
	/// @param[in] utt 语音文件路径
	/// @param[in] node_list 库节点列表
	/// @param[in] node_num 库节点数目
	/// @param[in] top_n Top n数目
	/// @param[in] utt_type 语音场景类型
	/// @param[in] sp_chan 指定处理声道，0：左声道，1：右声道
	/// @return _Rpc_TopSpeakerInfo Top n得分信息
	_Rpc_TopSpeakerInfo* KvpIdentifyTopSpeakerByFile(const string& utt, const vector<string> vp_node_arr, int32_t node_num, int32_t top_n, int32_t utt_type, int32_t sp_chan);
	
	/// @brief 将说话人模型以文件的形式加载到内存某库节点中
	/// @param[in] vp_node 库节点名称
	/// @param[in] iv_file 说话人模型文件路径
	/// @return KVP_CODE
	int32_t KvpIvectorLoadByFile(const string& vp_node, const string& iv_file);

	/// @brief 向总库中插入某个库节点
	/// @param[in] vp_node 节点名称
	/// @return KVP_CODE
	int32_t KvpInsertNode(const string& vp_node);

	/// @brief 从总库中删除某个库节点
	/// @param[in] vp_node 库节点名称
	/// @return KVP_CODE
	int32_t KvpDeleteNode(const string& vp_node);

	/// @brief 将说话人模型从一个库移到另一个库。
    /// @param spk_id 说话人ID。
    /// @param origin 原始库。
    /// @param target 目标库。
    /// @return KVP_CODE
	int32_t KvpMoveNode(const string& spk_id, const string& origin, const string& target);

	/// @brief 获取机器指纹
	/// @return 机器人指纹字符串
	string KvpGetFingerprint();

	/// @brief 获取授权信息
	/// @return _Rpc_LicenceInfo 授权信息
	_Rpc_LicenceInfo* KvpGetLicenceInfo();

	/// @brief 设置授权信息
	/// @note 如果已经初始化成功，则直接返回KVP_CODE_SUCCEED，否则进行初始化
	/// @param[in] licence 授权字符串
	/// @return KVP_CODE
	int32_t KvpSetLicence(const string& licence);

	/// @brief 判断Licence是否有效
	/// @return 授权标志
	bool KvpIsLicenceValid();

	/// @brief 注册说话人(二进制流格式)
	/// @param [in] utt 语音流。
	/// @param [in] samp_rate 语音数据采样率
	/// @param [in] vp_node 说话人待注册库节点名称。
	/// @param [in] spk_id 说话人ID。(如果存在返回错误码102)
	/// @return _Rpc_ModelInfo 说话人模型信息
	_Rpc_ModelInfo* KvpRegisterSpeakerByStream(const std::vector<int16_t>& utt, const int32_t samp_rate, const string& vp_node, const string& spk_id);

	/// @brief 说话人辨认(二进制流格式)
	/// @param [in] utt 语音流。
	/// @param [in] samp_rate 语音数据采样率。
	/// @param [in] node_list 库节点列表。
	/// @param [in] node_num 库节点数目。
	/// @param [in] top_n Top n数目。
	/// @param [in] utt_type 语音场景类型。
	/// @return _Rpc_TopSpeakerInfo Top n得分信息
	_Rpc_TopSpeakerInfo* KvpIdentifyTopSpeakerByStream(const std::vector<int16_t>& utt, const int32_t samp_rate, const std::vector<std::string>& node_list, int node_num, int top_n, int utt_type);

	/// 说话人确认（二进制流格式）。
	/// @param [in] utt 语音流。
	/// @param [in] samp_rate 语音数据采样率。
	/// @param [in] spk_id 说话人ID。
	/// @param [in] vp_node 库节点。
	/// @param [in] utt_type 语音场景类型。
	/// @return Rpc_ScoreInfo 得分信息 
	_Rpc_ScoreInfo* KvpVerifySpeakerByStream(const std::vector<int16_t>& utt, const int32_t samp_rate, const std::string& spk_id, const std::string& vp_node, const int32_t utt_type);

	/// @brief 1:1验证(给定2段语音进行比较，二进制流格式)
	/// @param[in] utt1 第1段语音流
	/// @param[in] utt1 samp_rate_1 语音数据采样率
	/// @param[in] utt_type1 指定第1段语音场景类型
	/// @param[in] utt2 第2段语音流
	/// @param [in] utt2 samp_rate_2 语音数据采样率
	/// @param[in] utt_type2 指定第2段语音场景类型
	/// @return _Rpc_ScoreInfo 验证得分信息
	_Rpc_ScoreInfo* KvpTempVerifySpeakerByStream(const std::vector<int16_t>& utt1, const int32_t samp_rate_1, int32_t utt_type1, std::vector<int16_t> utt2, const int32_t samp_rate_2, int32_t utt_type2);
 
	/// @brief 删除模块内部数据结构
	/// @param[in] _Asv_Type 数据结构类型
	/// @param[in] ptr 数据结构指针
	/* enum _Asv_Type {
	 *	_RPC_UttInfo,
	 *	_RPC_ModelInfo,
	 *	_RPC_ScoreInfo,
	 *	_RPC_SpeakerScore,
	 *	_RPC_TopSpeakerInfo,
	 *	_RPC_LicenceInfo
	 *};
	 */
	void Delete_Asv_Type(_Asv_Type type,void* ptr);

	string getIp();
	int getPort();
	int getInterval();
	bool IsConnect();
	//重连
	void reset();
	//心跳检测
	string KvpPing();
private:
	void connect();
	void close();

private:
	std::string m_ip;
	int m_port;
	int m_interval;
	void* m_socket_ptr;
	void* m_transport_ptr;
	void* m_client_ptr;
	bool m_isConnect;
	HANDLE m_mutex;
	HANDLE m_run_thread;
};

class ClientNilException : public std::exception
{
public:
	const char* what() const throw() {
		std::cout << "client is null." << std::endl;
		return NULL;
	}
};
