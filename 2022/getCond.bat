:: [UTF-8 en/ru]
:: using format get 2022 5
:: my_cookie - session id надо вручную подставлять! пока сессия не отвалится.
:: хром, отладка, network, тыкаю get your puzzle input, 
:: появляется пакет input, парсю глазками, нахожу куки, в них нахожу session=огромное слово
:: результат положить в session.cookie - сам файл в гит игноре.
echo off
if exist session.cookie (set /p my_cookie=<session.cookie) else (echo configure session in first)
set YEAR=2022
set DAY=%1
set OUT_DIR=day%DAY%
if not exist %OUT_DIR% mkdir %OUT_DIR%
set urlPath="https://adventofcode.com/%YEAR%/day/%DAY%/input"
set FILE_OUT="%OUT_DIR%/cond.input"
curl --cookie "%my_cookie%" "%urlPath%" > %FILE_OUT%
copy template.cpp "%OUT_DIR%/day%DAY%_1.cpp"
cd %OUT_DIR%
start /b code . "day%DAY%_1.cpp"
pause 5
:: почему не работает выход?

exit
pause 1
