/* 4 bit sprite format - Public Domain */
#include <stdio.h>

/* STB image library from https://github.com/nothings/stb */
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
typedef enum { false, true } bool;
unsigned short pallet[16] ={
    0x0,
    0x194a,
    0x792a,
    0x42a,
    0xaa86,
    0x5aa9,
    0xc618,
    0xff9d,
    0xf809,
    0xfd00,
    0xffe4,
    0x72a,
    0x2d7f,
    0x83b3,
    0xfbb5,
    0xfe75
};


void palletRead(FILE *Poutput,unsigned char *data){
   
    
    if (Poutput != NULL) {
     fprintf(Poutput,"//pallet\nconst unsigned short pallet[]={\n");
     for (int i=0; i<16; i++) {
         pallet[i] = ((data[(i)*3] >> 3) << 11) | (( data[(i)*3+1] >> 2) << 5) | ( data[(i)*3+2] >> 3);
         fprintf(Poutput,"0x%x,\n",pallet[i]);
     }
        
        fprintf(Poutput,"};\n\n");
    }

}
int check(unsigned char r,unsigned char g,unsigned char b,int x, int y){
    unsigned short m = ((r >> 3) << 11) | (( g >> 2) << 5) | ( b >> 3);
   
    for (int i=0; i<16; i++) {
        if (pallet[i]==m) {
            
            return i;
        }
    }
    printf("pixel at x%i,y%i not in pallet color:%i,%i,%i\n",x,y,r,g,b);
    return 0;
}
int main(int argc, const char * argv[]) {
    //bool GenPallet = true;
    char *outFile = NULL;
    if (argc < 2) {
         printf("no image suplied\n");
    }
    else{
        int px,py,pn;
                int sx,sy,n;
        unsigned char *data = stbi_load(argv[1], &sx, &sy, &n, 3);
        int last = 0;
        FILE *output = NULL;
        if (outFile == NULL) output = fopen("output.4BS", "w+");
        else output = fopen(outFile, "w+");
       
        if (output != NULL) {
            if (argc > 2) {
                unsigned char *pdata = stbi_load(argv[2], &px, &py, &pn, 3);
                palletRead(output,pdata);
            }else{
                fprintf(output, "//pico8 pallet\nconst unsigned short pallet[]={0x0000,\n0x194a,\n0x792a,\n0x042a,\n0xaa86,\n0x5aa9,\n0xc618,\n0xff9d,\n0xf809,\n0xfd00,\n0xffe4,\n0x072a,\n0x2d7f,\n0x83b3,\n0xfbb5,\n0xfe75};\n\n");
            }
            fprintf(output,"//sprite\nconst uint8_t sprite[]={%i, %i, /*width, height*/\n",sx,sy);
            for (int y=0; y<sy; y++) {
                for (int x=0; x<sx; x++) {
                    if(x%2){
                    fprintf(output,"0x%x%x,",last, check(data[((y*sx)+x)*3] , data[((y*sx)+x)*3 + 1], data[((y*sx)+x)*3 +2],x,y) );
                    }else{
                        last = check(data[((y*sx)+x)*3] , data[((y*sx)+x)*3 + 1], data[((y*sx)+x)*3 +2],x,y);
                    }
                }
                fprintf(output,"\n");
            }
            fprintf(output,"};");
            
        }
        stbi_image_free(data);
        fclose(output);
    }
    return 0;
}
