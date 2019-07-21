all:
	gcc -fno-builtin -O2 -nostdinc -nostdlib -ffreestanding -g -Wall -Wextra -Werror -I. -MMD -mno-red-zone -mcmodel=kernel -fno-pie -Wa,--divide   -c -o boot.o boot.S
	gcc -fno-builtin -O2 -nostdinc -nostdlib -ffreestanding -g -Wall -Wextra -Werror -I. -MMD -mno-red-zone -mcmodel=kernel -fno-pie   -c -o main.o main.c
	gcc -z max-page-size=0x1000 -fno-builtin -O2 -nostdinc -nostdlib -ffreestanding -g -Wall -Wextra -Werror -I. -MMD -mno-red-zone -mcmodel=kernel -fno-pie -no-pie -Wl,--build-id=none -T kernel.ld -o kernel boot.o main.o
