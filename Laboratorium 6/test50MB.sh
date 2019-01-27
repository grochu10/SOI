echo "POCZATEK TESTU NR 2"
echo "TWORZENIE SYSTEMU"
./a.out crt_fs fs 50000000
echo "INFORMACJE O SYSTEMIE"
./a.out info_fs fs
echo "POCZATKOWY STAN DYSKU"
./a.out ls_fs fs
echo "DODANIE DO SYSTEMU WIELU MALYCH PLIKOW"
for n in `seq 1 4`;
    do
        ./a.out cp_to fs maly.txt marvel"$n".txt
done
echo "STAN DYSKU PO DODANIU PLIKOW"
./a.out ls_fs fs
echo "USUNIECIE 2 MALYCH PLIKOW PLUS WYWYOLANIE Z NIEISTNIEJACYM"
./a.out rm_file_fs fs marvel2.txt
./a.out rm_file_fs fs marvel3.txt
./a.out rm_file_fs fs bsba.txt
echo "STAN DYSKU PO USUNIECIU PLIKOW"
./a.out ls_fs fs
echo "DODANIE DO SYSTEMU KILKU SREDNICH PLIKOW"
for n in `seq 1 3`;
    do
        ./a.out cp_to fs sredni.txt mgo\ poczatek"$n".txt
done
echo "STAN DYSKU PO DODANIU PLIKOW"
./a.out ls_fs fs
echo "USUNIECIE SREDNIEGO PLIKU "
./a.out rm_file_fs fs mgo\ poczatek1.txt
echo "STAN DYSKU PO USUNIECIU PLIKOW"
./a.out ls_fs fs
echo "DODANIE DO SYSTEMU DUZEGO PLIKU"
./a.out cp_to fs duzy.txt akcent1.txt
echo "STAN DYSKU PO DODANIU PLIKOW"
./a.out ls_fs fs
echo "AKTUALNA MAPA ZAJETOSCI"
./a.out disp_map fs
echo "SKOPIOWANIE KAZDEGO Z RODZAJOW PLIKOW Z SYSTEMU DO LINUXA"
./a.out cp_from fs mgo\ poczatek3.txt mgo\ poczatek_linux.txt
./a.out cp_from fs marvel4.txt marvel_linux.txt
./a.out cp_from fs akcent1.txt akcent_linux.txt
echo "SKOPIOWANIE KAZDEGO Z RODZAJOW PLIKOW Z SYSTEMU DO LINUXA JENDYM POLECENIEM"
./a.out gcp_from fs mgo\ poczatek2.txt marvel1.txt akcent1.txt mgo\ poczatek_linux2.txt marvel_linux2.txt akcent_linux2.txt
echo "STAN DYSKU"
./a.out ls_fs fs
echo "USUNIECIE DUZEGO PLIKU "
./a.out rm_file_fs fs akcent1.txt
echo "SKOPIOWANIE KAZDEGO DO RODZAJOW PLIKOW Z SYSTEMU Z LINUXA JENDYM POLECENIEM"
./a.out gcp_to fs sredni.txt maly.txt duzy.txt mgo\ poczatek11.txt marvel11.txt akcent11.txt
echo "STAN DYSKU PO DODANIU PLIKOW"
./a.out ls_fs fs
echo "AKTUALNA MAPA ZAJETOSCI"
./a.out disp_map fs
echo "INFORMACJE O SYSTEMIE"
./a.out info_fs fs
echo "USUNIECIE SYSTEMU"
./a.out rm_fs fs
echo "KONIEC TESTU NR 2"