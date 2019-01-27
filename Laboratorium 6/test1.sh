echo "POCZATEK TESTU NR 1"
echo "TWORZENIE SYSTEMU"
./a.out crt_fs fs 50000
echo "INFORMACJE O SYSTEMIE"
./a.out info_fs fs
echo "POCZATKOWY STAN DYSKU"
./a.out ls_fs fs
echo "DODANIE DO SYSTEMU WIELU MALYCH PLIKOW"
for n in `seq 1 8`;
    do
        ./a.out cp_to fs maly.txt marvel"$n".txt
done
echo "STAN DYSKU PO DODANIU PLIKOW"
./a.out ls_fs fs
echo "USUNIECIE 3 MALYCH PLIKOW PLUS WYWYOLANIE Z NIEISTNIEJACYM"
./a.out rm_file_fs fs marvel2.txt
./a.out rm_file_fs fs marvel6.txt
./a.out rm_file_fs fs marvel7.txt
./a.out rm_file_fs fs bsba.txt
echo "STAN DYSKU PO USUNIECIU PLIKOW"
./a.out ls_fs fs
echo "DODANIE DO SYSTEMU KILKU SREDNICH PLIKOW"
for n in `seq 1 5`;
    do
        ./a.out cp_to fs sredni.txt mgo\ poczatek"$n".txt
        echo ""
done
echo "STAN DYSKU PO DODANIU PLIKOW"
./a.out ls_fs fs
echo "USUNIECIE 2 SREDNICH "
./a.out rm_file_fs fs mgo\ poczatek1.txt
./a.out rm_file_fs fs mgo\ poczatek4.txt
echo "STAN DYSKU PO USUNIECIU PLIKOW"
./a.out lsfs fs
echo "DODANIE DO SYSTEMU WIELU MALYCH PLIKOW"
for n in `seq 1 4`;
    do
        ./a.out cp_to fs maly.txt marvel2"$n".txt
done
echo "STAN DYSKU PO DODANIU PLIKOW"
./a.out ls_fs fs
echo "AKTUALNA MAPA ZAJETOSCI"
./a.out disp_map fs
echo "INFORMACJE O SYSTEMIE"
./a.out info_fs fs
echo "USUNIECIE SYSTEMU"
./a.out rm_fs fs