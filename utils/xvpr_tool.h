#ifndef XVPR_TOOL_H
#define XVPR_TOOL_H

#include <Windows.h>
#include <string>

/* 声纹识别引擎句柄 */
typedef void XVPR;

/* 端点检测引擎句柄 */
typedef void XVAD;

/* 返回值类型 */
typedef int XVPR_CODE;

class XVPR_Tool
{
public:
    static XVPR_Tool* GetInstance();

    /// @brief 创建实时端点检测引擎
    /// @param [IN] sr 采样率
    /// @param [OUT] xvad_handle 引擎句柄
    bool xvpr_vad_create_vad(size_t sr,size_t max_length,XVAD* &xvad_handle);

    /// @brief 去除杂音
    /// @param [IN] sr 采样率
    /// @param [IN] input 输入语音流
    /// @param [IN] length 输入语音流采样点个数
    /// @param [IN] output 输出语音流
    /// @param [OUT] valid_size 有效语音采样点个数
    bool xvpr_vad_detect_valid_speech(XVAD *handle,short *input,size_t length,short **output,size_t &valid_size);

    /// @brief 实时端点检测
    /// @param [IN] handle 端点检测引擎句柄
    /// @param [IN] buffer 实时语音包
    /// @param [IN] buf_len 实时语音包采样点数
    /// @param [OUT] status 0-还在说话 1-停止说话
    bool xvpr_vad_is_speaking(XVAD *handle,short *stream,size_t buf_len,int &status);

    /// @brief 重置VAD引擎句柄
    /// @param [IN] handle 引擎句柄
    bool xvpr_vad_reset_vad(XVAD *handle);

private:
    XVPR_Tool();
    ~XVPR_Tool();

private:
    static XVPR_Tool* m_instance;
    HINSTANCE m_xvpr_dllHandle;
};

#endif // XVPR_TOOL_H
