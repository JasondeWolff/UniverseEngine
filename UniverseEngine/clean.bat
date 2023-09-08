cd bin
del /s /q *.exe
del /s /q *.lib
del /s /q *.idb
del /s /q *.pdb
cd..
cd intermediate
del /s /q *
cd..
rmdir /s /q intermediate
