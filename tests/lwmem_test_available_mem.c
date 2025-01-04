#include <stdio.h>
#include "lwmem/lwmem.h"

#if LWMEM_CFG_ALIGN_NUM != 4
#error "Test shall run with LWMEM_CFG_ALIGN_NUM == 4"
#endif

/* Test assert information */
#define TEST_ASSERT(condition)                                                                                         \
    do {                                                                                                               \
        if (!(condition)) {                                                                                            \
            printf("Assert failed on the line %d\r\n", __LINE__);                                                      \
            return;                                                                                                    \
        }                                                                                                              \
    } while (0)

void
lwmem_test_available_mem(void) {
    static lwmem_t lwmem;
    uint32_t data_buff[256 >> 2];
    void *par1 = NULL, *par2 = NULL, *par3 = NULL;
    const lwmem_region_t regions[] = {
        {data_buff, sizeof(data_buff)},
        {NULL, 0},
    };

    /* Setup the values */
    lwmem_assignmem_ex(&lwmem, regions);
    TEST_ASSERT(lwmem.mem_available_bytes == 248);

    /* Allocate 10 bytes, that shall be properly aligned + header used*/
    par1 = lwmem_malloc_ex(&lwmem, NULL, 10);
    TEST_ASSERT(par1 != NULL);
    TEST_ASSERT(lwmem.mem_available_bytes == (228));
    par2 = lwmem_malloc_ex(&lwmem, NULL, 10);
    TEST_ASSERT(par2 != NULL);
    TEST_ASSERT(lwmem.mem_available_bytes == (208));
    par3 = lwmem_malloc_ex(&lwmem, NULL, 10);
    TEST_ASSERT(par3 != NULL);
    TEST_ASSERT(lwmem.mem_available_bytes == (188));

    /* Free the entry and check the new available bytes */
    lwmem_free_s_ex(&lwmem, &par3);
    TEST_ASSERT(par3 == NULL);
    TEST_ASSERT(lwmem.mem_available_bytes == (208));

    /* Free remaining 2 */
    lwmem_free_s_ex(&lwmem, &par2);
    TEST_ASSERT(par2 == NULL);
    TEST_ASSERT(lwmem.mem_available_bytes == (228));
    lwmem_free_s_ex(&lwmem, &par1);
    TEST_ASSERT(par1 == NULL);
    TEST_ASSERT(lwmem.mem_available_bytes == (248));
}
