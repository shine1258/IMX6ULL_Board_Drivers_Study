.global _start // 全局标号

_start:
    /* 使能所有外设时钟 */ 
    ldr r0, = 0x020C4068 // CCGR0
    ldr r1, = 0xFFFFFFFF // 要向CCGR0写入的值
    str r1, [r0]        // 将0xFFFFFFFF写入CCGR0

    ldr r0, = 0x020C406C // CCGR1
    str r1, [r0]        

    ldr r0, = 0x020C4070 // CCGR2
    str r1, [r0]

    ldr r0, = 0x020C4074 // CCGR3
    str r1, [r0]

    ldr r0, = 0x020C4078 // CCGR4
    str r1, [r0]

    ldr r0, = 0x020C407C // CCGR5
    str r1, [r0]

    ldr r0, = 0x020C4080 // CCGR6
    str r1, [r0]

    /* 配置GPIO1_IO03的复用为GPIO */
    ldr r0, = 0x020E0068 // IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03
    ldr r1, = 0x00000005 // 要向IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03写入的值
    str r1, [r0]         // 将0x00000005写入IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03

    /* 配置GPIO1_IO03的的电气属性 
     * bit 0(摇摆率): 0 低速率
     * bit 5:3(驱动能力): 110 R0/6
     * bit 7:6(速度): 10 100MHz
     * bit 11(开漏): 0 关闭
     * bit 12(保持器使能): 1 使能
     * bit 13(上拉或保持): 0 保持
     * bit 15:14(设置上下拉阻值): 00 100K下拉
     * bit 16(使能整形): 0 关闭
     */
    ldr r0, = 0x020E02F4 // IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03
    ldr r1, = 0x10B0     // 要向IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03写入的值
    str r1, [r0]         // 将0x10B0写入IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03

    /* 设置GPIO为输出模式 */
    ldr r0, = 0x0209C004 // GPIO1_GDIR
    ldr r1, = 0x00000008 
    str r1, [r0]

    /* 输出低电平点灯 */
    ldr r0, = 0x0209C000 // GPIO1_DR
    ldr r1, = 0x00000000
    str r1, [r0]

loop:
    b loop
