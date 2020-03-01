add-symbol-file kernel/lunar.kernel
target remote | qemu-system-x86_64 -S -gdb stdio -monitor tcp::1337,server,nowait -m 128M -cdrom lunarOS.iso -d cpu_reset
