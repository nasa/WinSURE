# 1 "prseval.c"
 





 














# 1 "lib_def.h" 1
 











# 22 "prseval.c" 2

# 1 "cm_sys.h" 1
 




























 



 

# 1 "cm_stdio.h" 1
 











 

                    
                    
                    
                    
                    
                    
                    

# 1 "/usr/local/lib/gcc-lib/sparc/2.1/include/stdio.h" 1 3
 




extern	struct	_iobuf {
	int	_cnt;
	unsigned char *_ptr;
	unsigned char *_base;
	int	_bufsiz;
	short	_flag;
	char	_file;		 
} _iob[];








































extern struct _iobuf 	*fopen();
extern struct _iobuf 	*fdopen();
extern struct _iobuf 	*freopen();
extern struct _iobuf 	*popen();
extern struct _iobuf 	*tmpfile();
extern long	ftell();
extern char	*fgets();
extern char	*gets();
extern char	*sprintf();
extern char	*ctermid();
extern char	*cuserid();
extern char	*tempnam();
extern char	*tmpnam();






# 23 "cm_stdio.h" 2

# 1 "/usr/local/lib/gcc-lib/sparc/2.1/include/math.h" 1 3
 

 



 






 





extern double	__infinity();
extern double	acos( );
extern double	asin( );
extern double	atan( );
extern double	atan2( );
extern double	ceil( );
extern double	cos( );
extern double	cosh( );
extern double	exp( );
extern double	fabs( );
extern double	floor( );
extern double	fmod( );
extern double	frexp( );
extern double	ldexp( );
extern double	log( );
extern double	log10( );
extern double	modf( );
extern double	pow( );
extern double	sin( );
extern double	sinh( );
extern double	sqrt( );
extern double	tan( );
extern double	tanh( );


# 1 "/usr/include/floatingpoint.h" 1 3
 

 



 










# 1 "/usr/local/lib/gcc-lib/sparc/2.1/include/sys/ieeefp.h" 1 3
 

 



 







 


enum fp_direction_type 		 
	{
	fp_nearest	= 0,
	fp_tozero	= 1,
	fp_positive	= 2,
	fp_negative	= 3
	} ;

# 34 "/usr/local/lib/gcc-lib/sparc/2.1/include/sys/ieeefp.h" 3

# 43 "/usr/local/lib/gcc-lib/sparc/2.1/include/sys/ieeefp.h" 3


# 53 "/usr/local/lib/gcc-lib/sparc/2.1/include/sys/ieeefp.h" 3

enum fp_precision_type		 
	{
	fp_extended	= 0,
	fp_single	= 1,
	fp_double	= 2,
	fp_precision_3	= 3
	} ;


# 73 "/usr/local/lib/gcc-lib/sparc/2.1/include/sys/ieeefp.h" 3

enum fp_exception_type		 
	{
	fp_inexact	= 0,
	fp_division	= 1,
	fp_underflow	= 2,
	fp_overflow	= 3,
	fp_invalid	= 4
	} ;


enum fp_class_type		 
	{
	fp_zero		= 0,
	fp_subnormal	= 1,
	fp_normal	= 2,
	fp_infinity   	= 3,
	fp_quiet	= 4,
	fp_signaling	= 5
	} ;


# 18 "/usr/include/floatingpoint.h" 2 3


 

typedef float   single;
typedef unsigned long extended[3];	 

typedef long double quadruple;	 








typedef unsigned fp_exception_field_type;
 



typedef int     sigfpe_code_type;	 

typedef void    (*sigfpe_handler_type) ();
 





 

extern enum fp_direction_type fp_direction;
 



extern enum fp_precision_type fp_precision;
 



extern fp_exception_field_type fp_accrued_exceptions;
 





 


 

typedef char    decimal_string[512 ];
 

typedef struct {
	enum fp_class_type fpclass;
	int             sign;
	int             exponent;
	decimal_string  ds;	 


	int             more;	 


	int             ndigits; 


}
                decimal_record;

enum decimal_form {
	fixed_form,		 



	floating_form		 

};

typedef struct {
	enum fp_direction_type rd;
	 
	enum decimal_form df;	 
	int             ndigits; 
}
                decimal_mode;

enum decimal_string_form {	 
	invalid_form,		 
	whitespace_form,	 
	fixed_int_form,		 
	fixed_intdot_form,	 
	fixed_dotfrac_form,	 
	fixed_intdotfrac_form,	 
	floating_int_form,	 
	floating_intdot_form,	 
	floating_dotfrac_form,	 
	floating_intdotfrac_form,	 
	inf_form,		 
	infinity_form,		 
	nan_form,		 
	nanstring_form		 
};

 

extern void     double_to_decimal();
extern void     quadruple_to_decimal();
extern char    *econvert();
extern char    *fconvert();
extern char    *gconvert();
extern char    *qeconvert();
extern char    *qfconvert();
extern char    *qgconvert();

 



extern sigfpe_handler_type ieee_handlers[5	];
 






extern sigfpe_handler_type sigfpe();

extern void     single_to_decimal();
extern void     extended_to_decimal();

extern void     decimal_to_single();
extern void     decimal_to_double();
extern void     decimal_to_extended();
extern void     decimal_to_quadruple();

extern char    *seconvert();
extern char    *sfconvert();
extern char    *sgconvert();

extern void     string_to_decimal();
extern void     file_to_decimal();
extern void     func_to_decimal();

 

extern double   atof();

 

extern int      errno;

extern double   strtod();


# 47 "/usr/local/lib/gcc-lib/sparc/2.1/include/math.h" 2 3

extern double	acosh();
extern double	asinh();
extern double	atanh();
extern double	cbrt();
extern double	copysign();
extern double	erf();
extern double	erfc();
extern double	expm1();
extern int	finite();
extern double	hypot();
extern double	j0();
extern double	j1();
extern double	jn();
extern double	lgamma();
extern double	log1p();
extern double	rint();
extern double	y0();
extern double	y1();
extern double	yn();

 



 


enum fp_pi_type {
	fp_pi_infinite	= 0,	 
	fp_pi_66	= 1,	 
	fp_pi_53	= 2	 
};

 


extern enum	fp_pi_type fp_pi;

 


extern enum	fp_class_type fp_class();
extern int	ieee_flags();
extern int	ieee_handler();
extern void	ieee_retrospective();
extern int	ilogb();
extern double	infinity();
extern int	irint();
extern int	isinf();
extern int	isnan();
extern int	isnormal();
extern int	issubnormal();
extern int	iszero();
extern double	logb();
extern double	max_normal();
extern double	max_subnormal();
extern double	min_normal();
extern double	min_subnormal();
extern double	nextafter();
extern void	nonstandard_arithmetic();
extern double	quiet_nan();
extern double	remainder();
extern double	scalb();
extern double	scalbn();
extern double	signaling_nan();
extern int	signbit();
extern double	significand();
extern void	standard_arithmetic();

 


extern double	acospi();
extern double	aint();
extern double	anint();
extern double	annuity();
extern double	asinpi();
extern double	atan2pi();
extern double	atanpi();
extern double	compound();
extern double	cospi();
extern double	exp10();
extern double	exp2();
extern double	log2();
extern int	nint();
extern void	sincos();
extern void	sincospi();
extern double	sinpi();
extern double	tanpi();
extern int	matherr();


 














 







 







 







 




extern int		 ir_finite_();
extern enum fp_class_type ir_fp_class_();
extern int		 ir_ilogb_();
extern int		 ir_irint_();
extern int		 ir_isinf_();
extern int		 ir_isnan_();
extern int		 ir_isnormal_();
extern int		 ir_issubnormal_();
extern int		 ir_iszero_();
extern int		 ir_nint_();
extern int		 ir_signbit_();
extern void		 r_sincos_();
extern void		 r_sincospi_();
extern double  r_acos_();
extern double  r_acosh_();
extern double  r_acospi_();
extern double  r_aint_();
extern double  r_anint_();
extern double  r_annuity_();
extern double  r_asin_();
extern double  r_asinh_();
extern double  r_asinpi_();
extern double  r_atan2_();
extern double  r_atan2pi_();
extern double  r_atan_();
extern double  r_atanh_();
extern double  r_atanpi_();
extern double  r_cbrt_();
extern double  r_ceil_();
extern double  r_compound_();
extern double  r_copysign_();
extern double  r_cos_();
extern double  r_cosh_();
extern double  r_cospi_();
extern double  r_erf_();
extern double  r_erfc_();
extern double  r_exp10_();
extern double  r_exp2_();
extern double  r_exp_();
extern double  r_expm1_();
extern double  r_fabs_();
extern double  r_floor_();
extern double  r_fmod_();
extern double  r_hypot_();
extern double  r_infinity_();
extern double  r_j0_();
extern double  r_j1_();
extern double  r_jn_();
extern double  r_lgamma_();
extern double  r_log10_();
extern double  r_log1p_();
extern double  r_log2_();
extern double  r_log_();
extern double  r_logb_();
extern double  r_max_normal_();
extern double  r_max_subnormal_();
extern double  r_min_normal_();
extern double  r_min_subnormal_();
extern double  r_nextafter_();
extern double  r_pow_();
extern double  r_quiet_nan_();
extern double  r_remainder_();
extern double  r_rint_();
extern double  r_scalb_();
extern double  r_scalbn_();
extern double  r_signaling_nan_();
extern double  r_significand_();
extern double  r_sin_();
extern double  r_sinh_();
extern double  r_sinpi_();
extern double  r_sqrt_();
extern double  r_tan_();
extern double  r_tanh_();
extern double  r_tanpi_();
extern double  r_y0_();
extern double  r_y1_();
extern double  r_yn_();

 












struct exception {
	int type;
	char *name;
	double arg1;
	double arg2;
	double retval;
};

 





























extern int	signgam;
 




extern double cabs();	 



extern double drem();	 


extern double gamma();	 






# 24 "cm_stdio.h" 2

# 1 "/usr/local/lib/gcc-lib/sparc/2.1/include/string.h" 1 3
 




# 1 "/usr/local/lib/gcc-lib/sparc/2.1/include/sys/stdtypes.h" 1 3
 

 










typedef	int		sigset_t;	 

typedef	unsigned int	speed_t;	 
typedef	unsigned long	tcflag_t;	 
typedef	unsigned char	cc_t;		 
typedef	int		pid_t;		 

typedef	unsigned short	mode_t;		 
typedef	short		nlink_t;	 

typedef	long		clock_t;	 
typedef	long		time_t;		 



typedef	int		size_t;		 



typedef int		ptrdiff_t;	 




typedef	unsigned short	wchar_t;	 



# 6 "/usr/local/lib/gcc-lib/sparc/2.1/include/string.h" 2 3







extern char *	strcat( );
extern char *	strchr( );
extern int	strcmp( );
extern char *	strcpy( );
extern size_t	strcspn( );

extern char *	strdup( );

extern size_t	strlen( );
extern char *	strncat( );
extern int	strncmp( );
extern char *	strncpy( );
extern char *	strpbrk( );
extern char *	strrchr( );
extern size_t	strspn( );
extern char *	strstr( );
extern char *	strtok( );


# 25 "cm_stdio.h" 2

# 1 "/usr/include/ctype.h" 1 3
 











extern	char	_ctype_[];














# 26 "cm_stdio.h" 2

# 1 "/usr/include/errno.h" 1 3
 

 






# 1 "/usr/include/sys/errno.h" 1 3
 

 




 







































 



 



 

	 













	 













	 



 




 




 



 






 


 



 











 



# 10 "/usr/include/errno.h" 2 3

extern int errno;


# 27 "cm_stdio.h" 2

















# 1 "/usr/local/lib/gcc-lib/sparc/2.1/include/sys/types.h" 1 3
 

 








 






# 1 "/usr/include/sys/sysmacros.h" 1 3
 

 






 


 


 



# 19 "/usr/local/lib/gcc-lib/sparc/2.1/include/sys/types.h" 2 3





typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;
typedef	unsigned short	ushort;		 
typedef	unsigned int	uint;		 















typedef	struct  _physadr_t { int r[1]; } *physadr_t;
typedef	struct label_t {
	int	val[2];
} label_t;







typedef	struct	_quad_t { long val[2]; } quad_t;
typedef	long	daddr_t;
typedef	char *	caddr_t;
typedef	unsigned long	ino_t;
typedef	short	dev_t;
typedef	long	off_t;
typedef	unsigned short	uid_t;
typedef	unsigned short	gid_t;
typedef	long	key_t;
typedef	char *	addr_t;




 









typedef	long	fd_mask;









typedef	struct fd_set {
	fd_mask	fds_bits[(((256 )+(( (sizeof (fd_mask) * 8		)	)-1))/( (sizeof (fd_mask) * 8		)	)) ];
} fd_set;







# 113 "/usr/local/lib/gcc-lib/sparc/2.1/include/sys/types.h" 3



# 44 "cm_stdio.h" 2

# 1 "/usr/include/sys/stat.h" 1 3
 

 








struct	stat {
	dev_t	st_dev;
	ino_t	st_ino;
	mode_t	st_mode;
	short	st_nlink;
	uid_t	st_uid;
	gid_t	st_gid;
	dev_t	st_rdev;
	off_t	st_size;
	time_t	st_atime;
	int	st_spare1;
	time_t	st_mtime;
	int	st_spare2;
	time_t	st_ctime;
	int	st_spare3;
	long	st_blksize;
	long	st_blocks;
	long	st_spare4[2];
};






















































int	chmod( );
int	fstat( );
int	mkdir( );
int	mkfifo( );
int	stat( );
mode_t	umask( );



# 45 "cm_stdio.h" 2






# 61 "cm_stdio.h"

# 1 "/usr/local/lib/gcc-lib/sparc/2.1/include/stdlib.h" 1 3
 

 








extern unsigned int _mb_cur_max;




 
extern void	abort( );
extern int	abs( );
extern double	atof( );
extern int	atoi( );
extern long int	atol( );
extern char *	bsearch( 
);
extern char *	calloc( );
extern void	exit( );
extern int	free( );
extern char *	getenv( );
extern char *	malloc( );
extern int	qsort( 
);
extern int	rand( );
extern char *	realloc( );
extern int	srand( );

extern int    mbtowc( );
extern int    wctomb( );
extern size_t mbstowcs( );
extern size_t wcstombs( );


# 62 "cm_stdio.h" 2





# 1 "/usr/local/lib/gcc-lib/sparc/2.1/include/values.h" 1 3
 




 


 







 




 




 





































# 78 "/usr/local/lib/gcc-lib/sparc/2.1/include/values.h" 3

# 89 "/usr/local/lib/gcc-lib/sparc/2.1/include/values.h" 3






























# 67 "cm_stdio.h" 2

# 1 "x_stdio.h" 1
 







extern int fprintf();
extern int printf();


# 33 "x_stdio.h"

extern void fflush(struct _iobuf  *);
extern int system(char *);
extern int fputc(int,struct _iobuf  *);
extern long strtol(char *,char **,int);
extern int toupper(int);
extern int memcmp(const void *,const void *,size_t);
extern void *memcpy(void *,const void *,size_t);
extern void *memmove(void *,const void *,size_t);
extern char *memset(void *,int,size_t);
extern int fclose(struct _iobuf  *);
extern int setvbuf(struct _iobuf  *,char *,int,size_t);
extern int remove(char *);
extern void _flsbuf(unsigned char,struct _iobuf  *);
extern int _filbuf(struct _iobuf  *);
extern int fgetc(struct _iobuf  *);
extern int fseek(struct _iobuf  *,long,int);
extern void rewind(struct _iobuf  *);
extern size_t fread(void *,size_t,size_t,struct _iobuf  *);
extern size_t fwrite(void *,size_t,size_t,struct _iobuf  *);
extern void cfree(void *);





# 68 "cm_stdio.h" 2





 

# 36 "cm_sys.h" 2


# 1 "lib_def.h" 1
 











# 38 "cm_sys.h" 2

# 1 "../lib/commonio.h" 1
 







typedef unsigned char Boolean;




typedef unsigned char flagtyp;





