When we compile with gcc -O2 optiomization the DW_AT_location element is a location list that contains this localation DW_OP_GNU_entry_value.

objdump --dwarf=info suspect
```
 <1><2dc>: Abbrev Number: 20 (DW_TAG_subprogram)
    <2dd>   DW_AT_external    : 1	
    <2dd>   DW_AT_name        : (indirect string, offset: 0x185): sherbrook	
    <2e1>   DW_AT_decl_file   : 1	
    <2e2>   DW_AT_decl_line   : 6	
    <2e3>   DW_AT_prototyped  : 1	
    <2e3>   DW_AT_low_pc      : 0x400690	
    <2eb>   DW_AT_high_pc     : 0x1d	
    <2f3>   DW_AT_frame_base  : 1 byte block: 9c 	(DW_OP_call_frame_cfa)
    <2f5>   DW_AT_GNU_all_call_sites: 1	
    <2f5>   DW_AT_sibling     : <0x398>	
 <2><2f9>: Abbrev Number: 21 (DW_TAG_formal_parameter)
    <2fa>   DW_AT_name        : a	
    <2fc>   DW_AT_decl_file   : 1	
    <2fd>   DW_AT_decl_line   : 6	
    <2fe>   DW_AT_type        : <0x5e>	
    <302>   DW_AT_location    : 0x0	(location list)
 <2><306>: Abbrev Number: 21 (DW_TAG_formal_parameter)
    <307>   DW_AT_name        : b	
    <309>   DW_AT_decl_file   : 1	
    <30a>   DW_AT_decl_line   : 6	
    <30b>   DW_AT_type        : <0x8b>	
    <30f>   DW_AT_location    : 0x4c	(location list)
 <2><313>: Abbrev Number: 21 (DW_TAG_formal_parameter)
    <314>   DW_AT_name        : c	
    <316>   DW_AT_decl_file   : 1	
    <317>   DW_AT_decl_line   : 6	
    <318>   DW_AT_type        : <0x91>	
    <31c>   DW_AT_location    : 0x98	(location list)
 <2><320>: Abbrev Number: 21 (DW_TAG_formal_parameter)
    <321>   DW_AT_name        : d	
    <323>   DW_AT_decl_file   : 1	
    <324>   DW_AT_decl_line   : 6	
    <325>   DW_AT_type        : <0x89>	
    <329>   DW_AT_location    : 0xe4	(location list)
```

objdump --dwarf=loc suspect
```
    00000000 0000000000400690 00000000004006a6 (DW_OP_reg5 (rdi))
    00000013 00000000004006a6 00000000004006ac (DW_OP_reg1 (rdx))
    00000026 00000000004006ac 00000000004006ad (DW_OP_GNU_entry_value: (DW_OP_reg5 (rdi)); DW_OP_stack_value)
```
http://eli.thegreenplace.net/2011/02/07/how-debuggers-work-part-3-debugging-information/
