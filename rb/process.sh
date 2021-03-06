sed -n '/100/p' out.txt > out100.txt
sed -n '/110/p' out.txt > out110.txt
sed -n '/120/p' out.txt > out120.txt
sed -n '/130/p' out.txt > out130.txt
sed -n '/140/p' out.txt > out140.txt
sed -n '/150/p' out.txt > out150.txt

awk '{ print $2/(0.01*$1) " " $3/(0.01*$1) }' out.txt > temp.txt
awk '{ print $2/(0.01*$1) " " $3/(0.01*$1) }' out100.txt > temp100.txt
awk '{ print $2/(0.01*$1) " " $3/(0.01*$1) }' out110.txt > temp110.txt
awk '{ print $2/(0.01*$1) " " $3/(0.01*$1) }' out120.txt > temp120.txt
awk '{ print $2/(0.01*$1) " " $3/(0.01*$1) }' out130.txt > temp130.txt
awk '{ print $2/(0.01*$1) " " $3/(0.01*$1) }' out140.txt > temp140.txt
awk '{ print $2/(0.01*$1) " " $3/(0.01*$1) }' out150.txt > temp150.txt

sort temp.txt > fin_85.txt
sort temp.txt > fin_87.txt

grep ^0.2 temp.txt > fin_85.txt
grep ^0.2 temp100.txt > fin100_85.txt
grep ^0.2 temp110.txt > fin110_85.txt
grep ^0.2 temp120.txt > fin120_85.txt
grep ^0.2 temp130.txt > fin130_85.txt
grep ^0.2 temp140.txt > fin140_85.txt
grep ^0.2 temp150.txt > fin150_85.txt

grep ^0.1 temp.txt > fin_87.txt
grep ^0.1 temp100.txt > fin100_87.txt
grep ^0.1 temp110.txt > fin110_87.txt
grep ^0.1 temp120.txt > fin120_87.txt
grep ^0.1 temp130.txt > fin130_87.txt
grep ^0.1 temp140.txt > fin140_87.txt
grep ^0.1 temp150.txt > fin150_87.txt

root -l -q g_factor.C+\(\"fin_85.txt\",\"fin_87.txt\",100\)
root -l -q g_factor.C+\(\"fin100_85.txt\",\"fin100_87.txt\",100\)
root -l -q g_factor.C+\(\"fin110_85.txt\",\"fin110_87.txt\",110\)
root -l -q g_factor.C+\(\"fin120_85.txt\",\"fin120_87.txt\",120\)
root -l -q g_factor.C+\(\"fin130_85.txt\",\"fin130_87.txt\",130\)
root -l -q g_factor.C+\(\"fin140_85.txt\",\"fin140_87.txt\",140\)
root -l -q g_factor.C+\(\"fin150_85.txt\",\"fin150_87.txt\",150\)