extern short siglen(char *,short);
extern flagtyp safe_strcat_truncated(char *,char *,int);






typedef char *charptr;


# 39 "cm_sys.h" 2





 
 
 
 
 
 
 


                     
                     
                     
                     
                     
                     
                     





                  
                  
                  
                  
                  
                  
                  






 






 






 





 
# 113 "cm_sys.h"




 





 








              
              
              
              
              
              
              

 



 




typedef unsigned char unsigned_255_type;  

typedef unsigned char unsigned_31_type;   

# 162 "cm_sys.h"

extern unsigned short masks[];


 








typedef double real_type;




 













typedef long int_type;


 















# 221 "cm_sys.h"


 


# 1 "/usr/local/lib/gcc-lib/sparc/2.1/include/sys/unistd.h" 1 3
 




 


































 







 





 



















extern void	_exit( );
extern int	access( );
extern unsigned	alarm( );
extern int	chdir( );
extern int	chmod( );
extern int	chown( );
extern int	close( );
extern char	*ctermid( );
extern char	*cuserid( );
extern int	dup( );
extern int	dup2( );
extern int	execl( );
extern int	execle( );
extern int	execlp( );
extern int	execv( );
extern int	execve( );
extern int	execvp( );
extern pid_t	fork( );
extern long	fpathconf( );
extern char	*getcwd( );
extern gid_t	getegid( );
extern uid_t	geteuid( );
extern gid_t	getgid( );
extern int	getgroups( );
extern char	*getlogin( );
extern pid_t	getpgrp( );
extern pid_t	getpid( );
extern pid_t	getppid( );
extern uid_t	getuid( );
extern int	isatty( );
extern int	link( );
extern off_t	lseek( );
extern long	pathconf( );
extern int	pause( );
extern int	pipe( );
extern int	read( );
extern int	rmdir( );
extern int	setgid( );
extern int	setpgid( );
extern pid_t	setsid( );
extern int	setuid( );
extern unsigned	sleep( );
extern long	sysconf( );
extern pid_t	tcgetpgrp( );
extern int	tcsetpgrp( );
extern char	*ttyname( );
extern int	unlink( );
extern int	write( );



# 226 "cm_sys.h" 2








 






typedef struct stat stat_t;













             
             
             
             
             
             
             
             














































             
             
             
             
             
             
             
             





















               
               
               
               
               
               
               
               
               










               
               
               
               
               
               
               
               
               
               












typedef int Memix;      
typedef int Stacktop;   
typedef int File_offset_t;
typedef unsigned int memsize_t;



























                   
                   
                   
                   
                   
                   
                   
























                 
                 
                 
                 
                 
                 
                 







                 
                 
                 
                 
                 
                 
                 




















    
    
    
    
    
    
    
















   
   
   
   
   
   
   




                    
                    
                    
                    
                    
                    
                    














typedef char pathname_type[130 ];





extern long my_clock(void);





 

 

# 23 "prseval.c" 2

# 1 "cm_defs.h" 1


 




# 1 "../lib/commonio.h" 1
 

# 30 "../lib/commonio.h"

# 8 "cm_defs.h" 2












































               
               
               
               
               
               
               

 















             
             
             
             
             
             
             










                  
                  
                  
                  
                  
                  
                  





















                             


        
        
        
        
        
        
        
















      
      
      
      
      
      
      




                    
                    
                    
                    
                    
                    
                    



 











 
 





                    
                    
                    
                    
                    
                    
                    






 
 
 
 
 
 
 

       
       
       
       
       
       
       





 

# 24 "prseval.c" 2

# 1 "cm_types.h" 1


 


# 1 "cm_sys.h" 1
 


























# 552 "cm_sys.h"

# 6 "cm_types.h" 2


# 1 "cm_defs.h" 1
# 221 "cm_defs.h"

# 8 "cm_types.h" 2


typedef unsigned long relative_address_type;
typedef unsigned short bitsize_type;

typedef unsigned char Utiny;


typedef unsigned_255_type opcode_type;
typedef unsigned_255_type token;               
typedef unsigned_255_type rwtype;              
typedef unsigned_255_type mode_flag_type;      


typedef unsigned char *state_bitstring_type;

typedef short Subscript;


typedef unsigned_255_type type_flagword_type;        
typedef unsigned_255_type debug_flag_type;           
typedef unsigned_255_type ident_usage_flag_type;     


typedef struct t__scanning_character_info
   {
        short current_ch_lno;
        short lookahead_ch_lno;
        short current_ch_pos;
        short lookahead_ch_pos;
        char current_ch;
        char lookahead_ch;
   } scanning_character_info_type;


typedef short ssvar_value_type;
typedef struct t__state_offset
   {                                
      ssvar_value_type minval;      
      ssvar_value_type maxval;      
      bitsize_type bit_offset;      
      bitsize_type bit_length;      
   } state_offset_type;














 

 
typedef union t__pointer_union
   {
     relative_address_type relative_address;
     short parameter_count;
     void *vvv;                
     Boolean *bbb;             
     state_offset_type *sss;   
     char *ccc;                
     int_type *iii;            
     real_type *rrr;           
   } pointer_union_type;

typedef union t__value_union
   {
     Boolean bbb;             
     char ccc;                
     int_type iii;            
     real_type rrr;           
     state_offset_type sss;   
     pointer_union_type ptr;  

     struct qqiiis{
        int_type iiia;
        int_type iiib;} pair;

   } value_union_type;










typedef struct t__dim_pair
  {
    Subscript lower; 
    Subscript upper; 
  } dim_pair_type;













typedef struct t__identifier_info
  {
     pointer_union_type ptr;     
     union
        {
           struct qqbothidinfqq
              {
                 dim_pair_type first;
                 dim_pair_type second;
              } dims;
           dim_pair_type body;
        } index;
     short scope_level;          
     char name[((28                    +3)    +1)  ];  
     type_flagword_type flags;   
  } identifier_info_type;

typedef struct t__token_lookup
   {
     char token[3];
     token value;
   } token_lookup_type;

typedef struct t__rw_operator_lookup
   {
      rwtype rwsrc;
      token tokdest;
   } rw_operator_lookup_type;

typedef struct t__reserved_word_lookup
   {
      char text[15];
      rwtype rw;
   } reserved_word_lookup_type;

typedef struct t__token_info
   {
      identifier_info_type *id_info_ptr;
      short linnum;
      short pos;
      token tok;
      rwtype rw;
      char id[((28                    +3)    +1)  ];
   } token_info_type;

typedef identifier_info_type *idinf_p_type;



typedef idinf_p_type (*any_searchid_routine)(char *,type_flagword_type,short);

 

 

# 25 "prseval.c" 2

# 1 "cm_vars.h" 1


 

# 1 "lib_def.h" 1
 











# 5 "cm_vars.h" 2


# 1 "cm_defs.h" 1
# 221 "cm_defs.h"

# 7 "cm_vars.h" 2

# 1 "cm_types.h" 1
# 176 "cm_types.h"

# 8 "cm_vars.h" 2


extern  char date_string[12];
extern  char time_string[20];

                             
                             
                             
                             
                             
                             
                             

extern  struct _iobuf  *inputfile;   
extern  struct _iobuf  *outputfile;  
extern  struct _iobuf  *errorfile;   
extern  struct _iobuf  *modfile;     
extern  struct _iobuf  *listfile;    
extern  struct _iobuf  *objectfile;  
extern  struct _iobuf  *xreffile;    
extern  struct _iobuf  *exefile;     
extern  struct _iobuf  *scrfile;     
extern  struct _iobuf  *bugfile;     

extern  pathname_type filename_prefix;
extern  pathname_type input_filename;
extern  pathname_type output_filename;
extern  pathname_type mod_filename;
 
extern  pathname_type list_filename;
extern  pathname_type object_filename;
extern  pathname_type xref_filename;
extern  pathname_type exe_filename;
extern  pathname_type tempinputfilename[2 ];


                            
                            
                            
                            
                            
                            
                            

extern  char maxint_str[12 ];
extern  char neg_maxint_str[12 ];
extern  int_type maxint;
extern  real_type maxreal;
extern  real_type ln_maxreal;
extern  real_type ln_maxint;
extern  value_union_type std_real_value;
extern  value_union_type std_null_value;

                     
                     
                     
                     
                     
                     
                     

extern  Memix nele_identifier_table;
extern  Memix nele_ident_at_table_top;
extern  identifier_info_type *identifier_table;
extern  memsize_t identifier_table_bytes;
 
extern  Memix nele_constant_table;
extern  char *constant_table;
extern  memsize_t constant_table_bytes;
extern  ident_usage_flag_type *usage_table;

 

# 26 "prseval.c" 2

# 1 "cm_prsvars.h" 1


 

# 1 "lib_def.h" 1
 











# 5 "cm_prsvars.h" 2


# 1 "cm_types.h" 1
# 176 "cm_types.h"

# 7 "cm_prsvars.h" 2


extern  char unbracketed_err_word[6];    
extern  char unbracketed_wrn_word[10];   
extern  char unbracketed_svr_word[10];   
extern  char warning_spacing_char;
extern  short nesting_scope_level;
extern  debug_flag_type debug_flagword;
extern  char scratch_string_buffer[(1025 +1)  ];
extern  char os_malloc_limit_msg[60];
extern  int_type eval_sub_infix_expr_ans[3];
extern  short ee_oix_out,ee_ix_out;
extern  value_union_type ee_aux_val;

                  
                  
                  
                  
                  
                  
                  





extern  Boolean skipping_inside_comment; 
extern  short arrow_position;            

                       
                       
                       
                       
                       
                       
                       

extern char language[];               
extern short error_count;             
extern Boolean end_of_file_reached;   
extern Boolean capise_mode;           
extern short max_input_line_length;   
extern short max_output_line_length;  
extern short half_output_line_length; 

 

# 27 "prseval.c" 2

# 1 "pass.h" 1



# 1 "cm_types.h" 1
# 176 "cm_types.h"

# 4 "pass.h" 2


extern char encoded_state_string[];
extern reserved_word_lookup_type reserved_word_lookup_table[];
extern char tempnam_buffer[];

extern char encoded_aux_val[];
extern char enk_5_or_more_str[];


# 28 "prseval.c" 2


# 1 "tokdefs.h" 1
                          
                          
                          
                          
                          
                          
                          



 


 



 







 














 




















 









 




 

# 30 "prseval.c" 2

# 1 "rwdefs.h" 1

                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    



 
 






 
























# 59 "rwdefs.h"


 







 







 



 
 














 



 

 

# 31 "prseval.c" 2

# 1 "errdefs.h" 1


 




















































































































































































 

# 32 "prseval.c" 2

# 1 "prsdefs.h" 1


 






                 
                 
                 
                 
                 
                 
                 





 































 

 




 


 


 








 




 














 












 











 

 

 























 

















 














 






 

 










 





 

# 33 "prseval.c" 2

# 1 "prstypes.h" 1


 

# 1 "lib_def.h" 1
 











# 5 "prstypes.h" 2

# 1 "cm_types.h" 1
# 176 "cm_types.h"

# 6 "prstypes.h" 2

# 1 "prsdefs.h" 1
# 209 "prsdefs.h"

# 7 "prstypes.h" 2


typedef unsigned_255_type operation_type;
typedef identifier_info_type *operand_type;

typedef struct t__expression
   {
       operation_type *postfix_ops;    
       operation_type *infix_ops;      
       operand_type *operands;         
       short n_postfix_ops;            
       short n_infix_ops;              
       short n_operands;               
       short source_code_line_number;  
       Boolean in_error;               
       type_flagword_type rtntype;     
   } expression_type;


typedef struct t__binary_operand_item_info
   {
      short ixpo;
      short ixin;
      type_flagword_type type;
      type_flagword_type comp;
      type_flagword_type spec;
   } binary_operand_item_info_type;

typedef struct t__binary_operand_pair_info
   {
      binary_operand_item_info_type item[2];
      type_flagword_type ans;
      type_flagword_type spcans;
   } binary_operand_pair_info_type;

typedef struct t__built_in_parm_info
   {
       short parameter_count;
       type_flagword_type parameter_type;
       type_flagword_type return_type;
       operation_type opcode;
       operation_type aux_opcode;
       char label[16];
   } built_in_parm_info_type;


typedef struct t__macro_expansion_info
   {
       token_info_type *passed_token_list;
       unsigned short *passed_token_offset;
       unsigned short *passed_token_counts;
       short now_passed_ix;
       short now_passed_count;
       short passed_parameter_count;
       short ini_body_ix;
       short now_body_ix;
       short ovf_body_ix;
       short pos;
       short linnum;
   } macro_expansion_info_type;

# 76 "prstypes.h"

typedef void (*any_void_routine)(void);
typedef void (*any_flagword_routine)(type_flagword_type);
typedef void (*any_parse_statement_routine)(void);
typedef type_flagword_type (*any_parse_routine)(void);
typedef Boolean (*any_is_op_routine)(token);
typedef operation_type (*any_give_op_routine)(token);
typedef void (*any_promote_routine)
    (binary_operand_pair_info_type *,short,operation_type,token);


 

# 34 "prseval.c" 2

# 1 "prsvars.h" 1


 
 
# 1 "lib_def.h" 1
 











# 5 "prsvars.h" 2

# 1 "prstypes.h" 1
# 88 "prstypes.h"

# 6 "prsvars.h" 2

 
extern  char operation_text[4*256];
extern  void *full_storage;
extern  Boolean *garbage_postfix_ops;
extern  Boolean *garbage_infix_ops;
extern  Boolean *garbage_operands;
extern  unsigned short *offset_storage;
extern  unsigned short *counts_storage;
extern  token_info_type this,prev,ago;
extern  expression_type this_expr;
extern  operand_type *this_expr_operands_qq;
extern  operation_type *this_expr_ops_qq;
 
extern  Memix counts_offset_storage_count;
extern  state_bitstring_type eval_state_ssbits;
extern  token last_gotten_token;
extern  identifier_info_type *last_gotten_idinfo;
 
extern  Boolean parsing_convar_def;
extern  Boolean this_looked_up;
 
extern  short operand_max_size_used;
extern  short infix_max_size_used;
extern  short postfix_max_size_used;
 

# 35 "prseval.c" 2


# 1 "str_ext.h" 1


 

# 1 "../lib/commonio.h" 1
 

# 30 "../lib/commonio.h"

# 5 "str_ext.h" 2




# 1 "cm_types.h" 1
# 176 "cm_types.h"

# 9 "str_ext.h" 2




              
              
              
              
              
              
              








extern void init_strlib(void);
extern void re_init_strlib(void);
extern int i_strcmp(char *,char *);
extern int i_strncmp(char *,char *,int);
extern void capise_string(char *);






extern void append_suffix_to_prefix(char *,char *,char *);
extern void append_suffix_to_filename(char *,char *);


# 80 "str_ext.h"

 

extern Boolean string_is_too_big_int_type(char *);
extern Boolean is_a_blank_line(char *);



extern Boolean ident_valid_character(char);
extern Boolean exponent_valid_character(char);
extern void value_to_memory
       (pointer_union_type *,value_union_type *,type_flagword_type);
extern size_t type_size(type_flagword_type);
extern void standard_value(value_union_type *,type_flagword_type);
extern void *arr_addr(void *,short,type_flagword_type);
extern char *enk_value_union_val
       (value_union_type *,type_flagword_type,state_bitstring_type);
extern char *enk_aux_val(void *,type_flagword_type);
extern char *enk_aux_val_just(void *,type_flagword_type);
extern char *enk_ptr_union_value(pointer_union_type *,type_flagword_type,
                                 int_type,state_bitstring_type);
extern Boolean ssvar_vs_range_err(ssvar_value_type,state_offset_type *);
extern void encode_warning_and_severity(short,char *);
extern void encode_arrow_to_error(char *,short *,short *,Boolean,Boolean);
extern int_type decode_integer(char *);
extern real_type decode_real(char *);
extern short translate_tab_characters(char *buffer,short nch);
extern short strip_off_trailing_space(char *buffer);
extern void make_promotion_message
       (char *,type_flagword_type,type_flagword_type);
