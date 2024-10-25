.global _start
.global _bss_start
.global _bss_end

_bss_start:
    .word __bss_start

_bss_end:
    .word __bss_end

_start:
    /* 处理器进入SVC模式 */
    mrs r0, cpsr        // 将CPSR寄存器的值读入R0
    bic r0, r0, #0x1f   // 清除[4:0]位
    orr r0, r0, #0x13   // 设置[4:0]位为0x13
    msr cpsr, r0        // 将R0的值写入CPSR寄存器

    /* 清除bss段 */
    ldr r0, _bss_start
    ldr r1, _bss_end
    mov r2, #0

bss_loop:
    stmia r0!, {r2}
    cmp r0, r1          // 比较r0和r1的值
    ble bss_loop        // 如果r0 <= r1, 则继续循环

    /* 设置SP指针 */
    ldr sp, = 0x80200000
    b main              // 跳转到C语言的main函数