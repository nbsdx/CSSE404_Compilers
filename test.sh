BIN=./main
TESTDIR=./test/lexer

for f in $TESTDIR/*.in
do
    OUT=`basename $f in`out
    TMPFILE=`mktemp /tmp/${OUT}.XXXXXX` || exit
    echo "Running test $f..."
    $BIN $f > $TMPFILE
    if ! diff $TMPFILE $TESTDIR/$OUT; then
        echo "Test failed! See $TMPFILE"
    else
        rm $TMPFILE
    fi
done
