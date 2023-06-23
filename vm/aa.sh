cd ..
source ./activate
cd vm
make clean
make 
cd build

pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/fork-read:fork-read -p ../../tests/userprog/sample.txt:sample.txt --swap-disk=4 -- -q   -f run fork-read < /dev/null 2> tests/userprog/fork-read.errors |tee tests/userprog/fork-read.output
perl -I../.. ../../tests/userprog/fork-read.ck tests/userprog/fork-read tests/userprog/fork-read.result

pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/fork-close:fork-close -p ../../tests/userprog/sample.txt:sample.txt --swap-disk=4 -- -q   -f run fork-close < /dev/null 2> tests/userprog/fork-close.errors |tee tests/userprog/fork-close.output
perl -I../.. ../../tests/userprog/fork-close.ck tests/userprog/fork-close tests/userprog/fork-close.result

pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/fork-boundary:fork-boundary --swap-disk=4 -- -q   -f run fork-boundary < /dev/null 2> tests/userprog/fork-boundary.errors |tee tests/userprog/fork-boundary.output
perl -I../.. ../../tests/userprog/fork-boundary.ck tests/userprog/fork-boundary tests/userprog/fork-boundary.result

pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/exec-boundary:exec-boundary -p tests/userprog/child-simple:child-simple --swap-disk=4 -- -q   -f run exec-boundary < /dev/null 2> tests/userprog/exec-boundary.errors > tests/userprog/exec-boundary.output
perl -I../.. ../../tests/userprog/exec-boundary.ck tests/userprog/exec-boundary tests/userprog/exec-boundary.result

pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/exec-read:exec-read -p ../../tests/userprog/sample.txt:sample.txt -p tests/userprog/child-read:child-read --swap-disk=4 -- -q   -f run exec-read < /dev/null 2> tests/userprog/exec-read.errors > tests/userprog/exec-read.output
perl -I../.. ../../tests/userprog/exec-read.ck tests/userprog/exec-read tests/userprog/exec-read.result

