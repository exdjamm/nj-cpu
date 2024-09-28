for j in $(seq 1 $1); do

for i in $(ls --sort=size -rx ../test_log_compare/gen/ | egrep .ent); do

./nj.run ../test_log_compare/gen/$i

done
echo ""
done
