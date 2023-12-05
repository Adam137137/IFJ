RED='\033[0;031m'
GREEN='\033[0;32m'
X='\033[47m'
OFF='\033[0m'
declare -a tests_syntax=("test_empty.txt" "test_func0.txt" "test_func1.txt" "test_func2.txt" "test_func3.txt" "test_func4.txt" "test_func5.txt" "test_func6.txt" "test_func7.txt"
                        "test_let0.txt" "test_let1.txt" "test_let2.txt" "test_let3.txt" "test_let4.txt" "test_let5.txt" "test_let6.txt" "test_text.txt" "test_empty.txt" "test_if0.txt" "test_if1.txt" "test_if2.txt" "test_if3.txt"
                        "test_var0.txt" "test_var1.txt" "test_var2.txt" "test_var3.txt" "test_var4.txt" "test_var5.txt" "test_while0.txt" "test_while1.txt" "test_while2.txt")
declare -a tests_semantic=("test_funcparam1.txt" "test_funcparam2.txt" "test_funcparam3.txt" "test_funcreturn1.txt" "test_funcreturn2.txt" "test_mismatchtypes.txt" "test_notinitialized.txt" "test_undefinedfunc.txt"
                        "test_undefinedvar.txt" "test_wrongdefinition.txt" "test_nil.txt")
declare -a tests_math=("test_add0.txt" "test_add1.txt" "test_brackets0.txt" "test_brackets1.txt" "test_brackets2.txt" "test_brackets3.txt" "test_brackets4.txt" "test_brackets5.txt" "test_div0.txt" "test_div1.txt" "test_div2.txt"
                        "test_mul0.txt" "test_mul1.txt" "test_sub0.txt" "test_sub1.txt")
echo -e "${X}Testing syntax:${OFF}"
for i in "${tests_syntax[@]}"
do
    echo -e "Test: $i"
    ./Main < ./tests/syntax/$i 2> ./tests/cache.txt
    if [[ $(< ./tests/cache.txt) != "syntax error" ]]; then
        echo -e "${RED}TEST FAILED!${OFF}"
        echo $(< ./tests/cache.txt)
    else
        echo -e "${GREEN}TEST PASSED!${OFF}"
        echo $(< ./tests/cache.txt)
    fi
    echo ""
done

echo -e "${X}Testing semantic:${OFF}"
for i in "${tests_semantic[@]}"
do
    echo "Test: $i"
    ./Main < ./tests/semantic/$i 2> ./tests/cache.txt
    if ! grep -q "^semantic error:" ./tests/cache.txt; then
        echo -e "${RED}TEST FAILED!${OFF}"
        echo $(< ./tests/cache.txt)
    else
        echo -e "${GREEN}TEST PASSED!${OFF}"
        echo $(< ./tests/cache.txt)
    fi
    echo ""
done

echo -e "${X}Testing math:${OFF}"
for i in "${tests_math[@]}"
do
    echo -e "Test: $i"
    ./Main < ./tests/math/$i > ./tests/cache.txt
    if [[ $(< ./tests/cache.txt) == "OK" ]]; then
        echo -e "${GREEN}TEST PASSED!${OFF}"
        echo $(< ./tests/cache.txt)
    else
        echo -e "${RED}TEST FAILED!${OFF}"
        echo $(< ./tests/cache.txt)
    fi
    echo ""
done