extern void make_symbolic_type_list(char *,type_flagword_type);
extern void fprintf_hex_pointer(struct _iobuf  *,void *);
extern void fprintf_hex_reladdr(struct _iobuf  *,relative_address_type);
extern void fprintf_hex_memix(struct _iobuf  *,Memix);
extern char *enk_int_5_wide(int_type);

extern void slow_memmove(char *,char *,int);

extern void init_error_words(short,Boolean,Boolean);
extern Boolean remove_extent(char *,char *);

 

                    
                    
                    
                    
                    
                    
                    



 



 



 




 

# 37 "prseval.c" 2

# 1 "lio_ext.h" 1


 

# 1 "../lib/commonio.h" 1
 

# 30 "../lib/commonio.h"

# 5 "lio_ext.h" 2


# 17 "lio_ext.h"

extern void check_errno(short,char *);
extern struct _iobuf  *open_input_file(char *);
extern struct _iobuf  *open_output_file(char *);
extern struct _iobuf  *open_binary_input_file(char *);
extern struct _iobuf  *open_binary_output_file(char *);
extern flagtyp exists(char *);
extern flagtyp okay_to_read(char *);
extern flagtyp okay_to_write(char *);
extern flagtyp okay_to_delete(char *);







extern relative_address_type relative_identifier(identifier_info_type *);
extern identifier_info_type *absolute_identifier(relative_address_type);



 

# 38 "prseval.c" 2

# 1 "pio_ext.h" 1


 

# 1 "cm_types.h" 1
# 176 "cm_types.h"

# 5 "pio_ext.h" 2


# 32 "pio_ext.h"

extern void init_prsiolib(scanning_character_info_type *);
extern void restore_prsiolib(scanning_character_info_type *);
extern struct _iobuf  *open_input_using_big_buffer(char *);
extern struct _iobuf  *open_output_using_big_buffer(char *,size_t);
extern void free_big_input_buffer(void);
extern void safe_fprintf(struct _iobuf  *,char *);
extern void make_temporary_filename(char *);
extern void cleanup_temporary_filename(char *);
extern short adjust_wrapping_column(struct _iobuf  *,short,short,short,short,short *);
extern Boolean two_in_a_row(char,char);
extern void make_two_in_a_row(char *);
extern token get_ident(struct _iobuf  *,char *);
extern token get_digit_string(struct _iobuf  *,char *,Boolean);
extern type_flagword_type get_literal_text(struct _iobuf  *,char *,char);
extern void advance_1_char(void);
extern Boolean advance_over_spacing_eof(void);
extern Boolean skip_over_comment(char,char,short);
extern void show_err_at_pos_on_both(short,short,void *,type_flagword_type);
extern void show_wrn_at_pos_on_both(short,short,void *,type_flagword_type,
                                    short);
extern void setup_for_tail_msg(void *,type_flagword_type,char *);
extern void show_tail_err_on_both(short,void *,type_flagword_type,char *);
extern void show_tail_wrn_on_both
                (short,void *,type_flagword_type,char *,short);
extern void show_tail_err_at_pos_on_both
       (short,short,void *,type_flagword_type,char *);
extern void show_tail_wrn_at_pos_on_both
       (short,short,void *,type_flagword_type,short,char *);

 




 

# 39 "prseval.c" 2

# 1 "lex_ext.h" 1


 

# 1 "cm_types.h" 1
# 176 "cm_types.h"

# 5 "lex_ext.h" 2


# 68 "lex_ext.h"

extern void init_lexlib(reserved_word_lookup_type *);
extern void re_init_lexlib(void);
extern Memix set_idn_num_counts(Memix,Memix);
extern void lookup_as_symbolic_token(char *);
extern token lookup_operator_token(char *);
extern token give_rw_operation_token(rwtype);
extern int_type decode_expected_integer(char *,token,int_type);
extern real_type decode_expected_real(char *,token,real_type);
extern void decode_number(token,char *,int_type *,real_type *);
extern rwtype which_reserved_word(char *,Boolean);
extern token load_char_string_token(struct _iobuf  *,char *,char *,char);
extern void interpret_alpha_token(char *,token *,rwtype *);
extern token gettoken_only(char *,char *,rwtype *,short *,short *);
extern token dumtoken(rwtype *,char *,short *,short *,type_flagword_type);
extern token gettoken_work(rwtype *,char *,short *,short *);
extern void showtoken(token,rwtype,char *);
extern void allocate_identifier_table(Memix);
extern void free_identifier_table(void);
extern identifier_info_type *next_id_ptr(void);
extern void move_top_identifiers_down(void);
extern void preset_searchid_abort_if_exists(void);
extern void set_searchid_abort_if_exists(short);
extern void reset_searchid_abort_if_exists(void);
extern identifier_info_type *qqsearchid(char *,Boolean);
extern void declare_computational_ident
       (identifier_info_type *,type_flagword_type,value_union_type *);
extern void abort_on_table_overflow(void);
extern void *save_value_in_number_table(value_union_type *,type_flagword_type);
extern identifier_info_type *qqsearchid_new(char *,type_flagword_type,short);
extern identifier_info_type *searchid_old(char *,type_flagword_type,short);
extern identifier_info_type *searchid_dummy(char *,type_flagword_type,short);
extern identifier_info_type *searchid_oldssv(char *,type_flagword_type,short);
extern identifier_info_type
       *searchid_dummy_or_oldssv(char *,type_flagword_type,short);
extern identifier_info_type
       *searchid_oldssvconst(char *,type_flagword_type,short);
extern identifier_info_type *searchid_dcl(char *,type_flagword_type,short);
extern identifier_info_type 
       *searchid_dcl_at_top(char *,type_flagword_type,short);
extern identifier_info_type 
       *searchid_silent(char *,type_flagword_type,short,Boolean);
extern void deactivate_identifier_level(short);
extern Boolean id_unique_first_n(identifier_info_type *,int);
extern void push_ident_scope(void);
extern void pop_ident_scope(void);
extern int_type array_builtin_err(short,char *,char *);
extern void arraymin
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type);
extern void arrayrowmin
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type,short);
extern void arraycolmin
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type,short);
extern void arraymax
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type);
extern void arrayrowmax
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type,short);
extern void arraycolmax
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type,short);
extern void arraysum
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type);
extern void arrayrowsum
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type,short);
extern void arraycolsum
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type,short);
extern void arraycount
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type);
extern void arrayrowcount
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type,short);
extern void arraycolcount
       (identifier_info_type *,value_union_type *,
        type_flagword_type,state_bitstring_type,short);
extern void count_the_sub_array(identifier_info_type *,value_union_type *,
                                type_flagword_type,state_bitstring_type,
                                short,short,short);
extern void min_the_sub_array(identifier_info_type *,value_union_type *,
                                type_flagword_type,state_bitstring_type,
                                short,short,short);
extern void max_the_sub_array(identifier_info_type *,value_union_type *,
                                type_flagword_type,state_bitstring_type,
                                short,short,short);
extern void sum_the_sub_array(identifier_info_type *,value_union_type *,
                                type_flagword_type,state_bitstring_type,
                                short,short,short);
extern identifier_info_type *already_declared_err
       (identifier_info_type *,char *,type_flagword_type,short);
extern char *statement_name_reserved_word(rwtype);
extern Subscript compute_offset(Subscript,Subscript,identifier_info_type *);

 


 








 

# 40 "prseval.c" 2

# 1 "prs_ext.h" 1


 

# 1 "cm_types.h" 1
# 176 "cm_types.h"

# 5 "prs_ext.h" 2

# 1 "prstypes.h" 1
# 88 "prstypes.h"

# 6 "prs_ext.h" 2


 
# 43 "prs_ext.h"

extern Memix init_prslib(void);
extern void re_init_prslib(Memix *,Stacktop *);
extern Boolean is_bool_relation(token);
extern Boolean is_relation(token);
extern Boolean is_bool_op(token);
extern Boolean is_arith_op(token);
extern Boolean is_add_op(token);
extern Boolean is_mpy_op(token);
extern Boolean is_eq_relation(token);
extern Boolean is_ineq_relation(token);
extern Boolean is_not_op(token);
extern Boolean is_cat_op(token);
extern Boolean is_and_op(token);
extern Boolean is_or_op(token);
extern Boolean is_pow_op(token);
extern Boolean is_sign_op(token);
extern Boolean is_value_token(token);
extern Boolean is_literal_token(token);
extern Boolean is_unary_op(token);
operation_type give_relation_op(token);
operation_type give_add_op(token);
operation_type give_cat_op(token);
operation_type give_not_op(token);
operation_type give_and_op(token);
operation_type give_or_op(token);
operation_type give_sign_op(token);
operation_type give_unary_op(token);
operation_type give_infix_pow_op(token);
operation_type give_literal_type(token);
operation_type give_bool_relation_op(token);
extern operation_type give_pow_op(token,type_flagword_type,type_flagword_type);
extern operation_type give_mpy_op(token);
extern operation_type give_any_precedence_op(token,rwtype,token);

 
# 115 "prs_ext.h"

extern type_flagword_type recurse_the_parse(any_parse_routine,char *);
extern Boolean adjust_after_parse(binary_operand_item_info_type *);
extern void test_total_for_fits(long);
extern token_info_type *allocate_for_parse_stage
          (Memix,Memix,Memix,
           Memix,Memix,Memix *,Stacktop *,
           macro_expansion_info_type **);
extern void init_prslib_counts(Memix *,Stacktop *);
extern void free_after_parse_stage(macro_expansion_info_type **,
                                   token_info_type **);
extern void dummy_token(type_flagword_type);
extern void skip_to_next_specified(Boolean,token);
extern void skip_one_when_skipping(Boolean);
extern Boolean skip_nested_when_skipping(token,Boolean);
extern void skip_to_lower_precedence(operation_type);
extern int precedence_cmp(operation_type,operation_type);
extern void skip_to_next_semicolon(Boolean);
extern void skip_to_next_comma_or(Boolean,token);
extern void parse_must_be_semicolon(void);
extern operation_type give_promote_op
       (type_flagword_type,type_flagword_type,short);
extern Boolean promote_to_same_type
       (binary_operand_pair_info_type *,short,Boolean);
extern Memix increment_postfix_operation(void);
extern Memix increment_infix_operation(void);
extern Memix increment_operand(void);
extern void make_room_and_stuff_into_expr(short,operation_type);
extern short save_operator_and_advance_1_tok(token_info_type *);
extern short calc_operation_text_leng(operation_type,Boolean,Boolean *);
extern void show_operation_text(struct _iobuf  *,operation_type,Boolean);
extern void show_infix_or_postfix(operation_type *,short,char *);
extern void show_message(char *);
extern void save_postfix(operation_type);
extern Memix save_infix(operation_type);
extern void lookup_ident(void);
extern void save_operand(operand_type);
extern void next_term_or_factor(binary_operand_pair_info_type *);
extern void set_independent_of_variable(identifier_info_type *);
extern void start_independent_of_check(void);
extern void finish_independent_of_check(void);
extern void not_independent_of(void);

 
# 195 "prs_ext.h"

extern void init_expression(Boolean);
extern void make_inc_or_dec_variable_expr
       (identifier_info_type *,operation_type);
extern void make_inc_or_dec_array_ele_expr
       (identifier_info_type *,operation_type,operation_type);
extern Boolean qq_promote_and_save(binary_operand_pair_info_type *,
                                   short,operation_type,token);
extern void promote_and_save(binary_operand_pair_info_type *,
                             short,operation_type,token);
extern void promote_and_save_bool(binary_operand_pair_info_type *,
                                  short,operation_type,token);
extern void promote_and_save_multiplicative
     (binary_operand_pair_info_type *,short,operation_type,token);
extern void promote_and_save_power(binary_operand_pair_info_type *,
                                   short,operation_type,token);
extern type_flagword_type parse_left_to_right
   (any_parse_routine,any_parse_routine,char *,char *,
    any_is_op_routine,any_give_op_routine,any_promote_routine,Boolean);
extern void expand_tokens_if_function(void);
extern type_flagword_type parse_subscript(void);
extern type_flagword_type qq_parse_subscript(Boolean,any_flagword_routine);
extern type_flagword_type parse_builtin(void);
extern type_flagword_type parse_bin_numeric_item(void);
extern type_flagword_type parse_array_name(void);
extern type_flagword_type parse_array_name_aux(void);
extern type_flagword_type qq_parse_array_name(Boolean,Boolean);
extern type_flagword_type parse_numeric_item(void);
extern type_flagword_type parse_factor(void);
extern type_flagword_type parse_term(void);
extern type_flagword_type parse_term_expr(void);
extern type_flagword_type init_and_parse_rate_expr(Boolean *);
extern type_flagword_type
       init_and_parse_expression(Boolean *,Boolean,Boolean,short *);
extern type_flagword_type
       init_and_parse_bool_expr(Boolean *,Boolean,Boolean,short *);
extern type_flagword_type parse_numeric_expr_silently(void);
extern type_flagword_type parse_numeric_sub_expr_in_bool(void);
extern type_flagword_type parse_numeric_expression(void);
extern type_flagword_type parse_finish_numeric_comparison(void);
extern type_flagword_type parse_finish_bool_comparison(void);
extern type_flagword_type parse_simple_bool_item(void);
extern type_flagword_type parse_bool_item(void);
extern type_flagword_type parse_bool_factor(void);
extern type_flagword_type parse_bool_term(void);
extern type_flagword_type parse_bool_term_expr(void);
extern type_flagword_type parse_boolean_expression(void);

 
# 257 "prs_ext.h"

extern int_type test_value_0_to_max(int_type,int_type);
extern short eval_for_expected_count
       (expression_type *,int_type,Boolean,short,short,Boolean);
extern ssvar_value_type eval_for_expected_ssv
       (expression_type *,int_type,Boolean,short,short,Boolean,Boolean);
extern int_type fast_eval_for_expected_integer(expression_type *);
extern int_type eval_for_expected_integer
       (expression_type *,int_type,Boolean,short,short,short,Boolean);
extern Boolean fast_eval_for_expected_boolean(expression_type *);
extern Boolean eval_for_expected_boolean(expression_type *,Boolean,Boolean);
extern short find_postfix_sub_expr(expression_type *,short);
extern short skip_over_infix_expr
       (operation_type **,short *,Boolean,operation_type,operation_type,
        expression_type *,short);
extern short find_postfix_finish(expression_type *,short,short,operation_type);
extern void eval_sub_infix_expr
       (expression_type *,short *,operation_type **,operation_type *);
extern short print_infix_expr
       (struct _iobuf  *,expression_type *,short,short,short,Boolean,Boolean);

 








extern void make_boolean(state_offset_type *,type_flagword_type *);
extern void default_irange(state_offset_type *,bitsize_type,bitsize_type);
extern void parse_irange(state_offset_type *,ssvar_value_type);
extern void parse_int_for_flag_set_spec(unsigned_31_type *,Boolean);
extern void parse_flag_set_spec(unsigned_31_type *,Boolean,Boolean);
extern void show_this_expr(void);

 


















 






























 



















 


























# 400 "prs_ext.h"








 











 

# 41 "prseval.c" 2

# 1 "eval_ext.h" 1


 

# 1 "../lib/commonio.h" 1
 

# 30 "../lib/commonio.h"

# 5 "eval_ext.h" 2

# 1 "safeeval.h" 1



# 1 "cm_sys.h" 1
 


























# 552 "cm_sys.h"

# 4 "safeeval.h" 2









extern void eval_err(short,char *);
extern real_type real_eval_err(short,real_type,char *,real_type);
extern int_type int_eval_err(short,int_type,char *,int_type);
extern int_type ixtoy(int_type,int_type);
extern real_type rtoi(real_type,int_type);



# 6 "eval_ext.h" 2

# 1 "cm_types.h" 1
# 176 "cm_types.h"

# 7 "eval_ext.h" 2

# 1 "prstypes.h" 1
# 88 "prstypes.h"

# 8 "eval_ext.h" 2







extern void init_prseval(void);
extern type_flagword_type evaluate_expression
      (expression_type *,value_union_type *,Boolean,short,short,short,Boolean);
