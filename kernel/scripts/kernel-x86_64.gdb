add-symbol-file kernel/lunar.kernel
target remote | qemu-system-x86_64 -S -gdb stdio -m 128M -cpu host \
          -enable-kvm -cdrom lunaros.iso
