#ifndef FILEHASHCALCULATOR_H
#define FILEHASHCALCULATOR_H

#include <QWidget>
#include <QThread>

class FileHashCalculator : public QThread {
        Q_OBJECT

    public:
        FileHashCalculator( QWidget* parent, QString fileName, bool uppercase );
        virtual ~FileHashCalculator();

    protected:
        void run() override;

    private:
        const QString file_name;
        const bool use_uppercase;

    signals:
        void newHashString( int index, QString hash );
};

#endif // FILEHASHCALCULATOR_H
