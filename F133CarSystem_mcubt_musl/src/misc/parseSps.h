#ifndef	PARSE_SPS_H
#define PARSE_SPS_H
typedef  unsigned int UINT;
typedef  unsigned char BYTE;
typedef  unsigned long DWORD;
bool h264_decode_sps(BYTE * buf,unsigned int nLen,int &width,int &height,int &fps);

#endif //PARSE_SPS_H