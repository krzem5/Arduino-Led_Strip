@echo off
cls
rm -rf build&&python D:\boot\boot.py 5 compile ./ arduino:avr:uno&&python D:\boot\boot.py 5 upload ./ COM3 arduino:avr:uno