extern type_flagword_type fast_evaluate_full_expression
      (expression_type *,value_union_type *);


 

# 42 "prseval.c" 2

# 1 "fact_ext.h" 1



# 1 "cm_sys.h" 1
 


























# 552 "cm_sys.h"

# 4 "fact_ext.h" 2


extern int_type ifact(int_type);
extern int_type icomb(int_type,int_type);
extern int_type iperm(int_type,int_type);


# 43 "prseval.c" 2

# 1 "user_ext.h" 1
   
   
   
   
   
   
   



 





extern void abort_program(void);
extern void abort_program_quietly(void);


 


# 1 "cm_types.h" 1
# 176 "cm_types.h"

# 24 "user_ext.h" 2


# 56 "user_ext.h"

 
extern void show_err_on_both(short,void *,type_flagword_type);
extern void show_wrn_on_both(short,void *,type_flagword_type,short);
extern void show_err(struct _iobuf  *,short,void *,type_flagword_type);
extern void reset_arrow_sides(void);
extern void set_arrow_sides(Boolean,Boolean);

extern ssvar_value_type state_space_arr_value
       (state_bitstring_type,state_offset_type *,Subscript);
extern ssvar_value_type state_space_value
       (state_bitstring_type,state_offset_type *);

extern void add_xref_item(char *,char);
extern void enk_line_and_pos(char *,char);
extern void give_ch_info(short *,short *,char *,char *);
extern char *encode_space_bits(state_bitstring_type);
extern void user_advance_1_char(scanning_character_info_type *);
extern Boolean test_if_char_is_first_on_line(void);
extern void advance_one_token(void);
extern void get_tok_after_matching_paren(char *,Boolean);
extern token give_token_after_matching(rwtype *);
extern void user_show_extra_after_error
       (identifier_info_type *,int_type,int_type);
extern void expand_implicit(void);
extern void expand_regular(void);
extern short print_idinfo(struct _iobuf  *,identifier_info_type *,int,int);


extern void shew_logical_rw(rwtype);
extern void shew_logical_token(token);

 








 

# 44 "prseval.c" 2

# 1 "gamma.h" 1



# 1 "../lib/commonio.h" 1
 

# 30 "../lib/commonio.h"

# 4 "gamma.h" 2

# 1 "cm_sys.h" 1
 


























# 552 "cm_sys.h"

# 5 "gamma.h" 2


extern int cgamma(real_type,real_type *);
extern int lngamma(real_type,real_type *);
extern int qq_gamma(real_type,real_type *,Boolean);
extern int igamma(real_type,real_type,real_type *,real_type *,real_type *);


# 45 "prseval.c" 2


extern void eval_stack_push_error(void);
extern void eval_stack_pop_error(void);
extern short qq_do_op_val(expression_type *,short);
extern short qq_fast_do_op_val(expression_type *,short);
extern void qq_perform_eval_expr(unsigned char);
extern type_flagword_type qq_eval_expr
           (expression_type *,value_union_type *,Boolean,short,short,short);
extern type_flagword_type qq_fast_eval_expr
           (expression_type *,value_union_type *);

 
static type_flagword_type push_comps[     ((operation_type) 0xf5)  -      ((operation_type) 0xf0)  +1];
static value_union_type push_values[     ((operation_type) 0xf5)  -      ((operation_type) 0xf0)  +1];
static value_union_type eval_stack_ele[512 ];
static operand_type eval_stack_opnd[512 ];
static type_flagword_type eval_stack_x_comp[512 ];
static Stacktop eval_stack_top;
static short expr_line_number;
# 1 "prs_math.h" 1


 

# 1 "cm_sys.h" 1
 


























# 552 "cm_sys.h"

# 5 "prs_math.h" 2











                       
                       
                       
                       
                       
                       
                       
























                        
                        
                        
                        
                        
                        
                        












































 

# 65 "prseval.c" 2


 

                   
                   
                   
                   
                   
                   
                   

 
extern void do_op_inc(void);
extern void do_op_dec(void);
extern void do_op_not(void);
extern void do_op_neg(void);
extern void do_op_stnchr(void);
extern void do_op_stnboo(void);
extern void do_op_stnint(void);
extern void do_op_stnre(void);
extern void do_op_itor(void);
extern void do_op_btoi(void);
extern void do_op_sqrt(void);
extern void do_op_exp(void);
extern void do_op_ln(void);
extern void do_op_abs(void);
extern void do_op_sin(void);
extern void do_op_cos(void);
extern void do_op_tan(void);
extern void do_op_arcsin(void);
extern void do_op_arccos(void);
extern void do_op_arctan(void);
extern void do_op_fact(void);
extern void do_op_gam(void);
extern void do_op_size(void);
extern void do_op_count(void);
extern void do_op_imin(void);
extern void do_op_rmin(void);
extern void do_op_imax(void);
extern void do_op_rmax(void);
extern void do_op_isum(void);
extern void do_op_rsum(void);
extern void do_op_any(void);
extern void do_op_all(void);
 

                   
                   
                   
                   
                   
                   
                   

 
extern void do_op_or(void);
extern void do_op_xor(void);
extern void do_op_and(void);
extern void do_op_bool_eq(void);
extern void do_op_bool_ne(void);
extern void do_op_lt(void);
extern void do_op_gt(void);
extern void do_op_le(void);
extern void do_op_ge(void);
extern void do_op_eq(void);
extern void do_op_ne(void);
extern void do_op_add(void);
extern void do_op_sub(void);
extern void do_op_mul(void);
extern void do_op_mod(void);
extern void do_op_cyc(void);
extern void do_op_quo(void);
extern void do_op_dvd(void);
extern void do_op_rpowr(void);
extern void do_op_ipowi(void);
extern void do_op_rpowi(void);
extern void do_op_comb(void);
extern void do_op_perm(void);
extern void do_op_irowmin(void);
extern void do_op_rrowmin(void);
extern void do_op_icolmin(void);
extern void do_op_rcolmin(void);
extern void do_op_irowmax(void);
extern void do_op_rrowmax(void);
extern void do_op_icolmax(void);
extern void do_op_rcolmax(void);
extern void do_op_irowsum(void);
extern void do_op_rrowsum(void);
extern void do_op_icolsum(void);
extern void do_op_rcolsum(void);
extern void do_op_rowcount(void);
extern void do_op_colcount(void);
extern void do_op_rowany(void);
extern void do_op_colany(void);
extern void do_op_rowall(void);
extern void do_op_colall(void);
extern void do_op_ixdby(void);
 

                  
                  
                  
                  
                  
                  
                  

 
extern void do_op_ixdby2(void);
 

          
          
          
          
          
          
          

 
extern void do_op_liscount(void);
extern void do_op_ilismin(void);
extern void do_op_rlismin(void);
extern void do_op_ilismax(void);
extern void do_op_rlismax(void);
extern void do_op_ilissum(void);
extern void do_op_rlissum(void);
extern void do_op_lisany(void);
extern void do_op_lisall(void);
 

                        
                        
                        
                        
                        
                        
                        

static any_void_routine op_routines[256];
 
void init_prseval()
{
   register short i;
    
   for (i=0;i<256;++i) op_routines[i] = ((any_void_routine) 0 );
    
   push_values[      ((operation_type) 0xf0) -      ((operation_type) 0xf0)  ].iii = 0;
   push_values[     ((operation_type) 0xf1) -      ((operation_type) 0xf0)  ].bbb = (Boolean) 0 ;
   push_values[     ((operation_type) 0xf2) -      ((operation_type) 0xf0)  ].rrr = 0.0000000000000000000000000000;
   push_values[      ((operation_type) 0xf3) -      ((operation_type) 0xf0)  ].iii = 1;
   push_values[     ((operation_type) 0xf4) -      ((operation_type) 0xf0)  ].bbb =  (Boolean) 1 ;
   push_values[     ((operation_type) 0xf5) -      ((operation_type) 0xf0)  ].rrr = 1.0000000000000000000000000000;
   push_comps[      ((operation_type) 0xf0) -      ((operation_type) 0xf0)  ] =                ((type_flagword_type) 0x03) ;
   push_comps[     ((operation_type) 0xf1) -      ((operation_type) 0xf0)  ] =               ((type_flagword_type) 0x02) ;
   push_comps[     ((operation_type) 0xf2) -      ((operation_type) 0xf0)  ] =               ((type_flagword_type) 0x04) ;
   push_comps[      ((operation_type) 0xf3) -      ((operation_type) 0xf0)  ] =                ((type_flagword_type) 0x03) ;
   push_comps[     ((operation_type) 0xf4) -      ((operation_type) 0xf0)  ] =               ((type_flagword_type) 0x02) ;
   push_comps[     ((operation_type) 0xf5) -      ((operation_type) 0xf0)  ] =               ((type_flagword_type) 0x04) ;
    
   op_routines[    ((operation_type) 0x58) ] = do_op_inc ;
   op_routines[    ((operation_type) 0x59) ] = do_op_dec ;
   op_routines[    ((operation_type) 0x60) ] = do_op_not ;
   op_routines[    ((operation_type) 0x68) ] = do_op_neg ;
   op_routines[ ((operation_type) 0x69) ] = do_op_stnchr ;
   op_routines[ ((operation_type) 0x6a) ] = do_op_stnboo ;
   op_routines[ ((operation_type) 0x6b) ] = do_op_stnint ;
   op_routines[  ((operation_type) 0x6c) ] = do_op_stnre ;
   op_routines[   ((operation_type) 0x6d) ] = do_op_itor ;
   op_routines[   ((operation_type) 0x6e) ] = do_op_btoi ;
   op_routines[    ((operation_type) 0x70) ] = do_op_sqrt ;
   op_routines[     ((operation_type) 0x71) ] = do_op_exp ;
   op_routines[      ((operation_type) 0x72) ] = do_op_ln ;
   op_routines[     ((operation_type) 0x73) ] = do_op_abs ;
   op_routines[     ((operation_type) 0x74) ] = do_op_sin ;
   op_routines[     ((operation_type) 0x75) ] = do_op_cos ;
   op_routines[     ((operation_type) 0x76) ] = do_op_tan ;
   op_routines[  ((operation_type) 0x77) ] = do_op_arcsin ;
   op_routines[  ((operation_type) 0x78) ] = do_op_arccos ;
   op_routines[  ((operation_type) 0x79) ] = do_op_arctan ;
   op_routines[     ((operation_type) 0x7b) ] = do_op_gam ;
   op_routines[    ((operation_type) 0x7a) ] = do_op_fact ;
   op_routines[    ((operation_type) 0x7f) ] = do_op_size ;
   op_routines[   ((operation_type) 0x90) ] = do_op_count ;
   op_routines[    ((operation_type) 0x91) ] = do_op_imin ;
   op_routines[    ((operation_type) 0x92) ] = do_op_rmin ;
   op_routines[    ((operation_type) 0x93) ] = do_op_imax ;
   op_routines[    ((operation_type) 0x94) ] = do_op_rmax ;
   op_routines[    ((operation_type) 0x95) ] = do_op_isum ;
   op_routines[    ((operation_type) 0x96) ] = do_op_rsum ;
   op_routines[     ((operation_type) 0x97) ] = do_op_any ;
   op_routines[     ((operation_type) 0x98) ] = do_op_all ;
    
   op_routines[     ((operation_type) 0x10) ] = do_op_or ;
   op_routines[    ((operation_type) 0x11) ] = do_op_xor ;
   op_routines[    ((operation_type) 0x18) ] = do_op_and ;
   op_routines[((operation_type) 0x20) ] = do_op_bool_eq ;
   op_routines[((operation_type) 0x21) ] = do_op_bool_ne ;
   op_routines[     ((operation_type) 0x28) ] = do_op_lt ;
   op_routines[     ((operation_type) 0x29) ] = do_op_gt ;
   op_routines[     ((operation_type) 0x2a) ] = do_op_le ;
   op_routines[     ((operation_type) 0x2b) ] = do_op_ge ;
   op_routines[     ((operation_type) 0x2c) ] = do_op_eq ;
   op_routines[     ((operation_type) 0x2d) ] = do_op_ne ;
   op_routines[    ((operation_type) 0x30) ] = do_op_add ;
   op_routines[    ((operation_type) 0x31) ] = do_op_sub ;
   op_routines[    ((operation_type) 0x38) ] = do_op_mul ;
   op_routines[    ((operation_type) 0x3a) ] = do_op_mod ;
   op_routines[    ((operation_type) 0x3b) ] = do_op_cyc ;
   op_routines[    ((operation_type) 0x3c) ] = do_op_quo ;
   op_routines[    ((operation_type) 0x39) ] = do_op_dvd ;
   op_routines[  ((operation_type) 0x41) ] = do_op_rpowr ;
   op_routines[  ((operation_type) 0x42) ] = do_op_ipowi ;
   op_routines[  ((operation_type) 0x43) ] = do_op_rpowi ;
   op_routines[    ((operation_type) 0xa0) ] = do_op_comb ;
   op_routines[    ((operation_type) 0xa1) ] = do_op_perm ;
   op_routines[ ((operation_type) 0xb2) ] = do_op_irowmin ;
   op_routines[ ((operation_type) 0xb4) ] = do_op_rrowmin ;
   op_routines[ ((operation_type) 0xb3) ] = do_op_icolmin ;
   op_routines[ ((operation_type) 0xb5) ] = do_op_rcolmin ;
   op_routines[ ((operation_type) 0xb6) ] = do_op_irowmax ;
   op_routines[ ((operation_type) 0xb8) ] = do_op_rrowmax ;
   op_routines[ ((operation_type) 0xb7) ] = do_op_icolmax ;
   op_routines[ ((operation_type) 0xb9) ] = do_op_rcolmax ;
   op_routines[ ((operation_type) 0xba) ] = do_op_irowsum ;
   op_routines[ ((operation_type) 0xbc) ] = do_op_rrowsum ;
   op_routines[ ((operation_type) 0xbb) ] = do_op_icolsum ;
   op_routines[ ((operation_type) 0xbd) ] = do_op_rcolsum ;
   op_routines[((operation_type) 0xb0) ] = do_op_rowcount ;
   op_routines[((operation_type) 0xb1) ] = do_op_colcount ;
   op_routines[  ((operation_type) 0xbe) ] = do_op_rowany ;
   op_routines[  ((operation_type) 0xbf) ] = do_op_colany ;
   op_routines[  ((operation_type) 0xc0) ] = do_op_rowall ;
   op_routines[  ((operation_type) 0xc1) ] = do_op_colall ;





   op_routines[  ((operation_type) 0xf8) ] = do_op_ixdby ;
    
   op_routines[ ((operation_type) 0xf9) ] = do_op_ixdby2 ;
    
   op_routines[((operation_type) 0xd0) ] = do_op_liscount ;
   op_routines[ ((operation_type) 0xd1) ] = do_op_ilismin ;
   op_routines[ ((operation_type) 0xd2) ] = do_op_rlismin ;
   op_routines[ ((operation_type) 0xd3) ] = do_op_ilismax ;
   op_routines[ ((operation_type) 0xd4) ] = do_op_rlismax ;
   op_routines[ ((operation_type) 0xd5) ] = do_op_ilissum ;
   op_routines[ ((operation_type) 0xd6) ] = do_op_rlissum ;
   op_routines[  ((operation_type) 0xd7) ] = do_op_lisany ;
   op_routines[  ((operation_type) 0xd8) ] = do_op_lisall ;





    
}
 
void eval_err(short ee,char *txt)
{
   char msg[100];
    
   sprintf(msg,"%s(expression on line#%d)",txt,(int) expr_line_number);
   show_err_at_pos_on_both(ee,this.pos,msg,              ((type_flagword_type) 0x01) );
    
}
 






 
real_type real_eval_err(short ee,real_type x,char *s,real_type y)
 




