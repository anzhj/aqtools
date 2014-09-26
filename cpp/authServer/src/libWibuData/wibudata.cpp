#include "CodeMeter.h"
#include "wibudata.h"
#include <string>
#include <string.h>

static std::string strEP;

extern "C"
{

const char * ReadData(int* nCharLen, unsigned long ulFirmCode, unsigned long ulProductCode)
{
        // Create an "Entry Access"
        CMACCESS cmacc;
        memset(&cmacc, 0, sizeof(cmacc));
        cmacc.mflCtrl = CM_ACCESS_FORCE | CM_ACCESS_NOUSERLIMIT;
        cmacc.mulFirmCode = ulFirmCode;
        cmacc.mulProductCode = ulProductCode;
        //cmacc.mcmBoxInfo = mpabCmBoxInfo[nBoxIndex];
        HCMSysEntry hcmse = CmAccess(CM_ACCESS_LOCAL, &cmacc);
        if (0 ==hcmse) {
                return "Error";
        } // if

        CMBOXENTRY2 cmboxentry2;
        memset(&cmboxentry2, 0, sizeof(CMBOXENTRY2));
        if (0 == CmGetInfo(hcmse, CM_GEI_ENTRYINFO2, &cmboxentry2, sizeof(CMBOXENTRY2))) {
                return "Error";
        }

        int iReturn = CmGetInfo(hcmse, CM_GEI_ENTRYDATA, NULL, 0);
        int iCount = iReturn / sizeof(CMENTRYDATA);
        CMENTRYDATA *cmentrydata = new CMENTRYDATA[iCount];
        iReturn = CmGetInfo(hcmse, CM_GEI_ENTRYDATA, cmentrydata, iCount*sizeof(CMENTRYDATA));

        /*
        查看内部测试的数据内容
        string temp0 = (char*)cmentrydata[0].mabData;
        string temp1 = (char*)cmentrydata[1].mabData;
        string temp2 = (char*)cmentrydata[2].mabData;
        string temp3 = (char*)cmentrydata[3].mabData;
        string temp4 = (char*)cmentrydata[4].mabData;
        */

        //正常情况的话直接取第一部分的内容
        strEP =(char*)cmentrydata[0].mabData;

        //升级之后，由于license quantity也占用了保护数据第128区块内容导致结构变化。
        //先检测取出的第四个部分的内容是不是由cmnw开头的，如果是的话就把第三部分的数据取出来放在这个返回的变量中
        std::string temp = (char*)cmentrydata[3].mabData;
        std::string strtemp=temp.substr(0,4);
        if(strtemp == "cmnw"){
                strEP =(char*)cmentrydata[2].mabData;
        }

        delete [] cmentrydata;
        CmRelease(hcmse);

        *nCharLen = strEP.length();

        return strEP.c_str();
}

}
