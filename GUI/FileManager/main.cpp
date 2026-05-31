#include <QApplication>
#include "FileManagerWindow.h"
#include "EncodingManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("LWZip File Manager");
    app.setApplicationVersion("0.1.0");
    app.setOrganizationName("LWZip");

    EncodingManager encodingManager;

    FileManagerWindow window;
    window.show();

    return app.exec();
}
