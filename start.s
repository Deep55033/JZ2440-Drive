
.text
.global _start
_start:
	b reset
	ldr pc, _undefined_exception
	ldr pc, _swi_exception
	ldr pc, _loop  // prefetch abort 
	ldr pc, _loop  // data abort 
	ldr pc, _loop // Reserved
	ldr pc, _irq // irq
	ldr pc, _loop // fiq

_undefined_exception:
	.word  _do_undefined_exception

_swi_exception:
	.word _do_swi_exception

_irq:
	.word _do_irq

_do_irq:
	// 初始化栈指针
	ldr sp, =0x33D00000
	sub lr, lr, #4
	// 保存现场
	stmfd sp!, {r0-r12, lr}
	// 跳转到中断处理函数
	bl interrupt_handler
	// 恢复现场
	ldmfd sp!, {r0-r12, pc}^


_do_undefined_exception:
	// 初始化栈指针
	ldr sp, =0x33E00000
	// 保存现场
	stmfd sp!, {r0-r12, lr}
	mrs r0, cpsr
	ldr r1, =_undefined_info_str
	// 跳转到中断处理函数
	bl undefined_exception
	// 恢复现场
	ldmfd sp!, {r0-r12, pc}^

_undefined_info_str:
	.string "  Exception is _undefined_exception\r\n"

.align 4
_do_swi_exception:
	// 初始化栈指针
	ldr sp, =0x33F00000
	// 保存现场
	stmfd sp!, {r0-r12, lr}
	mov r0, lr
	sub r0, r0, #4
	ldr r1, =_swi_info_str
	// 跳转到中断处理函数
	bl swi_exception
	// 恢复现场
	ldmfd sp!, {r0-r12, pc}^

_swi_info_str:
	.string "  Exception is _swi_exception\r\n"

// 四字节对齐 字符串长度不固定
.align 4
reset:
	// 关闭看门狗
	ldr r0, =0x53000000
	mov r1, #0
	str r1, [r0]
	
	//  FCLK 400 MHZ HCLK 133.3333MHZ PCLK 66.666 MHZ
	// 设置 LOCKTIME 为 0xffffffff
	ldr r0, =0xFFFFFFFF
	ldr r1, =0x4C000000
	str r0, [r1]

	//  分频系数
	ldr r0, =0x4C000014
	mov  r1, #0x7
	str r1, [r0]

	// 设置异步总线模式  (芯片手册)
	mrc p15,0,r0,c1,c0,0
	orr r0,r0,#0xc0000000
	mcr p15,0,r0,c1,c0,0
	
	// 配置 MPLL 输入 12MHZ 输出 400MHZ
	ldr r0, =0x4C000004
	ldr r1, =(0x5c << 12) | (1 << 4) | (1 << 0)
	str r1, [r0]

	// 判断开发板是从nand启动还是从nor启动
	mov r0, #0x100
	mov r1, #0x0
	str r0, [r1]
	ldr r1, [r1]
	cmp r0, r1
	// 从nand启动
	ldreq sp, =4096
	// 从nor启动
	ldrne sp, =0x40000000 + 4096

	bl system_init

	// 开启中断
	mrs r0, cpsr 
	bic r0, r0, #0x80
	msr cpsr, r0 

	// 开启指令cache 可以提高程序执行速度
	MRC p15, 0, R0, c1, c0, 0
	orr R0, R0, #(1 << 12)
	MCR p15, 0, R0, c1, c0, 0

	// 重新指定栈指针
	ldr sp, =0x34000000

	// 跳转到 sdram
	ldr pc, =_sdram
_sdram:
	
	// 这个是一个 条件执行的汇编语句  bl print1 这个指令刚好触发条件
	// .word 0xdeadc0de  
	bl uart0_init

	// 触发软中断
	swi 0x123
	
	ldr pc, =main


_loop:
	b _loop
