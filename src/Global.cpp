#include "Global.hpp"
#include <string>
#include <vector>

void Global::showMessage(std::string message){
    std::wstring wideMessage(message.begin(), message.end());
    int bufferSize = WideCharToMultiByte(CP_ACP, 0, wideMessage.c_str(), -1, NULL, 0, NULL, NULL);
    std::vector<char> buffer(bufferSize);
    WideCharToMultiByte(CP_ACP, 0, wideMessage.c_str(), -1, buffer.data(), bufferSize, NULL, NULL);
    MessageBoxA(NULL, buffer.data(), "Error", MB_OK);
}

void Global::showMessage(const GLubyte* message){
    std::string strMessage(reinterpret_cast<const char*>(message));
    showMessage(strMessage);
}