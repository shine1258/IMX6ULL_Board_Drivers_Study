.global _start
.global _bss_start
.global _bss_end

_start:
    ldr pc, = Reset_Handler // 复位中断函数
    ldr pc, = Undefined_Handler // 未定义指令中断函数
    ldr pc, = SVC_Handler // SVC中断函数
    ldr pc, = PreAbort_Handler // 预取中止中断函数
    ldr pc, = DataAbort_Handler // 数据中止中断函数
    ldr pc, = NotUsed_Handler // 未使用中断函数
    ldr pc, = IRQ_Handler // IRQ中断函数
    ldr pc, = FIQ_Handler // FIQ中断函数

Reset_Handler:
    cpsid i // 关闭IRQ中断

    /* 关闭I, D Cache和MMU */
    mrc p15, 0, r0, c1, c0, 0 // 读取SCTLR寄存器的值到R0寄存器
    bic r0, r0, #(1 << 12)    // 关闭I Cache
    bic r0, r0, #(1 << 11)    // 关闭分支预测
    bic r0, r0, #(1 << 2)     // 关闭D Cache
    bic r0, r0, #(1 << 1)     // 关闭对齐
    bic r0, r0, #1            // 关闭MMUs
    mcr p15, 0, r0, c1, c0, 0 // 将R0寄存器的值写入SCTLR寄存器

    @ /* 设置中断向量偏移 */
    @ ldr r0, = 0x87800000
    @ dsb
    @ isb
    @ mcr p15, 0, r0, c12, c0, 0  // 设置VBAR寄存器为0x87800000
    @ dsb
    @ isb

_bss_start:
    .word __bss_start

_bss_end:
    .word __bss_end

    /* 清除bss段 */
    ldr r0, _bss_start
    ldr r1, _bss_end
    mov r2, #0

bss_loop:
    stmia r0!, {r2}
    cmp r0, r1          // 比较r0和r1的值
    ble bss_loop        // 如果r0 <= r1, 则继续循环

    /* 处理器进入IRQ模式 */
    mrs r0, cpsr            // 将CPSR寄存器的值读入R0
    bic r0, r0, #0x1f       // 清除[4:0]位
    orr r0, r0, #0x12       // 设置[4:0]位为0x12
    msr cpsr, r0            // 将R0的值写入CPSR寄存器
    ldr sp, = 0x80600000    // 设置IRQ模式下的sp指针

    /* 处理器进入SYS模式 */
    mrs r0, cpsr            // 将CPSR寄存器的值读入R0
    bic r0, r0, #0x1f       // 清除[4:0]位
    orr r0, r0, #0x1f       // 设置[4:0]位为0x1f
    msr cpsr, r0            // 将R0的值写入CPSR寄存器
    ldr sp, = 0x80400000    // 设置SYS模式下的sp指针
 
    /* 处理器进入SVC模式 */
    mrs r0, cpsr            // 将CPSR寄存器的值读入R0
    bic r0, r0, #0x1f       // 清除[4:0]位
    orr r0, r0, #0x13       // 设置[4:0]位为0x13
    msr cpsr, r0            // 将R0的值写入CPSR寄存器
    ldr sp, = 0x80200000    // 设置SVC模式下的sp指针

    cpsie i // 开启IRQ中断
    b main              // 跳转到C语言的main函数

Undefined_Handler:
    ldr r0, = Undefined_Handler
    bx r0

SVC_Handler:
    ldr r0, = SVC_Handler
    bx r0

PreAbort_Handler:
    ldr r0, = PreAbort_Handler
    bx r0

DataAbort_Handler:
    ldr r0, = DataAbort_Handler
    bx r0

NotUsed_Handler:
    ldr r0, = NotUsed_Handler
    bx r0

IRQ_Handler:
    push {lr}					/* 保存lr地址 */
	push {r0-r3, r12}			/* 保存r0-r3，r12寄存器 */

	mrs r0, spsr				/* 读取spsr寄存器 */
	push {r0}					/* 保存spsr寄存器 */

	mrc p15, 4, r1, c15, c0, 0 /* 从CP15的C0寄存器内的值到R1寄存器中
								* 参考文档ARM Cortex-A(armV7)编程手册V4.0.pdf P49
								* Cortex-A7 Technical ReferenceManua.pdf P68 P138
								*/							
	add r1, r1, #0X2000			/* GIC基地址加0X2000，也就是GIC的CPU接口端基地址 */
	ldr r0, [r1, #0XC]			/* GIC的CPU接口端基地址加0X0C就是GICC_IAR寄存器，
								 * GICC_IAR寄存器保存这当前发生中断的中断号，我们要根据
								 * 这个中断号来绝对调用哪个中断服务函数
								 */
	push {r0, r1}				/* 保存r0,r1 */
	
	cps #0x13					/* 进入SVC模式，允许其他中断再次进去 */
	
	push {lr}					/* 保存SVC模式的lr寄存器 */
	ldr r2, =system_irqhandler	/* 加载C语言中断处理函数到r2寄存器中*/
	blx r2						/* 运行C语言中断处理函数，带有一个参数，保存在R0寄存器中 */

	pop {lr}					/* 执行完C语言中断服务函数，lr出栈 */
	cps #0x12					/* 进入IRQ模式 */
	pop {r0, r1}				
	str r0, [r1, #0X10]			/* 中断执行完成，写EOIR */

	pop {r0}						
	msr spsr_cxsf, r0			/* 恢复spsr */

	pop {r0-r3, r12}			/* r0-r3,r12出栈 */
	pop {lr}					/* lr出栈 */
	subs pc, lr, #4				/* 将lr-4赋给pc */

FIQ_Handler:
    ldr r0, = FIQ_Handler
    bx r0 
