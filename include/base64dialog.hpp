#ifndef BASE64DIALOG_HPP
#define BASE64DIALOG_HPP

#include <memory>

#include "ui_base64dialog.h"

#include "filebase64calculator.hpp"

class Base64Dialog : public QDialog, private Ui::Base64Dialog {
        Q_OBJECT

    public:
        explicit Base64Dialog( QString fileName, QWidget* parent = 0 );
        virtual ~Base64Dialog();

    private slots:
        void on_complete( QByteArray base64 );

    private:
        QString m_filename;
        std::unique_ptr< FileBase64Calculator > m_base64calculator;

    protected:
        void closeEvent( QCloseEvent* event ) Q_DECL_OVERRIDE;
};

#endif // BASE64DIALOG_HPP
