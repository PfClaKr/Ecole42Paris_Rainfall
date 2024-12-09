After login, there is one excutable file name of level0.
```sh
level0@RainFall:~$ ls -la
total 737
dr-xr-x---+ 1 level0 level0     60 Mar  6  2016 .
dr-x--x--x  1 root   root      340 Sep 23  2015 ..
-rw-r--r--  1 level0 level0    220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level0 level0   3530 Sep 23  2015 .bashrc
-rwsr-x---+ 1 level1 users  747441 Mar  6  2016 level0
-rw-r--r--  1 level0 level0    675 Apr  3  2012 .profile
level0@RainFall:~$ ./level0
Segmentation fault (core dumped)
level0@RainFall:~$ ./level0 42
No !
level0@RainFall:~$
```
Without parameter, it gives us a Segmentation fault, but other than that, it gives me ```No !```. \
\
Run the binary file with GDB.
```sh
level0@RainFall:~$ gdb level0
(gdb) disas main
...
   0x08048ecc <+12>:    add    $0x4,%eax
   0x08048ecf <+15>:    mov    (%eax),%eax
   0x08048ed1 <+17>:    mov    %eax,(%esp)
   0x08048ed4 <+20>:    call   0x8049710 <atoi>
   0x08048ed9 <+25>:    cmp    $0x1a7,%eax # 0x1a7 = 423
   0x08048ede <+30>:    jne    0x8048f58 <main+152>
...
```
in the line main+25, it compares return value of atoi and 0x1a7, in decimal 423. \
try with this number.
```sh
level0@RainFall:~$ ./level0 423
$ whoami
level1
$ cat /home/user/level1/.pass
(hidden)
$ 
```
level0 passed!