{
   char xmsg[50 ];
   real_type vvv;
    
   *xmsg='\0';
   if (s!=((char *) 0 )){
      vvv=x; strcat(xmsg,enk_aux_val(&vvv,              ((type_flagword_type) 0x04) )) ;
      strcat(xmsg,s) ;
      vvv=y; strcat(xmsg,enk_aux_val(&vvv,              ((type_flagword_type) 0x04) )) ;
      strcat(xmsg,", ") ;}
   eval_err(ee,xmsg);
    
   return(0.000000000000 );
}
 
int_type int_eval_err(short ee,int_type x,char *s,int_type y)
 




{
   char xmsg[50 ];
   int_type vvv;
    
   *xmsg='\0';
   if (s!=((char *) 0 )){
      vvv=x; strcat(xmsg,enk_aux_val(&vvv,               ((type_flagword_type) 0x03) )) ;
      strcat(xmsg,s) ;
      vvv=y; strcat(xmsg,enk_aux_val(&vvv,               ((type_flagword_type) 0x03) )) ;
      strcat(xmsg,", ") ;}
   eval_err(ee,xmsg);
    
   return((int_type) 0);
}
 
int_type ixtoy(int_type x,int_type y)
 



{
   register int_type ans,i,n;
    
   if (y<0) return((int_type) 0);
   else if (y==0) return((int_type) 1);
   ans=1;
   for (i=0,n=y;i<y;++i) ans=((((((x) < ((long int) 0)) ? ((long int) -(x)) : ((long int) (x))) >1)&&((((ans) < ((long int) 0)) ? ((long int) -(ans)) : ((long int) (ans))) >=(1+(((int_type) ((long) 0x7fffffff) ) -1)/(((x) < ((long int) 0)) ? ((long int) -(x)) : ((long int) (x))) ))) ? int_eval_err(      111 ,ans,"*",x)  : ((ans)*(x))) ;
    
   return(ans);
}
 
real_type rtoi(real_type x,int_type y)
 



{
   register int_type i,n;
   register real_type ans;
    
   if (y<0) return(0.000000000000 );
   else if (y==0) return( 1.000000000000 );
   ans= 1.000000000000 ;
   for (i=0,n=y;i<n;++i) ans=(((fabs(x)> 1.000000000000 )&&(fabs(ans)>(((real_type) 1.797693134862315708e+308 ) /fabs(x)))) ? real_eval_err(         110 ,ans,"*",x)  : ((ans)*(x))) ;
    
   return(ans);
}
 
void eval_stack_push_error()
{
   char msg[512];
   char tmsg[12];
    
   strcpy(msg,"Expression evaluation stack overflow.  ");
   strcat(msg,"Try simplifying expression via use of named constants or ");
   strcat(msg,"re-build ");
   strcat(msg,language);
   strcat(msg," with a larger value for EVAL_STACK_DIM.  ");
   strcat(msg,"(expression on line#");
   sprintf(tmsg,"%d",(int) expr_line_number);
   strcat(msg,tmsg);
   strcat(msg,")");
   show_err_at_pos_on_both(          1 ,this.pos,msg,              ((type_flagword_type) 0x01) );
   abort_program();
    
}
 
void eval_stack_pop_error()
{
   char msg[512];
   char tmsg[12];
    
   strcpy(msg,"Cannot pop empty expression stack. (expression on line#");
   sprintf(tmsg,"%d",(int) expr_line_number);
   strcat(msg,tmsg);
   strcat(msg,")");
   show_err_at_pos_on_both(          1 ,this.pos,msg,              ((type_flagword_type) 0x01) );
   abort_program();
    
}
 
static value_union_type l_val,r_val,c_val,m_val[32 ];
static operand_type l_opnd,r_opnd,c_opnd,m_opnd[32 ];
static type_flagword_type l_comp,r_comp,c_comp,m_comp[32 ];
static type_flagword_type l_x_comp,r_x_comp,c_x_comp,m_x_comp[32 ];
static Boolean is_ssv,is_int,is_bool;
static any_void_routine rou_to_perform;
 



 

static Boolean trace_evaluation=(Boolean) 0 ;
static Boolean trace_for_const=(Boolean) 0 ;
 


















 
static char RightsidePopFmt[] = "popR: %s, %04x%s%s\n";
static char LeftsidePopFmt[]  = "popL: %s, %04x%s%s\n";
static char CenterPopFmt[]    = "popC: %s, %04x%s%s\n";
static char MultiPopFmt[]     = "popM: %s, %04x%s%s\n";
 














 

                   
                   
                   
                   
                   
                   
                   

 
void do_op_inc(){++l_val.iii;}
void do_op_dec(){--l_val.iii;}
void do_op_not(){l_val.bbb = (Boolean) (!l_val.bbb);}
void do_op_neg()
{if (is_int) l_val.iii=(-l_val.iii); else l_val.rrr=(-l_val.rrr);}
void do_op_stnchr(){l_val.ccc=' ';l_comp=              ((type_flagword_type) 0x01) ;}
void do_op_stnboo(){l_val.bbb=(Boolean) 0 ;l_comp=              ((type_flagword_type) 0x02) ;}
void do_op_stnint(){l_val.iii=0;l_comp=               ((type_flagword_type) 0x03) ;}
void do_op_stnre(){l_val.rrr=0.000000000000 ;l_comp=              ((type_flagword_type) 0x04) ;}
void do_op_itor(){l_val.rrr=l_val.iii;l_comp=              ((type_flagword_type) 0x04) ;}
void do_op_btoi(){l_val.iii=(int_type) l_val.bbb;l_comp=               ((type_flagword_type) 0x03) ;}
void do_op_sqrt(){l_val.rrr=(((l_val.rrr)<0.000000000000 ) ? real_eval_err(         102 ,l_val.rrr,"<",0.000000000000 )  : sqrt(l_val.rrr)) ;}
void do_op_exp(){l_val.rrr=(((l_val.rrr)>ln_maxreal) ? real_eval_err(          104 ,l_val.rrr,">",ln_maxreal)  : exp(l_val.rrr)) ;}
void do_op_ln(){
l_val.rrr=(((l_val.rrr)<=((real_type) 4.94065645841246544e-324 ) ) ? real_eval_err(         103 ,l_val.rrr,"<=",0.000000000000 )  : log(l_val.rrr)) ;
if (errno==	33		) real_eval_err(         103 ,l_val.rrr,0 ,l_val.rrr) ;}
void do_op_abs()
{if (is_int) l_val.iii=(((l_val.iii) < ((long int) 0)) ? ((long int) -(l_val.iii)) : ((long int) (l_val.iii))) ; else l_val.rrr=fabs(l_val.rrr);}
void do_op_sin(){l_val.rrr=sin(l_val.rrr);}
void do_op_cos(){l_val.rrr=cos(l_val.rrr);}
void do_op_tan(){l_val.rrr=tan(l_val.rrr);
if (errno==	34		) real_eval_err(         105 ,l_val.rrr,0 ,l_val.rrr) ;}
void do_op_arcsin(){l_val.rrr=((fabs(l_val.rrr)> 1.000000000000 ) ? real_eval_err(       106 ,l_val.rrr,"(ABS)>", 1.000000000000 )  : asin(l_val.rrr)) ;
if (errno==	34		) real_eval_err(       106 ,l_val.rrr,0 ,l_val.rrr) ;}
void do_op_arccos(){l_val.rrr=((fabs(l_val.rrr)> 1.000000000000 ) ? real_eval_err(       107 ,l_val.rrr,"(ABS)>", 1.000000000000 )  : acos(l_val.rrr)) ;
if (errno==	34		) real_eval_err(       107 ,l_val.rrr,0 ,l_val.rrr) ;}
void do_op_arctan(){l_val.rrr=atan(l_val.rrr);}
void do_op_fact(){l_val.iii = ifact(l_val.iii);}
void do_op_gam(){int ierr; real_type ans;
ierr=cgamma(l_val.rrr,&ans);
if (ierr!=0){real_eval_err(         110 ,0,0 ,0) ;}
l_val.rrr=ans;}
void do_op_size(){
l_val.iii = ((int_type) ((*l_opnd).index.dims.first.upper-(*l_opnd).index.dims.first.lower+1) ) *
            ((int_type) ((*l_opnd).index.dims.second.upper-(*l_opnd).index.dims.second.lower+1) );
l_comp=               ((type_flagword_type) 0x03) ;}
void do_op_count(){
arraycount(l_opnd,&l_val,l_comp,eval_state_ssbits); l_comp=               ((type_flagword_type) 0x03) ;}
void do_op_imin(){if (l_comp!=               ((type_flagword_type) 0x03) ) int_eval_err(       19 ,0,0 ,0) ;
arraymin(l_opnd,&l_val,               ((type_flagword_type) 0x03) ,eval_state_ssbits);}
void do_op_rmin(){if (l_comp!=              ((type_flagword_type) 0x04) ) real_eval_err(          41 ,0,0 ,0) ;
arraymin(l_opnd,&l_val,              ((type_flagword_type) 0x04) ,eval_state_ssbits);}
void do_op_imax(){if (l_comp!=               ((type_flagword_type) 0x03) ) int_eval_err(       19 ,0,0 ,0) ;
arraymax(l_opnd,&l_val,               ((type_flagword_type) 0x03) ,eval_state_ssbits);}
void do_op_rmax(){if (l_comp!=              ((type_flagword_type) 0x04) ) real_eval_err(          41 ,0,0 ,0) ;
arraymax(l_opnd,&l_val,              ((type_flagword_type) 0x04) ,eval_state_ssbits);}
void do_op_isum(){if (l_comp!=               ((type_flagword_type) 0x03) ) int_eval_err(       19 ,0,0 ,0) ;
arraysum(l_opnd,&l_val,               ((type_flagword_type) 0x03) ,eval_state_ssbits);}
void do_op_rsum(){if (l_comp!=              ((type_flagword_type) 0x04) ) real_eval_err(          41 ,0,0 ,0) ;
arraysum(l_opnd,&l_val,              ((type_flagword_type) 0x04) ,eval_state_ssbits);}
void do_op_any()
{do_op_count(); l_val.bbb = (Boolean) (l_val.iii>0); l_comp=              ((type_flagword_type) 0x02) ;}
void do_op_all(){int_type prod; do_op_count();
prod = ((int_type) ((*l_opnd).index.dims.first.upper-(*l_opnd).index.dims.first.lower+1) ) *
       ((int_type) ((*l_opnd).index.dims.second.upper-(*l_opnd).index.dims.second.lower+1) );
l_val.bbb = (Boolean) (l_val.iii==prod); l_comp=              ((type_flagword_type) 0x02) ;}
 

                   
                   
                   
                   
                   
                   
                   

 
void do_op_or(){l_val.bbb = (Boolean) (l_val.bbb || r_val.bbb);}
void do_op_xor()
{l_val.bbb=(Boolean) ((l_val.bbb || r_val.bbb)&&(!(l_val.bbb && r_val.bbb)));}
void do_op_and(){l_val.bbb = (Boolean) (l_val.bbb && r_val.bbb);}
void do_op_bool_eq(){l_val.bbb = (Boolean) (l_val.bbb == r_val.bbb);}
void do_op_bool_ne(){l_val.bbb = (Boolean) (l_val.bbb != r_val.bbb);}
void do_op_lt(){l_comp=              ((type_flagword_type) 0x02) ;
if (is_int) l_val.bbb=(Boolean) (l_val.iii < r_val.iii);
else l_val.bbb=(Boolean) (l_val.rrr < r_val.rrr);}
void do_op_gt(){l_comp=              ((type_flagword_type) 0x02) ;
if (is_int) l_val.bbb=(Boolean) (l_val.iii > r_val.iii);
else l_val.bbb=(Boolean) (l_val.rrr > r_val.rrr);}
void do_op_le(){l_comp=              ((type_flagword_type) 0x02) ;
if (is_int) l_val.bbb=(Boolean) (l_val.iii <= r_val.iii);
else l_val.bbb=(Boolean) (l_val.rrr <= r_val.rrr);}
void do_op_ge(){l_comp=              ((type_flagword_type) 0x02) ;
if (is_int) l_val.bbb=(Boolean) (l_val.iii >= r_val.iii);
else l_val.bbb=(Boolean) (l_val.rrr >= r_val.rrr);}
void do_op_eq(){l_comp=              ((type_flagword_type) 0x02) ;
if (is_int) l_val.bbb=(Boolean) (l_val.iii == r_val.iii);
else l_val.bbb=(Boolean) (l_val.rrr == r_val.rrr);}
void do_op_ne(){l_comp=              ((type_flagword_type) 0x02) ;
if (is_int) l_val.bbb=(Boolean) (l_val.iii != r_val.iii);
else l_val.bbb=(Boolean) (l_val.rrr != r_val.rrr);}
void do_op_add(){
if (is_int) l_val.iii=(((((l_val.iii)>0)&&((r_val.iii)>(((int_type) ((long) 0x7fffffff) ) -(l_val.iii)))) || (((l_val.iii)<0)&&((r_val.iii)<(-((int_type) ((long) 0x7fffffff) ) -(l_val.iii))))) ? int_eval_err(      111 ,l_val.iii,"+",r_val.iii)  : (l_val.iii)+(r_val.iii)) ;
else l_val.rrr=(((((l_val.rrr)>0.000000000000 )&&((r_val.rrr)>(((real_type) 1.797693134862315708e+308 ) -(l_val.rrr)))) || (((l_val.rrr)<0.000000000000 )&&((r_val.rrr)<(-((real_type) 1.797693134862315708e+308 ) -(l_val.rrr))))) ? real_eval_err(         110 ,l_val.rrr,"+",r_val.rrr)  : (l_val.rrr)+(r_val.rrr)) ;}
void do_op_sub(){
if (is_int) l_val.iii=(((((l_val.iii)>0)&&((-(r_val.iii))>(((int_type) ((long) 0x7fffffff) ) -(l_val.iii)))) || (((l_val.iii)<0)&&((-(r_val.iii))<(-((int_type) ((long) 0x7fffffff) ) -(l_val.iii))))) ? int_eval_err(      111 ,l_val.iii,"+",-(r_val.iii))  : (l_val.iii)+(-(r_val.iii)))  ;
else l_val.rrr=(((((l_val.rrr)>0.000000000000 )&&(((-(r_val.rrr)))>(((real_type) 1.797693134862315708e+308 ) -(l_val.rrr)))) || (((l_val.rrr)<0.000000000000 )&&(((-(r_val.rrr)))<(-((real_type) 1.797693134862315708e+308 ) -(l_val.rrr))))) ? real_eval_err(         110 ,l_val.rrr,"+",(-(r_val.rrr)))  : (l_val.rrr)+((-(r_val.rrr))))  ;}
void do_op_mul(){
if (is_int) l_val.iii=((((((r_val.iii) < ((long int) 0)) ? ((long int) -(r_val.iii)) : ((long int) (r_val.iii))) >1)&&((((l_val.iii) < ((long int) 0)) ? ((long int) -(l_val.iii)) : ((long int) (l_val.iii))) >=(1+(((int_type) ((long) 0x7fffffff) ) -1)/(((r_val.iii) < ((long int) 0)) ? ((long int) -(r_val.iii)) : ((long int) (r_val.iii))) ))) ? int_eval_err(      111 ,l_val.iii,"*",r_val.iii)  : ((l_val.iii)*(r_val.iii))) ;
else l_val.rrr=(((fabs(r_val.rrr)> 1.000000000000 )&&(fabs(l_val.rrr)>(((real_type) 1.797693134862315708e+308 ) /fabs(r_val.rrr)))) ? real_eval_err(         110 ,l_val.rrr,"*",r_val.rrr)  : ((l_val.rrr)*(r_val.rrr))) ;}
void do_op_mod(){l_val.iii=(((r_val.iii)==0) ? int_eval_err(           112 ,l_val.iii," MOD ",r_val.iii)  : ((l_val.iii)%(r_val.iii))) ;}
void do_op_cyc(){l_val.iii=(((r_val.iii)==0) ? int_eval_err(           170 ,l_val.iii," CYC ",r_val.iii)  : (1+(((l_val.iii)-1)%(r_val.iii)))) ;}
void do_op_quo(){l_val.iii=(((r_val.iii)==0) ? int_eval_err(           113 ,l_val.iii," DIV ",r_val.iii)  : ((l_val.iii)/(r_val.iii))) ;}
void do_op_dvd(){l_val.rrr=(((fabs(r_val.rrr))<=((real_type) 4.94065645841246544e-324 ) ) ? real_eval_err(           114 ,l_val.rrr,"/",r_val.rrr)  : (((fabs(r_val.rrr)< 1.000000000000 )&&(fabs(l_val.rrr)>(((real_type) 1.797693134862315708e+308 ) *fabs(r_val.rrr)))) ? real_eval_err(         110 ,l_val.rrr,"/",r_val.rrr)  : ((l_val.rrr)/(r_val.rrr)))) ;}
void do_op_rpowr(){l_val.rrr=(((l_val.rrr)<=((real_type) 4.94065645841246544e-324 ) ) ? real_eval_err(       109 ,l_val.rrr,"**",r_val.rrr)  : exp((((fabs(log(l_val.rrr))> 1.000000000000 )&&(fabs((r_val.rrr))>(((real_type) 1.797693134862315708e+308 ) /fabs(log(l_val.rrr))))) ? real_eval_err(         110 ,(r_val.rrr),"*",log(l_val.rrr))  : (((r_val.rrr))*(log(l_val.rrr)))) )) ;}
void do_op_ipowi(){l_val.iii=ixtoy(l_val.iii,r_val.iii);}
void do_op_rpowi(){l_val.rrr=rtoi(l_val.rrr,r_val.iii);}
void do_op_comb(){l_val.iii = icomb(l_val.iii,r_val.iii);}
void do_op_perm(){l_val.iii = iperm(l_val.iii,r_val.iii);}
void do_op_irowmin(){if (l_comp!=               ((type_flagword_type) 0x03) ) int_eval_err(       19 ,0,0 ,0) ;
arrayrowmin(l_opnd,&l_val,               ((type_flagword_type) 0x03) ,eval_state_ssbits,(short) r_val.iii);}
void do_op_rrowmin(){if (l_comp!=              ((type_flagword_type) 0x04) ) real_eval_err(          41 ,0,0 ,0) ;
arrayrowmin(l_opnd,&l_val,              ((type_flagword_type) 0x04) ,eval_state_ssbits,(short) r_val.iii);}
void do_op_icolmin(){if (l_comp!=               ((type_flagword_type) 0x03) ) int_eval_err(       19 ,0,0 ,0) ;
arraycolmin(l_opnd,&l_val,               ((type_flagword_type) 0x03) ,eval_state_ssbits,(short) r_val.iii);}
void do_op_rcolmin(){if (l_comp!=              ((type_flagword_type) 0x04) ) real_eval_err(          41 ,0,0 ,0) ;
arraycolmin(l_opnd,&l_val,              ((type_flagword_type) 0x04) ,eval_state_ssbits,(short) r_val.iii);}
void do_op_irowmax(){if (l_comp!=               ((type_flagword_type) 0x03) ) int_eval_err(       19 ,0,0 ,0) ;
arrayrowmax(l_opnd,&l_val,               ((type_flagword_type) 0x03) ,eval_state_ssbits,(short) r_val.iii);}
void do_op_rrowmax(){if (l_comp!=              ((type_flagword_type) 0x04) ) real_eval_err(          41 ,0,0 ,0) ;
arrayrowmax(l_opnd,&l_val,              ((type_flagword_type) 0x04) ,eval_state_ssbits,(short) r_val.iii);}
void do_op_icolmax(){if (l_comp!=               ((type_flagword_type) 0x03) ) int_eval_err(       19 ,0,0 ,0) ;
arraycolmax(l_opnd,&l_val,               ((type_flagword_type) 0x03) ,eval_state_ssbits,(short) r_val.iii);}
void do_op_rcolmax(){if (l_comp!=              ((type_flagword_type) 0x04) ) real_eval_err(          41 ,0,0 ,0) ;
arraycolmax(l_opnd,&l_val,              ((type_flagword_type) 0x04) ,eval_state_ssbits,(short) r_val.iii);}
void do_op_irowsum(){if (l_comp!=               ((type_flagword_type) 0x03) ) int_eval_err(       19 ,0,0 ,0) ;
arrayrowsum(l_opnd,&l_val,               ((type_flagword_type) 0x03) ,eval_state_ssbits,(short) r_val.iii);}
void do_op_rrowsum(){if (l_comp!=              ((type_flagword_type) 0x04) ) real_eval_err(          41 ,0,0 ,0) ;
arrayrowsum(l_opnd,&l_val,              ((type_flagword_type) 0x04) ,eval_state_ssbits,(short) r_val.iii);}
void do_op_icolsum(){if (l_comp!=               ((type_flagword_type) 0x03) ) int_eval_err(       19 ,0,0 ,0) ;
arraycolsum(l_opnd,&l_val,               ((type_flagword_type) 0x03) ,eval_state_ssbits,(short) r_val.iii);}
void do_op_rcolsum(){if (l_comp!=              ((type_flagword_type) 0x04) ) real_eval_err(          41 ,0,0 ,0) ;
arraycolsum(l_opnd,&l_val,              ((type_flagword_type) 0x04) ,eval_state_ssbits,(short) r_val.iii);}
void do_op_rowcount(){
arrayrowcount(l_opnd,&l_val,l_comp,eval_state_ssbits,r_val.iii);
l_comp=               ((type_flagword_type) 0x03) ;}
void do_op_colcount(){
arraycolcount(l_opnd,&l_val,l_comp,eval_state_ssbits,r_val.iii);
l_comp=               ((type_flagword_type) 0x03) ;}
void do_op_rowany()
{do_op_rowcount(); l_val.bbb = (Boolean) (l_val.iii>0); l_comp=              ((type_flagword_type) 0x02) ;}
void do_op_colany()
{do_op_colcount(); l_val.bbb = (Boolean) (l_val.iii>0); l_comp=              ((type_flagword_type) 0x02) ;}
void do_op_rowall()
{do_op_rowcount();
l_val.bbb = (Boolean) (l_val.iii==((*l_opnd).index.dims.second.upper-(*l_opnd).index.dims.second.lower+1) );
l_comp=              ((type_flagword_type) 0x02) ;}
void do_op_colall(){do_op_colcount();
l_val.bbb = (Boolean) (l_val.iii==((*l_opnd).index.dims.first.upper-(*l_opnd).index.dims.first.lower+1) );
l_comp=              ((type_flagword_type) 0x02) ;}
 
