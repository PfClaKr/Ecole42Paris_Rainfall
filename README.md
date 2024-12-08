# Ecole42Paris_Rainfall
We have to run RainFall.iso file in VM, and connect with ssh with ip which is displayed in the VM.
```bash
$> ssh level0@[ip] -p 4242
```
```text
  GCC stack protector support:            Enabled
  Strict user copy checks:                Disabled
  Restrict /dev/mem access:               Enabled
  Restrict /dev/kmem access:              Enabled
  grsecurity / PaX: No GRKERNSEC
  Kernel Heap Hardening: No KERNHEAP
 System-wide ASLR (kernel.randomize_va_space): Off (Setting: 0)
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX enabled    No PIE          No RPATH   No RUNPATH   /home/user/level0/level0
```
With this text, you are succeed of login.

#### GCC stack protector 
- the logic of protect of stack buffer overflow.
- usually insert Canary value in the return stack address.
- if there was attempt of attack stack, we can check Canary value it was corrupted.

#### Strict user copy checks
- strict user copy checks (also know as CONFIG_DEBUG_STRICT_USER_COPY_CHECKS) isn't implemented on x86_64 
[Ref.](https://lore.kernel.org/lkml/1306865673-20560-1-git-send-email-sboyd@codeaurora.org/T/)

#### Restrict /dev/mem /dev/kmem Access
- in the linux, /dev/mem and /dev/kmem is **physical memory** address.
- the role of access physical memory.

#### grsecurity / Pax
- both of them is some patch security of linux.
- protect vulnerability of kernel, protect memory, protect attack of permission.

#### Kernel Heap Hardening
- with reinforce of kernel heap memory, can protect memory vulnerability like heap buffer over flow.

#### System-wide ASLR (kernel.randomize_va_space)
- ASLR(Addrerss Space Layout Randomization), by this, its hard to attack the binary file like exploit(especially return-to-libarary).
[Ref.](https://linux-audit.com/linux-aslr-and-kernelrandomize_va_space-setting/)

#### Security configuration of binary file
|Name|Description|
|-----|---|
|RELRO|Relocation Read-only, option of memory protect while running.|
|Stack canary|Option of protect of buffer overflow.|
|NX|Non-Executable, Option of memory protect.|
|PIE|Position Independent Executable, fix library address position.|
|Rpath / Runpath|Path of runtime library.|

