#ifndef FILEBAS64CALCULATOR_HPP
#define FILEBAS64CALCULATOR_HPP

#include <QWidget>
#include <QThread>

class FileBase64Calculator : public QThread {
        Q_OBJECT

    public:
        FileBase64Calculator( QWidget* parent, QString fileName );
        virtual ~FileBase64Calculator();

    protected:
        void run() override;

    private:
        const QString file_name;

    signals:
        void completed( QByteArray base64 );
};

#endif // FILEBAS64CALCULATOR_HPP
