;=============================================================================
; (c) Copyright Raisonance S.A.S. 1989 - 2010, All rights reserved.
;=============================================================================
;
; This is the 'C' language "STARTUP" routine for RCSTM8, STM8 version.
;      File name:  STARTUP_STM8.ASM
;
;        Purpose:  To provide the initial startup code for STM8 or ST7 from
;                  power-up to the initial execution of the main() routine.
;
;          Scope:  This file contains all the code and information required
;                  to accomplish "Purpose". It is applicable to all memory
;                  models and RCSTM8 options.
;                  This file, assembled using the appropriate memory model,
;                  is automatically linked in by the linker when it encounters
;                  a "main" symbol in the object modules to be linked.
;
;                  The routine will be executed each time the controller
;                  experiences a "reset" condition.
;
;                  If you wish to use this startup routine with your assembly
;                  code too, you should add a "main" label in your asm code.
;
; Considerations:  The values selected are intended for a standard STM8 or ST7
;                  controller. In most cases these will work well for other
;                  processors under most conditions. The ranges for
;                  initialization, or the initialization values can be changed
;                  or modified as needed.
;
;        Caveats:  It is possible to modify this file and affect the execution
;                  the target processor in many ways. Often settings and
;                  selections have effects on each other. Some may not be
;                  compatible. This startup routine is provided "as is". If
;                  you change it to meet some design objective, you are
;                  responsible of the outcome of your selections, and
;                  their effect upon the target system.
;
;                  This file MUST be assembled in case-sensitive mode
;                  (assembler directive CASESENSITIVE or CS) to avoid any
;                  unresolved references by the linker.
;
;-----------------------------------------------------------------------------
;  NOTE:  YOU SHOULD ALTER THIS FILE ONLY AFTER YOU HAVE BUILT AND TESTED YOUR
;  CODE, AND YOU'RE CONVINCED THAT YOU CAN MAKE IT RUN OR WORK BETTER BY DOING
;  SO.  IF YOU ONLY NEED SOME SPECIFIC INITIALIZATIONS, IT IS RECOMMENDED THAT
;  YOU  DO  THEM  IN  THE FIRST FEW LINES OF YOUR MAIN() ROUTINE.  MODIFY THIS
;  FILE ONLY AS A LAST RESORT.
;=============================================================================
; $Id: startup_STM8.asm,v 1.2 2010/05/10 12:18:12 Bruno Exp $
;=============================================================================

        CSTART                  SEGMENT CODE INSECTION0
        RSEG                    CSTART

;-----------------------------------------------------------------------------
; External symbols
;-----------------------------------------------------------------------------

    ; The "?C?InitData" routine is responsible for initializing the global
    ; variables. If your application does not use global initialized variables,
    ; it will not be used.
    ; The routine is available from the standard library used with your model,
    ; for instance RCSTM8.LIB
    EXTRN   CODE(?C?InitData)
    
    ; The "?C?ZeroData" routine is responsible for initializing some global
    ; variables to 0.
    EXTRN   CODE(?C?ZeroData)

    ; The "main" routine is the main entry point for the user application
    EXTRN   CODE(main)


;-----------------------------------------------------------------------------
; Public symbols
; The symbol CST7_START is the reset entry point for the RLST7 linker.
; Several variants of this entry point may be required by your projects,
; depending of your project requirements.
; It is recommended that all these startup variants are defined as public
; symbols here.
;-----------------------------------------------------------------------------

    PUBLIC  CST7_START                  ; Minimal startup
    PUBLIC  CST7_START_V                ; Startup with global variables initialization
    PUBLIC  CST7_START_Z                ; Startup with a segment initialized to 0
    PUBLIC  CST7_START_V_Z              ; Startup with globals + initialization to 0


;=============================================================================
; Reset entry point. This is the first piece of code to get called upon
; RESET conditions
;=============================================================================

CST7_START:
CST7_START_V:
CST7_START_Z:
CST7_START_V_Z:

;-----------------------------------------------------------------------------
; Whole RAM cleanup routine (optional): Initializes the whole RAM to 00's.
; This code takes advantage of the fact that SP is at top of RAM and that all
; addresses below SP can be freely overwritten.
;-----------------------------------------------------------------------------
$IF(1)                                  ; $IF(0) will disable whole RAM cleanup
    LDW     X, SP
_ramcleanup:
    CLR     (X)
    DECW    X
    JRPL    _ramcleanup
$ELSE
    ; Comment out the following line if your project does not use any
    ; 0-initialized C global variables initialization.
    ; ?C?ZeroData must be called BEFORE ?C?InitData
    CALL    ?C?ZeroData                 ; Initialize null global variables
$ENDIF

; Comment out the following line if your project does not use any C global
; variables initialization. If you are unsure, just leave it as it is.
    CALL    ?C?InitData                 ; Initialize global variables.

    JP      main                        ; Call the user entry point (main)
                                        ; main must NEVER return!
;;; JPF     main                        ; Necessary if "main" is above address FFFFh


;=============================================================================
; Interrupt vector table (in fact just the reset vector is present here)
;=============================================================================

    CSEG    AT 08000h                   ; Interrupt vectors start at 8000h on STM8

    DB      082h, 000h                  ; The interrupt address in in 24-bits mode,
    DW      CST7_START                  ; and preceded by the 82h opcode.


END
