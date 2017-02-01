; Timer4B.s
; Runs on LM4F120/TM4C123
; Use Timer4B in periodic mode to request interrupts at a particular
; period.
; Andrew Lynch
; January 31, 2017

;  This example accompanies the book
;   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers"
;   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2015
;   Volume 1, Program 9.8

;  "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
;   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
;   Volume 2, Program 7.5, example 7.6

; Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
;   You may use, edit, run or distribute this file
;   as long as the above copyright notice remains
;THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
;OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
;MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
;VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
;OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
;For more information about my classes, my research, and my books, see
;http://users.ece.utexas.edu/~valvano/

; Timer4B is interrupt 71

NVIC_EN2_INT71         EQU 0x00000080  ; Interrupt 71 enable
NVIC_EN2_R             EQU 0xE000E108  ; Interrupt 64-95 Set Enable Register
NVIC_PRI17_R           EQU 0xE000E444  ; Interrupt 68-71 Priority Register

TIMER4_CFG_R           EQU 0x40034000
TIMER_CFG_32_BIT_TIMER EQU 0x00000000  ; 32-bit timer configuration,
                                      ; function is controlled by bits
                                      ; 1:0 of GPTMTAMR and GPTMTBMR
TIMER4_TBMR_R          EQU 0x40034008
TIMER_TBMR_TBMR_PERIOD EQU 0x00000002 ; Periodic Timer mode
    
TIMER4_CTL_R           EQU 0x4003400C
TIMER_CTL_TBEN         EQU 0x00000100  ; GPTM TimerB Enable

TIMER4_IMR_R           EQU 0x40034018
TIMER_IMR_TBTOIM       EQU 0x00000100  ; GPTM TimerB Time-Out Interrupt
                                      ; Mask
TIMER4_ICR_R           EQU 0x40034024
TIMER_ICR_TBTOCINT     EQU 0x00000100  ; GPTM TimerB Time-Out Raw
                                      ; Interrupt
TIMER4_TBILR_R         EQU 0x4003402C
TIMER_TBILR_M          EQU 0x00000FFF  ; GPTM TimerB Interval Load
                                      ; Register Low
TIMER4_TBPR_R          EQU 0x4003403C
    
SYSCTL_RCGCTIMER_R     EQU 0x400FE604
TIMER4_SYSCTL_EN       EQU 0x00000010

        AREA    |.text|, CODE, READONLY, ALIGN=2
        THUMB
        EXPORT   Timer4B_Init

; ***************** Timer4B_Init ****************
; Activate Timer4B interrupts to run user task periodically
; Input: R0  interrupt period
;        Units of period are 20ns (assuming 50 MHz clock)
;        Maximum is 2^16-1
;        Minimum is determined by length of ISR
; Output: none
; Modifies: R0, R1, R2
Timer4B_Init
    ; 0) activate clock for Timer4
    LDR R1, =SYSCTL_RCGCTIMER_R     ; activate Timer4 clock
    LDR R2, [R1]                    
    ORR R2, R2, #TIMER4_SYSCTL_EN       
    STR R2, [R1]                  
    NOP
    NOP                             ; allow time to finish activating
    ; 1) disable timer4B during setup
    LDR R1, =TIMER4_CTL_R           
    LDR R2, [R1]                   
    BIC R2, R2, #TIMER_CTL_TBEN         ; clear enable bit
    STR R2, [R1]                   
    ; 2) configure for 32-bit timer mode
    LDR R1, =TIMER4_CFG_R          
    MOV R2, #TIMER_CFG_32_BIT_TIMER             ; 32-bit mode
    STR R2, [R1]                
    ; 3) configure for periodic mode
    LDR R1, =TIMER4_TBMR_R         
    MOV R2, #TIMER_TBMR_TBMR_PERIOD             ; periodic mode
    STR R2, [R1]                   
    ; 4) reload value, period =(TBILR+1)*12.5ns
    LDR R1, =TIMER4_TBILR_R        
    SUB R0, R0, #TIMER_TBILR_M                  ; counts down from TBILR to 0
    STR R0, [R1]                  
    ; 5) 1us timer4B
    LDR R1, =TIMER4_TBPR_R          
    MOV R2, #0                      ; R2 = 0 (divide clock by 1)
    STR R2, [R1]                   
    ; 6) clear timer4B timeout flag
    LDR R1, =TIMER4_ICR_R          
    MOV R2, #TIMER_ICR_TBTOCINT             ; clear TBTORIS bit
    STR R2, [R1]                 
    ; 7) arm timeout interrupt
    LDR R1, =TIMER4_IMR_R           
    MOV R2, #TIMER_IMR_TBTOIM             ; Arm TBTORIS
    STR R2, [R1]                    
    ; 8) set NVIC interrupt 19 to priority 2
    LDR R1, =NVIC_PRI17_R           
    LDR R2, [R1]                    
    AND R2, R2, #0x00FFFFFF         ; clear interrupt 19 priority
    ORR R2, R2, #0xE0000000         ; interrupt 19 priority is in bits 31-29
    STR R2, [R1]                  
    ; 9) enable interrupt 19 in NVIC
    LDR R1, =NVIC_EN2_R            
    MOV R2, #NVIC_EN2_INT71             ; interrupt 19 enabled
    STR R2, [R1]                   
    ; 10) enable timer4B
    LDR R1, =TIMER4_CTL_R         
    LDR R2, [R1]                  
    ORR R2, R2, #TIMER_CTL_TBEN         ; set Timer4 enable bit
    STR R2, [R1]                   
    BX  LR                       

    ALIGN                           ; make sure the end of this section is aligned
    END                             ; end of file
