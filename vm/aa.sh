cd ..
source ./activate
cd vm
make clean
make 
cd build
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/fork-read:fork-read -p ../../tests/userprog/sample.txt:sample.txt --swap-disk=4 -- -q   -f run fork-read < /dev/null 2> tests/userprog/fork-read.errors |tee tests/userprog/fork-read.output
# perl -I../.. ../../tests/userprog/fork-read.ck tests/userprog/fork-read tests/userprog/fork-read.result

# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/fork-close:fork-close -p ../../tests/userprog/sample.txt:sample.txt --swap-disk=4 -- -q   -f run fork-close < /dev/null 2> tests/userprog/fork-close.errors |tee tests/userprog/fork-close.output
# perl -I../.. ../../tests/userprog/fork-close.ck tests/userprog/fork-close tests/userprog/fork-close.result

# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/fork-boundary:fork-boundary --swap-disk=4 -- -q   -f run fork-boundary < /dev/null 2> tests/userprog/fork-boundary.errors |tee tests/userprog/fork-boundary.output
# perl -I../.. ../../tests/userprog/fork-boundary.ck tests/userprog/fork-boundary tests/userprog/fork-boundary.result

# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/read-boundary:read-boundary -p ../../tests/userprog/sample.txt:sample.txt --swap-disk=4 -- -q   -f run read-boundary < /dev/null 2> tests/userprog/read-boundary.errors |tee tests/userprog/read-boundary.output
# perl -I../.. ../../tests/userprog/read-boundary.ck tests/userprog/read-boundary tests/userprog/read-boundary.result

pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/write-bad-ptr:write-bad-ptr -p ../../tests/userprog/sample.txt:sample.txt --swap-disk=4 -- -q   -f run write-bad-ptr < /dev/null 2> tests/userprog/write-bad-ptr.errors |tee tests/userprog/write-bad-ptr.output
perl -I../.. ../../tests/userprog/write-bad-ptr.ck tests/userprog/write-bad-ptr tests/userprog/write-bad-ptr.result

# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/write-bad-fd:write-bad-fd --swap-disk=4 -- -q   -f run write-bad-fd < /dev/null 2> tests/userprog/write-bad-fd.errors |tee tests/userprog/write-bad-fd.output
# perl -I../.. ../../tests/userprog/write-bad-fd.ck tests/userprog/write-bad-fd tests/userprog/write-bad-fd.result