#pragma once

typedef struct pde_t pde_t;

struct pde_t {
   uint64_t present: 1;
   uint64_t rw: 1;
   uint64_t us: 1;
   uint64_t pwt: 1;
   uint64_t pcd: 1;
   uint64_t accessed: 1;
   uint64_t dirty: 1;
   uint64_t pat: 1;
   uint64_t global: 1;
   uint64_t : 3;
   uint64_t base: 40; /* base address */
   uint64_t : 6;
   uint64_t pk: 4;
   uint64_t xd: 1;
};
