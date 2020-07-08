Comparing outputs
Test case 0 Failed

user output vs test output
1,2d0
< exit
<

You lost 1 points
----------------------------------------
----------------------------------------
Comparing outputs
Test case 1 Failed

user output vs test output
1,2d0
< echo "NzHHhRPB7wSpCluGslu94wwbUSf14EGIYPki0wp8T6uhHOIDFZZY2qNdmINDlEXNNctME4ejCUUJLb1sdQUP5sXy7EpThX4NiKuHDT11XRUyu8zldkPAyZybLv"; exit
<

You lost 2 points
----------------------------------------
----------------------------------------
Comparing outputs
Test case 2 Failed

user output vs test output
1,2d0
< echo -n "4MjS0G5hDPhjKAh7n7rd16UjyMUh6DmacyLXBTzanRdl3N4luMW3PbqYOp0Dsdxxuwh3n2Hkvcov8uyBnSE7ykUBYHjkiZuikSmZwTdPTf3TUJYJOOmkUiAzHonZeOjCuEnSSlcd"; exit
<

You lost 2 points
----------------------------------------
----------------------------------------
Comparing outputs
Test case 3 Failed

user output vs test output
1,2d0
< echo "\t  \v \a yZhF \v  \toIIb4uP4 \a" ; exit
<

You lost 5 points
----------------------------------------
----------------------------------------
Comparing outputs
Test case 4 Failed

user output vs test output
1,2d0
< which -s echo; echo $? ; exit
<

You lost 5 points
----------------------------------------
----------------------------------------
Comparing outputs
Test case 5 Failed

user output vs test output
1,2d0
< cd .. ; pwd ; exit
<

You lost 5 points
----------------------------------------
----------------------------------------
Comparing outputs
Test case 6 Failed

user output vs test output
1,2d0
< cd / ; pwd ; exit
<

You lost 5 points
----------------------------------------
----------------------------------------
Comparing outputs
Test case 7 Failed

user output vs test output
1,2d0
< cd ../../../ ; cd - ; cd .. ; pwd ; exit
<

You lost 5 points
----------------------------------------
----------------------------------------
Comparing outputs
Test case 8 Failed

user output vs test output
1,2d0
< cd .. ; cd . ; pwd ; exit
<

You lost 5 points
----------------------------------------
----------------------------------------
Comparing outputs
Test case 9 Failed

user output vs test output
1,2d0
< cd /tmp ; pwd -L ; exit
<

You lost 10 points
----------------------------------------
----------------------------------------
Comparing outputs
Test case 10 Failed

user output vs test output
1,2d0
< cd /tmp ; pwd -P ; exit
<

You lost 10 points
----------------------------------------
----------------------------------------
Comparing outputs
Test case 11 Failed

user output vs test output
1,3c1
< cd .. ; echo "$(pwd)" ; exit
<
< pwd /private/var/folders/0g/dddjws3n49x3yvgvt3b5wvqr0000gp/T/tmpsf7jhmjw/user_repo
---
> /private/var/folders/0g/dddjws3n49x3yvgvt3b5wvqr0000gp/T/tmpsf7jhmjw

You lost 10 points
----------------------------------------
----------------------------------------
Comparing outputs
Test case 12 Failed

user output vs test output
1,3c1
< cd .. ; echo "`echo \`pwd\``" ; exit
<
< echo pwd pwd /private/var/folders/0g/dddjws3n49x3yvgvt3b5wvqr0000gp/T/tmpsf7jhmjw/user_repo
---
> /private/var/folders/0g/dddjws3n49x3yvgvt3b5wvqr0000gp/T/tmpsf7jhmjw

You lost 10 points
----------------------------------------
----------------------------------------
Comparing outputs
Test case 13 Failed

user output vs test output
1,2d0
< echo ${SHLVL} ${HOME} ${LOGNAME} ${USR} ${TERM} ; exit
<

You lost 5 points
----------------------------------------
----------------------------------------
Comparing outputs
Test case 14 Failed

user output vs test output
1,2d0
< cd ~ ; pwd ; exit
<

You lost 5 points
----------------------------------------
----------------------------------------
Comparing outputs
Test case 15 Failed

user output vs test output
1,3c1
< export UNIT=location; export UFK=p2p ucode=cbl; echo $UNIT $UFK $ucode; exit
<
<
---
> location p2p cbl

You lost 5 points
