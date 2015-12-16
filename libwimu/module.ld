EXTERN(module);
ENTRY(module);

SECTIONS {
         . = 0x00000000 + SIZEOF_HEADERS;
         .text : { KEEP(*(.text.boot)); * (.text); }
		 .rodata : { *(.rodata) }
         .data : { * (.data); }
		 .bss : { * (.bss); }
		 .rel.plt : {
			*(.rel.plt);
		 }
		 __end__ = .;
}

