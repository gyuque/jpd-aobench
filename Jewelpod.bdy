[ARCH]
BODY=JEWELPOD;
SEC=RAM,0,2FFF,W;
SEC=I/O,7000,7FFF,W;
SEC=ROM,8000,FFEF,R;
SEC=Interrupt,FFF0,FFFF,R;
SEC=SysROM,10000,17FFF,W;
SEC=ReserveROM,28000,2FFFF,W;
SEC=ROM,230000,3FFFFF,R;
BANK=108,FFFF;
LOCATE=IRQVec,FFF5;
