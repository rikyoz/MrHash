#ifndef FILEHASHCALCULATOR_H
#define FILEHASHCALCULATOR_H

#include <QWidget>
#include <QMutex>
#include <QWaitCondition>
#include <QThread>

class FileHashCalculator : public QThread {
        Q_OBJECT

    public:
        FileHashCalculator( QWidget* parent, QString fileName );
        virtual ~FileHashCalculator();
        void stop();
        void resume();
        void pause();
        bool isPaused();

    protected:
        void run() override;

    private:
        const QString mFileName;

        QWaitCondition mPauseCondition;
        QMutex mMutex; //locked when accessing isPaused
        bool mIsPaused;

    signals:
        void newHashString( int index, QByteArray hash );
        void newChecksumValue( int index, quint64 value );
        void progressUpdate( float progress );
};

#endif // FILEHASHCALCULATOR_H
