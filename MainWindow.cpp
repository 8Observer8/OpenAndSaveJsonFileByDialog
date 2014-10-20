#include <QString>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QMessageBox>
#include <QByteArray>
#include <QDebug>
#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr( "Open File" ),
                QString(),
                tr( "Json Files (*.json)" ) );

    if ( fileName.isEmpty() ) {
        return;
    }

    // Open the file
    QFile file( fileName );
    if ( !file.open( QIODevice::ReadOnly ) ) {
        QMessageBox::critical(
                    this, tr( "Error" ),
                    tr( "Error: unable to open the file \"%1\"" ).arg( fileName ) );
        return;
    }

    // Read the file
    QByteArray data = file.readAll();

    // Create Json document
    QJsonDocument document;
    document = document.fromJson( data );

    // Get the text from the Json document
    QString text = document.object()["Text"].toString();
    ui->textEdit->setText( text );
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr( "Save File" ),
                QString(),
                tr( "Json Files (*.json)" ) );

    if ( fileName.isEmpty() ) {
        return;
    }

    // Get text
    QString text = ui->textEdit->toPlainText();

    // Create a Json object from the text
    QJsonObject json;
    json["Text"] = text;

    // Create a Json document
    QJsonDocument document;
    document.setObject( json );

    // Write the Json document to a file
    // Open a file
    QFile file( fileName );
    if ( !file.open( QIODevice::WriteOnly ) ) {
        QMessageBox::critical(
                    this, tr( "Error" ),
                    tr( "Error: unable to open the file \"%1\"" ).arg( fileName ) );
        return;
    }

    // Write to the file
    if ( !file.write( document.toJson() ) ) {
        QMessageBox::critical(
                    this, tr( "Error" ),
                    tr( "Error: unable to write to the file \"%1\"" ).arg( fileName ) );
        return;
    }
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}
