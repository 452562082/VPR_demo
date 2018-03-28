/***************************************************************************
 *                    __    ____       __ ______  ______
 *  Project           \ \  / /\ \     / /|  __  \|  __  \	
					   \ \/ /  \ \   / / | | _| || |__| |
					   / /\ \   \ \_/ /  | ||___|| |  \ \
					  /_/  \_\   \___/   |_|     |_|   \_\

 * Copyright (C) 2013 - 2014, Chimy, <chimy@shengwenyun.cn>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

#ifndef _xbusiness_vpr_h_
#define _xbusiness_vpr_h_

typedef unsigned int size_t;
#ifdef __cplusplus
extern "C" {
#endif

//#define bool int	// define bool
#define false 0		// define false
#define true 1		// define true

#ifndef NULL		// define NULL
#define NULL 0
#endif

#ifndef INT16
typedef short INT16;
#endif

#ifndef INT8
typedef signed char INT8;
#endif


/* 声纹识别引擎句柄 */
typedef void XVPR;

/* 端点检测引擎句柄 */
typedef void XVAD;

/* 返回值类型 */
typedef int XVPR_CODE;

/* 函数返回值及说明 */
XVPR_CODE static XVPR_CODE_SUCCESS = 0x0000;		// 调用成功
XVPR_CODE static XVPR_CODE_FAIL = 0x0001;			// 调用失败
XVPR_CODE static XVPR_CODE_CONNECT_ERR = 0x0002;	// 网络连接错误
XVPR_CODE static XVPR_CODE_PARAM_ERR = 0x0003;		// 参数设置错误


/********************************* 端点检测引擎核心接口 ******************************************/


/**
 * 创建实时端点检测引擎
 * @param sr 采样率
 * @param max_length 最长有效时间
 * @return 引擎句柄
 */
XVAD* xvpr_vad_create_vad(size_t sr, size_t max_length);

/**
 * 去除静音
 * @param sr 采样率
 * @param input 输入语音流
 * @param length 输入语音流采样点个数
 * @param output 输出语音流
 * @return 返回有效语音采样点个数
 */
size_t xvpr_vad_detect_valid_speech(XVAD *handle, short *input, size_t length, short **output);

/**
 * 实时端点检测
 * @param handle 端点检测引擎句柄
 * @param buffer 实时语音包
 * @param buf_len 实时语音包采样点数
 * @return 0-还在说话，1-停止说话
 */
int xvpr_vad_is_speaking(void *handle, short *stream, size_t buf_len);

/**
 * 获得有效语音长度
 * @param handle 端点检测引擎句柄
 */
size_t xvpr_vad_get_valid_length(XVAD *handle);

/** 
 * 重置VAD引擎句柄
 */
void xvpr_vad_reset_vad(XVAD *handle);


/********************************* 引擎辅助接口 ******************************************/

/**
 * 从语音文件读出数据
 * @param path 语音文件路径
 * @param skip_bits 需要跳过头部的字节数
 * @param buffer 返回语音数据（内存由接口内部申请，并且需要开发者调用【释放内存】的接口进行释放）
 * @return 返回语音采样点个数
 */
size_t xvpr_aid_read_buffer(const char *path, size_t skip_bits/* 跳过头部多少字节 */, short **buffer);

/** 
 * 将语音数据写入文件
 * @param path 语音文件路径
 * @param buffer 语音数据缓冲
 * @param buf_len 语音采样点个数
 */
int xvpr_aid_write_buffer(const char *path, short *buffer/* 语音数据 */, size_t buf_len);

/**
 * 释放内存
 * @param buffer 待释放的内存指针
 */
void xvpr_aid_release_buffer(short **buffer);

#ifdef __cplusplus
}
#endif

#endif	// _xbusiness_vpr_h_
