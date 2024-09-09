#ifndef __IDX_H_ENTRY_H__
#define __IDX_H_ENTRY_H__

typedef struct {
   unsigned char header[4];
   int length;
   unsigned char *data;
} IDX1_DATA ;

typedef struct {
   unsigned char header[4];
   int nimages;
   int nrows;
   int ncols;
   int length;
   unsigned char *data;
} IDX3_DATA ;

int fread_idx1_file( const char *slabelfname, IDX1_DATA *idxdata);
int fread_idx3_file( const char *slabelfname, IDX3_DATA *idxdata);

#endif  /* __IDX_H_ENTRY_H__ */
