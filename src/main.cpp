#include "../include/beef_application.h"

#include <iostream>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)nCmdShow;
    (void)lpCmdLine;
    (void)hPrevInstance;

    bp::BeefApplication app; 
    app.Initialize((void *)&hInstance, ysContextObject::DeviceAPI::DirectX11); 
    app.Run();
    app.Destroy();

    return 0;
}
