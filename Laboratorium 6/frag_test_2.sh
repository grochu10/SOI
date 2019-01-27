echo "POCZATEK TESTU FRAGMENTACJI POROWNAWCZEGO"
echo "TWORZENIE SYSTEMU"
./a.out crt_fs fs2 50000
echo "DODANIE PLIKOW DO SYSTEMU"
./a.out cp_to fs2 maly.txt marvel1.txt
./a.out cp_to fs2 maly.txt marvel21.txt
./a.out cp_to fs2 maly.txt marvel3.txt
./a.out cp_to fs2 maly.txt marvel4.txt
./a.out cp_to fs2 maly.txt marvel22.txt
./a.out cp_to fs2 maly.txt marvel23.txt
./a.out cp_to fs2 maly.txt marvel24.txt
./a.out cp_to fs2 maly.txt marvel32.txt
./a.out cp_to fs2 sredni.txt mgo\ poczatek2.txt
./a.out cp_to fs2 sredni.txt mgo\ poczatek3.txt
./a.out cp_to fs2 sredni.txt mgo\ poczatek22.txt
./a.out cp_to fs2 duzy.txt akcent2.txt
echo "STAN DYSKU PO DODANIU PLIKOW"
./a.out ls_fs fs2
echo "AKTUALNA MAPA ZAJETOSCI"
./a.out disp_map fs2
echo "INFORMACJE O SYSTEMIE"
./a.out info_fs fs2
echo "USUNIECIE SYSTEMU"
./a.out rm_fs fs2
echo "KONIEC TESTU FRAGMENTACJI POROWNAWCZEGO"