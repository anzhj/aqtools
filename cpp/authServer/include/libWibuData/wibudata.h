#ifndef __LIBWIBUDATA_H__
#define __LIBWIBUDATA_H__

extern "C"
{
	const char * ReadData(int* nCharLen, unsigned long ulFirmCode, unsigned long ulProductCode);
}

#endif //__LIBWIBUDATA_H__
