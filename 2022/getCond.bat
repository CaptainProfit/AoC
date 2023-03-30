::[UTF-8 en/ru]
::using format get 2022 5
::my_cookie - session id надо вручную подставлять! пока сессия не отвалится.
::хром, отладка, network, тыкаю get your puzzle input, появляется пакет input, парсю глазками, нахожу куки, в них нахожу session 
echo off :: не пали мои сессии!
set my_cookie="session=5xxx5;"
set YEAR=2022
set DAY=%1
set OUT_DIR=day%DAY%
if not exist %OUT_DIR% mkdir %OUT_DIR%
set urlPath="https://adventofcode.com/%YEAR%/day/%DAY%/input"
set FILE_OUT="%OUT_DIR%/cond.input"
curl --cookie "%my_cookie%" "%urlPath%" > %FILE_OUT%
copy template.cpp %OUT_DIR%/day%DAY%_1.cpp