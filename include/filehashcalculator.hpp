#ifndef FILEHASHCALCULATOR_H
#define FILEHASHCALCULATOR_H

#include <QWidget>
#include <QThread>

class FileHashCalculator : public QThread {
        Q_OBJECT

    public:
        FileHashCalculator( QWidget* parent, QString fileName );
        virtual ~FileHashCalculator();

    protected:
        void run() override;

    private:
        const QString file_name;

    signals:
        void newHashString( int index, QByteArray hash );
        void newChecksumValue( int index, quint64 value );
        void progressUpdate( float progress );
};

#endif // FILEHASHCALCULATOR_H
