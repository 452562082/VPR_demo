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
	/// @param[in] ip IP��ַ
	/// @param[in] port �˿ں�
	/// @param[in] interval �������ʱ����(ms)
	AsvRpcEngine(const std::string& ip, int port, int interval);
	~AsvRpcEngine();
	/// @brief ���������ļ���Ϣ
	/// @param[in] wavpath �����ļ�·��
	/// @return _Rpc_UttInfo ����������Ϣ
	_Rpc_UttInfo* KvpGetUttInfo(const string& wav_path);

	/// @brief ��ĳ˵����ID��ĳ����ڵ���ɾ��
	/// @param [in] vp_node ��ڵ�����
	/// @param [in] vp_dir ��ڵ㱾��·��
	/// @param [in] spk_id ˵����ID
	/// @return KVP_CODE
	int32_t KvpModelRemoveBySpkid(const string& vp_node, const string& spk_id);

	/// @brief ע��˵����
	/// @param[in] utt ����·��
	/// @param[in] vp_node ˵���˴�ע���ڵ�����
	/// @param[in] spk_id ˵����ID
	/// @param[in] sp_chan ָ����������
	/// @return _Rpc_ModelInfo ����˵����ģ����Ϣ
	_Rpc_ModelInfo* KvpRegisterSpeakerByFile(const string& utt, const string& vp_node, const string& spk_id, int32_t sp_chan);

	/// @brief 1:1��֤
	/// @param[in] utt ����·��
	/// @param[in] spk_id ˵����ID
	/// @param[in] vp_node ˵�������ڿ�ڵ�����
	/// @param[in] utt_type ������������
	/// @param[in] sp_chan ָ����������
	/// @return _Rpc_ScoreInfo 1:1��֤��Ϣ
	_Rpc_ScoreInfo* KvpVerifySpeakerByFile(const string& utt, const string& spk_id, const string& vp_node, int32_t utt_type, int32_t sp_chan);

	/// @brief 1:1��֤(����2���������бȽ�)
	/// @param[in] utt1 ��1������·��
	/// @param[in] sp_chan1 ָ����1�������ڵ�
	/// @param[in] utt_type1 ָ����1��������������
	/// @param[in] utt2 ��2������·��
	/// @param[in] sp_chan2 ָ����2����������
	/// @param[in] utt_type2 ָ����2��������������
	/// @return _Rpc_ScoreInfo ��֤�÷���Ϣ
	_Rpc_ScoreInfo* KvpTempVerifySpeakerByFile(const string& utt1, int32_t sp_chan1, int32_t utt_type1, const string& utt2, int32_t sp_chan2, int32_t utt_type2);

	/// @brief ˵���˱���(1:N)
	/// @param[in] utt �����ļ�·��
	/// @param[in] node_list ��ڵ��б�
	/// @param[in] node_num ��ڵ���Ŀ
	/// @param[in] top_n Top n��Ŀ
	/// @param[in] utt_type ������������
	/// @param[in] sp_chan ָ������������0����������1��������
	/// @return _Rpc_TopSpeakerInfo Top n�÷���Ϣ
	_Rpc_TopSpeakerInfo* KvpIdentifyTopSpeakerByFile(const string& utt, const vector<string> vp_node_arr, int32_t node_num, int32_t top_n, int32_t utt_type, int32_t sp_chan);
	
	/// @brief ��˵����ģ�����ļ�����ʽ���ص��ڴ�ĳ��ڵ���
	/// @param[in] vp_node ��ڵ�����
	/// @param[in] iv_file ˵����ģ���ļ�·��
	/// @return KVP_CODE
	int32_t KvpIvectorLoadByFile(const string& vp_node, const string& iv_file);

	/// @brief ���ܿ��в���ĳ����ڵ�
	/// @param[in] vp_node �ڵ�����
	/// @return KVP_CODE
	int32_t KvpInsertNode(const string& vp_node);

	/// @brief ���ܿ���ɾ��ĳ����ڵ�
	/// @param[in] vp_node ��ڵ�����
	/// @return KVP_CODE
	int32_t KvpDeleteNode(const string& vp_node);

	/// @brief ��˵����ģ�ʹ�һ�����Ƶ���һ���⡣
    /// @param spk_id ˵����ID��
    /// @param origin ԭʼ�⡣
    /// @param target Ŀ��⡣
    /// @return KVP_CODE
	int32_t KvpMoveNode(const string& spk_id, const string& origin, const string& target);

	/// @brief ��ȡ����ָ��
	/// @return ������ָ���ַ���
	string KvpGetFingerprint();

	/// @brief ��ȡ��Ȩ��Ϣ
	/// @return _Rpc_LicenceInfo ��Ȩ��Ϣ
	_Rpc_LicenceInfo* KvpGetLicenceInfo();

	/// @brief ������Ȩ��Ϣ
	/// @note ����Ѿ���ʼ���ɹ�����ֱ�ӷ���KVP_CODE_SUCCEED��������г�ʼ��
	/// @param[in] licence ��Ȩ�ַ���
	/// @return KVP_CODE
	int32_t KvpSetLicence(const string& licence);

	/// @brief �ж�Licence�Ƿ���Ч
	/// @return ��Ȩ��־
	bool KvpIsLicenceValid();

	/// @brief ע��˵����(����������ʽ)
	/// @param [in] utt ��������
	/// @param [in] samp_rate �������ݲ�����
	/// @param [in] vp_node ˵���˴�ע���ڵ����ơ�
	/// @param [in] spk_id ˵����ID��(������ڷ��ش�����102)
	/// @return _Rpc_ModelInfo ˵����ģ����Ϣ
	_Rpc_ModelInfo* KvpRegisterSpeakerByStream(const std::vector<int16_t>& utt, const int32_t samp_rate, const string& vp_node, const string& spk_id);

	/// @brief ˵���˱���(����������ʽ)
	/// @param [in] utt ��������
	/// @param [in] samp_rate �������ݲ����ʡ�
	/// @param [in] node_list ��ڵ��б�
	/// @param [in] node_num ��ڵ���Ŀ��
	/// @param [in] top_n Top n��Ŀ��
	/// @param [in] utt_type �����������͡�
	/// @return _Rpc_TopSpeakerInfo Top n�÷���Ϣ
	_Rpc_TopSpeakerInfo* KvpIdentifyTopSpeakerByStream(const std::vector<int16_t>& utt, const int32_t samp_rate, const std::vector<std::string>& node_list, int node_num, int top_n, int utt_type);

	/// ˵����ȷ�ϣ�����������ʽ����
	/// @param [in] utt ��������
	/// @param [in] samp_rate �������ݲ����ʡ�
	/// @param [in] spk_id ˵����ID��
	/// @param [in] vp_node ��ڵ㡣
	/// @param [in] utt_type �����������͡�
	/// @return Rpc_ScoreInfo �÷���Ϣ 
	_Rpc_ScoreInfo* KvpVerifySpeakerByStream(const std::vector<int16_t>& utt, const int32_t samp_rate, const std::string& spk_id, const std::string& vp_node, const int32_t utt_type);

	/// @brief 1:1��֤(����2���������бȽϣ�����������ʽ)
	/// @param[in] utt1 ��1��������
	/// @param[in] utt1 samp_rate_1 �������ݲ�����
	/// @param[in] utt_type1 ָ����1��������������
	/// @param[in] utt2 ��2��������
	/// @param [in] utt2 samp_rate_2 �������ݲ�����
	/// @param[in] utt_type2 ָ����2��������������
	/// @return _Rpc_ScoreInfo ��֤�÷���Ϣ
	_Rpc_ScoreInfo* KvpTempVerifySpeakerByStream(const std::vector<int16_t>& utt1, const int32_t samp_rate_1, int32_t utt_type1, std::vector<int16_t> utt2, const int32_t samp_rate_2, int32_t utt_type2);
 
	/// @brief ɾ��ģ���ڲ����ݽṹ
	/// @param[in] _Asv_Type ���ݽṹ����
	/// @param[in] ptr ���ݽṹָ��
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
	//����
	void reset();
	//�������
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
