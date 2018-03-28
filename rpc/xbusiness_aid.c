//#include "config.h"
#include "xbusiness_vpr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t xvpr_aid_read_buffer(const char *path, size_t skip_bits, short **buffer) 
{
	unsigned long buf_len = 0L;
	FILE *f = fopen(path, "rb");
	if (NULL == f) { return -1; }
	
	fseek(f, 0L, SEEK_END);
	buf_len = (ftell(f) - skip_bits)/2;
	fseek(f, 0L, SEEK_SET);
	fseek(f, skip_bits, SEEK_CUR);

	*buffer = (short *) malloc(sizeof(short) * buf_len);
	fread(*buffer, sizeof(short), buf_len, f);	
	fclose(f);

	return buf_len;
}

int xvpr_aid_write_buffer(const char *path, 
							 short *buffer,			/* 语音数据 */
							 size_t buf_len)	/* 语音采样点数 */
{
	FILE *f = fopen(path, "wb");
	if (NULL == f) { return -1; }

	fwrite(buffer, sizeof(short), buf_len, f);
	fclose(f);

	return 0;
}

void xvpr_aid_release_buffer(short **buffer) 
{
	if (NULL != *buffer) free(*buffer);
}