void do_op_ixdby()
{

   register short *pppsh0,*pppsh1 ;

   ssvar_value_type stateval;
   state_offset_type stateoff;
   register int_type tmp_off;
   l_x_comp=0;
   tmp_off=0;
   if ((r_val.iii<(*l_opnd).index.dims.first.lower)||
       (r_val.iii>(*l_opnd).index.dims.first.upper)){
      eval_err(  140 ,"");
      user_show_extra_after_error(l_opnd,r_val.iii,((int_type) -888888888) );}
   else
      tmp_off = r_val.iii - ((int_type) (*l_opnd).index.dims.first.lower);
   if (is_ssv){
      pppsh0 = (short *) (&stateoff); pppsh1 = (short *) ((*l_opnd).ptr.sss); *pppsh0++ = *pppsh1++; *pppsh0++ = *pppsh1++; *pppsh0++ = *pppsh1++; *pppsh0 = *pppsh1 ;
      stateoff.bit_offset +=
         ((bitsize_type) ((stateoff.bit_length * tmp_off)));
      stateval=state_space_value(eval_state_ssbits,&stateoff);
      if (l_comp==              ((type_flagword_type) 0x02) ) l_val.bbb=((stateval==0) ? (Boolean) 0  :  (Boolean) 1 );
      else l_val.iii = (int_type) stateval;}
   else if (is_int) l_val.iii = *(l_val.ptr.iii+tmp_off);
   else if (is_bool) l_val.bbb = *(l_val.ptr.bbb+tmp_off);
   else l_val.rrr = *(l_val.ptr.rrr+tmp_off);
}
 

                  
                  
                  
                  
                  
                  
                  

 
void do_op_ixdby2()
{

register short *pppsh0,*pppsh1 ;

   ssvar_value_type stateval;
   state_offset_type stateoff;
   register int_type tmp_off;
   l_x_comp=0;
   tmp_off=0;
   if ((c_val.iii<(*l_opnd).index.dims.first.lower)||
       (c_val.iii>(*l_opnd).index.dims.first.upper)){
      eval_err(  140 ,"");
      user_show_extra_after_error(l_opnd,c_val.iii,((int_type) -888888888) );}
   else if ((r_val.iii<(*l_opnd).index.dims.second.lower)||
            (r_val.iii>(*l_opnd).index.dims.second.upper)){
      eval_err(  140 ,"");
      user_show_extra_after_error(l_opnd,((int_type) -888888888) ,r_val.iii);}
   else{
      tmp_off = compute_offset(c_val.iii,r_val.iii,l_opnd);}
   if (is_ssv){
      pppsh0 = (short *) (&stateoff); pppsh1 = (short *) ((*l_opnd).ptr.sss); *pppsh0++ = *pppsh1++; *pppsh0++ = *pppsh1++; *pppsh0++ = *pppsh1++; *pppsh0 = *pppsh1 ;
      stateoff.bit_offset +=
           ((bitsize_type) ((stateoff.bit_length * tmp_off)));
      stateval=state_space_value(eval_state_ssbits,&stateoff);
      if (l_comp==              ((type_flagword_type) 0x02) ) l_val.bbb=((stateval==0) ? (Boolean) 0  :  (Boolean) 1 );
      else l_val.iii = (int_type) stateval;}
   else if (is_int) l_val.iii = *(l_val.ptr.iii+tmp_off);
   else if (is_bool) l_val.bbb = *(l_val.ptr.bbb+tmp_off);
   else l_val.rrr = *(l_val.ptr.rrr+tmp_off);
}
 

          
          
          
          
          
          
          

 
void do_op_liscount()
{
   int_type iii,knt;
   for (knt=iii=0;iii<c_val.iii;++iii){
      if ((*(m_opnd+iii))==0 ){
         if ((*(m_val+iii)).bbb) ++knt;}
      else{
         arraycount(*(m_opnd+iii),&l_val,*(m_comp+iii),eval_state_ssbits);
         knt += l_val.iii;}}
   l_val.iii = knt;
   l_comp=               ((type_flagword_type) 0x03) ;
}
 
void do_op_ilismin()
{
   register int_type temp;
   register int_type iii,cum;
   for (cum=iii=0;iii<c_val.iii;++iii){
      if ((*(m_comp+iii))!=               ((type_flagword_type) 0x03) ) int_eval_err(       19 ,0,0 ,0) ;
      if ((*(m_opnd+iii))==0 ){
         temp = (*(m_val+iii)).iii;
         if ((iii==0)||(temp<cum)) cum = temp;}
      else{
         arraymin(*(m_opnd+iii),&l_val,               ((type_flagword_type) 0x03) ,eval_state_ssbits);
         temp = l_val.iii;
         if ((iii==0)||(temp<cum)) cum = temp;}}
   l_val.iii = cum;
   l_comp=               ((type_flagword_type) 0x03) ;
}
 
void do_op_rlismin()
{
   register real_type temp;
   register int_type iii;
   register real_type cum;
   for (iii=0,cum=0.000000000000 ;iii<c_val.iii;++iii){
      if ((*(m_comp+iii))!=              ((type_flagword_type) 0x04) ) real_eval_err(          41 ,0,0 ,0) ;
      if ((*(m_opnd+iii))==0 ){
         temp = (*(m_val+iii)).rrr;
         if ((iii==0)||(temp<cum)) cum = temp;}
      else{
         arraymin(*(m_opnd+iii),&l_val,              ((type_flagword_type) 0x04) ,eval_state_ssbits);
         temp = l_val.rrr;
         if ((iii==0)||(temp<cum)) cum = temp;}}
   l_val.rrr = cum;
   l_comp=              ((type_flagword_type) 0x04) ;
}
 
void do_op_ilismax()
{
   register int_type temp;
   register int_type iii,cum;
   for (cum=iii=0;iii<c_val.iii;++iii){
      if ((*(m_comp+iii))!=               ((type_flagword_type) 0x03) ) int_eval_err(       19 ,0,0 ,0) ;
      if ((*(m_opnd+iii))==0 ){
         temp = (*(m_val+iii)).iii;
         if ((iii==0)||(temp>cum)) cum = temp;}
      else{
         arraymax(*(m_opnd+iii),&l_val,               ((type_flagword_type) 0x03) ,eval_state_ssbits);
         temp = l_val.iii;
         if ((iii==0)||(temp>cum)) cum = temp;}}
   l_val.iii = cum;
   l_comp=               ((type_flagword_type) 0x03) ;
}
 
void do_op_rlismax()
{
   register real_type temp;
   register int_type iii;
   register real_type cum;
   for (iii=0,cum=0.000000000000 ;iii<c_val.iii;++iii){
      if ((*(m_comp+iii))!=              ((type_flagword_type) 0x04) ) real_eval_err(          41 ,0,0 ,0) ;
      if ((*(m_opnd+iii))==0 ){
         temp = (*(m_val+iii)).rrr;
         if ((iii==0)||(temp>cum)) cum = temp;}
      else{
         arraymax(*(m_opnd+iii),&l_val,              ((type_flagword_type) 0x04) ,eval_state_ssbits);
         temp = l_val.rrr;
         if ((iii==0)||(temp>cum)) cum = temp;}}
   l_val.rrr = cum;
   l_comp=              ((type_flagword_type) 0x04) ;
}
 
void do_op_ilissum()
{
   int_type iii,cum;
   for (cum=iii=0;iii<c_val.iii;++iii){
      if ((*(m_comp+iii))!=               ((type_flagword_type) 0x03) ) int_eval_err(       19 ,0,0 ,0) ;
      if ((*(m_opnd+iii))==0 ){
         cum += (*(m_val+iii)).iii;}
      else{
         arraysum(*(m_opnd+iii),&l_val,               ((type_flagword_type) 0x03) ,eval_state_ssbits);
         cum += l_val.iii;}}
   l_val.iii = cum;
   l_comp=               ((type_flagword_type) 0x03) ;
}
 
void do_op_rlissum()
{
   int_type iii;
   real_type cum;
   for (iii=0,cum=0.000000000000 ;iii<c_val.iii;++iii){
      if ((*(m_comp+iii))!=              ((type_flagword_type) 0x04) ) real_eval_err(          41 ,0,0 ,0) ;
      if ((*(m_opnd+iii))==0 ){
         cum += (*(m_val+iii)).rrr;}
      else{
         arraysum(*(m_opnd+iii),&l_val,              ((type_flagword_type) 0x04) ,eval_state_ssbits);
         cum += l_val.rrr;}}
   l_val.rrr = cum;
   l_comp=              ((type_flagword_type) 0x04) ;
}
 
void do_op_lisany()
{
   do_op_liscount();
   l_val.bbb = (Boolean) (l_val.iii>0);
   l_comp=              ((type_flagword_type) 0x02) ;
}
 
void do_op_lisall()
{
   int_type iii;
   for (iii=0;iii<c_val.iii;++iii){
      if ((*(m_opnd+iii))==0 ){
         l_val.bbb = (*(m_val+iii)).bbb;}
      else{
         l_opnd = (*(m_opnd+iii));
         l_comp = (*(m_comp+iii));
         ((*(&l_val)).pair.iiia = (*(m_val+iii)).pair.iiia, (*(&l_val)).pair.iiib = (*(m_val+iii)).pair.iiib) ;
         do_op_all();}
      if (!l_val.bbb) goto done_pt;}
done_pt:
   l_comp=              ((type_flagword_type) 0x02) ;
}
 






 
 




void qq_perform_eval_expr(unsigned char errmsg_ix)
 
