#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#define MMIO_ADDR 0x1c180000
#define MMIO_LEN  0x20

int main(){
// open a handle into physical memory, requires root
int memfd = open("/dev/mem", O_RDWR);
// map the range [MMIO_ADDR, MMIO_ADDR+MMIO_LEN] into your virtual address space
double * shmem = mmap(0, MMIO_LEN, PROT_WRITE | PROT_READ, MAP_SHARED, memfd, MMIO_ADDR);

// do your deed
printf("%lf\n",shmem[3]);
printf("%lf\n",shmem[3]);
printf("%lf\n",shmem[3]);
printf("%lf\n",shmem[3]);
}

