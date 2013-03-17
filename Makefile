PRODUCT = "AOBench2"
OUTDIR	= .\Debug
.PRECIOUS: "$(OUTDIR)/%.asm"

OBJS = main handlers tinymath driver aobench

CC = udocc
AS = xasm16
LINKER = xlink16

INCLUDES = -I. -I"C:\Program Files\Sunplus\unSP IDE 2.2.1\library\CLib16\include"
OLEVEL   = 0
OPT      = -S -Wall -mglobal-var-iram -mISA=2.0 -O$(OLEVEL)
ASFLAGS  = -t4 -d
CASFLAGS = -t4
CFLAGS	= $(OPT) $(INCLUDES) 

"$(OUTDIR)/$(PRODUCT).bin": $(patsubst %, $(OUTDIR)/%.obj, $(OBJS)) 
	$(LINKER) $(PRODUCT).lik -body SPMC75F2313A -bfile SPMC75F2313A.cpt


"$(OUTDIR)/%.obj": "$(OUTDIR)/%.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -o $@ $<

$(OUTDIR)/%.asm: %.c
	$(CC) $(CFLAGS) -o $@ $<
	

clean:
	del udocc_error*.i
	del "$(OUTDIR)\*.obj"
	del "$(OUTDIR)\*.asm"
	del "$(OUTDIR)\$(PRODUCT).sbm"
	del "$(OUTDIR)\$(PRODUCT).map"
	del "$(OUTDIR)\$(PRODUCT).smy"
	del "$(OUTDIR)\$(PRODUCT).sym"
	del "$(OUTDIR)\$(PRODUCT).bin"