{
   static char *errmsg[] = 
      {
         "BAD UNARY OP",
         "BAD BINARY OP",
         "BAD TERNARY OP",
         "BAD VARIABLE ARGUMENT LENGTH BUILT-IN OP"
      };
    
   is_int=(Boolean) (l_comp==               ((type_flagword_type) 0x03) );
   is_bool=(Boolean) (l_comp==              ((type_flagword_type) 0x02) );
   is_ssv=(Boolean) 0 ;
   if (l_opnd != 0 ) is_ssv=(flagtyp) ((((*l_opnd).flags) & (               ((type_flagword_type) 0x20)  )) != 0x0) ;
   if (rou_to_perform==((any_void_routine) 0 )){
     show_err_at_pos_on_both(          1 ,this.pos,errmsg[errmsg_ix],              ((type_flagword_type) 0x01) );}
   else rou_to_perform();
   l_x_comp &=         ((type_flagword_type) 0xf8) ;
   l_x_comp |= l_comp;
    
}
 

 
short qq_do_op_val(expression_type *expr,short oix_in)
{
# 1 "doopval.c" 1

   register short *pppsh0,*pppsh1 ;




   ssvar_value_type stateval;
   short oix;
   identifier_info_type *idinfo;
   register type_flagword_type reg_flags,reg_x_comp,reg_comp;
   register Memix ixtop;
    
   oix=oix_in;
   idinfo = *((*expr).operands+((Memix) oix));
   reg_flags = (*idinfo).flags;
   l_x_comp = reg_x_comp = reg_flags & ((type_flagword_type) 0x87) ;
   l_comp = reg_comp = reg_x_comp & ((type_flagword_type) 0x07) ;
   is_ssv=(flagtyp) (((reg_flags) & (               ((type_flagword_type) 0x20)  )) != 0x0) ;

   if ((trace_for_const)&&((flagtyp) (((reg_flags) & (((type_flagword_type) 0x78) )) != 0x0) )){
      set_arrow_sides( (Boolean) 1 ,(Boolean) 0 );
      (*expr).in_error= (Boolean) 1 ;
      show_err_at_pos_on_both(      134 ,this.pos,
                              (*idinfo).name,              ((type_flagword_type) 0x01) );
      reset_arrow_sides();}

   oix++;
   if ((flagtyp) (((reg_x_comp) & (               ((type_flagword_type) 0x80)  )) != 0x0) ){
      if (is_ssv){
         pppsh0 = (short *) (&l_val.ptr.sss); pppsh1 = (short *) (&(*idinfo).ptr.sss); *pppsh0++ = *pppsh1++; *pppsh0++ = *pppsh1++; *pppsh0++ = *pppsh1++; *pppsh0 = *pppsh1 ;}
      else l_val.ptr.vvv=(*idinfo).ptr.vvv;}
   else{
      switch (is_ssv){
      case  (Boolean) 1 :
         stateval=state_space_value(eval_state_ssbits,(*idinfo).ptr.sss);
         switch (reg_comp)
         {
            case               ((type_flagword_type) 0x02) :
                l_val.bbb=((stateval==0) ? (Boolean) 0  :  (Boolean) 1 );
                break;
            default:
                l_val.iii = (int_type) stateval;
         }
         break;
      case (Boolean) 0 :
         switch (reg_comp)
         {
           case                ((type_flagword_type) 0x03) :
               l_val.iii = *((*idinfo).ptr.iii);
               break;
           case               ((type_flagword_type) 0x02) :
               l_val.bbb = *((*idinfo).ptr.bbb);
               break;
           case               ((type_flagword_type) 0x01) :
               l_val.ccc = *((*idinfo).ptr.ccc);
               break;
           default:
               l_val.rrr = *((*idinfo).ptr.rrr);
         }
         break;
      }}
   l_x_comp=(l_x_comp &         ((type_flagword_type) 0xf8) ) | reg_comp;
   if (trace_evaluation) fprintf(bugfile,"push: %s, %04x, %s\n\n", (enk_aux_val((void *) ((idinfo==0 ) ? ((void *) (&l_val)) : (idinfo)), ((idinfo==0 ) ? (l_x_comp) : (            ((type_flagword_type) 0x07) )))) ,(int) l_x_comp, (*(idinfo)).name); ++eval_stack_top; if (eval_stack_top>=512 ) eval_stack_push_error(); ixtop=((Memix) eval_stack_top); *(eval_stack_opnd+ixtop) = idinfo; *(eval_stack_x_comp+ixtop) = l_x_comp; ((*(eval_stack_ele+ixtop)).pair.iiia = (*(&l_val)).pair.iiia, (*(eval_stack_ele+ixtop)).pair.iiib = (*(&l_val)).pair.iiib)   ;
    
   return(oix);
# 919 "prseval.c" 2

}
 
