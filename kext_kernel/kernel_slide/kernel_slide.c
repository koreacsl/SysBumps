//
//  kernel_slide.c
//  kernel_slide
//
//  Created by hyerean JJang on 2023/01/05.
//

#include <mach/mach_types.h>
#include <vm/vm_kern.h>
#include <sys/systm.h>
#include <libkern/version.h>
#include <mach-o/loader.h>
#include <os/log.h>


kern_return_t kernel_slide_start(kmod_info_t * ki, void *d);
kern_return_t kernel_slide_stop(kmod_info_t *ki, void *d);

#define MAX_SLIDE_STEP      4096
#define VM_MIN_KERNEL_ADDRESS           ((vm_offset_t) 0xfffffe0000000000) //0xFFFFFF8000000000
#define VM_MIN_KERNEL_AND_KEXT_ADDRESS  (VM_MIN_KERNEL_ADDRESS) //0xFFFFFF8000000000
#define VM_KERNEL_LINK_ADDRESS          ((vm_offset_t) 0xfffffe0007004000)


static vm_offset_t get_vm_kernel_addrperm_ext(void)
{
    static vm_offset_t addrperm_ext = 0L;
    if (addrperm_ext != 0L) goto out_exit;
    vm_kernel_addrperm_external(VM_MIN_KERNEL_AND_KEXT_ADDRESS, &addrperm_ext);
    addrperm_ext -= VM_MIN_KERNEL_AND_KEXT_ADDRESS;
out_exit:
    return addrperm_ext;
}


static vm_offset_t get_vm_kernel_slide(void)
{
    static uint16_t i = MAX_SLIDE_STEP;
    static vm_offset_t fake = VM_MIN_KERNEL_AND_KEXT_ADDRESS;
    static vm_offset_t slide = 0L;

    if (get_vm_kernel_addrperm_ext() == 0L) goto out_exit;
    if (slide != 0L || i == 0) goto out_exit;

    while (--i) {
        vm_kernel_unslide_or_perm_external(fake, &slide);
        /* We assume unslide address is unique to those two */
        if (slide != fake && slide != fake + get_vm_kernel_addrperm_ext()) {
            slide = fake - slide;
            break;
        }
        fake += 0x100000L;
        slide = 0L;
    }

out_exit:
    return slide + VM_KERNEL_LINK_ADDRESS;
}


kern_return_t kernel_slide_start(kmod_info_t * ki, void *d)
{
    vm_offset_t kernel_base;
    
    kernel_base = get_vm_kernel_slide();
    
    os_log(OS_LOG_DEFAULT, "[SysBumps_log] base = 0x%x%08x\n", (uint32_t)(((uint64_t)kernel_base) >> 32), (uint32_t)(((uint64_t)kernel_base) & 0xFFFFFFFF));
    return KERN_SUCCESS;
}

kern_return_t kernel_slide_stop(kmod_info_t *ki, void *d)
{
    return KERN_SUCCESS;
}

