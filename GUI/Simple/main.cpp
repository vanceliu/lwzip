#include <QApplication>
#include "SimpleWindow.h"
#include "EncodingManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("LWZip");
    app.setApplicationVersion("0.1.0");
    app.setOrganizationName("LWZip");

    EncodingManager encodingManager;

    SimpleWindow window;
    window.show();

    return app.exec();
}
