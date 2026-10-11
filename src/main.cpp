#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QDebug>

#include "ui/mainwindow.h"
#include "utils/logger.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Face Recognition System");
    app.setApplicationVersion("1.0.0");
    app.setApplicationAuthor("Professional Development");

    Logger::info("Application started");

    // Initialize SQLite
    if (!QSqlDatabase::contains("faces_db")) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "faces_db");
        db.setDatabaseName("faces.db");
        if (!db.open()) {
            Logger::error("Failed to open database");
            QMessageBox::critical(nullptr, "Database Error", 
                                  "Cannot open faces.db database.");
            return 1;
        }
        Logger::info("Database connected successfully");
    }

    MainWindow window;
    window.setWindowTitle("Face Recognition System v1.0.0");
    window.setGeometry(100, 100, 1200, 800);
    window.show();

    Logger::info("Main window displayed");

    return app.exec();
}
