.syntax unified
.cpu cortex-m0
.thumb

.global Reset_Handler
.global main

.extern SysTick_Handler
.extern USART2_IRQHandler

.extern _sidata
.extern _sdata
.extern _edata
.extern _sbss
.extern _ebss
.extern _estack

.section .vector_table, "a", %progbits

.word _estack
.word Reset_Handler + 1

.word 0                 // NMI
.word 0                 // HardFault

.rept 7
.word 0
.endr

.word 0                 // SVCall

.word 0
.word 0

.word 0                 // PendSV

.word SysTick_Handler + 1

.word 0                 // WWDG
.word 0                 // Reserved
.word 0                 // RTC

.rept 24
.word 0
.endr

.word 0                // USART1
.word USART2_IRQHandler + 1

.section .text

Reset_Handler:

    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata

copy_data:

    cmp r1, r2
    bge zero_bss

    ldr r3, [r0]
    str r3, [r1]

    adds r0, #4
    adds r1, #4

    b copy_data

zero_bss:

    ldr r1, =_sbss
    ldr r2, =_ebss

    movs r3, #0

zero_loop:

    cmp r1, r2
    bge call_main

    str r3, [r1]

    adds r1, #4

    b zero_loop

call_main:

    bl main

hang:
    wfi
    b hang
