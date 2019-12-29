LunarOS
=======

[![Total alerts](https://img.shields.io/lgtm/alerts/g/tammela/lunarOS.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/tammela/lunarOS/alerts/)

[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/tammela/lunarOS.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/tammela/lunarOS/context:cpp)


An exokernel operating system inspired by the MIT experiments.

#### Build

```bash
make ARCH=x86_64 iso
```

#### Testing

```bash
make ARCH=x86_64 qemu
```

### License

See [`LICENSE`](./LICENSE).

LunarOS has substantial parts of its code base borrowed or inspired by other
open source operating systems.

LunarOS borrows code from the following sources:
- xv6-public
- OSDev wiki
- Multiboot2 specification
- NetBSD
