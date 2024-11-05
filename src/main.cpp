#pragma once
#include "jWindow.h"
#include "jApp.h"
#include <iostream>

int main(){
    HUAR::App app;

    try
    {
        app.Run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;  
    }
    
    return EXIT_SUCCESS;
}
    

