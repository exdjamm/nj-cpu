echo "seq $1";

for j in $(seq 1 $1); do

for i in $(ls --sort=size -rx ../test_log_compare/genpot/ | egrep .ent); do

./nj.run ../test_log_compare/genpot/$i

done
echo ""
done
