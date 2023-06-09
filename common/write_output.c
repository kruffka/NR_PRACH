#include <stdio.h>
#include <stdlib.h>
#include "tools_defs.h"

int write_file_matlab(const char *fname,
		              const char *vname,
					  void *data,
					  int length,
					  int dec,
					  unsigned int format,
            int multiVec)
{
  FILE *fp=NULL;
  int i;

  // AssertFatal((format&~MATLAB_RAW) <16,"");

  if (data == NULL)
    return -1;

  //printf("Writing %d elements of type %d to %s\n",length,format,fname);

  if (format == 10 || format ==11 || format == 12 || format == 14 || multiVec) {
    fp = fopen(fname,"a+");
  } else if (format != 10 && format !=11  && format != 12 && format != 14) {
    fp = fopen(fname,"w+");
  }

  if (fp== NULL) {
    printf("[OPENAIR][FILE OUTPUT] Cannot open file %s\n",fname);
    return(-1);
  }

  // if ( (format&MATLAB_RAW) == MATLAB_RAW ) {
  //   int sz[16]={sizeof(short), 2*sizeof(short),
	// 	sizeof(int), 2*sizeof(int),
	// 	sizeof(char), 2*sizeof(char),
	// 	sizeof(long long), 
	// 	sizeof(double), 2*sizeof(double),
	// 	sizeof(unsigned char),
	// 	sizeof(short),
	// 	sizeof(short),
	// 	sizeof(short),
	// 	sizeof(short),
	// 	sizeof(short),
	// 	sizeof(short)
  //   };
  //   int eltSz= sz[format&~MATLAB_RAW];
  //   if (dec==1) 
  //     fwrite(data, eltSz, length, fp);
  //   else 
  //     for (i=0; i<length; i+=dec)
	// fwrite(data+i*eltSz, eltSz, 1, fp);
    
  //   fclose(fp);
  //   return(0);	
  // }

  if ((format != 10 && format !=11  && format != 12 && format != 13 && format != 14) || multiVec)
    fprintf(fp,"%s = [",vname);

  switch (format) {
    case 0:   // real 16-bit
      for (i=0; i<length; i+=dec) {
        fprintf(fp,"%d\n",((short *)data)[i]);
      }

      break;

    case 1:  // complex 16-bit
    case 14:
    case 15:
      for (i=0; i<length<<1; i+=(2*dec)) {
        fprintf(fp,"%d + j*(%d)\n",((short *)data)[i],((short *)data)[i+1]);
      }

      break;

    case 2:  // real 32-bit
      for (i=0; i<length; i+=dec) {
        fprintf(fp,"%d\n",((int *)data)[i]);
      }

      break;

    case 3: // complex 32-bit
      for (i=0; i<length<<1; i+=(2*dec)) {
        fprintf(fp,"%d + j*(%d)\n",((int *)data)[i],((int *)data)[i+1]);
      }

      break;

    case 4: // real 8-bit
      for (i=0; i<length; i+=dec) {
        fprintf(fp,"%d\n",((char *)data)[i]);
      }

      break;

    case 5: // complex 8-bit
      for (i=0; i<length<<1; i+=(2*dec)) {
        fprintf(fp,"%d + j*(%d)\n",((char *)data)[i],((char *)data)[i+1]);
      }

      break;

    case 6:  // real 64-bit
      for (i=0; i<length; i+=dec) {
        fprintf(fp,"%lld\n",((long long *)data)[i]);
      }

      break;

    case 7: // real double
      for (i=0; i<length; i+=dec) {
        fprintf(fp,"%g\n",((double *)data)[i]);
      }

      break;

    case 8: // complex double
      for (i=0; i<length<<1; i+=2*dec) {
        fprintf(fp,"%g + j*(%g)\n",((double *)data)[i], ((double *)data)[i+1]);
      }

      break;

    case 9: // real unsigned 8-bit
      for (i=0; i<length; i+=dec) {
        fprintf(fp,"%d\n",((unsigned char *)data)[i]);
      }

      break;

    case 10 : // case eren 16 bit complex :
      for (i=0; i<length<<1; i+=(2*dec)) {
        if((i < 2*(length-1)) && (i > 0))
          fprintf(fp,"%d + j*(%d),",((short *)data)[i],((short *)data)[i+1]);
        else if (i == 2*(length-1))
          fprintf(fp,"%d + j*(%d);",((short *)data)[i],((short *)data)[i+1]);
        else if (i == 0)
          fprintf(fp,"\n%d + j*(%d),",((short *)data)[i],((short *)data)[i+1]);
      }

      break;

    case 11 : //case eren 16 bit real for channel magnitudes:
      for (i=0; i<length; i+=dec) {
        if((i <(length-1))&& (i > 0))
          fprintf(fp,"%d,",((short *)data)[i]);
        else if (i == (length-1))
          fprintf(fp,"%d;",((short *)data)[i]);
        else if (i == 0)
          fprintf(fp,"\n%d,",((short *)data)[i]);
      }

      printf("\n eren: length :%d",length);
      break;

    case 12 : // case eren for log2_maxh real unsigned 8 bit
      fprintf(fp,"%d \n",((unsigned char *)&data)[0]);
      break;

    case 13:
      // complex float
      for (i=0; i<length<<1; i+=(2*dec)) {
        fprintf(fp,"%f + j*(%f)\n",((float *)data)[i],((float *)data)[i+1]);
      }

      break;
  default:
    AssertFatal(1 == 0, "unknown dump format: %u\n", format);
  }

  if ((format != 10 && format !=11 && format !=12 && format != 13 && format != 15) || multiVec) {
    fprintf(fp,"];\n");
    fclose(fp);
    return(0);
  } else if (format == 10 || format ==11 || format == 12 || format == 13 || format == 15) {
    fclose(fp);
    return(0);
  }

  return 0;
}