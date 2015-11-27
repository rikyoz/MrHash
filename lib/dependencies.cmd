appveyor DownloadFile https://github.com/rikyoz/QtCryptoHash/releases/download/v0.1.0/QtCryptoHash-v0.1.0-msvc2013-win32-static.7z -FileName qtcryptohash-static.7z 
7z x qtcryptohash-static.7z -oqtcryptohash/ -aoa
appveyor DownloadFile https://github.com/rikyoz/QtCryptoHash/releases/download/v0.1.0/QtCryptoHash-v0.1.0-msvc2013-win64-static.7z -FileName qtcryptohash64-static.7z
7z x qtcryptohash64-static.7z -oqtcryptohash/ -aoa