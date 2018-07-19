#include <node.h>
#include <iostream>  
#include <string.h>  
#include <windows.h> 
#include <stdlib.h>
#include <vector>
#include <map>

using namespace std;

namespace usbDiskTool {

    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::String;
    using v8::Value;
    using v8::Array;

    BOOL IsUsbFlashDisk(LPSTR szDrive) {
        if (GetDriveType(szDrive) == DRIVE_REMOVABLE) {
            return true;
        } else {
            return false;
        }
    } 

    std::string stringToUTF8(const std::string & str) { 
        int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0); 
        wchar_t * pwBuf = new wchar_t[nwLen + 1];
        ZeroMemory(pwBuf, nwLen * 2 + 2); 
        ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen); 
        int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL); 
        char * pBuf = new char[nLen + 1]; 
        ZeroMemory(pBuf, nLen + 1); 
        ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL); 
        std::string retStr(pBuf); 
        delete []pwBuf; 
        delete []pBuf; 
        pwBuf = NULL; 
        pBuf  = NULL; 
        return retStr; 
    }  

    void getUsbFlashDisks(const FunctionCallbackInfo<Value>& args) { 
        Isolate* isolate = args.GetIsolate();
        DWORD dwLen = GetLogicalDriveStrings(0, NULL); 
        char* pszDriver = new char[dwLen];  
        GetLogicalDriveStrings(dwLen, pszDriver);   
        char* pDriver = pszDriver; 
        std::vector<std::pair<std::string,std::string>> targetsArr;
          
        while(*pDriver != '\0') {      
            if (IsUsbFlashDisk(TEXT(pDriver))) {
                // targetsArr.push_back(pDriver);
                CHAR szDriveName[MAX_PATH];  
                DWORD dwVolumeSerialNumber;  
                DWORD dwMaximumComponentLength;  
                DWORD dwFileSystemFlags;  
                CHAR szFileSystemNameBuffer[MAX_PATH];
                GetVolumeInformation(
                    pDriver,  
                    szDriveName,  
                    MAX_PATH,  
                    &dwVolumeSerialNumber,  
                    &dwMaximumComponentLength,  
                    &dwFileSystemFlags,  
                    szFileSystemNameBuffer,  
                    MAX_PATH  
                );
                targetsArr.push_back(std::make_pair<std::string,std::string>(std::string(pDriver), std::string(stringToUTF8(szDriveName))));
            } 
            pDriver += strlen(pDriver) + 1;
        } 
        delete[] pszDriver;
        Local<Array> array = Array::New(isolate,targetsArr.size());
        for(int i=0; i<targetsArr.size(); i++) {
            Local<v8::Object> obj=v8::Object::New(isolate);
             obj->Set(String::NewFromUtf8(isolate, "path"), String::NewFromUtf8(isolate,targetsArr[i].first.c_str()));
             obj->Set(String::NewFromUtf8(isolate, "name"), String::NewFromUtf8(isolate,targetsArr[i].second.c_str()));
            array->Set(i, obj);
        }
        args.GetReturnValue().Set(array);
    }

    void init(Local<Object> exports) {
        NODE_SET_METHOD(exports, "getUsbFlashDisks", getUsbFlashDisks);
    }

    NODE_MODULE(NODE_GYP_MODULE_NAME, init)
} 
