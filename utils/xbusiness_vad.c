//#include "config.h"
#include "xbusiness_vpr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct xvpr_vad_info {
	size_t sample_rate;
	size_t total_sil_buffers;
	size_t total_voice_buffers;
	size_t time_out;	/* 超时秒数 */
	size_t max_length;	// 默认10秒
};


XVAD* xvpr_vad_create_vad(size_t sr, size_t max_length)
{
    struct xvpr_vad_info *vad = (struct xvpr_vad_info*)malloc(sizeof(struct xvpr_vad_info));
    vad->sample_rate = sr;
    vad->max_length = max_length;
    return (XVAD*)vad;
}


size_t xvpr_vad_detect_valid_speech(XVAD *handle, short *input, size_t length, short **output)
{
	static int const VOC_BLOCK_LEN = 800;
	static int const MIN_VOC_ENG = 350;

	int j=0, p=0;
	short old1, old2, old3, cur_sample;
	size_t out_length = 0;
	short *cur_ptr = NULL;

	short win_buf[VOC_BLOCK_LEN+1];
	size_t num_of_win, num_of_mod, i, k, eng;

	*output = (short *) malloc(sizeof(short) * length);
	cur_ptr = *output;

	num_of_win = length/VOC_BLOCK_LEN;
	num_of_mod = length%VOC_BLOCK_LEN;

	for (i=0; i<num_of_win; i++)
	{
		eng = 0;
		for (k=0; k<VOC_BLOCK_LEN; k++)
		{
			eng += abs(input[VOC_BLOCK_LEN*i+k]);
		}

		if (eng > MIN_VOC_ENG*VOC_BLOCK_LEN)
		{
			j = p = 0;
			old1 = old2 = old3 = 0;
			for(k=0;k<VOC_BLOCK_LEN;k++)
			{
				cur_sample = input[VOC_BLOCK_LEN*i+k];
				if(cur_sample==old1 && old1==old2 && old2==old3)
				{
					if(p>=0) j=p;
				}
				else
				{
					win_buf[j++]=cur_sample;
					p=j-3;
				}

				old3=old2;
				old2=old1;
				old1=cur_sample;
			}

			memcpy(cur_ptr,win_buf,sizeof(short)*j);
			cur_ptr += j;
			out_length += j;
		}
	}

	eng = 0;
	for (i=0; i<num_of_mod; i++)
	{
		eng += abs(input[VOC_BLOCK_LEN*num_of_win+i]);
	}
	if (eng > MIN_VOC_ENG*num_of_mod)
	{
		j = p = 0;
		old1 = old2 = old3 = 0;
		for(i=0;i<num_of_mod;i++)
		{
			cur_sample = input[VOC_BLOCK_LEN*num_of_win+i];
			if(cur_sample==old1 && old1==old2 && old2==old3)
			{
				if(p>=0) j=p;
			}
			else
			{
				win_buf[j++]=cur_sample;
				p=j-3;
			}

			old3=old2;
			old2=old1;
			old1=cur_sample;
		}

		memcpy(cur_ptr,win_buf,sizeof(short)*j);
		out_length += j;
	}

	return out_length;

}

static short get_buffer_mean(short *buffer, size_t buf_len) 
{
	int i;
	float sum = 0.0f;

	if (buffer == NULL || buf_len <= 0) {
		return 0;
	}
	
	for (i=0; i < buf_len; i++) {
        short n = buffer[i];
		sum += abs(buffer[i]);
	}

	return (short) (sum/buf_len);
}

static const short _voice_threshold = 350;
/* 判断说话停止所需的连续静音帧数 */
static const int _num_of_sil_buffers = 3;
/* 判断说话停止必须要满足的条件，即语音帧总数不少于6帧 */
static const int _num_of_total_buffers = 6;
int xvpr_vad_is_speaking(void *handle, short *stream, size_t buf_len)
{
	short mean = get_buffer_mean(stream, buf_len);	

	if (mean > _voice_threshold) {
        ((struct xvpr_vad_info *) handle)->total_voice_buffers += 1;
        ((struct xvpr_vad_info *) handle)->total_sil_buffers = 0;
	} else {
        ((struct xvpr_vad_info *) handle)->total_sil_buffers += 1;
	}

	if (((struct xvpr_vad_info *) handle)->total_sil_buffers > _num_of_sil_buffers 
		&& (((struct xvpr_vad_info *) handle)->total_sil_buffers+((struct xvpr_vad_info *) handle)->total_voice_buffers) > _num_of_total_buffers
		&& ((struct xvpr_vad_info *) handle)->total_voice_buffers > 0) {
		((struct xvpr_vad_info *) handle)->total_sil_buffers = 0;
		((struct xvpr_vad_info *) handle)->total_voice_buffers = 0;

		return false;
	} 

	if (((struct xvpr_vad_info *) handle)->total_voice_buffers > ((struct xvpr_vad_info *) handle)->max_length) {
		return false;
	}

	return true;
}

size_t xvpr_vad_get_valid_length(XVAD *handle)
{
	if (NULL == handle) return 0;
	return ((struct xvpr_vad_info *) handle)->total_voice_buffers;
}

/**
 * 重置端点检测引擎
 * @param handle 引擎句柄
 */
void xvpr_vad_reset_vad(XVAD *handle)
{
	((struct xvpr_vad_info *) handle)->total_sil_buffers = 0;
	((struct xvpr_vad_info *) handle)->total_voice_buffers = 0;
}
