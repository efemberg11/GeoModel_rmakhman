#include "fsl_mainwindow.h"
#include <iostream>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    FSLMainWindow w;
    
    
    if(argc>1 && std::string(argv[1]) == "-c")
    {
    w.show();
    w.load_configuration_CL(std::string(argv[2]));
    }
    
    else if(argc>1 && std::string(argv[1]) != "-c")
    {
    std::cout << std::endl;
    std::cerr << "Usage:" << std::endl;
    std::cerr << "fsl" << std::endl;
    std::cerr << "fsl -c /path/to/ConfigFile" << std::endl;
    std::cout << std::endl;
    exit(-1);
    }
    
    else
    {
    w.show();
    }
    
    return a.exec();
}
