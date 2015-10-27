#include <QString>
#include <QtTest>

#include "../include/qhasher.hpp"

class Hash : public QObject {
        Q_OBJECT

    public:
        Hash();

    private Q_SLOTS:
        void initTestCase();
        void cleanupTestCase();
        void testCRC16();
        void testCRC32();
        void testCRC64();
        void testTiger();
        void testRipeMD();
        void testHaval();
};

Hash::Hash() {}

void Hash::initTestCase() {}

void Hash::cleanupTestCase() {}

void Hash::testCRC16() {
    QString calc_hash1 = QHasher::hash( "394", false, QHashAlgorithm::CRC16 );
    QString calc_hash2 = QHasher::hash( "Hello World!", false, QHashAlgorithm::CRC16 );
    QString calc_hash3 = QHasher::hash( "°@§éìò*", false, QHashAlgorithm::CRC16 );
    QString calc_hash4 = QHasher::hash( "¾ͣᵦ ᴻᶯ", false, QHashAlgorithm::CRC16 );
    QVERIFY2( calc_hash1 == "b91b", "Failed Test 1" );
    QVERIFY2( calc_hash2 == "bbb", "Failed Test 2" );
    QVERIFY2( calc_hash3 == "de36", "Failed Test 3" );
    QVERIFY2( calc_hash4 == "8f50", "Failed Test 4" );

}

void Hash::testCRC32() {
    QString calc_hash1 = QHasher::hash( "1234567890", false, QHashAlgorithm::CRC32 );
    QString calc_hash2 = QHasher::hash( "Lorem ipsum dolor sit amet", false, QHashAlgorithm::CRC32 );
    QString calc_hash3 = QHasher::hash( "#°@§çàèéìòù", false, QHashAlgorithm::CRC32 );
    QString calc_hash4 = QHasher::hash( "κόσμε", false, QHashAlgorithm::CRC32 );
    QVERIFY2( calc_hash1 == "261daee5", "Failed Test 1" );
    QVERIFY2( calc_hash2 == "5f29d461", "Failed Test 2" );
    QVERIFY2( calc_hash3 == "f037091", "Failed Test 3" );
    QVERIFY2( calc_hash4 == "7683b466", "Failed Test 4" );
}

void Hash::testCRC64() {
    QString calc_hash1 = QHasher::hash( "42", false, QHashAlgorithm::CRC64 );
    QString calc_hash2 = QHasher::hash( "Ciao Mondo!", false, QHashAlgorithm::CRC64 );
    QString calc_hash3 = QHasher::hash( "à*@òì^ùò", false, QHashAlgorithm::CRC64 );
    QString calc_hash4 = QHasher::hash( "¾ͣΔ֍ᵦ░⅞ ", false, QHashAlgorithm::CRC64 );
    QVERIFY2( calc_hash1 == "91895d8ea76f72e4", "Failed Test 1" );
    QVERIFY2( calc_hash2 == "b7b143e2d7955099", "Failed Test 2" );
    QVERIFY2( calc_hash3 == "4e2fed92d5a1b781", "Failed Test 3" );
    QVERIFY2( calc_hash4 == "9ea1bf21510ba84d", "Failed Test 4" );
}

void Hash::testTiger() {
    QString calc_hash1 = QHasher::hash( "abc", false, QHashAlgorithm::TIGER );
    QVERIFY2( calc_hash1 == "f258c1e88414ab2a527ab541ffc5b8bf935f7b951c132951", "Failed Test 1" );
}

void Hash::testRipeMD() {
    QString calc_hash1 = QHasher::hash( "message digest", false, QHashAlgorithm::RIPEMD160 );
    QVERIFY2( calc_hash1 == "5d0689ef49d2fae572b881b123a85ffa21595f36", "Failed Test 1" );
}

void Hash::testHaval() {
    QString calc_hash1 = QHasher::hash( "haval test vector", false, QHashAlgorithm::HAVAL192 );
    QVERIFY2( calc_hash1 == "459a0399abac23ceb4d6a89e7eb312a69eca335fa180a504", "Failed Test 1" );
}

QTEST_APPLESS_MAIN(Hash)

#include "tst_hash.moc"
