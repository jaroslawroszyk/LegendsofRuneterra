# BakcylProgramowania_2020

## Dependencies
You must install these dependencies on your own: sqlite3, jsoncpp, qt5, libcurl

Ubuntu example:
```sh
sudo apt-get install libsqlite3-dev libjsoncpp-dev qt5-default libqt5charts5-dev libcurl4-openssl-dev unzip
```

## Build
Build requires cmake 3.14+

```
mkdir build
cd build
cmake ..
make
make db_builder
// download jsons from riot. This sucks. TODO: add autodownload to db_builder
curl --remote-name-all --parallel https://dd.b.pvp.net/latest/core/pl_pl/data/globals-pl_pl.json https://dd.b.pvp.net/latest/set1/pl_pl/data/set1-pl_pl.json https://dd.b.pvp.net/latest/set2/pl_pl/data/set2-pl_pl.json https://dd.b.pvp.net/latest/set3/pl_pl/data/set3-pl_pl.json https://dd.b.pvp.net/latest/set4/pl_pl/data/set4-pl_pl.json
./bin/db_builder
make card_api_example
```
