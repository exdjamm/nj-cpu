for i in $(ls --sort=size -rx ../test_log_compare/gen/ | egrep .ent); do
echo file: $i;

for j in $(seq 1 $1); do

./nj.run ../test_log_compare/gen/$i > ../test_log_compare/output_c2/out_$i.log

done

done