type_flagword_type qq_eval_expr
              (expression_type *expr,value_union_type *val,
               Boolean sub_index_expr,short ix0,short ix9,short oix0)
{
# 1 "evalexpr.c" 1
 









































   register operation_type op;
   register short ix,nix,nargs,iarg;
   short oix;
   register Memix ixtop;




    
   eval_stack_top=(-1) ;
   expr_line_number=(*expr).source_code_line_number;




   nix=((ix9<0) ? (*expr).n_postfix_ops : ix9);
   oix=oix0;
   ix=ix0;
   if (trace_evaluation)
      show_infix_or_postfix((*expr).postfix_ops,(*expr).n_postfix_ops,
                            "postfix");

   op = ((ix<nix) ? (*((*expr).postfix_ops+((Memix) ix))) :      ((operation_type) 0xfd) );

   if (ix>=nix)
      show_err_at_pos_on_both(          1 ,this.pos,
                              "Attempt to evaluate the empty expression.",
                                            ((type_flagword_type) 0x01) );




   while ((ix<nix)&&(!(sub_index_expr && (((((op)==  ((operation_type) 0xf8) ) || ((op)==  ((operation_type) 0xe8) )) && (eval_stack_top==0))  || (((op)== ((operation_type) 0xf9) ) && (eval_stack_top==1)) )) ))

   {

      if (trace_evaluation)
         fprintf(bugfile,"doit: ix=%d, op=%02x (%s)\n",
                          (int) ix,(int) op,operation_text+4*op);

      rou_to_perform = op_routines[op];
      if (((op) >=      ((operation_type) 0xfe) ) ) oix=qq_do_op_val(expr,oix) ;
      else if ((((op)>=  ((operation_type) 0x80)  )&&((op)<=    ((operation_type) 0x88)  )) )
          ;
      else if ((((op)>=     ((operation_type) 0x10)  )&&((op)<((operation_type) 0x4f) )) ){
          





         if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(&r_val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(&r_val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; r_opnd = *(eval_stack_opnd+ixtop); r_x_comp = *(eval_stack_x_comp+ixtop); --eval_stack_top ; if (trace_evaluation) fprintf(bugfile,RightsidePopFmt, (enk_aux_val((void *) ((r_opnd==0 ) ? ((void *) (&r_val)) : (r_opnd)), ((r_opnd==0 ) ? (r_x_comp) : (            ((type_flagword_type) 0x07) )))) ,(int) r_x_comp, ((r_opnd==0 ) ? "" : ", "), ((r_opnd==0 ) ? "" : (*(r_opnd)).name)) ;
         if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(&l_val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(&l_val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; l_opnd = *(eval_stack_opnd+ixtop); l_x_comp = *(eval_stack_x_comp+ixtop); --eval_stack_top ; if (trace_evaluation) fprintf(bugfile,LeftsidePopFmt, (enk_aux_val((void *) ((l_opnd==0 ) ? ((void *) (&l_val)) : (l_opnd)), ((l_opnd==0 ) ? (l_x_comp) : (            ((type_flagword_type) 0x07) )))) ,(int) l_x_comp, ((l_opnd==0 ) ? "" : ", "), ((l_opnd==0 ) ? "" : (*(l_opnd)).name)) ;
         r_comp = r_x_comp & ((type_flagword_type) 0x07) ;
         l_comp = l_x_comp & ((type_flagword_type) 0x07) ;
         qq_perform_eval_expr(((unsigned char) 1) );
         if (trace_evaluation) fprintf(bugfile,"push: %s, %04x\n\n", (enk_aux_val((void *) ((0 ==0 ) ? ((void *) (&l_val)) : (0 )), ((0 ==0 ) ? (l_x_comp) : (            ((type_flagword_type) 0x07) )))) ,(int) l_x_comp); ++eval_stack_top; if (eval_stack_top>=512 ) eval_stack_push_error(); ixtop=((Memix) eval_stack_top); *(eval_stack_opnd+ixtop) = 0 ; *(eval_stack_x_comp+ixtop) = l_x_comp; ((*(eval_stack_ele+ixtop)).pair.iiia = (*(&l_val)).pair.iiia, (*(eval_stack_ele+ixtop)).pair.iiib = (*(&l_val)).pair.iiib)   ;}
      else if ((((op)>=    ((operation_type) 0x58) )&&((op)<    ((operation_type) 0xa0)  )) ){
          






         if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(&l_val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(&l_val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; l_opnd = *(eval_stack_opnd+ixtop); l_x_comp = *(eval_stack_x_comp+ixtop); --eval_stack_top ; if (trace_evaluation) fprintf(bugfile,LeftsidePopFmt, (enk_aux_val((void *) ((l_opnd==0 ) ? ((void *) (&l_val)) : (l_opnd)), ((l_opnd==0 ) ? (l_x_comp) : (            ((type_flagword_type) 0x07) )))) ,(int) l_x_comp, ((l_opnd==0 ) ? "" : ", "), ((l_opnd==0 ) ? "" : (*(l_opnd)).name)) ;
         l_comp = l_x_comp & ((type_flagword_type) 0x07) ;
         qq_perform_eval_expr(((unsigned char) 0) );



         if (trace_evaluation) fprintf(bugfile,"push: %s, %04x\n\n", (enk_aux_val((void *) ((0 ==0 ) ? ((void *) (&l_val)) : (0 )), ((0 ==0 ) ? (l_x_comp) : (            ((type_flagword_type) 0x07) )))) ,(int) l_x_comp); ++eval_stack_top; if (eval_stack_top>=512 ) eval_stack_push_error(); ixtop=((Memix) eval_stack_top); *(eval_stack_opnd+ixtop) = 0 ; *(eval_stack_x_comp+ixtop) = l_x_comp; ((*(eval_stack_ele+ixtop)).pair.iiia = (*(&l_val)).pair.iiia, (*(eval_stack_ele+ixtop)).pair.iiib = (*(&l_val)).pair.iiib)   ;}






      else if (((op)== ((operation_type) 0xf9) ) ){



         if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(&r_val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(&r_val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; r_opnd = *(eval_stack_opnd+ixtop); r_x_comp = *(eval_stack_x_comp+ixtop); --eval_stack_top ; if (trace_evaluation) fprintf(bugfile,RightsidePopFmt, (enk_aux_val((void *) ((r_opnd==0 ) ? ((void *) (&r_val)) : (r_opnd)), ((r_opnd==0 ) ? (r_x_comp) : (            ((type_flagword_type) 0x07) )))) ,(int) r_x_comp, ((r_opnd==0 ) ? "" : ", "), ((r_opnd==0 ) ? "" : (*(r_opnd)).name)) ;
         if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(&c_val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(&c_val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; c_opnd = *(eval_stack_opnd+ixtop); c_x_comp = *(eval_stack_x_comp+ixtop); --eval_stack_top ; if (trace_evaluation) fprintf(bugfile,CenterPopFmt, (enk_aux_val((void *) ((c_opnd==0 ) ? ((void *) (&c_val)) : (c_opnd)), ((c_opnd==0 ) ? (c_x_comp) : (            ((type_flagword_type) 0x07) )))) ,(int) c_x_comp, ((c_opnd==0 ) ? "" : ", "), ((c_opnd==0 ) ? "" : (*(c_opnd)).name)) ;
         if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(&l_val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(&l_val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; l_opnd = *(eval_stack_opnd+ixtop); l_x_comp = *(eval_stack_x_comp+ixtop); --eval_stack_top ; if (trace_evaluation) fprintf(bugfile,LeftsidePopFmt, (enk_aux_val((void *) ((l_opnd==0 ) ? ((void *) (&l_val)) : (l_opnd)), ((l_opnd==0 ) ? (l_x_comp) : (            ((type_flagword_type) 0x07) )))) ,(int) l_x_comp, ((l_opnd==0 ) ? "" : ", "), ((l_opnd==0 ) ? "" : (*(l_opnd)).name)) ;
         r_comp = r_x_comp & ((type_flagword_type) 0x07) ;
         c_comp = c_x_comp & ((type_flagword_type) 0x07) ;
         l_comp = l_x_comp & ((type_flagword_type) 0x07) ;
         qq_perform_eval_expr(((unsigned char) 2) );
         if (trace_evaluation) fprintf(bugfile,"push: %s, %04x\n\n", (enk_aux_val((void *) ((0 ==0 ) ? ((void *) (&l_val)) : (0 )), ((0 ==0 ) ? (l_x_comp) : (            ((type_flagword_type) 0x07) )))) ,(int) l_x_comp); ++eval_stack_top; if (eval_stack_top>=512 ) eval_stack_push_error(); ixtop=((Memix) eval_stack_top); *(eval_stack_opnd+ixtop) = 0 ; *(eval_stack_x_comp+ixtop) = l_x_comp; ((*(eval_stack_ele+ixtop)).pair.iiia = (*(&l_val)).pair.iiia, (*(eval_stack_ele+ixtop)).pair.iiib = (*(&l_val)).pair.iiib)   ;}
      else if ((((op)>=((operation_type) 0xd0)  )&&((op)<  ((operation_type) 0xe8) )) ){
          
         if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(&c_val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(&c_val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; c_opnd = *(eval_stack_opnd+ixtop); c_x_comp = *(eval_stack_x_comp+ixtop); --eval_stack_top ; if (trace_evaluation) fprintf(bugfile,CenterPopFmt, (enk_aux_val((void *) ((c_opnd==0 ) ? ((void *) (&c_val)) : (c_opnd)), ((c_opnd==0 ) ? (c_x_comp) : (            ((type_flagword_type) 0x07) )))) ,(int) c_x_comp, ((c_opnd==0 ) ? "" : ", "), ((c_opnd==0 ) ? "" : (*(c_opnd)).name)) ;
         c_comp = c_x_comp & ((type_flagword_type) 0x07) ;
         nargs=c_val.iii;
         l_x_comp=               ((type_flagword_type) 0x03) ; l_val.iii=0;
         if (nargs>32 ){
            show_err_at_pos_on_both
                (   95 ,this.pos,&c_val.iii,               ((type_flagword_type) 0x03) );}
         else if (nargs<1){
            show_err_at_pos_on_both
                (    94 ,this.pos,&c_val.iii,               ((type_flagword_type) 0x03) );}
         else{
            for (iarg=nargs-1;iarg>=0;--iarg){
               if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(m_val+iarg)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(m_val+iarg)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; *(m_opnd+iarg) = *(eval_stack_opnd+ixtop); *(m_x_comp+iarg) = *(eval_stack_x_comp+ixtop); --eval_stack_top ; if (trace_evaluation) fprintf(bugfile,                              MultiPopFmt, (enk_aux_val((void *) ((*(m_opnd+iarg)==0 ) ? ((void *) (m_val+iarg)) : (*(m_opnd+iarg))), ((*(m_opnd+iarg)==0 ) ? (*(m_x_comp+iarg)) : (            ((type_flagword_type) 0x07) )))) ,(int) *(m_x_comp+iarg), ((*(m_opnd+iarg)==0 ) ? "" : ", "), ((*(m_opnd+iarg)==0 ) ? "" : (*(*(m_opnd+iarg))).name)) ;

               *(m_comp+iarg) = (*(m_x_comp+iarg)) & ((type_flagword_type) 0x07) ;}
            l_x_comp=(*m_x_comp);
            qq_perform_eval_expr(((unsigned char) 3) );}
         if (trace_evaluation) fprintf(bugfile,"push: %s, %04x\n\n", (enk_aux_val((void *) ((0 ==0 ) ? ((void *) (&l_val)) : (0 )), ((0 ==0 ) ? (l_x_comp) : (            ((type_flagword_type) 0x07) )))) ,(int) l_x_comp); ++eval_stack_top; if (eval_stack_top>=512 ) eval_stack_push_error(); ixtop=((Memix) eval_stack_top); *(eval_stack_opnd+ixtop) = 0 ; *(eval_stack_x_comp+ixtop) = l_x_comp; ((*(eval_stack_ele+ixtop)).pair.iiia = (*(&l_val)).pair.iiia, (*(eval_stack_ele+ixtop)).pair.iiib = (*(&l_val)).pair.iiib)   ;}
      else{
          





         if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(&r_val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(&r_val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; r_opnd = *(eval_stack_opnd+ixtop); r_x_comp = *(eval_stack_x_comp+ixtop); --eval_stack_top ; if (trace_evaluation) fprintf(bugfile,RightsidePopFmt, (enk_aux_val((void *) ((r_opnd==0 ) ? ((void *) (&r_val)) : (r_opnd)), ((r_opnd==0 ) ? (r_x_comp) : (            ((type_flagword_type) 0x07) )))) ,(int) r_x_comp, ((r_opnd==0 ) ? "" : ", "), ((r_opnd==0 ) ? "" : (*(r_opnd)).name)) ;
         if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(&l_val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(&l_val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; l_opnd = *(eval_stack_opnd+ixtop); l_x_comp = *(eval_stack_x_comp+ixtop); --eval_stack_top ; if (trace_evaluation) fprintf(bugfile,LeftsidePopFmt, (enk_aux_val((void *) ((l_opnd==0 ) ? ((void *) (&l_val)) : (l_opnd)), ((l_opnd==0 ) ? (l_x_comp) : (            ((type_flagword_type) 0x07) )))) ,(int) l_x_comp, ((l_opnd==0 ) ? "" : ", "), ((l_opnd==0 ) ? "" : (*(l_opnd)).name)) ;
         r_comp = r_x_comp & ((type_flagword_type) 0x07) ;
         l_comp = l_x_comp & ((type_flagword_type) 0x07) ;
         qq_perform_eval_expr(((unsigned char) 1) );
         if (trace_evaluation) fprintf(bugfile,"push: %s, %04x\n\n", (enk_aux_val((void *) ((0 ==0 ) ? ((void *) (&l_val)) : (0 )), ((0 ==0 ) ? (l_x_comp) : (            ((type_flagword_type) 0x07) )))) ,(int) l_x_comp); ++eval_stack_top; if (eval_stack_top>=512 ) eval_stack_push_error(); ixtop=((Memix) eval_stack_top); *(eval_stack_opnd+ixtop) = 0 ; *(eval_stack_x_comp+ixtop) = l_x_comp; ((*(eval_stack_ele+ixtop)).pair.iiia = (*(&l_val)).pair.iiia, (*(eval_stack_ele+ixtop)).pair.iiib = (*(&l_val)).pair.iiib)   ;}
      ++ix;
      op = ((ix<nix) ? (*((*expr).postfix_ops+((Memix) ix))) :      ((operation_type) 0xfd) );}
   ixtop=(Memix) eval_stack_top;
   ((*(val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ;
    
   ee_ix_out=ix;
   ee_oix_out=oix;

   if (sub_index_expr && (op== ((operation_type) 0xf9) )){
      ((*(&ee_aux_val)).pair.iiia = (*(val)).pair.iiia, (*(&ee_aux_val)).pair.iiib = (*(val)).pair.iiib) ;
      --ixtop;
      ((*(val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ;}

   return((type_flagword_type) 
          (*(eval_stack_x_comp+ixtop)) & ((type_flagword_type) 0x07) );

# 926 "prseval.c" 2

}
 
type_flagword_type evaluate_expression
                      (expression_type *expr,value_union_type *val,
                       Boolean sub_index_expr,short ix0,short ix9,short oix0,
                       Boolean for_const)
{
   type_flagword_type ans;
    
   trace_evaluation = (flagtyp) (((debug_flagword) & ( ((debug_flag_type) 0x80) )) != 0x0) ;
   trace_for_const = for_const;
   if (trace_evaluation){
      fprintf(bugfile,"\nevaluate_expression(,,%c,%d,%d,%d,%c) :: \n",
              (sub_index_expr ? 'T' : 'F'),(int) ix0,(int) ix9,(int) oix0,
              (trace_for_const ? 'T' : 'F'));
      show_infix_or_postfix((*expr).postfix_ops,(*expr).n_postfix_ops,
                            "postfix");}
   if ((*expr).in_error){
      show_err_at_pos_on_both(          1 ,this.pos,
                              "Attempt to evaluate an expression in error.",
                                            ((type_flagword_type) 0x01) );
      l_comp=((*expr).rtntype & ((type_flagword_type) 0x07) );
      standard_value(val,l_comp);
      return((*expr).rtntype);}
   ans = qq_eval_expr(expr,val,sub_index_expr,ix0,ix9,oix0);
    
   return(ans);
}
 










 
short qq_fast_do_op_val(expression_type *expr,short oix_in)
{
# 1 "doopval.c" 1

   register short *pppsh0,*pppsh1 ;




   ssvar_value_type stateval;
   short oix;
   identifier_info_type *idinfo;
   register type_flagword_type reg_flags,reg_x_comp,reg_comp;
   register Memix ixtop;
    
   oix=oix_in;
   idinfo = *((*expr).operands+((Memix) oix));
   reg_flags = (*idinfo).flags;
   l_x_comp = reg_x_comp = reg_flags & ((type_flagword_type) 0x87) ;
   l_comp = reg_comp = reg_x_comp & ((type_flagword_type) 0x07) ;
   is_ssv=(flagtyp) (((reg_flags) & (               ((type_flagword_type) 0x20)  )) != 0x0) ;








   oix++;
   if ((flagtyp) (((reg_x_comp) & (               ((type_flagword_type) 0x80)  )) != 0x0) ){
      if (is_ssv){
         pppsh0 = (short *) (&l_val.ptr.sss); pppsh1 = (short *) (&(*idinfo).ptr.sss); *pppsh0++ = *pppsh1++; *pppsh0++ = *pppsh1++; *pppsh0++ = *pppsh1++; *pppsh0 = *pppsh1 ;}
      else l_val.ptr.vvv=(*idinfo).ptr.vvv;}
   else{
      switch (is_ssv){
      case  (Boolean) 1 :
         stateval=state_space_value(eval_state_ssbits,(*idinfo).ptr.sss);
         switch (reg_comp)
         {
            case               ((type_flagword_type) 0x02) :
                l_val.bbb=((stateval==0) ? (Boolean) 0  :  (Boolean) 1 );
                break;
            default:
                l_val.iii = (int_type) stateval;
         }
         break;
      case (Boolean) 0 :
         switch (reg_comp)
         {
           case                ((type_flagword_type) 0x03) :
               l_val.iii = *((*idinfo).ptr.iii);
               break;
           case               ((type_flagword_type) 0x02) :
               l_val.bbb = *((*idinfo).ptr.bbb);
               break;
           case               ((type_flagword_type) 0x01) :
               l_val.ccc = *((*idinfo).ptr.ccc);
               break;
           default:
               l_val.rrr = *((*idinfo).ptr.rrr);
         }
         break;
      }}
   l_x_comp=(l_x_comp &         ((type_flagword_type) 0xf8) ) | reg_comp;
   ++eval_stack_top; if (eval_stack_top>=512 ) eval_stack_push_error(); ixtop=((Memix) eval_stack_top); *(eval_stack_opnd+ixtop) = idinfo; *(eval_stack_x_comp+ixtop) = l_x_comp; ((*(eval_stack_ele+ixtop)).pair.iiia = (*(&l_val)).pair.iiia, (*(eval_stack_ele+ixtop)).pair.iiib = (*(&l_val)).pair.iiib)   ;
    
   return(oix);
# 969 "prseval.c" 2

}
 
type_flagword_type qq_fast_eval_expr
              (expression_type *expr,value_union_type *val)
{
# 1 "evalexpr.c" 1
 









































   register operation_type op;
   register short ix,nix,nargs,iarg;
   short oix;
   register Memix ixtop;




    
   eval_stack_top=(-1) ;
   expr_line_number=(*expr).source_code_line_number;

   nix=(*expr).n_postfix_ops;
   ix=oix=0;








   op = ((ix<nix) ? (*((*expr).postfix_ops+((Memix) ix))) :      ((operation_type) 0xfd) );







   while (ix<nix)



   {





      rou_to_perform = op_routines[op];
      if (((op) >=      ((operation_type) 0xfe) ) ) oix=qq_fast_do_op_val(expr,oix) ;
      else if ((((op)>=  ((operation_type) 0x80)  )&&((op)<=    ((operation_type) 0x88)  )) )
          ;
      else if ((((op)>=     ((operation_type) 0x10)  )&&((op)<((operation_type) 0x4f) )) ){
          





         if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(&r_val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(&r_val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; r_opnd = *(eval_stack_opnd+ixtop); r_x_comp = *(eval_stack_x_comp+ixtop); --eval_stack_top  ;
         if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(&l_val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(&l_val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; l_opnd = *(eval_stack_opnd+ixtop); l_x_comp = *(eval_stack_x_comp+ixtop); --eval_stack_top  ;
         r_comp = r_x_comp & ((type_flagword_type) 0x07) ;
         l_comp = l_x_comp & ((type_flagword_type) 0x07) ;
         qq_perform_eval_expr(((unsigned char) 1) );
         ++eval_stack_top; if (eval_stack_top>=512 ) eval_stack_push_error(); ixtop=((Memix) eval_stack_top); *(eval_stack_opnd+ixtop) = 0 ; *(eval_stack_x_comp+ixtop) = l_x_comp; ((*(eval_stack_ele+ixtop)).pair.iiia = (*(&l_val)).pair.iiia, (*(eval_stack_ele+ixtop)).pair.iiib = (*(&l_val)).pair.iiib)   ;}
      else if ((((op)>=    ((operation_type) 0x58) )&&((op)<    ((operation_type) 0xa0)  )) ){
          






         if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(&l_val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(&l_val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; l_opnd = *(eval_stack_opnd+ixtop); l_x_comp = *(eval_stack_x_comp+ixtop); --eval_stack_top  ;
         l_comp = l_x_comp & ((type_flagword_type) 0x07) ;
         qq_perform_eval_expr(((unsigned char) 0) );



         ++eval_stack_top; if (eval_stack_top>=512 ) eval_stack_push_error(); ixtop=((Memix) eval_stack_top); *(eval_stack_opnd+ixtop) = 0 ; *(eval_stack_x_comp+ixtop) = l_x_comp; ((*(eval_stack_ele+ixtop)).pair.iiia = (*(&l_val)).pair.iiia, (*(eval_stack_ele+ixtop)).pair.iiib = (*(&l_val)).pair.iiib)   ;}






      else if (((op)== ((operation_type) 0xf9) ) ){



         if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(&r_val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(&r_val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; r_opnd = *(eval_stack_opnd+ixtop); r_x_comp = *(eval_stack_x_comp+ixtop); --eval_stack_top  ;
         if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(&c_val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(&c_val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; c_opnd = *(eval_stack_opnd+ixtop); c_x_comp = *(eval_stack_x_comp+ixtop); --eval_stack_top  ;
         if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(&l_val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(&l_val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; l_opnd = *(eval_stack_opnd+ixtop); l_x_comp = *(eval_stack_x_comp+ixtop); --eval_stack_top  ;
         r_comp = r_x_comp & ((type_flagword_type) 0x07) ;
         c_comp = c_x_comp & ((type_flagword_type) 0x07) ;
         l_comp = l_x_comp & ((type_flagword_type) 0x07) ;
         qq_perform_eval_expr(((unsigned char) 2) );
         ++eval_stack_top; if (eval_stack_top>=512 ) eval_stack_push_error(); ixtop=((Memix) eval_stack_top); *(eval_stack_opnd+ixtop) = 0 ; *(eval_stack_x_comp+ixtop) = l_x_comp; ((*(eval_stack_ele+ixtop)).pair.iiia = (*(&l_val)).pair.iiia, (*(eval_stack_ele+ixtop)).pair.iiib = (*(&l_val)).pair.iiib)   ;}
      else if ((((op)>=((operation_type) 0xd0)  )&&((op)<  ((operation_type) 0xe8) )) ){
          
         if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(&c_val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(&c_val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; c_opnd = *(eval_stack_opnd+ixtop); c_x_comp = *(eval_stack_x_comp+ixtop); --eval_stack_top  ;
         c_comp = c_x_comp & ((type_flagword_type) 0x07) ;
         nargs=c_val.iii;
         l_x_comp=               ((type_flagword_type) 0x03) ; l_val.iii=0;
         if (nargs>32 ){
            show_err_at_pos_on_both
                (   95 ,this.pos,&c_val.iii,               ((type_flagword_type) 0x03) );}
         else if (nargs<1){
            show_err_at_pos_on_both
                (    94 ,this.pos,&c_val.iii,               ((type_flagword_type) 0x03) );}
         else{
            for (iarg=nargs-1;iarg>=0;--iarg){
               if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(m_val+iarg)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(m_val+iarg)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; *(m_opnd+iarg) = *(eval_stack_opnd+ixtop); *(m_x_comp+iarg) = *(eval_stack_x_comp+ixtop); --eval_stack_top  ;

               *(m_comp+iarg) = (*(m_x_comp+iarg)) & ((type_flagword_type) 0x07) ;}
            l_x_comp=(*m_x_comp);
            qq_perform_eval_expr(((unsigned char) 3) );}
         ++eval_stack_top; if (eval_stack_top>=512 ) eval_stack_push_error(); ixtop=((Memix) eval_stack_top); *(eval_stack_opnd+ixtop) = 0 ; *(eval_stack_x_comp+ixtop) = l_x_comp; ((*(eval_stack_ele+ixtop)).pair.iiia = (*(&l_val)).pair.iiia, (*(eval_stack_ele+ixtop)).pair.iiib = (*(&l_val)).pair.iiib)   ;}
      else{
          





         if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(&r_val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(&r_val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; r_opnd = *(eval_stack_opnd+ixtop); r_x_comp = *(eval_stack_x_comp+ixtop); --eval_stack_top  ;
         if (eval_stack_top<0) eval_stack_pop_error(); ixtop=(Memix) eval_stack_top; ((*(&l_val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(&l_val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ; l_opnd = *(eval_stack_opnd+ixtop); l_x_comp = *(eval_stack_x_comp+ixtop); --eval_stack_top  ;
         r_comp = r_x_comp & ((type_flagword_type) 0x07) ;
         l_comp = l_x_comp & ((type_flagword_type) 0x07) ;
         qq_perform_eval_expr(((unsigned char) 1) );
         ++eval_stack_top; if (eval_stack_top>=512 ) eval_stack_push_error(); ixtop=((Memix) eval_stack_top); *(eval_stack_opnd+ixtop) = 0 ; *(eval_stack_x_comp+ixtop) = l_x_comp; ((*(eval_stack_ele+ixtop)).pair.iiia = (*(&l_val)).pair.iiia, (*(eval_stack_ele+ixtop)).pair.iiib = (*(&l_val)).pair.iiib)   ;}
      ++ix;
      op = ((ix<nix) ? (*((*expr).postfix_ops+((Memix) ix))) :      ((operation_type) 0xfd) );}
   ixtop=(Memix) eval_stack_top;
   ((*(val)).pair.iiia = (*(eval_stack_ele+ixtop)).pair.iiia, (*(val)).pair.iiib = (*(eval_stack_ele+ixtop)).pair.iiib) ;
    
   ee_ix_out=ix;
   ee_oix_out=oix;






   return((type_flagword_type) 
          (*(eval_stack_x_comp+ixtop)) & ((type_flagword_type) 0x07) );

# 975 "prseval.c" 2

}
 
type_flagword_type fast_evaluate_full_expression
                      (expression_type *expr,value_union_type *val)
{
   type_flagword_type ans;
    
   trace_evaluation = (Boolean) 0 ;
   trace_for_const = (Boolean) 0 ;
   ans = qq_fast_eval_expr(expr,val);
    
   return(ans);
}
 
