;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; (c) Copyright IBM Corp. 2000, 2017
;;
;;  This program and the accompanying materials are made available
;;  under the terms of the Eclipse Public License v1.0 and
;;  Apache License v2.0 which accompanies this distribution.
;;
;;      The Eclipse Public License is available at
;;      http://www.eclipse.org/legal/epl-v10.html
;;
;;      The Apache License v2.0 is available at
;;      http://www.opensource.org/licenses/apache2.0.php
;;
;; Contributors:
;;    Multiple authors (IBM Corp.) - initial implementation and documentation
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

   _TEXT SEGMENT PARA USE32 PUBLIC 'CODE'

   public compareAndExchange1
   public compareAndExchange2
   public compareAndExchange4
   public compareAndExchange8b
   public _patchingFence16

; Atomically Compare And Exchange the value contained data pointed to by ptr
; with oldValue, and if equal, replace by newValue
; Return zero on failure. Nonzero otherwise.
;
; prototype: int32_t compareAndExchange1(uint8_t *ptr, uint8_t oldValue, uint8_t newValue)

compareAndExchange1 PROC NEAR
                mov        ecx, [esp + 04h]		; fetch ptr
        	mov        eax, [esp + 08h]		; fetch oldValue
        	mov        edx, [esp + 0ch]		; fetch newValue
        	lock cmpxchg byte ptr [ecx], dl
        	jnz        LfailedCMPXCHG
LpassedCMPXCHG:
        	or         eax, 1		; return nozero
        	ret
compareAndExchange1 ENDP

compareAndExchange2 PROC NEAR
        	mov        ecx, [esp + 04h]		; fetch ptr
        	mov        eax, [esp + 08h]		; fetch oldValue
        	mov        edx, [esp + 0ch]		; fetch newValue
        	lock cmpxchg word ptr [ecx], dx
        	jnz        LfailedCMPXCHG
        	jmp        LpassedCMPXCHG
compareAndExchange2 ENDP

compareAndExchange4 PROC NEAR
        	mov        ecx, [esp + 04h]		; fetch ptr
        	mov        eax, [esp + 08h]		; fetch oldValue
        	mov        edx, [esp + 0ch]		; fetch newValue
        	lock cmpxchg dword ptr [ecx], edx
        	jz         LpassedCMPXCHG
LfailedCMPXCHG:
        	xor        eax, eax
        	ret
compareAndExchange4 ENDP

; int32_t compareAndExchange8b(uint64_t *ptr, uint64_t oldValue, uint64_t newValue);

compareAndExchange8b PROC NEAR
        	push       ebx           		
        	push       esi
        	mov        esi, [esp + 0ch]		; fetch ptr
        	mov        eax, [esp + 010h]		; fetch oldValue lo
        	mov        edx, [esp + 014h]		; fetch oldValue hi
        	mov        ebx, [esp + 018h]		; fetch newValue lo
        	mov        ecx, [esp + 01ch]		; fetch newValue hi
   ifdef WINDOWS
        	lock cmpxchg8b qword ptr [esi]
   else
        	lock cmpxchg8b [esi]
   endif
        	pop        esi
        	pop        ebx
        	jz         LpassedCMPXCHG
        	xor        eax, eax
        	ret
compareAndExchange8b ENDP

; Execute the fence required to ensure patching operations are strongly ordered
; so that another processor executing the same code does not see inconsistent
; results.
;
; prototype: void _patchingFence16(void *startAddress)
;

        align   16
_patchingFence16:             ; Entry point for C code
        ret

   _TEXT ends
