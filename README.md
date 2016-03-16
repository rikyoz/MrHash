# Mr. Hash <img src="/res/icon.png" width="40px" height="40px" align="right" />

**Mr. Hash** is a cross-platform tool for calculating the hashes of files and plain texts.

[![](http://img.shields.io/badge/version-v0.3.2-blue.png?style=flat)](https://github.com/rikyoz/mrhash/releases/latest)
![](https://img.shields.io/badge/platform-Windows%20|%20Linux%20|%20Mac%20OS%20X-red.png?style=flat)
![](http://img.shields.io/badge/architecture-x86%20|%20x64-yellow.png?style=flat)
[![](http://img.shields.io/badge/license-GNU%20GPL%20v2-lightgrey.png?style=flat)](/LICENSE)

| Builds |
|:-------:|
|[![Build  Status](https://img.shields.io/appveyor/ci/rikyoz/mrhash.png?label=%20win%20build&logo=data%3Aimage%2fsvg%2bxml%3Bbase64%2CPD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiIHN0YW5kYWxvbmU9Im5vIj8%2bPCFET0NUWVBFIHN2ZyBQVUJMSUMgIi0vL1czQy8vRFREIFNWRyAxLjEvL0VOIiAiaHR0cDovL3d3dy53My5vcmcvR3JhcGhpY3MvU1ZHLzEuMS9EVEQvc3ZnMTEuZHRkIj48c3ZnICAgeG1sbnM6c3ZnPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyIgICB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciICAgdmVyc2lvbj0iMS4xIiAgIHdpZHRoPSIyMjAiICAgaGVpZ2h0PSIyMjAiICAgaWQ9InN2ZzUzNTgiPiAgPGRlZnMgICAgIGlkPSJkZWZzNTM2MCIgLz4gICA8cGF0aCAgICAgZD0iTSAxMDQuODY3NSwxMDQuODY3NSAwLDEwNC44Njc1IDAsMCBsIDEwNC44Njc1LDAgMCwxMDQuODY3NSB6IiAgICAgaWQ9InBhdGg1MDU4IiAgICAgc3R5bGU9ImZpbGw6I2YxNTExYjtmaWxsLW9wYWNpdHk6MTtmaWxsLXJ1bGU6bm9uemVybztzdHJva2U6bm9uZSIgLz4gIDxwYXRoICAgICBkPSJtIDIyMC42NTM3NSwxMDQuODY3NSAtMTA0Ljg2NjI1LDAgMCwtMTA0Ljg2NzUgMTA0Ljg2NjI1LDAgMCwxMDQuODY3NSB6IiAgICAgaWQ9InBhdGg1MDYwIiAgICAgc3R5bGU9ImZpbGw6IzgwY2MyODtmaWxsLW9wYWNpdHk6MTtmaWxsLXJ1bGU6bm9uemVybztzdHJva2U6bm9uZSIgLz4gIDxwYXRoICAgICBkPSJtIDEwNC44NjUsMjIwLjY5NSAtMTA0Ljg2NSwwIDAsLTEwNC44Njc1IDEwNC44NjUsMCAwLDEwNC44Njc1IHoiICAgICBpZD0icGF0aDUwNjIiICAgICBzdHlsZT0iZmlsbDojMDBhZGVmO2ZpbGwtb3BhY2l0eToxO2ZpbGwtcnVsZTpub256ZXJvO3N0cm9rZTpub25lIiAvPiAgPHBhdGggICAgIGQ9Im0gMjIwLjY1Mzc1LDIyMC42OTUgLTEwNC44NjYyNSwwIDAsLTEwNC44Njc1IDEwNC44NjYyNSwwIDAsMTA0Ljg2NzUgeiIgICAgIGlkPSJwYXRoNTA2NCIgICAgIHN0eWxlPSJmaWxsOiNmYmJjMDk7ZmlsbC1vcGFjaXR5OjE7ZmlsbC1ydWxlOm5vbnplcm87c3Ryb2tlOm5vbmUiIC8%2bPC9zdmc%2b)](https://ci.appveyor.com/project/rikyoz/mrhash)|
|![Build Not Available](https://img.shields.io/badge/linux%20build-not%20available-lightgrey.svg?logo=data%3Aimage%2fpng%3Bbase64%2CiVBORw0KGgoAAAANSUhEUgAAABQAAAAYCAYAAAD6S912AAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAA5AAAAOQBXaJ52AAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAAM0SURBVDiNnZRdaFtlGMd%2f7%2bnZWaRJT0DTxbY0sTMiwmxlURztLLSUDYYXCttoRIQzL0a3MESZ6EWlF%2b2NQi%2bLOB2iFwYEGYikF5LdCRaRYQZmRdeyj5LWkpR8mCU5fbzITknT5jj2h%2ffifZ73%2bb38n%2fO8B9z1EnANyAObwJdA9%2f%2fUtFUAyAHirFgsJsBNwPM4wLebYS3LalekuQCfHB8fZ2hoCE3TmJubw7IsNE0DOPE4QH1iYoKpqSmi0SimaTI5OYmu6wBPtS1yAZZzG3d4pesHPj27TG57kWK%2bk%2b3tbXDpoRswd6r3W44PNDZbpess309h23WAF4GDwIPWIjfLpd%2fv9vBrxubG3zaZe3D41QtEIs9hWZYXlz6202epVEpuXZ%2bRzV%2fiIsVbIiISj8fFMAwBloCOR4VFo9FotVarSasymYwopZzx%2bfhRLHcBidHR0QO6rmPbNvV6vdGDUgmPx8PIyIhz9hMa%2fXQFfuT3%2bwfC4TAA2WwWpRQA6XSa2dlZhoeH8fl8GIZhAF8Aqp3VnkAg8O%2f09PQeq81aWVmRvr4%2bCYVCjvWT7YAzoVBI1tbWXIEiIpZlSTAYdPp5bT%2fLGnBucHCQYDC4E1xdXd335kgkQrVadbZjPJzpZuALSqneUCi0E6hUKlQqlX2BXq8XwzAQEQAv8AQ0vZTLZw372WAH6eIyIoJSCl3XnYLdquWpF1fRNI3%2b%2fn4KhcLdXC5X2snLTxyUpJmUpCm3v%2fbJO2%2b9IeVyWURE9ptFSV%2bSpc8PCyCdHiV%2fXfXdlkXzGQBNkuYCmpnl4VMKH9IYCSQZGxtjfX3d%2bbvsUr1a4Gj%2fPyzOdnJjwWsPPK2FETkGoCTZdQ9UT3OBCBz%2foMjLE3Hm5%2bf3AH%2f76ghHe%2b%2b0hgsgkxqoUmtGKfjmQz8z05d3xYvFIrFYjEp%2bZW9fwQdqQUnSLND4Snv0%2fVI3m4H3qdVqZDIZEokEGxsbZL%2frotvf%2bjjUj3SoCzrwHnAGONYKNjvuc%2fr8%2bT8bdtgCDijF836vOtR0rITinDqRTwDo6uTWFeCKpNCpmq8h6k2Q14GyLVwEfm6%2bpDFFchXUaeAmtvauOpX7w8n%2fB2yDkWCN8JJoAAAAAElFTkSuQmCC)|
|![Build Not Available](https://img.shields.io/badge/mac%20build-not%20available-lightgrey.svg?logo=data%3Aimage%2fpng%3Bbase64%2CiVBORw0KGgoAAAANSUhEUgAAABQAAAAXCAYAAAALHW%2bjAAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAACtAAAArQBf4zpPAAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAAO1SURBVDiNnVRdaFxFFP5m7s9m%2b7PbJtt2G023JbXkQRTEp7zqm7X%2bJcoafAjaBiwKCvXBBx%2bqIIhPPgkRkVSIJVhXCmLVEosUxEANWh%2ba5kdK23Sb%2fbm7e3fv3b0zc3yYe282bYToXA4zc%2becb76Z851h%2bI%2ft3LmZo72702Nuw7159Njzp%2b5dN7cKRETs4sXvP%2b3fl32VMRiBCD7YzI9vFfDChe8mDuZyJyzbMpSiwHHqP2zmt2WGmUzmNcMwoSCxVirPjI7mf%2fnfDCcnJ%2felU6lHODewVir9eO368uv%2f5su6J7Ozs88OHOg%2fwQ1jgDMj8Nv%2bslOp%2frq8%2fPfKkYcGj1Wdmsz09fIdO7c9xhjvUySbzXrz6tLKjfdHRkZ%2bjwGJiM3PXzmTO5jLG4axgTWBIISgoN0J7IRtExGICKQUOh0fzUYDlUqpdvvW3YkXXsyfZQAwNzf30eEjg6c452CsizgRCACRAinSPRHavo%2b6U4HveVBSQEqBesO9vbiy%2bLg5PT09MJB74KQGY2Bg63hgYFAA6X9SKDhOBUG7DSUlOGcgxdAJhFMqOScnJt5aNYeGDr%2fR05Pcxhg0IGMxQ8YISnEAEk23Ab%2fV1JnkHKSU9mUMpXLlw5defqUAAGYqvXs4DNdfF6A%2btUDDcUAADNOCFCIE0jFNt3Wr4rQ%2biWVjmXwvIpDQiXMOxhk67TbqTg2mZa%2bzZ%2bvbA4CQcmF8fNyPATk3E5sJqeW68D0PPT3JDQvsHl9isLp%2fcSECN0pB1NVrVUghkUgkAdLSCQfhGPHcsszBqamp7euAUq6CKLwvQnntLmQgYVk2lJJQoVSISIMRwrFu25PJ%2fel08s0YsFavXdGOhJpTBRHBsm1IKaCkBCmlLQZWWp%2bhgQh9u9Jvz8x8%2bQQA8KXrK2eCoCObrou64yBhJyCl1Ka0KRUKW6kNQBHrhG1l9u%2fZ%2b9W335w9zQDg6l9%2fzPut%2bqPZ7INdWtRHU0rpo0ebhJUhpYAU6z0AVCr1SQ4AS4sLH6dTvUrK7kBtKmQYWVyGKqppfZtBIHzHrX4eq%2bDy5UuF%2fmz2Gca73gYiqPAhiFkqGdavjBkSEdbK5c%2beenr0eBxdLK6%2b03Abd6JkRExVt0XAEeMwUW7LW6g4%2fnv36fT8%2bcJzh3IHvkgk7FR0h1FmgyAAZyy%2bBr2ZgOf5d4pFJz%2baz%2f98HyAAFApfP7kn0%2feubZkPCylv%2bC3vp2KpdMnzgps7kvZQatfO4YRlDYMo67c7f5ad5umxsbHfovh%2fAJNdi91dvezVAAAAAElFTkSuQmCC)|



## Features
+ Supports hashing of **files** and **plain text**
+ Calculates the most used **hashing algorithms**, such as **MD5**, **SHA1** and [**many others**](#supported-algorithms)
+ Supports **CRC16**, **CRC32** and **CRC64** checksum algorithms
+ Simple and easy-to-use GUI
+ Cross platform: it runs on **Windows**, **Linux** and **Mac OS X**

## Screenshots
<p align="center">
<a href="https://raw.github.com/rikyoz/mrhash/screenshots/doc/img/v0.3.2/screenshot0.png" target="_blank">
<img src="https://raw.github.com/rikyoz/mrhash/screenshots/doc/img/v0.3.2/screenshot0.thumb.png" />
</a>
<a href="https://raw.github.com/rikyoz/mrhash/screenshots/doc/img/v0.3.2/screenshot1.png" target="_blank">
<img src="https://raw.github.com/rikyoz/mrhash/screenshots/doc/img/v0.3.2/screenshot1.thumb.png" />
</a>
<a href="https://raw.github.com/rikyoz/mrhash/screenshots/doc/img/v0.3.2/screenshot1.png" target="_blank">
<img src="https://raw.github.com/rikyoz/mrhash/screenshots/doc/img/v0.3.2/screenshot2.thumb.png" />
</a>
</p>

## Supported Algorithms
+ MD4
+ MD5
+ SHA1
+ SHA-224, SHA-256, SHA-384, SHA-512
+ SHA3-224, SHA3-256, SHA3-384, SHA3-512
+ Tiger
+ HAVAL-128, HAVAL-160, HAVAL-192, HAVAL-224, HAVAL-256
+ RIPEMD-160
+ CRC16
+ CRC32
+ CRC64

## License (GPL v2)
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
