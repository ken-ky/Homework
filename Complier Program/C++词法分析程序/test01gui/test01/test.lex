read READ
num ID
read READ
_9a3 ID
if IF
1 NUM
* MUL
( LEC
2 NUM
+ ADD
3 NUM
) REC
<> NEQ
1 NUM
- SUB
( LEC
1 NUM
+ ADD
4 NUM
) REC
% MOD
5 NUM
/ DIV
6 NUM
then THEN
num ID
:= ASS
3 NUM
repeat REPEAT
if IF
num ID
< LES
7 NUM
then THEN
num ID
:= ASS
num ID
+ ADD
2 NUM
else ELSE
num ID
:= ASS
num ID
+ ADD
1 NUM
end END
until UNTIL
num ID
= EQU
10 NUM
end END
write WRITE
( LEC
num ID
+ ADD
3 NUM
) REC
/ DIV
2 NUM
* MUL
( LEC
num ID
/ DIV
2 NUM
- SUB
3 NUM
) REC
% MOD
