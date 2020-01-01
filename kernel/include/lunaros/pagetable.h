#pragma once

#include <std/stdint.h>

/*
** x86 page table maps.
** To keep a page as read-only, set the `rw` field as 0.
** To keep a page as execution disabled, set the `xd` field as 1.
** To keep the page from user access, set the `us` field as 0.
*/

typedef struct pml4e_t pml4e_t;

struct pml4e_t {
   uint64_t present: 1;
   uint64_t rw: 1;      /* read/write */
   uint64_t us: 1;      /* user/supervisor */
   uint64_t pwt: 1;
   uint64_t pcd: 1;
   uint64_t accessed: 1;
   uint64_t : 1;        /* ignored */
   uint64_t : 1;        /* reserved (must be 0) */
   uint64_t : 4;        /* ignored */
   uint64_t base: 40;   /* base address */
   uint64_t : 12;       /* ignored */
   uint64_t xd: 1;      /* execute disabled */
};

/* 1 GiB page entry */
typedef struct huge_pdpte_t huge_pdpte_t;

struct huge_pdpte_t {
   uint64_t present: 1;
   uint64_t rw: 1;      /* read/write */
   uint64_t us: 1;      /* user/supervisor */
   uint64_t pwt: 1;
   uint64_t pcd: 1;
   uint64_t accessed: 1;
   uint64_t dirty: 1;
   uint64_t ps: 1;      /* page size */
   uint64_t global: 1;
   uint64_t : 3;        /* ignored */
   uint64_t pat : 1;
   uint64_t : 17;       /* reserved (must be 0) */
   uint64_t base: 22;   /* base address */
   uint64_t : 7;        /* ignored */
   uint64_t pk: 4;      /* protection key */
   uint64_t xd: 1;      /* execute disabled */
};

typedef struct pdpte_t pdpte_t;

struct pdpte_t {
   uint64_t present: 1;
   uint64_t rw: 1;      /* read/write */
   uint64_t us: 1;      /* user/supervisor */
   uint64_t pwt: 1;
   uint64_t pcd: 1;
   uint64_t accessed: 1;
   uint64_t : 1;        /* ignored */
   uint64_t ps: 1;      /* page size */
   uint64_t : 4;        /* ignored */
   uint64_t base: 40;   /* base address */
   uint64_t : 11;       /* ignored */
   uint64_t xd: 1;      /* execute disabled */
};

/* 2 MiB page entry */
typedef struct huge_pde_t huge_pde_t;

struct huge_pde_t {
   uint64_t present: 1;
   uint64_t rw: 1;      /* read/write */
   uint64_t us: 1;      /* user/supervisor */
   uint64_t pwt: 1;
   uint64_t pcd: 1;
   uint64_t accessed: 1;
   uint64_t dirty: 1;
   uint64_t ps: 1;      /* page size */
   uint64_t global: 1;
   uint64_t : 3;        /* ignored */
   uint64_t pat : 1;
   uint64_t : 8;        /* reserved (must be 0) */
   uint64_t base: 32;   /* base address */
   uint64_t : 7;        /* ignored */
   uint64_t pk: 4;      /* protection key */
   uint64_t xd: 1;      /* execute disabled */
};

typedef struct pde_t pde_t;

struct pde_t {
   uint64_t present: 1;
   uint64_t rw: 1;      /* read/write */
   uint64_t us: 1;      /* user/supervisor */
   uint64_t pwt: 1;
   uint64_t pcd: 1;
   uint64_t accessed: 1;
   uint64_t : 1;        /* ignored */
   uint64_t ps: 1;      /* page size */
   uint64_t : 4;        /* ignored */
   uint64_t base: 40;   /* base address */
   uint64_t : 11;       /* ignored */
   uint64_t xd: 1;      /* execute disabled */
};

typedef struct pte_t pte_t;

struct pte_t {
   uint64_t present: 1;
   uint64_t rw: 1;      /* read/write */
   uint64_t us: 1;      /* user/supervisor */
   uint64_t pwt: 1;
   uint64_t pcd: 1;
   uint64_t accessed: 1;
   uint64_t dirty: 1;
   uint64_t pat: 1;
   uint64_t global: 1;
   uint64_t : 3;        /* ignored */
   uint64_t base: 40;   /* base address */
   uint64_t : 7;        /* ignored */
   uint64_t pk: 4;      /* protection key */
   uint64_t xd: 1;      /* execute disabled */
};

