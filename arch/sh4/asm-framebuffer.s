
	.globl _framebuffer_lowlevel_init, _framebuffer_check_cable, _framebuffer_clrscr

	.text

	! Clear the framebuffer
        ! r4 = pixel colour
_framebuffer_clrscr:        
        mov.l   vrambase,r0
        mov.l   clrcount,r1
clrloop:
        mov.w   r4,@r0  ! clear one pixel
        dt      r1
        bf/s    clrloop
        add     #2,r0
        rts
        nop

        .align  4
vrambase:
        .long   0xa5000000
clrcount:
        .long   640*480

        ! Set up video registers to the desired
        ! video mode (only 640*480 supported right now)
        !
        ! Note: This function does not currently initialize
        !       all registers, but assume that the boot ROM
        !       has set up reasonable defaults for syncs etc.
        !
        ! TODO: PAL
        
        ! r4 = cabletype (0=VGA, 2=RGB, 3=Composite)
        ! r5 = pixel mode (0=RGB555, 1=RGB565, 3=RGB888)
_framebuffer_lowlevel_init:
        ! Look up bytes per pixel as shift value
        mov     #3,r1
        and     r5,r1
        mova    bppshifttab,r0
        mov.b   @(r0,r1),r5
        ! Get video HW address
        mov.l   videobase,r0
        mov     #0,r2
        mov.l   r2,@(8,r0)
        add     #0x40,r0
        ! Set border colour
	mov.l	_whitecol,r2
	!mov     #0xFFFFFFFF,r2
	!mov	#0xFFFF,r2
        mov.l   r2,@r0
        ! Set pixel clock and colour mode
        shll2   r1
        mov     #240/2,r3       ! Non-VGA screen has 240 display lines
        shll    r3
        mov     #2,r2
        tst     r2,r4
        bf/s    khz15
        add     #1,r1
        shll    r3              ! Double # of display lines for VGA
        ! Set double pixel clock
        mov     #1,r2
        rotr    r2
        shlr8   r2
        or      r2,r1
khz15:  
        mov.l   r1,@(4,r0)
        ! Set video base address
        mov     #0,r1
        mov.l   r1,@(0x10,r0)
        ! Video base address for short fields should be offset by one line
        mov     #640/16,r1
        shll2   r1
        shll2   r1
        shld    r5,r1
        mov.l   r1,@(0x14,r0)
        ! Set screen size and modulo, and interlace flag
        mov.l   r4,@-r15
        mov     #1,r2
        shll8   r2
        mov     #640/16,r1
        shll2   r1
        shld    r5,r1
        mov     #2,r5
        tst     r5,r4
        bt/s    nonlace ! VGA => no interlace
        mov     #1,r4
        add     r1,r4   ! add one line to offset => display every other line
        add     #0x50,r2        ! enable LACE
nonlace:
        shll8   r4
        shll2   r4
        add     r3,r4
        add     #-1,r4
        shll8   r4
        shll2   r4
        add     r1,r4
        add     #-1,r4
        mov.l   r4,@(0x1c,r0)
        mov.l   @r15+,r4
        add     #0x7c,r0
        mov.l   r2,@(0x14,r0)
        ! Set vertical pos and border
        mov     #36,r1
        mov     r1,r2
        shll16  r1
        or      r2,r1
        mov.l   r1,@(0x34,r0)
        add     r3,r1
        mov.l   r1,@(0x20,r0)
        ! Horizontal pos
        mov.w   hpos,r1
        mov.l   r1,@(0x30,r0)
        
        ! Select RGB/CVBS
        mov.l   cvbsbase,r1
        rotr    r4
        bf/s    rgbmode
        mov     #0,r0
        mov     #3,r0
rgbmode:
        shll8   r0
        mov.l   r0,@r1
        
        rts
        nop

        .align  4
_whitecol:	.long	0xffffffff
videobase:
        .long   0xa05f8000
cvbsbase:
        .long   0xa0702c00
bppshifttab:
        .byte   1,1,0,2
hpos:
        .word   0xa4

        ! Check type of A/V cable connected
        !
        ! 0 = VGA
        ! 1 = ---
        ! 2 = RGB
        ! 3 = Composite

_framebuffer_check_cable:
        ! set PORT8 and PORT9 to input
        mov.l   porta,r0
        mov.l   pctra_clr,r2
        mov.l   @r0,r1
        mov.l   pctra_set,r3
        and     r2,r1
        or      r3,r1
        mov.l   r1,@r0
        ! read PORT8 and PORT9
        mov.w   @(4,r0),r0
        shlr8   r0
        rts
        and     #3,r0

        .align 4
porta:
        .long   0xff80002c
pctra_clr:
        .long   0xfff0ffff
pctra_set:
        .long   0x000a0000

