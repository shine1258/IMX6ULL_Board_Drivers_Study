#include "bsp_clk.h"
#include "MCIMX6Y2.h"

/// @brief 使能外设时钟
void clk_enable(void)
{
    CCM->CCGR0 = 0xFFFFFFFF;
    CCM->CCGR1 = 0xFFFFFFFF;
    CCM->CCGR2 = 0xFFFFFFFF;
    CCM->CCGR3 = 0xFFFFFFFF;
    CCM->CCGR4 = 0xFFFFFFFF;
    CCM->CCGR5 = 0xFFFFFFFF;
    CCM->CCGR6 = 0xFFFFFFFF;
}

/// @brief 初始化时钟
void clk_init_imx6ull(void)
{
    /* 设置主频为528MHz */
    /* 系统时钟切换 */
    if ((((CCM->CCSR) >> 2) & 0x1) == 0) { // 如果当前时钟使用pll1_main_clk(PLL1)
        CCM->CCSR &= ~(1 << 8); // 设置step_clk为24MHz
        CCM->CCSR |= (1 << 2); // 选择作为step_clk
    }

    CCM_ANALOG->PLL_ARM = (1 << 13) | (88 << 0); // 设置PLL1为1056MHz
    CCM->CACRR = 1; // ARM内核时钟为PLL1的2分频，528MHz
    CCM->CCSR &= ~(1 << 2); // 切换回pll1_main_clk(PLL1)

    int reg;

    /* 设置PLL2的4路PFD */
    reg = CCM_ANALOG->PFD_528;
    reg &= ~(0x3F3F3F3F); // 清除原来的设置
    reg |= 27; // PLL2_PFD0=352MHz
    reg |= (16 << 8); // PLL2_PFD1=594MHz
    reg |= (24 << 16); // PLL2_PFD2=396MHz
    reg |= (32 << 24); // PLL2_PFD3=297MHz
    CCM_ANALOG->PFD_528 = reg;

    /* 设置PLL3的4路PFD */
    reg = CCM_ANALOG->PFD_480;
    reg &= ~(0x3F3F3F3F); // 清除原来的设置
    reg |= 12; // PLL3_PFD0=720MHz
    reg |= (16 << 8); // PLL3_PFD1=540MHz
    reg |= (17 << 16); // PLL3_PFD2=508.2MHz
    reg |= (19 << 24); // PLL3_PFD3=454.7MHz
    CCM_ANALOG->PFD_480 = reg;

    /* 设置AHB_CLK_ROOT为132MHz */
    CCM->CBCMR &= ~(3 << 18);
    CCM->CBCMR |= (1 << 18); // 选择PRE_PERIPH_CLK_SEL为PLL2_PFD2 = 396MHz
    CCM->CBCDR &= ~(1 << 25); // 选择PERIPH_CLK_SEL为PLL2
    while ((CCM->CDHIPR >> 5) & 0x1) { } // 等待握手完成
    // 内部boot rom中已经设置了AHB_PODF为3分频
    // CCM->CBCDR &= ~(7 << 10);
    // CCM->CBCDR |= (2 << 10); // 设置AHB_PODF为3分频
    // while ((CCM->CDHIPR >> 1) & 0x1) { } // 等待握手完成

    /* 设置IPG_CLK_ROOT为66MHz */
    CCM->CBCDR &= ~(3 << 8);
    CCM->CBCDR |= (1 << 8); // 设置IPG_PODF为2分频

    /* 设置PERCLK_CLK_ROOT为66MHz */
    CCM->CSCMR1 &= ~(1 << 6); // PERCLK_CLK_ROOT时钟源选择IPG
    CCM->CSCMR1 &= ~(63); // PERCLK_PODF为1分频
}