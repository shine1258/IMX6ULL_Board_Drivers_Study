.global _start

_start:
    /* 处理器进入SVC模式 */
    mrs r0, cpsr        // 将CPSR寄存器的值读入R0
    bic r0, r0, #0x1f   // 清除[4:0]位
    orr r0, r0, #0x13   // 设置[4:0]位为0x13
    msr cpsr, r0        // 将R0的值写入CPSR寄存器

    /* 设置SP指针 */
    ldr sp, = 0x80200000
    b main              // 跳转到C语言的main函数