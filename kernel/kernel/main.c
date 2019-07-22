void main (void) {
   unsigned char *vga = (unsigned char *)0xb8000;
   vga[0] = '.';
   vga[1] = 4;

   while (1);
}
